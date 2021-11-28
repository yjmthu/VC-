
// CalculatorDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Calculator.h"
#include "CalculatorDlg.h"
#include "afxdialogex.h"
#include "FormulaPaser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCalculatorDlg 对话框



CCalculatorDlg::CCalculatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CALCULATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CCalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_formula);
	DDX_Control(pDX, IDC_EDIT2, m_results);
}

BEGIN_MESSAGE_MAP(CCalculatorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CCalculatorDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON30, &CCalculatorDlg::OnBnClickedButton30)
	ON_BN_CLICKED(IDC_BUTTON28, &CCalculatorDlg::OnBnClickedButton28)
	ON_BN_CLICKED(IDC_BUTTON13, &CCalculatorDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON24, &CCalculatorDlg::OnBnClickedButton24)
	ON_BN_CLICKED(IDC_BUTTON12, &CCalculatorDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON11, &CCalculatorDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON31, &CCalculatorDlg::OnBnClickedButton31)
	ON_BN_CLICKED(IDC_BUTTON25, &CCalculatorDlg::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_BUTTON15, &CCalculatorDlg::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON33, &CCalculatorDlg::OnBnClickedButton33)
	ON_BN_CLICKED(IDC_BUTTON32, &CCalculatorDlg::OnBnClickedButton32)
	ON_BN_CLICKED(IDC_BUTTON27, &CCalculatorDlg::OnBnClickedButton27)
	ON_BN_CLICKED(IDC_BUTTON21, &CCalculatorDlg::OnBnClickedButton21)
	ON_BN_CLICKED(IDC_BUTTON20, &CCalculatorDlg::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON22, &CCalculatorDlg::OnBnClickedButton22)
	ON_BN_CLICKED(IDC_BUTTON8, &CCalculatorDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON26, &CCalculatorDlg::OnBnClickedButton26)
	ON_BN_CLICKED(IDC_BUTTON16, &CCalculatorDlg::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON9, &CCalculatorDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON23, &CCalculatorDlg::OnBnClickedButton23)
	ON_BN_CLICKED(IDC_BUTTON17, &CCalculatorDlg::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, &CCalculatorDlg::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON19, &CCalculatorDlg::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON7, &CCalculatorDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON10, &CCalculatorDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalculatorDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON29, &CCalculatorDlg::OnBnClickedButton29)
	ON_BN_CLICKED(IDC_BUTTON4, &CCalculatorDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON37, &CCalculatorDlg::OnBnClickedButton37)
	ON_BN_CLICKED(IDC_BUTTON36, &CCalculatorDlg::OnBnClickedButton36)
	ON_BN_CLICKED(IDC_BUTTON40, &CCalculatorDlg::OnBnClickedButton40)
	ON_BN_CLICKED(IDC_BUTTON39, &CCalculatorDlg::OnBnClickedButton39)
	ON_BN_CLICKED(IDC_BUTTON38, &CCalculatorDlg::OnBnClickedButton38)
	ON_BN_CLICKED(IDC_BUTTON14, &CCalculatorDlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON45, &CCalculatorDlg::OnBnClickedButton45)
	ON_BN_CLICKED(IDC_BUTTON44, &CCalculatorDlg::OnBnClickedButton44)
	ON_BN_CLICKED(IDC_BUTTON5, &CCalculatorDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON41, &CCalculatorDlg::OnBnClickedButton41)
	ON_BN_CLICKED(IDC_BUTTON46, &CCalculatorDlg::OnBnClickedButton46)
	ON_BN_CLICKED(IDC_BUTTON42, &CCalculatorDlg::OnBnClickedButton42)
	ON_BN_CLICKED(IDC_BUTTON43, &CCalculatorDlg::OnBnClickedButton43)
	ON_BN_CLICKED(IDC_BUTTON35, &CCalculatorDlg::OnBnClickedButton35)
END_MESSAGE_MAP()


// CCalculatorDlg 消息处理程序

BOOL CCalculatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCalculatorDlg::OnPaint()
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
HCURSOR CCalculatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CCalculatorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(TEXT("科学计算器"));
	return 0;
}

