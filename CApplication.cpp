#include "CApplication.h"
#include <string>

SDL_Renderer* CApplication::sRenderer = NULL;
SDL_Window* CApplication::sWindow = NULL;
SDL_Joystick* CApplication::sGameController=NULL;
#ifdef WINDOWS
std::string CApplication::sBMOS_Root = "c:/home/pi/bmos/";
#else
std::string CApplication::sBMOS_Root = "/home/pi/bmos/";
#endif