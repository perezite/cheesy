// File dxinit.h

#ifndef __DXINIT_H_
#define __DXINIT_H_

#include <windows.h>
#include <ddraw.h>

//==========F U N K T I O N E N==============================================================
// Erstellt ein DirectX Vollbild-Fenster
int InitFullscreenApp(HWND *hWnd, HINSTANCE hInstance, int nCmdShow, int res_x, int res_y, int ColorDepth);
// Setzt den Clipper
int InitClipper(int Width, int Height);
// �berzieht den Bildschirm mit Schwarz
void ClearScreen(void);
//Liefert den BackBuffer
LPDIRECTDRAWSURFACE7 GetBackBuffer(void);
//Liefert die Primary Surface
LPDIRECTDRAWSURFACE7 GetPrimarySurface(void);
//Liefert das DD-Hauptobjekt
LPDIRECTDRAW7 GetDDraw7(void);
// Zerst�rt alle Objekte und beendet das Programm
bool QuitApp(char ErrorString[128]);

#endif
