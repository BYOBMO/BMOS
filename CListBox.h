#pragma once
#include "CWindow.h"
#include "CListBoxItem.h"
#include "CVScrollBar.h"

#include <vector>
class CListBox :public CWindow
{
public:

    CListBox();
    virtual ~CListBox();

    void SetSize(int w, int h);
    CListBoxItem* AddItem(std::string col1, std::string col2);
    CListBoxItem* AddItem(std::string col1, std::string col2, SDL_Color color);
    CListBoxItem* AddItem(std::string col1, std::string col2, SDL_Color color, CFile::Type iconType);

    CVScrollBar* mScrollBar;

    virtual void Draw();
    void LayoutItems();
    
    void KeyDown(SDL_KeyboardEvent e);

    int mListHeight;
    int mScrollPos;
    int mListWidth;

    int ScrollSize();
    void SetScrollPos(int pos);

    void OnScroll(int position);
    void RemoveItem(CListBoxItem* item);
   
    void Clear();
    virtual void Focused(bool isFocus);
    void AddWindow(CWindow* window);

    void AddItemRemovedHandler(std::function<void(CListBoxItem*)> callback);
    std::vector<std::function<void(CListBoxItem*)>> mItemRemovedCallbacks;

    std::vector<CListBoxItem*>mItems;
};

