#include <windows.h>
#include <string>
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateControls(HWND hwnd);
void HandleCommand(WPARAM wParam, HWND hwndEdit);
void RunMessageLoop();
int WideStringToInt(const wchar_t* str);
HWND CreateMainWindow(HINSTANCE hInstance, int nCmdShow);
int WideStringToInt(const wchar_t* str) {
    return _wtoi(str);
}
HWND CreateMainWindow(HINSTANCE hInstance, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"test that pc speaker";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);
    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"PC Speaker Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 150, 150,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return NULL;
    }

    ShowWindow(hwnd, nCmdShow);
    return hwnd;
}
void CreateControls(HWND hwnd) {
    CreateWindow(
        L"BUTTON",
        L"Beep",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10,
        10,
        100,
        30,
        hwnd,
        (HMENU) 1,
        (HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);
    CreateWindow(
        L"EDIT",
        L"750",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
        10,
        50,
        100,
        30,
        hwnd,
        NULL,
        (HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);
}
void HandleCommand(WPARAM wParam, HWND hwndEdit) {
    if (LOWORD(wParam) == 1) {
        wchar_t buffer[10];
        GetWindowText(hwndEdit, buffer, 10);
        int frequency = WideStringToInt(buffer);
        Beep(frequency, 300);
    }
}
void RunMessageLoop() {
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hwndEdit;

    switch (uMsg) {
        case WM_CREATE:
            CreateControls(hwnd);
            hwndEdit = FindWindowEx(hwnd, NULL, L"EDIT", NULL);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_COMMAND:
            HandleCommand(wParam, hwndEdit);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HWND hwnd = CreateMainWindow(hInstance, nCmdShow);
    if (hwnd == NULL) {
        return 0;
    }
    RunMessageLoop();
    return 0;
}
