#include "CFiles.h"

#include <stdio.h>
#include <string.h>

#ifdef WINDOWS
#include <Windows.h>
#else
#include <dirent.h>
#endif

#include <fstream>
#include <iostream>
//#include <filesystem>
#include <boost/filesystem.hpp>
using namespace std;


bool CFiles::Copy(std::string from, std::string to)
{
    try
    {
        std::cout << from << " " << boost::filesystem::file_size(from) << '\n';
        std::cout << to + '\n';
        boost::filesystem::copy_file(from, to, boost::filesystem::copy_option::overwrite_if_exists);
    }
    catch (...)
    {
        return(false);
    }
    return(true);
}

bool CFiles::Delete(std::string path)
{
    try
    {
        boost::filesystem::remove(path);
    }
    catch (...)
    {
        return(false);
    }
    return(true);
}

CFiles::CFiles()
{
}

void CFiles::GetFiles(std::string path, std::string ext)
{
    char* substr = NULL;
    mFiles.clear();
    mPath = path;
    std::string _ext = ext;

    if (ext != "" && ext != "*")
    {
        _ext = "." + ext;
    }

    boost::filesystem::path p(path);
    boost::filesystem::directory_iterator it(p);
    boost::filesystem::directory_entry de;

    for (; it != boost::filesystem::directory_iterator(); it++)
    {
        de = (*it);

        if (ext == "" || ext == "*")
        {
            std::string s = de.path().filename().string();
            mFiles.push_back(s);
        }
        else
        {
            std::string s = de.path().filename().string();
            substr = strstr((char*)(s.c_str()), _ext.c_str());
            if (substr != NULL)
            {
                std::string s = de.path().filename().string();
                mFiles.push_back(s);
            }
        }
    }

	std::sort(mFiles.begin(), mFiles.end());
}