//This file is part of BMOS.
//
//BMOS is free software : you can redistribute itand /or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//BMOS is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with BMOS.If not, see < https://www.gnu.org/licenses/>.

#include "CDesktop.h"
#include "CMenuBarItem.h"
#include "CDropDownMenu.h"
#include "CSettingsForm.h"
#include "CFolderForm.h"
#include "CIcon.h"
#include "CFolder.h"
#include "CMessageBox.h"
#include "CTextReader.h"
#include "CFiles.h"
#include "CFile.h"
#include "Utils.h"
#include "CPhotoViewer.h"
#include <stdio.h>
#include <ctime>
#include <boost/filesystem.hpp>
#ifndef WINDOWS
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

void SDL_Refresh();

CTexture* CDesktop::sBuffer = NULL;
CDesktop* CDesktop::sDesktop = NULL;
CWindow* CDesktop::sFocused = NULL;

int wpid = 0;

void CDesktop::BringFormToFront(CForm* frontForm)
{
	for (auto rit = std::begin(mForms); rit != std::end(mForms); ++rit)
	{
		CForm* form = (*rit);

		if (form == frontForm)
		{
			mForms.erase(rit);
			break;
		}
	}

	mForms.insert(mForms.begin(), frontForm);
	SetFocused(frontForm);
}

void CDesktop::BringIconToFront(CIcon* front)
{
	for (auto rit = std::begin(mIcons); rit != std::end(mIcons); ++rit)
	{
		CIcon* icon = (*rit);

		if (icon == front)
		{
			mIcons.erase(rit);
			break;
		}
	}

	mIcons.insert(mIcons.begin(), front);
}

void CDesktop::SetFocused(CWindow* window)
{
	if (window == sFocused)
	{
		return;
	}

	if (window == sDesktop)
	{
		//printf("Desktop focused\n");
	}
	if (sFocused != NULL)
	{
		sFocused->mSelected = false;
		sFocused->Focused(false);
	}
	sFocused = window;

	if (sFocused != NULL)
	{
		sFocused->Focused(true);
	}
}

CDropDownMenu* CDesktop::CreateMenu()
{
	CDropDownMenu* menu = new CDropDownMenu();

	if (sDesktop != NULL)
	{
		sDesktop->AddMenu(menu);
	}

	return(menu);
}

void CDesktop::RemoveMenu(CDropDownMenu* menu)
{
	if (sDesktop != NULL)
	{
		sDesktop->DeleteMenu(menu);
	}
}

CDesktop::~CDesktop()
{
	//printf("~CDesktop()\n");
	CFile::Deinit();
}

CDesktop::CDesktop(): CWindow()
{
	//printf("CDesktop()\n");
	CFile::Init();
	mOSStatus = true;
	sDesktop = this;

	mVoiceReboot = false;
	mVoiceShutdown = false;

	mFace = NULL;
	mCursor = NULL;
	mTrash = NULL;
	mFAQ = NULL;
	mRecording = false;
	mRecordPid = 0;
	mDragWindow = NULL;

	mMessageBox = NULL;
	mMouseOverWindow = NULL;

	mFaceTimer = UINT32_MAX;
	mRandomVideoTimer = UINT32_MAX;
	mSlideTimer = UINT32_MAX;

	mBlueButtonTimeout = UINT32_MAX;
	mDPadMouse = true;
	mKeyMouseEvent.button.timestamp = UINT32_MAX;

	std::srand(std::time(nullptr));

	LoadSettings();

	try
	{
		CVoiceCommand vc;

		mVoiceCommands.GetConfig(CApplication::sBMOS_Root + "commands.conf");
	}
	catch (...)
	{
		printf("Error reading voice commands.\n");
	}

	try
	{
		bool b = mKeyMap.LoadFile(CApplication::sBMOS_Root + "bmo.txt");
	}
	catch (...)
	{
	}
}

CDesktop::CDesktop(int w, int h): CDesktop()
{
	//printf("CDesktop(%d, %d)\n", w, h);
	using namespace std::placeholders;

	mCurrentVideo = 0;

	mCurrentFace = 0;
	mFaces.GetFiles(CApplication::sBMOS_Root + "/faces/", "jpg");

	mCurrentSlide = 0;
	mPictures.GetFiles(CApplication::sBMOS_Root + "/pictures/", "jpg");


	mCursor = new CTexture();
	mCursor->LoadFromFile("images/cursor.png");
	mCursor->SetPosition(50, h - mCursor->GetHeight());
	SDL_WarpMouseInWindow(CApplication::sWindow, mCursor->X, mCursor->Y);

	mFiles1.push_back(CFile("command.electricslide.exe", RandomSize(), CFile::Type::EXE));
	mFiles1.push_back(CFile("command.cleanjokes.exe", RandomSize(), CFile::Type::EXE));
	mFiles1.push_back(CFile("concblow.wav", RandomSize(), CFile::Type::BIN));
	mFiles1.push_back(CFile("CORE SYSTEM DRIVERS.sys", RandomSize(), CFile::Type::CORE));
	mFiles1.push_back(CFile("controls.bmochop.sys", RandomSize(), CFile::Type::BIN));
	mFiles1.push_back(CFile("controls.bmokick.sys", RandomSize(), CFile::Type::BIN));
	mFiles1.push_back(CFile("controls.brownnote.sys", RandomSize(), CFile::Type::BIN));
	mFiles1.push_back(CFile("controls.ForceField.sys", RandomSize(), CFile::Type::BIN));
	mFiles1.push_back(CFile("Frownrecognition.sys", RandomSize(), CFile::Type::BIN));

	mFiles2.push_back(CFile("Farerecognition.txt", RandomSize(), CFile::Type::TXT));
	mFiles2.push_back(CFile("Filesourcescript.exe", RandomSize(), CFile::Type::EXE));
	mFiles2.push_back(CFile("Filesource.source.exe", RandomSize(), CFile::Type::EXE));
	mFiles2.push_back(CFile("FinnStuff", "-", CFile::Type::FOLDER));
	mFiles2.push_back(CFile("Fundament.eth.sys", RandomSize(), CFile::Type::BIN));
	mFiles2.push_back(CFile("Fundament.sys", RandomSize(), CFile::Type::BIN));
	mFiles2.push_back(CFile("GooberHunt.dmg", RandomSize(), CFile::Type::G));
	mFiles2.push_back(CFile("GooberHunt.info.txt", RandomSize(), CFile::Type::G));
	mFiles2.push_back(CFile("GooberHunt.remove.exe", RandomSize(), CFile::Type::G));
	mFiles2.push_back(CFile("glyph.source.exe", RandomSize(), CFile::Type::EXE));


	mFiles3.push_back(CFile("Cold_Fusion.pdf", RandomSize(), CFile::Type::TXT));
	mFiles3.push_back(CFile("Backup.zip", RandomSize(), CFile::Type::BIN));
	mFiles3.push_back(CFile("Frontdown.zip", RandomSize(), CFile::Type::BIN));
	mFiles3.push_back(CFile("passwords.txt", RandomSize(), CFile::Type::TXT));
	mFiles3.push_back(CFile("PotatoRecipes.pdf", RandomSize(), CFile::Type::TXT));
	mFiles3.push_back(CFile("GayBlade.source.exe", RandomSize(), CFile::Type::G));
	mFiles3.push_back(CFile("KnifeStorm.exe", RandomSize(), CFile::Type::EXE));
	mFiles3.push_back(CFile("KnifeStorm.sav", RandomSize(), CFile::Type::BIN));
	mFiles3.push_back(CFile("debug.trace.bin", RandomSize(), CFile::Type::BIN));
	mFiles3.push_back(CFile("WordsOfWisdom.txt", RandomSize(), CFile::Type::TXT));
	mFiles3.push_back(CFile("AmongUs.lnk", RandomSize(), CFile::Type::BIN));
	mFiles3.push_back(CFile("BlueScreen.dth", RandomSize(), CFile::Type::DTH));

	CFolder* folder;

	mMenuBar.CreateBlank(w, 40, SDL_TEXTUREACCESS_TARGET);
	mMenuBar.AddIcon("images/start.png");
	CMenuBarItem* item;

	item = new CMenuBarItem();
	item->SetTitle("FILE");
	mMenuBar.AddMenuItem(item, 0, std::bind(&CDesktop::OnFile, this, _1));
	item->AddMenuItem("Open", std::bind(&CDesktop::OnFileMenu, this, _1));
	item->AddMenuItem("Save As", std::bind(&CDesktop::OnFileMenu, this, _1));
	item->AddMenuItem("Exit", std::bind(&CDesktop::OnFileMenu, this, _1));
	item->AddMenuItem("---------", std::bind(&CDesktop::OnFileMenu, this, _1));
	item->AddMenuItem("Shutdown", std::bind(&CDesktop::OnFileMenu, this, _1));
	item->AddMenuItem("Reboot", std::bind(&CDesktop::OnFileMenu, this, _1));
	SetFocused(item);

	item = new CMenuBarItem();
	item->SetTitle("EDIT");
	mMenuBar.AddMenuItem(item, 1, std::bind(&CDesktop::OnEdit, this, _1));
	item->AddMenuItem("WiFi", std::bind(&CDesktop::OnEditMenu, this, _1));
	item->AddMenuItem("Settings", std::bind(&CDesktop::OnMenuSettings, this, _1));
	item->AddMenuItem("Audio", std::bind(&CDesktop::OnMenuAudio, this, _1));

	item = new CMenuBarItem();
	item->SetTitle("VIEW");
	mMenuBar.AddMenuItem(item, 2, std::bind(&CDesktop::OnView, this, _1));
	item->AddMenuItem("Photos", std::bind(&CDesktop::OnMenuPhotos, this, _1));
	item->AddMenuItem("Beemotes", std::bind(&CDesktop::OnMenuVoices, this, _1));
	item->AddMenuItem("Video Games", std::bind(&CDesktop::OnMenuVideoGames, this, _1));
	if (boost::filesystem::exists(CApplication::sBMOS_Root + "/parade/parade") &&
		boost::filesystem::exists(CApplication::sBMOS_Root + "/credentials.json"))
	{
		item->AddMenuItem("Conversation Parade", std::bind(&CDesktop::OnMenuParade, this, _1));
	}
	item->AddMenuItem("Hi Scores", std::bind(&CDesktop::OnMenuHiScores, this, _1));
	item->AddMenuItem("Lo Scores", std::bind(&CDesktop::OnMenuLoScores, this, _1));


	mWindowsMenu = new CMenuBarItem();
	mWindowsMenu->SetTitle("WINDOW");
	mMenuBar.AddMenuItem(mWindowsMenu, 3, std::bind(&CDesktop::OnWindows, this, _1));


	item = new CMenuBarItem();
	item->SetTitle("HELP");
	mMenuBar.AddMenuItem(item, 4, std::bind(&CDesktop::OnHelp, this, _1));
	item->AddMenuItem("About", std::bind(&CDesktop::OnMenuAbout, this, _1));
	item->AddMenuItem("FAQ", std::bind(&CDesktop::OnMenuFAQ, this, _1));
	item->AddMenuItem("---------", std::bind(&CDesktop::OnHelp, this, _1));
	item->AddMenuItem("Repair", std::bind(&CDesktop::OnMenuRepair, this, _1));

	mMenuBar.AddRightIcon("images/dpad.png", 1);
	mMenuBar.mRightIcon->mVisible = true;
	LayoutWindows();


	mMessageBox = new CMessageBox(175, 150, 450, 350);
	mMessageBox->SetTitle("Caption");
	AddWinForm(mMessageBox);
	mMessageBox->AddLine("Shutdown?");

	mMessageBox->Hide();



	mTrash = new CIcon("images/trash.png", 2);
	mTrash->SetPosition(800 - mTrash->m_Width-15, 600 - mTrash->mTexH-25);
	mTrash->SetBounds(2, 43, 70, mTrash->mTexH - 40);
	mTrash->mDraggable = false;
	AddIcon(mTrash);
	mTrash->AddHandler(std::bind(&CDesktop::OnTrash, this, _1));

	mDiskDrive = new CFolder("images/diskdrive.png", 3);
	mDiskDrive->SetPosition(15, 600 - mDiskDrive->mTexH - 25);
	//mDiskDrive->SetBounds(2, 43, 70, mDiskDrive->mTexH - 40);
	mDiskDrive->mDraggable = false;
	mDiskDrive->mName = "USB Drive";
	mDiskDrive->AddDoubleClickHandler(std::bind(&CDesktop::OnDriveOpen, this, _1));
	AddIcon(mDiskDrive);


	folder = new CFolder();
	folder->SetPosition(20, 80);
	folder->SetBounds(2, 2, folder->m_Width -4, folder->mTexH - 4);
	folder->AddClickHandler(std::bind(&CDesktop::OnIconSelected, this, _1));
	folder->mDraggable = true;
	folder->AddDoubleClickHandler(std::bind(&CDesktop::OnFolderOpen, this, _1));
	folder->AddFiles(mFiles1);
	folder->mName = "Important";
	AddIcon(folder);

	folder = new CFolder();
	folder->SetPosition(625, 300);
	folder->SetBounds(2, 2, folder->m_Width - 4, folder->mTexH - 4);
	folder->AddDoubleClickHandler(std::bind(&CDesktop::OnFolderOpen, this, _1));
	folder->mDraggable = true;
	folder->AddFiles(mFiles2);
	folder->mName = "Stuff";
	AddIcon(folder);

	folder = new CFolder();
	folder->SetPosition(500, 150);
	folder->SetBounds(2, 2, folder->m_Width - 4, folder->mTexH - 4);
	folder->AddDoubleClickHandler(std::bind(&CDesktop::OnFolderOpen, this, _1));
	folder->mDraggable = true;
	folder->AddFiles(mFiles2);
	folder->mName = "More Stuff";
	AddIcon(folder);

	folder = new CFolder();
	folder->SetPosition(150, 375);
	folder->SetBounds(2, 2, folder->m_Width - 4, folder->mTexH - 4);
	folder->AddDoubleClickHandler(std::bind(&CDesktop::OnFolderOpen, this, _1));
	folder->mDraggable = true;
	folder->AddFiles(mFiles3);
	folder->mName = "Potatoes";
	AddIcon(folder);
	

	mSettingsForm = new CSettingsForm(150, 50, 500, 375);
	mSettingsForm->SetTitle("Settings");
	mSettingsForm->AddOKHandler(std::bind(&CDesktop::OnSettingsOK, this, _1));
	mSettingsForm->Hide();

	mAudioForm = new CAudioForm(150, 50, 500, 375);
	mAudioForm->SetTitle("Audio Settings");
	mAudioForm->AddOKHandler(std::bind(&CDesktop::OnAudioOK, this, _1));
	mAudioForm->Hide();

	mWiFiForm = new CWiFi(150, 50, 500, 275);
	mWiFiForm->SetTitle("WiFi Settings");
	mWiFiForm->Hide();

	std::vector<CFile> questions;

	questions.push_back(CFile("Getting started.", " ", CFile::Type::Q, 1));
	questions.push_back(CFile("How can I view images?", " ", CFile::Type::Q, 2));
	questions.push_back(CFile("What are beemotes?", " ", CFile::Type::Q, 3));
	questions.push_back(CFile("How do I setup WiFi?", " ", CFile::Type::Q, 4));
	questions.push_back(CFile("Will BMO listen to me?", " ", CFile::Type::Q, 5));
	questions.push_back(CFile("How do I play video games?", " ", CFile::Type::Q, 6));
#if IS_BMO
	questions.push_back(CFile("How can I restore your damaged files?", " ", CFile::Type::Q, 7));
#endif

	questions.push_back(CFile("What time is it?", " ", CFile::Type::Q, 8));
	questions.push_back(CFile("What do homies do?", " ", CFile::Type::Q, 9));

#if IS_BMO
	questions.push_back(CFile("Are you an iPad?", " ", CFile::Type::Q, 10));
	questions.push_back(CFile("Open the pod bay doors.", " ", CFile::Type::Q, 11));
#endif

	//questions.push_back(CFile("Are you a robot?", " ", CFile::Type::Q));
	//questions.push_back(CFile("Can you fix my shower?", " ", CFile::Type::Q));
	//questions.push_back(CFile("What is 2 + 1?", " ", CFile::Type::Q));
	//questions.push_back(CFile("Can I push the red button?", " ", CFile::Type::Q));
	


	mFAQ = new CFolder();
	mFAQ->mVisible = false;
	mFAQ->mName ="FAQ  (Double click to view)";
	mFAQ->AddFiles(questions);

	mPhotos = new CFolder();
	mPhotos->mFilter = "jpg";
	mPhotos->mVisible = false;
	mPhotos->mName = "Photos";
#ifdef WINDOWS
	mPhotos->mPath = "c:/home/pi/bmos/pictures/";
#else
	mPhotos->mPath = "/home/pi/bmos/pictures/";
#endif // WINDOWS

	mBeemotes = new CFolder();
	mBeemotes->mFilter = "mp4";
	mBeemotes->mVisible = false;
	mBeemotes->mName = "Beemotes";
#ifdef WINDOWS
	mBeemotes->mPath = "c:/home/pi/bmos/videos/";
#else
	mBeemotes->mPath = "/home/pi/bmos/videos/";
#endif

	AddIcon(mFAQ);
	AddIcon(mPhotos);
	AddIcon(mBeemotes);

	AddWinForm(mSettingsForm);
	AddWinForm(mAudioForm);
	AddWinForm(mWiFiForm);
}

