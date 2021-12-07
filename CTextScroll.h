#pragma once
#include "CWindow.h"

#include <vector>
#include <string>

class CTextScroll :
    public CWindow
{
public:
    vector<std::string> mText;
    int mFirstLine, mLastLine;
    int mOffset;
    int mPosition;
    bool mUpdate;

    CTextScroll();

    void AddLine(std::string line);
    virtual void Draw();
    void SetScroll(int position);
    void SetSize(int w, int h);

    virtual void KeyDown(SDL_KeyboardEvent e);

    void AddTextPositionHandler(std::function<void(CTextScroll*)> callback);

    std::vector<std::function<void(CTextScroll*)>> mCallbacks;
};

