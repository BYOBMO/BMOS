#include "CListBox.h"
#include "CDesktop.h"

#include <functional>

CListBox::CListBox(): CWindow()
{
	using namespace std::placeholders;

	mListHeight = 0;
	mScrollPos = 0;
	mScrollBar = new CVScrollBar();
	mScrollBar->AddOnScrollHandler(std::bind(&CListBox::OnScroll, this, _1));
	AddWindow(mScrollBar);
}

CListBox::~CListBox()
{
	//printf("~CListBox()\n");
	Clear();
	delete(mScrollBar);
	mWindows.clear();
}

void CListBox::SetSize(int w, int h)
{
	CreateBlank(w, h, SDL_TEXTUREACCESS_TARGET);
	mScrollBar->SetHeight(mTexH);
	mScrollBar->SetPosition(Width() - mScrollBar->Width(), 0);
}

void CListBox::Draw()
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

	if (IsFocus())
	{
		SDL_Rect rect;

		
		SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
		rect = { 0,0, Width() - mScrollBar->Width(), GetHeight() - 4 };
		SDL_RenderDrawRect(renderer, &rect);

		rect = { 1,1, Width() - mScrollBar->Width()-2, GetHeight() - 6 };
		SDL_RenderDrawRect(renderer, &rect);
	}

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_SetRenderTarget(renderer, oldTarget);

	CTexture::Draw();
}

void CListBox::Focused(bool isFocus)
{
	//printf("CListBox::Focus() %s %d\n", mID.c_str(), isFocus);
}

void CListBox::Clear()
{
	int i;

	for (auto rit = mWindows.begin(); rit != mWindows.end(); )
	{
		CListBoxItem* lbi = dynamic_cast<CListBoxItem*>(*rit);

		if (lbi)
		{
			//printf("Delete lbi\n");

			rit = mWindows.erase(rit);
		}
		else
		{
			++rit;
		}
	}

	for (i = 0; i < mItems.size(); i++)
	{
		//CDesktop::sDesktop->WindowDelete(mItems[i]);
		delete(mItems[i]);
	}

	mItems.clear();
}

void CListBox::RemoveItem(CListBoxItem* item)
{
	int pos = -1;

	for (int i = 0; i < mItems.size(); i++)
	{
		if (mItems[i] == item)
		{
			pos = i;
			break;
		}
	}

	if (pos >= 0)
	{
		mItems.erase(mItems.begin() + pos);
		LayoutItems();
		mScrollBar->SetRange(0, ScrollSize());
	}

	for (int i = 0; i < mWindows.size(); i++)
	{
		if (mWindows[i] == item)
		{
			pos = i;
			break;
		}
	}

	if (pos >= 0)
	{
		mWindows.erase(mWindows.begin() + pos);
	}

	for (int i = 0; i < mItemRemovedCallbacks.size(); i++)
	{
		if (mItemRemovedCallbacks[i])
		{
			mItemRemovedCallbacks[i](item);
		}
	}
}

CListBoxItem* CListBox::AddItem(std::string column1, std::string column2)
{
	return(AddItem(column1, column2, CText::cBlack));
}

CListBoxItem* CListBox::AddItem(std::string column1, std::string column2, SDL_Color color)
{
	CListBoxItem* lbi = new CListBoxItem();
	this->AddWindow(lbi);
	lbi->SetColor(color);
	mItems.push_back(lbi);
	lbi->SetPosition(0, 0);

	lbi->SetText(column1, column2);
	lbi->CreateTextSurface(Width() - mScrollBar->Width(), 0);
	lbi->CreateTextureFromSurface();

	LayoutItems();

	//printf("List H = %d\n", mListHeight);

	mScrollBar->SetRange(0, ScrollSize());
	return(lbi);
}

CListBoxItem* CListBox::AddItem(std::string column1, std::string column2, SDL_Color color, CFile::Type iconType)
{
	CListBoxItem* lbi = new CListBoxItem();
	this->AddWindow(lbi);
	lbi->SetColor(color);
	mItems.push_back(lbi);
	lbi->SetPosition(0, 0);
	lbi->mIconType = iconType;
	lbi->SetText(column1, column2);
	lbi->CreateTextSurface(Width() - mScrollBar->Width(), 0);
	lbi->CreateTextureFromSurface();

	LayoutItems();

	//printf("List H = %d\n", mListHeight);

	mScrollBar->SetRange(0, ScrollSize());
	return(lbi);
}

