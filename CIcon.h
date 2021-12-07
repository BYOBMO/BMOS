#pragma once
#ifdef WINDOWS
#define _HAS_STD_BYTE 0
#endif
#include "CWindow.h"
#include <SDL2/SDL.h>

#include <functional>

using namespace std;

class CIcon : public CWindow
{
public:
    int m_NumFrames;
    int m_CurFrame;
    int m_Width;
   
    bool mDragX;
    bool mDragY;

    int mLastMouseX, mLastMouseY;

    std::function<void(int)> mCallback;
    void AddHandler(std::function<void(int)> callback);

    std::function<void(CIcon*)> mOnPositionCallback;
    void AddOnPositionHandler(std::function<void(CIcon*)> callback);

    std::function<void(CIcon* icon)> mClickCallback;
    void AddClickHandler(std::function<void(CIcon *icon)> callback);

    std::function<void(CIcon* icon)> mDoubleClickCallback;
    void AddDoubleClickHandler(std::function<void(CIcon* icon)> callback);

    CIcon(std::string path, int numFrames);

    void KeyDown(SDL_KeyboardEvent e);
    void SetFrame(int frame);
    virtual void Draw();
    virtual CWindow* OnMouseMotion(int x, int y);
    //virtual CWindow* OnMouseButton( SDL_MouseButtonEvent e);
    virtual void MouseLeave();
    virtual void Clicked(SDL_MouseButtonEvent e);
    void Pressed();

    virtual int Width();

};

