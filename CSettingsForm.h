#pragma once
#include "CForm.h"
#include "CMenuBarItem.h"
#include "CButton.h"
#include "CMenuBarItem.h"

class CSettingsForm : public CForm
{
public:

	CMenuBarItem* mFaceIntervalMenu;
	CMenuBarItem* mSlideIntervalMenu;
	CMenuBarItem* mRandomIntervalMenu;
	CMenuBarItem* mSlideShow;
	CMenuBarItem* mRandomVideo;


	void AddOKHandler(std::function<void(CSettingsForm*)> callback);
	std::vector<std::function<void(CSettingsForm*)>> mCallbacks;

	CSettingsForm(int x, int y, int w, int h);

	void OnMenu(CMenuBarItem* item);
	void OnButton(CButton* button);

	void SetFaceInterval(int i);
	int GetFaceInterval();

	void SetSlideInterval(int i);
	int GetSlideInterval();

	void SetRandomInterval(int i);
	int GetRandomInterval();

	void SetSlideshow(bool b);
	bool GetSlideshow();

	void SetRandomVideo(bool b);
	bool GetRandomVideo();

};

