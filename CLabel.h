#pragma once
#include "CWindow.h"
#include "CText.h"

#include <functional>

class CLabel :
	public CWindow
{

public:
	CLabel();

	bool mBorder;

	unsigned mBgColor = 0xEBEBEB;
	unsigned mBgColorSelect = 0xB0B0B0;
	unsigned mDrawColor = 0xEBEBEB;

	virtual void Draw();
	void SetText(std::string text);
	void SetText(std::string text, TTF_Font* font);
	void SetText(std::string text, TTF_Font* font, int w, int h);

	void Resize(int w, int h);

	virtual void OnTextEvent(SDL_TextInputEvent e);

	CText mText;
};

