#include "CMenuBarItem.h"
#include "Utils.h"
#include "CDesktop.h"

CMenuBarItem::CMenuBarItem() : CWindow()
{
	mComboBox = false;
	mDropMenu = NULL;
	mDropIcon = NULL;
	mFont = CText::sFontLarge;
}

CMenuBarItem::~CMenuBarItem()
{
	//printf("~CMenuBarItem()\n");
	if (mDropMenu != NULL)
	{
		CDesktop::RemoveMenu(mDropMenu);
		delete(mDropMenu);
		mDropMenu = NULL;
	}
}

void CMenuBarItem::RemoveDropDown()
{
	if (mDropMenu != NULL)
	{
		CDesktop::RemoveMenu(mDropMenu);
		delete(mDropMenu);
		mDropMenu = NULL;
	}
}
void CMenuBarItem::SetTitle(std::string title)
{
	mText = title;
	mTitle.SetText(mFont, title, CText::cBlack);

	if (mComboBox && mDropMenu != NULL)
	{
		Resize(mDropMenu->Width() + mDropIcon->mTexH, mDropIcon->mTexH);
	}
	else
	{
		CreateBlank(mTitle.mTexW, mTitle.mTexH, SDL_TEXTUREACCESS_TARGET);
	}
}

void CMenuBarItem::SetTitle(std::string title, TTF_Font *font)
{
	mText = title;
	mTitle.SetText(font, title, CText::cBlack);
	if (mComboBox && mDropMenu!=NULL)
	{
		Resize(mDropMenu->Width() + mDropIcon->mTexH, mDropIcon->mTexH);
	}
	else
	{
		CreateBlank(mTitle.mTexW, mTitle.mTexH, SDL_TEXTUREACCESS_TARGET);
	}
}

void CMenuBarItem::MouseEnter()
{
	CWindow::MouseEnter();
	Focus();
	//mSelected = true; XXX
}

void CMenuBarItem::MouseLeave()
{
	CWindow::MouseLeave();
	
	//mSelected = false; XXX
}

void CMenuBarItem::ComboBox()
{
	mComboBox = true;
	mBgColor = 0xE2FFE2;
	mBgColorSelect = 0xCCFFCC;
	mDrawColor = 0xE2FFE2;

	mDropIcon = new CIcon("images/drop.png", 1);
}

void CMenuBarItem::Resize(int w, int h)
{
	CreateBlank(w, h, SDL_TEXTUREACCESS_TARGET);
}

void CMenuBarItem::Draw()
{
	SDL_Renderer* renderer = CApplication::sRenderer;


	Uint8 r, g, b, a;

	if (mVisible == false)
	{
		return;
	}

	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

	if (IsFocus()) // XXX
	{
		SDL_SetRenderDrawColor(renderer, RChannel(mBgColorSelect), GChannel(mBgColorSelect), BChannel(mBgColorSelect), 255);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, RChannel(mDrawColor), GChannel(mDrawColor), BChannel(mDrawColor), 255);
	}



	SDL_Rect rect;

	rect.x = X;
	rect.y = Y;
	rect.w = mTexW;
	rect.h = mTexH;
	SDL_RenderFillRect(renderer, &rect);

	if (mComboBox)
	{
		if (mDropIcon != NULL)
		{
			mDropIcon->X = X + (Width() - mDropIcon->Width()) - 4;
			mDropIcon->Y = Y + ((mTexH - mDropIcon->mTexH)/2);
			mDropIcon->Draw();
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &rect);
		if (IsFocus())
		{
			rect.x++;
			rect.y++;
			rect.w -= 2;
			rect.h -= 2;
			SDL_RenderDrawRect(renderer, &rect);
		}
	}
	else if (IsFocus())
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &rect);
	}


	mTitle.SetPosition(X, Y + (mTexH - mTitle.mTexH)/2);
	mTitle.Draw();

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

CWindow* CMenuBarItem::OnMouseMotion(int x, int y)
{
	CWindow *handled;

	int h = 0;
	handled = CWindow::OnMouseMotion(x, y);
	if (handled == this)
	{
		h = 1;
		Focus();
	}
	//printf("%d,%d %d,%d  %d\n", X, Y, x, y, h);

	return(handled);
}

void CMenuBarItem::Pressed()
{
	for (auto cb = std::begin(mCallbacks); cb != std::end(mCallbacks); ++cb) {
		std::function<void(CMenuBarItem*)> callback;

		callback = *cb;

		if (callback)
		{
			callback(this);
		}
		else
		{
			printf("MenuBarItem: No handler\n");
		}
	}

	if (mDropMenu != NULL)
	{
		int x, y;

		GetScreenCoordinates(&x, &y);
		x = std::max(0, x - (mDropMenu->Width() -Width()));
		mDropMenu->SetPosition(x, y + mTexH);
		mDropMenu->Show();
		CDesktop::SetFocused(mDropMenu);
		//mSelected = false; XXX
		mDropMenu->SelectFirstItem();
	}
}


void CMenuBarItem::Clicked(SDL_MouseButtonEvent e)
{
	CWindow::Clicked(e);
	Pressed();
}


void CMenuBarItem::KeyDown(SDL_KeyboardEvent e)
{
	CWindow::KeyDown(e);
	if (e.keysym.sym == SDLK_DOWN || e.keysym.sym == SDLK_RIGHT)
	{
		if (mParent != NULL)
		{
			mParent->KeyDown(e);
		}
	}
	else if (e.keysym.sym == SDLK_UP || e.keysym.sym == SDLK_LEFT)
	{
		if (mParent != NULL)
		{
			mParent->KeyDown(e);
		}
	}
	else if ((e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_LALT || e.keysym.sym == SDLK_RALT) && IsFocus()) // XXX
	{
		Pressed();
	}
}


void CMenuBarItem::AddMenuItem(std::string text, std::function<void(CMenuBarItem*)> callback)
{
	if (mDropMenu == NULL)
	{
		int x, y;

		mDropMenu = CDesktop::CreateMenu();
		GetScreenCoordinates(&x, &y);
		mDropMenu->SetPosition(x, y+mTexH);
		mDropMenu->mParentItem = this;
	}

	mDropMenu->AddMenuItem(text, callback, mFont);
	if (mComboBox && mTexH>0)
	{
		Resize(mDropMenu->Width()+ mTexH, mTexH);
	}

}

void CMenuBarItem::AddMenuItem(std::string text, std::function<void(CMenuBarItem*)> callback, int w, int h)
{
	if (mDropMenu == NULL)
	{
		int x, y;

		mDropMenu = CDesktop::CreateMenu();
		GetScreenCoordinates(&x, &y);
		mDropMenu->SetPosition(x, y + mTexH);
		mDropMenu->mParentItem = this;
	}

	mDropMenu->AddMenuItem(text, callback, mFont);
}