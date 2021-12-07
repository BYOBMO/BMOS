//This file is part of BMOS.
//
//BMOS is free software : you can redistribute itand /or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//BMOS is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with BMOS.If not, see < https://www.gnu.org/licenses/>.

#include "CForm.h"
#include "CIcon.h"
#include "CDesktop.h"

void CForm::SetTitle(std::string title)
{
	mTitleBar.SetTitle(title);
}

CForm::CForm(): CWindow()
{
	mPanel = new CPanel();
	mDrag = false;
	mLastMouseX = 0;
	mLastMouseY = 0;
	AddWindow(&mTitleBar);
	AddWindow(mPanel);
}

CForm::CForm(int x, int y, int w, int h) : CForm()
{
	using namespace std::placeholders;

	SDL_Renderer* renderer = CApplication::sRenderer;

	CreateBlank(w, h, SDL_TEXTUREACCESS_TARGET);
	SetPosition(x, y);

	mTitleBar.CreateBlank(w-4, 32, SDL_TEXTUREACCESS_TARGET);
	mTitleBar.SetPosition(2, 2);
	mTitleBar.Create(renderer);
	mTitleBar.AddClickHandler(std::bind(&CForm::OnClose, this, _1));

	mPanel->CreateBlank(w-4, h-32, SDL_TEXTUREACCESS_TARGET);
	mPanel->SetPosition(2, 34);
}

CForm::~CForm()
{
	//printf("Delete form %s\n", mTitleBar.mTitle.mText.c_str());
	RemoveWindow(&mTitleBar);
	RemoveWindow(mPanel);
	delete(mPanel);
}

CForm* CForm::FormFromWindow(CWindow* window)
{
	CWindow* win = window;
	CForm* form = nullptr;
	form = dynamic_cast<CForm*>(win);

	while (form == nullptr)
	{
		win = win->mParent;
		if (win == nullptr)
		{
			break;
		}
		else
		{
			form = dynamic_cast<CForm*>(win);
			if (form)
			{
				break;
			}
		}
	}

	return(form);
}

void CForm::Resize(int w, int h)
{
	CreateBlank(w, h, SDL_TEXTUREACCESS_TARGET);
	mTitleBar.CreateBlank(w - 4, 32, SDL_TEXTUREACCESS_TARGET);
	mPanel->CreateBlank(w - 4, h - 32, SDL_TEXTUREACCESS_TARGET);
}


void CForm::Close()
{
	Hide();
	if (mCloseCallback)
	{
		mCloseCallback(this);
	}
}

void CForm::OnClose(CIcon *)
{
	Hide();
	if (mCloseCallback)
	{
		mCloseCallback(this);
	}
}

void CForm::Draw()
{
	SDL_Renderer* renderer = CApplication::sRenderer;
	int err;

	Uint8 r, g, b, a;
	SDL_Texture* oldTarget;

	if (mVisible == false)
	{
		return;
	}

	oldTarget = SDL_GetRenderTarget(renderer);
	err = SDL_SetRenderTarget(renderer, mTexture);
	if (err == -1)
	{
		printf("CForm::Draw() SDL_SetRenderTarget=-1");
	}
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);


	SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
	SDL_RenderClear(renderer);

	SDL_Rect rect;

	CWindow::Draw();

	rect.x = 0;
	rect.y = 0;
	rect.w = mTexW;
	rect.h = mTexH;
	SDL_SetRenderDrawColor(renderer, 195, 195, 195, 255);
	SDL_RenderDrawRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	rect.x = 1;
	rect.y = 1;
	rect.w = mTexW - 1;
	rect.h = mTexH - 1;
	SDL_RenderDrawRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255);
	SDL_RenderDrawLine(renderer, 1, mTexH - 2, mTexW - 2, mTexH - 2);
	SDL_RenderDrawLine(renderer, mTexW - 2, mTexH - 2, mTexW - 2, 1);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(renderer, 0, mTexH - 1, mTexW, mTexH - 1);
	SDL_RenderDrawLine(renderer, mTexW - 1, mTexH, mTexW - 1, 0);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_SetRenderTarget(renderer, oldTarget);

	CTexture::Draw();
}

CWindow* CForm::OnMouseMotion(int x, int y)
{
	CWindow* handled;

	int mx, my;

	if (mDrag)
	{
		X += (x - mLastMouseX);
		Y += (y - mLastMouseY);
	}
	
	mLastMouseX = x;
	mLastMouseY = y;

	mx = x - this->X;
	my = y - this->Y;

	handled = CWindow::OnMouseMotion(x, y);

	std::stringstream ss;
	ss << "X: " << mx << " Y: " << my;

	//SetTitle(renderer, ss.str().c_str());

	return(handled);
}

CWindow* CForm::OnMouseButton(SDL_MouseButtonEvent e)
{
	CWindow* handled;
	int mx, my;

	if (mVisible == false)
	{
		return(NULL);
	}

	mx = e.x - X;
	my = e.y - Y;

	//printf("%d:%d %d:%d\n", X, Y, mx, my);

	if (e.button == SDL_BUTTON_LEFT && e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (mx > 0 && mx < Width() && my>0 && my < mTexH && CWindow::MouseClickHandled() == false)
		{
			//printf("Bring to front %s\n", mTitleBar.mTitle.mText.c_str());
			CDesktop::sDesktop->BringFormToFront(this);

			if (mx < Width() - mTitleBar.mCloseBox->Width() && my < mTitleBar.mTexH)
			{
				mDrag = true;
			}
		}
	}
	else if (e.button == SDL_BUTTON_LEFT && e.type == SDL_MOUSEBUTTONUP)
	{
		mDrag = false;
	}

	handled = CWindow::OnMouseButton(e);

	return(handled);
}

void CForm::MouseLeave()
{
	CWindow::MouseLeave();

	mDrag = false;
	//printf("Form Mouse Leave\n");
}

void CForm::MouseEnter()
{
	CWindow::MouseEnter();

	mDrag = false;
	//printf("Form Mouse Enter\n");
}

void CForm::KeyDown(SDL_KeyboardEvent e)
{
	if (e.keysym.sym == SDLK_UP)
	{
		mTitleBar.SelectClose();
	}
	else if (e.keysym.sym == SDLK_DOWN)
	{
		mPanel->SelectFirst();
	}
}

void CForm::Show()
{
	CWindow::Show();
	mPanel->SelectFirst();
	CDesktop::sDesktop->BringFormToFront(this);
}

void CForm::Hide()
{
	CWindow::Hide();
}

void CForm::AddOnCloseHandler(std::function<void(CForm*)> callback)
{
	mCloseCallback = callback;
}