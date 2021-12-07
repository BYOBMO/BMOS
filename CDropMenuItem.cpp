#include "CDropMenuItem.h"
#include "Utils.h"

void CDropMenuItem::SetTitle(std::string title)
{
	mTitle.SetText(CText::sFont48, title, CText::cBlack);
	CreateBlank(mTitle.mTexW, mTitle.mTexH, SDL_TEXTUREACCESS_TARGET);
}

void CDropMenuItem::Draw()
{
	SDL_Renderer* renderer = CTexture::sRenderer;


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
	rect.w = mTitle.mTexW;
	rect.h = mTitle.mTexH;
	SDL_RenderFillRect(renderer, &rect);


	mTitle.SetPosition(X, Y);
	mTitle.Draw();

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

bool CDropMenuItem::OnMouseMotion(int x, int y)
{
	SDL_Renderer* renderer = CTexture::sRenderer;
	bool handled = true;

	int mx, my;

	mx = x - X;
	my = y - Y;

	mDrawColor = mBgColor;
	if (mx > 1 && mx < mTexW - 1 && my>1 && my < mTexH - 1 && sMouseMotionHandled==false)
	{
		mDrawColor = mBgColorSelect;
		handled = true;
	}

	sMouseMotionHandled = handled;
	return(handled);
}

bool CDropMenuItem::OnMouseButton(SDL_MouseButtonEvent e)
{
	SDL_Renderer* renderer = CTexture::sRenderer;

	int mx, my;

	mx = e.x - X;
	my = e.y - Y;

	//printf("%d:%d %d:%d\n", X, Y, mx, my);

	if (e.button == SDL_BUTTON_LEFT && e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (mx > 1 && mx < mTexW - 1 && my>1 && my < mTexH - 1)
		{
			if (mCallback)
			{
				mCallback(this);
				return(true);
			}
			else
			{
				printf("CDropMenuItem: No handler\n");
			}
		}
	}

	return(false);
}