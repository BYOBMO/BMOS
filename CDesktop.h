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

#pragma once
#ifdef WINDOWS
#define _HAS_STD_BYTE 0
#endif

#include "CMenuBar.h"
#include "CWindow.h"
#include "CSurfWindow.h"
#include "CIcon.h"
#include "CForm.h"

#include "CJoystick.h"
#include "CDropDownMenu.h"
#include "CDragWindow.h"
#include "CSettingsForm.h"
#include "CAudioForm.h"
#include "CWiFi.h"
#include "CFolderForm.h"
#include "CMessageBox.h"
#include "CListBoxItem.h"
#include "CFile.h"
#include "CFolder.h"
#include "CApplication.h"
#include "CFiles.h"
#include <SDL2/SDL.h>
#include "CVoiceProcess.h"
#include "CKeyMap.h"
#include "CSettings.h"
#include "CPhoto.h"
#include <vector>

using namespace std;

class CDesktop: public CWindow
{
public:
	static CWindow* sFocused;
	static CTexture* sBuffer;
	static CDesktop* sDesktop;

	static CDropDownMenu* CreateMenu();
	static void RemoveMenu(CDropDownMenu* menu);
	static void SetFocused(CWindow* window);

	bool mOSStatus;
	Uint32 mBlueButtonTimeout;
	Uint32 mFaceTimer;
	Uint32 mRandomVideoTimer;
	Uint32 mSlideTimer;
	bool mDPadMouse;
	SDL_Event mKeyMouseEvent;
	CVoiceProcess mVoiceCommands;
	CKeyMap mKeyMap;
	CTexture* mCursor;
	CTexture* mFace;
	CFiles mFaces;
	CFiles mPictures;
	int mCurrentSlide;
	int mCurrentFace;
	int mCurrentVideo;

	bool mVoiceShutdown;
	bool mVoiceReboot;

	bool mRecording;
	CSettings mSettings;
	CJoystick mJoystick;
	int mConfigureJoystick;

	CWindow* mMouseOverWindow;
	std::vector<CSurfWindow*> mSurfWindows;
	std::vector<CFile>mFiles1;
	std::vector<CFile>mFiles2;
	std::vector<CFile>mFiles3;
	std::vector<CForm*> mForms;
	std::vector<CIcon*> mIcons;
	std::vector<CDropDownMenu*> mMenus;
	CDragWindow* mDragWindow;
	CMessageBox* mMessageBox;
	int mRecordPid;
	CSettingsForm* mSettingsForm;
	CAudioForm* mAudioForm;
	CWiFi* mWiFiForm;
	CFolder* mFAQ;
	CMenuBar mMenuBar;
	CMenuBarItem* mWindowsMenu;
	CIcon* mTrash;
	CFolder* mDiskDrive;
	CFolder* mPhotos;
	CFolder* mBeemotes;
	CLabel* mLabel;

	CDesktop();
	CDesktop(int w, int h);
	virtual ~CDesktop();

	void LoadSettings();
	void Init(bool desktop);

	void BringFormToFront(CForm*);
	void BringIconToFront(CIcon*);

	void SetDragWindow(CDragWindow* win);
	void OnDragDrop(CDragWindow* win);

	void SetFace(int face);
	void SetFace(std::string face);
	void SetFace(std::string folder, std::string face);
	void SetPicture(int pic);

	void AddWinForm(CForm* form);
	void AddIcon(CIcon* icon);
	void AddMenu(CDropDownMenu* menu);
	void DeleteMenu(CDropDownMenu* menu);

	void LayoutWindows();

	void OnTrash(int i);
	void OnFile(CMenuBarItem* item);
	void OnEdit(CMenuBarItem* item);
	void OnWindows(CMenuBarItem* item);
	void OnHelp(CMenuBarItem* item);
	void OnView(CMenuBarItem* item);

