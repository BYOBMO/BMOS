#pragma once
#include "CSurface.h"

#include <string>
#include <vector>

class CFile
{
public:
	enum class Type
	{
		EXE,
		TXT,
		BIN,
		CORE,
		FOLDER,
		BMO,
		BMO1,
		G,
		Q,
		FILE,
		DTH,
		JPG,
		NONE
	};

	static std::vector<CSurface*> sIcons;
	static void Init();
	static void Deinit();
	static Type MapType(std::string ext);

	std::string mName;
	std::string mSize;
	Type mType;

	CFile(std::string name, std::string size, Type fileType);

};