void CDesktop::LoadSettings()
{
	try
	{
		mSettings.LoadFile("settings.txt");
		mFaceTimer = UINT32_MAX;
		mRandomVideoTimer = UINT32_MAX;
		mSlideTimer = UINT32_MAX;

		mFaceTimer = SDL_GetTicks() + mSettings.mFaceInterval * 1000;
		if (mSettings.mSlideShow)
		{
			mSlideTimer = SDL_GetTicks() + mSettings.mSlideInterval * 1000;
		}
		if (mSettings.mRandomwVids)
		{
			mRandomVideoTimer = SDL_GetTicks() + mSettings.mRandomVidInterval * 1000 * 60;
		}

		mCurrentFace = 0;
		mFaces.GetFiles(CApplication::sBMOS_Root + "/faces/", "jpg");

		mCurrentSlide = 0;
		mPictures.GetFiles(CApplication::sBMOS_Root + "/pictures/", "jpg");
	}
	catch (...)
	{
	}
}

void CDesktop::OnTrash(int i)
{
	//printf("%d\n", i);
}

void CDesktop::OnFile(CMenuBarItem* item)
{

}

void CDesktop::OnEdit(CMenuBarItem* item)
{

}

void CDesktop::OnView(CMenuBarItem* item)
{

}


void CDesktop::OnHelp(CMenuBarItem* item)
{

}

void CDesktop::Init(bool desktop)
{
	if (desktop)
	{
		mVisible = true;
		if (mFace != NULL)
		{
			delete(mFace);
			mFace = NULL;
		}
	}
	else
	{
		mVisible = false;
		SetFace(1);
	}
}

void CDesktop::OnFileMenu(CMenuBarItem* item)
{
	using namespace std::placeholders;

	//printf("%s\n", item->mText.c_str());

	if (item->mText == "Exit")
	{
		mVisible = false;
		SetFace(1);
		//mDPadMouse = false;
		//mMenuBar.mRightIcon->mVisible = false;

		LoadSettings();
	}
	else if (item->mText == "Shutdown")
	{
		mMessageBox->ShowMessage("Do you really want to\nShutdown ?", "Message", CMessageBox::MessageStyle::OK_Cancel);
		mMessageBox->AddOnCloseHandler(std::bind(&CDesktop::OnConfirmShutdown, this, _1, _2));
	}
	else if (item->mText == "Reboot")
	{
		mMessageBox->ShowMessage("Reboot ?", "Message", CMessageBox::MessageStyle::OK_Cancel);
		mMessageBox->AddOnCloseHandler(std::bind(&CDesktop::OnConfirmReboot, this, _1, _2));
	}
}

void CDesktop::OnEditMenu(CMenuBarItem* item)
{
	//printf("%s\n", item->mText.c_str());
	if (item->mText == "WiFi")
	{
		mWiFiForm->SetIPAddress();
		mWiFiForm->Show();
	}
}

void CDesktop::OnViewMenu(CMenuBarItem* item)
{
	//printf("%s\n", item->mText.c_str());
}

void CDesktop::OnWindowMenu(CMenuBarItem* item)
{
	//printf("%s\n", item->mText.c_str());
	CForm* theForm = NULL;

	if (item->mText == "Video Games")
	{
		if (mOSStatus == false)
		{
			PlayVideo((char*)"uhuhuh.mp4", 0);
		}
		else
		{
			PlayVideo((char*)"videogames.mp4", 0);

			SDL_DestroyRenderer(CApplication::sRenderer);
			SDL_DestroyWindow(CApplication::sWindow);

			SDL_Quit();

#ifndef WINDOWS
			system("/home/pi/bmos/scripts/emulaunch.sh -desktop &");
#endif
			SDL_DestroyRenderer(CApplication::sRenderer);
			SDL_DestroyWindow(CApplication::sWindow);

			SDL_Quit();


			exit(EXIT_SUCCESS);
		}
	}
	else
	{
		for (auto rit = std::rbegin(mForms); rit != std::rend(mForms); ++rit)
		{
			CForm* form = (*rit);

			if (form->mVisible && form->mTitleBar.mTitle.mText == item->mText)
			{
				BringFormToFront(form);
				theForm = form;
				break;
			}
		}

		BringFormToFront(theForm);
	}
}

void CDesktop::OnHelpMenu(CMenuBarItem* item)
{
	//printf("%s\n", item->mText.c_str());
}


