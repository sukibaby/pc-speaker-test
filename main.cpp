#include <windows.h>
#include <string>
#include <wingdi.h>
#include <cstdlib>
#include <ctime>
#pragma comment(lib, "Msimg32.lib")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateControls(HWND hwnd);
void HandleCommand(WPARAM wParam, HWND hwndEdit);
void RunMessageLoop();
int WideStringToInt(const wchar_t* str);
HWND CreateMainWindow(HINSTANCE hInstance, int nCmdShow);
void CenterControls(HWND hwnd, HWND hwndButton, HWND hwndEdit);

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
        CW_USEDEFAULT, CW_USEDEFAULT, 250, 150,
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
    HWND hwndButton = CreateWindow(
        L"BUTTON",
        L"Beep",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        0, 0, 100, 30,
        hwnd,
        (HMENU) 1,
        (HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);
    HWND hwndEdit = CreateWindow(
        L"EDIT",
        L"750",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
        0, 0, 100, 30,
        hwnd,
        NULL,
        (HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);
    CenterControls(hwnd, hwndButton, hwndEdit);
}

void CenterControls(HWND hwnd, HWND hwndButton, HWND hwndEdit) {
    RECT rect;
    GetClientRect(hwnd, &rect);

    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    int buttonWidth = 100;
    int buttonHeight = 30;
    int editWidth = 100;
    int editHeight = 30;

    int buttonX = (windowWidth - buttonWidth) / 2;
    int buttonY = (windowHeight - buttonHeight) / 2 - 20;
    int editX = (windowWidth - editWidth) / 2;
    int editY = (windowHeight - editHeight) / 2 + 20;

    SetWindowPos(hwndButton, NULL, buttonX, buttonY, buttonWidth, buttonHeight, SWP_NOZORDER);
    SetWindowPos(hwndEdit, NULL, editX, editY, editWidth, editHeight, SWP_NOZORDER);
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
    static HWND hwndButton;
    static HWND hwndEdit;

    switch (uMsg) {
        case WM_CREATE:
            CreateControls(hwnd);
            hwndButton = FindWindowEx(hwnd, NULL, L"BUTTON", NULL);
            hwndEdit = FindWindowEx(hwnd, NULL, L"EDIT", NULL);
            break;

        case WM_SIZE:
            CenterControls(hwnd, hwndButton, hwndEdit);
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            TRIVERTEX vertex[4];
            GRADIENT_TRIANGLE gTriangle[2];

            // Generate random colors for each vertex
            vertex[0].x = 0;
            vertex[0].y = 0;
            vertex[0].Red = rand() % 0xFFFF;
            vertex[0].Green = rand() % 0xFFFF;
            vertex[0].Blue = rand() % 0xFFFF;
            vertex[0].Alpha = 0x0000;

            vertex[1].x = ps.rcPaint.right;
            vertex[1].y = 0;
            vertex[1].Red = rand() % 0xFFFF;
            vertex[1].Green = rand() % 0xFFFF;
            vertex[1].Blue = rand() % 0xFFFF;
            vertex[1].Alpha = 0x0000;

            vertex[2].x = 0;
            vertex[2].y = ps.rcPaint.bottom;
            vertex[2].Red = rand() % 0xFFFF;
            vertex[2].Green = rand() % 0xFFFF;
            vertex[2].Blue = rand() % 0xFFFF;
            vertex[2].Alpha = 0x0000;

            vertex[3].x = ps.rcPaint.right;
            vertex[3].y = ps.rcPaint.bottom;
            vertex[3].Red = rand() % 0xFFFF;
            vertex[3].Green = rand() % 0xFFFF;
            vertex[3].Blue = rand() % 0xFFFF;
            vertex[3].Alpha = 0x0000;

            // Define the gradient triangles
            gTriangle[0].Vertex1 = 0;
            gTriangle[0].Vertex2 = 1;
            gTriangle[0].Vertex3 = 2;

            gTriangle[1].Vertex1 = 1;
            gTriangle[1].Vertex2 = 2;
            gTriangle[1].Vertex3 = 3;

            // Fill the triangles with the gradient
            GradientFill(hdc, vertex, 4, gTriangle, 2, GRADIENT_FILL_TRIANGLE);

            EndPaint(hwnd, &ps);
        } break;

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
    srand(static_cast<unsigned int>(time(0)));

    HWND hwnd = CreateMainWindow(hInstance, nCmdShow);
    if (hwnd == NULL) {
        return 0;
    }
    RunMessageLoop();
    return 0;
}
