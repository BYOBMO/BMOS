#pragma once
#include "CWindow.h"
#include "CTexture.h"
#include "CText.h"
#include "CTitleBar.h"
#include "CPanel.h"

class CForm: public CWindow
{
public:
	CTitleBar mTitleBar;
	CPanel *mPanel;
	
	int mLastMouseX, mLastMouseY;
	static CForm* FormFromWindow(CWindow* window);

	CForm();
	CForm(int x, int y, int w, int h);
	virtual ~CForm();

	virtual void MouseLeave();
	virtual void MouseEnter();
	virtual CWindow* OnMouseMotion(int x, int y);
	virtual CWindow* OnMouseButton(SDL_MouseButtonEvent e);
	void KeyDown(SDL_KeyboardEvent e);

	virtual void Draw();
	void SetTitle(std::string title);
	//CText mTitle;

	virtual void Show();
	virtual void Hide();
	void Resize(int w, int h);

	virtual void OnClose(CIcon*);
	virtual void Close();

	std::function<void(CForm*)> mCloseCallback;
	void AddOnCloseHandler(std::function<void(CForm*)> callback);
};

