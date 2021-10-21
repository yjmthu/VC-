// PaintText.cpp : 定义应用程序的入口点。
//

#include "windows.h"

// 全局变量:
HINSTANCE hInst;                                 // 当前实例
WCHAR szTitle[] = L"《西游记》前五回标题";        // 标题栏文本
WCHAR szWindowClass[] = L"PaintText";            // 主窗口类名

constexpr DWORD deltaColor = RGB(0XFF / 4, 0XFF / 4, 0XFF / 4);          // 颜色均匀变化的增量

constexpr WCHAR textFamily[5][16] = {                                    // 要绘制的文本
    L"灵根育孕源流出 心性修持大道生",
    L"悟彻菩提真妙理 断魔归本合元神",
    L"四海千山皆拱伏 九幽十类尽除名",
    L"官封弼马心何足 名注齐天意未宁",
    L"乱蟠桃大圣偷丹 反天宫诸神捉怪",
};

constexpr WCHAR fontFamily[5][3] = {                                     // 五行字对应的字体
    L"楷体",
    L"黑体",
    L"宋体",
    L"仿宋",
    L"隶书"
};

const HBRUSH backgroundBrush = CreateSolidBrush(0X800080);               // 绘制用户区所用的紫色画刷


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
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
    wcex.hbrBackground  = backgroundBrush;                 // 选择全局变量紫色画刷，使用户区背景色为紫色
    wcex.lpszMenuName   = NULL;
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
    static short k = 0;

    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HFONT font;
            DWORD color = 0;
            HDC hdc = BeginPaint(hWnd, &ps);
            INT x = 0, y = 0;
            RECT rc;
            GetClientRect(hWnd, &rc);                     // 获取用户区的尺寸
            SetBkMode(hdc, TRANSPARENT);                  // 文字的背景透明
            void* hBrushOld = SelectObject(hdc, backgroundBrush);    // 选择紫色画刷填充用户区
            for (int i = 0; i < 5; i++)
            {
                SetTextColor(hdc, color);
                color += deltaColor;
                for (int j = 0; j < 16; j++)
                {
                    font = CreateFont(                    // 创建合适的字体
                        8 + 32 * j / 15,
                        0, 0, 0,
                        FW_THIN,
                        0, 0, 0, GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                        fontFamily[i]);
                    SelectObject(hdc, font);
                    TextOut(hdc, x, y, textFamily[i]+j, 1);
                    x += 8 + 32 * j / 15;                                // 文字横坐标改变
                    DeleteObject(font);
                }
                Sleep(800);
                Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);    // 重绘用户区
                x = 0;                                                   // 横坐标置0
                y += 40;                                                 // 纵坐标加上40
            }
            SelectObject(hdc, hBrushOld);                                // 选回旧画刷
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        DeleteObject(backgroundBrush);                                   // 程序退出时，清除紫色画刷
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
