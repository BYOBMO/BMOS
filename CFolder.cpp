#include "CFolder.h"
#include "CFiles.h"
#include <boost/filesystem.hpp>
#include <functional>
#include "CApplication.h"

CFolder::CFolder():CIcon("images/folder.png", 3)
{
	mForm = NULL;
	mName = "";
	mPath = "";
	mFilter = "*";
}

CFolder::CFolder(std::string path, int numFrames): CIcon(path, numFrames)
{
	mForm = NULL;
	mName = "";
	mPath = "";
	mFilter = "*";
}

void CFolder::ReadFolder(std::string path, std::string ext)
{
	CFiles files;

	files.GetFiles(path, ext);

	for (int i = 0; i < files.mFiles.size(); i++)
	{
		AddFile(files.mFiles[i]);
	}
}

void CFolder::AddFile(std::string filename)
{
	mFiles.push_back(CFile(filename, "", CFile::Type::NONE));
}

void CFolder::AddFile(std::string filename, std::string size)
{
	mFiles.push_back(CFile(filename, size, CFile::Type::NONE));
}

void CFolder::AddFile(std::string filename, std::string size, CFile::Type type)
{
	mFiles.push_back(CFile(filename, size, type));
}

void CFolder::AddFiles(std::vector<CFile> files)
{
	ClearFiles();
	for (int i = 0; i < files.size(); i++)
	{
		mFiles.push_back(files[i]);
	}
}

void CFolder::ClearFiles()
{
	mFiles.clear();
}

void CFolder::CreateFileList(bool draggable)
{
	CListBoxItem* lbi;
	using namespace std::placeholders;

	if (mForm == NULL)
	{
		return;
	}

	mForm->ClearList();
	for (int i = 0; i < mFiles.size(); i++)
	{
		if (mFiles[i].mType == CFile::Type::CORE)
		{
			lbi = mForm->AddItem(mFiles[i].mName, mFiles[i].mSize, CText::cRed, mFiles[i].mType);
		}
		else
		{
			lbi = mForm->AddItem(mFiles[i].mName, mFiles[i].mSize, CText::cBlack, mFiles[i].mType);
		}
		lbi->AddDoubleClickHandler(std::bind(&CFolderForm::OnListItemDoubleClick, mForm, _1));
		
		lbi->mDraggable = draggable;
	}

	mForm->SizeToListbox();
}
void CFolder::AddItemDoubleClickedHandler(std::function<void(CFolderForm*, CListBoxItem*)> callback)
{
	if (mForm == NULL)
	{
		return;
	}

	mForm->AddItemDoubleClickedHandler(callback);
}

CFolderForm* CFolder::CreateForm(int x, int y, int w, int h)
{
	using namespace std::placeholders;

	mForm = new CFolderForm(x, y, w, h);
	mForm->AddItemRemovedHandler(std::bind(&CFolder::OnFolderItemRemoved, this, _1, _2));
	mForm->AddFileCopiedHandler(std::bind(&CFolder::OnFolderReceiveCopy, this, _1));
	return(mForm);
}

//void CFolder::OnFolderReceiveCopy(std::string fromFolderPath, std::string fromFileName)
//{
//	std::string from = fromFolderPath + "/" + fromFileName;
//	std::string to = this->mPath + "/" + fromFileName;
//	printf("Copy %s to %s\n", from, to);
//
//	CFiles::Copy(from, to);
//}

void CFolder::OnFolderReceiveCopy(std::string from)
{
	bool b;

	boost::filesystem::path p(from);
	std::string to;

	to = mPath + "/" + p.filename().string();

	b = CFiles::Copy(from, to);
	if (b == true)
	{
		if (mForm != NULL)
		{
			CFiles files;

			std::vector<CFile> fileObjects;
			files.GetFiles(mPath, mFilter);

			for (int i = 0; i < files.mFiles.size(); i++)
			{
				CFile::Type type = CFile::MapType(mFilter);
				fileObjects.push_back(CFile(files.mFiles[i], " ", type));
			}

			AddFiles(fileObjects);
			CreateFileList(true);
		}
	}
}

void CFolder::OnFolderItemRemoved(CForm* form, CListBoxItem* item)
{
	CFolderForm* folderForm = dynamic_cast<CFolderForm*>(form);

	if (folderForm)
	{
		if (folderForm->mFolderPath != "NONE" && folderForm->mFolderPath != "")
		{
			if (strstr(folderForm->mFolderPath.c_str(), "bmos/") != NULL)
			{
				std::string fpath = folderForm->mFolderPath + "/" + item->mColumn1;
				//printf("Delete %s\n", fpath.c_str());
				CFiles::Delete(fpath);
			}
		}
	}

	for (auto it = std::begin(mFiles); it != std::end(mFiles); ++it)
	{
		CFile file = (*it);

		if (file.mName == item->mColumn1)
		{
			mFiles.erase(it);
			break;
		}
	}
}