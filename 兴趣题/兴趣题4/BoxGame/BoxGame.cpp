#include <windows.h>
#include "BoxGame.h"
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")
#include <stdio.h>
// 一共10关
constexpr int NLEVEL = 10;

HINSTANCE hInst;
HWND hdlgSelect; // 选择关卡对话框句柄
HWND hLevelCombo; // 输入的关卡
HWND hwnd;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); 	//窗口函数说明
BOOL CALLBACK DlgHelpProc(HWND,UINT,WPARAM,LPARAM); // 处理帮助对话框
BOOL CALLBACK DlgSelectProc(HWND, UINT, WPARAM, LPARAM); // 处理选择关卡对话框
void IniMap(int GameMap[][9], int GameMapOri[][8][9], int level); // 初始化地图, 地图大小8行9列


//游戏地图
// 0：墙   1：路   2：人   3：终点   4：箱子   5：人在终点上   6：箱子在终点上
int GameMap[8][9] = {0}; 
int countMap[NLEVEL]= {3,3,3,4,4,5,2,7,6,6}; // 每关的箱子/终点数
int GameMapOri[NLEVEL][8][9] = {
	                {0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 1, 2, 3, 3, 3, 0, 0,
                     0, 0, 1, 1, 1, 0, 0 ,0 ,0,
                     0, 0, 0, 4, 1, 1, 1, 1, 0,
                     0, 1, 1, 1, 0, 4, 0, 1, 0,
                     0, 1, 4, 1, 0, 1, 1, 1, 0,
                     0, 1, 1, 1, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0},  // 1								
					{0, 2, 1, 1, 1, 0, 0, 0, 0,
					 0, 1, 4, 4, 0, 1, 0, 0, 0,
					 0, 1, 4, 1, 0, 1, 0, 3, 0,
					 0, 0, 0, 1, 0, 0, 0, 3, 0,
					 0, 0, 0, 1, 1, 1, 1, 3, 0,
					 0, 0, 1, 1, 1, 0, 1, 1, 0,
					 0, 0, 1, 1, 1, 0, 0, 0, 0,
					 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 2
					{0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 2, 1, 0, 0, 0, 0, 0,
                     0, 0, 1, 4, 1, 1, 0 ,0 ,0,
                     0, 0, 0, 1, 0, 1, 0, 0, 0,
                     0, 3, 0, 1, 0, 1, 1, 0, 0,
                     0, 3, 4, 1, 1, 0, 1, 0, 0,
                     0, 3, 1, 1, 1, 4, 1, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0},  // 3
					{0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 1, 1, 1, 1, 1, 0, 0,
                     0, 0, 4, 0, 0, 0, 1 ,1 ,1,
                     0, 1, 2, 1, 4, 1, 1, 4, 1,
                     0, 1, 3, 3, 0, 1, 4, 1, 0,
                     0, 0, 3, 3, 0, 1, 1, 1, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0},  // 4
					{0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 1, 1, 1, 1, 1, 0, 0, 0,
                     0, 1, 4, 4, 4, 0, 0 ,0 ,0,
                     0, 1, 1, 0, 3, 3, 0, 0, 0,
                     0, 0, 1, 1, 3, 3, 4, 1, 0,
                     0, 0, 1, 2, 1, 1, 1, 1, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0},  // 5
					{0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 1, 1, 0, 1, 2, 0,
                     0, 0, 1, 1, 1, 0, 1 ,1 ,0,
                     0, 0, 4, 1, 4, 1, 4, 1, 0,
                     0, 0, 1, 4, 0, 0, 1, 1, 0,
                     0, 0, 1, 4, 1, 0, 1, 1, 0,
                     3, 3, 3, 3, 3, 1, 1, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0},  // 6
					{0, 0, 0, 0, 0, 0, 0, 0, 0,
					 0, 0, 0, 0, 1, 1, 0, 0, 0,
					 0, 0, 0, 0, 1, 1, 0 ,0 ,0,
					 0, 0, 1, 1, 4, 4, 0, 0, 0,
					 0, 0, 1, 3, 1, 3, 1, 0, 0,
					 0, 0, 0, 0, 1, 1, 2, 0, 0,
					 0, 0, 0, 0, 0, 0, 0, 0, 0,
					 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 7
					{0, 0, 0, 1, 1, 1, 0, 0, 0,
					 0, 0, 1, 1, 4, 3, 3 ,0 ,0,
					 0, 0, 4, 1, 0, 3, 4, 3, 0,
					 0, 1, 1, 1, 0, 3, 3, 3, 0,
					 0, 1, 4, 0, 0, 0, 4, 1, 0,
					 0, 1, 1, 4, 1, 4, 1, 0, 0,
					 0, 0, 2, 1, 1, 1, 1, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0,},  // 8
					{0, 0, 0, 0, 0, 0, 0, 0, 0,
					 0, 0, 1, 0, 0, 0, 1, 1, 0,
					 0, 1, 1, 1, 0, 1, 1, 1, 0,
					 0, 0, 1, 4, 3, 3, 4, 1, 0,
					 0, 2, 1, 4, 3, 6, 1, 0, 0,
					 0, 0, 1, 4, 3, 3, 4, 1, 0,
					 0, 0, 1, 1, 0, 1, 1, 1, 0,
					 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 9
					{0, 0, 0, 0, 0, 2, 0, 0, 0,
					 0, 0, 0, 1, 1, 1, 1, 1, 0,
					 0, 0, 0, 1, 0, 1, 4 ,1 ,0,
					 3, 3, 3, 4, 0, 4, 1, 0, 0,
					 3, 3, 3, 1, 4, 1, 1, 1, 0,
					 0, 0, 1, 4, 1, 0, 0, 1, 0,
					 0, 0, 1, 4, 1, 1, 1, 1, 0,
					 0, 0, 1, 1, 1, 1, 0, 0, 0}}; //10
// x1, y1: 当前人的位置;
// x2, y2: 移动到的位置;
// x3, y3: 移动方向的下下个位置，推箱子时为箱子可能到达的位置
// x是行, y是列
int x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0; 
int mx = 0, my = 0; // 鼠标左击的位置
int level = 0; // 当前关卡数[0, NLEVEL-1]
BOOL isPlay = FALSE; // 是否正在进行游戏

//------------ 以下初始化窗口类----------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int	nCmdShow)
{
		
		MSG Msg;
		WNDCLASS wndclass;
		HACCEL hAccel; // ---加速键资源---
		char lpszMenuName[] = "GameMenu"; // ----菜单名称----
		char lpszClassName[] = "BoxGame";			
		char lpszTitle[]= "BoxGame";			

        //窗口类的定义
		wndclass.style = 0;						
		wndclass.lpfnWndProc = WndProc ;		
		wndclass.cbClsExtra	= 0 ;				
		wndclass.cbWndExtra	= 0 ;				
		wndclass.hInstance = hInstance ;		;
		wndclass.hIcon = LoadIcon(hInstance,"BoxIcon");
		wndclass.hCursor = LoadCursor(hInstance,"BoxCursor");
		wndclass.hbrBackground =(HBRUSH) CreateSolidBrush(RGB(236, 240, 77)); //窗口背景为黄色（路的颜色）
		wndclass.lpszMenuName = NULL; //lpszMenuName;			
		wndclass.lpszClassName = lpszClassName; 			

//--------------- 以下进行窗口类的注册 -----------------
	    if( !RegisterClass( &wndclass))		
		{
			MessageBeep(0) ;
			return FALSE ;
		}

        //创建窗口
		hwnd = CreateWindow(
						lpszClassName,		
						lpszTitle,				
						WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,//WS_OVERLAPPEDWINDOW,	
						CW_USEDEFAULT,
						0,			
						558,            //窗口宽
						580,			//窗口高
						NULL,					
						NULL,					
						hInstance,				
						NULL) ;		
		hInst = hInstance; //--------
		ShowWindow(hwnd, nCmdShow); 			
		UpdateWindow(hwnd); 	
		
		//-----加载加速键资源-----
		hAccel = LoadAccelerators(hInstance, lpszMenuName);      
		while(GetMessage(&Msg, NULL, 0, 0)){ 
			// 截获非模态对话框消息并发往处理函数
			if(!IsDialogMessage(hdlgSelect, &Msg)){
				// 在消息循环中截获加速键消息
				if (!TranslateAccelerator(hwnd,hAccel,&Msg)){
					TranslateMessage(&Msg) ;
					DispatchMessage(&Msg) ;
				} 
			}
		}

		return Msg.wParam;      
}
// 窗口函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM  wParam, LPARAM lParam)
{
	HDC hDC;
	HDC hdcmem;			//内存设备环境句柄	
	HBITMAP hBm;		//位图句柄
	BITMAP bm;			//BITMAP结构
	PAINTSTRUCT PtStr;
	HBRUSH hBrush;
	HMENU hMenu;
	int count;
	HWND hToolbar;
	TBBUTTON tbb[4];
	LPNMHDR lpnmhdr;
	LPTOOLTIPTEXT lpttext;
	switch(message){	
		case WM_CREATE:
			if(isPlay){		// 正在玩游戏		
				HINSTANCE hThisInstance = GetModuleHandle(NULL);
                // 初始化地图
				IniMap(GameMap, GameMapOri, level);
				// 加载菜单
				hMenu = LoadMenu(hThisInstance, "GAMEMENU");
				SetMenu(hwnd, hMenu);				
				// 创建工具条图像列表	
				HIMAGELIST  hImgList = ImageList_Create(15, 15, ILC_COLOR32|ILC_MASK, 0, 0); 
				// 打开位图，将其加进图像列表
				int ImgID[4];
				HBITMAP hBitmap = NULL;
				hBitmap = LoadBitmap(hThisInstance, MAKEINTRESOURCE(IDB_RESTART));
				ImgID[0] = ImageList_Add(hImgList, hBitmap, NULL);
				hBitmap = LoadBitmap(hThisInstance, MAKEINTRESOURCE(IDB_NEXT));
				ImgID[1] = ImageList_Add(hImgList, hBitmap, NULL);
				hBitmap = LoadBitmap(hThisInstance, MAKEINTRESOURCE(IDB_PREV));
				ImgID[2] = ImageList_Add(hImgList, hBitmap, NULL);
				hBitmap = LoadBitmap(hThisInstance, MAKEINTRESOURCE(IDB_SET));
				ImgID[3] = ImageList_Add(hImgList, hBitmap, NULL);				
				// 创建工具条
				hToolbar = CreateWindowEx(0,TOOLBARCLASSNAME,NULL,WS_CHILD|WS_VISIBLE|TBSTYLE_TOOLTIPS,0,0,0,0,hwnd,(HMENU)IDR_TOOLBAR,hThisInstance,NULL);
				SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
				SendMessage(hToolbar, TB_SETIMAGELIST, 0, (LPARAM)hImgList); // 为工具栏设置图像列表				
				ZeroMemory(tbb, sizeof(tbb));
				tbb[0].iBitmap = ImgID[0];
				tbb[0].fsState = TBSTATE_ENABLED;
				tbb[0].fsStyle = TBSTYLE_BUTTON;
				tbb[0].idCommand = IDM_RESTART;
				tbb[1].iBitmap = ImgID[1];
				tbb[1].fsState = TBSTATE_ENABLED;
				tbb[1].fsStyle = TBSTYLE_BUTTON;
				tbb[1].idCommand = IDM_NEXT;
				tbb[2].iBitmap = ImgID[2];
				tbb[2].fsState = TBSTATE_ENABLED;
				tbb[2].fsStyle = TBSTYLE_BUTTON;
				tbb[2].idCommand = IDM_PREV;
				tbb[3].iBitmap = ImgID[3];
				tbb[3].fsState = TBSTATE_ENABLED;
				tbb[3].fsStyle = TBSTYLE_BUTTON;
				tbb[3].idCommand = IDM_SELECT;
				SendMessage(hToolbar,TB_ADDBUTTONS,sizeof(tbb)/sizeof(TBBUTTON),(LPARAM)&tbb);	
			}		
			break;
		case WM_NOTIFY:
			// 鼠标移动到工具条上方时，显示提示信息
			lpnmhdr = (LPNMHDR)lParam;
			if(lpnmhdr->code==TTN_GETDISPINFO){
				lpttext = (LPTOOLTIPTEXT)lParam;
					switch(lpttext->hdr.idFrom){
					case IDM_RESTART:
						lpttext->lpszText = "重新开始";
						break;
					case IDM_NEXT:
						lpttext->lpszText = "下一关";
						break;
					case IDM_PREV:
						lpttext->lpszText = "上一关";
						break;
					case IDM_SELECT:
						lpttext->lpszText = "选择关卡";
						break;
				}
			}
			break;
		//处理菜单消息	   
   		case WM_COMMAND:
			switch(LOWORD(wParam)){
			case IDM_RESTART:
				// 重新开始游戏
				IniMap(GameMap, GameMapOri, level); // 初始化地图
				InvalidateRect(hwnd,NULL,1); // 绘制地图
				break;
			case IDM_NEXT:
				// 下一关
				level++;
				if(level>NLEVEL-1){
					MessageBox(hwnd, "没有下一关!", "提醒", MB_OK);
					level--;
				}else{
					IniMap(GameMap, GameMapOri, level);
					InvalidateRect(hwnd,NULL,1);
				}
				break;
			case IDM_PREV:
				// 上一关
				level--;
				if (level<0){
					MessageBox(hwnd, "没有上一关!", "提醒", MB_OK);
					level++;
				}else{
					IniMap(GameMap, GameMapOri, level);
					InvalidateRect(hwnd,NULL,1);
				}
				break;
			case IDM_SELECT:
				// 选择“选择关卡”菜单时，弹出非模式对话框，选择关卡
				hdlgSelect = CreateDialog(hInst, "DLGSELECT", hwnd, (DLGPROC)DlgSelectProc);
				break;
		    case IDM_EXIT:
			  	SendMessage(hwnd, WM_DESTROY, 0, 0); // 选择“退出”菜单项时，向应用程序发出WM_DESTROY消息
        		break;
			case IDM_HELP:
				// 选择“帮助”菜单时，弹出模式对话框，介绍游戏操作方法
				DialogBox(hInst,"DLGHELP", hwnd, (DLGPROC)DlgHelpProc);
				break;
		 	}
	        break;	  

	//当前地图的绘制, 每一块的大小为 60*60, 整块地图 8*9
	// 0：墙   1：路   2：人   3：终点   4：箱子   5：人在终点上   6：箱子在终点上
	case WM_PAINT:
		if(isPlay){
			hDC = BeginPaint(hwnd, &PtStr);
			SetViewportOrgEx(hDC,0,30,NULL); // 设置视口原点
			count = 0;

			for(int i=0; i<8; i++)
			{
				for(int j=0; j<9; j++)
				{
					int top = i * 60;
                    int left = j * 60;

					if(GameMap[i][j] == 0)  //墙为灰色
					{
						hBrush = CreateSolidBrush(RGB(88, 88, 88)); 
						SelectObject(hDC, hBrush);  			
						RoundRect(hDC, left, top, left + 60, top + 60, 5, 5); 
						DeleteObject(hBrush);
					}
					if(GameMap[i][j] == 2 || GameMap[i][j] == 5)  //人为蓝色
					{
						hBrush = CreateSolidBrush(RGB(0, 162, 232)); 
						SelectObject(hDC, hBrush);  			
						RoundRect(hDC, left, top, left + 60, top + 60, 50, 50); //50是圆角的直径
						DeleteObject(hBrush);
						x1 = i;
						y1 = j;
					}
					if(GameMap[i][j] == 3)  //终点为红色
					{
						hBrush = CreateSolidBrush(RGB(237, 28, 36)); 
						SelectObject(hDC, hBrush);  			
						RoundRect(hDC, left, top, left + 60, top + 60, 5, 5); 
						DeleteObject(hBrush);
					}
					if(GameMap[i][j] == 4 || GameMap[i][j] == 6)   //箱子为橙色
					{
						hBrush = CreateSolidBrush(RGB(255, 127, 39)); 
						SelectObject(hDC, hBrush);  			
						RoundRect(hDC, left, top, left + 60, top + 60, 5, 5); 
						DeleteObject(hBrush);
						if(GameMap[i][j] == 6)//如果箱子在终点
							count++;
					}
				}
			}

			EndPaint(hwnd, &PtStr);

			//判断游戏是否胜利
			if(count == countMap[level]){
				MessageBox(NULL, TEXT("恭喜胜利啦!"), TEXT("成功"), MB_OK);
				// 重新开始游戏
				level++;
				if(level < NLEVEL){
					IniMap(GameMap, GameMapOri, level);
					count = 0;
					InvalidateRect(hwnd,NULL,1);
				} else {
					MessageBox(NULL,TEXT("游戏结束了o(╯□╰)o"),TEXT("游戏结束"), MB_OK);
					level--;
				}

			}
		}
		else{
			// 加载位图，游戏开始之前载入欢迎画面
			hBm = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_WELCOME));
			GetObject(hBm, sizeof(BITMAP), (LPVOID)&bm);
			hDC = BeginPaint(hwnd, &PtStr);
			hdcmem = CreateCompatibleDC(hDC);
			SelectObject(hdcmem,hBm); 
			RECT rect;
			GetClientRect(hwnd,&rect);
			StretchBlt(hDC,0,0,rect.right,rect.bottom,hdcmem,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
			SetStretchBltMode(hdcmem,STRETCH_HALFTONE);
			EndPaint(hwnd,&PtStr);
		}
		break;
	//鼠标消息
	case WM_LBUTTONDOWN:
		if(isPlay){
            //利用鼠标左键进行移动
			//读取鼠标左击的位置
			my = LOWORD(lParam) / 60; 
			mx = (HIWORD(lParam)-30) / 60; // 因为工具栏占了30距离，所以在这里把它减掉 
			//如果鼠标左击位置在当前人的位置的相邻块区，则移动；否则不动
			if(mx == x1 && my == y1 - 1)
			{
				x2 = x1;
				y2 = y1 - 1;
				x3 = x1;
				y3 = y1 - 2;
			}
			else if(mx == x1 && my == y1 + 1)
			{
				x2 = x1;
				y2 = y1 + 1;
				x3 = x1;
				y3 = y1 + 2;
			}
			else if(mx == x1 - 1 && my == y1)
			{
				x2 = x1 - 1;
				y2 = y1;
				x3 = x1 - 2;
				y3 = y1;
			}
			else if(mx == x1 + 1 && my == y1)
			{
				x2 = x1 + 1;
				y2 = y1;
				x3 = x1 + 2;
				y3 = y1;
			}
			else
			{
				return 0;
			}
			//如果要移动到的位置是路或终点
			if(GameMap[x2][y2] == 1 || GameMap[x2][y2] == 3)
			{
				if(GameMap[x2][y2] == 1) //移动到路
					GameMap[x2][y2] = 2; //路变成人
				else if(GameMap[x2][y2] == 3) //移动到终点
					GameMap[x2][y2] = 5; //人在终点上
				if(GameMap[x1][y1] == 2) //移动前人在路上
					GameMap[x1][y1] = 1; //移动前的位置变为路
				else if(GameMap[x1][y1] == 5) //移动前人在终点
					GameMap[x1][y1] = 3; //移动前位置变终点
				InvalidateRect(hwnd, NULL, 1);
			}
			//如果要移动到的位置是箱子，进一步考虑箱子前面的块区
			if(GameMap[x2][y2] == 4 || GameMap[x2][y2] == 6)
			{
				if(GameMap[x3][y3] == 1 || GameMap[x3][y3] == 3)//箱子前是路或者终点
				{
					if(GameMap[x3][y3] == 1) //箱子前是路
						GameMap[x3][y3] = 4;
					else if(GameMap[x3][y3] == 3) //箱子前是终点
						GameMap[x3][y3] = 6;

					if(GameMap[x2][y2] == 4) //箱子之前不在终点
						GameMap[x2][y2] = 2;  //人
					else if(GameMap[x2][y2] == 6) //箱子之前在终点
						GameMap[x2][y2] = 5; //人在终点

					if(GameMap[x1][y1] == 2) //人之前不在终点
						GameMap[x1][y1] = 1;  //人变路
					else if(GameMap[x1][y1] == 5) //人之前在终点
						GameMap[x1][y1] = 3; //人变终点
					InvalidateRect(hwnd, NULL, 1);
				}
			}
		}else{
			// 点击屏幕任意位置开始游戏
			isPlay = TRUE;								
			SendMessage(hwnd,WM_CREATE,0,0);
			InvalidateRect(hwnd,NULL,1);
		}		
		return 0;
	
	//键盘按下消息--------------------------------
	case WM_KEYDOWN:
        //用键盘操作
		switch(wParam)
		{
		case 'A':
		case VK_LEFT:   //向左键按下
			x2 = x1;
			y2 = y1 - 1;
			x3 = x1;
			y3 = y1 - 2;
			break;
		case 'D':
		case VK_RIGHT:  //向右键按下
			x2 = x1;
			y2 = y1 + 1;
			x3 = x1;
			y3 = y1 + 2;
			break;
		case 'W':
		case VK_UP:     //向上键按下
			x2 = x1 - 1;
			y2 = y1;
			x3 = x1 - 2;
			y3 = y1;
			break;
		case 'X':
		case VK_DOWN:   //向下键按下
			x2 = x1 + 1;
			y2 = y1;
			x3 = x1 + 2;
			y3 = y1;
			break;
		}
		
		//如果要移动到的位置是路或终点
		if(GameMap[x2][y2] == 1 || GameMap[x2][y2] == 3)
		{
			if(GameMap[x2][y2] == 1)
				GameMap[x2][y2] = 2;
			else if(GameMap[x2][y2] == 3)
				GameMap[x2][y2] = 5;
			if(GameMap[x1][y1] == 2)
				GameMap[x1][y1] = 1;
			else if(GameMap[x1][y1] == 5)
				GameMap[x1][y1] = 3;
			InvalidateRect(hwnd, NULL, 1);
		}
		//如果要移动到的位置是箱子，进一步考虑箱子前面的块区
		if(GameMap[x2][y2] == 4 || GameMap[x2][y2] == 6)
		{
			if(GameMap[x3][y3] == 1 || GameMap[x3][y3] == 3)
			{
				if(GameMap[x3][y3] == 1)
					GameMap[x3][y3] = 4;
				else if(GameMap[x3][y3] == 3)
					GameMap[x3][y3] = 6;
				if(GameMap[x2][y2] == 4)
					GameMap[x2][y2] = 2;
				else if(GameMap[x2][y2] == 6)
					GameMap[x2][y2] = 5;
				if(GameMap[x1][y1] == 2)
					GameMap[x1][y1] = 1;
				else if(GameMap[x1][y1] == 5)
					GameMap[x1][y1] = 3;
				InvalidateRect(hwnd, NULL, 1);
			}
		}
			
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);					//调用PostQuitMessage发出WM_QUIT消息

	default:								//缺省时采用系统消息缺省处理函数
		return  DefWindowProc(hwnd,message,wParam,lParam);
	}
	return (0);
}
// 帮助对话框处理程序
BOOL CALLBACK DlgHelpProc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
     	//初始化对话框 
     	case WM_INITDIALOG:
	    	return 1;
	 	//处理对话框消息 
	 	case WM_COMMAND:
    		switch (LOWORD(wParam)){
		   		case IDOK:              //关闭对话框  
					EndDialog(hdlg, 0) ;
					return 1;
				}
				break ;
		case WM_CLOSE:
			EndDialog(hdlg, 0) ;
			return 1;
    }
	return 0;
}

