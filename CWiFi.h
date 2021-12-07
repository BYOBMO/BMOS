#pragma once
#include "CForm.h"
#include "CButton.h"
#include "CTextBox.h"
#include "CLabel.h"

class CWiFi :
    public CForm
{
public:
    CTextBox* mSSID;
    CTextBox* mPasscode;
    CButton* mOk;
    CLabel* mIPLabel;

    CWiFi(int x, int y, int w, int h);
    void OnButton(CButton* button);

    bool UpdateWiFi(std::string ssid, std::string passphrase);
    string GetIPAddress();
    void SetIPAddress();

    void AddOKHandler(std::function<void(CWiFi*)> callback);
    std::vector<std::function<void(CWiFi*)>> mCallbacks;
};

