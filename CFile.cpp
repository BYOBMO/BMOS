#include "CFile.h"
#include "CApplication.h"

std::vector<CSurface*> CFile::sIcons;
void CFile::Init()
{
	for (int i = 0; i <= (int)Type::NONE; i++)
	{
		sIcons.push_back(new CSurface());
	}
	
	sIcons[(int)Type::BIN]->LoadImage("images/icon_bin.png");
	sIcons[(int)Type::BMO]->LoadImage("images/icon_bmo.png");
	sIcons[(int)Type::BMO1]->LoadImage("images/icon_bmo1.png");
	sIcons[(int)Type::CORE]->LoadImage("images/icon_core.png");
	sIcons[(int)Type::EXE]->LoadImage("images/icon_exe.png");
	sIcons[(int)Type::FOLDER]->LoadImage("images/icon_folder.png");
	sIcons[(int)Type::TXT]->LoadImage("images/icon_txt.png");
	sIcons[(int)Type::FILE]->LoadImage("images/icon_file.png");
	sIcons[(int)Type::G]->LoadImage("images/icon_g.png");
	sIcons[(int)Type::Q]->LoadImage("images/icon_q.png");
	sIcons[(int)Type::DTH]->LoadImage("images/icon_dth.png");
	sIcons[(int)Type::JPG]->LoadImage("images/icon_jpg.png");
	sIcons[(int)Type::NONE]->LoadImage("images/icon_none.png");
}

void CFile::Deinit()
{
	for (int i = 0; i < sIcons.size(); i++)
	{
		delete(sIcons[i]);
	}
}

CFile::CFile(std::string name, std::string size, Type fileType)
{
	mTag = 0;
	mName = name;
	mSize = size;
	mType = fileType;
}

CFile::CFile(std::string name, std::string size, Type fileType, int tag)
{
	mTag = tag;
	mName = name;
	mSize = size;
	mType = fileType;
}

CFile::Type CFile::MapType(std::string ext)
{
	if (ext == "jpg")
	{
		return(CFile::Type::JPG);
	}
	else if (ext == "bin")
	{
		return(CFile::Type::BIN);
	}
	else if (ext == "mp4")
	{
		return(CFile::Type::BMO);
	}
	else if (ext == "exe")
	{
		return(CFile::Type::EXE);
	}

	return(CFile::Type::TXT);
}