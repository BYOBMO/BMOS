#pragma once

#ifndef IS_BMO
#define IS_BMO 1
#endif

#include<string>

#define RChannel(hex) (Uint8)((hex >> 16) & (unsigned int)0xff) 
#define GChannel(hex) (Uint8)((hex >> 8) & (unsigned int)0xff) 
#define BChannel(hex) (Uint8)((hex) & (unsigned int)0xff) 

std::string RandomSize();


// Unfortunately SDL_TICKS_PASSED is broken. It uses a signed int for
// a comparison of the difference between A and B. If the difference is 
// large enough then the number will wrap around.
// A simple boolean comparison should fix it.
#define SDL_TICKS_PASSED_FIXED(A, B)  (A>B)

