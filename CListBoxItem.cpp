#include "CListBoxItem.h"
#include "CDesktop.h"
#include "Utils.h"
#include "CDragWindow.h"
#include "CFile.h"
#include "CFiles.h"
CListBoxItem::CListBoxItem() : CWindow()
{
	mDrag = false;
	mLastMouseX = 0;
	mLastMouseY = 0;
	mColor = CText::cBlack;
	
	mMinWidth = 0;
	mColumn1 = "";
	mColumn2 = "";

	mIconType = CFile::Type::NONE;

	mItemSurface = NULL;
}


CListBoxItem::~CListBoxItem()
{
	//printf("~CListBoxItem() %s\n", mID.c_str());
	if (mItemSurface != NULL)
	{
		delete(mItemSurface);
	}
}

void CListBoxItem::CreateTextSurface(int w, int h)
{
	CSurface *left, *right;
	int _w, _h;
	if (mItemSurface == NULL)
	{
		mItemSurface = new CSurface();
	}

	left = new CSurface();
	right = new CSurface();
	left->RenderText(CText::sFontSmall, mColumn1, mColor);
	right->RenderText(CText::sFontSmall, mColumn2, mColor);

	mMinWidth = left->mSurface->w + 20 + right->mSurface->w;
	mMinWidth+=32;
	//_w = w;
	//if (_w == 0)
	//{
		_w = std::max(mMinWidth, w);
	//}

	//_h = h;
	//if (_h == 0)
	//{
		_h = std::max(left->mSurface->h, h);;
	//}



	mItemSurface->RenderText(CText::sFontSmall, mColumn1, mColor, _w, _h+10, CSurface::HAlignment::Left);

	mItemSurface->Blit(right, (_w - right->mSurface->w), (mItemSurface->mSurface->h - right->mSurface->h)/2);

	SDL_Surface* imgSurf = CFile::sIcons[(int)mIconType]->mSurface;

	if (imgSurf == NULL)
	{
		
	}

	mItemSurface->Blit(imgSurf, 0, (mItemSurface->mSurface->h - imgSurf->h) / 2);


	delete(right);
	delete(left);
}

void CListBoxItem::CreateTextureFromSurface()
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	if (mItemSurface != NULL)
	{
		free();
		mTexture = SDL_CreateTextureFromSurface(renderer, mItemSurface->mSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}

		mTexH = mItemSurface->mSurface->h;
		mTexW = mItemSurface->mSurface->w;
	}
}

void CListBoxItem::SetText(std::string col1)
{
	mColumn1 = col1;

	if (mColumn1 == "")
	{
		mColumn1 = " ";
	}
}

void CListBoxItem::SetText(std::string col1, std::string col2)
{
	mColumn1 = col1;
	mColumn2 = col2;

	if (mColumn1 == "")
	{
		mColumn1 = " ";
	}

	if (mColumn2 == "")
	{
		mColumn2 = " ";
	}
}

//
//void CListBoxItem::SetText(std::string title, TTF_Font* font)
//{
//	mText.SetText(font, title, mColor);
//	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
//}
//
//void CListBoxItem::SetText(std::string title, TTF_Font* font, int w, int h)
//{
//	mText.SetText(font, title, mColor, w, h, CTexture::HAlignment::Left);
//	CreateBlank(mText.mTexW, mText.mTexH, SDL_TEXTUREACCESS_TARGET);
//}

void CListBoxItem::SetColor(SDL_Color color)
{
	mColor = color;
}

void CListBoxItem::Dropped(CWindow *theItem, DropAction action, CWindow* dropTarget)
{
	if (action == DropAction::Delete)
	{
		//printf("Delete %s\n", mColumn1.c_str());
		if (mParent != NULL)
		{
			CListBox *listbox;
			listbox = dynamic_cast<CListBox*>(mParent);
			if (listbox)
			{
				listbox->RemoveItem(this);
				mDelete = true;
			}
		}
	}
	else if (action == DropAction::Copy)
	{
		if (mParent != NULL)
		{
			mParent->Dropped(theItem, action, dropTarget);
		}

	}
}

