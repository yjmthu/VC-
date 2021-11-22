
// ChenJiDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ChenJi.h"
#include "ChenJiDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChenJiDlg 对话框



CChenJiDlg::CChenJiDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHENJI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChenJiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_rBtnGrade);
	DDX_Control(pDX, IDC_COMBO1, m_cBxSubjects);
	DDX_Control(pDX, IDC_LIST1, m_lstBoxGrade);
	DDX_Control(pDX, IDC_EDIT1, m_eDtName);
}

BEGIN_MESSAGE_MAP(CChenJiDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CChenJiDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CChenJiDlg::OnBnClickedButton2)
	ON_LBN_DBLCLK(IDC_LIST1, &CChenJiDlg::OnLbnDblclkList1)
END_MESSAGE_MAP()


// CChenJiDlg 消息处理程序

BOOL CChenJiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowText(TEXT("成绩记录程序"));
	m_lstBoxGrade.AddString(TEXT("姓名 \t科目 \t成绩"));
	m_cBxSubjects.AddString(TEXT("语文"));
	m_cBxSubjects.AddString(TEXT("数学"));
	m_cBxSubjects.AddString(TEXT("英语"));
	m_cBxSubjects.SetCurSel(0);
	m_rBtnGrade.SetCheck(IDC_RADIO1);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChenJiDlg::OnPaint()
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
HCURSOR CChenJiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CChenJiDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	constexpr TCHAR* subjext[] = { TEXT("语文"), TEXT("数学"), TEXT("英语") };
	if (!m_eDtName.GetWindowTextLength())
	{
		MessageBox(TEXT("请输入姓名!"));
		return;
	}
	CString name, str;
	int subject_index = m_cBxSubjects.GetCurSel();
	m_eDtName.GetWindowText(name);
	str.Format(TEXT("%s \t%s \t%c"), (const TCHAR*)name, subjext[subject_index], TEXT("ABCD")[GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO4) - IDC_RADIO1]);
	if (std::find_if(students.begin(), students.end(), [&](const info& a)->bool {
		return a.first == name && a.second == subject_index;
		}) != students.end())
	{
		MessageBox(TEXT("请不要重复添加!"));
		return;
	}
	int index = m_lstBoxGrade.GetCurSel();
	students.emplace(students.begin() + index - 1, name, subject_index);
	m_lstBoxGrade.InsertString(index, str);
}


void CChenJiDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	constexpr TCHAR* subjext[] = { TEXT("语文"), TEXT("数学"), TEXT("英语") };
	if (!m_eDtName.GetWindowTextLength())
	{
		MessageBox(TEXT("请输入姓名!"));
		return;
	}
	CString name, str;
	int subject_index = m_cBxSubjects.GetCurSel();
	m_eDtName.GetWindowText(name);
	str.Format(TEXT("%s \t%s \t%c"), (const TCHAR*)name, subjext[subject_index], TEXT("ABCD")[GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO4) - IDC_RADIO1]);
	if (std::find_if(students.begin(), students.end(), [&](const info& a)->bool {
		return a.first == name && a.second == subject_index;
		}) != students.end())
	{
		MessageBox(TEXT("请不要重复添加!"));
		return;
	}
	students.emplace_back(name, subject_index);
	m_lstBoxGrade.AddString(str);
}


void CChenJiDlg::OnLbnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码
	int someone = m_lstBoxGrade.GetCurSel();
	if (someone)
	{
		students.erase(students.begin() + someone - 1);
		m_lstBoxGrade.DeleteString(someone);
	}
}
