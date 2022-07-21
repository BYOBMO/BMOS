#include "CTextBox.h"
#include "CText.h"
#include "Utils.h"
#include "CApplication.h"

#include <functional>

CTextBox::CTextBox() : CWindow()
{
	mBorder = true;
	mCanFocus = true;
}

void CTextBox::SetText(std::string title)
{
	mText.SetText(CText::sFontLarge, title, CText::cBlack);
	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
}

void CTextBox::SetText(std::string title, TTF_Font* font)
{
	mText.SetText(font, title, CText::cBlack);
	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
}

void CTextBox::SetText(std::string title, TTF_Font* font, int w, int h)
{
	mText.SetText(font, title, CText::cBlack, w, h, CTexture::HAlignment::Left);
	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
}

void CTextBox::Resize(int w, int h)
{
	CreateBlank(w, h, SDL_TEXTUREACCESS_TARGET);
}

void CTextBox::OnTextEvent(SDL_TextInputEvent e)
{
	mText.SetText(CText::sFontSmall, mText.mText + e.text, CText::cBlack, 300, 40, CTexture::HAlignment::Left);

}

void CTextBox::KeyDown(SDL_KeyboardEvent e)
{
	CWindow* handled = NULL;
	int x, y;

	if (e.keysym.sym == SDLK_DOWN)
	{
		if (mParent != NULL)
		{
			mParent->KeyDown(e);
		}
	}
	else if (e.keysym.sym == SDLK_UP)
	{
		if (mParent != NULL)
		{
			mParent->KeyDown(e);
		}
	}
	else if ((e.keysym.sym == SDLK_BACKSPACE) && IsFocus())
	{
		std::string newText = mText.mText;

		if (newText.length() > 0)
		{
			newText.pop_back();
			if (newText == "")
			{
				newText = "";
			}
		}
		mText.SetText(CText::sFontSmall, newText, CText::cBlack, 300, 40, CTexture::HAlignment::Left);
	}
}

void CTextBox::Draw()
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


	if (IsFocus())
	{
		SDL_SetRenderDrawColor(renderer, 0, 196, 128, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}
	rect.x = X;
	rect.y = Y;
	rect.w = mTexW;
	rect.h = mTexH;

	SDL_RenderDrawRect(renderer, &rect);

	if (IsFocus())
	{
		rect.x = X+1;
		rect.y = Y+1;
		rect.w = mTexW-2;
		rect.h = mTexH-2;
		SDL_RenderDrawRect(renderer, &rect);
	}

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
