#pragma once
#include <SDL2/SDL.h>
#include <string>

class CApplication
{
public:
	static SDL_Renderer* sRenderer;
	static SDL_Window* sWindow;
	static std::string sBMOS_Root;
	static SDL_Joystick* sGameController;
};

