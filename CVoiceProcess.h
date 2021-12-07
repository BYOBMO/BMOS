#pragma once
#include "CVoiceCommand.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
//#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/xml_parser.hpp>
#include <vector>
#include <string>

//#include <termios.h>
//#include <unistd.h>
#include <sstream>

#define DATA_SIZE 200
#define DURATION_DEFAULT "1"
#define COM_DURATION_DEFAULT "3"

using namespace std;

class CVoiceProcess {
private:
    vector<string> voice, commands;
protected:
    string version;
public:

    bool passthrough;


    int debug;
    CVoiceProcess();
    ~CVoiceProcess();

    CVoiceCommand ProcessMessage(std::string message);
    void GetConfig(std::string config_file);


};




