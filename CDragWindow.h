#pragma once
#include "CWindow.h"

#include <functional>

class CDragWindow : public CWindow
{
public:
    int mXOffset, mYOffset;
    Uint8 mOldR, mOldG, mOldB, mOldA;
    SDL_Texture* mOldTarget;
    CWindow* mDragItem;

    CDragWindow();
    virtual void Draw();
    void SetCanvasSize(int w, int h);
    void SetMouseOffset(int x, int y);
    void SetBackground(int r, int g, int b);

    void AddDropHandler(std::function<void(CDragWindow*)> callback);
    virtual CWindow* OnMouseMotion(int x, int y);
    virtual CWindow* OnMouseButton(SDL_MouseButtonEvent e);

    

    void BeginDraw();
    void EndDraw();
    void DrawTexture(CTexture* tex, int x, int y);
    void Drop();

    std::vector<std::function<void(CDragWindow*)>> mDropCallbacks;
};

