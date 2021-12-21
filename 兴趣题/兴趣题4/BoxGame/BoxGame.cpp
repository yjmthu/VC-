#include <windows.h>
#include "BoxGame.h"
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")
#include <stdio.h>
// һ��10��
constexpr int NLEVEL = 10;

HINSTANCE hInst;
HWND hdlgSelect; // ѡ��ؿ��Ի�����
HWND hLevelCombo; // ����Ĺؿ�
HWND hwnd;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); 	//���ں���˵��
BOOL CALLBACK DlgHelpProc(HWND,UINT,WPARAM,LPARAM); // ��������Ի���
BOOL CALLBACK DlgSelectProc(HWND, UINT, WPARAM, LPARAM); // ����ѡ��ؿ��Ի���
void IniMap(int GameMap[][9], int GameMapOri[][8][9], int level); // ��ʼ����ͼ, ��ͼ��С8��9��


//��Ϸ��ͼ
// 0��ǽ   1��·   2����   3���յ�   4������   5�������յ���   6���������յ���
int GameMap[8][9] = {0}; 
int countMap[NLEVEL]= {3,3,3,4,4,5,2,7,6,6}; // ÿ�ص�����/�յ���
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
// x1, y1: ��ǰ�˵�λ��;
// x2, y2: �ƶ�����λ��;
// x3, y3: �ƶ���������¸�λ�ã�������ʱΪ���ӿ��ܵ����λ��
// x����, y����
int x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0; 
int mx = 0, my = 0; // ��������λ��
int level = 0; // ��ǰ�ؿ���[0, NLEVEL-1]
BOOL isPlay = FALSE; // �Ƿ����ڽ�����Ϸ

