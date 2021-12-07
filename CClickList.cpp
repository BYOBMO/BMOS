#include "CClickList.h"
#include "Utils.h"

CClickList::CClickList():CWindow()
{
	mActive = false;
	mWidth = 100;
	mHeight = 36;
	mIndex = -1;
}

CClickList::CClickList(int w, int h) : CClickList()
{
	mWidth = w;
	mHeight = h;
}

void CClickList::AddItem(std::string str)
{
	CText* text = new CText();
	text->SetText(CText::sFont36, str, CText::cBlack, mWidth, mHeight);
	mList.push_back(text);

	if (mIndex == -1)
	{
		mIndex = 0;
	}
}

void CClickList::Draw()
{
	SDL_Renderer* renderer = CTexture::sRenderer;
	CText* text;

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
	rect.w = mWidth;
	rect.h = mHeight;
	SDL_RenderFillRect(renderer, &rect);

	if (mIndex >= 0 && mIndex < mList.size())
	{
		text = mList[mIndex];

		text->SetPosition(X, Y);
		text->Draw();
	}

	rect.x = X;
	rect.y = Y;
	rect.w = mWidth;
	rect.h = mHeight;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);


	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

bool CClickList::Click()
{
	bool handled = false;
	for (auto cb = std::begin(mCallbacks); cb != std::end(mCallbacks); ++cb) {
		std::function<void(CButton*)> callback;

		callback = *cb;

		if (callback)
		{
			callback(this);
			handled = true;
		}
		else
		{
			printf("CButton: No handler\n");
		}
	}

	return(handled);
}

bool CClickList::OnMouseButton(SDL_MouseButtonEvent e)
{
	int x, y;

	if (mVisible == false)
	{
		return(false);
	}

	bool handled = false;

	int mx, my;

	mx = e.x - X;
	my = e.y - Y;

	//printf("%d:%d %d:%d\n", X, Y, mx, my);

	if (e.button == SDL_BUTTON_LEFT && e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (mx > 1 && mx < mTexW - 1 && my>1 && my < mTexH - 1)
		{
			handled = Click();
		}
	}

	return(handled);
}

bool CClickList::OnKeyDown(SDL_KeyboardEvent e)
{
	bool handled = false;
	int x, y;

	if (e.keysym.sym == SDLK_DOWN || e.keysym.sym == SDLK_UP)
	{
		if (mParent != NULL)
		{
			handled = mParent->OnKeyDown(e);
		}
	}
	else if ((e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_LALT || e.keysym.sym == SDLK_RALT) && mSelected)
	{
		handled = Click();
	}
	return(handled);
}