void CDesktop::OnWindows(CMenuBarItem* item)
{
	using namespace std::placeholders;

	//printf("OnWindows\n");

	mWindowsMenu->RemoveDropDown();
	
	for (auto rit = std::rbegin(mForms); rit != std::rend(mForms); ++rit)
	{
		CForm* form = (*rit);

		if (form->mVisible)
		{
			mWindowsMenu->AddMenuItem(form->mTitleBar.mTitle.mText, std::bind(&CDesktop::OnWindowMenu, this, _1));
		}
	}
}

void CDesktop::CloseAll()
{
	for (auto rit = std::rbegin(mMenus); rit != std::rend(mMenus); ++rit)
	{
		(*rit)->mVisible = false;
	}
}

void CDesktop::OnMenuOpen(CMenuBarItem* item)
{
	//printf("Open\n");
}

void CDesktop::OnMenuSave(CMenuBarItem* item)
{
	//printf("Save\n");
}

void CDesktop::OnMenuExit(CMenuBarItem* item)
{
	//printf("Exit\n");

	SetFace(1);
}

void CDesktop::OnMenuSettings(CMenuBarItem* item)
{
	mSettingsForm->SetFaceInterval(mSettings.mFaceInterval);
	mSettingsForm->SetRandomInterval(mSettings.mRandomVidInterval);
	mSettingsForm->SetSlideInterval(mSettings.mSlideInterval);
	mSettingsForm->SetRandomVideo(mSettings.mRandomwVids);
	mSettingsForm->SetSlideshow(mSettings.mSlideShow);

	mSettingsForm->Show();
}

void CDesktop::OnSettingsOK(CSettingsForm* form)
{
	mSettings.mFaceInterval = mSettingsForm->GetFaceInterval();
	mSettings.mRandomVidInterval = mSettingsForm->GetRandomInterval();
	mSettings.mSlideInterval = mSettingsForm->GetSlideInterval();
	mSettings.mRandomwVids = mSettingsForm->GetRandomVideo();
	mSettings.mSlideShow = mSettingsForm->GetSlideshow();

	try
	{
		mSettings.SaveFile("settings.txt");
	}
	catch (...)
	{
	}

	mFaceTimer = SDL_GetTicks() + mSettings.mFaceInterval * 1000;
}

void CDesktop::OnMenuAudio(CMenuBarItem* item)
{
	int volume, mic;
	volume = mSettings.GetAudioSetting(2);
	mic = mSettings.GetAudioSetting(1);

	mAudioForm->SetVolume(volume);
	mAudioForm->SetMic(mic);

	mAudioForm->SetVoiceRecognition(mSettings.mVoiceRecognition);

	mAudioForm->Show();
}

void CDesktop::OnAudioOK(CAudioForm* form)
{
	int volume, mic;

	volume = mAudioForm->GetVolume();
	if (volume != -1)
	{
		mSettings.SetAudioSetting(2, volume);
	}

	mic = mAudioForm->GetMic();
	if (mic != -1)
	{
		mSettings.SetAudioSetting(1, mic);
	}

	mSettings.mVoiceRecognition = mAudioForm->GetVoiceRecognition();

	try
	{
		mSettings.SaveFile("settings.txt");
	}
	catch (...)
	{
	}
}

void CDesktop::OnMenuHiScores(CMenuBarItem* item)
{
	ShowTextFile("Hi Scores", CApplication::sBMOS_Root + "/scores/hiscores.txt");
}

void CDesktop::OnMenuLoScores(CMenuBarItem* item)
{
	ShowTextFile("Lo Scores", CApplication::sBMOS_Root + "/scores/loscores.txt");
}

void CDesktop::OnMenuVideoGames(CMenuBarItem* item)
{
	if (mOSStatus == false)
	{
		PlayVideo((char*)"uhuhuh.mp4", 0);
	}
	else
	{
		PlayVideo((char*)"videogames.mp4", 0);
#ifndef WINDOWS
		SDL_DestroyRenderer(CApplication::sRenderer);
		SDL_DestroyWindow(CApplication::sWindow);

		SDL_Quit();


		system("/home/pi/bmos/scripts/emulaunch.sh -desktop &");

		SDL_DestroyRenderer(CApplication::sRenderer);
		SDL_DestroyWindow(CApplication::sWindow);

		SDL_Quit();


		exit(EXIT_SUCCESS);
#endif

	}
}

void CDesktop::OnMenuParade(CMenuBarItem* item)
{
	if (mOSStatus == false)
	{
		PlayVideo((char*)"uhuhuh.mp4", 0);
	}
	else
	{
#ifndef WINDOWS
		system("/home/pi/bmos/scripts/parade.sh");
#endif
	}
}

void CDesktop::OnMenuVoices(CMenuBarItem* item)
{
	using namespace std::placeholders;

	std::string path;

	if (mBeemotes->mForm == NULL)
	{
		int formx, formy;

		formx = 100;
		formy = 100;

		CFolderForm* f = mBeemotes->CreateForm(formx, formy, 250, 400);
		AddWinForm(f);

		f->AddOnCloseHandler(std::bind(&CDesktop::OnFolderClose, this, _1));
		mBeemotes->AddItemDoubleClickedHandler(std::bind(&CDesktop::OnVoiceItem, this, _1, _2));

		if (formx > 800 - f->Width())
		{
			f->X = 800 - f->Width() - 10;
		}

		if (formy > 600 - f->GetHeight())
		{
			f->Y = 600 - f->GetHeight() - 10;
		}


		CFiles files;
#ifdef WINDOWS
		path = "C:/home/pi/bmos/videos/";
#else
		path = "/home/pi/bmos/videos/";
#endif

		files.GetFiles(path, mBeemotes->mFilter);
		f->mFolderPath = path;

		std::vector<CFile> fileObjects;

		for (int i = 0; i < files.mFiles.size(); i++)
		{
			CFile::Type type = CFile::Type::BMO1;
			fileObjects.push_back(CFile(files.mFiles[i], " ", type));
		}

		mBeemotes->AddFiles(fileObjects);
		mBeemotes->CreateFileList(true);
		f->SetTitle(mBeemotes->mName);
		f->Show();


	}
}

void CDesktop::OnMenuPhotos(CMenuBarItem* item)
{
	using namespace std::placeholders;

	std::string path;

	if (mPhotos->mForm == NULL)
	{
		int formx, formy;

		formx = 100;
		formy = 100;

		CFolderForm* f = mPhotos->CreateForm(formx, formy, 250, 400);
		AddWinForm(f);

		f->AddOnCloseHandler(std::bind(&CDesktop::OnFolderClose, this, _1));
		mPhotos->AddItemDoubleClickedHandler(std::bind(&CDesktop::OnPhotoItem, this, _1, _2));

		if (formx > 800 - f->Width())
		{
			f->X = 800 - f->Width() - 10;
		}

		if (formy > 600 - f->GetHeight())
		{
			f->Y = 600 - f->GetHeight() - 10;
		}

		CFiles files;
#ifdef WINDOWS
		path = "C:/home/pi/bmos/pictures/";
#else
		path = "/home/pi/bmos/pictures/";
#endif

		std::vector<CFile> fileObjects;
		files.GetFiles(path, mPhotos->mFilter);
		f->mFolderPath = path;

		for (int i = 0; i < files.mFiles.size(); i++)
		{
			CFile::Type type = CFile::Type::JPG;
			fileObjects.push_back(CFile(files.mFiles[i], " ", type));
		}

		mPhotos->AddFiles(fileObjects);
		mPhotos->CreateFileList(true);
		f->SetTitle(mPhotos->mName);


		f->Show();


	}
}

void CDesktop::OnPhotoItem(CFolderForm* form, CListBoxItem* item)
{
	using namespace std::placeholders;

	std::string path;

	path = form->mFolderPath + item->mColumn1;
	//printf("%s\n", item->mColumn1.c_str());
	CPhotoViewer* photoView = new CPhotoViewer(0, mMenuBar.GetHeight(), 800, 600 - mMenuBar.GetHeight());
	photoView->LoadPicture(path);
	photoView->AddOnCloseHandler(std::bind(&CDesktop::OnFolderClose, this, _1));
	photoView->SetTitle(item->mColumn1);
	AddWinForm(photoView);
	BringFormToFront(photoView);
}

void CDesktop::OnVoiceClose(CForm* form)
{
	form->mDelete = true;
	sFocused = NULL;
}

void CDesktop::OnVoiceItem(CFolderForm* form, CListBoxItem* item)
{
	//printf("%s\n", item->mColumn1.c_str());
	PlayVideo((char*)item->mColumn1.c_str(), 0);
}

void CDesktop::OnMenuFAQ(CMenuBarItem* item)
{
	using namespace std::placeholders;
	int r1, r2;


	if (mFAQ != NULL && mFAQ->mForm == NULL)
	{
		int formx, formy;

		formx = 100;
		formy = 100;

		CFolderForm* f = mFAQ->CreateForm(formx, formy, 600, 350);
		CListBoxItem* lbi;


		f->AddOnCloseHandler(std::bind(&CDesktop::OnFolderClose, this, _1));
		mFAQ->AddItemDoubleClickedHandler(std::bind(&CDesktop::OnFAQItemDoubleClick, this, _1, _2));
		mFAQ->CreateFileList(false);


		AddWinForm(f);

		if (formx > 800 - f->Width())
		{
			f->X = 800 - f->Width() - 10;
		}

		if (formy > 600 - f->GetHeight())
		{
			f->Y = 600 - f->GetHeight() - 10;
		}
		f->SetTitle(mFAQ->mName);
		f->Show();
	}
}

void CDesktop::OnFAQItemDoubleClick(CFolderForm* form, CListBoxItem* lbi)
{
	//printf("Double click %s\n", lbi->mColumn1.c_str());

	if (lbi->mTag == 7) 
	{
		// How can I restore your damaged files?
		if (mOSStatus == false)
		{
			PlayVideo((char*)"mofactory.mp4", 0);
		}
		else
		{
			PlayVideo((char*)"RepairMo.mp4", 0);
		}
	}
	else if (lbi->mColumn1 == "Open the pod bay doors.")
	{
		if (mOSStatus == false)
		{
			PlayVideo((char*)"uhuhuh.mp4", 0);
		}
		else
		{
			PlayVideo((char*)"SorryDave.mp4", 0);
		}
	}
	else if (lbi->mColumn1 == "What do homies do?")
	{
		if (mOSStatus == false)
		{
			PlayVideo((char*)"uhuhuh.mp4", 0);
		}
		else
		{
			PlayVideo((char*)"homies.mp4", 0);
		}
	}
	else if (lbi->mColumn1 == "What time is it?")
	{
		if (mOSStatus == false)
		{
			PlayVideo((char*)"uhuhuh.mp4", 0);
		}
		else
		{
			PlayVideo((char*)"theme.mp4", 0);
		}
	}
	else if (lbi->mColumn1 == "Are you an iPad?")
	{
		if (mOSStatus == false)
		{
			PlayVideo((char*)"uhuhuh.mp4", 0);
		}
		else
		{
			PlayVideo((char*)"ipad2.mp4", 0);
		}
	}
	else if (lbi->mTag == 1)
	{
		// Getting started.
		ShowTextFile("Getting Started", CApplication::sBMOS_Root + "/faq/manual.txt");
	}
	else if (lbi->mColumn1 == "What are beemotes?")
	{
		ShowTextFile("Beemotes", CApplication::sBMOS_Root + "/faq/beemotes.txt");
	}
	else if (lbi->mColumn1 == "How can I view images?")
	{
		ShowTextFile("Images", CApplication::sBMOS_Root + "/faq/imageviewer.txt");
	}
	else if (lbi->mColumn1 == "How do I play video games?")
	{
		ShowTextFile("Video Games", CApplication::sBMOS_Root + "/faq/videogames.txt");
	}
	else if (lbi->mColumn1 == "Will BMO listen to me?")
	{
		ShowTextFile("Voice", CApplication::sBMOS_Root + "/faq/voice.txt");
	}
	else if (lbi->mColumn1 == "How do I setup WiFi?")
	{
		ShowTextFile("WiFi", CApplication::sBMOS_Root + "/faq/wifi.txt");
	}


}

