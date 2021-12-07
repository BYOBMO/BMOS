#pragma once
#include <string>

class CVoiceCommand
{
public:
	CVoiceCommand();
	std::string mMessage;
	std::string mCommand;
	std::string mArgument;
};

