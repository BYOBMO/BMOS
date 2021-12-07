#pragma once
#include "CWindow.h"
#include "CText.h"
#include "CIcon.h"

#include <functional>
#include <vector>

class CDropDownMenu;

class CMenuBarItem :
    public CWindow
{
public:
	unsigned mBgColor = 0x808080;
	unsigned mBgColorSelect = 0xA4A4A4;
	unsigned mDrawColor = 0x808080;

	CDropDownMenu* mDropMenu;
	TTF_Font* mFont;

	CMenuBarItem();
	virtual ~CMenuBarItem();

	bool mComboBox;
	void ComboBox();
	CIcon* mDropIcon;

	std::string mText;

	virtual void Draw();
	void SetTitle(std::string title);
	void SetTitle(std::string title, TTF_Font* font);
	virtual CWindow* OnMouseMotion(int x, int y);
	virtual void KeyDown(SDL_KeyboardEvent e);
	virtual void MouseEnter();
	virtual void MouseLeave();

	void RemoveDropDown();

	void Resize(int w, int h);

	void Clicked(SDL_MouseButtonEvent e);
	void Pressed();

	void AddMenuItem(std::string text, std::function<void(CMenuBarItem*)> callback);
	void AddMenuItem(std::string text, std::function<void(CMenuBarItem*)> callback, int w, int h);

	CText mTitle;

	std::vector<std::function<void(CMenuBarItem*)>> mCallbacks;
};

