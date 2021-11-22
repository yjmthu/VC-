
// ChenJiDlg.h: 头文件
//

#pragma once

#include <vector>
// CChenJiDlg 对话框
class CChenJiDlg : public CDialogEx
{
// 构造
public:
	CChenJiDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHENJI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	typedef std::pair<CString, int> info;
	std::vector<info> students;
	CButton m_rBtnGrade;
	CComboBox m_cBxSubjects;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
private:
	CListBox m_lstBoxGrade;
private:
	CEdit m_eDtName;
public:
	afx_msg void OnLbnDblclkList1();
};
