#include "CLabel.h"
#include "CText.h"
#include "Utils.h"
#include "CApplication.h"

#include <functional>

CLabel::CLabel() : CWindow()
{
	mBorder = false;
	mCanFocus = false;
}

void CLabel::SetText(std::string title)
{
	mText.SetText(CText::sFontLarge, title, CText::cBlack);
	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
}

void CLabel::SetText(std::string title, TTF_Font* font)
{
	mText.SetText(font, title, CText::cBlack);
	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
}

void CLabel::SetText(std::string title, TTF_Font* font, int w, int h)
{
	mText.SetText(font, title, CText::cBlack, w, h, CTexture::HAlignment::Left);
	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
}

void CLabel::Resize(int w, int h)
{
	CreateBlank(w, h, SDL_TEXTUREACCESS_TARGET);
}

void CLabel::OnTextEvent(SDL_TextInputEvent e)
{
	mText.SetText(CText::sFontSmall, mText.mText + e.text, CText::cBlack, 300, 40, CTexture::HAlignment::Left);

}

void CLabel::Draw()
{
	SDL_Renderer* renderer = CApplication::sRenderer;


	Uint8 r, g, b, a;

	if (mVisible == false)
	{
		return;
	}

	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);


	SDL_SetRenderDrawColor(renderer, RChannel(mDrawColor), GChannel(mDrawColor), BChannel(mDrawColor), 255);

	SDL_Rect rect;

	rect.x = X;
	rect.y = Y;
	rect.w = Width();
	rect.h = mTexH;
	SDL_RenderFillRect(renderer, &rect);


	mText.SetPosition(X, Y);
	mText.Draw();

	if (mBorder)
	{
		rect.x = X;
		rect.y = Y;
		rect.w = mTexW;
		rect.h = mTexH;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &rect);
	}

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
