#pragma once
#ifdef WINDOWS
#define _HAS_STD_BYTE 0
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>

using namespace std;

class CSurface
{
public:
	enum class HAlignment
	{
		Left,
		Center,
		Right
	};

	
	SDL_Surface* mSurface;
	Uint8 mAlpha;

	CSurface();
	CSurface(int w, int h);
	virtual ~CSurface();

	void free();

	void CreateSurface(int w, int h);
	void Blit(CSurface* src, int destX, int destY);
	void Blit(SDL_Surface* src, int destX, int destY);
	void BlitEx(CSurface* src, int destX, int destY);
	void BlitEx(CSurface* src, int destX, int destY, HAlignment align);
	bool RenderTexture(TTF_Font* font, std::string textureText, SDL_Color textColor, int w, int h, HAlignment halign);
	bool RenderText(TTF_Font* font, std::string text, SDL_Color textColor, int w, int h, HAlignment halign);
	bool RenderText(TTF_Font* font, std::string text, SDL_Color textColor);
	bool LoadImage(std::string path);

	void Reset();
};