	void OnFileMenu(CMenuBarItem* item);
	void OnEditMenu(CMenuBarItem* item);
	void OnViewMenu(CMenuBarItem* item);
	void OnWindowMenu(CMenuBarItem* item);
	void OnHelpMenu(CMenuBarItem* item);

	void OnMenuOpen(CMenuBarItem* item);
	void OnMenuSave(CMenuBarItem* item);
	void OnMenuExit(CMenuBarItem* item);
	void OnMenuSettings(CMenuBarItem* item);
	void OnMenuAudio(CMenuBarItem* item);
	void OnMenuVoices(CMenuBarItem* item);
	void OnMenuPhotos(CMenuBarItem* item);
	void OnMenuVideoGames(CMenuBarItem* item);	
	void OnMenuParade(CMenuBarItem* item);
	void OnMenuHiScores(CMenuBarItem* item);
	void OnMenuLoScores(CMenuBarItem* item);
	void OnMenuAbout(CMenuBarItem* item);
	void OnMenuRepair(CMenuBarItem* item);
	void OnMenuFAQ(CMenuBarItem* item);
	void OnFAQItemDoubleClick(CFolderForm* form, CListBoxItem* lbi);

	void OnSettingsOK(CSettingsForm* form);
	void OnAudioOK(CAudioForm* form);

	void OnIconSelected(CIcon* icon);
	void OnFolderOpen(CIcon* folder);
	void OnDriveOpen(CIcon* folder);

	void OnConfirmShutdown(CMessageBox* box, CMessageBox::MessageResult result);
	void OnConfirmReboot(CMessageBox* box, CMessageBox::MessageResult result);

	void OnDiskDriveItemDoubleClick(CFolderForm* form, CListBoxItem* lbi);
	void ShowTextFile(std::string title, std::string file);
	void OnConfirmInstallBmoConfig(CMessageBox* box, CMessageBox::MessageResult result);
	void OnConfirmRunScript(CMessageBox* box, CMessageBox::MessageResult result);
	void OnConfirmUpdateBMOS(CMessageBox* box, CMessageBox::MessageResult result);
	void OnConfirmInstallCredentials(CMessageBox* box, CMessageBox::MessageResult result);
	void OnConfirmInstallVoice(CMessageBox* box, CMessageBox::MessageResult result);
	void OnConfirmInstallMedia(CMessageBox* box, CMessageBox::MessageResult result);

	void OnFolderClose(CForm* form);
	void OnFolderItemRemoved(CForm* form, CListBoxItem* item);
	void OnFolderItemDoubleClick(CFolderForm* form, CListBoxItem* lbi);

	void OnVoiceClose(CForm* form);
	void OnVoiceItem(CFolderForm* form, CListBoxItem* item);

	void OnPhotoItem(CFolderForm *form, CListBoxItem* item);

	void Update();

	void SetMousePosition(int x, int y);
	virtual CWindow* OnKeyDown(SDL_KeyboardEvent e);
	virtual CWindow* OnKeyUp(SDL_KeyboardEvent e);
	virtual void OnTextEvent(SDL_TextInputEvent e);
	void OnTextEditEvent(SDL_TextEditingEvent e);
	virtual CWindow* OnMouseMotion(int x, int y);
	virtual CWindow* OnMouseButton(SDL_MouseButtonEvent e);
	void GetJoystickAxis(bool* left, bool* right, bool* up, bool* down);

	void StartRecord();
	void EndRecord();
	void ProcessGoogleVoice();


	void CloseAll();
	void WindowDelete(CWindow* win);

	void AddSurfWindow(CSurfWindow* win);
	virtual void Draw();

	void PlayVideo(char* filename, int face);
	void PlayVideoUSB(char* filename, int face);
	void PlayVideoSync(char* filename);

	void StartConfigureJoystick();
	void EndConfigureJoystick();
	void ConfigureJoystick(Uint32 type, Uint8 val, int val2);
	void ShowJoystick();

	void shutdown();
	void reboot();
};

