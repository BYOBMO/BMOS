#pragma once
#include "CForm.h"
#include "CMenuBarItem.h"
#include "CButton.h"
#include "CMenuBarItem.h"

class CAudioForm : public CForm
{
public:

	CMenuBarItem* mVolumeMenu;
	CMenuBarItem* mMicMenu;
	CMenuBarItem* mVoiceRecognition;

	void AddOKHandler(std::function<void(CAudioForm*)> callback);
	std::vector<std::function<void(CAudioForm*)>> mCallbacks;

	CAudioForm(int x, int y, int w, int h);

	void OnMenu(CMenuBarItem* item);
	void OnButton(CButton* button);

	void SetVolume(int i);
	int GetVolume();

	void SetMic(int i);
	int GetMic();

	void SetVoiceRecognition(bool b);
	bool GetVoiceRecognition();
};

