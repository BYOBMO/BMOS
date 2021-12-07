#pragma once

#ifdef WINDOWS
#define _HAS_STD_BYTE 0
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <sstream>
#include "CRectangle.h"

using namespace std;

class CTexture
{
public:
	

	enum HAlignment
	{
		Left,
		Center,
		Right
	};

	CTexture();
	virtual ~CTexture();
	void DrawBounds(SDL_Renderer *renderer, int x, int y);
	virtual void Draw(SDL_Rect *srcRect, SDL_Rect *dstRect, float angle);
	void Draw(int x, int y, int w, int h, float angle);
	void Draw(int x, int y, float angle);
	virtual void Draw();
	void DrawScaled(int w, int h);
	void DrawToFit(int w, int h);

	void SetTexture(SDL_Texture *tex);
	void SetPosition(int x, int y);
	bool LoadFromFile(std::string path);
	int GetWidth() { return(mTexW); }
	int GetHeight() { return(mTexH); }
	void setAngle(float a) { Angle = a; }
	float getAngle() { return(Angle); }
	void SetBounds(CRectangle bounds) { mBounds = bounds; }
	void SetBounds(int x, int y, int w, int h) { mBounds.x = x; mBounds.y = y; mBounds.w = w; mBounds.h = h; }
	CRectangle GetBounds() { return(mBounds);}
	void free();
	bool LoadFromRenderedText(SDL_Renderer *renderer, TTF_Font *font, std::string textureText, SDL_Color textColor);
	bool LoadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font, std::string textureText, SDL_Color textColor, int w, int h, HAlignment halign);
	bool CreateBlank(int width, int height, SDL_TextureAccess access);
	void SetAlpha(Uint8 alpha);
	Uint8 GetAlpha() { return(mAlpha); }
	void SetAsRenderTarget();
	bool Collide(CRectangle r);

public:
	int X, Y;
	SDL_Texture *mTexture;
	int mTexW;
	int mTexH;
	float Angle;
	CRectangle mBounds;
	Uint8 mAlpha;
};

