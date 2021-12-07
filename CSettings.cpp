#include "CSettings.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include<boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

CSettings::CSettings()
{
	mFaceInterval = 20;
	mSlideInterval = 20;
	mRandomVidInterval = 5;

	mSlideShow = false;
	mRandomwVids = false;
	mVoiceRecognition = false;
}

bool CSettings::LoadFile(std::string filename)
{

    ifstream file(filename.c_str(), ios::in);
    if (!file.is_open())
    {
        printf("Can't find config file.\n");
        return(false);
    }
    string line;
    int i = 1;
    while (getline(file, line))
    {
        boost::algorithm::replace_all(line, "\r", "");
        boost::algorithm::replace_all(line, "\n", "");

        const char c[2] = ":";
        char* tok;
        tok = strtok((char*)line.c_str(), c);
        if (tok != NULL)
        {
            std::string key;
            std::string value;

            key = tok;
            tok = strtok(NULL, c);
            if (tok != NULL)
            {
                value = tok;

                if (key == "FaceInterval")
                {
                    mFaceInterval = atoi(value.c_str());
                }
                else if (key == "Slideshow")
                {
                    mSlideShow = false;
                    if (value == "1")
                        mSlideShow = true;

                }
                else if (key == "SlideInterval")
                {
                    mSlideInterval = atoi(value.c_str());
                }
                else if (key == "RandomVideo")
                {
                    mRandomwVids = false;
                    if (value == "1")
                        mRandomwVids = true;
                }
                else if (key == "RandomVideoInterval")
                {
                    mRandomVidInterval = atoi(value.c_str());
                }
                else if (key == "VoiceRecognition")
                {
                    mVoiceRecognition = false;
                    if (value == "1")
                        mVoiceRecognition = true;
                }
                
            }
        }
        ++i;
    }
    file.close();

    return(true);
}

bool CSettings::SaveFile(std::string filename)
{
    ofstream ofile(filename.c_str(), ios::out);
    if (!ofile.is_open())
    {
        printf("Can't write settings.txt.\n");
        return(false);
    }

    ofile << "FaceInterval:" << mFaceInterval << endl;
    ofile << "Slideshow:" << mSlideShow << endl;
    ofile << "SlideInterval:" << mSlideInterval << endl;
    ofile << "RandomVideo:" << mRandomwVids << endl;
    ofile << "RandomVideoInterval:" << mRandomVidInterval << endl;
    ofile << "VoiceRecognition:" << mVoiceRecognition << endl;

    ofile.close();


	return(true);
}

int CSettings::GetAudioSetting(int numid)
{
    char message[1000];
    char volCmd[1000];
    int n;
    FILE* cmd = NULL;
    if (numid == 1)
    {
        sprintf(volCmd, "/home/pi/bmos/scripts/getmic.sh");
    }
    else if (numid == 2)
    {
        sprintf(volCmd, "/home/pi/bmos/scripts/getvolume.sh");
    }
    else
    {
        return(-1);
    }

#ifndef WINDOWS
    cmd = popen(volCmd, "r");
    try
    {
        fscanf(cmd, "%d", &n);
    }
    catch (...)
    {
        n = -1;
    }
    pclose(cmd);
    printf("Cmd: %s\n", volCmd);
    printf("n: %d\n", n);
    try
    {
        numid = n;
    }
    catch(...)
    {
        numid = -1;
    }
#endif

    return(numid);
}

int CSettings::SetAudioSetting(int numid, int val)
{
    char volCmd[1000];
    int n;
    FILE* cmd = NULL;

    sprintf(volCmd, "/home/pi/bmos/scripts/vol.sh %d %d", val, numid);
#ifndef WINDOWS
    system(volCmd);
#endif


    return(0);
}