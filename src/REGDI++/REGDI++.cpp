#include <Windows.h>
#include <unordered_map>
#include <chrono>
#include <string>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

HINSTANCE hInst;
HWND hWndMainWindow;
LPCWSTR szTitle = L"REGDI++";
LPCWSTR szWindowClass = L"REGDIPlusPlusClass";

const int size = 6;
const int columnGrid = 99;
const int maxElements = columnGrid * (columnGrid + 1);
static std::unordered_map<HWND, HBRUSH> hwndBrushMap(maxElements);

std::wstring format_duration(std::chrono::milliseconds ms) {
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(ms);
    ms -= std::chrono::duration_cast<std::chrono::milliseconds>(secs);
    auto mins = std::chrono::duration_cast<std::chrono::minutes>(secs);
    secs -= std::chrono::duration_cast<std::chrono::seconds>(mins);

    std::wstringstream ss;
    ss << std::setfill(L'0') << std::setw(2) << mins.count() << L":" << std::setfill(L'0') 
        << std::setw(2) << secs.count() << L"." << std::setfill(L'0') << std::setw(3) << ms.count();
    return ss.str();
}

static HWND label;
static int drawCount = -1;
static std::chrono::steady_clock::time_point startPoint;
static std::chrono::milliseconds controlGenerationMs;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        label = CreateWindowW(L"Static", L"Wait",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            275, 275, 50, 16,
            hWnd, 0, hInst, NULL);
    }
    break;


    case WM_SYSCOMMAND:
    {
        if (wParam == SC_RESTORE)
            drawCount = 0;

        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;

    case WM_CTLCOLORSTATIC:
    {
        if (drawCount == 0)
        {
            SetWindowText(hWndMainWindow, (L"Controls created in: " + format_duration(controlGenerationMs) + L" || Painting...").c_str());
            startPoint = std::chrono::steady_clock::now();
        }

        if (drawCount == maxElements - 1)
        {
            auto finishPoint = std::chrono::steady_clock::now();
            auto paintMs = std::chrono::duration_cast<std::chrono::milliseconds>(finishPoint - startPoint);
            SetWindowText(hWndMainWindow, (L"Controls created in: " + format_duration(controlGenerationMs) + L" || Controls painted in: " + format_duration(paintMs)).c_str());
        }

        drawCount++;

        HDC hdcStatic = (HDC)wParam;
        auto hwnd = WindowFromDC(hdcStatic);
        return (INT_PTR)hwndBrushMap[hwnd];
    }
    break;

    case WM_TIMER:
    {
        KillTimer(hWndMainWindow, 1);

        DestroyWindow(label);
        SetWindowText(hWndMainWindow, L"Creating controls...");

        auto start = std::chrono::steady_clock::now();

        for (size_t i = 0; i < columnGrid; i++)
        {
            for (size_t j = 0; j < columnGrid + 1; j++)
            {
                hwndBrushMap[(CreateWindowW(L"Static", NULL,
                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                    i * size, j * size, size, size,
                    hWnd, 0, hInst, NULL))] = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
            }
        }

        drawCount = 0;
        
        auto finish = std::chrono::steady_clock::now();
        controlGenerationMs = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    hWndMainWindow = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, 0, 609, 639, nullptr, nullptr, hInstance, nullptr);

    if (!hWndMainWindow)
        return FALSE;

    ShowWindow(hWndMainWindow, nCmdShow);
    UpdateWindow(hWndMainWindow);

    return TRUE;
}

ATOM RegisterClassInstance(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = szWindowClass;

    return RegisterClassExW(&wcex);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    RegisterClassInstance(hInstance);
    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    auto test = SetTimer(hWndMainWindow, 1, 2000, NULL);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}