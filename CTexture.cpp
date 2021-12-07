#include "CTexture.h"
#include "CApplication.h"

#include <stdio.h>
#include <sstream>


CTexture::CTexture()
{
	mTexture = NULL;
	Angle = 0.0;
	X = 0;
	Y = 0;
	mTexW = 0;
	mTexH = 0;
	mBounds = { 0,0,0,0 };
	mAlpha = 0xFF;
}


CTexture::~CTexture()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
	}
}

void CTexture::DrawBounds(SDL_Renderer *renderer, int x, int y)
{
	if (mTexture != NULL)
	{
		//Render to screen
		if (mBounds.w > 0 && mBounds.h > 0)
		{

			SDL_Rect r;

			r.x = x + mBounds.x;
			r.y = y + mBounds.y;
			r.w = mBounds.w;
			r.h = mBounds.h;

			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xff);
			SDL_RenderDrawRect(renderer, &r);
		}
	}
}

void CTexture::SetAlpha(Uint8 alpha)
{
	mAlpha = alpha;
}

void CTexture::Draw(SDL_Rect *srcRect, SDL_Rect *dstRect, float angle)
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	if (mTexture != NULL)
	{
		SDL_SetTextureAlphaMod(mTexture, mAlpha);
		//Render to screen
		SDL_RenderCopyEx(renderer, mTexture, srcRect, dstRect, angle, NULL, SDL_FLIP_NONE);
	}
}

void CTexture::Draw(int x, int y, int w, int h, float angle)
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	if (mTexture != NULL)
	{
		//Set rendering space and render to screen
		SDL_Rect renderQuad = { x, y, w, h };

		//Set clip rendering dimensions
		//if (clip != NULL)
		//{
		//	renderQuad.w = clip->w;
		//	renderQuad.h = clip->h;
		//}

		//Render to screen
		SDL_SetTextureAlphaMod(mTexture, mAlpha);
		SDL_RenderCopyEx(renderer, mTexture, NULL, &renderQuad, angle, NULL, SDL_FLIP_NONE);
	}
}

void CTexture::Draw(int x, int y, float angle)
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	if (mTexture != NULL)
	{
		//Set rendering space and render to screen
		SDL_Rect renderQuad = { x, y, mTexW, mTexH };

		//Set clip rendering dimensions
		//if (clip != NULL)
		//{
		//	renderQuad.w = clip->w;
		//	renderQuad.h = clip->h;
		//}

		//Render to screen
		SDL_SetTextureAlphaMod(mTexture, mAlpha);
		SDL_RenderCopyEx(renderer, mTexture, NULL, &renderQuad, angle, NULL, SDL_FLIP_NONE);
	}
}

void CTexture::Draw()
{
	int i;
	SDL_Renderer* renderer = CApplication::sRenderer;

	if (mTexture != NULL)
	{

		//Set rendering space and render to screen
		SDL_Rect renderQuad = { X, Y, mTexW, mTexH };


		//Render to screen
		i = SDL_SetTextureAlphaMod(mTexture, mAlpha);
		if (i == -1)
		{
			printf("CTexture::Draw() DL_SetTextureAlphaMod==-1");
		}

		i = SDL_RenderCopyEx(renderer, mTexture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
		if (i == -1)
		{
			printf("CTexture::Draw() DL_SetTextureAlphaMod==-1");
		}
	}
	else
	{
		printf("CTexture::Draw() mTexture==NULL\n");
	}
}

void CTexture::DrawScaled( int w, int h)
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	if (mTexture != NULL)
	{
		float oldX, oldY;
		float xscale, yscale;

		SDL_RenderGetScale(renderer, &oldX, &oldY);

		//Set rendering space and render to screen
		SDL_Rect renderQuad = { X, Y, mTexW, mTexH };
		//SDL_Rect renderQuad = { X, Y, 320, 240 };

		//Set clip rendering dimensions
		//if (clip != NULL)
		//{
		//	renderQuad.w = clip->w;
		//	renderQuad.h = clip->h;
		//}

		//Render to screen
		SDL_SetTextureAlphaMod(mTexture, mAlpha);

		xscale = float(w) / (float)mTexW;
		yscale = float(h) / (float)mTexH;

		SDL_RenderSetScale(renderer, xscale, yscale);
		SDL_RenderCopyEx(renderer, mTexture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderSetScale(renderer, oldX, oldY);
	}
}

void CTexture::DrawToFit(int w, int h)
{
	SDL_Renderer* renderer = CApplication::sRenderer;
	SDL_Texture* tex;
	SDL_Surface* surf;
	int newW, newH;
	double xratio, yratio, ratio;



	yratio = (double)h / (double)mTexH;
	xratio = (double)w / (double)mTexW;
	ratio = std::min(xratio, yratio);

	newW = (double)mTexW * ratio;
	newH = (double)mTexH * ratio;

	SDL_Rect dstRect = { (w - newW) / 2, (h - newH) / 2, newW, newH };

	SDL_RenderCopy(renderer, mTexture, NULL, &dstRect);

}

void CTexture::SetTexture(SDL_Texture *tex)
{
	mTexture = tex;
}

void CTexture::SetPosition(int x, int y)
{
	X = x;
	Y = y;
}

bool CTexture::CreateBlank(int width, int height, SDL_TextureAccess access)
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	free();

	//Create uninitialized texture
	mTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, access, width, height);
	if (mTexture == NULL)
	{
		printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		mTexW = width;
		mTexH = height;
	}

	return mTexture != NULL;
}

void CTexture::SetAsRenderTarget()
{
	SDL_Renderer* renderer = CApplication::sRenderer;

	//Make self render target
	SDL_SetRenderTarget(renderer, mTexture);
}

bool CTexture::LoadFromFile(std::string path)
{
	//printf("LoadFromFile %s\n", path.c_str());
	SDL_Renderer* renderer = CApplication::sRenderer;

	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		//SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0x00, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mTexW = loadedSurface->w;
			mTexH = loadedSurface->h;

			//printf("%d:%d\n", mTexW, mTexH);
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;

	//printf("End LoadFromFile\n");
	return mTexture != NULL;
}

void CTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mTexW = 0;
		mTexH = 0;
	}
}

bool CTexture::LoadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font, std::string textureText, SDL_Color textColor, int w, int h, HAlignment halign)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	string txt = " ";
	if (textureText != "")
	{
		txt = textureText;
	}

	//printf("LoadFromRenderedText(%s)\n", txt.c_str());

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, txt.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
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

		if (newSurf == NULL)
		{
			printf("LoadFromRenderedText() newSurf==NULL\n");
			return(false);
		}

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

		//SDL_FillRect(newSurf, NULL, SDL_MapRGB(newSurf->format, 255, 255, 0));

		int i = SDL_BlitSurface(textSurface, &src, newSurf, &dst);
		if (i != 0)
		{
			printf("%s\n", SDL_GetError());
			return(false);
		}

		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(renderer, newSurf);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mTexW = newSurf->w;
			mTexH = newSurf->h;
		}

		SDL_FreeSurface(newSurf);
		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}
bool CTexture::LoadFromRenderedText(SDL_Renderer *renderer, TTF_Font *font, std::string textureText, SDL_Color textColor)
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

		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mTexW = textSurface->w;
			mTexH = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}

bool CTexture::Collide(CRectangle r)
{
	CRectangle r2 = GetBounds();

	if (r2.intersects(r))
		return(true);

	return(false);
}

