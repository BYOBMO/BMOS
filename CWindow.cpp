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

#include "CWindow.h"
#include "CDesktop.h"

CWindow* CWindow::sMouseMotionHandled=NULL;
CWindow* CWindow::sMouseClickHandled = NULL;
int CWindow::sID = 0;

bool CWindow::MouseClickHandled()
{
	//if (sMouseClickHandled != NULL)
	//{
	//	return(true);
	//}

	return(false);
}

bool CWindow::MouseMotionHandled()
{
	//if (sMouseMotionHandled != NULL)
	//{
	//	return(true);
	//}

	return(false);
}


CWindow::CWindow(): CTexture()
{
	mVisible = true;
	mSelected = false;
	mMouseIn = false;
	mParent = NULL;
	mCanFocus = true;
	mDelete = false;
	mDrag = false;
	mDraggable = false;

	char s[100];
	sprintf(s, "%d", sID++);
	mID = s;
	mDropCallback = nullptr;
}

CWindow::~CWindow()
{
	//printf("~CWindow()\n");
	if (CDesktop::sDesktop != NULL)
	{
		CDesktop::sDesktop->WindowDelete(this);
	}
}

void CWindow::FlagForDeletion()
{
	mDelete = true;
	mVisible = false;
}

bool CWindow::IsFocus()
{
	if (CDesktop::sDesktop->sFocused == this)
	{
		return(true);
	}

	return(false);
}

void CWindow::Focus()
{
	CDesktop::sDesktop->SetFocused(this);
}

void CWindow::MouseEnter()
{
	mMouseIn = true;
	//printf("MouseEnter %s\n", mID.c_str());
}

void CWindow::MouseLeave()
{
	mMouseIn = false;
	//printf("MouseLeave %s\n", mID.c_str());
}

void CWindow::Draw()
{
	if (mVisible == false)
	{
		return;
	}

	vector<CWindow*>::iterator window;

	window = mWindows.begin();
	for (; window != mWindows.end(); window++)
	{
		(*window)->Draw();
	}
}


void CWindow::AddWindow(CWindow* window)
{
	window->mParent = this;
	mWindows.push_back(window);
}

void CWindow::RemoveWindow(CWindow* win)
{
	vector<CWindow*>::iterator window;

	window = mWindows.begin();
	for (; window != mWindows.end(); window++)
	{
		if ((*window) == win)
		{
			mWindows.erase(window);
			break;
		}

	}
}

void CWindow::DeleteWindows()
{
	vector<CWindow*>::iterator window;

	window = mWindows.begin();
	for (; window != mWindows.end(); window++)
	{
		(*window)->DeleteWindows();
		delete(*window);
	}
}

int CWindow::Width()
{
	return (mTexW);
}

CWindow* CWindow::OnMouseMotion(int x, int y)
{
	CWindow* _handled = NULL;
	CWindow* handled = NULL;

	vector<CWindow*>::iterator window;
	int mx, my;

	if (mVisible == false)
	{
		return(NULL);
	}

	mx = x - X;
	my = y - Y;

	int w = Width();

	if (mID == "0" || (mx > 0 && my > 0 && mx < w && my < mTexH && CWindow::MouseMotionHandled()==false))
	{
		_handled = this;
		//if (mMouseIn == false)
		//{
		//	MouserEnter();
		//}

		for (auto rit = std::rbegin(mWindows); rit != std::rend(mWindows); ++rit) {
			handled = (*rit)->OnMouseMotion(mx, my);
			if (handled != NULL)
			{
				break;
			}
		}

	}
	else if (mMouseIn)
	{
		//MouseLeave();
	}


	if (handled != NULL)
	{
		sMouseMotionHandled = handled;
		return(handled);
	}
	else if (_handled != NULL)
	{
		sMouseMotionHandled = _handled;
		return(_handled);
	}
	

	return(NULL);
}

CWindow* CWindow::OnMouseButton(SDL_MouseButtonEvent e)
{
	CWindow* _handled = NULL;
	CWindow* handled = NULL;

	vector<CWindow*>::iterator window;

	if (mVisible == false)
	{
		return(NULL);
	}

	e.x = e.x - X;
	e.y = e.y - Y;

	int w = Width();

	if (mID=="0" || (e.x > 0 && e.y > 0 && e.x < w && e.y < mTexH && CWindow::MouseClickHandled() == false))
	{
		_handled = this;

		for (auto rit = std::rbegin(mWindows); rit != std::rend(mWindows); ++rit) {
			handled = (*rit)->OnMouseButton(e);
			if (handled != NULL)
			{
				break;
			}
		}
	}



	if (handled != NULL)
	{
		sMouseClickHandled = handled;
		return(handled);
	}
	else if (_handled != NULL)
	{
		sMouseClickHandled = _handled;
		return(_handled);
	}

	return(NULL);
}

void CWindow::Focused(bool focus)
{

}

void CWindow::OnTextEvent(SDL_TextInputEvent e)
{
}

CWindow* CWindow::OnKeyDown(SDL_KeyboardEvent e)
{
	CWindow* handled = NULL;

	return(handled);
}

CWindow* CWindow::OnKeyUp(SDL_KeyboardEvent e)
{
	return(NULL);
}

void CWindow::Show()
{
	mVisible = true;
}

void CWindow::Hide()
{
	mVisible = false;
}

void CWindow::GetScreenCoordinates(int* x, int* y)
{
	int _x, _y;
	int px, py;

	_x = X;
	_y = Y;

	if (mParent != NULL)
	{
		mParent->GetScreenCoordinates(&px, &py);
		_x = px + X;
		_y = py + Y;
	}

	*x = _x;
	*y = _y;
}

void CWindow::Dropped(CWindow *theItem, DropAction action, CWindow* dropTarget)
{
	if (mParent != NULL)
	{
		mParent->Dropped(theItem, action, dropTarget);
	}
}

void CWindow::Clicked(SDL_MouseButtonEvent e)
{
	//printf("CWindow::Clicked(%s %d %d)\n", mID.c_str(), e.type, e.button);
}

void CWindow::KeyDown(SDL_KeyboardEvent e)
{
	//printf("CWindow::KeyDown(ID:%s type:%d sym:%d)\n", mID.c_str(), e.type, e.keysym.sym);
}

void CWindow::KeyUp(SDL_KeyboardEvent e)
{
	//printf("CWindow::KeyUp(ID:%s type:%d sym:%d)\n", mID.c_str(), e.type, e.keysym.sym);
}

bool CWindow::IsVisible()
{
	bool vis;

	if (mParent != NULL)
	{
		vis = mParent->IsVisible();
		if (vis == false)
		{
			return(false);
		}
	}

	return(mVisible);
}