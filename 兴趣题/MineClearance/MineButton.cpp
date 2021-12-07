#include "pch.h"
#include "MineButton.h"
#include "resource.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

// MineButton

std::vector<std::vector<MineButton>>* MineButton::ptr = nullptr;
unsigned MineButton::size = GameSize::Small;
std::time_t MineButton::game_time = 0;
bool MineButton::game_unstart = true;
bool MineButton::game_finished = false;

int MineButton::getMineNumber()
{
	int all = 0;
	MineType tp;
	tp = ptr->at(indexX)[indexY - 1].type;
	all += (tp == MineType::MaskMine || tp == MineType::Mine || tp == MineType::FlagMine);
	tp = ptr->at(indexX)[indexY + 1].type;
	all += (tp == MineType::MaskMine || tp == MineType::Mine || tp == MineType::FlagMine);
	tp = ptr->at(indexX - 1)[indexY].type;
	all += (tp == MineType::MaskMine || tp == MineType::Mine || tp == MineType::FlagMine);
	tp = ptr->at(indexX - 1)[indexY - 1].type;
	all += (tp == MineType::MaskMine || tp == MineType::Mine || tp == MineType::FlagMine);
	tp = ptr->at(indexX - 1)[indexY + 1].type;
	all += (tp == MineType::MaskMine || tp == MineType::Mine || tp == MineType::FlagMine);
	tp = ptr->at(indexX + 1)[indexY].type;
	all += (tp == MineType::MaskMine || tp == MineType::Mine || tp == MineType::FlagMine);
	tp = ptr->at(indexX + 1)[indexY - 1].type;
	all += (tp == MineType::MaskMine || tp == MineType::Mine || tp == MineType::FlagMine);
	tp = ptr->at(indexX + 1)[indexY + 1].type;
	all += (tp == MineType::MaskMine || tp == MineType::Mine || tp == MineType::FlagMine);
	return all;
}

void MineButton::OpenSquare()
{
	if (indexY == 0 || indexX == (size & 0XFF) - 1 ||
		indexX == 0 || indexY == (size >> 8) - 1) return;
	if (type == MineType::MaskNumber)
	{
		type = MineType::Number;
		InvalidateRect(NULL);
		if (!getMineNumber())
		{
			ptr->at(indexX)[indexY - 1].OpenSquare();
			ptr->at(indexX)[indexY + 1].OpenSquare();
			ptr->at(indexX - 1)[indexY].OpenSquare();
			ptr->at(indexX - 1)[indexY - 1].OpenSquare();
			ptr->at(indexX - 1)[indexY + 1].OpenSquare();
			ptr->at(indexX + 1)[indexY].OpenSquare();
			ptr->at(indexX + 1)[indexY - 1].OpenSquare();
			ptr->at(indexX + 1)[indexY + 1].OpenSquare();
		}
	}
}

MineButton::MineButton(MineType type) :type(type)
{
}

MineButton::MineButton(const MineButton&b) : type(b.type)
{
}

MineButton::~MineButton()
{
}

void MineButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void MineButton::DrawItem(LPDRAWITEMSTRUCT lp)
{
	CRect rc = lp->rcItem;
	CFont ft;
	ft.CreateFont(rc.Height() * 2 / 3, rc.Width() / 2, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("宋体"));
	CDC dc;
	dc.Attach(lp->hDC);
	CBrush br_black, br_white;
	br_white.CreateSolidBrush(0XFFFFFF);
	br_black.CreateSolidBrush(0);
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	dc.SelectObject(&pen);
	dc.SelectObject(&ft);
	TCHAR text[2] = { '0', '\0' };
	dc.SelectObject(&br_white);
	dc.Rectangle(0, 0, rc.Width(), rc.Height());
	switch (type)
	{
	case MineType::Mine:
		dc.SelectObject(&br_black);
		dc.Ellipse(rc.Width() / 6, rc.Width() / 6, rc.Width() * 5 / 6, rc.Height() * 5 / 6);
		break;
	case MineType::Number:
		dc.SetBkMode(TRANSPARENT);
		*text += getMineNumber();
		dc.DrawText(text, CRect(0, 0, rc.Width(), rc.Height()), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	case MineType::FlagMine:
	case MineType::FlagNumber:
	{
		dc.SelectObject(&br_black);
		dc.MoveTo({ rc.Width() / 2, rc.Height() / 8 });
		dc.LineTo({ rc.Width() / 2, rc.Height() * 7 / 8 });
		POINT pt[3] = { rc.Width() / 2, rc.Height() / 8, rc.Width() * 7 / 8, rc.Height() / 4, rc.Width() / 2, rc.Height() * 3 / 8 };
		dc.Polygon(pt, 3);
		break;
	}
	default:
		break;
	}
	dc.DeleteDC();
}

BEGIN_MESSAGE_MAP(MineButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

inline CString formatTime(std::time_t time)
{
	CString str;
	std::time_t d, h, m, s;
	d = time / (60 * 60 * 24);
	time %= 60 * 60 * 24;
	h = time / (60 * 60);
	time %= 60 * 60;
	m = time / 60;
	s = time % 60;
	if (d)
		str.Format(TEXT("%lld天%lld小时%lld分钟%lld秒"), d, h, m, s);
	else if (h)
		str.Format(TEXT("%lld小时%lld分钟%lld秒"), h, m, s);
	else if (m)
		str.Format(TEXT("%lld分钟%lld秒"), m, s);
	else
		str.Format(TEXT("%lld秒"), s);
	return str;
}

void MineButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (game_unstart)
	{
		game_unstart = false;
		game_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	if (game_finished)
		return;
	if (type == MineType::MaskMine)
	{
		type = MineType::Mine;
		int m = size & 0XFF, n = size >> 8;
		for (int i = 1; i < m - 1; ++i)
		{
			for (int j = 1; j < n-1; ++j)
			{
				auto & b = ptr->at(i)[j];
				switch (b.type)
				{
				case MineType::FlagMine:
				case MineType::MaskMine:
					b.type = MineType::Mine;
					break;
				case MineType::FlagNumber:
				case MineType::MaskNumber:
					b.type = MineType::Number;
					break;
				default:
					break;
				}
				b.InvalidateRect(NULL);
			}
		}
		game_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - game_time;
		game_finished = true;
		MessageBox(TEXT("游戏失败!\n总共用时: ") + formatTime(game_time), TEXT("提示"));
		return;
	}
	else if (type == MineType::MaskNumber)
	{
		OpenSquare();
		int m = size & 0XFF, n = size >> 8;
		for (int i = 1; i < m - 1; ++i)
		{
			for (int j = 1; j < n - 1; ++j)
			{
				const auto& b = ptr->at(i)[j];
				switch (b.type)
				{
				case MineType::Mine:
				case MineType::MaskMine:
				case MineType::MaskNumber:
				case MineType::FlagNumber:
					return;
				default:
					break;
				}
			}
		}
		game_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - game_time;
		game_finished = true;
		MessageBox(TEXT("恭喜获胜!\n总共用时: ") + formatTime(game_time), TEXT("提示"));
	}
}

void MineButton::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (game_unstart)
	{
		game_unstart = false;
		game_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	if (game_finished)
		return;
	switch (type)
	{
	case MineType::MaskMine:
		type = MineType::FlagMine;
		break;
	case MineType::MaskNumber:
		type = MineType::FlagNumber;
		break;
	case MineType::FlagNumber:
		type = MineType::MaskNumber;
		break;
	case MineType::FlagMine:
		type = MineType::MaskMine;
		break;
	default:
		break;
	}
	InvalidateRect(NULL);
	int m = size & 0XFF, n = size >> 8;
	for (int i = 1; i < m - 1; ++i)
	{
		for (int j = 1; j < n - 1; ++j)
		{
			const auto& b = ptr->at(i)[j];
			switch (b.type)
			{
			case MineType::Mine:
			case MineType::MaskMine:
			case MineType::MaskNumber:
			case MineType::FlagNumber:
				return;
			default:
				break;
			}
		}
	}
	game_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - game_time;
	game_finished = true;
	MessageBox(TEXT("恭喜获胜!\n总共用时: ") + formatTime(game_time), TEXT("提示"));
	return;
}