void CListBoxItem::Draw()
{
	SDL_Renderer* renderer = CApplication::sRenderer;
	int w, h;

	

	Uint8 r, g, b, a;

	if (mVisible == false)
	{
		return;
	}

	SDL_GetRendererOutputSize(renderer, &w, &h);
	if (Y + mItemSurface->mSurface->h < 0 || Y > h)
	{
		return;
	}

	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

	if (mMouseIn) // XXX
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
	rect.h = mItemSurface->mSurface->h;
	SDL_RenderFillRect(renderer, &rect);


	//mText.SetPosition(X, Y);
	//mText.Draw();

	if (mTexture != NULL)
	{
		//Set rendering space and render to screen
		SDL_Rect renderQuad = { X, Y, mItemSurface->mSurface->w, mItemSurface->mSurface->h };

		//Render to screen
		SDL_SetTextureAlphaMod(mTexture, mAlpha);

		SDL_RenderCopyEx(renderer, mTexture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
	}

	rect.x = X;
	rect.y = Y;
	rect.w = mTexW;
	rect.h = mItemSurface->mSurface->h;
	SDL_SetRenderDrawColor(renderer, 195, 195, 195, 255);

	if (mMouseIn)
	{
		SDL_RenderDrawRect(renderer, &rect);
	}


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


void CListBoxItem::Clicked(SDL_MouseButtonEvent e)
{
	//printf("lbi %d\n", e.clicks);
	CWindow::Clicked(e);
	if (e.button == SDL_BUTTON_LEFT && e.type == SDL_MOUSEBUTTONDOWN && e.clicks==1)
	{
		if (mDraggable)
		{
			mDrag = true;
		}

		for (auto cb = std::begin(mClickCallbacks); cb != std::end(mClickCallbacks); ++cb) {
			std::function<void(CListBoxItem*)> callback;

			callback = *cb;

			if (callback)
			{
				callback(this);
			}
			else
			{
				printf("CListBoxItem: No handler\n");
			}
		}
	}
	if (e.button == SDL_BUTTON_LEFT && e.type == SDL_MOUSEBUTTONDOWN && e.clicks == 2)
	{
		Pressed();
	}
	else if (e.button == SDL_BUTTON_LEFT && e.type == SDL_MOUSEBUTTONUP)
	{
		mDrag = false;
	}
}


void CListBoxItem::AddClickHandler(std::function<void(CListBoxItem*)> callback)
{
	mClickCallbacks.push_back(callback);
}

void CListBoxItem::AddDoubleClickHandler(std::function<void(CListBoxItem*)> callback)
{
	mDoubleClickCallbacks.push_back(callback);
}


void CListBoxItem::Pressed()
{
	for (auto cb = std::begin(mDoubleClickCallbacks); cb != std::end(mDoubleClickCallbacks); ++cb) {
		std::function<void(CListBoxItem*)> callback;

		callback = *cb;

		if (callback)
		{
			callback(this);
		}
		else
		{
			printf("CListBoxItem::DoubleClick No handler\n");
		}
	}
}


CWindow* CListBoxItem::OnMouseMotion(int x, int y)
{
	CWindow* handled;

	int mx, my;

	mx = x - X;
	my = y - Y;

	if (mDrag)
	{
		int dx = (x - mLastMouseX);
		int dy = (y - mLastMouseY);

		if (abs(dx) > 15 || abs(dy) > 15)
		{
			CDragWindow* dragWin = new CDragWindow();
			dragWin->mDragItem = this;
			dragWin->SetCanvasSize(Width(), mItemSurface->mSurface->h);

			//dragWin->SetMouseOffset(mx, my);
			dragWin->SetMouseOffset(Width()/2, mItemSurface->mSurface->h /2);
			dragWin->BeginDraw();
			dragWin->SetBackground(200, 200, 200);
			dragWin->DrawTexture(this, 0, 0);
			dragWin->EndDraw();
			CDesktop::sDesktop->SetDragWindow(dragWin);

			//printf("Start drag\n");
			mDrag = false;
		}
	}
	else
	{
		mLastMouseX = x;
		mLastMouseY = y;
	}

	handled = CWindow::OnMouseMotion(x, y);

	//mSelected = false;
	//mDrawColor = mBgColor;
	//if (mx > 1 && mx < mTexW - 1 && my>1 && my < mTexH - 1)
	//{
	//	mSelected = true;
	//}

	return(handled);
}



void CListBoxItem::KeyDown(SDL_KeyboardEvent e)
{
	int x, y;

	CWindow::KeyDown(e);
	if (e.keysym.sym == SDLK_DOWN || e.keysym.sym == SDLK_UP)
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