void CDesktop::OnMenuAbout(CMenuBarItem* item)
{

	std::string msg = "BMOS\n \nVersion: 3.1415\nStatus: OK";
	if (mOSStatus == false)
	{
		msg = "BMOS\n \nVersion: 3.1415\nStatus: CORRUPTED\n \nContact MO Co. for support.";
	}

	mMessageBox->ShowMessage(msg, "About", CMessageBox::MessageStyle::OK, CMessageBox::MessageAlign::Center);


	mMessageBox->AddOnCloseHandler(nullptr);
}

void CDesktop::OnMenuRepair(CMenuBarItem* item)
{
	mOSStatus = true;
	PlayVideo((char*)"repair.mp4", 0);
	for (auto iconIt = mIcons.begin(); iconIt != mIcons.end(); iconIt++)
	{
		CFolder* _folder;
		CFolder* found = NULL;
		_folder = dynamic_cast<CFolder*>((*iconIt));

		if (_folder)
		{




			if (_folder->mName == "Important")
			{
				_folder->ClearFiles();
				_folder->AddFiles(mFiles1);
				found = _folder;
			}
			else if (_folder->mName == "Stuff")
			{
				_folder->ClearFiles();
				_folder->AddFiles(mFiles2);
				found = _folder;
			}
			else if (_folder->mName == "More Stuff")
			{
				_folder->ClearFiles();
				_folder->AddFiles(mFiles2);
				found = _folder;
			}
			else if (_folder->mName == "Potatoes")
			{
				_folder->ClearFiles();
				_folder->AddFiles(mFiles3);
				found = _folder;
			}

			if (found != NULL)
			{
				if (found->mForm != NULL)
				{
					found->mForm->mDelete = true;
				}
			}
		}
	}
}

void CDesktop::ShowTextFile(std::string title, std::string file)
{
	using namespace std::placeholders;

	CTextReader* tr = new CTextReader(13, 50, 775, 450);
	tr->AddOnCloseHandler(std::bind(&CDesktop::OnFolderClose, this, _1));
	tr->SetTitle(title);
	AddWinForm(tr);
	tr->LoadFile(file);

	tr->Show();
}




void CDesktop::OnIconSelected(CIcon* icon)
{
	for (auto rit = std::rbegin(mWindows); rit != std::rend(mWindows); ++rit) {
		(*rit)->mSelected = false;
	}

	icon->mSelected = true;
}

void CDesktop::AddWinForm(CForm* form)
{
	mForms.push_back(form);
	LayoutWindows();
}

void CDesktop::AddIcon(CIcon* icon)
{
	mIcons.push_back(icon);
	LayoutWindows();
}

void CDesktop::AddMenu(CDropDownMenu* menu)
{
	mMenus.push_back(menu);
	LayoutWindows();
}

void CDesktop::DeleteMenu(CDropDownMenu* menu)
{
	for (auto it = std::begin(mMenus); it != std::end(mMenus); ++it)
	{
		CDropDownMenu* m = (*it);

		if (menu == m)
		{
			mMenus.erase(it);
			break;
		}
	}
	LayoutWindows();
}

void CDesktop::LayoutWindows()
{
	mWindows.clear();

	for (auto rit = std::rbegin(mIcons); rit != std::rend(mIcons); ++rit) 
	{
		CIcon* icon = (*rit);

		AddWindow(icon);
	}

	for (auto rit = std::rbegin(mForms); rit != std::rend(mForms); ++rit)
	{
		CForm* form = (*rit);

		if (form != mMessageBox)
		{
			AddWindow(form);
		}
	}

	if (mMessageBox != NULL)
	{
		AddWindow(mMessageBox);
	}

	AddWindow(&mMenuBar);
	for (auto rit = std::rbegin(mMenus); rit != std::rend(mMenus); ++rit)
	{
		AddWindow(*rit);
	}

	if (mDragWindow != NULL)
	{
		AddWindow(mDragWindow);
	}
}


void CDesktop::SetDragWindow(CDragWindow* win)
{
	using namespace std::placeholders;

	if (mDragWindow != NULL)
	{
		delete(mDragWindow);
		mDragWindow = NULL;
	}

	win->AddDropHandler(std::bind(&CDesktop::OnDragDrop, this, _1));
	mDragWindow = win;
}



void CDesktop::OnDragDrop(CDragWindow* win)
{
	mDragWindow->mVisible = false;
	if (CWindow::sMouseMotionHandled != NULL)
	{
		CForm* form;
		
		//printf("Drop ID: %s\n", CWindow::sMouseMotionHandled->mID.c_str());
		if (CWindow::sMouseMotionHandled == mTrash)
		{
			if (win->mDragItem != NULL)
			{
				win->mDragItem->Dropped(win->mDragItem, DropAction::Delete, mTrash);
				if (win->mDragItem->mDelete)
				{
					delete(win->mDragItem);
					win->mDragItem = NULL;
				}
			}
			//printf("Trash\n");
		}
		else if ((form = CForm::FormFromWindow(CWindow::sMouseMotionHandled)) != nullptr)
		{
			CFolderForm* folderform;
			folderform = dynamic_cast<CFolderForm*>(form);
			if (folderform)
			{
				if (win->mDragItem != NULL)
				{
					win->mDragItem->Dropped(win->mDragItem, DropAction::Copy, folderform);
					//printf("Copy to folder\n");
				}
			}
		}
	}
	else
	{
		printf("Drop ID NULL\n");
	}
	mDragWindow->mDelete = true;


	//printf("Drop\n");
}

CWindow* CDesktop::OnMouseMotion(int x, int y)
{
	CWindow* win;

	if (mFace != NULL)
	{
		return(NULL);
	}
	
	mCursor->SetPosition(x, y);

	if (mMessageBox->mVisible)
	{
		win = mMessageBox->OnMouseMotion(x, y);
	}
	else if (mMouseOverWindow != NULL && mMouseOverWindow->mDrag == true)
	{
		int sx, sy;
		mMouseOverWindow->mParent->GetScreenCoordinates(&sx, &sy);
		mMouseOverWindow->OnMouseMotion(x-sx, y-sy);
		return(NULL);
	}
	else
	{
		win = CWindow::OnMouseMotion(x, y);
	}

	if (win != mMouseOverWindow)
	{

		CIcon* icon;
		icon = dynamic_cast<CIcon*>(sFocused);
		if (icon && icon->mDrag)
		{
			return(NULL);
		}

		if (win != NULL)
		{
			//printf("MouseOver %s\n", win->mID.c_str());
			win->MouseEnter();
		}
		else
		{
			//printf("MouseOver NULL\n");
		}

		if (mMouseOverWindow != NULL)
		{
			mMouseOverWindow->MouseLeave();
		}
	}

	mMouseOverWindow = win;
	//SetFocused(win); XXX

	return(NULL);
}


int CheckPid()
{
	int status;
	int pid=0;

#ifndef WINDOWS
	pid = waitpid(wpid, &status, WNOHANG);
	if (pid != 0)
	{
		printf("Thread over\n");
		wpid = 0;
	}
	else
	{
		//printf("pid = %d\n");
	}
#endif
	return(pid);
}

void CDesktop::GetJoystickAxis(bool* left, bool* right, bool* up, bool* down)
{
	Sint16 x_move, y_move;
	*left = false;
	*right = false;
	*up = false;
	*down = false;

	if (CApplication::sGameController == NULL)
	{
		return;
	}

	x_move = SDL_JoystickGetAxis(CApplication::sGameController, 0);
	y_move = SDL_JoystickGetAxis(CApplication::sGameController, 1);



	//Left of dead zone
	if (x_move < -8000)
	{
		*left = true;
	}
	//Right of dead zone
	else if (x_move > 8000)
	{
		*right = true;
	}


	//Y axis motion

		//Below of dead zone
	if (y_move < -8000)
	{
		*up = true;
	}
	//Above of dead zone
	else if (y_move > 8000)
	{
		*down = true;
	}


}

