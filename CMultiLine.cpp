#include "CMultiLine.h"
#include "CText.h"
#include "CApplication.h"

CMultiLine::CMultiLine()
{
	mTextSurface = new CSurfWindow();
	mCanFocus = false;
}

void CMultiLine::Clear()
{
	mLines.clear();
	mTextSurface->Reset();
}

void CMultiLine::AddLine(string text)
{
	AddLine(text, CSurface::HAlignment::Left);
}

void CMultiLine::AddLine(string text, CSurface::HAlignment align)
{
	mLines.push_back(text);

	CSurface* textSurf = new CSurface();
	textSurf->RenderTexture(CText::sFontSmall, text, CText::cBlack, 0, 0, CSurface::HAlignment::Left);
	mTextSurface->BlitEx(textSurf, 0, mTextSurface->Height(), align);

	delete(textSurf);
}

void CMultiLine::RenderSurface()
{
	int len = mLines.size();
	int i;
	CSurface* textSuf = new CSurface();

	for (i = 0; i < len; i++)
	{
		textSuf->RenderTexture(CText::sFontSmall, mLines[i], CText::cBlack, 0, 0, CSurface::HAlignment::Left);
		mTextSurface->BlitEx(textSuf, 0, mTextSurface->Height());
	}
}

void CMultiLine::Draw()
{
	SDL_Renderer* renderer = CApplication::sRenderer;
	Uint8 r, g, b, a;
	int w, h;

	SDL_GetRendererOutputSize(renderer, &w, &h);

	if (mTextSurface == NULL || mTextSurface->Width() == 0 || mTextSurface->Height() == 0)
	{
		return;
	}

	//SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	//SDL_Rect rect = {X, Y, mTextSurface->Width(),mTextSurface->Height()};
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	//SDL_RenderFillRect(renderer, &rect);
	//SDL_SetRenderDrawColor(renderer, r, g, b, a);


	mTextSurface->X = X;
	mTextSurface->Y = Y;
	if (mTextSurface->mSurface->h <= h)
	{
		mTextSurface->DrawTexture(renderer);
	}
	else
	{
		mTextSurface->DrawTexture(renderer, w, h);
	}
}