#include "CWiFi.h"
#include "CButton.h"
#include "CLabel.h"
#include "CTextBox.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include<boost/algorithm/string.hpp>

CWiFi::CWiFi(int x, int y, int w, int h) : CForm(x, y, w, h)
{
	using namespace std::placeholders;

	int spacing = 55;
	int base = 30;

	CLabel* label;
	label = new CLabel();
	mPanel->AddWindow(label);
	label->SetPosition(15, base);
	label->SetText("SSID:", CText::sFontSmall, 300, 40);

	mSSID = new CTextBox();
	mPanel->AddWindow(mSSID);
	mSSID->SetText("", CText::sFontSmall, 300, 40);
	mSSID->SetPosition(mPanel->Width() - mSSID->Width() - 10, label->Y);


	label = new CLabel();
	mPanel->AddWindow(label);
	label->SetPosition(15, base + spacing * 1);
	label->SetText("Passcode:", CText::sFontSmall, 300, 40);

	mPasscode = new CTextBox();
	mPanel->AddWindow(mPasscode);
	mPasscode->SetText("", CText::sFontSmall, 300, 40);
	mPasscode->SetPosition(mPanel->Width() - mPasscode->Width() - 10, label->Y);

    mIPLabel = new CLabel();
    mPanel->AddWindow(mIPLabel);
    mIPLabel->SetPosition(15, base + spacing * 2);
    string ip = "IP Address: " + GetIPAddress();

    mIPLabel->SetText(ip, CText::sFontSmall, 480, 40);


	mOk = new CButton();
	mPanel->AddWindow(mOk);

	mOk->SetText("OK", CText::sFontSmall, 100, 40);
	mOk->AddClickHandler(std::bind(&CWiFi::OnButton, this, _1));

	mOk->SetPosition((Width() - mOk->Width()) / 2, mPanel->GetHeight() - mOk->GetHeight() - 10);
}

void CWiFi::SetIPAddress()
{
    string ip = "IP Address: " + GetIPAddress();

    mIPLabel->SetText(ip, CText::sFontSmall, 480, 40);
}

string CWiFi::GetIPAddress()
{
    char message[1000];
    char ipCmd[1000];
    int n;
    FILE* cmd = NULL;
    string ipAddress = "";

    sprintf(ipCmd, "/home/pi/bmos/scripts/getip.sh");
    sprintf(message, "Not Found");
    ipAddress = message;

#ifndef WINDOWS
    cmd = popen(ipCmd, "r");
    try
    {
        fscanf(cmd, "%s", message);
    }
    catch (...)
    {
        sprintf(message, "Not Found");
    }
    pclose(cmd);
    printf("Cmd: %s\n", ipCmd);
    printf("ip: %s\n", message);

    ipAddress = message;
#endif

    return(ipAddress);
}

void CWiFi::AddOKHandler(std::function<void(CWiFi*)> callback)
{
    mCallbacks.push_back(callback);
}


void CWiFi::OnButton(CButton* button)
{
    string ssid, pwd;

    ssid = mSSID->mText.mText;
    pwd = mPasscode->mText.mText;

	printf("%s\n", button->mText.mText.c_str());
    if (ssid != "" && pwd != "")
    {
        UpdateWiFi(ssid, pwd);
    }

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

bool CWiFi::UpdateWiFi(std::string ssid, std::string psk)
{
    std::string config_file;
    std::vector<std::string> mLines;

#ifdef WINDOWS
    config_file= "C:/etc/wpa_supplicant/wpa_supplicant.conf";
#else
    config_file = "/etc/wpa_supplicant/wpa_supplicant.conf";
#endif

    ifstream file(config_file.c_str(), ios::in);
    if (!file.is_open())
    {
        printf("Can't read wpa_supplicant.conf.\n");
        return(false);
    }
    string line;
    int i = 1;
    while (getline(file, line))
    {
        boost::algorithm::replace_all(line, "\r", "");
        boost::algorithm::replace_all(line, "\n", "");

        int loc;
        if (line.find("ssid=") != std::string::npos)
        {
            line = "\tssid=";
            line = line + "\"" + ssid + "\"";
        }
        if (line.find("psk=") != std::string::npos)
        {
            line = "\tpsk=";
            line = line + "\"" + psk + "\"";
        }

        mLines.push_back(line);
        ++i;
    }
    file.close();

#ifdef WINDOWS
    config_file = "c:/home/pi/bmos/wpa_supplicant.conf";
#else
    config_file = "/home/pi/bmos/wpa_supplicant.conf";
#endif

    ofstream ofile(config_file.c_str(), ios::out);
    if (!ofile.is_open())
    {
        printf("Can't write wpa_supplicant.conf.\n");
        return(false);
    }

    for (i = 0; i < mLines.size(); i++)
    {
        ofile << mLines[i] << endl;
    }

    ofile.close();

#ifndef WINDOWS
    system("sudo cp /home/pi/bmos/wpa_supplicant.conf /etc/wpa_supplicant");
    system("sudo /home/pi/bmos/scripts/restartwifi.sh");
#endif

    return(true);

}