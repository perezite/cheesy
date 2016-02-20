// File: dxinit.cpp

#include "dxinit.h"
#include <windows.h>


//===============P R O T O T Y P E N==============================================================

SDL_Surface *InitFullscreenApp(int res_x, int res_y, int ColorDepth);
void ClearScreen(void);
bool QuitApp(char ErrorString[128]);
SDL_Surface *GetSDLScreen(void);
int GetSDLScreenWidth(void);
int GetSDLScreenHeigth(void);
//extern LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM  lParam);

//===============G L O B A L S====================================================================

SDL_Surface *SDL_Screen;
int SDL_ScreenWidth, SDL_ScreenHeight;

//===============F U N K T I O N E N==============================================================

// dummy
void MaleText(const char text[128], int x, int y, int r, int g, int b)
{
    return;
}

//---------Erstellt ein SDL Vollbild-Fenster--------------------------------------------------
SDL_Surface *InitFullscreenApp(int res_x, int res_y, int ColorDepth)
{

    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return NULL;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(res_x, res_y, ColorDepth,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set %dx%d video: %s\n", res_x, res_y, SDL_GetError());
        return NULL;
    }

    SDL_Screen = screen;

    return screen;
}


// Ueberzieht den Bildschirm mit Schwarz

void ClearScreen(void)
{
    SDL_FillRect(SDL_Screen, 0, SDL_MapRGB(SDL_Screen->format, 0, 0, 0));
    return;
}

//-----------Zerstört alle Objekte und beendet das Programm---------------------------------------
bool QuitApp(char ErrorString[128])
{
    return true;
} //Ende: QuitApp


//--- blabla
SDL_Surface *GetSDLScreen(void)
{
    return SDL_Screen;
}


//--- blabla
int GetSDLScreenWidth(void)
{
    return SDL_ScreenWidth;
}


//--- blabla
int GetSDLScreenHeigth(void)
{
    return SDL_ScreenHeight;
}


//--- blabla
int FlipScreen(void)
{
    SDL_Flip(SDL_Screen);
    return 0;
}

