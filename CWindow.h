#pragma once

#include "CTexture.h"
#include <SDL2/SDL.h>

#include <vector>
#include <functional>

class CWindow: public CTexture
{
public:
	enum DropAction { None, Delete, Move, Copy, Invoke };

	CWindow();
	virtual ~CWindow();
	
	std::string mID;

	static bool MouseClickHandled();
	static bool MouseMotionHandled();
	static CWindow* sMouseMotionHandled;
	static CWindow* sMouseClickHandled;
	static int sID;

	CWindow* mParent;

	bool mSelected;
	bool mVisible;
	bool mCanFocus;
	bool mDelete;
	bool mDrag;
	bool mDraggable;
	bool mMouseIn;

	std::vector<CWindow*> mWindows;
	std::function<void(CWindow*)> mDropCallback;

	virtual void Draw();
	void AddWindow(CWindow* window);
	void RemoveWindow(CWindow *win);

	void DeleteWindows();
	virtual CWindow* OnMouseMotion(int x, int y);
	virtual CWindow* OnMouseButton(SDL_MouseButtonEvent e);
	virtual CWindow* OnKeyDown(SDL_KeyboardEvent e);
	virtual CWindow* OnKeyUp(SDL_KeyboardEvent e);
	virtual void OnTextEvent(SDL_TextInputEvent e);

	virtual void MouseEnter();	
	virtual void MouseLeave();
	virtual void Focused(bool focus);
	bool IsFocus();
	void Focus();

	virtual void Clicked(SDL_MouseButtonEvent e);
	virtual void KeyDown(SDL_KeyboardEvent e);
	virtual void KeyUp(SDL_KeyboardEvent e);

	void Show();
	void Hide();
	void FlagForDeletion();

	virtual int Width();

	void GetScreenCoordinates(int* x, int* y);
	virtual void Dropped(CWindow* theItem, DropAction action, CWindow* dropTarget);
	bool IsVisible();

private:

};