void CCalculatorDlg::OnEnChangeEdit1()
{
	CString text;
	m_formula.GetWindowText(text);
	if (!m_formula.GetWindowTextLength())
	{
		m_results.SetWindowText(NULL);
		return;
	}
	FormulaPaser<TCHAR> paser((const TCHAR*)text);
	m_results.SetWindowText(paser.outstr(false).c_str());
	m_formula.SetFocus();
}


void CCalculatorDlg::OnBnClickedButton34()
{
	PostQuitMessage(0);
}


void CCalculatorDlg::OnBnClickedButton30()
{
	CString text;
	m_formula.GetWindowText(text);
	if (!m_formula.GetWindowTextLength())
	{
		m_results.SetWindowText(NULL);
		return;
	}
	FormulaPaser<TCHAR> paser((const TCHAR*)text);
	m_results.SetWindowText(paser.outstr(true).c_str());
	m_formula.SetFocus();
}

void CCalculatorDlg::OnBnClickedButton15()
{
	m_formula.SetWindowText(NULL);
	m_results.SetWindowText(NULL);
	m_formula.SetFocus();
}

void CCalculatorDlg::OnBnClickedButton28()
{
	m_formula.ReplaceSel(TEXT("pi()"));
}


void CCalculatorDlg::OnBnClickedButton13()
{
	m_formula.ReplaceSel(TEXT("e()"));
}


void CCalculatorDlg::OnBnClickedButton24()
{
	m_formula.ReplaceSel(TEXT("ln("));
}


void CCalculatorDlg::OnBnClickedButton12()
{
	m_formula.ReplaceSel(TEXT("*"));
}


void CCalculatorDlg::OnBnClickedButton11()
{
	m_formula.ReplaceSel(TEXT("/"));
}


void CCalculatorDlg::OnBnClickedButton31()
{
	m_formula.ReplaceSel(TEXT("+"));
}


void CCalculatorDlg::OnBnClickedButton25()
{
	m_formula.ReplaceSel(TEXT("-"));
}


void CCalculatorDlg::OnBnClickedButton33()
{
	m_formula.ReplaceSel(TEXT("0"));
}


void CCalculatorDlg::OnBnClickedButton32()
{
	m_formula.ReplaceSel(TEXT("."));
}


void CCalculatorDlg::OnBnClickedButton27()
{
	m_formula.ReplaceSel(TEXT("1"));
}


void CCalculatorDlg::OnBnClickedButton21()
{
	m_formula.ReplaceSel(TEXT("2"));
}


void CCalculatorDlg::OnBnClickedButton20()
{
	m_formula.ReplaceSel(TEXT("3"));
}


void CCalculatorDlg::OnBnClickedButton22()
{
	m_formula.ReplaceSel(TEXT("6"));
}


void CCalculatorDlg::OnBnClickedButton8()
{
	m_formula.ReplaceSel(TEXT("5"));
}


void CCalculatorDlg::OnBnClickedButton26()
{
	m_formula.ReplaceSel(TEXT("4"));
}


void CCalculatorDlg::OnBnClickedButton16()
{
	m_formula.ReplaceSel(TEXT("9"));
}


void CCalculatorDlg::OnBnClickedButton9()
{
	m_formula.ReplaceSel(TEXT("8"));
}


void CCalculatorDlg::OnBnClickedButton23()
{
	m_formula.ReplaceSel(TEXT("7"));
}


void CCalculatorDlg::OnBnClickedButton17()
{
	m_formula.ReplaceSel(TEXT("sin("));
}


void CCalculatorDlg::OnBnClickedButton18()
{
	m_formula.ReplaceSel(TEXT("cos("));
}


void CCalculatorDlg::OnBnClickedButton19()
{
	m_formula.ReplaceSel(TEXT("tan("));
}


void CCalculatorDlg::OnBnClickedButton35()
{
	m_formula.ReplaceSel(TEXT("exp("));
}

void CCalculatorDlg::OnBnClickedButton7()
{
	m_formula.ReplaceSel(TEXT("lg("));
}


void CCalculatorDlg::OnBnClickedButton10()
{
	m_formula.ReplaceSel(TEXT("abs("));
}


void CCalculatorDlg::OnBnClickedButton3()
{
	m_formula.ReplaceSel(TEXT("arc"));
}


