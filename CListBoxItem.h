#pragma once
#include "CLabel.h"
#include "CText.h"
#include "CSurface.h"
#include "CFile.h"

class CListBoxItem : public CWindow
{
public:

    unsigned mBgColor = 0xFFFFFF;
    unsigned mBgColorSelect = 0xCCFFCC;
    unsigned mDrawColor = 0xFFFFFF;
    int mMinWidth;
    int mTag;
    CFile::Type mIconType;

    int mLastMouseX, mLastMouseY;

    std::string mColumn1;
    std::string mColumn2;
    CSurface* mItemSurface;

    SDL_Color mColor;

    void SetColor(SDL_Color color);

    CListBoxItem();
    virtual ~CListBoxItem();

    virtual void Draw();
    void CreateTextSurface(int w, int h);
    void CreateTextureFromSurface(); 

    void SetText(std::string col1);
    void SetText(std::string col1, std::string col2);
    //void SetText(std::string text, TTF_Font* font);
    //void SetText(std::string text, TTF_Font* font, int w, int h);
    virtual CWindow* OnMouseMotion(int x, int y);
    //virtual CWindow* OnMouseButton(SDL_MouseButtonEvent e);
    virtual void KeyDown(SDL_KeyboardEvent e);
    void Pressed();

    void AddClickHandler(std::function<void(CListBoxItem*)> callback);
    void AddDoubleClickHandler(std::function<void(CListBoxItem*)> callback);
    virtual void Clicked(SDL_MouseButtonEvent e);
    virtual void Dropped(CWindow* theItem,DropAction action, CWindow* dropTarget);

    std::vector<std::function<void(CListBoxItem*)>> mClickCallbacks;
    std::vector<std::function<void(CListBoxItem*)>> mDoubleClickCallbacks;
};

