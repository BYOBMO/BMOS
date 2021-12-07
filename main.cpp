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

#ifdef WINDOWS
#define _HAS_STD_BYTE 0
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "CText.h"
#include "CTexture.h"
#include <string.h>
#include <chrono>
#include <ctime>  
#include "CWindow.h"
#include "CDesktop.h"
#include "CForm.h"
#include "CFile.h"

#ifndef WINDOWS
#include <unistd.h>
#include <sys/wait.h>
#endif

#ifdef WINDOWS
#include <windows.h>
#endif

#include <stdio.h>

#include <sys/stat.h>
#include <string>

#include "Utils.h"

using namespace std;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
CTexture *gFace;
//The window renderer
SDL_Renderer* gRenderer = NULL;
TTF_Font *gFont = NULL;
CText mText;
int lastPos = 0;
int rpos = 0;
unsigned mcolor = 0x00FF00;
unsigned pcolor = 0xFFFF00;
CTexture *gBuffer;
int dispW, dispH;
//CTexture *gCursor;
CDesktop *gDesktop;
SDL_Joystick *gGameController;

enum GameMode
{
	Desktop,
	Face
};

class CPage
{
public:
	CPage() { 
		numLines = 0; 
		halign = false;
		valign = false;
		highlight = 0;
		duration = 3;
	}

	void AddLine(std::string newLine) {
		if (numLines < 20) { 
			lines[numLines] = newLine; 
			numLines++; 
		}
	}

	void AddLine(const char *newLine)
	{
		std::string newStr(newLine);

		AddLine(newStr);
	}


	void SetLine(int i, char* str)
	{
		if (i < numLines)
		{
			lines[i] = str;
		}
	}

	void SetLine(int i, std::string str)
	{
		if (i < numLines)
		{
			lines[i] = str;
		}
	}

	std::string lines[20];
	int numLines;
	bool halign;
	bool valign;
	int highlight;
	int duration;
};



int exists(const char *name)
{
  struct stat   buffer;
  return (stat (name, &buffer) == 0);
}

unsigned int getWindowFlags()
{
	return SDL_WINDOW_OPENGL;

} // getWindowFlags

void setupWindow()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

} // setupWindow

bool init()
{
	//Initialization flag
	bool success = true;

	char* base_path = SDL_GetBasePath();

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Load joystick
		gGameController = SDL_JoystickOpen(0);
		if (gGameController == NULL)
		{
			//printf("Unable to open game controller. SDL Error: %s\n", SDL_GetError());
		}

		CApplication::sGameController = gGameController;

		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled.");
		}

		setupWindow();

		//Create window

#ifdef WINDOWS
		unsigned int windowFlags = getWindowFlags();
		gWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720, 480, windowFlags);
#else
		gWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720, 480, SDL_WINDOW_FULLSCREEN_DESKTOP);
#endif
		if (gWindow == NULL)
		{
			printf("Window could not be created. SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			CApplication::sWindow = gWindow;
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				CApplication::sRenderer = gRenderer;

				SDL_RendererInfo info;
				SDL_GetRendererInfo(gRenderer, &info);


				//printf("%s:%d:%d\n", info.name, info.max_texture_width, info.max_texture_height);

				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				//SDL_RenderSetLogicalSize(gRenderer, 640, 480);


				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize. SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize. SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
				else
				{

					std::string fontname = "fonts/8bitoperator/8bitoperator.ttf";
					//std::string fontname = "fonts/eight.ttf";



					gFont = TTF_OpenFont(fontname.c_str(), 48);



					if (gFont == NULL)
					{
						printf("Failed to load font. SDL_ttf Error: %s\n", TTF_GetError());
					}

					CText::sFontSmall = TTF_OpenFont(fontname.c_str(), 34);


					if (CText::sFontSmall == NULL)
					{
						printf("Failed to load sFont36 SDL_ttf Error: %s\n", TTF_GetError());
					}

					CText::sFontLarge = TTF_OpenFont(fontname.c_str(), 44);

					if (CText::sFontLarge == NULL)
					{
						printf("Failed to load sFont36 SDL_ttf Error: %s\n", TTF_GetError());
					}
				}
			}

		}

		SDL_ShowCursor(0);
	}

	return success;
}


