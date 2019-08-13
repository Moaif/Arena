#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "./Engine/MemLeaks.h"
#include <SDL.h>
#include <assert.h>
#include <string>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Useful typedefs ---------
typedef unsigned int uint;

// Deletes a buffer
#define RELEASE( x ) \
    {									  \
       if( x != nullptr )   \
       {						      \
         delete x;                  \
	     x = nullptr;             \
       }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{                              \
       if( x != nullptr )              \
       {                            \
           delete[] x;                \
	       x = nullptr;                    \
		 }                            \
                              \
	 }

//Random 
#ifdef _DEBUG
#define RAND() getPseudoRand() 
#else
#define RAND() getRand()
#endif

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FULLSCREEN false
#define VSYNC true
#define TITLE "Arena"

int getPseudoRand();
int getRand();
void toFileLog(std::string message);

#define ASSERT(condition, statement) \
    do { \
        if (!(condition)) { toFileLog(statement); assert(condition); } \
    } while (false)
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT(message) __FILE__ ":" TOSTRING(__LINE__) " " message

#endif //__GLOBALS_H__