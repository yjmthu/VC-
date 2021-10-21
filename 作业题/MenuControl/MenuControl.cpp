// MenuControl.cpp : 定义应用程序的入口点。
//
#include <utility>
#include <vector>
#include "framework.h"
#include "MenuControl.h"

#define MAX_LOADSTRING 100

typedef BOOL(_stdcall* drawMethord) (HDC, int, int, int, int);

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
UCHAR drawMode = 0;
drawMethord fuc[2] = { Ellipse,Rectangle };

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MENUCONTROL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MENUCONTROL));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MENUCONTROL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MENUCONTROL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static const HMENU hmenu = GetMenu(hWnd);
    static HMENU haddmenu = nullptr;
    constexpr WCHAR menuNames[2][3] = {L"圆形", L"矩形"};
    static std::vector<std::pair<bool, RECT>> history;
    switch (message)
    {
    case WM_CREATE:
        EnableMenuItem(hmenu, IDM_EXIT, MF_GRAYED);
        break;
    case WM_COMMAND:
        {
            BOOL drawModeTemp = TRUE;
            RECT rectClient;
            GetClientRect(hWnd, &rectClient);
            BOOL menuRect = TRUE;
            double scale = 0.6;
            switch (LOWORD(wParam))
            {
            case ID_32779:
                history.clear();
            case ID_32778:
                if (history.size()) history.pop_back();
            case IDM_EXIT:
                if (haddmenu)
                    DeleteMenu(hmenu, 1, MF_BYPOSITION);
                haddmenu = nullptr;
                CheckMenuItem(hmenu, ID_Menu, MF_UNCHECKED);
                CheckMenuItem(hmenu, ID_32772, MF_UNCHECKED);
                EnableMenuItem(hmenu, IDM_EXIT, MF_GRAYED);
                DrawMenuBar(hWnd);
                InvalidateRect(hWnd, 0, TRUE);
                break;
            case ID_Menu:
                menuRect = FALSE;
            case ID_32772:
                if (haddmenu) DeleteMenu(hmenu, 1, MF_BYPOSITION);
                haddmenu = CreateMenu();
                AppendMenu(haddmenu, MF_ENABLED, ID_32773, L"绘制图形");
                AppendMenu(haddmenu, MF_ENABLED, ID_32774, L"移动图形");
                AppendMenu(haddmenu, MF_ENABLED, ID_32775, L"放大");
                AppendMenu(haddmenu, MF_ENABLED, ID_32776, L"缩小");
                AppendMenu(haddmenu, MF_ENABLED, ID_32777, L"重绘");
                AppendMenu(haddmenu, MF_ENABLED, ID_32778, L"清除本次");
                InsertMenu(hmenu, 1, MF_POPUP | MF_BYPOSITION, (UINT)haddmenu, menuNames[menuRect]);
                CheckMenuItem(hmenu, ID_Menu, menuRect ? MF_UNCHECKED: MF_CHECKED);
                CheckMenuItem(hmenu, ID_32772, menuRect ? MF_CHECKED : MF_UNCHECKED);
                EnableMenuItem(hmenu, IDM_EXIT, MF_ENABLED);
                DrawMenuBar(hWnd);
                history.emplace_back(menuRect, RECT{ rectClient.right / 2 - 50, rectClient.bottom / 2 - 50 , rectClient.right / 2 + 50, rectClient.bottom / 2 + 50 });
            case ID_32773:
                drawModeTemp = FALSE;
            case ID_32774:
                CheckMenuItem(haddmenu, ID_32773, drawModeTemp ? MF_UNCHECKED : MF_CHECKED);
                CheckMenuItem(haddmenu, ID_32774, drawModeTemp ? MF_CHECKED : MF_UNCHECKED);
                drawMode = drawModeTemp;
                InvalidateRect(hWnd, 0, TRUE);
                break;
            case ID_32775:
                scale = 1.6;
            case ID_32776:
            {
                int x, y, w, h;
                x = history.back().second.right + history.back().second.left;
                y = history.back().second.bottom + history.back().second.top;
                w = (history.back().second.right - history.back().second.left) * scale;
                h = (history.back().second.bottom - history.back().second.top) * scale;
                history.back().second = { (x - w) / 2,(y - h) / 2, (x + w) / 2,(y + h) / 2 };
                InvalidateRect(hWnd, 0, TRUE);
                break;
            }
            case ID_32777:
                history.back().second = { rectClient.right / 2 - 50, rectClient.bottom / 2 - 50 , rectClient.right / 2 + 50, rectClient.bottom / 2 + 50 };
                InvalidateRect(hWnd, 0, TRUE);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            if (history.size())
            {
                RECT clientRect;
                GetClientRect(hWnd, &clientRect);
                HDC mdc = CreateCompatibleDC(hdc);
                HBITMAP hBmp = CreateCompatibleBitmap(mdc, clientRect.right, clientRect.bottom);
                SelectObject(mdc, hBmp);
                const auto brush = SelectObject(hdc, GetSysColorBrush(COLOR_WINDOW));
                Rectangle(mdc, -1, -1, clientRect.right + 2, clientRect.bottom + 2);
                SelectObject(mdc, GetStockObject(NULL_BRUSH));
                for (const auto& p : history)
                    fuc[p.first](mdc, p.second.left, p.second.top, p.second.right, p.second.bottom);
                BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mdc, 0, 0, SRCCOPY);
                SelectObject(mdc, brush);
                DeleteObject(hBmp);
                DeleteDC(mdc);
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_KEYDOWN:
        if (!haddmenu)
            break;
        if (history.back().first)
        {
            if (drawMode)
            {
                switch (wParam)
                {
                case VK_RIGHT:
                    ++history.back().second.right;
                    ++history.back().second.left;
                    break;
                case VK_LEFT:
                    --history.back().second.left;
                    --history.back().second.right;
                    break;
                case VK_UP:
                    --history.back().second.top;
                    --history.back().second.bottom;
                    break;
                case VK_DOWN:
                    ++history.back().second.bottom;
                    ++history.back().second.top;
                default:
                    break;
                }
            }
            else
            {
                switch (wParam)
                {
                case VK_RIGHT:
                    ++history.back().second.right;
                    break;
                case VK_LEFT:
                    --history.back().second.right;
                    break;
                case VK_UP:
                    --history.back().second.bottom;
                    break;
                case VK_DOWN:
                    ++history.back().second.bottom;
                default:
                    break;
                }
            }
        }
        else
        {
            if (drawMode)
            {
                switch (wParam)
                {
                case VK_RIGHT:
                    ++history.back().second.right;
                    ++history.back().second.left;
                    break;
                case VK_LEFT:
                    --history.back().second.left;
                    --history.back().second.right;
                    break;
                case VK_UP:
                    --history.back().second.top;
                    --history.back().second.bottom;
                    break;
                case VK_DOWN:
                    ++history.back().second.bottom;
                    ++history.back().second.top;
                default:
                    break;
                }
            }
            else
            {
                switch (wParam)
                {
                case VK_LEFT:
                case VK_UP:
                    --history.back().second.bottom;
                    --history.back().second.right;
                    break;
                case VK_RIGHT:
                case VK_DOWN:
                    ++history.back().second.bottom;
                    ++history.back().second.right;
                default:
                    break;
                }
            }
        }
        InvalidateRect(hWnd, 0, TRUE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