int CreatePage(std::string str, int start, CPage &page)
{
	int pos;
	int row = 0;
	pos = 0;
	std::string teststr = "";
	char c;
	int textW, textH;
	bool done = false;
	const char *s;
	unsigned int i;

	int space = 0;
	pos = start;
	int index = -1;

	while (!done && row<9)
	{
		teststr = "";
		space = pos;

		for (i = pos; i < str.length(); i++)
		{
			done = true;
			c = str[i];
			if (c == ' ')
			{
				space = i;
			}
			else if (c == '\\')
			{
				index = i + 1;
				done = true;
				break;
			}

			teststr += str[i];
			s = teststr.c_str();
			TTF_SizeText(gFont, s, &textW, &textH);
			if (textW > 600)
			{
				if (space == pos)
				{
					teststr = str.substr(pos, i-pos);
					pos = i;
				}
				else
				{
					teststr = str.substr(pos, space - pos);
					pos = space + 1;
				}
			
				index = pos;
				done = false;
				break;
			}
			index = i+1;
		}

		page.AddLine(teststr);
		row++;
	}
	
	if (index >= (int)str.length())
	{
		index = -1;
	}

	return(index);
}

bool DrawPage(SDL_Color color, CPage &page)
{
	int row = 0;
	int textW, textH;
	const char *c;
	int x, y;
	int yHeight=0;
	int yOffset = 0;


	if (page.valign)
	{
		// Center page vertically.
		for (row = 0; row < page.numLines; row++)
		{
			c = page.lines[row].c_str();
			TTF_SizeText(gFont, c, &textW, &textH);

			yHeight += (textH + 2);
		}
		
		yOffset = (590 - yHeight) / 2;
	}

	if (yOffset < 0)
	{
		yOffset = 0;
	}

	for (row = 0; row < page.numLines; row++)
	{
		c = page.lines[row].c_str();
		TTF_SizeText(gFont, c, &textW, &textH);

		x = 0;
		if (page.halign)
		{
			// Center text horizontally.
			x = (800 - textW) / 2;
		}

		y = yOffset + (5 + row * (textH + 2));
		mText.SetText(gFont, c, color);
		mText.SetPosition(x, y);


		mText.Draw();

		if (page.highlight>0 && row==0)
		{
			SDL_Color textYellow = { RChannel(pcolor), GChannel(pcolor), BChannel(pcolor) };

			std::string s = page.lines[row].substr(0, page.highlight);

			c = s.c_str();
			y = yOffset + (5 + row * (textH + 2));
			mText.SetText(gFont, c, textYellow);
			mText.SetPosition(x, y);

			mText.Draw();
		}
	}


	return(true);
}

bool createWindow()
{
	//printf("createWindow\n");
	if (!init())
	{
		printf("Failed to initialize.\n");
		return(false);
	}
	else
	{
		gBuffer = new CTexture();
		gBuffer->CreateBlank(800, 600, SDL_TEXTUREACCESS_TARGET);
		CDesktop::sBuffer = gBuffer;
	}
	//printf("end createWindow\n");
	return(true);
}

void destroyWindow()
{
	printf("destroyWindow\n");
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	delete(gBuffer);
	gBuffer = NULL;

	//SDL_ShowCursor(initialCursorState);
	SDL_Quit();

	gRenderer = NULL;

} // destroyWindow

