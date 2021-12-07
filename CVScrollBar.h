#pragma once
#include "CWindow.h"
#include "CIcon.h"

class CVScrollBar :
    public CWindow
{
public:
    unsigned mBgColor = 0xE2FFE2;
    unsigned mBgColorSelect = 0xCCFFCC;
    unsigned mDrawColor = 0xE2FFE2;

    int mMin, mMax;
    int mPosition;

    CIcon* mSlider;

    CVScrollBar();
    void SetHeight(int h);

    virtual void Draw();

    void SetRange(int min, int max);
    void SetPos(int pos);
    void SetPos(int pos, bool update);
    void OnScrollPosition(CIcon* slider);

    std::function<void(int)> mOnScrollCallback;
    void AddOnScrollHandler(std::function<void(int)> callback);
};

