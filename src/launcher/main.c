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

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev, PWSTR cmd, int show) {
    (void)instance; (void)prev; (void)cmd; (void)show;
    wchar_t root[MAX_PATH], exe[MAX_PATH], cwd[MAX_PATH];
    DWORD n = GetModuleFileNameW(NULL, root, MAX_PATH);
    if (!n || n >= MAX_PATH) return 2;
    wchar_t *slash = wcsrchr(root, L'\\');
    if (!slash) return 2;
    *slash = L'\0';
    if (swprintf(exe, MAX_PATH, L"%ls\\VCMI\\VCMI_client.exe", root) < 0 || !exists(exe)) {
        if (swprintf(exe, MAX_PATH, L"%ls\\VCMI\\bin\\VCMI_client.exe", root) < 0 || !exists(exe)) {
            if (swprintf(exe, MAX_PATH, L"%ls\\VCMI_client.exe", root) < 0 || !exists(exe)) {
            MessageBoxW(NULL, L"Не найден VCMI_client.exe. Убедитесь, что папка VCMI находится рядом с H3R-CL.exe.", L"Heroes III: Recreate", MB_OK | MB_ICONERROR);
            return 3;
            }
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
