#include "CVScrollBar.h"
#include "Utils.h"
#include "CApplication.h"
#include <functional>

CVScrollBar::CVScrollBar() : CWindow()
{
	using namespace std::placeholders;
	mMin = 0;
	mMax = 0;
	mPosition = 0;
	SetHeight(100);
	mSlider = new CIcon("images/vslider.png", 1);
	AddWindow(mSlider);
	mSlider->SetPosition(0, 0);
	mSlider->mDraggable = true;
	mSlider->mDragX = false;
	mCanFocus = false;
	mSlider->AddOnPositionHandler(std::bind(&CVScrollBar::OnScrollPosition, this, _1));
}

void CVScrollBar::SetHeight(int h)
{
	CreateBlank(32, h, SDL_TEXTUREACCESS_TARGET);
}

void CVScrollBar::Draw()
{
	SDL_Renderer* renderer = CApplication::sRenderer;
	Uint8 r, g, b, a;
	SDL_Texture* oldTarget;

	oldTarget = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, mTexture);

	if (mVisible == false)
	{
		return;
	}

	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);


	SDL_SetRenderDrawColor(renderer, RChannel(mBgColor), GChannel(mBgColor), BChannel(mBgColor), 255);


	SDL_RenderClear(renderer);

	SDL_Rect rect;



	CWindow::Draw();

	SDL_SetRenderDrawColor(renderer, 0,0,0,255);

	rect.x = 0;
	rect.y = 0;
	rect.w = Width();
	rect.h = mTexH;
	SDL_RenderDrawRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_SetRenderTarget(renderer, oldTarget);

	CTexture::Draw();
}

void CVScrollBar::OnScrollPosition(CIcon* slider)
{
	double percent;

	if (slider->Y < 0)
	{
		slider->Y = 0;
	}
	else if (slider->Y > (mTexH - slider->mTexH))
	{
		slider->Y = (mTexH - slider->mTexH);
	}

	percent = (double)(slider->Y) / (double)(this->mTexH - slider->mTexH);
	//printf("%d %d %f\n", slider->X, slider->Y, percent);
	mPosition = mMin + (mMax - mMin) * percent;
	if (mOnScrollCallback)
	{
		mOnScrollCallback(mPosition);
	}
}

void CVScrollBar::AddOnScrollHandler(std::function<void(int)> callback)
{
	mOnScrollCallback = callback;
}

void CVScrollBar::SetRange(int min, int max)
{
	mMin = min;
	mMax = max;
}

void CVScrollBar::SetPos(int pos)
{
	SetPos(pos, true);
}

void CVScrollBar::SetPos(int pos, bool update)
{
	double d;

	d = (double)(this->mTexH - mSlider->mTexH) / (double)(mMax - mMin);

	mSlider->Y = pos * d;
	if (update)
	{
		OnScrollPosition(mSlider);
	}
}