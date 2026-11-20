#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <shellapi.h>
#include <wchar.h>
#include <stdio.h>

static BOOL exists(const wchar_t *p) {
    DWORD a = GetFileAttributesW(p);
    return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
}

static BOOL directory_has_entries(const wchar_t *path) {
    wchar_t pattern[MAX_PATH];
    WIN32_FIND_DATAW data;
    HANDLE h;
    if (swprintf(pattern, MAX_PATH, L"%ls\\*", path) < 0) return FALSE;
    h = FindFirstFileW(pattern, &data);
    if (h == INVALID_HANDLE_VALUE) return FALSE;
    do {
        if (wcscmp(data.cFileName, L".") != 0 && wcscmp(data.cFileName, L"..") != 0) {
            FindClose(h);
            return TRUE;
        }
    } while (FindNextFileW(h, &data));
    FindClose(h);
    return FALSE;
}

static BOOL join_path(wchar_t *out, size_t cap, const wchar_t *base, const wchar_t *tail) {
    int n = swprintf(out, cap, L"%ls\\%ls", base, tail);
    return n >= 0 && (size_t)n < cap;
}

static void append_line(wchar_t *report, size_t cap, const wchar_t *label, BOOL ok, const wchar_t *detail) {
    size_t used = wcslen(report);
    if (used >= cap) return;
    swprintf(report + used, cap - used, L"%ls: %ls — %ls\r\n",
        label, ok ? L"OK" : L"НЕТ", detail);
}

static int run_diagnostics(const wchar_t *root) {
    wchar_t engine[MAX_PATH], client[MAX_PATH], data[MAX_PATH], maps[MAX_PATH], mods[MAX_PATH];
    wchar_t report[4096] = L"Heroes III: Recreate — проверка установки\r\n\r\n";
    BOOL engine_ok, client_ok, data_ok, maps_ok, mods_ok;
    join_path(engine, MAX_PATH, root, L"Game Engine\\vcmi");
    join_path(client, MAX_PATH, engine, L"VCMI_client.exe");
    if (!exists(client)) join_path(client, MAX_PATH, engine, L"bin\\VCMI_client.exe");
    join_path(data, MAX_PATH, engine, L"data");
    join_path(maps, MAX_PATH, engine, L"h3mmaps");
    join_path(mods, MAX_PATH, engine, L"Mods");
    engine_ok = GetFileAttributesW(engine) != INVALID_FILE_ATTRIBUTES;
    client_ok = exists(client);
    data_ok = directory_has_entries(data);
    maps_ok = directory_has_entries(maps);
    mods_ok = GetFileAttributesW(mods) != INVALID_FILE_ATTRIBUTES;
    append_line(report, 4096, L"Папка движка", engine_ok, engine);
    append_line(report, 4096, L"Исполняемый файл VCMI", client_ok, client_ok ? client : L"VCMI_client.exe не найден");
    append_line(report, 4096, L"Игровые данные (data)", data_ok, data_ok ? L"папка содержит файлы" : L"папка пуста или отсутствует; нужны игровые ресурсы");
    append_line(report, 4096, L"Карты (h3mmaps)", maps_ok, maps_ok ? L"папка содержит файлы" : L"папка пуста или отсутствует; карты пока недоступны");
    append_line(report, 4096, L"Папка модов", mods_ok, mods_ok ? mods : L"папка Mods отсутствует");
    wcscat(report, L"\r\nРежим проверки не запускает игру и не требует оригинальных файлов. Он проверяет структуру установки. Для полноценной игры движку всё ещё нужны совместимые игровые данные.");
    MessageBoxW(NULL, report, L"Recreate — диагностика", MB_OK | (client_ok ? MB_ICONINFORMATION : MB_ICONWARNING));
    return client_ok ? 0 : 3;
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev, PWSTR cmd, int show) {
    (void)instance; (void)prev; (void)show;
    wchar_t root[MAX_PATH], exe[MAX_PATH], cwd[MAX_PATH];
    DWORD n = GetModuleFileNameW(NULL, root, MAX_PATH);
    if (!n || n >= MAX_PATH) return 2;
    wchar_t *slash = wcsrchr(root, L'\\');
    if (!slash) return 2;
    *slash = L'\0';

    if (cmd && (wcsstr(cmd, L"/check") || wcsstr(cmd, L"--check")))
        return run_diagnostics(root);

    wchar_t data[MAX_PATH], maps[MAX_PATH];
    join_path(data, MAX_PATH, root, L"Game Engine\\vcmi\\data");
    join_path(maps, MAX_PATH, root, L"Game Engine\\vcmi\\h3mmaps");
    if (!directory_has_entries(data) || !directory_has_entries(maps)) {
        int answer = MessageBoxW(NULL,
            L"Игровые ресурсы Heroes III не найдены. Без них полноценная игра не запустится.\n\n"
            L"Запустить режим проверки установки? Он проверит движок и папки, но не запускает игру.",
            L"Heroes III: Recreate — нет игровых данных",
            MB_YESNO | MB_ICONINFORMATION);
        if (answer == IDYES) return run_diagnostics(root);
        return 5;
    }

    if (!join_path(exe, MAX_PATH, root, L"Game Engine\\vcmi\\VCMI_client.exe") || !exists(exe)) {
        if (!join_path(exe, MAX_PATH, root, L"Game Engine\\vcmi\\bin\\VCMI_client.exe") || !exists(exe)) {
            MessageBoxW(NULL, L"Не найден VCMI_client.exe. Проверьте папку Game Engine\\vcmi.", L"Heroes III: Recreate", MB_OK | MB_ICONERROR);
            return 3;
        }
    }
    wcsncpy(cwd, exe, MAX_PATH - 1); cwd[MAX_PATH - 1] = 0;
    slash = wcsrchr(cwd, L'\\'); if (slash) *slash = 0;
    SHELLEXECUTEINFOW si; ZeroMemory(&si, sizeof(si));
    si.cbSize = sizeof(si); si.fMask = SEE_MASK_NOCLOSEPROCESS;
    si.lpFile = exe; si.lpDirectory = cwd; si.nShow = SW_SHOWNORMAL;
    if (!ShellExecuteExW(&si)) {
        wchar_t msg[160]; swprintf(msg, 160, L"Не удалось запустить VCMI (ошибка Windows %lu).", (unsigned long)GetLastError());
        MessageBoxW(NULL, msg, L"Heroes III: Recreate", MB_OK | MB_ICONERROR); return 4;
    }
    if (si.hProcess) CloseHandle(si.hProcess);
    return 0;
}
