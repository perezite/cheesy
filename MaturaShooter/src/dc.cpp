//**************************************************************************//
//File: dc.cpp															    //
//Enthält Funktionen für die Grafikdarstellung mit den Device Contexts		//
//*************************************************************************//

#include "dc.h"
//==========E X T E R N A L S====================================================================
extern HWND				hWnd;
//==========G L O B A L S========================================================================
HDC						hDC;							// Device Context für Grafikausgaben
HANDLE					hPen;							// Pen für die Darstellung der Pixel-Darstellung
//==========F U N K T I O N E N==================================================================
//----------Funktion zum Einstellen der Schriftart-----------------------------------------------
void SetFontPrefs(int height, int width, int weight)
{	
	HANDLE font;
	LOGFONT lpLFont;
	memset(&lpLFont, 0, sizeof(lpLFont));
	lpLFont.lfHeight = height;
	lpLFont.lfWidth = width;
	lpLFont.lfWeight = weight;
	lpLFont.lfCharSet = ANSI_CHARSET;
	lpLFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	font = CreateFontIndirect(&lpLFont);
	SelectObject(hDC, font);
	DeleteObject(font);
	return;
}
//----------Malt Text in den Backbuffer--------------------------------------------------------
void MaleText(const char text[128], int x, int y, int r, int g, int b)
{
	(GetBackBuffer())->GetDC(&hDC);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(r, g, b));
	TextOut(hDC, x, y, text, strlen(text));
	(GetBackBuffer())->ReleaseDC(hDC);
	return;
}
//----------Malt ein Pixel in den Backbuffer---------------------------------------------------
void MalePixel(int x, int y, int r, int g, int b)
{
	(GetBackBuffer())->GetDC(&hDC);
	hPen = CreatePen(PS_SOLID, 1, RGB(r,g,b));
	SelectObject(hDC, hPen);
	MoveToEx(hDC, x, y, NULL);
	LineTo(hDC, x + 1, y + 1);
	DeleteObject(hPen);
	(GetBackBuffer())->ReleaseDC(hDC);
	return;
}
 

