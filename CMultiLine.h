#pragma once
#include "CWindow.h"
#include "CSurfWindow.h"

class CMultiLine : public CWindow
{
public:

    CMultiLine();

    vector<string> mLines;
    CSurfWindow* mTextSurface;

    void Clear();
    void AddLine(string text);
    void AddLine(string text, CSurface::HAlignment align);
    void RenderSurface();

    virtual void Draw();
};

