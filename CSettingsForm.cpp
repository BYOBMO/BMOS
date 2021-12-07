#include "CSettingsForm.h"
#include "CMenuBarItem.h"
#include "CButton.h"
#include "CLabel.h"
#include "CTextBox.h"
#include "CListBoxItem.h"

CSettingsForm::CSettingsForm(int x, int y, int w, int h) : CForm(x, y, w, h)
{
	using namespace std::placeholders;

	CButton* button;

	int spacing = 45;
	int base = 10;

	CLabel* label;
	label = new CLabel();
	mPanel->AddWindow(label);
	label->SetPosition(15, base);
	label->SetText("Face Interval(sec)", CText::sFontSmall, 300, 40);

	mFaceIntervalMenu = new CMenuBarItem();
	mFaceIntervalMenu->mFont = CText::sFontSmall;
	mPanel->AddWindow(mFaceIntervalMenu);
	mFaceIntervalMenu->ComboBox();
	mFaceIntervalMenu->AddMenuItem("5", std::bind(&CSettingsForm::OnMenu, this, _1));
	mFaceIntervalMenu->AddMenuItem("10", std::bind(&CSettingsForm::OnMenu, this, _1));
	mFaceIntervalMenu->AddMenuItem("15", std::bind(&CSettingsForm::OnMenu, this, _1));
	mFaceIntervalMenu->AddMenuItem("20", std::bind(&CSettingsForm::OnMenu, this, _1));
	mFaceIntervalMenu->AddMenuItem("30", std::bind(&CSettingsForm::OnMenu, this, _1));
	mFaceIntervalMenu->SetTitle("10");
	mFaceIntervalMenu->SetPosition(Width() - mFaceIntervalMenu->Width() - 10, label->Y);

	label = new CLabel();
	mPanel->AddWindow(label);
	label->SetPosition(15, base + spacing*1);
	label->SetText("Slideshow", CText::sFontSmall, 300, 40);

	mSlideShow = new CMenuBarItem();
	mSlideShow->mFont = CText::sFontSmall;
	mPanel->AddWindow(mSlideShow);
	mSlideShow->ComboBox();
	mSlideShow->AddMenuItem("On", std::bind(&CSettingsForm::OnMenu, this, _1));
	mSlideShow->AddMenuItem("Off", std::bind(&CSettingsForm::OnMenu, this, _1));
	mSlideShow->SetTitle("Off");
	mSlideShow->SetPosition(Width() - mSlideShow->Width() - 10, label->Y);

	label = new CLabel();
	mPanel->AddWindow(label);
	label->SetPosition(15, base + spacing * 2);
	label->SetText("Slide Interval(sec)", CText::sFontSmall, 300, 40);

	mSlideIntervalMenu = new CMenuBarItem();
	mSlideIntervalMenu->mFont = CText::sFontSmall;
	mPanel->AddWindow(mSlideIntervalMenu);
	mSlideIntervalMenu->ComboBox();
	mSlideIntervalMenu->AddMenuItem("5", std::bind(&CSettingsForm::OnMenu, this, _1));
	mSlideIntervalMenu->AddMenuItem("10", std::bind(&CSettingsForm::OnMenu, this, _1));
	mSlideIntervalMenu->AddMenuItem("15", std::bind(&CSettingsForm::OnMenu, this, _1));
	mSlideIntervalMenu->AddMenuItem("20", std::bind(&CSettingsForm::OnMenu, this, _1));
	mSlideIntervalMenu->AddMenuItem("30", std::bind(&CSettingsForm::OnMenu, this, _1));
	mSlideIntervalMenu->SetTitle("10");
	mSlideIntervalMenu->SetPosition(Width() - mSlideIntervalMenu->Width() - 10, label->Y);

	label = new CLabel();
	mPanel->AddWindow(label);
	label->SetPosition(15, base + spacing * 3);
	label->SetText("Random Video", CText::sFontSmall, 300, 40);

	mRandomVideo = new CMenuBarItem();
	mRandomVideo->mFont = CText::sFontSmall;
	mPanel->AddWindow(mRandomVideo);
	mRandomVideo->ComboBox();
	mRandomVideo->AddMenuItem("On", std::bind(&CSettingsForm::OnMenu, this, _1));
	mRandomVideo->AddMenuItem("Off", std::bind(&CSettingsForm::OnMenu, this, _1));
	mRandomVideo->SetTitle("Off");
	mRandomVideo->SetPosition(Width() - mRandomVideo->Width() - 10, label->Y);

	label = new CLabel();
	mPanel->AddWindow(label);
	label->SetPosition(15, base + spacing * 4);
	label->SetText("Video Interval(min)", CText::sFontSmall, 300, 40);

	mRandomIntervalMenu = new CMenuBarItem();
	mRandomIntervalMenu->mFont = CText::sFontSmall;
	mPanel->AddWindow(mRandomIntervalMenu);
	mRandomIntervalMenu->ComboBox();
	mRandomIntervalMenu->AddMenuItem("1", std::bind(&CSettingsForm::OnMenu, this, _1));
	mRandomIntervalMenu->AddMenuItem("2", std::bind(&CSettingsForm::OnMenu, this, _1));
	mRandomIntervalMenu->AddMenuItem("3", std::bind(&CSettingsForm::OnMenu, this, _1));
	mRandomIntervalMenu->AddMenuItem("4", std::bind(&CSettingsForm::OnMenu, this, _1));
	mRandomIntervalMenu->AddMenuItem("5", std::bind(&CSettingsForm::OnMenu, this, _1));
	mRandomIntervalMenu->AddMenuItem("10", std::bind(&CSettingsForm::OnMenu, this, _1));
	mRandomIntervalMenu->SetTitle("5");
	mRandomIntervalMenu->SetPosition(Width() - mRandomIntervalMenu->Width() - 10, label->Y);


	button = new CButton();
	mPanel->AddWindow(button);
	
	button->SetText("OK", CText::sFontSmall, 100, 40);
	button->AddClickHandler(std::bind(&CSettingsForm::OnButton, this, _1));

	button->SetPosition((Width() - button->Width())/2, mPanel->GetHeight() - button->GetHeight()-10);
}

