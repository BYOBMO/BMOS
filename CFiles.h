#pragma once

#include <vector>
#include <string>
using namespace std;

class CFiles
{
public:

	std::vector<std::string> mFiles;
	std::string mPath;
	CFiles();

	static bool Copy(std::string from, std::string to);
	static bool Delete(std::string path);
	void GetFiles(std::string path, std::string ext);
};

