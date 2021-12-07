#pragma once
#ifdef WINDOWS
#define _HAS_STD_BYTE 0
#endif
#include "CButton.h"
#include "CLabel.h"
#include "CForm.h"
#include "CSurfWindow.h"
#include "CMultiLine.h"

using namespace std;

class CMessageBox :
    public CForm
{
public:
    enum class MessageResult
    {
        OK,
        Cancel,
        Yes,
        No
    };

    enum class MessageStyle
    {
        OK,
        OK_Cancel,
        Yes_No,
        None
    };

    enum class MessageAlign
    {
        Left,
        Center,
        Right
    };
    CMessageBox(int x, int y, int w, int h);

    CButton* btnOK;
    CButton* btnCancel;
    CMultiLine* mMultiLine;
    string mTag;

    void ShowMessage(std::string text, std::string caption, MessageStyle style);
    void ShowMessage(std::string text, std::string caption, MessageStyle style, MessageAlign align);

    void AddLine(string text);
    void AddLine(string text, CSurface::HAlignment align);

    void AddOnCloseHandler(std::function<void(CMessageBox*, MessageResult)> callback);
    std::function<void(CMessageBox*, MessageResult)> OnCloseHandler;

    void OnButton(CButton* button);
    virtual void OnClose(CIcon*);
};

