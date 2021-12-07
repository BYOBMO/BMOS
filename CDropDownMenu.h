#pragma once
#include "CMenuBarItem.h"
#include "CWindow.h"


#include <functional>

class CDropDownMenu :
    public CWindow
{

public:
    CDropDownMenu();
    virtual ~CDropDownMenu();

    CMenuBarItem* mParentItem;

    virtual void Draw();
    //virtual CWindow* OnMouseButton(SDL_MouseButtonEvent e);
    virtual void KeyDown(SDL_KeyboardEvent e);

    void SelectFirstItem();
    void ClearAll();
    void AddMenuItem(std::string text, std::function<void(CMenuBarItem*)> callback, TTF_Font* font);

    void OnItemClicked(CMenuBarItem* item);
};

