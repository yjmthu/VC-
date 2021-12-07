
// MineClearanceDlg.h: 头文件
//

#pragma once
#include <vector>
#include "MineButton.h"
// CMineClearanceDlg 对话框

class CMineClearanceDlg : public CDialogEx
{
// 构造
public:
	CMineClearanceDlg(CWnd* pParent = nullptr);	// 标准构造函数
	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MINECLEARANCE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
	std::vector<std::vector<MineButton>> m_BtnS;
	UINT _diff = 1;
	void UpdateGame(bool erase=true);

// 实现
protected:
	HICON m_hIcon;
	CMenu m_menu;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGameSize(UINT nID);
	afx_msg void OnGameDiff(UINT nID);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};
