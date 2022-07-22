#include "CJoystick.h"
#include<boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

CJoystick::CJoystick()
{
    mJoyA = 1;
    mJoyB = 2;
    mJoyX = 0;
    mJoyY = 3;
    mJoyStart = 9;
    mJoySelect = 8;
    mJoyRT = 5;
    mJoyLT = 4;
    mJoyAxisY = 1;
    mJoyAxisX = 0;
}

CJoystick::~CJoystick()
{

}

void CJoystick::GetConfig(std::string config_file)
{
    //fprintf(stderr, "Opening config file...\n");
    ifstream file(config_file.c_str(), ios::in);
    if (!file.is_open())
    {
        printf("Can't find joystick file. Using defaults\n");
        return;
    }
    string line;
    int i = 1;
    while (getline(file, line)) 
    {
        boost::algorithm::replace_last(line, "\t", " ");
        boost::algorithm::replace_all(line, "\t", "");
		boost::algorithm::replace_all(line, "\r", "");
		boost::algorithm::replace_all(line, "\n", "");
        unsigned int loc = line.find("=");
        if (loc < 500 && loc != string::npos && line[0] != '#') {
            //This isn't a comment and is formatted properly
            try
            {
                string v = line.substr(0, loc);
                string c = line.substr(loc + 1);
                if (v == "A")
                {
                    mJoyA = atoi(c.c_str());
                }
                else if (v == "B")
                {
                    mJoyB = atoi(c.c_str());
                }
                else if (v == "X")
                {
                    mJoyX = atoi(c.c_str());
                }
                else if (v == "Y")
                {
                    mJoyY = atoi(c.c_str());
                }
                else if (v == "START")
                {
                    mJoyStart = atoi(c.c_str());
                }
                else if (v == "SELECT")
                {
                    mJoySelect = atoi(c.c_str());
                }
                else if (v == "RT")
                {
                    mJoyRT = atoi(c.c_str());
                }
                else if (v == "LT")
                {
                    mJoyLT = atoi(c.c_str());
                }
                else if (v == "XAXIS")
                {
                    mJoyAxisX = atoi(c.c_str());
                }
                else if (v == "YAXIS")
                {
                    mJoyAxisY = atoi(c.c_str());
                }
            }
            catch(...)
            {
            }
        }
        else if (line[0] != '#') {
            printf("Formatting error on line %d\n", i);
        }
        ++i;
    }
    file.close();
}


bool CJoystick::SaveFile(std::string filename)
{
    ofstream ofile(filename.c_str(), ios::out);
    if (!ofile.is_open())
    {
        printf("Can't write settings.txt.\n");
        return(false);
    }

    ofile << "A=" << mJoyA << endl;
    ofile << "B=" << mJoyB << endl;
    ofile << "X=" << mJoyX << endl;
    ofile << "Y=" << mJoyY << endl;
    ofile << "START=" << mJoyStart << endl;
    ofile << "SELECT=" << mJoySelect << endl;
    ofile << "RT=" << mJoyRT << endl;
    ofile << "LT=" << mJoyLT << endl;
    ofile << "YAXIS=" << mJoyAxisY << endl;
    ofile << "XAXIS=" << mJoyAxisX << endl;

    ofile.close();


    return(true);
}