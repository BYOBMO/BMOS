#pragma once
#include "CLabel.h"
#include "CText.h"

#include <vector>

class CClickList : public CWindow
{
public:
	CClickList();
	CClickList(int w, int h);
	bool mActive;
	int mWidth, mHeight;
	int mIndex;

	unsigned mBgColor = 0xEBEBEB;
	unsigned mBgColorSelect = 0xB0B0B0;
	unsigned mDrawColor = 0xEBEBEB;

	std::vector<CText*> mList;

	void AddItem(std::string item);
	virtual void Draw();
	virtual bool OnMouseButton(SDL_MouseButtonEvent e);
	virtual bool OnKeyDown(SDL_KeyboardEvent e);
	bool Click();
};

