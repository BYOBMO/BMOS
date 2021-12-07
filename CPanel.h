#pragma once
#include "CWindow.h"

class CPanel: public CWindow
{
public:
	CPanel();
	virtual ~CPanel();

	unsigned mBgColor = 0xEBEBEB;

	virtual void Draw();
	virtual void KeyDown(SDL_KeyboardEvent e);
	void SelectFirst();

};