void CDesktop::Update()
{
	int mx, my;
	int x, y;
	bool dirty = false;
	Uint32 ticks = SDL_GetTicks();

	if (wpid != 0)
	{
		CheckPid();
		mFaceTimer = SDL_GetTicks() + mSettings.mFaceInterval * 1000;
	}

	if (mVisible == false)
	{
		if (mSettings.mRandomwVids)
		{
			if (mKeyMap.mRandomVids.size()>0 && SDL_TICKS_PASSED_FIXED(ticks, mRandomVideoTimer))
			{
				int r1 = std::rand();
				r1 = (r1 % mKeyMap.mRandomVids.size());
				PlayVideo((char*)mKeyMap.mRandomVids[r1].c_str(), 0);
				mRandomVideoTimer = SDL_GetTicks() + mSettings.mRandomVidInterval * 1000 * 60;
			}
		}

		if (mSettings.mSlideShow)
		{
			if (mVoiceReboot == false && mVoiceShutdown == false)
			{
				std::string path;
				path = CApplication::sBMOS_Root + "/pictures/";

				if (mPictures.mFiles.size() == 0)
				{
					mSettings.mSlideShow = false;
				}
				else if (SDL_TICKS_PASSED_FIXED(ticks, mSlideTimer))
				{
					mCurrentSlide++;
					if (mCurrentSlide >= mPictures.mFiles.size())
					{
						mCurrentSlide = 0;
					}
					SetFace(path, mPictures.mFiles[mCurrentSlide]);
					mSlideTimer = SDL_GetTicks() + mSettings.mSlideInterval * 1000;
				}
			}
		}
		else if (SDL_TICKS_PASSED_FIXED(ticks, mFaceTimer))
		{
			if (mVoiceReboot == false && mVoiceShutdown == false)
			{
				mCurrentFace++;
				if (mCurrentFace >= mFaces.mFiles.size())
				{
					mCurrentFace = 0;
				}
				SetFace(mCurrentFace);
				mFaceTimer = SDL_GetTicks() + mSettings.mFaceInterval * 1000;
			}
		}

	}

	for (auto rit = mForms.begin(); rit != mForms.end(); )
	{
		CForm* form = (*rit);

		if (form->mDelete)
		{
			//printf("Delete Form\n");
			for (auto iconIt = mIcons.begin(); iconIt != mIcons.end(); iconIt++)
			{
				CFolder* _folder;
				_folder = dynamic_cast<CFolder*>((*iconIt));

				if (_folder && _folder->mForm==form)
				{
					//printf("Remove Form from folder\n");
					_folder->mForm = NULL;
				}
			}

			dirty = true;
			delete(form);
			rit = mForms.erase(rit);
		}
		else
		{
			++rit;
		}
	}

	if (dirty && mForms.size() > 0)
	{
		for (auto rit = mForms.begin(); rit != mForms.end(); rit++)
		{
			CForm* form = (*rit);
			if (form->mVisible == true)
			{
				form->Show();
				break;
			}
		}
	}


	if (mDragWindow != NULL && mDragWindow->mDelete)
	{
		//printf("Delete mDragWindow\n");
		delete(mDragWindow);
		mDragWindow = NULL;
		dirty = true;
	}
	if (dirty)
	{
		LayoutWindows();
	}

	if (mVisible == false)
	{
		return;
	}

	SDL_GetMouseState(&mx, &my);
	//printf("%d, %d\n", mx, my);
	const Uint8* state = SDL_GetKeyboardState(NULL);
	bool up, down, left, right;

	x = mx;
	y = my;

	up = false;
	down = false;
	left = false;
	right = false;

	bool jup, jdown, jleft, jright;
	GetJoystickAxis(&jleft, &jright, &jup, &jdown);
	Uint8 jbut = false;
	
	if (CApplication::sGameController != NULL)
	{
		jbut = SDL_JoystickGetButton(CApplication::sGameController, 2);
	}


	if (state[SDL_SCANCODE_UP] == true && (state[SDL_SCANCODE_LSHIFT] == true || state[SDL_SCANCODE_RSHIFT] == true))
	{
		mVisible = !mVisible;
		if (mVisible == false)
		{
			SetFace(1);
		}
		return;
	}

	if (state[SDL_SCANCODE_LSHIFT]==false && state[SDL_SCANCODE_RSHIFT] == false && jbut == false)
	{
		mBlueButtonTimeout = UINT32_MAX;
	}
	else if (mBlueButtonTimeout == UINT32_MAX)
	{
		mBlueButtonTimeout = SDL_GetTicks() + 2000;
		//printf("Set timeout %u\n", mBlueButtonTimeout);
	}
	else if (SDL_TICKS_PASSED_FIXED(ticks, mBlueButtonTimeout))
	{

		mDPadMouse = !mDPadMouse;
		mMenuBar.mRightIcon->mVisible = mDPadMouse;
		//printf("DPad Mouse = %d %u\n", mDPadMouse, mBlueButtonTimeout);
		mBlueButtonTimeout = UINT32_MAX;
	}

	if (mDPadMouse)
	{
		if (state[SDL_SCANCODE_RIGHT] || jright)
		{
			right = true;
		}
		if (state[SDL_SCANCODE_LEFT] || jleft)
		{
			left = true;
		}
		if (state[SDL_SCANCODE_UP] || jup)
		{
			up = true;
		}
		if (state[SDL_SCANCODE_DOWN] || jdown)
		{
			down = true;
		}

		if (up)
		{
			my -= 5;
		}

		if (down)
		{
			my += 5;
		}

		if (left)
		{
			mx -= 5;
		}

		if (right)
		{
			mx += 5;
		}

		if (mx != x || my != y)
		{
			//printf("Warp %d %d, %d, %d, %d, %d\n", mx, my, up, down, left, right);
			SDL_WarpMouseInWindow(CApplication::sWindow, mx, my);
		}
	}
}

CWindow* CDesktop::OnKeyUp(SDL_KeyboardEvent e)
{
	if (wpid != 0)
	{
		//printf("Skip KeyUp");
		return(NULL);
	}

	if (e.keysym.sym == SDLK_LALT)
	{
		//printf("SDLK_LALT\n");
	}
	else if (e.keysym.sym == SDLK_LSHIFT)
	{
		//printf("SDLK_LSHIFT\n");
	}
	else if (e.keysym.sym == SDLK_LCTRL)
	{
		//printf("SDLK_LCTRL\n");
	}

	if ((e.keysym.sym == SDLK_LCTRL || e.keysym.sym == SDLK_RCTRL)  && mRecording == true)
	{
		mRecording = false;

		if (mVisible)
		{
			delete(mFace);
			mFace = NULL;
		}
		else
		{
			SetFace(0);
		}
		EndRecord();
		return(NULL);
	}

	if (mVisible==false && mFace!=NULL)
	{
		// Face mode
		if (e.keysym.sym == SDLK_LALT || e.keysym.sym == SDLK_RALT)
		{
			//printf("Face Up\n");
			int mx, my;
			SDL_GetMouseState(&mx, &my);

			mKeyMouseEvent.type = SDL_MOUSEBUTTONUP;
			mKeyMouseEvent.button.button = SDL_BUTTON_LEFT;
			mKeyMouseEvent.button.clicks = 1;
			mKeyMouseEvent.button.timestamp = SDL_GetTicks();
			mKeyMouseEvent.button.x = mx;
			mKeyMouseEvent.button.y = my;
		}
		else if (e.keysym.sym == SDLK_RETURN)
		{
		}

		return(NULL);
	}

	if (mDPadMouse)
	{
		// DPad mouse mode on desktop.
		if (e.keysym.sym == SDLK_LALT || e.keysym.sym == SDLK_RALT)
		{
			//printf("DPad Mouse Up\n");
			int mx, my;
			SDL_GetMouseState(&mx, &my);
			
			mKeyMouseEvent.type = SDL_MOUSEBUTTONUP;
			mKeyMouseEvent.button.button = SDL_BUTTON_LEFT;
			mKeyMouseEvent.button.clicks = 1;
			mKeyMouseEvent.button.timestamp = SDL_GetTicks();
			mKeyMouseEvent.button.x = mx;
			mKeyMouseEvent.button.y = my;

			SDL_PushEvent(&mKeyMouseEvent);
		}
		else if (e.keysym.sym == SDLK_RETURN)
		{
		}

		return(NULL);
	}

	if (sFocused != NULL && sFocused->mVisible)
	{
		// Keyboard mode on desktop.
		sFocused->KeyUp(e);
	}

	return(NULL);
}

void CDesktop::OnTextEvent(SDL_TextInputEvent e)
{
	if (mVisible == false && mFace != NULL)
	{
		// Face mode
		std::string text = e.text;
		//printf("%s\n", e.text);
		CKeyCommand kc;

		kc = mKeyMap.GetKey(text);
		if (kc.mCommand == "mp4")
		{
			PlayVideo((char*)kc.mArgument1.c_str(), 0);
			if (kc.mArgument2 != "")
			{
				SetFace(kc.mArgument2);
			}
		}
		else if (kc.mCommand == "vg")
		{
			PlayVideoSync((char*)kc.mArgument1.c_str());
#ifndef WINDOWS
			system("/home/pi/bmos/scripts/emulaunch.sh -desktop &");
#endif
			//printf("SDL Quit()\n");

			SDL_DestroyRenderer(CApplication::sRenderer);
			SDL_DestroyWindow(CApplication::sWindow);

			SDL_Quit();

			exit(EXIT_SUCCESS);
		}
		else if (kc.mCommand == "face")
		{
			SetFace(kc.mArgument1);
		}
		else if (kc.mCommand == "system")
		{
#ifndef WINDOWS
			system(kc.mArgument1.c_str());
#endif
		}
	}
	else
	{
		if (sFocused != NULL && sFocused->mID != "0" && sFocused->IsVisible())
		{
			//sFocused->mSelected = true; XXX

			if (sFocused != this)
			{
				sFocused->OnTextEvent(e);
			}
		}
	}
}

void CDesktop::OnTextEditEvent(SDL_TextEditingEvent e)
{
	if (mVisible == false && mFace != NULL)
	{

		/*
		Update the composition text.
		Update the cursor position.
		Update the selection length (if any).
		*/
		//printf("%s %d %d \n", e.text, e.start, e.length);
	}

}

