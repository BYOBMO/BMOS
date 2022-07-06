#pragma once
#include "CKeyCommand.h"

#include <string>
#include <vector>

class CKeyMap
{
public:
	CKeyMap();
	bool LoadFile(std::string path);
	CKeyCommand GetKey(std::string key);

	std::vector<CKeyCommand> mKeys;
	std::vector<std::string> mRandomVids;
	std::vector<std::string> mKeyedVids;
};

