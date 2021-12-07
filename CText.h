#pragma once
#ifdef WINDOWS
#define _HAS_STD_BYTE 0
#endif

#include <SDL2/SDL_ttf.h>

#include "CTexture.h"

using namespace std;

class CText : public CTexture
{
private:
	CTexture *DropTex;

public:
	CText();
	std::string mText;

	bool DropShadow;
	void Draw();
	void SetText(TTF_Font *font, std::string textureText, SDL_Color textColor);
	void SetText(TTF_Font* font, std::string textureText, SDL_Color textColor, int w, int h, HAlignment halign);
	static TTF_Font* sFontSmall;
	static TTF_Font* sFontLarge;
	static SDL_Color cBlack;
	static SDL_Color cRed;
};

