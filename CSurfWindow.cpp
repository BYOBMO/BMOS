#include "CSurfWindow.h"

#include <vector>
#include <functional>


CSurfWindow::CSurfWindow()
{
	mParent = NULL;
	mVisible = true;
	X = 0;
	Y = 0;
	mBgColor = 0x000000;
}

CSurfWindow::CSurfWindow(int w, int h): CSurface(w, h)
{
	mParent = NULL;
	mVisible = true;
	X = 0;
	Y = 0;
	mBgColor = 0x000000;
}

CSurfWindow::~CSurfWindow()
{

}

void CSurfWindow::AddWindow(CSurfWindow* window)
{
	window->mParent = this;
	mWindows.push_back(window);
}

void CSurfWindow::Draw(CSurface* dest)
{
	if (mVisible == false || mSurface==NULL)
	{
		return;
	}

	SDL_FillRect(mSurface, NULL, mBgColor);

	std::vector<CSurfWindow*>::iterator window;

	window = mWindows.begin();
	for (; window != mWindows.end(); window++)
	{
		(*window)->Draw(this);
	}

	if (dest != NULL)
	{
		dest->Blit(this, X, Y);
	}
}

void CSurfWindow::DrawTexture(SDL_Renderer* renderer)
{
	SDL_Texture* tex;

	if (mSurface == NULL)
	{
		return;
	}


	tex = SDL_CreateTextureFromSurface(renderer, mSurface);
	SDL_Rect dst = { X, Y, mSurface->w, mSurface->h };

	SDL_RenderCopy(renderer, tex, NULL, &dst);

	SDL_DestroyTexture(tex);
}

void CSurfWindow::DrawTexture(SDL_Renderer *renderer, int w, int h)
{
	SDL_Texture* tex;
	SDL_Surface* surf;

	if (w == 0)
		w = mSurface->w;

	if (h==0)
		h = mSurface->h;

	w = std::min(w, mSurface->w);
	h = std::min(h, mSurface->h);

	if (mSurface == NULL)
	{
		return;
	}

	surf = SDL_CreateRGBSurface(0,
		w,
		h,
		mSurface->format->BitsPerPixel,
		mSurface->format->Rmask,
		mSurface->format->Gmask,
		mSurface->format->Bmask,
		mSurface->format->Amask);

	SDL_Rect srcRect = {-X, -Y, w, h};
	SDL_Rect dstRect = {0, 0, w, h};
	SDL_BlitSurface(mSurface, &srcRect, surf, &dstRect);

	tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_Rect dst = { 0, 0, surf->w, surf->h };

	SDL_RenderCopy(renderer, tex, NULL, &dst);

	SDL_FreeSurface(surf);
	SDL_DestroyTexture(tex);
}

void CSurfWindow::DrawTextureScaled(SDL_Renderer* renderer, int w, int h)
{
	SDL_Texture* tex;
	SDL_Surface* surf;
	int newW, newH;
	double xratio, yratio, ratio;



	if (mSurface == NULL)
	{
		return;
	}

	surf = SDL_CreateRGBSurface(0,
		w,
		h,
		mSurface->format->BitsPerPixel,
		mSurface->format->Rmask,
		mSurface->format->Gmask,
		mSurface->format->Bmask,
		mSurface->format->Amask);

	yratio = (double)h / (double)mSurface->h;
	xratio = (double)w / (double)mSurface->w;
	ratio = std::min(xratio, yratio);

	newW = (double)mSurface->w * ratio;
	newH = (double)mSurface->h * ratio;

	SDL_Rect srcRect = { -X, -Y, w, h };
	SDL_Rect dstRect = { (w - newW) / 2, (h - newH) / 2, newW, newH };
	SDL_BlitScaled(mSurface, NULL, surf, &dstRect);

	tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_Rect dst = { 0, 0, surf->w, surf->h };

	SDL_RenderCopy(renderer, tex, NULL, &dst);

	SDL_FreeSurface(surf);
	SDL_DestroyTexture(tex);
}

int CSurfWindow::Height()
{
	if (mSurface == NULL)
	{
		return(0);
	}

	return(mSurface->h);
}

int CSurfWindow::Width()
{
	if (mSurface == NULL)
	{
		return(0);
	}

	return(mSurface->w);
}