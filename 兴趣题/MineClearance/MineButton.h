#pragma once
#include <afxwin.h>
#include <vector>
#include <ctime>

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
	static unsigned size;
	static std::vector<std::vector<MineButton>>* ptr;
	static std::time_t game_time;
	static bool game_unstart, game_finished;
	explicit MineButton(MineType type = MineType::MaskNumber);
	MineButton(const MineButton&);
	virtual ~MineButton();
	virtual void PreSubclassWindow();  //修改自绘属性
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);  //绘制 圆形按钮
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);  //鼠标按下
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

