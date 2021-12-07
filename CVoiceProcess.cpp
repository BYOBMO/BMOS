#include "CVoiceProcess.h"
#include<boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

CVoiceProcess::CVoiceProcess()
{
    debug = 0;
    passthrough = false;
}

CVoiceProcess::~CVoiceProcess()
{

}

void CVoiceProcess::GetConfig(std::string config_file)
{
    //fprintf(stderr, "Opening config file...\n");
    ifstream file(config_file.c_str(), ios::in);
    if (!file.is_open())
    {
        printf("Can't find config file.\n");
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
        unsigned int loc = line.find("==");
        if (loc < 500 && loc != string::npos && line[0] != '#') {
            //This isn't a comment and is formatted properly
            string v = line.substr(0, loc);
            string c = line.substr(loc + 2);
            to_upper(v);
            voice.push_back(v);
            commands.push_back(c);
        }
        else if (line[0] != '#') {
            printf("Formatting error on ine %d\n", i);
        }
        ++i;
    }
    file.close();
}


CVoiceCommand CVoiceProcess::ProcessMessage(std::string message) {
    CVoiceCommand vcmd;

    unsigned int i = 0, loc = 0;
    string tmp = message;
    string sTmp = message;
    to_upper(sTmp);
    string rawCommand = "";

    vcmd.mMessage = message;

    while (i < voice.size()) {
        loc = sTmp.find(voice[i]);
        if (loc == 0) {
            tmp = commands[i];
            loc = tmp.find("...");
            if (loc != string::npos) {
                //Found ... Initiating special options
                string newcommand = tmp.substr(0, loc - 1);
                string options = message;
                newcommand += options.substr(voice[i].length());
                rawCommand = newcommand;
                if (passthrough)
                    printf("%s", newcommand.c_str());
                else {
                    printf("command1: %s\n", newcommand.c_str());
                    //system(newcommand.c_str());
                    //sleep(1);
                }
            }
            else {
                rawCommand = tmp.c_str();
                if (passthrough)
                    printf("%s", tmp.c_str());
                else {
                    printf("command2: %s\n", tmp.c_str());
                    //system(tmp.c_str());
                    //sleep(1);
                }
            }
            break;
        }
        else if (voice[i][0] == '~') {
            // see whether the voice keyword is *anywhere* in the message
            string v = voice[i].substr(1, string::npos);
            loc = sTmp.find(v);
            //printf("v: %s\tloc: %d\tsTmp: %s\n",v.c_str(),loc,sTmp.c_str());
            if (loc != string::npos && loc != -1) {
                // if it does, return
                rawCommand = commands[i].c_str();
                if (passthrough)
                    printf("%s", commands[i].c_str());
                else {
                    printf("command3: %s\n", commands[i].c_str());
                    //system(commands[i].c_str());
                    //sleep(1);
                }
                break;
            }
        }

        ++i;
    }


	printf("rawCommand = %s\n", (char*)rawCommand.c_str());

    const char c[2] = ":";
    char* tok;
    tok = strtok((char*)rawCommand.c_str(), c);
    if (tok != NULL)
    {
        vcmd.mCommand = tok;
        tok = strtok(NULL, c);
        if (tok != NULL)
        {
            vcmd.mArgument = tok;
        }
    }
    
    return(vcmd);
}
