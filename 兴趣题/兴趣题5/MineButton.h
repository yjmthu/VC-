#pragma once
#include <afxwin.h>
#include <vector>
#include <ctime>

// 地雷未挖开, 地雷挖开, 空格未挖开, 空格挖开, 旗插在地雷上, 旗插在空格上
enum class MineType
{
	MaskMine, Mine, MaskNumber, Number, FlagMine, FlagNumber
};

enum GameSize
{
	Small = (RGB(12, 12, 0)), Medium = (RGB(22, 32, 0)), Big = (RGB(32, 42, 0))
};

class MineButton : public CButton
{
private:
	//BOOL m_bTrackLeave = FALSE;
	int getMineNumber();
	void OpenSquare();
	
public:
	MineType type;
	std::size_t indexX = 0, indexY = 0;
	static unsigned size;                                // 界面大小
	static std::vector<std::vector<MineButton>>* ptr;
	static std::time_t game_time;
	static bool game_unstart, game_finished;
	explicit MineButton(MineType type = MineType::MaskNumber);
	MineButton(const MineButton&);
	virtual ~MineButton();
	virtual void PreSubclassWindow();  //修改自绘属性
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);  //绘制按钮
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);  //鼠标按下
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

