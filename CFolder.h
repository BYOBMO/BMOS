#pragma once
#include "CWindow.h"
#include "CIcon.h"
#include "CFolderForm.h"
#include <vector>
#include "CFile.h"

using namespace std;

class CFolder :
    public CIcon
{
public:
    std::string mName;
    std::string mPath;
    std::string mFilter;

    CFolderForm* mForm;

    std::vector<CFile> mFiles;

    CFolder();
    CFolder(std::string path, int numFrames);
    void ReadFolder(std::string path, std::string ext);
    void AddFile(std::string filename);
    void AddFile(std::string filename, std::string size);
    void AddFile(std::string filename, std::string size, CFile::Type type);
    void AddFiles(std::vector<CFile> files);
    void ClearFiles();

    CFolderForm* CreateForm(int x, int y, int w, int h);
    void CreateFileList(bool draggable);
    void AddItemDoubleClickedHandler(std::function<void(CFolderForm *form, CListBoxItem*)> callback);

    void OnFolderItemRemoved(CForm* form, CListBoxItem* item);
    void OnFolderReceiveCopy(std::string from);
};