CWindow* CDesktop::OnKeyDown(SDL_KeyboardEvent e)
{
	CWindow* handled = NULL;

	if (wpid != 0)
	{
		if ((e.keysym.sym == SDLK_LSHIFT || e.keysym.sym == SDLK_RSHIFT || e.keysym.sym == SDLK_SPACE || e.keysym.sym == SDLK_ESCAPE || e.keysym.sym != SDLK_UP) && e.repeat == 0)
		{
			if (e.keysym.sym != SDLK_UP && mVisible == false && mFace != NULL)
			{
				// Face mode. DPad can kill videos.
			}
			else
			{
				system("/home/pi/bmos/scripts/dbuscontrol.sh stop");
			}
		}
		return(NULL);
	}

	if ((e.keysym.sym == SDLK_LCTRL || e.keysym.sym == SDLK_RCTRL) && e.repeat == 0)
	{
		if (mSettings.mVoiceRecognition == true)
		{
			mRecording = true;
			StartRecord();
			return(NULL);
		}
	}

	if (mVisible == false && mFace != NULL)
	{
		// Face mode
		if ((e.keysym.sym == SDLK_LALT || e.keysym.sym == SDLK_RALT) && e.repeat == 0)
		{
			//printf("Face Down\n");
			int mx, my;
			SDL_GetMouseState(&mx, &my);
			mKeyMouseEvent.type = SDL_MOUSEBUTTONDOWN;
			mKeyMouseEvent.button.button = SDL_BUTTON_LEFT;
			Uint32 delta = SDL_GetTicks() - mKeyMouseEvent.button.timestamp;

			if (mKeyMouseEvent.button.timestamp == UINT32_MAX)
			{
				mKeyMouseEvent.button.clicks = 1;
				mKeyMouseEvent.button.timestamp = SDL_GetTicks();
			}
			else if (delta < 150)
			{
				mKeyMouseEvent.button.clicks = 2;
				mKeyMouseEvent.button.timestamp = UINT32_MAX;
			}
			else
			{
				mKeyMouseEvent.button.clicks = 1;
				mKeyMouseEvent.button.timestamp = UINT32_MAX;
			}

			mKeyMouseEvent.button.x = mx;
			mKeyMouseEvent.button.y = my;

			//printf("%d, %d\n", delta, mKeyMouseEvent.button.clicks);

			if (mKeyMouseEvent.button.clicks == 2)
			{
				delete(mFace);
				mFace = NULL;
				mVisible = true;
			}
		}
		else if (e.keysym.sym >= SDLK_0 && e.keysym.sym <= SDLK_9)
		{
			int face = e.keysym.sym - SDLK_0;
			SetFace(face);
		}
		else if (e.keysym.sym == SDLK_COMMA || e.keysym.sym == SDLK_LEFT)
		{
			if (mSettings.mSlideShow)
			{
				mCurrentSlide--;
				mCurrentSlide = std::max(mCurrentSlide, 0);
				SetPicture(mCurrentSlide);
				mSlideTimer = UINT32_MAX;
			}
			else
			{
				mCurrentFace--;
				mCurrentFace = std::max(mCurrentFace, 0);
				SetFace(mCurrentFace);
			}
		}
		else if (e.keysym.sym == SDLK_TAB)
		{
			if (mSettings.mSlideShow)
			{
				mSlideTimer = SDL_GetTicks() + mSettings.mSlideInterval * 1000;
			}
		}
		else if (e.keysym.sym == SDLK_PERIOD || e.keysym.sym == SDLK_RIGHT)
		{
			if (mSettings.mSlideShow)
			{
				mCurrentSlide++;
				mCurrentSlide = std::min(mCurrentSlide, (int)mPictures.mFiles.size() - 1);
				SetPicture(mCurrentSlide);
				mSlideTimer = UINT32_MAX;
			}
			else
			{
				mCurrentFace++;
				mCurrentFace = std::min(mCurrentFace, (int)mFaces.mFiles.size() - 1);
				SetFace(mCurrentFace);
			}

		}
		else if (e.keysym.sym == SDLK_DOWN)
		{
			int r1 = std::rand();
			r1 = (r1 % mKeyMap.mRandomVids.size());
			PlayVideo((char*)mKeyMap.mRandomVids[r1].c_str(), 0);
		}
		else if (e.keysym.sym == SDLK_UP)
		{
			CKeyCommand kc;

			if (wpid != 0)
			{
				system("/home/pi/bmos/scripts/dbuscontrol.sh stop");
			}

			int r1 = std::rand();
			r1 = (r1 % mKeyMap.mKeyedVids.size());

			kc = mKeyMap.GetKey(mKeyMap.mKeyedVids[r1]);
			if (kc.mCommand == "mp4")
			{
				PlayVideo((char*)kc.mArgument1.c_str(), 0);
				if (kc.mArgument2 != "")
				{
					SetFace(kc.mArgument2);
				}
			}
		}
		else if (e.keysym.sym == SDLK_RETURN)
		{
		}
		return(NULL);
	}

	if (mDPadMouse)
	{
		if ((e.keysym.sym == SDLK_LALT || e.keysym.sym == SDLK_RALT) && e.repeat==0)
		{
			//printf("DPad Mouse Down\n");
			int mx, my;
			SDL_GetMouseState(&mx, &my);
			mKeyMouseEvent.type = SDL_MOUSEBUTTONDOWN;
			mKeyMouseEvent.button.button = SDL_BUTTON_LEFT;
			Uint32 delta = SDL_GetTicks() - mKeyMouseEvent.button.timestamp;
			
			if (mKeyMouseEvent.button.timestamp == UINT32_MAX)
			{
				mKeyMouseEvent.button.clicks = 1;
				mKeyMouseEvent.button.timestamp = SDL_GetTicks();
			}
			else if (delta < 150)
			{
				mKeyMouseEvent.button.clicks = 2;
				mKeyMouseEvent.button.timestamp = UINT32_MAX;
			}
			else
			{
				mKeyMouseEvent.button.clicks = 1;
				mKeyMouseEvent.button.timestamp = UINT32_MAX;
			}

			mKeyMouseEvent.button.x = mx;
			mKeyMouseEvent.button.y = my;

			//printf("%d, %d\n", delta, mKeyMouseEvent.button.clicks);

			SDL_PushEvent(&mKeyMouseEvent);
		}
		else if (e.keysym.sym == SDLK_RETURN)
		{
		}
		else if (sFocused != NULL && sFocused->mID != "0" && sFocused->IsVisible())
		{
			//sFocused->mSelected = true; XXX

			if (sFocused != this)
			{
				sFocused->KeyDown(e);
			}
		}
		return(NULL);
	}

	if (sFocused != NULL && sFocused->mID != "0" && sFocused->IsVisible())
	{
		//sFocused->mSelected = true; XXX

		if (sFocused != this)
		{
			sFocused->KeyDown(e);
		}
	}
	else
	{
		//IF nothing is focused, just focus on the main menu.
		int i;
		CMenuBarItem* mbi;
		for (i = 0; i < mMenuBar.mWindows.size(); i++)
		{
			mbi = dynamic_cast<CMenuBarItem*>(mMenuBar.mWindows[i]);
			if (mbi)
			{
				SetFocused(mbi);
				//mbi->mSelected = true; XXX
				mbi->KeyDown(e);
				break;
			}
		}
	}
	return(handled);
}

CWindow* CDesktop::OnMouseButton(SDL_MouseButtonEvent e)
{
	CWindow* win;

	if (mFace != NULL)
	{
		return(NULL);
	}

	if (mMouseOverWindow != NULL && mMouseOverWindow->mDrag)
	{
		mMouseOverWindow->Clicked(e);
		return(NULL);
	}
	else
	{
		win = CWindow::OnMouseButton(e);
	}
	
	if (win != NULL)
	{
		//printf("OnMouseButton %s %d\n", win->mID.c_str(), e.type);
	}
	else
	{
		//printf("OnMouseButton NULL %d\n", e.type);
	}

	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		CloseAll();
		SetFocused(win);
		if (win != NULL)
		{
			win->Clicked(e);
		}

	}
	else if (e.type == SDL_MOUSEBUTTONUP)
	{
		//SetFocused(win);
		if (win != NULL)
		{
			//win->Clicked(e);
		}
	}


	return(NULL);
}

void CDesktop::OnFolderClose(CForm* form)
{
	form->mDelete = true;
	sFocused = NULL;
}



void CDesktop::OnFolderOpen(CIcon* folder)
{
	using namespace std::placeholders;
	int r1, r2;



	CFolder* _folder;
	_folder = dynamic_cast<CFolder*>(folder);

	if (_folder && _folder->mForm==NULL)
	{
		int formx, formy;

		formx = _folder->X + 25;
		formy = _folder->Y + 25;

		CFolderForm* f = _folder->CreateForm(formx, formy, 250, 250);

		f->SetTitle("Files");
		f->AddOnCloseHandler(std::bind(&CDesktop::OnFolderClose, this, _1));
		f->AddItemRemovedHandler(std::bind(&CDesktop::OnFolderItemRemoved, this, _1, _2));
		_folder->AddItemDoubleClickedHandler(std::bind(&CDesktop::OnFolderItemDoubleClick, this, _1, _2));
		_folder->CreateFileList(true);


		AddWinForm(f);

		if (formx > 800 - f->Width())
		{
			f->X =800 - f->Width()-10;
		}

		if (formy > 600 - f->GetHeight())
		{
			f->Y = 600 - f->GetHeight()-10;
		}
		f->SetTitle(_folder->mName);
		f->Show();
	}
}

void CDesktop::OnDriveOpen(CIcon* folder)
{
	using namespace std::placeholders;
	int r1, r2;



	CFolder* _folder;
	_folder = dynamic_cast<CFolder*>(folder);

	if (_folder && _folder->mForm == NULL)
	{
		int formx, formy;

		formx = _folder->X + 25;
		formy = _folder->Y + 25;

		CFolderForm* f = _folder->CreateForm(formx, formy, 350, 400);

		f->SetTitle("USB Drive");
		f->AddOnCloseHandler(std::bind(&CDesktop::OnFolderClose, this, _1));
		f->AddItemRemovedHandler(std::bind(&CDesktop::OnFolderItemRemoved, this, _1, _2));
		_folder->AddItemDoubleClickedHandler(std::bind(&CDesktop::OnDiskDriveItemDoubleClick, this, _1, _2));

		std::string path;

#ifdef  WINDOWS
		path = "c:/home/media/usb/";
#else
		path = "/media/usb/";
#endif //  WINDOWS

		mDiskDrive->mPath = path;


		CFiles files;
		files.GetFiles(path, "*");

		std::vector<CFile> fileObjects;

		f->mFolderPath = path;

		for (int i = 0; i < files.mFiles.size(); i++)
		{
			try
			{
				std::string ext;
				ext = files.mFiles[i].substr(files.mFiles[i].size() - 4);

				std::string ext3;
				ext3 = files.mFiles[i].substr(files.mFiles[i].size() - 3);

				CFile::Type type = CFile::MapType(ext);
				if (ext == ".jpg")
				{
					CFile::Type type = CFile::Type::JPG;
					if (files.mFiles[i].size() >= 8)
					{
						if (files.mFiles[i].substr(0, 3) == "bmo")
						{
							type = CFile::Type::BMO;
						}
					}
					
					fileObjects.push_back(CFile(files.mFiles[i], " ", type));
				}
				else if (ext == ".mp4")
				{
					CFile::Type type = CFile::Type::BMO1;
					fileObjects.push_back(CFile(files.mFiles[i], " ", type));
				}
				else if (ext == ".txt")
				{
					CFile::Type type = CFile::Type::TXT;
					fileObjects.push_back(CFile(files.mFiles[i], " ", type));
				}
				else if (ext == "json")
				{
					CFile::Type type = CFile::Type::BIN;
					fileObjects.push_back(CFile(files.mFiles[i], " ", type));
				}
				else if (ext == ".bin")
				{
					CFile::Type type = CFile::Type::BIN;
					fileObjects.push_back(CFile(files.mFiles[i], " ", type));
				}
				else if (ext3 == ".sh")
				{
					CFile::Type type = CFile::Type::EXE;
					fileObjects.push_back(CFile(files.mFiles[i], " ", type));
				}
			}
			catch(...)
			{
			}
		}

		_folder->AddFiles(fileObjects);
		_folder->CreateFileList(true);


		AddWinForm(f);

		if (formx > 800 - f->Width())
		{
			f->X = 800 - f->Width() - 10;
		}

		if (formy > 600 - f->GetHeight())
		{
			f->Y = 600 - f->GetHeight() - 10;
		}
		f->SetTitle(_folder->mName);
		f->Show();
	}
}

void CDesktop::OnFolderItemDoubleClick(CFolderForm* form, CListBoxItem* lbi)
{
	//printf("CDesktop::Double click %s\n", lbi->mColumn1.c_str());
	char str[1000];
	sprintf(str, "BMO says you do not have\npermission to view\n%s\n \nAnd that's final!", lbi->mColumn1.c_str());

	mMessageBox->ShowMessage(str, "Access Denied", CMessageBox::MessageStyle::OK,CMessageBox::MessageAlign::Center);
}

