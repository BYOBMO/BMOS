#include "CTextScroll.h"
#include "CText.h"
#include "CSurfWindow.h"
#include "CApplication.h"

CTextScroll::CTextScroll(): CWindow()
{
	mUpdate = false;
	mFirstLine = 0;
	mLastLine = 0;
	mOffset = 0;
	mPosition = 0;
}

void CTextScroll::AddLine(string text)
{
	mText.push_back(text);
	mUpdate = true;
}

void CTextScroll::SetScroll(int position)
{
	int fontH = TTF_FontHeight(CText::sFontSmall);
	mPosition = position;
	mUpdate = true;

	mFirstLine = position / fontH;
	mLastLine = mFirstLine + (mParent->GetHeight() / fontH) + 1;
	if (mLastLine > mText.size())
	{
		mLastLine = mText.size();
	}
	mOffset = position - (mFirstLine * fontH);

	//printf("%d %d %d %d %d\n", position, mFirstLine, mLastLine, mOffset, fontH);
}

void CTextScroll::Draw()
{
	int i;

	if (mUpdate)
	{
		SDL_Texture* oldTarget;
		CSurfWindow* surf = new CSurfWindow();
		int fontH = TTF_FontHeight(CText::sFontSmall);
		oldTarget = SDL_GetRenderTarget(CApplication::sRenderer);
		SDL_SetRenderTarget(CApplication::sRenderer, mTexture);
		SDL_RenderClear(CApplication::sRenderer);
		for (i = mFirstLine; i < mLastLine; i++)
		{
			surf->Y = (i - mFirstLine) * fontH - mOffset;
			surf->RenderText(CText::sFontSmall, mText[i], CText::cBlack);
			surf->DrawTexture(CApplication::sRenderer);
		}

		SDL_SetRenderTarget(CApplication::sRenderer, oldTarget);
		delete(surf);
		mUpdate = false;
	}

	CTexture::Draw();
}

void CTextScroll::KeyDown(SDL_KeyboardEvent e)
{
	if (e.keysym.sym == SDLK_UP)
	{
		if (mOffset == 0 && mFirstLine == 0)
		{
			if (mParent != NULL)
			{
				mParent->KeyDown(e);
			}
		}
		else
		{
			mPosition = mPosition - 10;
			if (mPosition < 0)
			{
				mPosition = 0;
			}
			SetScroll(mPosition);
		}
	}
	else if (e.keysym.sym == SDLK_PAGEUP)
	{
		mPosition = mPosition - mParent->GetHeight();
		if (mPosition < 0)
		{
			mPosition = 0;
		}
		SetScroll(mPosition);
	}
	else if (e.keysym.sym == SDLK_PAGEDOWN)
	{
		mPosition += mParent->GetHeight();
		SetScroll(mPosition);
	}
	else if (e.keysym.sym == SDLK_DOWN)
	{
		mPosition += 10;
		SetScroll(mPosition);
	}
	else if (mParent != NULL)
	{
		mParent->KeyDown(e);
		return;
	}

	for (int i = 0; i < mCallbacks.size(); i++)
	{
		mCallbacks[i](this);
	}
}

void CTextScroll::AddTextPositionHandler(std::function<void(CTextScroll*)> callback)
{
	mCallbacks.push_back(callback);
}

void CTextScroll::SetSize(int w, int h)
{
	CreateBlank(w, h, SDL_TEXTUREACCESS_TARGET);
}