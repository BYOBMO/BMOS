#include "CPanel.h"
#include "CIcon.h"
#include "CDesktop.h"
#include "Utils.h"

CPanel::CPanel(): CWindow()
{

}

CPanel::~CPanel()
{
	//printf("~Cpanel()\n");
}

void CPanel::Draw()
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	Uint8 r, g, b, a;
	SDL_Texture* oldTarget;

	if (mVisible == false)
	{
		return;
	}

	oldTarget = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, mTexture);

	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

	SDL_SetRenderDrawColor(renderer, RChannel(mBgColor), GChannel(mBgColor), BChannel(mBgColor), 255);
	
	SDL_RenderClear(renderer);

	CWindow::Draw();

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_SetRenderTarget(renderer, oldTarget);

	CTexture::Draw();
}

void CPanel::KeyDown(SDL_KeyboardEvent e)
{

	int len;
	int i;
	int idx = -1;
	CWindow* win = NULL;

	len = (int)mWindows.size();

	for (i = 0; i < len; i++)
	{
		win = mWindows[i];
		if (win->IsFocus())
		{
			idx = i;
			break;
		}
	}

	if (idx != -1)
	{
		if (e.keysym.sym == SDLK_DOWN)
		{
			for (i = idx + 1; i < mWindows.size(); i++)
			{
				if (mWindows[i]->mCanFocus)
				{
					CDesktop::SetFocused(mWindows[i]);
					break;
				}
			}
		}
		else if (e.keysym.sym == SDLK_UP)
		{
			bool found = false;
			for (i = idx-1; i >= 0; i--)
			{
				if (mWindows[i]->mCanFocus)
				{
					found = true;
					CDesktop::SetFocused(mWindows[i]);
					break;
				}
			}

			if (found == false)
			{
				if (mParent != NULL)
				{
					mParent->KeyDown(e);
				}
			}
		}
	}
	else if (mParent != NULL)
	{
		mParent->KeyDown(e);
	}

}

void CPanel::SelectFirst()
{
	int i;

	for (i = 0; i < mWindows.size(); i++)
	{
		if (mWindows[i]->mCanFocus)
		{
			CDesktop::SetFocused(mWindows[i]);
			break;
		}
	}
}
