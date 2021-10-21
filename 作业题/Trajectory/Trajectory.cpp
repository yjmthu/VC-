// PaintText.cpp : 定义应用程序的入口点。
//
#include <vector>
#include <windows.h>
#include <windowsx.h>

// 全局变量:
HINSTANCE hInst;                                // 当前实例
constexpr WCHAR szTitle[] = L"画点连线";                  // 标题栏文本
constexpr WCHAR szWindowClass[] = L"Trajectory";            // 主窗口类名

constexpr int MY_TIMER = 2021;

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
    if (!InitInstance(hInstance, nCmdShow))
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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

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
    static std::vector<std::vector<POINT>> pPts;
    static INT paintLine = 0;
    switch (message)
    {
    case WM_SIZE:
        paintLine = 2;   // 重画所有线条，防止调整窗口大小导致黑线消失
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // 只有多于一个点的时候才会画点或者连线，因为第一个点是鼠标左键刚按下时的位置。
        if (pPts.size() && pPts.back().size() > 1)
        {
            if (paintLine == 1)   // 只需要画最后一条线
            {
                std::vector<POINT>::const_iterator iter = pPts.back().begin();
                MoveToEx(hdc, iter->x, iter->y, NULL);
                while (++iter != pPts.back().end()) LineTo(hdc, iter->x, iter->y);
            }
            else if (paintLine == 2)   // 需要重画所有线
            {
                std::vector<POINT>::const_iterator iter;
                for (const auto& p : pPts)
                {
                    // if (p.size() <= 1) break;
                    iter = p.begin();
                    MoveToEx(hdc, iter->x, iter->y, NULL);
                    while (++iter != p.end()) LineTo(hdc, iter->x, iter->y);
                }
            }
            else // 画出pts中增加的那个点
            {
                SetPixel(hdc, pPts.back().back().x, pPts.back().back().y, 0x000000);
            }
        }
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_CREATE:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        SetCapture(hWnd);    // 不间断捕获鼠标，防止鼠标移出用户区而捕捉不到鼠标左键释放消息
        // 左键按下，开始Timer
        SetTimer(hWnd, MY_TIMER, 50, nullptr);
        // 记录按下的点的位置
        pPts.push_back(std::vector<POINT>(1, POINT{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }));
        break;
    case WM_LBUTTONUP:
        ReleaseCapture();
        KillTimer(hWnd, MY_TIMER);
        // 单击后释放，并且没有移动鼠标，清空内容
        if (pPts.size() && pPts.back().size() == 1 && pPts.back().front().x == GET_X_LPARAM(lParam) && pPts.back().front().y == GET_Y_LPARAM(lParam))
        {
            pPts.clear();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        // 单机后释放，鼠标运动了，将所有的点依次连起来
        paintLine = 1;
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_TIMER:
    {
        switch (wParam)
        {
        case MY_TIMER:
        {
            POINT curPos;
            GetCursorPos(&curPos);
            ScreenToClient(hWnd, &curPos);
            if (pPts.back().back().x != curPos.x || pPts.back().back().y != curPos.y)
            {
                pPts.back().push_back(curPos); // 只有鼠标移动了才把点添加进去，以免鼠标不动却重复添加同样一个点
            }
            paintLine = 0;      // 画点而不是连线
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
