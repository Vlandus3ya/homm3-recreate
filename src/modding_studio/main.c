#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#define _WIN32_WINNT 0x0601

#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <commctrl.h>
#include <wchar.h>
#include <stdio.h>

#define APP_TITLE L"Heroes of Might and Magic III: Recreate - Modding Studio"
#define ID_OPEN_MAP       1001
#define ID_OPEN_CAMPAIGN  1002
#define ID_START_EDITOR   1003
#define ID_BATCH_CONVERT  1004

static wchar_t g_editorPath[MAX_PATH];
static HWND g_statusLabel;
static HWND g_mainWindow;
static HFONT g_titleFont, g_bodyFont, g_headingFont, g_smallFont;
static HBRUSH g_backgroundBrush;
static const COLORREF COLOR_BG = RGB(242, 245, 249);
static const COLORREF COLOR_HEADER = RGB(25, 43, 68);
static const COLORREF COLOR_MUTED = RGB(100, 113, 130);

static void SetStatus(const wchar_t *message)
{
    if (g_statusLabel)
        SetWindowTextW(g_statusLabel, message);
}

static BOOL FileExists(const wchar_t *path)
{
    DWORD attributes = GetFileAttributesW(path);
    return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

static void GetApplicationDirectory(wchar_t *buffer, DWORD capacity)
{
    DWORD length = GetModuleFileNameW(NULL, buffer, capacity);
    if (length == 0 || length >= capacity)
    {
        if (capacity > 0)
            buffer[0] = L'\0';
        return;
    }

    wchar_t *lastSlash = wcsrchr(buffer, L'\\');
    if (!lastSlash)
        lastSlash = wcsrchr(buffer, L'/');
    if (lastSlash)
        *lastSlash = L'\0';
    else
        buffer[0] = L'\0';
}

static BOOL JoinPath(wchar_t *out, size_t outCount, const wchar_t *base, const wchar_t *relative)
{
    int written = swprintf(out, outCount, L"%ls\\%ls", base, relative);
    return written >= 0 && (size_t)written < outCount;
}

static BOOL LocateEditor(void)
{
    wchar_t appDirectory[MAX_PATH];
    wchar_t candidate[MAX_PATH];
    GetApplicationDirectory(appDirectory, MAX_PATH);
    if (appDirectory[0] == L'\0')
        return FALSE;

    static const wchar_t *relativeCandidates[] = {
        L"Game Engine\\vcmi\\VCMI_mapeditor.exe",
        L"Game Engine\\vcmi\\bin\\VCMI_mapeditor.exe",
        L"VCMI_mapeditor.exe",
        L"VCMI\\VCMI_mapeditor.exe",
        L"VCMI\\bin\\VCMI_mapeditor.exe",
        L"bin\\VCMI_mapeditor.exe",
        L"external\\vcmi\\VCMI_mapeditor.exe",
        L"external\\vcmi\\bin\\VCMI_mapeditor.exe"
    };

    for (size_t i = 0; i < sizeof(relativeCandidates) / sizeof(relativeCandidates[0]); ++i)
    {
        if (JoinPath(candidate, MAX_PATH, appDirectory, relativeCandidates[i]) && FileExists(candidate))
        {
            wcsncpy(g_editorPath, candidate, MAX_PATH - 1);
            g_editorPath[MAX_PATH - 1] = L'\0';
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL ChooseEditorExecutable(void)
{
    OPENFILENAMEW dialog;
    ZeroMemory(&dialog, sizeof(dialog));
    g_editorPath[0] = L'\0';
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = g_mainWindow;
    dialog.lpstrFilter = L"VCMI Map Editor (VCMI_mapeditor.exe)\0VCMI_mapeditor.exe\0Executables (*.exe)\0*.exe\0All files (*.*)\0*.*\0\0";
    dialog.lpstrFile = g_editorPath;
    dialog.nMaxFile = MAX_PATH;
    dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    dialog.lpstrTitle = L"Locate the VCMI Map Editor";
    return GetOpenFileNameW(&dialog);
}

static BOOL ChooseProjectFile(BOOL campaign, wchar_t *filePath, DWORD capacity)
{
    OPENFILENAMEW dialog;
    ZeroMemory(&dialog, sizeof(dialog));
    filePath[0] = L'\0';
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = g_mainWindow;
    dialog.lpstrFilter = campaign
        ? L"Campaign files (*.h3c;*.vcmp)\0*.h3c;*.vcmp\0All files (*.*)\0*.*\0\0"
        : L"Map files (*.h3m;*.vmap)\0*.h3m;*.vmap\0All files (*.*)\0*.*\0\0";
    dialog.lpstrFile = filePath;
    dialog.nMaxFile = capacity;
    dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    dialog.lpstrTitle = campaign ? L"Open a campaign in Modding Studio" : L"Open a map in Modding Studio";
    return GetOpenFileNameW(&dialog);
}

static BOOL LaunchEditor(const wchar_t *projectFile)
{
    if (!g_editorPath[0] && !LocateEditor() && !ChooseEditorExecutable())
    {
        SetStatus(L"VCMI Map Editor was not selected. Place VCMI_mapeditor.exe beside this app or choose it manually.");
        return FALSE;
    }

    wchar_t commandLine[2 * MAX_PATH + 8];
    int written;
    if (projectFile && projectFile[0])
        written = swprintf(commandLine, sizeof(commandLine) / sizeof(commandLine[0]), L"\"%ls\" \"%ls\"", g_editorPath, projectFile);
    else
        written = swprintf(commandLine, sizeof(commandLine) / sizeof(commandLine[0]), L"\"%ls\"", g_editorPath);

    if (written < 0 || (size_t)written >= sizeof(commandLine) / sizeof(commandLine[0]))
    {
        SetStatus(L"The editor command line is too long.");
        return FALSE;
    }

    wchar_t workingDirectory[MAX_PATH];
    wcsncpy(workingDirectory, g_editorPath, MAX_PATH - 1);
    workingDirectory[MAX_PATH - 1] = L'\0';
    wchar_t *lastSlash = wcsrchr(workingDirectory, L'\\');
    if (lastSlash)
        *lastSlash = L'\0';

    STARTUPINFOW startup;
    PROCESS_INFORMATION process;
    ZeroMemory(&startup, sizeof(startup));
    ZeroMemory(&process, sizeof(process));
    startup.cb = sizeof(startup);

    if (!CreateProcessW(g_editorPath, commandLine, NULL, NULL, FALSE, 0, NULL,
                        workingDirectory[0] ? workingDirectory : NULL, &startup, &process))
    {
        DWORD error = GetLastError();
        wchar_t message[256];
        swprintf(message, sizeof(message) / sizeof(message[0]),
                 L"Could not start VCMI Map Editor (Windows error %lu).", (unsigned long)error);
        SetStatus(message);
        return FALSE;
    }

    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    SetStatus(projectFile && projectFile[0]
        ? L"Project opened in VCMI Map Editor."
        : L"VCMI Map Editor started.");
    return TRUE;
}

static HWND AddControl(HWND parent, const wchar_t *cls, const wchar_t *text,
                       DWORD style, int x, int y, int w, int h, int id, HFONT font)
{
    HINSTANCE instance = (HINSTANCE)GetWindowLongPtrW(parent, GWLP_HINSTANCE);
    HWND control = CreateWindowExW(0, cls, text, WS_CHILD | WS_VISIBLE | style,
        x, y, w, h, parent, id ? (HMENU)(INT_PTR)id : NULL, instance, NULL);
    if (font) SendMessageW(control, WM_SETFONT, (WPARAM)font, TRUE);
    return control;
}

static void CreateAppControls(HWND window)
{
    g_titleFont = CreateFontW(29, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    g_bodyFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    g_headingFont = CreateFontW(17, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    g_smallFont = CreateFontW(13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

    AddControl(window, L"BUTTON", L"ПРОЕКТЫ", BS_GROUPBOX, 28, 145, 690, 145, 0, g_headingFont);
    AddControl(window, L"BUTTON", L"ИНСТРУМЕНТЫ", BS_GROUPBOX, 28, 305, 690, 145, 0, g_headingFont);

    AddControl(window, L"BUTTON", L"Открыть карту  (.h3m / .vmap)",
        BS_PUSHBUTTON | WS_TABSTOP, 52, 190, 310, 48, ID_OPEN_MAP, g_bodyFont);
    AddControl(window, L"BUTTON", L"Открыть кампанию  (.h3c / .vcmp)",
        BS_PUSHBUTTON | WS_TABSTOP, 382, 190, 310, 48, ID_OPEN_CAMPAIGN, g_bodyFont);
    AddControl(window, L"STATIC", L"Выберите существующую карту или кампанию для открытия в редакторе.",
        SS_LEFT, 54, 247, 630, 25, 0, g_smallFont);

    AddControl(window, L"BUTTON", L"Запустить редактор карт",
        BS_PUSHBUTTON | WS_TABSTOP, 52, 348, 310, 48, ID_START_EDITOR, g_bodyFont);
    AddControl(window, L"BUTTON", L"Пакетная конвертация  .h3m  →  .vmap",
        BS_PUSHBUTTON | WS_TABSTOP, 382, 348, 310, 48, ID_BATCH_CONVERT, g_bodyFont);
    AddControl(window, L"STATIC", L"Конвертер находится в меню File редактора VCMI Map Editor.",
        SS_LEFT, 54, 405, 630, 25, 0, g_smallFont);

    g_statusLabel = AddControl(window, L"STATIC",
        L"Готово к работе. Редактор будет найден автоматически или выбран вручную.",
        SS_LEFT, 32, 476, 690, 30, 0, g_smallFont);
}

static LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_ERASEBKGND:
        {
            RECT r; GetClientRect(window, &r);
            FillRect((HDC)wParam, &r, g_backgroundBrush);
            return 1;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(window, &ps);
            RECT header = {0, 0, 760, 124};
            HBRUSH hb = CreateSolidBrush(COLOR_HEADER);
            FillRect(dc, &header, hb);
            DeleteObject(hb);
            SetBkMode(dc, TRANSPARENT);
            SetTextColor(dc, RGB(255, 255, 255));
            HFONT old = (HFONT)SelectObject(dc, g_titleFont);
            TextOutW(dc, 34, 22, L"HEROES III: RECREATE", 19);
            SelectObject(dc, g_bodyFont);
            SetTextColor(dc, RGB(211, 224, 240));
            TextOutW(dc, 37, 68, L"MODDING STUDIO  |  РЕДАКТОР ПРОЕКТОВ", 37);
            SelectObject(dc, g_smallFont);
            TextOutW(dc, 37, 96, L"Карты • кампании • инструменты конвертации", 42);
            SelectObject(dc, old);
            EndPaint(window, &ps);
            return 0;
        }
        case WM_CTLCOLORSTATIC:
        {
            HDC dc = (HDC)wParam;
            SetBkMode(dc, TRANSPARENT);
            SetTextColor(dc, COLOR_MUTED);
            return (LRESULT)g_backgroundBrush;
        }
        case WM_CREATE:
            CreateAppControls(window);
            return 0;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case ID_OPEN_MAP:
                {
                    wchar_t filePath[MAX_PATH];
                    if (ChooseProjectFile(FALSE, filePath, MAX_PATH))
                        LaunchEditor(filePath);
                    return 0;
                }
                case ID_OPEN_CAMPAIGN:
                {
                    wchar_t filePath[MAX_PATH];
                    if (ChooseProjectFile(TRUE, filePath, MAX_PATH))
                        LaunchEditor(filePath);
                    return 0;
                }
                case ID_START_EDITOR:
                    LaunchEditor(NULL);
                    return 0;
                case ID_BATCH_CONVERT:
                    if (LaunchEditor(NULL))
                    {
                        MessageBoxW(window,
                            L"В VCMI Map Editor выберите File -> h3m converter.\n"
                            L"Отметьте нужные .h3m карты (можно выбрать сразу все), затем укажите отдельную пустую папку для результата.\n\n"
                            L"Редактор сохранит карты в формате .vmap. Исходные .h3m останутся без изменений.",
                            L"Пакетная конвертация H3M в VMap", MB_OK | MB_ICONINFORMATION);
                        SetStatus(L"VCMI Map Editor открыт. Используйте File -> h3m converter.");
                    }
                    return 0;
            }
            break;

        case WM_DESTROY:
            if (g_titleFont) DeleteObject(g_titleFont);
            if (g_bodyFont) DeleteObject(g_bodyFont);
            if (g_headingFont) DeleteObject(g_headingFont);
            if (g_smallFont) DeleteObject(g_smallFont);
            if (g_backgroundBrush) DeleteObject(g_backgroundBrush);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(window, message, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR commandLine, int showCommand)
{
    (void)previousInstance;
    (void)commandLine;

    INITCOMMONCONTROLSEX controls;
    controls.dwSize = sizeof(controls);
    controls.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&controls);

    g_backgroundBrush = CreateSolidBrush(COLOR_BG);

    WNDCLASSEXW windowClass;
    ZeroMemory(&windowClass, sizeof(windowClass));
    windowClass.cbSize = sizeof(windowClass);
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = instance;
    windowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
    windowClass.hbrBackground = g_backgroundBrush;
    windowClass.lpszClassName = L"HoMM3RecreateModdingStudioWindow";
    windowClass.hIcon = LoadIconW(NULL, IDI_APPLICATION);
    windowClass.hIconSm = windowClass.hIcon;

    if (!RegisterClassExW(&windowClass))
        return 1;

    HWND window = CreateWindowExW(WS_EX_APPWINDOW, windowClass.lpszClassName, APP_TITLE,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 780, 570,
        NULL, NULL, instance, NULL);
    if (!window)
        return 2;

    g_mainWindow = window;
    ShowWindow(window, showCommand);
    UpdateWindow(window);

    MSG message;
    while (GetMessageW(&message, NULL, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return (int)message.wParam;
}
