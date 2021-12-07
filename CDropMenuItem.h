#pragma once
#include "CWindow.h"
#include "CText.h"

#include <functional>

class CDropMenuItem :
    public CWindow
{
public:
	unsigned mBgColor = 0x808080;
	unsigned mBgColorSelect = 0xA4A4A4;
	unsigned mDrawColor = 0x808080;;

	virtual void Draw();
	void SetTitle(std::string title);
	virtual bool OnMouseMotion(int x, int y);
	virtual bool OnMouseButton(SDL_MouseButtonEvent e);

	CText mTitle;
	std::function<void(CDropMenuItem*)> mCallback;
};

