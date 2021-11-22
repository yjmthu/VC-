// EatBeans.cpp : 定义应用程序的入口点。
//

#include <cmath>
#include <deque>
#include <vector>
#include <fstream>
#include <string>
#include <Windows.h>

// 全局变量:
HINSTANCE hInst;                                // 当前实例
TCHAR szTitle[] = TEXT("吃黄豆游戏");                  // 标题栏文本
TCHAR szWindowClass[] = TEXT("EatBeans");            // 主窗口类名
constexpr int TIMER_MONSTER_MOVE = 2021;

enum class GameMode
{
    Put, Eat
};

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

inline LONG get_distance_square(POINT& a, POINT& b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

inline BOOL point_in_rect(const POINT& pt, const RECT& rt)
{
    return (rt.left < pt.x) && (pt.x < rt.right) && (rt.top < pt.y) && (pt.y < rt.bottom);
}

inline HFONT CreateFontB(HWND hWnd, const TCHAR* family, int height, bool bold=true)
{
    return CreateFont(height,		//字体的高度
        0,		//由系统根据高宽比选取字体最佳宽度值
        0,		//文本的倾斜度为0，表示水平 
        0,		//字体与基线的倾斜度为0， 表示与基线平行
        bold?FW_HEAVY:FW_THIN,		//字体的粗度，FW_HEAVY为最粗
        0,			//非斜体字
        0,			//无下划线
        0,			//无删除线
        GB2312_CHARSET,	//表示所用的字符集为ANSI_CHARSET
        OUT_DEFAULT_PRECIS,		//输出精度为默认精度
        CLIP_DEFAULT_PRECIS,		//剪裁精度为默认精度
        DEFAULT_QUALITY,		//输出质量为默认值
        DEFAULT_PITCH | FF_DONTCARE,	//字间距和字体系列使用默认值
        family		//字体名称
    );
}

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
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
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
    static short beanSpeed = 10;
    static short monsterSpeed = 4;
    constexpr int beanRadius = 5; 
    constexpr int monsterRadius = 20;
    constexpr WCHAR tip[] = L"玩法：1. 按上下左右键移动红豆位置；2. 按Enter键放置黄豆；3. 鼠标在黄色矩形区域内点击也可放置黄豆。";
    static std::deque<POINT> pts(1, { 2 * monsterRadius + beanRadius, 2 * monsterRadius + beanRadius });
    static POINT monster = POINT{ monsterRadius, monsterRadius };
    static GameMode mode = GameMode::Put;
    static RECT btn1, btn2;
    static std::vector<LONG> mouseDirection {monsterRadius * 2, monsterRadius, monsterRadius * 2, monsterRadius };
    static RECT rect;
    static HFONT hFontBtn = CreateFontB(hWnd, TEXT("楷体"), monsterRadius-4);
    static HFONT hFontLabel = CreateFontB(hWnd, TEXT("华文行楷"), monsterRadius);
    static HFONT hFontTip = CreateFontB(hWnd, TEXT("微软雅黑"), monsterRadius-1, false);

    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH hBrush, hBrushOld;
        HPEN hPen, hPenOld;
        HFONT hFontOld = static_cast<HFONT>(SelectObject(hdc, hFontBtn));
        hBrush = CreateSolidBrush(0xa0f0f0);
        hPen = CreatePen(PS_DASH, 1, 0);
        hBrushOld = static_cast<HBRUSH>(SelectObject(hdc, hBrush));
        hPenOld = static_cast<HPEN>(SelectObject(hdc, hPen));
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        hBrush = CreateSolidBrush(0x808080);
        hPen = CreatePen(PS_SOLID, 1, 0);
        DeleteObject(SelectObject(hdc, hBrush));
        DeleteObject(SelectObject(hdc, hPen));
        Rectangle(hdc, btn1.left, btn1.top, btn1.right, btn1.bottom);
        Rectangle(hdc, btn2.left, btn2.top, btn2.right, btn2.bottom);
        SetTextColor(hdc, RGB(0, 0xFF, 0));
        SetBkMode(hdc, TRANSPARENT);
        TextOut(hdc, btn1.left+2, btn1.top, TEXT("重玩"), 2);
        TextOut(hdc, btn2.left+2, btn2.top, TEXT("吃豆"), 2);
        SelectObject(hdc, hFontTip);
        SetTextColor(hdc, 0);
        TextOutW(hdc, rect.left, rect.bottom, tip, wcslen(tip));

        if (mode == GameMode::Put)
        {
            SelectObject(hdc, hFontLabel);
            SetTextColor(hdc, RGB(0, 0, 255));
            TEXTMETRIC tm;
            GetTextMetrics(hdc, &tm);
            std::wstring len = L"放了";
            len += std::to_wstring(pts.size()-1) + L"粒黄豆";
            TextOutW(hdc, (rect.right - (size_t)rect.left - len.size() * tm.tmAveCharWidth) / 2, 0, len.c_str(), len.size());

            hBrush = CreateSolidBrush(0X800080);
            hPen = CreatePen(PS_SOLID, 2, RGB(0xFF, 0, 0));
            DeleteObject(SelectObject(hdc, hBrush));
            DeleteObject(SelectObject(hdc, hPen));
            Pie(hdc, 0, 0, monsterRadius * 2, monsterRadius * 2, mouseDirection[0], mouseDirection[1], mouseDirection[2], mouseDirection[3]);
            hBrush = CreateSolidBrush(0XFFFF);
            DeleteObject(SelectObject(hdc, hBrush));
            DeleteObject(SelectObject(hdc, hPenOld));
            std::deque<POINT>::const_iterator iter = pts.cend();
            while (--iter != pts.cbegin())
                Ellipse(hdc, iter->x - beanRadius, iter->y - beanRadius, iter->x + beanRadius, iter->y + beanRadius);
            hBrush = CreateSolidBrush(0XFF);
            DeleteObject(SelectObject(hdc, hBrush));
            Ellipse(hdc, iter->x - beanRadius, iter->y - beanRadius, iter->x + beanRadius, iter->y + beanRadius);
            SelectObject(hdc, hBrushOld);
            DeleteObject(hBrush);
            DeleteObject(SelectObject(hdc, hPen));
        }
        else if (pts.size())
        {
            SelectObject(hdc, hFontLabel);
            SetTextColor(hdc, RGB(0, 0, 255));
            TEXTMETRIC tm;
            GetTextMetrics(hdc, &tm);
            std::wstring len = L"剩余";
            len += std::to_wstring(pts.size()) + L"粒黄豆";
            TextOutW(hdc, (rect.right - (size_t)rect.left - len.size() * tm.tmAveCharWidth) / 2, 0, len.c_str(), len.size());

            hBrush = CreateSolidBrush(0XFFFF);
            DeleteObject(SelectObject(hdc, hBrush));
            DeleteObject(SelectObject(hdc, hPenOld));
            for (const auto& p : pts)
                Ellipse(hdc, p.x - beanRadius, p.y - beanRadius, p.x + beanRadius, p.y + beanRadius);
            hBrush = CreateSolidBrush(0X800080);
            DeleteObject(SelectObject(hdc, hBrush));
            hPen = CreatePen(PS_SOLID, 2, RGB(0xFF, 0, 0));
            hPenOld = static_cast<HPEN>(SelectObject(hdc, hPen));
            Pie(hdc, monster.x - monsterRadius, monster.y - monsterRadius,
                monster.x + monsterRadius, monster.y + monsterRadius,
                mouseDirection[0], mouseDirection[1], mouseDirection[2], mouseDirection[3]);
            SelectObject(hdc, hBrushOld);
            DeleteObject(hBrush);
        }
        else
        {
            SelectObject(hdc, hFontLabel);
            SetTextColor(hdc, RGB(0, 0, 255));
            TEXTMETRIC tm;
            GetTextMetrics(hdc, &tm);
            TextOutW(hdc, (rect.right - rect.left - 7 * tm.tmAveCharWidth) / 2, 0, L"豆子被吃完了！", 7);

            hBrush = CreateSolidBrush(0X800080);
            DeleteObject(SelectObject(hdc, hBrush));
            hPen = CreatePen(PS_SOLID, 2, RGB(0xFF, 0, 0));
            DeleteObject(SelectObject(hdc, hPen));
            Pie(hdc, monster.x - monsterRadius, monster.y - monsterRadius,
                monster.x + monsterRadius, monster.y + monsterRadius,
                mouseDirection[0], mouseDirection[1], mouseDirection[2], mouseDirection[3]);
            DeleteObject(SelectObject(hdc, hBrushOld));
            DeleteObject(SelectObject(hdc, hPenOld));
        }
        SelectObject(hdc, hFontOld);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_KEYDOWN:
    {
        if (mode != GameMode::Put) break;
        switch (wParam)
        {
        case VK_RIGHT:
            pts.front().x += beanSpeed;
            if (pts.front().x > rect.right)
                pts.front().x = rect.right;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_LEFT:
            pts.front().x -= beanSpeed;
            if (pts.front().x < rect.left)
                pts.front().x = rect.left;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_UP:
            pts.front().y -= beanSpeed;
            if (pts.front().y < rect.top)
                pts.front().y = rect.top;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_DOWN:
            pts.front().y += beanSpeed;
            if (pts.front().y > rect.bottom)
                pts.front().y = rect.bottom;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_RETURN:
            if (pts.size() >= 2 && std::find_if(pts.begin() + 1, pts.end(),
                [](const POINT& pt)->bool { return pts.front().x == pt.x && pts.front().y == pt.y; }) != pts.end())
                break;
            pts.push_front(pts.front());
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        default:
            break;
        }
        break;
    }
    case WM_SIZE:
    {
        GetClientRect(hWnd, &rect);
        rect.left += monsterRadius;
        rect.top += monsterRadius;
        rect.right -= monsterRadius;
        rect.bottom -= monsterRadius;
        btn1 = { rect.right - 2 * monsterRadius, 2, rect.right - 2, rect.top - 2 };
        btn2 = { rect.right - 4 * monsterRadius - 5, 2, rect.right - 2 * monsterRadius - 7, rect.top - 2 };
        break;
    }
    case WM_TIMER:
    {
        switch (wParam)
        {
        case TIMER_MONSTER_MOVE:
        {
            if (!pts.size())
            {
                KillTimer(hWnd, TIMER_MONSTER_MOVE);
                mouseDirection = { monster.x + monsterRadius, monster.y, monster.x + monsterRadius, monster.y };
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            }
            double distance;
            LONG temp;
            std::deque<POINT>::iterator iter = pts.begin();
            distance = get_distance_square(monster, *iter);
            while (++iter != pts.end())
            {
                temp = get_distance_square(monster, *iter);
                if (temp < distance)
                {
                    std::swap<POINT>(pts.front(), *iter);
                    distance = temp;
                }
            }

            distance = sqrt(distance);
            if (distance <= monsterSpeed || distance <= beanRadius)
            {
                monster = pts.front();
                pts.erase(pts.begin());
                if (pts.size()) mouseDirection = { pts.front().x, pts.front().y, pts.front().x, pts.front().y };
            }
            else if (distance <= (double)monsterRadius + monsterSpeed + beanRadius)
            {
                double rate = monsterSpeed / (double)distance;
                double dx = pts.front().x - (double)monster.x;
                double dy = pts.front().y - (double)monster.y;
                monster.x += static_cast<LONG>(rate * dx);
                monster.y += static_cast<LONG>(rate * dy);
                distance -= monsterSpeed;                // 目前还没有移动，预先计算移动后的距离
                double sin_sita = dx / distance;
                double cos_sita = dy / distance;
                double cos_phi = beanRadius / distance;
                double sin_phi = sqrt(dx * dx + dy * dy - (double)beanRadius * beanRadius) / distance;
                double sin_alhpa = sin_phi * cos_sita - cos_phi * sin_sita;
                double cos_alpha = cos_phi * cos_sita + sin_phi * sin_sita;
                double sin_beta = sin_phi * sin_sita - cos_phi * cos_sita;
                double cos_beta = cos_phi * sin_sita + sin_phi * cos_sita;

                mouseDirection = {
                    pts.front().x + static_cast<LONG>(beanRadius * cos_beta),
                    pts.front().y - static_cast<LONG>(beanRadius * sin_beta),
                    pts.front().x - static_cast<LONG>(beanRadius * sin_alhpa),
                    pts.front().y + static_cast<LONG>(beanRadius * cos_alpha)
                };
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else
            {
                double rate = monsterSpeed / (double)distance;
                monster.x += static_cast<LONG>(rate * (pts.front().x - (double)monster.x));
                monster.y += static_cast<LONG>(rate * (pts.front().y - (double)monster.y));
                mouseDirection = { pts.front().x, pts.front().y, pts.front().x, pts.front().y };
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        }
        default:
            break;
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
        POINT pt{ LOWORD(lParam), HIWORD(lParam) };
        if (point_in_rect(pt, rect) && mode == GameMode::Put)
        {
            pts.front() = pt;
            if (pts.size() >= 2 && std::find_if(pts.begin() + 1, pts.end(),
                [](const POINT& pt)->bool { return pts.front().x == pt.x && pts.front().y == pt.y; }) != pts.end())
                break;
            pts.push_front(pts.front());
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (point_in_rect(pt, btn2))
        {
            if (mode == GameMode::Put && pts.size() > 1)
            {
                pts.erase(pts.begin());
                mode = GameMode::Eat;
                SetTimer(hWnd, TIMER_MONSTER_MOVE, 50, 0);
            }
        }
        else if (point_in_rect(pt, btn1))
        {
            if (mode == GameMode::Eat && !pts.size())
            {
                mode = GameMode::Put;
                pts.resize(1);
                pts.front() = { 2 * monsterRadius + beanRadius, 2 * monsterRadius + beanRadius };
                monster.x = monsterRadius;
                monster.y = monsterRadius;
                mouseDirection = { monsterRadius, monster.y, monsterRadius, monster.y };
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        break;
    }
    case WM_DESTROY:
        DeleteObject(hFontBtn);
        DeleteObject(hFontLabel);
        DeleteObject(hFontTip);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

