#pragma once

#include <string>

class CSettings
{
public:
	CSettings();
	bool LoadFile(std::string filename);
	bool SaveFile(std::string filename);

	bool mVoiceRecognition;
	bool mSlideShow;
	bool mRandomwVids;

	int mSlideInterval;
	int mRandomVidInterval;
	int mFaceInterval;

	int GetAudioSetting(int numid);
	int SetAudioSetting(int numid, int val);
};

