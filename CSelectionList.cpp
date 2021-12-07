#include "CSelectionList.h"
#include "Utils.h"
#include "CDesktop.h"

CSelectionList::CSelectionList() : CWindow()
{

}

void CSelectionList::SetText(std::string title)
{
	mText.SetText(CText::sFontSmall, title, CText::cBlack);
	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
}

void CSelectionList::SetText(std::string title, TTF_Font* font)
{
	mText.SetText(font, title, CText::cBlack);
	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
}

void CSelectionList::SetText(std::string title, TTF_Font* font, int w, int h)
{
	mText.SetText(font, title, CText::cBlack, w, h, CTexture::HAlignment::Center);
	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
}

void CSelectionList::Resize(int w, int h)
{
	CreateBlank(w, h, SDL_TEXTUREACCESS_TARGET);
}

void CSelectionList::Draw()
{
	SDL_Renderer* renderer = CApplication::sRenderer;


	Uint8 r, g, b, a;

	if (mVisible == false)
	{
		return;
	}

	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

	if (mMouseIn)
	{
		SDL_SetRenderDrawColor(renderer, RChannel(mBgColorSelect), GChannel(mBgColorSelect), BChannel(mBgColorSelect), 255);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, RChannel(mBgColor), GChannel(mBgColor), BChannel(mBgColor), 255);
	}



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
		SDL_Rect r = rect;

		r.x++;
		r.y++;
		r.w -= 2;
		r.h -= 2;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &r);
	}

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void CSelectionList::Pressed()
{
	for (auto cb = std::begin(mCallbacks); cb != std::end(mCallbacks); ++cb) {
		std::function<void(CSelectionList*)> callback;

		callback = *cb;

		if (callback)
		{
			callback(this);
		}
		else
		{
			printf("CButton: No handler\n");
		}
	}

}

void CSelectionList::Clicked(SDL_MouseButtonEvent e)
{
	CWindow::Clicked(e);

	Pressed();
}


CWindow* CSelectionList::OnMouseButton(SDL_MouseButtonEvent e)
{
	int x, y;
	CWindow* handled = NULL;

	handled = CWindow::OnMouseButton(e);
	if (mVisible == false)
	{
		return(NULL);
	}


	return(handled);
}

void CSelectionList::KeyDown(SDL_KeyboardEvent e)
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
	else if ((e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_LALT || e.keysym.sym == SDLK_RALT) && IsFocus()) // XXX
	{
		Pressed();
	}
}

void CSelectionList::KeyUp(SDL_KeyboardEvent e)
{

}

void CSelectionList::AddClickHandler(std::function<void(CSelectionList*)> callback)
{
	mCallbacks.push_back(callback);
}