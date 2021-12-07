#include "CAudioForm.h"
#include "CMenuBarItem.h"
#include "CButton.h"
#include "CLabel.h"
#include "CTextBox.h"
#include "CListBoxItem.h"

CAudioForm::CAudioForm(int x, int y, int w, int h) : CForm(x, y, w, h)
{
	using namespace std::placeholders;

	CButton* button;

	int spacing = 45;
	int base = 10;

	CLabel* label;
	label = new CLabel();
	mPanel->AddWindow(label);
	label->SetPosition(15, base);
	label->SetText("Volume", CText::sFontSmall, 300, 40);

	mVolumeMenu = new CMenuBarItem();
	mVolumeMenu->mFont = CText::sFontSmall;
	mPanel->AddWindow(mVolumeMenu);
	mVolumeMenu->ComboBox();
	mVolumeMenu->AddMenuItem("Mute", std::bind(&CAudioForm::OnMenu, this, _1));
	mVolumeMenu->AddMenuItem("10", std::bind(&CAudioForm::OnMenu, this, _1));
	mVolumeMenu->AddMenuItem("30", std::bind(&CAudioForm::OnMenu, this, _1));
	mVolumeMenu->AddMenuItem("50", std::bind(&CAudioForm::OnMenu, this, _1));
	mVolumeMenu->AddMenuItem("70", std::bind(&CAudioForm::OnMenu, this, _1));
	mVolumeMenu->AddMenuItem("90", std::bind(&CAudioForm::OnMenu, this, _1));

	mVolumeMenu->SetTitle(" ");
	mVolumeMenu->SetPosition(Width() - mVolumeMenu->Width() - 10, label->Y);

	label = new CLabel();
	mPanel->AddWindow(label);
	label->SetPosition(15, base + spacing * 1);
	label->SetText("Microphone", CText::sFontSmall, 300, 40);

	mMicMenu = new CMenuBarItem();
	mMicMenu->mFont = CText::sFontSmall;
	mPanel->AddWindow(mMicMenu);
	mMicMenu->ComboBox();
	mMicMenu->AddMenuItem("Mute", std::bind(&CAudioForm::OnMenu, this, _1));
	mMicMenu->AddMenuItem("25", std::bind(&CAudioForm::OnMenu, this, _1));
	mMicMenu->AddMenuItem("35", std::bind(&CAudioForm::OnMenu, this, _1));
	mMicMenu->AddMenuItem("45", std::bind(&CAudioForm::OnMenu, this, _1));
	mMicMenu->AddMenuItem("55", std::bind(&CAudioForm::OnMenu, this, _1));
	mMicMenu->SetTitle(" ");
	mMicMenu->SetPosition(Width() - mMicMenu->Width() - 10, label->Y);

	label = new CLabel();
	mPanel->AddWindow(label);
	label->SetPosition(15, base + spacing * 5);
	label->SetText("Voice Recognition", CText::sFontSmall, 300, 40);


	mVoiceRecognition = new CMenuBarItem();
	mVoiceRecognition->mFont = CText::sFontSmall;
	mPanel->AddWindow(mVoiceRecognition);
	mVoiceRecognition->ComboBox();
	mVoiceRecognition->AddMenuItem("On", std::bind(&CAudioForm::OnMenu, this, _1));
	mVoiceRecognition->AddMenuItem("Off", std::bind(&CAudioForm::OnMenu, this, _1));
	mVoiceRecognition->SetTitle("Off");
	mVoiceRecognition->SetPosition(Width() - mVoiceRecognition->Width() - 10, label->Y);

	button = new CButton();
	mPanel->AddWindow(button);

	button->SetText("OK", CText::sFontSmall, 100, 40);
	button->AddClickHandler(std::bind(&CAudioForm::OnButton, this, _1));

	button->SetPosition((Width() - button->Width()) / 2, mPanel->GetHeight() - button->GetHeight() - 10);
}

void CAudioForm::AddOKHandler(std::function<void(CAudioForm*)> callback)
{
	mCallbacks.push_back(callback);
}

void CAudioForm::OnMenu(CMenuBarItem* item)
{
	//printf("%s\n", item->mText.c_str());
}

void CAudioForm::OnButton(CButton* button)
{
	for (int i = 0; i < mCallbacks.size(); i++)
	{
		try
		{
			mCallbacks[i](this);
		}
		catch (...)
		{

		}
	}

	Hide();
}

void CAudioForm::SetVolume(int i)
{
	if (i < 0 && i>100)
	{
		return;
	}

	char t[100];
	sprintf(t, "%d", i);
	mVolumeMenu->SetTitle(t);
}

int CAudioForm::GetVolume()
{
	int i = -1;

	try
	{
		if (mVolumeMenu->mText == "Mute")
		{
			i = 0;
		}
		else
		{
			i = atoi(mVolumeMenu->mText.c_str());
		}
	}
	catch (...)
	{

	}

	return(i);
}

void CAudioForm::SetMic(int i)
{
	if (i < 0 && i>100)
	{
		return;
	}

	char t[100];
	sprintf(t, "%d", i);
	mMicMenu->SetTitle(t);
}

int CAudioForm::GetMic()
{
	int i = -1;

	try
	{
		if (mMicMenu->mText == "Mute")
		{
			i = 0;
		}
		else
		{
			i = atoi(mMicMenu->mText.c_str());
		}
	}
	catch (...)
	{

	}

	return(i);
}



void CAudioForm::SetVoiceRecognition(bool b)
{
	if (b)
	{
		mVoiceRecognition->SetTitle("On");
	}
	else
	{
		mVoiceRecognition->SetTitle("Off");
	}
}


bool CAudioForm::GetVoiceRecognition()
{
	if (mVoiceRecognition->mText == "On")
	{
		return(true);
	}

	return(false);
}