//用下拉选项对话框选择关卡
BOOL CALLBACK DlgSelectProc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
        //初始化消息框 
        case WM_INITDIALOG:
		{
			// 下拉选项框
			hLevelCombo = CreateWindow("combobox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, 35, 70, 500, hdlg, (HMENU)IDC_COMBO, hInst, NULL);
			// 添加NLEVEL个关卡数
			for (int i = 1; i <= NLEVEL; i++) {
				char temp[3] { 0 };
				_itoa_s(i, temp, 3, 10);            // 10是10进制
				SendMessage(hLevelCombo, CB_ADDSTRING, 0, (LPARAM)temp);
			}
			char buff[3]{ 0 };
			_itoa_s(level + 1, buff, 3, 10);
			SendMessage(hLevelCombo, CB_SELECTSTRING, -1, (LPARAM)buff);
			return 0;
		}
	    //处理消息框消息 
	    case WM_COMMAND:
	        switch (LOWORD(wParam)){
		        case IDOK:
					//获得选择的关卡 
					level = SendMessage(hLevelCombo, CB_GETCURSEL, 0, 0); // 获得编号从0开始
			       	DestroyWindow(hdlg);	
					IniMap(GameMap, GameMapOri, level);
					InvalidateRect(hwnd,NULL,1);
		           	return 1 ;
       	    }
			break;						
		case WM_CLOSE:
			DestroyWindow( hdlg) ;
        return 1; 							
	}
    return 0;    
}

// 初始化当前地图为所选关卡的地图
void IniMap(int GameMap[][9], int GameMapOri[][8][9], int level){
	for (int i=0; i<8; i++){
		for (int j=0; j<9; j++){
			GameMap[i][j] = GameMapOri[level][i][j];
		}
	}
	x1 = 0; y1 = 0; x2 = 0; y2 = 0; x3 = 0; y3 = 0; // 初始化坐标
	mx = 0; my = 0; // 初始化mx和my
}