void handleDesktop(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_TEXTINPUT:
		/* Add new text onto the end of our text */
		gDesktop->OnTextEvent(e.text);
		break;
	case SDL_TEXTEDITING:
		gDesktop->OnTextEditEvent(e.edit);
		break;
	}
	

	

	if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
	{
		e.button.x = (int)(e.button.x * (800.0 / (float)dispW));
		e.button.y = (int)(e.button.y * (600.0 / (float)dispH));

		CWindow::sMouseClickHandled = NULL;
		gDesktop->OnMouseButton(e.button);
		gDesktop->LayoutWindows();
	}
	else if (e.type == SDL_KEYDOWN)
	{
		if (e.key.keysym.sym == SDLK_F4)
		{
			const Uint8* state = SDL_GetKeyboardState(NULL);
			if (state[SDL_SCANCODE_LSHIFT] == true)
			{
				SDL_Event ee;
				ee.type = SDL_QUIT;
				SDL_PushEvent(&ee);
			}
		}
		CWindow::sMouseClickHandled = NULL;
		gDesktop->OnKeyDown(e.key);
		gDesktop->LayoutWindows();
	}
	else if (e.type == SDL_KEYUP)
	{
		gDesktop->OnKeyUp(e.key);
		gDesktop->LayoutWindows();
	}
	else if (e.type == SDL_MOUSEMOTION)
	{
		int mouseX = e.motion.x;
		int mouseY = e.motion.y;
		//printf("mx:%d, my%d\n", mouseX, mouseY);

		if (mouseX > dispW)
		{
			mouseX = dispW;
			SDL_WarpMouseInWindow(gWindow, mouseX, mouseY);
		}
		else if (mouseY > dispH)
		{
			mouseY = dispH;
			SDL_WarpMouseInWindow(gWindow, mouseX, mouseY);
		}
		else if (mouseX < 0)
		{
			mouseX = 0;
			SDL_WarpMouseInWindow(gWindow, mouseX, mouseY);
		}
		else if (mouseY < 0)
		{
			mouseY = 0;
			SDL_WarpMouseInWindow(gWindow, mouseX, mouseY);
		}
		else
		{
			mouseX = (int)(mouseX * (800.0 / (float)dispW));
			mouseY = (int)(mouseY * (600.0 / (float)dispH));
			gDesktop->SetMousePosition(mouseX, mouseY);
		}
	}
	else if (e.type == SDL_JOYBUTTONDOWN)
	{
		SDL_Event evt;
		//printf("down %d\n", e.jbutton.button);
		switch (e.jbutton.button)
		{
		case 0:
			evt.type = SDL_KEYDOWN;
			evt.key.keysym.sym = SDLK_LCTRL;
			evt.key.repeat = 0;
			SDL_PushEvent(&evt);
		break;
		case 1:
			break;
		case 2:
			evt.type = SDL_KEYDOWN;
			evt.key.keysym.sym = SDLK_LSHIFT;
			evt.key.repeat = 0;
			SDL_PushEvent(&evt);
			break;
		case 3:
			evt.type = SDL_KEYDOWN;
			evt.key.keysym.sym = SDLK_LALT;
			evt.key.repeat = 0;
			SDL_PushEvent(&evt);
			break;
		}
	}
	else if (e.type == SDL_JOYBUTTONUP)
	{
		SDL_Event evt;
		//printf("up %d\n", e.jbutton.button);
		switch (e.jbutton.button)
		{
		case 0:
			evt.type = SDL_KEYUP;
			evt.key.keysym.sym = SDLK_LCTRL;
			evt.key.repeat = 0;
			SDL_PushEvent(&evt);
		break;
		case 1:
			break;
		case 2:
			evt.type = SDL_KEYUP;
			evt.key.keysym.sym = SDLK_LSHIFT;
			evt.key.repeat = 0;
			SDL_PushEvent(&evt);
			break;
		case 3:
			evt.type = SDL_KEYUP;
			evt.key.keysym.sym = SDLK_LALT;
			evt.key.repeat = 0;
			SDL_PushEvent(&evt);
			break;


		}
	}
	else if (e.type == SDL_JOYAXISMOTION)
	{
		SDL_Event evt;

		//Motion on controller 0
		if (e.jaxis.which == 0)
		{
			//X axis motion
			if (e.jaxis.axis == 0)
			{
				//Left of dead zone
				if (e.jaxis.value < -8000)
				{
					// Map joystick left to a keyboard stroke.
					//printf("Stick left\n");
					evt.type = SDL_KEYDOWN;
					evt.key.keysym.sym = SDLK_LEFT;
					evt.key.repeat = 0;
					SDL_PushEvent(&evt);
				}
				//Right of dead zone
				else if (e.jaxis.value > 8000)
				{
					// Map jouystick right to keyboard stroke.
					//printf("Stick right\n");
					

					evt.type = SDL_KEYDOWN;
					evt.key.keysym.sym = SDLK_RIGHT;
					evt.key.repeat = 0;
					SDL_PushEvent(&evt);
				}
				else
				{
					// Zero out the joystick when centered.
					//printf("Stick X center\n");
				}
			}
			//Y axis motion
			else if (e.jaxis.axis == 1)
			{
				//Below of dead zone
				if (e.jaxis.value < -8000)
				{
					//printf("Stick up\n");
					evt.type = SDL_KEYDOWN;
					evt.key.keysym.sym = SDLK_UP;
					evt.key.repeat = 0;
					SDL_PushEvent(&evt);
				}
				//Above of dead zone
				else if (e.jaxis.value > 8000)
				{
					//printf("Stick down\n");
					evt.type = SDL_KEYDOWN;
					evt.key.keysym.sym = SDLK_DOWN;
					evt.key.repeat = 0;
					SDL_PushEvent(&evt);
				}
				else
				{
					//printf("Stick Y center\n");
				}
			}
		}
	}
	else if (e.type == SDL_JOYDEVICEADDED)
	{
		printf("Joystick device: %d %d\n", e.jdevice.type, e.jdevice.which);
		if (e.jdevice.which == 0)
		{
			if (CApplication::sGameController != NULL)
			{
				SDL_JoystickClose(CApplication::sGameController);
			}
			CApplication::sGameController = SDL_JoystickOpen(0);
		}
	}
	
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) 
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) 
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

void loadFace(CTexture* tex, std::string path)
{
#ifdef WINDOWS
	path = ReplaceAll(path, std::string("/"), std::string("\\"));
	path = "C:\\" + path;
#else
#endif

	tex->LoadFromFile(path);
}




