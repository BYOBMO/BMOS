#include "CMessageBox.h"
#include "CButton.h"
#include "CLabel.h"
#include <string.h>

#include <functional>

CMessageBox::CMessageBox(int x, int y, int w, int h) : CForm(x, y, w, h)
{
	using namespace std::placeholders;
	int panelW = mPanel->Width();


	btnCancel = new CButton();
	mPanel->AddWindow(btnCancel);
	btnCancel->SetText("Cancel", CText::sFontSmall, 120, 40);
	btnCancel->SetPosition(panelW - btnCancel->Width() - 5, mPanel->mTexH - btnCancel->mTexH - 10);
	btnCancel->AddClickHandler(std::bind(&CMessageBox::OnButton, this, _1));
	//btnCancel->AddClickHandler(std::bind(&CSettingsForm::OnButton, this, _1));

	btnOK = new CButton();
	mPanel->AddWindow(btnOK);
	btnOK->SetText("OK", CText::sFontSmall, 120, 40);
	btnOK->SetPosition(btnCancel->X - btnOK->Width() - 15, mPanel->mTexH - btnCancel->mTexH - 10);
	btnOK->AddClickHandler(std::bind(&CMessageBox::OnButton, this, _1));


	//button->AddClickHandler(std::bind(&CSettingsForm::OnButton, this, _1));
	mMultiLine = new CMultiLine();
	mPanel->AddWindow(mMultiLine);
}

void CMessageBox::AddLine(string text)
{
	AddLine(text, CSurface::HAlignment::Left);
}

void CMessageBox::AddLine(string text, CSurface::HAlignment align)
{
	mMultiLine->AddLine(text, align);
	mMultiLine->X = (mPanel->Width() - mMultiLine->mTextSurface->Width()) / 2;
	mMultiLine->Y = (btnOK->Y - mMultiLine->mTextSurface->Height()) / 2;
}

void CMessageBox::AddOnCloseHandler(std::function<void(CMessageBox*, MessageResult)> callback)
{
	OnCloseHandler = callback;
}

void CMessageBox::OnButton(CButton* button)
{

		if (button == btnCancel)
		{
			if (OnCloseHandler)
			{
				OnCloseHandler(this, MessageResult::Cancel);
				OnCloseHandler = nullptr;
			}
			Hide();
		}
		else if (button == btnOK)
		{
			if (OnCloseHandler)
			{
				OnCloseHandler(this, MessageResult::OK);
				OnCloseHandler = nullptr;
			}
			Hide();
		}

}

void CMessageBox::OnClose(CIcon* icon)
{
	CForm::OnClose(icon);
	if (OnCloseHandler)
	{
		OnCloseHandler(this, MessageResult::Cancel);
		OnCloseHandler = nullptr;
	}
}

void CMessageBox::ShowMessage(std::string text, std::string caption, MessageStyle style, MessageAlign align)
{
	char* line;
	const char c[2] = "\n";
	int panelW = mPanel->Width();

	OnCloseHandler = nullptr;

	switch (style)
	{
	case MessageStyle::OK:
		btnCancel->mVisible = false;
		btnCancel->mCanFocus = false;

		btnOK->mVisible = true;
		btnOK->mCanFocus = true;
		btnOK->SetPosition(panelW - btnCancel->Width() - 5, mPanel->mTexH - btnCancel->mTexH - 10);
		break;

	case MessageStyle::OK_Cancel:
		btnCancel->mVisible = true;
		btnCancel->mCanFocus = true;
		btnCancel->SetPosition(panelW - btnCancel->Width() - 5, mPanel->mTexH - btnCancel->mTexH - 10);

		btnOK->mVisible = true;
		btnOK->mCanFocus = true;
		btnOK->SetPosition(btnCancel->X - btnOK->Width() - 15, mPanel->mTexH - btnCancel->mTexH - 10);
		break;
	}
	line = strtok((char*)text.c_str(), c);
	mTitleBar.SetTitle(caption);
	mMultiLine->Clear();

	while (line != NULL)
	{
		if (align == MessageAlign::Center)
		{
			AddLine(line, CSurface::HAlignment::Center);
		}
		else
		{
			AddLine(line);
		}
		line = strtok(NULL, c);
	}
	Show();
}

void CMessageBox::ShowMessage(std::string text, std::string caption, MessageStyle style)
{
	ShowMessage(text, caption, style, MessageAlign::Left);
}