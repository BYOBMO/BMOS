#include "CFolderForm.h"
#include "CListBox.h"
#include "CFiles.h"

CFolderForm::CFolderForm() :CForm()
{
	//printf("CFolderForm()\n");
	Setup();
}

CFolderForm::CFolderForm(int x, int y, int w, int h):CForm(x, y, w, h)
{

	//printf("CFolderForm(int x, int y, int w, int h)\n");
	Setup();
}

CFolderForm::~CFolderForm()
{
	//printf("~CFolderForm()\n");
	mPanel->RemoveWindow(mListBox);
	delete(mListBox);
}

void CFolderForm::Setup()
{
	int i;
	using namespace std::placeholders;

	CListBoxItem* lbi;
	mFolderPath = "NONE";
	mListBox = new CListBox();
	mListBox->SetSize(Width(), mPanel->GetHeight());

	mListBox->SetPosition(0, 0);
	mListBox->AddItemRemovedHandler(std::bind(&CFolderForm::OnItemRemoved, this, _1));
	mPanel->AddWindow(mListBox);

	mListBox->SetScrollPos(0);
}

void CFolderForm::Draw()
{
	SizeToListbox();
	CForm::Draw();
}

void CFolderForm::SizeToListbox()
{
	if (mListBox->Width() > Width())
	{
		int _x, _y;
		_x = X;
		_y = Y;

		Resize(mListBox->Width(), mTexH);

		X = _x;
		Y = _y;
	}
}

void CFolderForm::OnItemRemoved(CListBoxItem* item)
{
	for (int i = 0; i < mItemRemovedCallbacks.size(); i++)
	{
		if (mItemRemovedCallbacks[i])
		{
			mItemRemovedCallbacks[i](this, item);
		}
	}
}

CListBoxItem* CFolderForm::AddItem(std::string col1, std::string col2)
{
	return(mListBox->AddItem(col1, col2));
}

CListBoxItem* CFolderForm::AddItem(std::string col1, std::string col2, SDL_Color color)
{
	return(mListBox->AddItem(col1, col2, color));
}

CListBoxItem* CFolderForm::AddItem(std::string col1, std::string col2, SDL_Color color, CFile::Type iconType)
{
	return(mListBox->AddItem(col1, col2, color, iconType));
}

void CFolderForm::OnListItemDoubleClick(CListBoxItem* lbi)
{
	//printf("CFolderForm:Double click %s\n", lbi->mColumn1.c_str());
	for (int i = 0; i < mItemDoubleClickedCallbacks.size(); i++)
	{
		if (mItemDoubleClickedCallbacks[i])
		{
			mItemDoubleClickedCallbacks[i](this, lbi);
		}
	}
}

void CFolderForm::OnListItemClick(CListBoxItem* lbi)
{
	//printf("Click %s\n", lbi->mColumn1.c_str());
}

void CFolderForm::AddItemRemovedHandler(std::function<void(CFolderForm*, CListBoxItem*)> callback)
{
	mItemRemovedCallbacks.push_back(callback);
}

void CFolderForm::AddItemDoubleClickedHandler(std::function<void(CFolderForm*, CListBoxItem*)> callback)
{
	mItemDoubleClickedCallbacks.push_back(callback);
}

void CFolderForm::AddFileCopiedHandler(std::function<void(std::string)> callback)
{
	mFileCopiedCallbacks.push_back(callback);
}

void CFolderForm::CopyFile(std::string file)
{
	for (int i = 0; i < mFileCopiedCallbacks.size(); i++)
	{
		if (mFileCopiedCallbacks[i])
		{
			mFileCopiedCallbacks[i](file);
		}
	}
}

void CFolderForm::ClearList()
{
	mListBox->Clear();
}

void CFolderForm::Dropped(CWindow* theItem, DropAction action, CWindow* dropTarget)
{
	CListBoxItem* lbi = dynamic_cast<CListBoxItem*>(theItem);
	if (action == DropAction::Copy && lbi)
	{

			CForm* form = CForm::FormFromWindow(dropTarget);
			if (form != NULL)
			{
				CFolderForm* toFolder = dynamic_cast<CFolderForm*>(form);

				if (toFolder)
				{

					std::string to = toFolder->mFolderPath + lbi->mColumn1;
					std::string from = this->mFolderPath + lbi->mColumn1;

					toFolder->CopyFile(from);
				}
			}
	}
}