//------------ ���³�ʼ��������----------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int	nCmdShow)
{
		
		MSG Msg;
		WNDCLASS wndclass;
		HACCEL hAccel; // ---���ټ���Դ---
		char lpszMenuName[] = "GameMenu"; // ----�˵�����----
		char lpszClassName[] = "BoxGame";			
		char lpszTitle[]= "BoxGame";			

        //������Ķ���
		wndclass.style = 0;						
		wndclass.lpfnWndProc = WndProc ;		
		wndclass.cbClsExtra	= 0 ;				
		wndclass.cbWndExtra	= 0 ;				
		wndclass.hInstance = hInstance ;		;
		wndclass.hIcon = LoadIcon(hInstance,"BoxIcon");
		wndclass.hCursor = LoadCursor(hInstance,"BoxCursor");
		wndclass.hbrBackground =(HBRUSH) CreateSolidBrush(RGB(236, 240, 77)); //���ڱ���Ϊ��ɫ��·����ɫ��
		wndclass.lpszMenuName = NULL; //lpszMenuName;			
		wndclass.lpszClassName = lpszClassName; 			

//--------------- ���½��д������ע�� -----------------
	    if( !RegisterClass( &wndclass))		
		{
			MessageBeep(0) ;
			return FALSE ;
		}

        //��������
		hwnd = CreateWindow(
						lpszClassName,		
						lpszTitle,				
						WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,//WS_OVERLAPPEDWINDOW,	
						CW_USEDEFAULT,
						0,			
						558,            //���ڿ�
						580,			//���ڸ�
						NULL,					
						NULL,					
						hInstance,				
						NULL) ;		
		hInst = hInstance; //--------
		ShowWindow(hwnd, nCmdShow); 			
		UpdateWindow(hwnd); 	
		
		//-----���ؼ��ټ���Դ-----
		hAccel = LoadAccelerators(hInstance, lpszMenuName);      
		while(GetMessage(&Msg, NULL, 0, 0)){ 
			// �ػ��ģ̬�Ի�����Ϣ������������
			if(!IsDialogMessage(hdlgSelect, &Msg)){
				// ����Ϣѭ���нػ���ټ���Ϣ
				if (!TranslateAccelerator(hwnd,hAccel,&Msg)){
					TranslateMessage(&Msg) ;
					DispatchMessage(&Msg) ;
				} 
			}
		}

		return Msg.wParam;      
}
// ���ں���
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM  wParam, LPARAM lParam)
{
	HDC hDC;
	HDC hdcmem;			//�ڴ��豸�������	
	HBITMAP hBm;		//λͼ���
	BITMAP bm;			//BITMAP�ṹ
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
			if(isPlay){		// ��������Ϸ		
				HINSTANCE hThisInstance = GetModuleHandle(NULL);
                // ��ʼ����ͼ
				IniMap(GameMap, GameMapOri, level);
				// ���ز˵�
				hMenu = LoadMenu(hThisInstance, "GAMEMENU");
				SetMenu(hwnd, hMenu);				
				// ����������ͼ���б�	
				HIMAGELIST  hImgList = ImageList_Create(15, 15, ILC_COLOR32|ILC_MASK, 0, 0); 
				// ��λͼ������ӽ�ͼ���б�
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
				// ����������
				hToolbar = CreateWindowEx(0,TOOLBARCLASSNAME,NULL,WS_CHILD|WS_VISIBLE|TBSTYLE_TOOLTIPS,0,0,0,0,hwnd,(HMENU)IDR_TOOLBAR,hThisInstance,NULL);
				SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
				SendMessage(hToolbar, TB_SETIMAGELIST, 0, (LPARAM)hImgList); // Ϊ����������ͼ���б�				
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
			// ����ƶ����������Ϸ�ʱ����ʾ��ʾ��Ϣ
			lpnmhdr = (LPNMHDR)lParam;
			if(lpnmhdr->code==TTN_GETDISPINFO){
				lpttext = (LPTOOLTIPTEXT)lParam;
					switch(lpttext->hdr.idFrom){
					case IDM_RESTART:
						lpttext->lpszText = "���¿�ʼ";
						break;
					case IDM_NEXT:
						lpttext->lpszText = "��һ��";
						break;
					case IDM_PREV:
						lpttext->lpszText = "��һ��";
						break;
					case IDM_SELECT:
						lpttext->lpszText = "ѡ��ؿ�";
						break;
				}
			}
			break;
		//����˵���Ϣ	   
   		case WM_COMMAND:
			switch(LOWORD(wParam)){
			case IDM_RESTART:
				// ���¿�ʼ��Ϸ
				IniMap(GameMap, GameMapOri, level); // ��ʼ����ͼ
				InvalidateRect(hwnd,NULL,1); // ���Ƶ�ͼ
				break;
			case IDM_NEXT:
				// ��һ��
				level++;
				if(level>NLEVEL-1){
					MessageBox(hwnd, "û����һ��!", "����", MB_OK);
					level--;
				}else{
					IniMap(GameMap, GameMapOri, level);
					InvalidateRect(hwnd,NULL,1);
				}
				break;
			case IDM_PREV:
				// ��һ��
				level--;
				if (level<0){
					MessageBox(hwnd, "û����һ��!", "����", MB_OK);
					level++;
				}else{
					IniMap(GameMap, GameMapOri, level);
					InvalidateRect(hwnd,NULL,1);
				}
				break;
			case IDM_SELECT:
				// ѡ��ѡ��ؿ����˵�ʱ��������ģʽ�Ի���ѡ��ؿ�
				hdlgSelect = CreateDialog(hInst, "DLGSELECT", hwnd, (DLGPROC)DlgSelectProc);
				break;
		    case IDM_EXIT:
			  	SendMessage(hwnd, WM_DESTROY, 0, 0); // ѡ���˳����˵���ʱ����Ӧ�ó��򷢳�WM_DESTROY��Ϣ
        		break;
			case IDM_HELP:
				// ѡ�񡰰������˵�ʱ������ģʽ�Ի��򣬽�����Ϸ��������
				DialogBox(hInst,"DLGHELP", hwnd, (DLGPROC)DlgHelpProc);
				break;
		 	}
	        break;	  

	//��ǰ��ͼ�Ļ���, ÿһ��Ĵ�СΪ 60*60, �����ͼ 8*9
	// 0��ǽ   1��·   2����   3���յ�   4������   5�������յ���   6���������յ���
	case WM_PAINT:
		if(isPlay){
			hDC = BeginPaint(hwnd, &PtStr);
			SetViewportOrgEx(hDC,0,30,NULL); // �����ӿ�ԭ��
			count = 0;

			for(int i=0; i<8; i++)
			{
				for(int j=0; j<9; j++)
				{
					int top = i * 60;
                    int left = j * 60;

					if(GameMap[i][j] == 0)  //ǽΪ��ɫ
					{
						hBrush = CreateSolidBrush(RGB(88, 88, 88)); 
						SelectObject(hDC, hBrush);  			
						RoundRect(hDC, left, top, left + 60, top + 60, 5, 5); 
						DeleteObject(hBrush);
					}
					if(GameMap[i][j] == 2 || GameMap[i][j] == 5)  //��Ϊ��ɫ
					{
						hBrush = CreateSolidBrush(RGB(0, 162, 232)); 
						SelectObject(hDC, hBrush);  			
						RoundRect(hDC, left, top, left + 60, top + 60, 50, 50); //50��Բ�ǵ�ֱ��
						DeleteObject(hBrush);
						x1 = i;
						y1 = j;
					}
					if(GameMap[i][j] == 3)  //�յ�Ϊ��ɫ
					{
						hBrush = CreateSolidBrush(RGB(237, 28, 36)); 
						SelectObject(hDC, hBrush);  			
						RoundRect(hDC, left, top, left + 60, top + 60, 5, 5); 
						DeleteObject(hBrush);
					}
					if(GameMap[i][j] == 4 || GameMap[i][j] == 6)   //����Ϊ��ɫ
					{
						hBrush = CreateSolidBrush(RGB(255, 127, 39)); 
						SelectObject(hDC, hBrush);  			
						RoundRect(hDC, left, top, left + 60, top + 60, 5, 5); 
						DeleteObject(hBrush);
						if(GameMap[i][j] == 6)//����������յ�
							count++;
					}
				}
			}

			EndPaint(hwnd, &PtStr);

			//�ж���Ϸ�Ƿ�ʤ��
			if(count == countMap[level]){
				MessageBox(NULL, TEXT("��ϲʤ����!"), TEXT("�ɹ�"), MB_OK);
				// ���¿�ʼ��Ϸ
				level++;
				if(level < NLEVEL){
					IniMap(GameMap, GameMapOri, level);
					count = 0;
					InvalidateRect(hwnd,NULL,1);
				} else {
					MessageBox(NULL,TEXT("��Ϸ������o(�s���t)o"),TEXT("��Ϸ����"), MB_OK);
					level--;
				}

			}
		}
		else{
			// ����λͼ����Ϸ��ʼ֮ǰ���뻶ӭ����
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
	//�����Ϣ
	case WM_LBUTTONDOWN:
		if(isPlay){
            //���������������ƶ�
			//��ȡ��������λ��
			my = LOWORD(lParam) / 60; 
			mx = (HIWORD(lParam)-30) / 60; // ��Ϊ������ռ��30���룬����������������� 
			//���������λ���ڵ�ǰ�˵�λ�õ����ڿ��������ƶ������򲻶�
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
			//���Ҫ�ƶ�����λ����·���յ�
			if(GameMap[x2][y2] == 1 || GameMap[x2][y2] == 3)
			{
				if(GameMap[x2][y2] == 1) //�ƶ���·
					GameMap[x2][y2] = 2; //·�����
				else if(GameMap[x2][y2] == 3) //�ƶ����յ�
					GameMap[x2][y2] = 5; //�����յ���
				if(GameMap[x1][y1] == 2) //�ƶ�ǰ����·��
					GameMap[x1][y1] = 1; //�ƶ�ǰ��λ�ñ�Ϊ·
				else if(GameMap[x1][y1] == 5) //�ƶ�ǰ�����յ�
					GameMap[x1][y1] = 3; //�ƶ�ǰλ�ñ��յ�
				InvalidateRect(hwnd, NULL, 1);
			}
			//���Ҫ�ƶ�����λ�������ӣ���һ����������ǰ��Ŀ���
			if(GameMap[x2][y2] == 4 || GameMap[x2][y2] == 6)
			{
				if(GameMap[x3][y3] == 1 || GameMap[x3][y3] == 3)//����ǰ��·�����յ�
				{
					if(GameMap[x3][y3] == 1) //����ǰ��·
						GameMap[x3][y3] = 4;
					else if(GameMap[x3][y3] == 3) //����ǰ���յ�
						GameMap[x3][y3] = 6;

					if(GameMap[x2][y2] == 4) //����֮ǰ�����յ�
						GameMap[x2][y2] = 2;  //��
					else if(GameMap[x2][y2] == 6) //����֮ǰ���յ�
						GameMap[x2][y2] = 5; //�����յ�

					if(GameMap[x1][y1] == 2) //��֮ǰ�����յ�
						GameMap[x1][y1] = 1;  //�˱�·
					else if(GameMap[x1][y1] == 5) //��֮ǰ���յ�
						GameMap[x1][y1] = 3; //�˱��յ�
					InvalidateRect(hwnd, NULL, 1);
				}
			}
		}else{
			// �����Ļ����λ�ÿ�ʼ��Ϸ
			isPlay = TRUE;								
			SendMessage(hwnd,WM_CREATE,0,0);
			InvalidateRect(hwnd,NULL,1);
		}		
		return 0;
	
	//���̰�����Ϣ--------------------------------
	case WM_KEYDOWN:
        //�ü��̲���
		switch(wParam)
		{
		case 'A':
		case VK_LEFT:   //���������
			x2 = x1;
			y2 = y1 - 1;
			x3 = x1;
			y3 = y1 - 2;
			break;
		case 'D':
		case VK_RIGHT:  //���Ҽ�����
			x2 = x1;
			y2 = y1 + 1;
			x3 = x1;
			y3 = y1 + 2;
			break;
		case 'W':
		case VK_UP:     //���ϼ�����
			x2 = x1 - 1;
			y2 = y1;
			x3 = x1 - 2;
			y3 = y1;
			break;
		case 'X':
		case VK_DOWN:   //���¼�����
			x2 = x1 + 1;
			y2 = y1;
			x3 = x1 + 2;
			y3 = y1;
			break;
		}
		
		//���Ҫ�ƶ�����λ����·���յ�
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
		//���Ҫ�ƶ�����λ�������ӣ���һ����������ǰ��Ŀ���
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
		PostQuitMessage(0);					//����PostQuitMessage����WM_QUIT��Ϣ

	default:								//ȱʡʱ����ϵͳ��Ϣȱʡ������
		return  DefWindowProc(hwnd,message,wParam,lParam);
	}
	return (0);
}
// �����Ի��������
BOOL CALLBACK DlgHelpProc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
     	//��ʼ���Ի��� 
     	case WM_INITDIALOG:
	    	return 1;
	 	//����Ի�����Ϣ 
	 	case WM_COMMAND:
    		switch (LOWORD(wParam)){
		   		case IDOK:              //�رնԻ���  
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

//������ѡ��Ի���ѡ��ؿ�
BOOL CALLBACK DlgSelectProc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
        //��ʼ����Ϣ�� 
        case WM_INITDIALOG:
		{
			// ����ѡ���
			hLevelCombo = CreateWindow("combobox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, 35, 70, 500, hdlg, (HMENU)IDC_COMBO, hInst, NULL);
			// ���NLEVEL���ؿ���
			for (int i = 1; i <= NLEVEL; i++) {
				char temp[3] { 0 };
				_itoa_s(i, temp, 3, 10);            // 10��10����
				SendMessage(hLevelCombo, CB_ADDSTRING, 0, (LPARAM)temp);
			}
			char buff[3]{ 0 };
			_itoa_s(level + 1, buff, 3, 10);
			SendMessage(hLevelCombo, CB_SELECTSTRING, -1, (LPARAM)buff);
			return 0;
		}
	    //������Ϣ����Ϣ 
	    case WM_COMMAND:
	        switch (LOWORD(wParam)){
		        case IDOK:
					//���ѡ��Ĺؿ� 
					level = SendMessage(hLevelCombo, CB_GETCURSEL, 0, 0); // ��ñ�Ŵ�0��ʼ
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

// ��ʼ����ǰ��ͼΪ��ѡ�ؿ��ĵ�ͼ
void IniMap(int GameMap[][9], int GameMapOri[][8][9], int level){
	for (int i=0; i<8; i++){
		for (int j=0; j<9; j++){
			GameMap[i][j] = GameMapOri[level][i][j];
		}
	}
	x1 = 0; y1 = 0; x2 = 0; y2 = 0; x3 = 0; y3 = 0; // ��ʼ������
	mx = 0; my = 0; // ��ʼ��mx��my
}