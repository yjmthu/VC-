// RegularPentagon.cpp : 定义应用程序的入口点。
//

#include <time.h>
#include <math.h>
#include <vector>
#include  <algorithm>
#include <Windows.h>

#ifdef min
#undef min
#endif // min

constexpr double PI = 3.14159265358979323846;

// 全局变量:
HINSTANCE hInst;                                // 当前实例
TCHAR szTitle[] = TEXT("正五边形");                  // 标题栏文本
TCHAR szWindowClass[] = TEXT("RegularPentagon");            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD getRandomColor(const std::vector<DWORD>& invalid_colors);           // 生成随机颜色

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

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
   int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN), w = sw/2, h = sh/2;

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, sw/4
      , sh/4, w, h, nullptr, nullptr, hInstance, nullptr);

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
    static int cases = 0;
    switch (message)
    {
    case WM_CREATE:
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // 确定关键点
            RECT rect;
            GetClientRect(hWnd, &rect);
            const int ox = (rect.right - rect.left) /2, oy = (rect.bottom - rect.top) / 2;
            const int r1 = 2 * std::min(ox, oy) / 3, r2 = r1 * sin(PI / 10) / sin(3 * PI / 10);
            POINT pts[10]{
                ox, oy - r1,
                ox - r1 * cos(PI / 10), oy - r1 * sin(PI / 10),
                ox - r1 * sin(PI / 5), oy + r1 * cos(PI / 5),
                ox + r1 * sin(PI / 5), oy + r1 * cos(PI / 5),
                ox + r1 * cos(PI / 10), oy - r1 * sin(PI / 10),
                ox, oy + r2,
                ox + r2 * cos(PI / 10), oy + r2 * sin(PI / 10),
                ox + r2 * sin(PI / 5), oy - r2 * cos(PI / 5),
                ox - r2 * sin(PI / 5), oy - r2 * cos(PI / 5),
                ox - r2 * cos(PI / 10), oy + r2 * sin(PI / 10),
            };

            // 先绘制填充颜色, 然后再绘制边线，以免遮盖
            HBRUSH hBrush, hBrushOld;
            std::vector<DWORD> colors{ 0x0000FF, 0xFFFFFF };    // 规定除五边形外边框外，不能是红色或者白色
            colors.push_back(getRandomColor(colors));
            hBrush = CreateSolidBrush(colors.back());
            hBrushOld = static_cast<HBRUSH>(SelectObject(hdc, hBrush));
            Polygon(hdc, pts + 5, 5);
            SelectObject(hdc, hBrushOld);
            DeleteObject(hBrush);

            for (int i = 0; i < 5; ++i)
            {
                colors.push_back(getRandomColor(colors));
                hBrush = CreateSolidBrush(colors.back());
                hBrushOld = static_cast<HBRUSH>(SelectObject(hdc, hBrush));
                POINT temp_pts[2][3] = {
                    { pts[i], pts[(i + 2) % 5 + 5], pts[(i + 3) % 5 + 5] },
                    { pts[i + 5], pts[(i + 2) % 5], pts[(i + 3) % 5] }
                };
                Polygon(hdc, temp_pts[0], 3);
                colors.push_back(getRandomColor(colors));
                hBrush = CreateSolidBrush(colors.back());
                DeleteObject(SelectObject(hdc, hBrush));
                Polygon(hdc, temp_pts[1], 3);
                SelectObject(hdc, hBrushOld);
                DeleteObject(hBrush);
            }

            // 绘制红色五边形轮廓
            HPEN hPen = CreatePen(PS_SOLID, 5, colors.front());        // 红色画笔
            HPEN hPenOld = static_cast<HPEN>(SelectObject(hdc, hPen));
            MoveToEx(hdc, pts->x, pts->y, NULL);                       // 移动到起点
            for (int i = 0; ++i < 6;)
                LineTo(hdc, pts[i % 5].x, pts[i % 5].y);
            SelectObject(hdc, hPenOld);
            DeleteObject(hPen);

            // 绘制五条不同颜色的线条。
            MoveToEx(hdc, pts->x, pts->y, NULL);                      // 移动到起点
            for (int i = 0; ++i < 6;)
            {
                int j = (2 * i) % 5;
                colors.push_back(getRandomColor(colors));
                hPen = CreatePen(PS_SOLID, 4, colors.back());
                hPenOld = static_cast<HPEN>(SelectObject(hdc, hPen));
                LineTo(hdc, pts[j].x, pts[j].y);
                SelectObject(hdc, hPenOld);
                DeleteObject(hPen);
            }
            EndPaint(hWnd, &ps);
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

DWORD getRandomColor(const std::vector<DWORD>& invalid_colors)
{
    static char init = (srand(time(0)), 0);
    DWORD rgb = RGB(rand() % 256, rand() % 256, rand() % 256);
    if (std::find(invalid_colors.begin(), invalid_colors.end(), rgb) != invalid_colors.end())
        return getRandomColor(invalid_colors);
    return rgb;
}
