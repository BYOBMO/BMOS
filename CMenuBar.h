#pragma once

#include "CWindow.h"
#include "CMenuBarItem.h"
#include "CIcon.h"

class CMenuBar : public CWindow
{
public:
	int mLast;
	CIcon* mIcon;
	CIcon* mRightIcon;

	CMenuBar();
	virtual void Draw();

	void AddMenuItem(CMenuBarItem* item, int index, std::function<void(CMenuBarItem*)> callback);
	virtual void KeyDown(SDL_KeyboardEvent e);
	void AddIcon(std::string fname);
	void AddRightIcon(std::string fname, int frames);
};

