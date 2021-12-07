#pragma once
#include "CWindow.h"
#include "CSurfWindow.h"

class CPhoto :
    public CWindow
{
public:
    CPhoto();

    void LoadPicture(std::string path);
    virtual void KeyDown(SDL_KeyboardEvent e);
    virtual void Draw();
};

