#include "CDragWindow.h"
#include "CApplication.h"

CDragWindow::CDragWindow() : CWindow()
{
	CreateBlank(100, 100, SDL_TEXTUREACCESS_TARGET);
	mXOffset = 0;
	mYOffset = 0;
	mOldTarget = NULL;
	mDragItem = NULL;
}

void CDragWindow::SetMouseOffset(int x, int y)
{
	mXOffset = x;
	mYOffset = y;
}

void CDragWindow::SetCanvasSize(int w, int h)
{
	Uint8 r, g, b, a;
	SDL_Texture* oldTarget;

	SDL_Renderer* renderer = CApplication::sRenderer;

	CreateBlank(w, h, SDL_TEXTUREACCESS_TARGET);

	oldTarget = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, mTexture);

	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_SetRenderTarget(renderer, oldTarget);
}

void CDragWindow::SetBackground(int r, int g, int b)
{
	SDL_Renderer* renderer = CApplication::sRenderer;


	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderClear(renderer);
}

void CDragWindow::BeginDraw()
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	mOldTarget = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, mTexture);

	SDL_GetRenderDrawColor(renderer, &mOldR, &mOldG, &mOldB, &mOldA);
}

void CDragWindow::EndDraw()
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	SDL_SetRenderDrawColor(renderer, mOldR, mOldG, mOldB, mOldA);
	SDL_SetRenderTarget(renderer, mOldTarget);
}

void CDragWindow::DrawTexture(CTexture *tex, int x, int y)
{
	//SDL_Renderer* renderer = CApplication::sRenderer;
	//SDL_Rect rect;

	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//rect = { x, y, Width(), mTexH };


	tex->Draw(x, y, 0.0);
	//SDL_RenderDrawRect(renderer, &rect);
}

void CDragWindow::Draw()
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	Uint8 r, g, b, a;


	if (mVisible == false)
	{
		return;
	}

	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128);
	SDL_Rect rect;

	rect = { X-1, Y-1, Width()+2, mTexH +2};

	//SDL_RenderFillRect(renderer, &rect);

	//CWindow::Draw();

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
	SetAlpha(128);
	CTexture::Draw();
}

void CDragWindow::AddDropHandler(std::function<void(CDragWindow*)> callback)
{
	mDropCallbacks.push_back(callback);
}

void CDragWindow::Drop()
{
	for (auto cb = std::begin(mDropCallbacks); cb != std::end(mDropCallbacks); ++cb) {
		std::function<void(CDragWindow*)> callback;

		callback = *cb;

		if (callback)
		{
			callback(this);
		}
		else
		{
			printf("CDragWindow::Drop: No handler\n");
		}
	}
}

CWindow* CDragWindow::OnMouseMotion(int x, int y)
{
	CWindow* handled=NULL;

	int _x, _y;

	SDL_GetMouseState(&_x, &_y);

	X = x - mXOffset;
	Y = y - mYOffset;


	return(handled);
}


CWindow* CDragWindow::OnMouseButton(SDL_MouseButtonEvent e)
{
	int mx, my;

	if (e.button == SDL_BUTTON_LEFT && e.type == SDL_MOUSEBUTTONUP)
	{
		Drop();
	}

	return(NULL);
}
