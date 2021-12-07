#include "CDropDownMenu.h"
#include "CDesktop.h"

#include "CIcon.h"
#include <functional>
using namespace std::placeholders;

CDropDownMenu::CDropDownMenu() : CWindow()
{
	mID += "CDropDownMenu";

	mParentItem = NULL;
	CreateBlank(120, 36, SDL_TEXTUREACCESS_TARGET);
	Hide();
}

CDropDownMenu::~CDropDownMenu()
{
	//printf("~CDropDownMenu()\n");
	for (auto it = mWindows.begin(); it != mWindows.end(); )
	{
		CMenuBarItem* mbi = dynamic_cast<CMenuBarItem*>(*it);

		if (mbi)
		{
			//printf("Remove menubar item\n");

			it = mWindows.erase(it);
			delete(mbi);
		}
		else
		{
			++it;
		}
	}
}

void CDropDownMenu::Draw()
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

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	CWindow::Draw();

	SDL_Rect rect = { 0, 0, mTexW, mTexH};

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_SetRenderTarget(renderer, oldTarget);

	CTexture::Draw();
}

void CDropDownMenu::AddMenuItem(std::string text, std::function<void(CMenuBarItem*)> callback, TTF_Font* font)
{
	CMenuBarItem* item;
	int y;

	item = new CMenuBarItem();
	item->SetTitle(text, font);
	
	item->mCallbacks.push_back(std::bind(&CDropDownMenu::OnItemClicked, this, _1));
	item->mCallbacks.push_back(callback);
	y = (int)mWindows.size() * (item->mTexH + 5);
	item->SetPosition(0, y);
	item->mBgColor = 0xFFFFFF;
	item->mDrawColor = 0xFFFFFF;
	item->mBgColorSelect = 0xCCFFCC;
	AddWindow(item);

	int w = mTexW;

	if (item->mTexW > w)
	{
		w = item->mTexW;
	}

	for (auto rit = std::rbegin(mWindows); rit != std::rend(mWindows); ++rit) {
		item = (CMenuBarItem*)*rit;
		item->Resize(w, item->mTexH);
	}

	CreateBlank(w, (int)mWindows.size() * (item->mTexH + 5), SDL_TEXTUREACCESS_TARGET);
}

void CDropDownMenu::OnItemClicked(CMenuBarItem* item)
{
	Hide();
	CDesktop::SetFocused(mParentItem);

	if (mParentItem != NULL && mParentItem->mComboBox)
	{
		mParentItem->SetTitle(item->mText);
	}
}

void CDropDownMenu::KeyDown(SDL_KeyboardEvent e)
{
	int len;
	int i;
	int selected = -1;
	CMenuBarItem* item = NULL;

	CWindow::KeyDown(e);
	len = (int)mWindows.size();
	
	for (i=0; i<len; i++)
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

	ClearAll();

	if (selected != -1)
	{
		if (e.keysym.sym == SDLK_UP)
		{
			
			selected--;
			if (selected < 0)
			{
				selected = 0;
				
				CDesktop::SetFocused(mParentItem);
				Hide();
				return;
			}
		}
		else if (e.keysym.sym == SDLK_DOWN)
		{
			
			selected++;
			if (selected >= len)
			{
				selected = len-1;
			}
		}
		else if (e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_LALT || e.keysym.sym == SDLK_RALT)
		{
			item->Pressed();
		}
		CDesktop::sDesktop->SetFocused(mWindows[selected]);
		//mWindows[selected]->mSelected = true; XXX
	}
}

void CDropDownMenu::ClearAll()
{
	int len;
	int i;
	int selected = -1;
	CMenuBarItem* item = NULL;

	len = (int)mWindows.size();

	for (i = 0; i < len; i++)
	{
		item = dynamic_cast<CMenuBarItem*>(mWindows[i]);
		if (item)
		{
			//item->mSelected = false; XXX
			if (item->IsFocus())
			{
				CDesktop::sDesktop->SetFocused(NULL);
			}
		}
	}
}

void CDropDownMenu::SelectFirstItem()
{
	ClearAll();
	if (mWindows.size() > 0)
	{
		CDesktop::sDesktop->SetFocused(mWindows[0]);
		//mWindows[0]->mSelected = true; XXX
	}
}