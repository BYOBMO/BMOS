#pragma once
#include "CWindow.h"
#include "CText.h"
#include "CIcon.h"

class CTitleBar : public CWindow
{
public:

	//static CTexture *sCloseBox;

	CText mTitle;
	CIcon *mCloseBox;

	CTitleBar();

	void Create(SDL_Renderer* renderer);
	void SetTitle(std::string title);

	static void Init();

	virtual void Draw();
	void SelectClose();
	void KeyDown(SDL_KeyboardEvent e);
	void AddClickHandler(std::function<void(CIcon*)> callback);
};

