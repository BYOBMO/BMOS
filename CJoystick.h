#pragma once
#include "CJoystick.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <string>

#include <sstream>


using namespace std;

class CJoystick {
private:


protected:

public:

    int mJoyA;
    int mJoyB;
    int mJoyX;
    int mJoyY;
    int mJoyStart;
    int mJoySelect;
    int mJoyRT;
    int mJoyLT;
    int mJoyAxisY;
    int mJoyAxisX;

    CJoystick();
    ~CJoystick();

    void GetConfig(std::string config_file);
    bool SaveFile(std::string filename);

};