void CCalculatorDlg::OnBnClickedButton29()
{
	m_formula.ReplaceSel(TEXT("ans()"));
}


void CCalculatorDlg::OnBnClickedButton4()
{
	m_formula.ReplaceSel(TEXT("deg("));
}


void CCalculatorDlg::OnBnClickedButton37()
{
	m_formula.ReplaceSel(TEXT("rad("));
}


void CCalculatorDlg::OnBnClickedButton36()
{
	m_formula.ReplaceSel(TEXT("("));
}


void CCalculatorDlg::OnBnClickedButton40()
{
	m_formula.ReplaceSel(TEXT(")"));
}

void CCalculatorDlg::OnBnClickedButton39()
{
	DWORD dwSel = m_formula.GetSel();
	int left = HIWORD(dwSel), right = LOWORD(dwSel);
	if (left != right)
		m_formula.SetSel(left);
	else if (0 < left--)
		m_formula.SetSel(left, left);
	m_formula.SetFocus();
}


void CCalculatorDlg::OnBnClickedButton38()
{
	DWORD dwSel = m_formula.GetSel();
	int left = HIWORD(dwSel), right = LOWORD(dwSel);
	if (left != right)
		m_formula.SetSel(left);
	if (left++ < m_formula.GetWindowTextLength())
		m_formula.SetSel(left, left);
	m_formula.SetFocus();
}


void CCalculatorDlg::OnBnClickedButton14()
{
	DWORD dwSel = m_formula.GetSel();
	int left = HIWORD(dwSel), right = LOWORD(dwSel);
	if (left && left == right)
		m_formula.SetSel(--left, right);
	m_formula.ReplaceSel(TEXT(""));
	m_formula.SetFocus();
}


void CCalculatorDlg::OnBnClickedButton45()
{
	m_formula.ReplaceSel(TEXT("^"));
}


void CCalculatorDlg::OnBnClickedButton44()
{
	m_formula.ReplaceSel(TEXT("fact("));
}


void CCalculatorDlg::OnBnClickedButton5()
{
	m_formula.ReplaceSel(TEXT("sqrt("));
}


void CCalculatorDlg::OnBnClickedButton41()
{
	m_formula.ReplaceSel(TEXT("sinh("));
}


void CCalculatorDlg::OnBnClickedButton46()
{
	m_formula.ReplaceSel(TEXT("cosh("));
}


void CCalculatorDlg::OnBnClickedButton42()
{
	m_formula.ReplaceSel(TEXT("tanh("));
}


void CCalculatorDlg::OnBnClickedButton43()
{
	m_formula.ReplaceSel(TEXT("rand()"));
}

BOOL CCalculatorDlg::PreTranslateMessage(MSG* pMsg)
{
	static bool shift = false;
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_SHIFT:
			shift = true;
			break;
		case VK_DELETE:
			m_formula.SetWindowText(NULL);
			m_results.SetWindowText(NULL);
			m_formula.SetFocus();
			return TRUE;
		case VK_BACK:
		{
			DWORD dwSel = m_formula.GetSel();
			int left = HIWORD(dwSel), right = LOWORD(dwSel);
			if (left && left == right)
				m_formula.SetSel(--left, right);
			m_formula.ReplaceSel(TEXT(""));
			m_formula.SetFocus();
			return TRUE;
		}
		case VK_OEM_PLUS:
			if (shift) break;
		case VK_RETURN:
		{
			CString text;
			m_formula.GetWindowText(text);
			if (!m_formula.GetWindowTextLength())
			{
				m_results.SetWindowText(NULL);
				return TRUE;
			}
			FormulaPaser<TCHAR> paser((const TCHAR*)text);
			m_results.SetWindowText(paser.outstr(true).c_str());
			m_formula.SetFocus();
			return TRUE;
		}
		default:
			if ((strchr("+-*/^().", pMsg->wParam) || isalnum(pMsg->wParam)) && m_formula.GetSafeHwnd() != GetFocus()->GetSafeHwnd())
			{
				TCHAR text[2] = { pMsg->wParam, '\0' };
				m_formula.ReplaceSel(text);
				m_formula.SetFocus();
				return TRUE;
			}
		}
	}
	else if (pMsg->message == WM_KEYUP && pMsg->wParam == VK_SHIFT)
	{
		shift = false;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
