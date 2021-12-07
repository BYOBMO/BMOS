#include "CTitleBar.h"
#include "CDesktop.h"



void CTitleBar::Create(SDL_Renderer* renderer)
{
	mCloseBox = new CIcon("images/x.png", 2);
	mCloseBox->mDraggable = false;
	AddWindow(mCloseBox);
}

CTitleBar::CTitleBar() : CWindow()
{
	SDL_Renderer* renderer = CApplication::sRenderer;


	Create(renderer);
}



void CTitleBar::SetTitle(std::string title)
{
	mTitle.SetText(CText::sFontSmall, title, CText::cBlack);
}

void CTitleBar::Init()
{

}

void CTitleBar::Draw()
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	Uint8 r, g, b, a;
	SDL_Texture* oldTarget;

	oldTarget = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, mTexture);

	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);


	SDL_SetRenderDrawColor(renderer, 164, 164, 164, 255);
	SDL_RenderClear(renderer);

	//sCloseBox.SetPosition(mTexW - sCloseBox.GetWidth(), 0);
	//sCloseBox.Draw(renderer);

	mCloseBox->SetPosition(mTexW - mCloseBox->m_Width, 0);
	mCloseBox->Draw();

	mTitle.SetPosition((mTexW - mTitle.mTexW) / 2, (mTexH - mTitle.mTexH) / 2);
	mTitle.Draw();

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

void CTitleBar::AddClickHandler(std::function<void(CIcon*)> callback)
{
	mCloseBox->AddClickHandler(callback);
}

void CTitleBar::SelectClose()
{
	CDesktop::SetFocused(mCloseBox);
}

void CTitleBar::KeyDown(SDL_KeyboardEvent e)
{
	CWindow::KeyDown(e);
	if (e.keysym.sym == SDLK_DOWN)
	{
		if (mParent != NULL)
		{
			mParent->KeyDown(e);
		}
	}
	else if (e.keysym.sym == SDLK_UP )
	{
		SelectClose();
	}

}