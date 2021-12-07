#include "CButton.h"
#include "Utils.h"
#include "CApplication.h"

CButton::CButton() : CWindow()
{

}

void CButton::SetText(std::string title)
{
	mText.SetText(CText::sFontSmall, title, CText::cBlack);
	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
}

void CButton::SetText(std::string title, TTF_Font* font)
{
	mText.SetText(font, title, CText::cBlack);
	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
}

void CButton::SetText(std::string title, TTF_Font* font, int w, int h)
{
	mText.SetText(font, title, CText::cBlack, w, h, CTexture::HAlignment::Center);
	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
}

void CButton::Resize(int w, int h)
{
	CreateBlank(w, h, SDL_TEXTUREACCESS_TARGET);
}

void CButton::Draw()
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

	rect.x = X;
	rect.y = Y;
	rect.w = mTexW;
	rect.h = mTexH;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &rect);


	rect.x = X+1;
	rect.y = Y+1;
	rect.w = mTexW - 1;
	rect.h = mTexH - 1;
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
	SDL_RenderDrawRect(renderer, &rect);

	//SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255);
	//SDL_RenderDrawLine(renderer, X+1, Y+mTexH - 2, X+mTexW - 2, Y+mTexH - 2);
	//SDL_RenderDrawLine(renderer, X+mTexW - 2, Y+mTexH - 2, X+mTexW - 2, Y+1);

	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//SDL_RenderDrawLine(renderer, X+0, Y+mTexH - 1, X+mTexW, Y+mTexH - 1);
	//SDL_RenderDrawLine(renderer, X+mTexW - 1, Y+mTexH, X+mTexW - 1, Y+0);

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

//CWindow* CButton::OnMouseMotion(int x, int y)
//{
//	CWindow* handled;
//
//	int mx, my;
//
//	handled = CWindow::OnMouseMotion(x, y);
//
//	mx = x - X;
//	my = y - Y;
//
//	mSelected = false;
//	mDrawColor = mBgColor;
//	if (mx > 1 && mx < mTexW - 1 && my>1 && my < mTexH - 1)
//	{
//		mSelected = true;
//	}
//
//	return(handled);
//}

void CButton::Pressed()
{
	for (auto cb = std::begin(mCallbacks); cb != std::end(mCallbacks); ++cb) {
		std::function<void(CButton*)> callback;

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

void CButton::Clicked(SDL_MouseButtonEvent e)
{
	CWindow::Clicked(e);

	Pressed();
}

//CWindow* CButton::Click()
//{
//	CWindow* handled = NULL;
//	for (auto cb = std::begin(mCallbacks); cb != std::end(mCallbacks); ++cb) {
//		std::function<void(CButton*)> callback;
//
//		callback = *cb;
//
//		if (callback)
//		{
//			callback(this);
//			handled = this;
//		}
//		else
//		{
//			printf("CButton: No handler\n");
//		}
//	}
//
//	return(handled);
//}

CWindow* CButton::OnMouseButton(SDL_MouseButtonEvent e)
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

void CButton::KeyDown(SDL_KeyboardEvent e)
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
	else if ((e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_LALT || e.keysym.sym == SDLK_RALT) && IsFocus())
	{
		Pressed();
	}
}

void CButton::KeyUp(SDL_KeyboardEvent e)
{

}

void CButton::AddClickHandler(std::function<void(CButton*)> callback)
{
	mCallbacks.push_back(callback);
}