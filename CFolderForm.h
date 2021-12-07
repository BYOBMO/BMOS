#pragma once
#include "CForm.h"
#include "CListBox.h"
#include "CVScrollBar.h"

#include <vector>

class CFolderForm : public CForm
{
public:

	CListBox* mListBox;
	CVScrollBar* mScrollBar;

	std::string mFolderPath;

	CFolderForm();
	CFolderForm(int x, int y, int w, int h);
	virtual ~CFolderForm();

	CListBoxItem* AddItem(std::string col1, std::string col2);
	CListBoxItem* AddItem(std::string col1, std::string col2, SDL_Color color);
	CListBoxItem* AddItem(std::string col1, std::string col2, SDL_Color color, CFile::Type iconType);

	void OnListItemDoubleClick(CListBoxItem* lbi);
	void OnListItemClick(CListBoxItem* lbi);
	void OnItemRemoved(CListBoxItem* item);

	void Dropped(CWindow* theItem, DropAction action, CWindow* dropTarget);

	void AddItemRemovedHandler(std::function<void(CFolderForm*, CListBoxItem*)> callback);
	std::vector<std::function<void(CFolderForm*, CListBoxItem*)>> mItemRemovedCallbacks;

	void AddItemDoubleClickedHandler(std::function<void(CFolderForm*, CListBoxItem*)> callback);
	std::vector<std::function<void(CFolderForm*, CListBoxItem*)>> mItemDoubleClickedCallbacks;

	void AddFileCopiedHandler(std::function<void(std::string)> callback);
	std::vector<std::function<void(std::string)>> mFileCopiedCallbacks;

	void CopyFile(std::string file);

	virtual void Draw();

	void SizeToListbox();
	void ClearList();

	void Setup();
};

