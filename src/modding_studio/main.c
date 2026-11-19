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
static HFONT g_titleFont;
static HFONT g_bodyFont;

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

static void CreateAppControls(HWND window)
{
    HINSTANCE instance = (HINSTANCE)GetWindowLongPtrW(window, GWLP_HINSTANCE);
    g_titleFont = CreateFontW(25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                              DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                              CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    g_bodyFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                             CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

    HWND title = CreateWindowExW(0, L"STATIC", L"MODDING STUDIO",
        WS_CHILD | WS_VISIBLE, 28, 24, 500, 38, window, NULL, instance, NULL);
    SendMessageW(title, WM_SETFONT, (WPARAM)g_titleFont, TRUE);

    HWND subtitle = CreateWindowExW(0, L"STATIC",
        L"Create and edit maps and campaigns with the VCMI editor.",
        WS_CHILD | WS_VISIBLE, 30, 68, 510, 26, window, NULL, instance, NULL);
    SendMessageW(subtitle, WM_SETFONT, (WPARAM)g_bodyFont, TRUE);

    HWND mapButton = CreateWindowExW(0, L"BUTTON", L"Open Map (.h3m / .vmap)",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        30, 122, 240, 44, window, (HMENU)(INT_PTR)ID_OPEN_MAP, instance, NULL);
    SendMessageW(mapButton, WM_SETFONT, (WPARAM)g_bodyFont, TRUE);

    HWND campaignButton = CreateWindowExW(0, L"BUTTON", L"Open Campaign (.h3c / .vcmp)",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        286, 122, 270, 44, window, (HMENU)(INT_PTR)ID_OPEN_CAMPAIGN, instance, NULL);
    SendMessageW(campaignButton, WM_SETFONT, (WPARAM)g_bodyFont, TRUE);

    HWND startButton = CreateWindowExW(0, L"BUTTON", L"Start VCMI Map Editor",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        30, 184, 526, 40, window, (HMENU)(INT_PTR)ID_START_EDITOR, instance, NULL);
    SendMessageW(startButton, WM_SETFONT, (WPARAM)g_bodyFont, TRUE);

    HWND convertButton = CreateWindowExW(0, L"BUTTON", L"Batch convert maps: .h3m -> .vmap",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        30, 238, 526, 44, window, (HMENU)(INT_PTR)ID_BATCH_CONVERT, instance, NULL);
    SendMessageW(convertButton, WM_SETFONT, (WPARAM)g_bodyFont, TRUE);

    g_statusLabel = CreateWindowExW(0, L"STATIC",
        L"Ready. VCMI Map Editor is searched beside this application and in common subfolders.",
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        30, 300, 526, 54, window, NULL, instance, NULL);
    SendMessageW(g_statusLabel, WM_SETFONT, (WPARAM)g_bodyFont, TRUE);
}

static LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
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

    WNDCLASSEXW windowClass;
    ZeroMemory(&windowClass, sizeof(windowClass));
    windowClass.cbSize = sizeof(windowClass);
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = instance;
    windowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClass.lpszClassName = L"HoMM3RecreateModdingStudioWindow";
    windowClass.hIcon = LoadIconW(NULL, IDI_APPLICATION);
    windowClass.hIconSm = windowClass.hIcon;

    if (!RegisterClassExW(&windowClass))
        return 1;

    HWND window = CreateWindowExW(WS_EX_APPWINDOW, windowClass.lpszClassName, APP_TITLE,
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 610, 430,
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
