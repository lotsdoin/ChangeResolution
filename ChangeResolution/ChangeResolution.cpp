// ChangeResolution.cpp: 定义应用程序的入口点。
//
#include <windows.h>
#include <stdio.h>

// Function prototypes
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void change_resolution(int width, int height);

// Button IDs
#define BUTTON_4K 1
#define BUTTON_2K 2
#define BUTTON_1080P 3

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASS wc = { 0 };

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = "ResolutionSwitcher";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClass(&wc)) {
        return -1;
    }
    // 获取屏幕分辨率信息和标题栏高度信息
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int titleBarHeight = GetSystemMetrics(SM_CYSIZE);
    HWND hwnd = CreateWindow("ResolutionSwitcher", "Resolution Switcher",
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                             (screenWidth - 280) / 2,  // 设置窗口 X 坐标为居中
                             (screenHeight - 200 - titleBarHeight) / 2,  // 设置窗口 Y 坐标为居中
                             280, 200,
                             NULL, NULL, hInst, NULL);

    if (!hwnd) {
        return -1;
    }

    MSG msg = { 0 };

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindow("BUTTON", "4K (3840x2160)",
            WS_VISIBLE | WS_CHILD,
            20, 20, 220, 30,
            hwnd, (HMENU)BUTTON_4K, NULL, NULL);

        CreateWindow("BUTTON", "2K (2560x1440)",
            WS_VISIBLE | WS_CHILD,
            20, 60, 220, 30,
            hwnd, (HMENU)BUTTON_2K, NULL, NULL);

        CreateWindow("BUTTON", "1080p (1920x1080)",
            WS_VISIBLE | WS_CHILD,
            20, 100, 220, 30,
            hwnd, (HMENU)BUTTON_1080P, NULL, NULL);
        break;
    }
    case WM_COMMAND: {
        switch (wp) {
        case BUTTON_4K:
            change_resolution(3840, 2160);
            break;
        case BUTTON_2K:
            change_resolution(2560, 1440);
            break;
        case BUTTON_1080P:
            change_resolution(1920, 1080);
            break;
        }
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    default: {
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    }

    return 0;
}

void change_resolution(int width, int height) {
    DEVMODE dm;
    memset(&dm, 0, sizeof(dm));
    dm.dmSize = sizeof(dm);

    // Get the current display settings
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

    // Set the new resolution
    dm.dmPelsWidth = width;
    dm.dmPelsHeight = height;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

    // Change the display settings
    LONG result = ChangeDisplaySettings(&dm, 0);
    if (result != DISP_CHANGE_SUCCESSFUL) {
        MessageBox(NULL, TEXT("Failed to change resolution"), TEXT("Error"), MB_OK | MB_ICONERROR);
    }
    else {
        MessageBox(NULL, TEXT("Successfully changed resolution"), TEXT("Success"), MB_OK | MB_ICONINFORMATION);
    }
}