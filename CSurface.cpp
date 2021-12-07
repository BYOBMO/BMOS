#include "CSurface.h"
#include <stdio.h>

CSurface::CSurface() 
{
	mSurface = NULL;
	CreateSurface(1, 1);
    mAlpha = 0xFF;
}

CSurface::CSurface(int w, int h): CSurface()
{
	mSurface = NULL;
    CreateSurface(w, h);
}

CSurface::~CSurface()
{
	if (mSurface != NULL)
	{
		SDL_FreeSurface(mSurface);
	}
}

void CSurface::Reset()
{
	free();
	CreateSurface(1, 1);
}

bool CSurface::RenderText(TTF_Font* font, std::string text, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	 mSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
	if (mSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}

	//Return success
	return mSurface != NULL;
}

bool CSurface::RenderText(TTF_Font* font, std::string text, SDL_Color textColor, int w, int h, HAlignment halign)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		mSurface = SDL_CreateRGBSurface(0,
			w,
			h,
			textSurface->format->BitsPerPixel,
			textSurface->format->Rmask,
			textSurface->format->Gmask,
			textSurface->format->Bmask,
			textSurface->format->Amask);

		SDL_Rect src, dst;

		src = { 0,0,textSurface->w,textSurface->h };

		switch (halign)
		{
		case HAlignment::Left:
			dst = { 0, (h - textSurface->h) / 2, textSurface->w, textSurface->h };
			break;
		case HAlignment::Center:
			dst = { (w - textSurface->w) / 2, (h - textSurface->h) / 2, textSurface->w, textSurface->h };
			break;
		case HAlignment::Right:
			dst = { w - textSurface->w, (h - textSurface->h) / 2, textSurface->w, textSurface->h };
			break;
		}

		dst.x += 32;

		int i = SDL_BlitSurface(textSurface, &src, mSurface, &dst);
		if (i != 0)
		{
			printf("%s\n", SDL_GetError());
			return(false);
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mSurface != NULL;
}

void CSurface::CreateSurface(int w, int h)
{
    Uint32 rmask, gmask, bmask, amask;

    if (mSurface != NULL)
    {
        SDL_FreeSurface(mSurface);
    }
    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
//
//    surface = SDL_CreateRGBSurface(0, w, h, 32,
//        rmask, gmask, bmask, amask);
//    if (surface == NULL) {
//        SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
//        exit(1);
//    }

    /* or using the default masks for the depth: */
    mSurface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
    if (mSurface == NULL) {
        printf("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
    }
}

void CSurface::Blit(CSurface* src, int destX, int destY)
{
    if (mSurface == NULL || src->mSurface == NULL)
    {
        return;
    }

    SDL_Rect dst = { destX, destY, 0, 0 };

    SDL_BlitSurface(src->mSurface, NULL, mSurface, &dst);
}

void CSurface::Blit(SDL_Surface* src, int destX, int destY)
{
	if (mSurface == NULL || src == NULL)
	{
		return;
	}

	SDL_Rect dst = { destX, destY, 0, 0 };

	SDL_BlitSurface(src, NULL, mSurface, &dst);
}

void CSurface::BlitEx(CSurface* src, int destX, int destY, HAlignment align)
{
	int bottom;
	int right;
	int _x = 0;

	if (mSurface == NULL || src->mSurface == NULL)
	{
		return;
	}

	bottom = destY + src->mSurface->h;
	right = destX + src->mSurface->w;

	if (bottom > mSurface->h || right > mSurface->w)
	{
		SDL_Surface* oldSurface = mSurface;

		mSurface = SDL_CreateRGBSurface(0,
			std::max(oldSurface->w, right),
			std::max(oldSurface->h, bottom),
			src->mSurface->format->BitsPerPixel,
			src->mSurface->format->Rmask,
			src->mSurface->format->Gmask,
			src->mSurface->format->Bmask,
			src->mSurface->format->Amask);

		if (mSurface == NULL)
		{
			mSurface = oldSurface;
			printf("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
			return;
		}

		_x = 0;
		if (align == HAlignment::Center)
		{
			_x = (mSurface->w - oldSurface->w) / 2;
		}
		Blit(oldSurface, _x, 0);
		SDL_FreeSurface(oldSurface);
	}

	//SDL_Rect dst = { destX, destY, 0, 0 };
	_x = destX;
	if (align == HAlignment::Center)
	{
		_x = (mSurface->w - src->mSurface->w) / 2;
	}
	SDL_Rect dst = { _x, destY, 0, 0 };

	SDL_BlitSurface(src->mSurface, NULL, mSurface, &dst);
}

void CSurface::BlitEx(CSurface* src, int destX, int destY)
{
	BlitEx(src, destX, destY, HAlignment::Left);
}

bool CSurface::RenderTexture(TTF_Font* font, std::string textureText, SDL_Color textColor, int w, int h, HAlignment halign)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		if (w != 0 && h != 0)
		{
			SDL_Surface* newSurf;
			newSurf = SDL_CreateRGBSurface(0,
				w,
				h,
				textSurface->format->BitsPerPixel,
				textSurface->format->Rmask,
				textSurface->format->Gmask,
				textSurface->format->Bmask,
				textSurface->format->Amask);

			SDL_Rect src, dst;

			src = { 0,0,textSurface->w,textSurface->h };

			switch (halign)
			{
			case HAlignment::Left:
				dst = { 0, (h - textSurface->h) / 2, textSurface->w, textSurface->h };
				break;
			case HAlignment::Center:
				dst = { (w - textSurface->w) / 2, (h - textSurface->h) / 2, textSurface->w, textSurface->h };
				break;
			case HAlignment::Right:
				dst = { w - textSurface->w, (h - textSurface->h) / 2, textSurface->w, textSurface->h };
				break;
			}


			int i = SDL_BlitSurface(textSurface, &src, newSurf, &dst);
			if (i != 0)
			{
				printf("%s\n", SDL_GetError());
				return(false);
			}

			mSurface = newSurf;
			SDL_FreeSurface(textSurface);
		}
		else
		{
			mSurface = textSurface;
		}
	}

	//Return success
	return mSurface != NULL;
}

void CSurface::free()
{
	//Free texture if it exists
	if (mSurface != NULL)
	{
		SDL_FreeSurface(mSurface);
		mSurface = NULL;
	}
}

bool CSurface::LoadImage(std::string path)
{
	//Get rid of preexisting texture
	free();

	//Load image at specified path
	mSurface = IMG_Load(path.c_str());
	if (mSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}

	return mSurface != NULL;
}