void CDesktop::OnDiskDriveItemDoubleClick(CFolderForm* form, CListBoxItem* lbi)
{
	char str[1000];

	if (lbi == NULL)
	{
		return;
	}

	using namespace std::placeholders;

	std::string itemText = lbi->mColumn1;
	//printf("CDesktop::Double click %s\n", itemText.c_str());

	if (itemText == "bmo.txt")
	{
		sprintf(str, "Install %s?\n \nThis will replace the\ncurrent key mappings.", itemText.c_str());
		mMessageBox->ShowMessage(str, "Install", CMessageBox::MessageStyle::OK_Cancel, CMessageBox::MessageAlign::Center);
		mMessageBox->AddOnCloseHandler(std::bind(&CDesktop::OnConfirmInstallBmoConfig, this, _1, _2));
	}
	else if (itemText == "update.bin")
	{
		sprintf(str, "IMPORTANT!\n \nBefore selecting OK\nverify that you have the\ncorrect update file.");
		mMessageBox->ShowMessage(str, "Update BMOS?", CMessageBox::MessageStyle::OK_Cancel, CMessageBox::MessageAlign::Center);
		mMessageBox->AddOnCloseHandler(std::bind(&CDesktop::OnConfirmUpdateBMOS, this, _1, _2));
	}
	else if (itemText.find(".txt") != std::string::npos)
	{
		CTextReader* tr = new CTextReader(50, 50, 700, 450);
		tr->AddOnCloseHandler(std::bind(&CDesktop::OnFolderClose, this, _1));
		tr->SetTitle(itemText.c_str());
		AddWinForm(tr);
		tr->LoadFile(mDiskDrive->mPath + "/" + itemText.c_str());

		tr->Show();
	}
	else if (itemText == "install.bin")
	{
		sprintf(str, "This will copy all beemotes,\nfaces, and pictures\nfrom the USB drive.\nProceed?");
		mMessageBox->ShowMessage(str, "Copy Media", CMessageBox::MessageStyle::OK_Cancel, CMessageBox::MessageAlign::Center);
		mMessageBox->AddOnCloseHandler(std::bind(&CDesktop::OnConfirmInstallMedia, this, _1, _2));
	}
	else if (itemText == "credentials.json")
	{
		sprintf(str, "Install %s?", itemText.c_str());
		mMessageBox->ShowMessage(str, "Install", CMessageBox::MessageStyle::OK_Cancel, CMessageBox::MessageAlign::Center);
		mMessageBox->AddOnCloseHandler(std::bind(&CDesktop::OnConfirmInstallCredentials, this, _1, _2));
	}
	else if (itemText == "commands.conf")
	{
		sprintf(str, "Install %s?", itemText.c_str());
		mMessageBox->ShowMessage(str, "Install", CMessageBox::MessageStyle::OK_Cancel, CMessageBox::MessageAlign::Center);
		mMessageBox->AddOnCloseHandler(std::bind(&CDesktop::OnConfirmInstallVoice, this, _1, _2));
	}
	else if (itemText.find(".jpg") != std::string::npos)
	{
		OnPhotoItem(form, lbi);
	}
	else if (itemText.find(".mp4") != std::string::npos)
	{
		PlayVideoUSB((char*)lbi->mColumn1.c_str(), 0);
	}
	else if (itemText.find(".sh") != std::string::npos)
	{
		sprintf(str, "Run script %s?", itemText.c_str());
		mMessageBox->mTag = itemText;
		mMessageBox->ShowMessage(str, "Run", CMessageBox::MessageStyle::OK_Cancel, CMessageBox::MessageAlign::Center);
		mMessageBox->AddOnCloseHandler(std::bind(&CDesktop::OnConfirmRunScript, this, _1, _2));
	}
	else
	{
		sprintf(str, "BMO says you do not have\npermission to view\n%s\n \nAnd that's final!", itemText.c_str());
		mMessageBox->ShowMessage(str, "Access Denied", CMessageBox::MessageStyle::OK, CMessageBox::MessageAlign::Center);
	}
}

void CDesktop::OnFolderItemRemoved(CForm* form, CListBoxItem* item)
{
	if (item != NULL)
	{
		SDL_Refresh();
		CFolderForm *folderForm = dynamic_cast<CFolderForm*>(form);

		if (folderForm)
		{
			if (folderForm->mFolderPath == "NONE" || folderForm->mFolderPath == "")
			{
				if (item->mColumn1 == "CORE SYSTEM DRIVERS.sys")
				{
					PlayVideo((char*)"coresys.mp4", 0);
					mOSStatus = false;
				}
				else
				{
					PlayVideo((char*)"ah.mp4", 0);
				}
			}
			else if (strstr(folderForm->mFolderPath.c_str(), "bmos/") != NULL)
			{
				std::string fpath = folderForm->mFolderPath + "/" + item->mColumn1;
				//printf("Delete %s\n", fpath.c_str());
				CFiles::Delete(fpath);
			}
		}
	}
}

void CDesktop::WindowDelete(CWindow* win)
{
	if (sFocused == win)
	{
		sFocused = NULL;
	}

	if (mMouseOverWindow == win)
	{
		mMouseOverWindow = NULL;
	}

}

void CDesktop::AddSurfWindow(CSurfWindow* win)
{
	mSurfWindows.push_back(win);
}

void CDesktop::Draw()
{
	if (mFace != NULL)
	{
		int w, h;
		SDL_GetRendererOutputSize(CApplication::sRenderer, &w, &h);
		mFace->DrawToFit(w, h);

		return;
	}
	if (mVisible == false)
	{
		return;
	}

	CWindow::Draw();
	mCursor->Draw();

	//std::vector<CSurfWindow*>::iterator swindow;

	//swindow = mSurfWindows.begin();
	//for (; swindow != mSurfWindows.end(); swindow++)
	//{
	//	(*swindow)->Draw(NULL);
	//	(*swindow)->DrawTexture(CApplication::sRenderer);
	//}

}

void CDesktop::shutdown()
{
	SetFace("blank.jpeg");
	SDL_Refresh();
	PlayVideoSync((char*)"wet.mp4");
	printf("Shutdown OK\n");
#ifndef WINDOWS
	system("sudo halt");
#endif
	exit(EXIT_SUCCESS);
}

void CDesktop::reboot()
{
	printf("Reboot OK\n");
	SetFace("blank.jpeg");
	SDL_Refresh();
	PlayVideoSync((char*)"power.mp4");
#ifndef WINDOWS
	system("sudo reboot");
#endif

	exit(EXIT_SUCCESS);
}

void CDesktop::OnConfirmShutdown(CMessageBox* box, CMessageBox::MessageResult result)
{
	if (result == CMessageBox::MessageResult::OK)
	{
		shutdown();
	}
	else if (result == CMessageBox::MessageResult::Cancel)
	{
		printf("Shutdown Cancel\n");
	}
}

void CDesktop::OnConfirmReboot(CMessageBox* box, CMessageBox::MessageResult result)
{
	if (result == CMessageBox::MessageResult::OK)
	{
		reboot();
	}
	else if (result == CMessageBox::MessageResult::Cancel)
	{
		printf("Reboot Cancel\n");
	}
}

void CDesktop::OnConfirmInstallBmoConfig(CMessageBox* box, CMessageBox::MessageResult result)
{
	if (result == CMessageBox::MessageResult::OK)
	{
		//printf("OnConfirmInstallBmoConfig OK\n");
		std::string src = mDiskDrive->mPath + "/bmo.txt";
		std::string dst = CApplication::sBMOS_Root + "/bmo.txt";

		CFiles::Copy(src, dst);
		PlayVideo((char*)"ah.mp4", 0);
		try
		{
			bool b = mKeyMap.LoadFile(CApplication::sBMOS_Root + "bmo.txt");
		}
		catch (...)
		{
		}
	}
	else if (result == CMessageBox::MessageResult::Cancel)
	{
		//printf("OnConfirmInstallBmoConfig Cancel\n");
	}
}

void CDesktop::OnConfirmUpdateBMOS(CMessageBox* box, CMessageBox::MessageResult result)
{
	if (result == CMessageBox::MessageResult::OK)
	{
		//printf("OnConfirmUpdateBMOS OK\n");
		std::string update = CApplication::sBMOS_Root + "/scripts/update.sh " + mDiskDrive->mPath + "/update.bin";
#ifndef WINDOWS
		system(update.c_str());
#endif

		//PlayVideo((char*)"ah.mp4", 0);
		SDL_DestroyRenderer(CApplication::sRenderer);
		SDL_DestroyWindow(CApplication::sWindow);

		SDL_Quit();


		exit(EXIT_SUCCESS);
	}
	else if (result == CMessageBox::MessageResult::Cancel)
	{
		//printf("OnConfirmUpdateBMOS Cancel\n");
	}
}

void CDesktop::OnConfirmRunScript(CMessageBox* box, CMessageBox::MessageResult result)
{
	if (result == CMessageBox::MessageResult::OK)
	{
		std::string cmd = "sudo cp " + mDiskDrive->mPath + "/" + mMessageBox->mTag + " /home/pi/script.sh";
#ifndef WINDOWS
		system(cmd.c_str());
#endif

		cmd = "sudo /home/pi/script.sh";
#ifndef WINDOWS
		system(cmd.c_str());
#endif

		cmd = "sudo rm /home/pi/script.sh";
#ifndef WINDOWS
		system(cmd.c_str());
#endif

	}
	else if (result == CMessageBox::MessageResult::Cancel)
	{

	}
}

void CDesktop::OnConfirmInstallVoice(CMessageBox* box, CMessageBox::MessageResult result)
{
	if (result == CMessageBox::MessageResult::OK)
	{
		//printf("OnConfirmInstallVoice OK\n");
		std::string src = mDiskDrive->mPath + "/commands.conf";
		std::string dst = CApplication::sBMOS_Root + "/commands.conf";

		CFiles::Copy(src, dst);
		PlayVideo((char*)"ah.mp4", 0);
		try
		{
			CVoiceCommand vc;

			mVoiceCommands.GetConfig(CApplication::sBMOS_Root + "commands.conf");
		}
		catch (...)
		{
			printf("Error reading voice commands.\n");
		}
	}
	else if (result == CMessageBox::MessageResult::Cancel)
	{
		//printf("OnConfirmInstallVoice Cancel\n");
	}
}

void CDesktop::OnConfirmInstallCredentials(CMessageBox* box, CMessageBox::MessageResult result)
{
	if (result == CMessageBox::MessageResult::OK)
	{
		//printf("OnConfirmInstallCredentials OK\n");
		std::string src = mDiskDrive->mPath + "/credentials.json";
		std::string dst = CApplication::sBMOS_Root + "/credentials.json";

		CFiles::Copy(src, dst);
		PlayVideo((char*)"ah.mp4", 0);
	}
	else if (result == CMessageBox::MessageResult::Cancel)
	{
		//printf("OnConfirmInstallCredentials Cancel\n");
	}
}

void CDesktop::OnConfirmInstallMedia(CMessageBox* box, CMessageBox::MessageResult result)
{
	if (result == CMessageBox::MessageResult::OK)
	{
		int i;
		std::string src;
		std::string dst;

		CFiles *files = new CFiles();
		files->GetFiles(mDiskDrive->mPath, "jpg");
		for (i = 0; i < files->mFiles.size(); i++)
		{
			std::string src = mDiskDrive->mPath + "/" + files->mFiles[i];
			if (files->mFiles[i].find("bmo") == 0)
			{
				dst = CApplication::sBMOS_Root + "/faces/" + files->mFiles[i];
			}
			else
			{
				dst = CApplication::sBMOS_Root + "/pictures/" + files->mFiles[i];
			}

			CFiles::Copy(src, dst);
		}
		

		files->GetFiles(mDiskDrive->mPath, "mp4");
		for (i = 0; i < files->mFiles.size(); i++)
		{
			src = mDiskDrive->mPath + "/" + files->mFiles[i];
			dst = CApplication::sBMOS_Root + "/videos/" + files->mFiles[i];

			CFiles::Copy(src, dst);
		}

		//printf("OnConfirmInstallMedia OK\n");

		PlayVideo((char*)"ah.mp4", 0);
		delete(files);

	}
	else if (result == CMessageBox::MessageResult::Cancel)
	{
		//printf("OnConfirmInstallMedia Cancel\n");
	}
}