void CSettingsForm::AddOKHandler(std::function<void(CSettingsForm*)> callback)
{


	mCallbacks.push_back(callback);
}

void CSettingsForm::OnMenu(CMenuBarItem* item)
{
	//printf("%s\n", item->mText.c_str());
}

void CSettingsForm::OnButton(CButton* button)
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

void CSettingsForm::SetFaceInterval(int i)
{
	if (i < 1 && i>30)
	{
		return;
	}

	char t[100];
	sprintf(t, "%d", i);
	mFaceIntervalMenu->SetTitle(t);
}

int CSettingsForm::GetFaceInterval()
{
	int i=20;

	try
	{
		i = atoi(mFaceIntervalMenu->mText.c_str());
	}
	catch (...)
	{

	}

	return(i);
}

void CSettingsForm::SetSlideInterval(int i)
{
	if (i < 1 && i>30)
	{
		return;
	}

	char t[100];
	sprintf(t, "%d", i);
	mSlideIntervalMenu->SetTitle(t);
}

int CSettingsForm::GetSlideInterval()
{
	int i = 20;

	try
	{
		i = atoi(mSlideIntervalMenu->mText.c_str());
	}
	catch (...)
	{

	}

	return(i);
}

void CSettingsForm::SetRandomInterval(int i)
{
	if (i < 1 && i>30)
	{
		return;
	}

	char t[100];
	sprintf(t, "%d", i);
	mRandomIntervalMenu->SetTitle(t);
}

int CSettingsForm::GetRandomInterval()
{
	int i = 5;

	try
	{
		i = atoi(mRandomIntervalMenu->mText.c_str());
	}
	catch (...)
	{

	}

	return(i);
}

void CSettingsForm::SetSlideshow(bool b)
{
	if (b)
	{
		mSlideShow->SetTitle("On");
	}
	else
	{
		mSlideShow->SetTitle("Off");
	}
}

bool CSettingsForm::GetSlideshow()
{
	if (mSlideShow->mText == "On")
	{
		return(true);
	}

	return(false);
}



bool CSettingsForm::GetRandomVideo()
{
	if (mRandomVideo->mText == "On")
	{
		return(true);
	}

	return(false);
}

void CSettingsForm::SetRandomVideo(bool b)
{
	if (b)
	{
		mRandomVideo->SetTitle("On");
	}
	else
	{
		mRandomVideo->SetTitle("Off");
	}
}

