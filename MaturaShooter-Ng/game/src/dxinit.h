// File dxinit.h

#ifndef __DXINIT_H_
#define __DXINIT_H_

#include <windows.h>
#include <SDL.h>

//==========F U N K T I O N E N==============================================================

// Erstellt ein DirectX Vollbild-Fenster
SDL_Surface *InitFullscreenApp(int res_x, int res_y, int ColorDepth);
// Überzieht den Bildschirm mit Schwarz
void ClearScreen(void);
// Zerstört alle Objekte und beendet das Programm
bool QuitApp(char ErrorString[128]);

SDL_Surface *GetSDLScreen(void);

int GetSDLScreenWidth(void);

int GetSDLScreenHeight(void);

int FlipScreen(void);

// dummy
void MaleText(const char text[128], int x, int y, int r, int g, int b);

#endif