void CListBox::LayoutItems()
{
	int i;
	mListHeight = 0;
	mListWidth = 0;

	for (i = 0; i < mItems.size(); i++)
	{
		mItems[i]->SetPosition(0, (i * (mItems[i]->GetHeight() + 0)) - mScrollPos);
		mListHeight += (mItems[i]->GetHeight() + 0);
		if (mItems[i]->Width() > mListWidth)
		{
			mListWidth = mItems[i]->Width();
		}
	}

	if (mListWidth + mScrollBar->Width() > Width())
	{
		//printf("Resizing from %d to %d\n", Width(), mListWidth);
		SetSize(mListWidth + mScrollBar->Width(), mTexH);
	}

	for (i = 0; i < mItems.size(); i++)
	{
		CListBoxItem* lbi;

		lbi = mItems[i];

		if (lbi->Width() < Width() - mScrollBar->Width())
		{
			lbi->CreateTextSurface(Width() - mScrollBar->Width(), 0);
			lbi->CreateTextureFromSurface();
		}
		//lbi->SetText(lbi->mText.mText, CText::sFont36, Width() - mScrollBar->Width(), lbi->GetHeight());
	}

	mScrollBar->SetRange(0, ScrollSize());
}

int CListBox::ScrollSize()
{
	int i;

	i = mListHeight - mTexH;

	return(i);
}

void CListBox::SetScrollPos(int pos)
{
	//printf("scrollpos = %d\n", pos);
	mScrollPos = pos;
	LayoutItems();
}

void CListBox::OnScroll(int position)
{
	//printf("ScrollPos = %d\n", position);
	SetScrollPos(position);
}

void CListBox::KeyDown(SDL_KeyboardEvent e)
{
	int len;
	int i;
	int selected = -1;
	CListBoxItem* item = NULL;

	CWindow::KeyDown(e);
	len = (int)mItems.size();

	for (i = 0; i < len; i++)
	{
		item = dynamic_cast<CListBoxItem*>(mItems[i]);
		if (item)
		{
			if (item->IsFocus()) // XXX
			{
				selected = i;
				break;
			}
		}
	}

	//ClearAll();

	if (selected != -1)
	{
		if (e.keysym.sym == SDLK_UP)
		{

			selected--;
			if (selected < 0)
			{
				selected = 0;

				if (mParent != NULL)
				{
					mParent->KeyDown(e);
				}

				return;
			}
		}
		else if (e.keysym.sym == SDLK_DOWN)
		{

			selected++;
			if (selected >= len)
			{
				selected = len - 1;
			}
		}
		else if (e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_LALT || e.keysym.sym == SDLK_RALT)
		{
			item->Pressed();
		}
		CDesktop::sDesktop->SetFocused(mItems[selected]);
		//mItems[selected]->mSelected = true; XXX

		if ((mItems[selected]->Y + mItems[selected]->mItemSurface->mSurface->h) > mTexH)
		{
			int delta = (mItems[selected]->Y + mItems[selected]->mItemSurface->mSurface->h) - mTexH;

			//printf("Scroll %d\n", delta);
			mScrollBar->SetPos(mScrollPos + delta);
		}
		else if (mItems[selected]->Y < 0)
		{
			int pos = selected * mItems[selected]->mItemSurface->mSurface->h;

			//printf("Scroll %d\n", pos);
			mScrollBar->SetPos(pos);
		}
	}
	else if (len > 0 && (e.keysym.sym == SDLK_DOWN || e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_LALT || e.keysym.sym == SDLK_RALT))
	{
		CDesktop::sDesktop->SetFocused(mItems[0]);
		//mItems[0]->mSelected = true; XXX
	}
	else if (mParent != NULL)
	{
		mParent->KeyDown(e);
	}
}

void CListBox::AddWindow(CWindow* window)
{
	window->mParent = this;
	mWindows.insert(mWindows.begin(), window);
}

void CListBox::AddItemRemovedHandler(std::function<void(CListBoxItem*)> callback)
{
	mItemRemovedCallbacks.push_back(callback);
}