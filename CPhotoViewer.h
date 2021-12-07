#pragma once
#include "CForm.h"
#include "CSurfWindow.h"
#include "CPhoto.h"

#include <string>

class CPhotoViewer :
    public CForm
{
public:

    CPhotoViewer(int x, int y, int w, int h);
    CPhoto* mPhoto;

    void LoadPicture(std::string path);
    virtual void Draw();

};

