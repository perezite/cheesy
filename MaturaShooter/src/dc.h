//**************************************************************************//
//File: dc.h															    //
//Enthält Funktionen für die Grafikdarstellung mit den Device Contexts		//
//*************************************************************************//

#ifndef __TEXT_H_
#define __TEXT_H_

#include <windows.h>
#include "WinMain.h"
//=========F U N K T I O N E N===============================================================
//Funktion zum Einstellen der Schriftart
void SetFontPrefs(int height, int width, int weight);
//Malt Text in den Backbuffer
void MaleText(const char text[128], int x, int y, int r, int g, int b);
//Malt ein Pixel in den Backbuffer
void MalePixel(int x, int y, int r, int g, int b);

#endif