void CDesktop::PlayVideoSync(char* filename)
{
	char s[1000];
#ifdef WINDOWS
	//printf("PlayVideo(%s)\n", filename);
#else
	sprintf(s, "omxplayer --aspect-mode fill --layer 10010 -o alsa --no-keys --no-osd /home/pi/bmos/videos/%s > /dev/null", filename);
	system(s);
#endif

}

void SDL_Refresh()
{

	if (CDesktop::sDesktop != NULL)
	{
		CDesktop::sDesktop->Update();

		if (CDesktop::sBuffer != NULL)
		{
			CDesktop::sBuffer->SetAsRenderTarget();
			//SDL_SetTextureBlendMode(gBuffer.mTexture, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(CApplication::sRenderer, 193, 243, 213, 255);
			SDL_RenderClear(CApplication::sRenderer);

			CDesktop::sDesktop->Draw();

			//Reset render target
			SDL_SetRenderTarget(CApplication::sRenderer, NULL);

			CDesktop::sBuffer->DrawScaled(720, 480);
		}


		SDL_RenderPresent(CApplication::sRenderer);
	}
}



void CDesktop::PlayVideo(char* filename, int face)
{
	char vide[1024];
	sprintf(vide, "/home/pi/bmos/videos/%s", filename);
	char c2;

	if (wpid != 0)
	{
		printf("video playing...");
		return;
	}

	char* argv[] = { (char*)"omxplayer", 
		(char*)"--aspect-mode",
		(char*)"fill",
		(char*)"--layer",
		(char*)"10010",
		(char*)"-o",
		(char*)"alsa", 
		(char*)"--no-keys", 
		(char*)"--no-osd", (char*)
		vide, NULL };

#ifdef WINDOWS
	//printf("PlayVideo(%s)\n", filename);
#else



	//printf("%s\n", vide);

	pid_t pid = fork();

	if (pid == -1) {
		// When fork() returns -1, an error happened.
		perror("fork failed");
		//exit(EXIT_FAILURE);
		return;
	}
	else if (pid == 0) {
		int fd = open("/dev/null", O_RDWR, S_IRUSR | S_IWUSR);

		dup2(fd, 1);   // make stdout go to file
		execvp("omxplayer", argv);
		close(fd);
	}
	else {
		// When fork() returns a positive number, we are in the parent process
		// and the return value is the PID of the newly created child process.
		wpid = pid;
		//printf("wpid=%d\n", wpid);
		//int status;
		//
		usleep(2000000);
		//SetFace("bmo99.jpg");
		////imagetable(w, h, face);
		//int wpid = 0;
		//while (wpid == 0)
		//{
		//	wpid = waitpid(pid, &status, WNOHANG);
		//	if (wpid == 0)
		//	{
		//		//c2 = getch();
		//		//if (c2 == ' ')
		//		//{
		//		//	system("./dbuscontrol.sh stop");
		//		//}
		//	}

		//	usleep(10000);
		//	//SDL_Refresh();
		//}
	}
#endif
}

void CDesktop::PlayVideoUSB(char* filename, int face)
{
	char vide[1024];
	sprintf(vide, "/media/usb/%s", filename);
	char c2;

	char* argv[] = { (char*)"omxplayer",
		(char*)"--layer",
		(char*)"10010",
		(char*)"-o",
		(char*)"alsa",
		(char*)"--no-keys",
		(char*)"--no-osd", (char*)
		vide, NULL };

#ifdef WINDOWS
	//printf("PlayVideo(%s)\n", filename);
#else
	pid_t pid = fork();

	if (pid == -1) {
		// When fork() returns -1, an error happened.
		perror("fork failed");
		//exit(EXIT_FAILURE);
		return;
	}
	else if (pid == 0) {
		execvp("omxplayer", argv);
	}
	else {
		wpid = pid;
		usleep(2000000);
	}
#endif
}

void CDesktop::SetMousePosition(int x, int y)
{
	CWindow::sMouseMotionHandled = NULL;

	OnMouseMotion(x, y);
	LayoutWindows();
}

void CDesktop::SetPicture(int pic)
{
	char str[1000];
	if (mFace == NULL)
	{
		mFace = new CTexture();
	}

	sprintf(str, "%s/pictures/%s", CApplication::sBMOS_Root.c_str(), mPictures.mFiles[pic].c_str());

	std::string path = str;

	mFace->LoadFromFile(path);
}


void CDesktop::SetFace(int face)
{
	char str[1000];
	if (mFace == NULL)
	{
		mFace = new CTexture();
	}

	sprintf(str, "%s/faces/%s", CApplication::sBMOS_Root.c_str(), mFaces.mFiles[face].c_str());

	std::string path = str;

	mFace->LoadFromFile(path);
}

void CDesktop::SetFace(std::string face)
{
	char str[1000];
	if (mFace == NULL)
	{
		mFace = new CTexture();
	}

	sprintf(str, "%s/faces/%s", CApplication::sBMOS_Root.c_str(), face.c_str());

	std::string path = str;

	mFace->LoadFromFile(path);
}

void CDesktop::SetFace(std::string folder, std::string face)
{
	char str[1000];
	if (mFace == NULL)
	{
		mFace = new CTexture();
	}

#ifdef WINDOWS
	sprintf(str, "%s/%s", folder.c_str(), face.c_str());
#else
	sprintf(str, "%s/%s", folder.c_str(), face.c_str());
#endif

	std::string path = str;

	mFace->LoadFromFile(path);
}

void CDesktop::StartRecord()
{
	//printf("StartRecord()\n");
#ifndef WINDOWS
	if (mRecordPid != 0)
	{
		//printf("mRecordPid %d\n", mRecordPid);
		return;
	}

	SetFace("bmo17.jpg");
	//printf("Record\n");

	mRecordPid = fork();

	if (mRecordPid == -1)
	{
		// When fork() returns -1, an error happened.
		perror("fork failed");
		//exit(EXIT_FAILURE);
		return;
	}
	else if (mRecordPid == 0)
	{
		//printf("Start arecord\n");
		char* argv[] = {(char*)"arecord", 
			(char*)"-d",
			(char*)"5",
			(char*)"-D",
			(char*)"pcm.micboost",
			(char*)"-c2",
			(char*)"-r",
			(char*)"48000",
			(char*)"-f",
			(char*)"S32_LE",
			(char*)"-t",
			(char*)"wav",
			(char*)"-V",
			(char*)"mono",
			(char*)"-v",
			(char *)"out.wav",
			NULL };


	//	char* argv[] = { (char*)"arecord",
	//(char*)"-d 5",
	//(char*)"--format=S16_LE",
	//(char*)"--rate=16000",
	//(char*)"--file-type=raw",
	//(char*)"out.raw", NULL };

		execvp("arecord", argv);
		perror("execvp of arecord  failed");
		exit(1);
	}

	//printf("Continue %d\n", mRecordPid);
#else
	//printf("Start recording.\n");
#endif

}

void CDesktop::EndRecord()
{
	int status, ret;
	char message[200];
	message[0] = '\0';

#ifndef WINDOWS
	if (mRecordPid <= 0)
	{
		//printf("No recording: %d\n", mRecordPid);
		return;
	}

	//printf("Stop recording %d\n", mRecordPid);

	ret = 0;
	while (ret == 0)
	{
		kill(mRecordPid, SIGINT);
		//printf("Kill SIGINT %d\n", mRecordPid);
		ret = waitpid(mRecordPid, &status, WNOHANG);

			if (ret == -1)
			{
				printf("parent:error\n");
				break;
			}
	}

	if (ret == mRecordPid)
		printf("Parent: Child process waited for.\n");
	else
		printf("Parent: returnPID= %d\n", ret);

	mRecordPid = 0;
#else
	CVoiceCommand vc;
	vc = mVoiceCommands.ProcessMessage("");

	printf("End recording.\n");
#endif

	ProcessGoogleVoice();
}

void CDesktop::ProcessGoogleVoice()
{
	CVoiceCommand vc;

	char message[1000];
	FILE* cmd = NULL;
	printf("Found audio\n");
#ifndef WINDOWS
	string command = "/home/pi/bmos/scripts/google-voice.sh";

	cmd = popen(command.c_str(), "r");
	fscanf(cmd, "\"%[^\"\n]\"", message);
	printf("Cmd: %s\n", command.c_str());
	printf("Mess: %s\n", message);

	std::string msg = message;

	vc = mVoiceCommands.ProcessMessage(message);
	printf("vc.mCommand=%s\n", vc.mCommand.c_str());
	printf("vc.mArgument=%s\n", vc.mArgument.c_str());
	fclose(cmd);

	if (msg == "confirm")
	{
		if (mVoiceShutdown == true)
		{
			shutdown();
		}
		else if (mVoiceReboot == true)
		{
			reboot();
		}
	}
	else if (vc.mCommand == "")
	{
		printf("vc.mCommand not found\n");
		PlayVideo((char*)"hmm.mp4", 0);
	}
	else if (vc.mCommand == "shutdown")
	{
		if (vc.mArgument == "noconfirm")
		{
			shutdown();
		}
		else
		{
			mVoiceShutdown = true;
			SetFace("shutdown.jpeg");
			return;
		}
	}
	else if (vc.mCommand == "reboot")
	{
		if (vc.mArgument == "noconfirm")
		{
			reboot();
		}
		else
		{
			mVoiceReboot = true;
			SetFace("reboot.jpeg");
			return;
		}
	}
	else if (vc.mCommand == "system")
	{
		if (vc.mArgument.find("parade.sh") != std::string::npos)
		{
			system((vc.mArgument).c_str());
		}
		else
		{
			system((vc.mArgument + "&").c_str());
		}
	}
	else if (vc.mCommand == "mp4")
	{
		PlayVideo((char*)vc.mArgument.c_str(), 0);
	}
	else if (vc.mCommand == "launch")
	{
		if (vc.mArgument == "photos")
		{
			OnMenuPhotos(NULL);
		}
		else if (vc.mArgument == "about")
		{
			OnMenuAbout(NULL);
		}
		else if (vc.mArgument == "videogames")
		{
			PlayVideoSync((char*)"videogames.mp4");
#ifndef WINDOWS
			system("/home/pi/bmos/scripts/emulaunch.sh -desktop &");
#endif
			SDL_DestroyRenderer(CApplication::sRenderer);
			SDL_DestroyWindow(CApplication::sWindow);

			SDL_Quit();


			exit(EXIT_SUCCESS);
		}
	}
#endif
	mVoiceShutdown = false;
	mVoiceReboot = false;
}

