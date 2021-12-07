#include "CTextReader.h"
#include "CText.h"
#include "CVScrollBar.h"
#include <string.h>
#include "CSurfWindow.h"
#include "CApplication.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include<boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

CTextReader::CTextReader(int x, int y, int w, int h) : CForm(x, y, w, h)
{
	using namespace std::placeholders;
	int panelW = mPanel->Width();

	mTextScroll = new CTextScroll();
	mTextScroll->SetPosition(0, 0);
	mTextScroll->AddTextPositionHandler(std::bind(&CTextReader::OnTextPositionChange, this, _1));
	mPanel->AddWindow(mTextScroll);
	mScrollBar = new CVScrollBar();
	mScrollBar->AddOnScrollHandler(std::bind(&CTextReader::OnScroll, this, _1));
	mScrollBar->SetPosition(mPanel->Width() - mScrollBar->Width(), 0);
	mScrollBar->SetHeight(mPanel->GetHeight());
	mPanel->AddWindow(mScrollBar);

	mTextScroll->SetSize(Width() - mScrollBar->Width(), mPanel->GetHeight());
}

CTextReader::~CTextReader()
{
	RemoveWindow(mScrollBar);
	mPanel->RemoveWindow(mTextScroll);

	delete(mScrollBar);

	delete(mTextScroll);
}

void CTextReader::OnScroll(int position)
{

	if (position < 0)
		position = 0;

	if (position != mTextScroll->mPosition)
	{
		mTextScroll->SetScroll(position);
	}
}

void CTextReader::OnTextPositionChange(CTextScroll *scroll)
{
	if (scroll->mPosition != mScrollBar->mPosition)
	{
		mScrollBar->SetPos(scroll->mPosition, false);
	}
}

void CTextReader::AddLine(string text)
{

	mTextScroll->AddLine(text);
	mScrollBar->SetRange(0, mTextScroll->mText.size()*TTF_FontHeight(CText::sFontSmall) - mTextScroll->mParent->GetHeight());

}

void CTextReader::SetText(std::string text, std::string caption)
{

	char* line;
	const char c[2] = "\n";

	line = strtok((char*)text.c_str(), c);
	mTitleBar.SetTitle(caption);

	while (line != NULL)
	{
		AddLine(line);
		line = strtok(NULL, c);
	}

	mTitleBar.SetTitle(caption);
}

void CTextReader::SetTtile(std::string title)
{
	mTitleBar.SetTitle(title);
}

void CTextReader::LoadFile(std::string filename)
{
	mTextScroll->mText.clear();

    ifstream file(filename.c_str(), ios::in);
    if (!file.is_open())
    {
        printf("Can't find file.\n");
        return;
    }
    string line;
    int i = 1;
	while (getline(file, line))
	{
		boost::algorithm::replace_all(line, "\r", "");
		boost::algorithm::replace_all(line, "\n", "");

		if (line == "")
		{
			line = " ";
		}
		AddLine(line);
		i++;

	}
    file.close();
	mTextScroll->SetScroll(0);
}