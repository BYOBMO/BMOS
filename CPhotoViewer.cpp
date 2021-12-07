#include "CPhotoViewer.h"


CPhotoViewer::CPhotoViewer(int x, int y, int w, int h) : CForm(x, y, w, h)
{
	mPhoto = new CPhoto();
	mPhoto->CreateBlank(mPanel->Width(), mPanel->GetHeight(), SDL_TEXTUREACCESS_TARGET);
	mPhoto->SetPosition(0, 0);

	mPanel->AddWindow(mPhoto);
}



void CPhotoViewer::LoadPicture(std::string path)
{
	mPhoto->LoadPicture(path);
}

void CPhotoViewer::Draw()
{

	CForm::Draw();

}

