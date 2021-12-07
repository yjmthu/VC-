
// MineClearanceDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MineClearance.h"
#include "MineClearanceDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMineClearanceDlg 对话框



CMineClearanceDlg::CMineClearanceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MINECLEARANCE_DIALOG, pParent)
{
	MineButton::ptr = &m_BtnS;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME_ICON);
	m_menu.LoadMenu(IDR_MAINFRAME_MENU);
}

void CMineClearanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CMineClearanceDlg::UpdateGame(bool erase)
{
	static bool menu_height = true;
	MineButton::game_unstart = true;
	MineButton::game_finished = false;
	unsigned id = 2020, x = 0, y = 0, IX = 0, IY = 0, nwidth = MineButton::size >> 8, nheight = MineButton::size & 0XFF;
	if (erase)
	{
		CRect view_rect, view_client;
		unsigned short w = MineButton::size == GameSize::Small ? 50 : (MineButton::size == GameSize::Medium) ? 40 : 30;
		GetWindowRect(&view_rect);
		GetClientRect(&view_client);
		CSize sz;
		sz.cx = (nwidth - 2) * w;
		sz.cy = menu_height ? (menu_height = false, (nheight - 2) * w + GetSystemMetrics(SM_CYMENU)) : (nheight - 2) * w;
		sz.cx += view_rect.Width() - view_client.Width();
		sz.cy += view_rect.Height() - view_client.Height();
		SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE);

		srand((unsigned)time(NULL));
		m_BtnS.resize(nheight);
		for (auto& i : m_BtnS)
		{
			i.resize(nwidth);
			if (IX == 0 || IX == nheight - 1)
			{
				++IX;
				continue;
			}
			for (auto& j : i)
			{
				if (IY == 0 || IY == nwidth - 1)
				{
					++IY;
					continue;
				}
				if (rand() % 10 < _diff) j.type = MineType::MaskMine;
				j.indexX = IX;
				j.indexY = IY;
				j.Create(NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(x, y, x + w, y + w), this, ++id);
				x += w;
				++IY;
			}
			x = 0;
			y += w;
			IY = 0;
			++IX;
		}
	}
	else
	{
		srand((unsigned)time(NULL));
		for (auto& i : m_BtnS)
		{
			if (IX == 0 || IX == nheight - 1)
			{
				++IX;
				continue;
			}
			for (auto& j : i)
			{
				if (IY == 0 || IY == nwidth - 1)
				{
					++IY;
					continue;
				}
				j.type = (rand() % 10 < _diff) ? MineType::MaskMine : MineType::MaskNumber;
				j.InvalidateRect(NULL);
				++IY;
			}
			IY = 0;
			++IX;
		}
	}
}

BEGIN_MESSAGE_MAP(CMineClearanceDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_1010, ID_3040, &CMineClearanceDlg::OnGameSize)
	ON_COMMAND_RANGE(ID_32771, ID_32775, &CMineClearanceDlg::OnGameDiff)
END_MESSAGE_MAP()


// CMineClearanceDlg 消息处理程序

BOOL CMineClearanceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	SetMenu(&m_menu);
	m_menu.CheckMenuRadioItem(ID_1010, ID_3040, ID_1010, 1);
	m_menu.CheckMenuRadioItem(ID_32771, ID_32775, ID_32771, 1);

	// TODO: 在此添加额外的初始化代码
	SetWindowText(TEXT("扫雷"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMineClearanceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMineClearanceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CMineClearanceDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	UpdateGame();

	return 0;
}


void CMineClearanceDlg::OnGameSize(UINT nID)
{
	bool u = false;
	switch (nID)
	{
	case ID_1010:
		u = MineButton::size != GameSize::Small;
		MineButton::size = GameSize::Small;
		break;
	case ID_2030:
		u = MineButton::size != GameSize::Medium;
		MineButton::size = GameSize::Medium;
		break;
	case ID_3040:
		u = MineButton::size != GameSize::Big;
		MineButton::size = GameSize::Big;
		break;
	default:
		break;
	}
	if (u) {
		InvalidateRect(NULL);
		m_BtnS.clear();
		UpdateGame();
	}
	m_menu.CheckMenuRadioItem(ID_1010, ID_3040, nID, 1);
}

void CMineClearanceDlg::OnGameDiff(UINT nID)
{
	_diff = 1 + nID - ID_32771;
	m_menu.CheckMenuRadioItem(ID_32771, ID_32775, nID, 1);
	UpdateGame(false);
}



BOOL CMineClearanceDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX ^ WS_SYSMENU;
	return CDialogEx::PreCreateWindow(cs);
}
