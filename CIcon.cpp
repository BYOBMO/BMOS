#include "CIcon.h"
#include "CTexture.h"
#include "CDesktop.h"

CIcon::CIcon(std::string path, int numFrames): CWindow()
{

	mDrag = false;
	mDragX=true;
	mDragY=true;
	mLastMouseX = 0;
	mLastMouseY = 0;
	mDraggable = false;

	m_CurFrame = 0;
	m_NumFrames = numFrames;

	LoadFromFile(path);
	m_Width = mTexW / numFrames;

	mBounds = { 0,0, m_Width, mTexH };
}

void CIcon::SetFrame(int frame)
{

}

void CIcon::Draw()
{
	int frame = 0;

	int x;
	SDL_Rect src, dst;

	if (mVisible == false)
	{
		return;
	}

	if ((mMouseIn || IsFocus()) && m_NumFrames >= 2)
	{
		frame = 1;
		if (m_NumFrames >= 3)
		{
			frame = 2;
		}

	}

	//if (mSelected && m_NumFrames>=3)
	//{
	//	frame = 2;
	//}

	x = m_Width * frame;

	src.x = x;
	src.y = 0;
	src.w = m_Width;
	src.h = mTexH;

	dst.x = X;
	dst.y = Y;
	dst.w = m_Width;
	dst.h = mTexH;

	CTexture::Draw(&src, &dst, 0.0);
}

CWindow* CIcon::OnMouseMotion(int x, int y)
{
	CWindow* handled = NULL;

	if (mDrag && mDraggable)
	{
		if (mDragX)
		{
			X += (x - mLastMouseX);
		}

		if (mDragY)
		{
			if (mLastMouseY > Y + mTexH || mLastMouseY < 0)
			{

			}
			else
			{
				Y += (y - mLastMouseY);
			}
		}

		if (mOnPositionCallback)
		{
			mOnPositionCallback(this);
		}
	}
	
	mLastMouseX = x;
	mLastMouseY = y;

	handled = CWindow::OnMouseMotion(x, y);


	return(handled);
}

void CIcon::Clicked(SDL_MouseButtonEvent e)
{
	CWindow::Clicked(e);

	if (e.button == SDL_BUTTON_LEFT && e.type == SDL_MOUSEBUTTONDOWN && e.clicks==1)
	{


		if (this->mParent == CDesktop::sDesktop)
		{
			CDesktop::sDesktop->BringIconToFront(this);
		}

		if (mDraggable)
		{
			mDrag = true;
		}

		if (mClickCallback)
		{
			mClickCallback(this);
		}
		else
		{
			//printf("CIcon::Clicked\n");
		}

	}
	else if (e.button == SDL_BUTTON_LEFT && e.type == SDL_MOUSEBUTTONDOWN && e.clicks == 2)
	{
		if (this->mParent == CDesktop::sDesktop)
		{
			CDesktop::sDesktop->BringIconToFront(this);
		}

		if (mDoubleClickCallback)
		{
			mDoubleClickCallback(this);
		}
		else
		{
			//printf("CIcon::DoubleClicked\n");
		}

	}
	else if (e.button == SDL_BUTTON_LEFT && e.type == SDL_MOUSEBUTTONUP)
	{
		mDrag = false;
	}
}

void CIcon::Pressed()
{
	if (this->mParent == CDesktop::sDesktop)
	{
		CDesktop::sDesktop->BringIconToFront(this);
	}

	if (mDraggable)
	{
		mDrag = true;
	}

	if (mClickCallback)
	{
		mClickCallback(this);
	}
	else
	{
		//printf("CIcon::Clicked\n");
	}
}

void CIcon::KeyDown(SDL_KeyboardEvent e)
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

void CIcon::AddHandler(std::function<void(int)> callback)
{
	mCallback = callback;
}

void CIcon::AddClickHandler(std::function<void(CIcon *icon)> callback)
{
	mClickCallback = callback;
}

void CIcon::AddDoubleClickHandler(std::function<void(CIcon* icon)> callback)
{
	mDoubleClickCallback = callback;
}

void  CIcon::AddOnPositionHandler(std::function<void(CIcon*)> callback)
{
	mOnPositionCallback = callback;
}

int CIcon::Width()
{
	return(m_Width);
}

void CIcon::MouseLeave()
{
	CWindow::MouseLeave();

	mDrag = false;
	//printf("Icon Mouse Leave\n");
}