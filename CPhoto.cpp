#include "CPhoto.h"
#include "CApplication.h"

CPhoto::CPhoto() :CWindow()
{

}

void CPhoto::LoadPicture(std::string path)
{
	//printf("LoadPicture %s\n", path.c_str());
	SDL_Renderer* renderer = CApplication::sRenderer;
	CSurfWindow* surf = new CSurfWindow();
	SDL_Texture* oldTexture;

	oldTexture = SDL_GetRenderTarget(renderer);

	surf->LoadImage(path);

	SDL_SetRenderTarget(renderer, mTexture);
	surf->DrawTextureScaled(renderer, Width(), GetHeight());
	SDL_SetRenderTarget(renderer, oldTexture);
	delete(surf);
}

void CPhoto::Draw()
{
	SDL_Rect src, dst;

	src.x = 0;
	src.y = 0;
	src.w = Width();
	src.h = GetHeight();

	dst.x = 0;
	dst.y = 0;
	dst.w = mParent->Width();
	dst.h = mParent->GetHeight();

	CTexture::Draw(&src, &dst, 0.0);
}

void CPhoto::KeyDown(SDL_KeyboardEvent e)
{

	if (mParent != NULL)
	{
		mParent->KeyDown(e);
	}

}
