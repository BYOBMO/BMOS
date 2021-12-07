#include "CKeyMap.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include<boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

CKeyMap::CKeyMap()
{
}

bool CKeyMap::LoadFile(std::string config_file)
{
    mKeys.clear();
    mRandomVids.clear();

    //fprintf(stderr, "Opening config file %s\n", config_file.c_str());
    ifstream file(config_file.c_str(), ios::in);
    if (!file.is_open())
    {
        printf("Can't find config file.\n");
        return(false);
    }
    string line;
    int i = 1;
    while (getline(file, line))
    {
        boost::algorithm::replace_last(line, "\t", " ");
        boost::algorithm::replace_all(line, "\t", "");
        boost::algorithm::replace_all(line, "\r", "");
        boost::algorithm::replace_all(line, "\n", "");
        
        const char c[2] = ":";
        char* tok;
        tok = strtok((char*)line.c_str(), c);
        if (tok != NULL)
        {
            CKeyCommand cmd;
            cmd.mKey = tok;
            if (cmd.mKey == "random")
            {
                tok = strtok(NULL, c);
                if (tok != NULL)
                {
                    mRandomVids.push_back(std::string(tok));
                }
            }
            else
            {
                tok = strtok(NULL, c);
                if (tok != NULL)
                {
                    cmd.mCommand = tok;
                    tok = strtok(NULL, c);
                    if (tok != NULL)
                    {
                        cmd.mArgument1 = tok;
                        tok = strtok(NULL, c);
                        if (tok != NULL)
                        {
                            cmd.mArgument2 = tok;
                        }
                        mKeys.push_back(cmd);
                    }
                }
            }
        }
        ++i;
    }
    file.close();

	return(true);
}

CKeyCommand CKeyMap::GetKey(std::string key)
{
	CKeyCommand cmd;

	int i;
	for (i = 0; i < mKeys.size(); i++)
	{
		if (key == mKeys[i].mKey)
		{
			return(mKeys[i]);
		}
	}

	return(cmd);
}