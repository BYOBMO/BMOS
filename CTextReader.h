#pragma once
#include "CWindow.h"
#include "CForm.h"
//#include "CMultiLine.h"
#include "CVScrollBar.h"
#include "CTextScroll.h"

#include <vector>
#include <string>

class CTextReader :
    public CForm
{
public:
    CVScrollBar* mScrollBar;
    CTextScroll* mTextScroll;

    CTextReader(int x, int y, int w, int h);
    ~CTextReader();

    //CMultiLine* mMultiLine;

    void SetText(std::string text, std::string caption);

    void AddLine(string text);
    void SetTtile(std::string title);

    void OnScroll(int position);
    void LoadFile(std::string filename);
    void OnTextPositionChange(CTextScroll *scroll);

};

