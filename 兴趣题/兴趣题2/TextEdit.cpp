#include <windows.h>
#include <string>
#include <sstream>

long WINAPI WndProc(HWND hWnd, UINT iMessage, UINT wParam, LONG lParam);
BOOL InitWindowsClass(HINSTANCE hInstance);
BOOL InitWindows(HINSTANCE hInstance, int nCmdShow);

#undef max
#undef min

//主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG Message;
	if (!InitWindowsClass(hInstance))
		return FALSE;
	if (!InitWindows(hInstance, nCmdShow))
		return FALSE;
	while (GetMessage(&Message, 0, 0, 0))//消息循环
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

//消息处理函数
long WINAPI WndProc(HWND hWnd, UINT iMessage, UINT wParam, LONG lParam)
{
#define BufSize 30				//设置存放字符的缓冲区大小
	static DWORD dwCharSet = DEFAULT_CHARSET;
	static HFONT font;
	static char cCharBuf[BufSize];	//设置静态字符数组，存放输入的字符，字符个数不能超出缓冲区大小
	static int nNumChar = 0;		//现有字符个数
	static int nArrayPos = 0;		//字符的位置
	static int nLnHeight;
	static int nCharWidth;
	static bool nCtrlDown = false, nAltDown = false, nShiftDown = false, outRight = false;
	static POINT star[11];
	static unsigned char gbkCode = 0;
	static std::string msg;
	static SCROLLINFO sbi;
	int x, n;
	HDC hDC;
	TEXTMETRIC tm;
	PAINTSTRUCT PtStr; //定义指向包含绘图信息的结构体变量
	switch (iMessage)  //处理消息
	{
	case WM_CHAR:				//遇到非系统字符所作的处理
	{
		if (wParam == VK_BACK)  //处理“遇到回退键的消息”
		{
			if (nArrayPos == 0) //如果已经在一行文字的开始处，则提示用户"不能回退"
				MessageBox(hWnd, "当前位置是文本的起始位置，不能回退", NULL, MB_OK);
			else
			{
				n = isascii(cCharBuf[nArrayPos - 1]) ? 1 : 2;   // GBK编码中两个字节表示一个汉字
				nArrayPos -= n; //每按一次回退键就回退一个字符的位置
				nNumChar -= n;	//对现有字符总数进行计数
				for (x = nArrayPos; x < nNumChar; ++x)
					cCharBuf[x] = cCharBuf[x + n];
				outRight = false;
				InvalidateRect(hWnd, NULL, TRUE);	//刷新用户区，并向应用程序发送WM_PAINT消息
			}
			break;
		}
		if (wParam == VK_ESCAPE)                     //处理按下Escape键消息
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);       // 关闭窗口
			break;
		}
		if (!isascii(wParam))
		{
			if (outRight) { break; }
			if (gbkCode)
			{
				if (nNumChar + 2 > BufSize)
				{
					gbkCode = 0, outRight = true, MessageBox(hWnd, "缓冲区已满，不能再输入字符了\n若需要删除字符，请用BackSpace键", NULL, MB_OK);
					break;
				}
				else
				{
					for (x = nNumChar + 1; x > nArrayPos; --x)
						cCharBuf[x] = cCharBuf[x - 2];
					cCharBuf[nArrayPos] = gbkCode;
					cCharBuf[nArrayPos + 1] = (unsigned char)wParam;
					gbkCode = 0;
					nNumChar += 2;
					nArrayPos += 2;
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				}
			}
			else
			{
				gbkCode = (unsigned char)wParam;
				break;
			}
		}
		if (nNumChar >= BufSize) //如果写入的字符数超过缓冲区大小，则报警
		{
			if (!outRight) outRight = true, MessageBox(hWnd, "缓冲区已满，不能再输入字符了\n若需要删除字符，请用BackSpace键", NULL, MB_OK);
			break;
		}
		outRight = false;
		for (x = nNumChar; x > nArrayPos; x = x - 1)
			cCharBuf[x] = cCharBuf[x - 1];
		cCharBuf[nArrayPos] = (unsigned char)wParam;
		++nArrayPos;
		++nNumChar;
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	case WM_INPUTLANGCHANGE:
		dwCharSet = wParam;

	case WM_CREATE:      //处理窗口创建消息
	{
		sbi.cbSize = sizeof(sbi);
		sbi.fMask = SIF_ALL;
		sbi.nMin = 0;
		sbi.nMax = 0;
		sbi.nPage = 10;
		sbi.nPos = 0;
		SetScrollInfo(hWnd, SB_HORZ, &sbi, TRUE);

		hDC = GetDC(hWnd);
		font = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL);
		SelectObject(hDC, font);
		GetTextMetrics(hDC, &tm);  //获取字体信息
		nLnHeight = tm.tmHeight + tm.tmExternalLeading;
		nCharWidth = tm.tmAveCharWidth;
		SelectObject(hDC, GetStockObject(SYSTEM_FONT));
		ReleaseDC(hWnd, hDC);

#undef min
		static constexpr double PI = 3.14159265358979323846;
		RECT rect;
		GetClientRect(hWnd, &rect);
		const long ox = (rect.right - rect.left) / 2, oy = (rect.bottom - rect.top) / 2;
		const long r1 = 2 * std::min(ox, oy) / 3, r2 = r1 * sin(PI / 10) / sin(3 * PI / 10);
		POINT pt[11]{
			ox, oy - r1,
			ox - r2 * sin(PI / 5), oy - r2 * cos(PI / 5),
			ox - r1 * cos(PI / 10), oy - r1 * sin(PI / 10),
			ox - r2 * cos(PI / 10), oy + r2 * sin(PI / 10),
			ox - r1 * sin(PI / 5), oy + r1 * cos(PI / 5),
			ox, oy + r2,
			ox + r1 * sin(PI / 5), oy + r1 * cos(PI / 5),
			ox + r2 * cos(PI / 10), oy + r2 * sin(PI / 10),
			ox + r1 * cos(PI / 10), oy - r1 * sin(PI / 10),
			ox + r2 * sin(PI / 5), oy - r2 * cos(PI / 5),
			rect.right - rect.left, rect.bottom - rect.top
		};
		for (int i = 0; i < 11; ++i)
			star[i] = pt[i];
	}
	break;
	case WM_SETFOCUS:
		CreateCaret(hWnd, NULL, 1, nLnHeight);
		SetCaretPos((nArrayPos + 1) * nCharWidth, nLnHeight);
		ShowCaret(hWnd);
		break;
	case WM_KILLFOCUS:
		HideCaret(hWnd);
		DestroyCaret();
		nShiftDown = nCtrlDown = nAltDown = false;
		break;
	case WM_SYSKEYDOWN:
		nAltDown = true;
		break;
	case WM_SYSKEYUP:
		nAltDown = false;
		break;
	case WM_KEYDOWN:     //处理按下键消息
	{
		switch (wParam)
		{
		case VK_END: //处理按下键为End时的消息
			nArrayPos = nNumChar;	//输入位置从本行的末尾开始
			SetCaretPos((nArrayPos + 1) * nCharWidth, nLnHeight);
			break;
		case VK_HOME:			//处理按下键为Home时的消息
			nArrayPos = 0;		//输入位置为本行的起始位置
			SetCaretPos((nArrayPos + 1) * nCharWidth, nLnHeight);
			break;
		case VK_DELETE:			          //处理按下键为Delete时的消息
			if (nArrayPos == nNumChar)       //输入位置处于本行的末尾
				MessageBox(hWnd, "缓冲区已空，没有字符可供删除", NULL, MB_OK);
			else
			{
				n = isascii(cCharBuf[nArrayPos]) ? 1 : 2;
				for (x = nArrayPos; x < nNumChar; x = x + n)
					cCharBuf[x] = cCharBuf[x + n];	//每删除一个字符，缓冲区中总字符数减1
				nNumChar = nNumChar - n;
				outRight = false;
				SetCaretPos((nArrayPos + 1) * nCharWidth, nLnHeight);
				InvalidateRect(hWnd, NULL, TRUE); //用户区刷新
			}
			break;
		case VK_LEFT: //处理按下左方向键时的消息
			if (nShiftDown && nCtrlDown)
			{
				if (star[2].x == 0) break;
				for (int i = 0; i < 10; i++)
					--(star[i].x);
				InvalidateRect(hWnd, NULL, TRUE); //用户区刷新
				break;
			}
			if (nArrayPos > 0)
				nArrayPos -= isascii(cCharBuf[nArrayPos - 1]) ? 1 : 2;  //当前输入位置往前移一个位置，再输入字符时，等于插入字符
			else	//已经移到起始输入位置，不能再往前了
				MessageBox(hWnd, "您已经移动到起始位置，不能再往左移动了", NULL, MB_OK);
			SetCaretPos((nArrayPos + 1) * nCharWidth, nLnHeight);
			break;
		case VK_RIGHT://处理按下右方向键时的消息
			if (nShiftDown && nCtrlDown)
			{
				if (star[8].x == star[10].x) break;
				for (int i = 0; i < 10; i++)
					++(star[i].x);
				InvalidateRect(hWnd, NULL, TRUE); //用户区刷新
				break;
			}
			if (nArrayPos < nNumChar)	//如果当前位置没有到缓冲区的最后位置，还能向右移动	
				nArrayPos += isascii(cCharBuf[nArrayPos]) ? 1 : 2;
			else
				MessageBox(hWnd, "已经到缓冲区的末尾,不能再向右移动了", NULL, MB_OK);
			SetCaretPos((nArrayPos + 1) * nCharWidth, nLnHeight);
			break;
		case VK_UP:
			if (nShiftDown && nCtrlDown && star[0].y)
			{
				for (int i = 0; i < 10; i++)
					--(star[i].y);
				InvalidateRect(hWnd, NULL, TRUE); //用户区刷新
				break;
			}
			break;
		case VK_DOWN:
			if (nShiftDown && nCtrlDown && star[4].y != star[10].y)
			{
				for (int i = 0; i < 10; i++)
					++(star[i].y);
				InvalidateRect(hWnd, NULL, TRUE); //用户区刷新
				break;
			}
			break;
		case VK_CONTROL:
			nCtrlDown = true;
			break;
		case VK_SHIFT:		//当按shift键时，变量置为真
			nShiftDown = true;
			break;
		case '1':
			if (!nCtrlDown || !nAltDown) break;
			if (nLnHeight * 4 > star[10].y)
			{
				MessageBox(hWnd, "高度限制, 不能再放大了!", "提示", 0);
				return TRUE;
			}
			nLnHeight *= 2;
			nCharWidth *= 2;
			DeleteObject(font);
			font = CreateFont(nLnHeight, nCharWidth, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL);
			HideCaret(hWnd);
			DestroyCaret();
			CreateCaret(hWnd, NULL, 1, nLnHeight);
			ShowCaret(hWnd);
			InvalidateRect(hWnd, NULL, TRUE);
			return TRUE;
		case VK_F1:
			MessageBox(hWnd, "按下Ctrl+ALT+1可将字符串大小放大一倍;\n按下Ctrl+Shift+(上下左右键), 五角星会上下左右移动.", "帮助", 0);
			return TRUE;
		}
	}
	break;
	case WM_KEYUP:
		if (wParam == VK_CONTROL)
			nCtrlDown = false;
		else if (wParam == VK_SHIFT)
			nShiftDown = false;
		break;
	case WM_PAINT: //处理重画消息
	{
		auto width = nCharWidth * (nNumChar + 3);
		sbi.nMax = width > star[10].x? 100:0;
		HideCaret(hWnd);
		hDC = BeginPaint(hWnd, &PtStr);
		TextOut(hDC, 0, 0, msg.c_str(), msg.length());
		auto hBrushOld = SelectObject(hDC, CreateSolidBrush(0XFF));
		SelectObject(hDC, font);
		Polygon(hDC, star, 10);
		DeleteObject(SelectObject(hDC, hBrushOld));
		SetBkMode(hDC, TRANSPARENT);                  // 文字的背景透明
		TextOut(hDC, sbi.nMax ? (nCharWidth - (sbi.nPos - sbi.nMin) * (width - star[10].x) / (sbi.nMax - sbi.nMin)) : (nCharWidth), nLnHeight, cCharBuf, nNumChar); //输出缓冲区中的文本
		SelectObject(hDC, GetStockObject(SYSTEM_FONT));
		ReleaseDC(hWnd, hDC);
		EndPaint(hWnd, &PtStr);
		SetCaretPos(sbi.nMax ? ((nArrayPos + 1) * nCharWidth - (sbi.nPos - sbi.nMin) * (width - star[10].x) / (sbi.nMax - sbi.nMin)) : ((nArrayPos + 1) * nCharWidth), nLnHeight);
		ShowCaret(hWnd);
		SetScrollInfo(hWnd, SB_HORZ, &sbi, TRUE);
	}
	break;
	case WM_MOUSEMOVE:
		hDC = GetDC(hWnd);
		msg = (std::ostringstream() << " X = " << LOWORD(lParam) << ", Y = " << HIWORD(lParam) << "       ").str();
		TextOut(hDC, 0, 0, msg.c_str(), msg.length());
		ReleaseDC(hWnd, hDC);
		break;
	case WM_DESTROY: //处理结束应用程序消息
		DeleteObject(font);
		PostQuitMessage(0);//结束应用程序
		break;
	case WM_HSCROLL:
		sbi.cbSize = sizeof(sbi);
		sbi.fMask = SIF_ALL;
		if (!lParam)
			GetScrollInfo(hWnd, SB_HORZ, &sbi);
		else
			GetScrollInfo(HWND(lParam), SB_CTL, &sbi);
		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			sbi.nPos -= 1;
			break;
		case SB_LINERIGHT:
			sbi.nPos += 1;
			break;
		case SB_THUMBPOSITION:
			sbi.nPos = sbi.nTrackPos;
			break;
		case SB_PAGELEFT:
			sbi.nPos -= 10;
			break;
		case SB_PAGERIGHT:
			sbi.nPos += 10;
			break;
		default:
			break;
		}
		sbi.nPos = std::max(sbi.nPos, sbi.nMin);
		sbi.nPos = std::min(sbi.nPos, sbi.nMax);
		if (sbi.nPos != GetScrollPos(hWnd, SB_HORZ))
		{
			SetScrollInfo(hWnd, SB_HORZ, &sbi, TRUE);
			InvalidateRect(hWnd, NULL, TRUE);   //发送WM_PAINT消息，更新窗口
		}
		break;
	case WM_SIZE:
		star[10].x = LOWORD(lParam);
		star[10].y = HIWORD(lParam);
		break;
	default://其他消息处理程序
		return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	}
	return 0;
}

BOOL InitWindowsClass(HINSTANCE hInstance)//初始化窗口类
{
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
	WndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	WndClass.hIcon = LoadIcon(NULL, "END");
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = "WinKeyboard";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	return RegisterClass(&WndClass);
}

BOOL InitWindows(HINSTANCE hInstance, int nCmdShow)//初始化窗口
{
	HWND hWnd;
	hWnd = CreateWindow("WinKeyboard",  //生成窗口
		"键盘操作例程",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX | WS_HSCROLL,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (!hWnd)
		return FALSE;
	ShowWindow(hWnd, nCmdShow);	//显示窗口
	UpdateWindow(hWnd);			//绘制用户区
	return TRUE;
}
