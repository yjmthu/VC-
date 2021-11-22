
// CalculatorDlg.h: 头文件
//

#pragma once


// CCalculatorDlg 对话框
class CCalculatorDlg : public CDialogEx
{
// 构造
public:
	CCalculatorDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALCULATOR_DIALOG };
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
	CEdit m_formula;
	CEdit m_results;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton34();
	afx_msg void OnBnClickedButton30();
	afx_msg void OnBnClickedButton28();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton24();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton31();
	afx_msg void OnBnClickedButton25();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton33();
	afx_msg void OnBnClickedButton32();
	afx_msg void OnBnClickedButton27();
	afx_msg void OnBnClickedButton21();
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton26();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton23();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton19();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton29();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton37();
	afx_msg void OnBnClickedButton36();
	afx_msg void OnBnClickedButton40();
	afx_msg void OnBnClickedButton39();
	afx_msg void OnBnClickedButton38();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton45();
	afx_msg void OnBnClickedButton44();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton41();
	afx_msg void OnBnClickedButton46();
	afx_msg void OnBnClickedButton42();
	afx_msg void OnBnClickedButton43();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton35();
};
