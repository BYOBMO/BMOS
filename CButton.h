#pragma once
#include "CWindow.h"
#include "CText.h"

#include <functional>

class CButton :
    public CWindow
{

public:
    CButton();

	unsigned mBgColor = 0xE2FFE2;
	unsigned mBgColorSelect = 0xCCFFCC;
	unsigned mDrawColor = 0xE2FFE2;

	virtual void Draw();
	void SetText(std::string text);
	void SetText(std::string text, TTF_Font* font);
	void SetText(std::string text, TTF_Font* font, int w, int h);
	//virtual CWindow* OnMouseMotion(int x, int y);
	virtual CWindow* OnMouseButton(SDL_MouseButtonEvent e);
	virtual void KeyDown(SDL_KeyboardEvent e);
	virtual void KeyUp(SDL_KeyboardEvent e);
	void Resize(int w, int h);
	//CWindow* Click();
	virtual void Clicked(SDL_MouseButtonEvent e);
	void Pressed();

	void AddClickHandler(std::function<void(CButton*)> callback);

	CText mText;

	std::vector<std::function<void(CButton*)>> mCallbacks;
};