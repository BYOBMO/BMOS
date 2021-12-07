#include "CText.h"
#include "CApplication.h"

#include <string>

TTF_Font* CText::sFontSmall = NULL;
TTF_Font* CText::sFontLarge = NULL;
SDL_Color CText::cBlack = { 0, 0, 0 };
SDL_Color CText::cRed = { 255, 0, 0 };

CText::CText() : CTexture()
{
	mText = "";
	DropShadow = false;
	DropTex = new CTexture();
}


void CText::SetText(TTF_Font* font, std::string textureText, SDL_Color textColor, int w, int h, HAlignment halign)
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	mText = textureText;

	//printf("CText::SetText: %s\n", textureText.c_str());
	LoadFromRenderedText(renderer, font, textureText, textColor, w, h, halign);

	if (DropShadow)
	{
		SDL_Color clr = { 0, 0, 0 };

		DropTex->LoadFromRenderedText(renderer, font, textureText, clr);
	}

}

void CText::SetText(TTF_Font* font, std::string textureText, SDL_Color textColor)
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	mText = textureText;

	//printf("CText::SetText: %s\n", textureText.c_str());
	LoadFromRenderedText(renderer, font, textureText, textColor);

	if (DropShadow)
	{
		SDL_Color clr = { 0, 0, 0 };

		DropTex->LoadFromRenderedText(renderer, font, textureText, clr);
	}

}

void CText::Draw()
{
	CTexture::Draw();
}