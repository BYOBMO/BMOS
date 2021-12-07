#pragma once
#ifdef WINDOWS
#define _HAS_STD_BYTE 0
#endif
#include "CSurface.h"
#include <vector>
using namespace std;


class CSurfWindow : public CSurface
{
public:
	CSurfWindow();
	CSurfWindow(int w, int h);
	virtual ~CSurfWindow();

	bool mVisible;
	std::vector<CSurfWindow*> mWindows;
	int X, Y;
	Uint32 mBgColor;

	CSurfWindow* mParent;
	virtual void Draw(CSurface*);
	virtual void DrawTexture(SDL_Renderer* renderer);
	virtual void DrawTexture(SDL_Renderer* renderer, int w, int h);
	virtual void DrawTextureScaled(SDL_Renderer* renderer, int w, int h);
	void AddWindow(CSurfWindow* window);

	int Height();
	int Width();
};

