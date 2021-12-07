#include "CMenuBar.h"
#include "CDesktop.h"

CMenuBar::CMenuBar(): CWindow()
{
	mLast = 75;
	mIcon = NULL;
	mRightIcon = NULL;
}

void CMenuBar::Draw()
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


	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
	SDL_RenderClear(renderer);

	CWindow::Draw();


	SDL_Rect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = mTexW;
	rect.h = mTexH;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderDrawRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_SetRenderTarget(renderer, oldTarget);

	CTexture::Draw();

}

void CMenuBar::AddMenuItem(CMenuBarItem* item, int index, std::function<void(CMenuBarItem*)> callback)
{
	int x = mLast + 10;
	int y = (mTexH - item->mTexH) / 2;
	item->SetPosition(x, y);
	item->mCallbacks.push_back(callback);
	AddWindow(item);

	mLast += item->mTexW + 35;

}

void CMenuBar::KeyDown(SDL_KeyboardEvent e)
{
	int len;
	int i;
	int selected = -1;
	CMenuBarItem* item = NULL;

	CWindow::KeyDown(e);

	len = (int)mWindows.size();

	for (i = 0; i < len; i++)
	{
		item = dynamic_cast<CMenuBarItem*>(mWindows[i]);
		if (item)
		{
			if (item->IsFocus()) // XXX
			{
				selected = i;
				break;
			}
		}
	}

	if (selected != -1)
	{
		if (e.keysym.sym == SDLK_UP || e.keysym.sym == SDLK_LEFT)
		{
			//mWindows[selected]->mSelected = false; XXX
			selected--;
			if (mIcon==NULL && selected < 0)
			{
				selected = 0;
			}
			else if (selected < 1)
			{
				selected = 1;
			}
		}
		else if (e.keysym.sym == SDLK_DOWN || e.keysym.sym == SDLK_RIGHT)
		{
			//mWindows[selected]->mSelected = false; XXX
			selected++;
			if (mRightIcon == NULL && selected >= len)
			{
				selected = len - 1;
			}
			else if (selected >= len-1)
			{
				selected = len - 2;
			}
		}
		else if (e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_LALT || e.keysym.sym == SDLK_RALT)
		{
			//item->Click();
		}
		
		CDesktop::sDesktop->SetFocused(mWindows[selected]);
		//mWindows[selected]->mSelected = true; XXX
	}

}

void CMenuBar::AddIcon(std::string fname)
{
	mIcon = new CIcon(fname, 1);
	mIcon->SetPosition(0, 0);
	mIcon->mCanFocus = false;
	AddWindow(mIcon);
}

void CMenuBar::AddRightIcon(std::string fname, int frames)
{
	mRightIcon = new CIcon(fname, frames);
	mRightIcon->SetPosition(Width() - mRightIcon->Width(), 0);
	mRightIcon->mCanFocus = false;
	mRightIcon->mVisible = false;
	AddWindow(mRightIcon);
}