bool CheckUSB()
{
	FILE* cmd = NULL;
	bool found = false;

#ifndef WINDOWS
	char message[100];
	sprintf(message, "%s", "NONE");
	string command = "df -h | grep /media/usb0";


	cmd = popen(command.c_str(), "r");
	fscanf(cmd, "%[^\"\r\n]", message);
	//printf("Cmd: %s\n", command.c_str());
	//printf("Mess: %s\n", message);
	if (strstr(message, "/media/usb0") != NULL)
	{
		found = true;
	}
	fclose(cmd);
#endif
	return(found);
}
#undef main
int main(int argc, char **argv)
{
	char message[1000];
	strcpy(message, "");
	Uint32 usbCheck = SDL_GetTicks() + 5000;
	bool done;
	CTexture* crack = NULL;
	CPage pages[10];
	bool startDesktop = false;

	int counter = 0;

	if (argc > 1)
	{
		if (strcmp(argv[1], "-desktop") == 0)
		{
			startDesktop = true;
		}
	}
	if (!createWindow())
	{
		printf("Failed to initialize!\n");
		SDL_Quit();
	}
	else
	{
		SDL_DisplayMode current;

		int should_be_zero = SDL_GetCurrentDisplayMode(0, &current);

		if (should_be_zero != 0)
		{
			// In case of error...
			SDL_Log("Could not get display mode for video display #%d: %s", 0, SDL_GetError());
		}
		else
		{
			// On success, print the current display mode.
			SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz.", 0, current.w, current.h, current.refresh_rate);
#ifdef WINDOWS
			dispW = 720;
			dispH = 480;
#else
			dispW = current.w;
			dispH = current.h;
#endif
		}


		SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);

		//printf("CreateDesktop\n");
		gDesktop = new CDesktop(800, 600);
		gDesktop->mVisible = true;
		//printf("End CreateDesktop\n");
		SDL_Color textYellow = { RChannel(pcolor), GChannel(pcolor), BChannel(pcolor) };
		SDL_Color textGreen = { RChannel(mcolor), GChannel(mcolor), BChannel(mcolor) };
		SDL_Color textRed = { 0xFF, 0, 0 };

		//Main loop flag
		bool quit = false;
		std::stringstream cntStr;
		
		cntStr << counter;

		CreatePage("X", 0, pages[1]);
		pages[1].AddLine("Y");

		//Event handler
		SDL_Event e;
		//SDL_SetRelativeMouseMode(SDL_TRUE);
		//SDL_ShowCursor(SDL_TRUE);
		auto start = std::chrono::system_clock::now();
		// Some computation here
		auto end = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = end - start;
		double xs = (float)dispW / 800.0;
		double ys = (float)dispH / 600.0;
		SDL_StartTextInput();

		gDesktop->Init(startDesktop);

		while (!quit)
		{
			const Uint8* state = SDL_GetKeyboardState(NULL);

			//if (SDL_TICKS_PASSED_FIXED(SDL_GetTicks(), usbCheck))
			//{
			//	if (CheckUSB())
			//	{
			//		printf("USB\n");
			//	}
			//	usbCheck = SDL_GetTicks() + 5000;
			//}

			counter++;
			char str[100];

			if (counter > 100)
			{
				end = std::chrono::system_clock::now();
				elapsed_seconds = end - start;

				int fps = (int)((double)counter / (double)elapsed_seconds.count());


				sprintf(str, "%d", fps);

				pages[1].SetLine(1, str);

				counter = 0;
				start = std::chrono::system_clock::now();
			}

			gDesktop->Update();

			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				else
				{
					handleDesktop(e);
				}

			}
			std::stringstream ss;
			//ss << "X: " << mouseX << " Y: " << mouseY;
			if (CDesktop::sFocused != NULL)
			{
				ss << CDesktop::sFocused->mID << " " << CDesktop::sFocused->mSelected << " " << CDesktop::sFocused->mVisible;
			}
			else
			{
				ss << "null";
			}

			pages[1].SetLine(0, ss.str().c_str());
			
			if (gBuffer != NULL)
			{
				gBuffer->SetAsRenderTarget();
				//SDL_SetTextureBlendMode(gBuffer.mTexture, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawColor(gRenderer, 193, 243, 213, 255);
				SDL_RenderClear(gRenderer);


				pages[1].halign = false;
				pages[1].valign = true;


				gDesktop->Draw();

				//done = DrawPage(textRed, pages[1]);

				//Reset render target
				SDL_SetRenderTarget(gRenderer, NULL);

				gBuffer->DrawScaled(dispW, dispH);
			}
			if (gDesktop->mVisible)
			{
				
				if (gDesktop->mOSStatus == false)
				{
					if (crack == NULL)
					{
						crack = new CTexture();
						crack->LoadFromFile("images/crack.png");
					}
					crack->Draw(0,0,dispW, dispH, 0.0);
				}
				else if (crack != NULL)
				{
					delete(crack);
					crack = NULL;
				}
			}

			SDL_RenderPresent(gRenderer);
		}


		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);

		SDL_Quit();
	}
	return 0;
}
