//file: WinMain.h

#ifndef __WINMAIN_H_
#define __WINMAIN_H_

#include "dxinit.h"

//===========E X T E R N A L S===================================================================
//extern int nVirtKey;							// Gedr�ckte Taste
extern float fFrameTime;						// Zeit f�r den letzten Framedurchlauf (in float)
extern DWORD dwFrameTime;						// Zeit f�r den letzten Framedurchlauf (in DWORD)
extern float Fps;								// Frames per Second
//===========D E F I N E S=======================================================================
// Setzt den aktuellen Programmteil
void SetProgControl(int NewProgControl);
//===========D E F I N E S=======================================================================
// Fenster-Status
#define WC_ENABLED				4				// Fenster wurde aufgerufen
#define WC_DISABLED				5				// Fenster wurde minimiert oder hat den Fokus verloren
// Programm Status
#define PC_INITGAME				10				// Laden der Grafiken, Sounds, Fenster etc.
#define PC_LOADMENU				11				// Laden des Men�s
#define PC_SHOWMENU				12				// Hauptmen� anzeigen
#define PC_MAINLOOP				13				// Das eigentliche Spiel
#define PC_FADEOUT				14				// Nach Zerst�rung des Raumschiffs Todesanzeige
#define PC_LOADHIGHSCORE		15				// Laden des Highscore-Bildschirms
#define PC_SHOWHIGHSCORE		16				// Anzeigen des Highscore-Bildschirms
#define PC_SHOWSCORE			17				// Anzeigen der Punktzahl, falls man in den Highscore gekommen ist
#define PC_SHOWNOSCORE			18				// Anzeigen, dass man nicht in den Score gekommen ist
#define PC_SAVESCORE			19				// Speichert den erzielten Score in der Highscore-Liste (falls gross genug)
#define PC_RESETGAME			20				// Spiel-Objekte und -Variablen neu initalisieren
#define PC_SHOWDEATH			21				// Zeigt noch die Zerst�rung des Spielerschiffes
#define PC_ENDPROG              22              // End the program
// Buttons-Typ
#define BD_BOOL					0				// Button ver�ndert eine Bool-Variable
#define BD_INT					1				// Button ver�ndert eine Int-Variable
#define BD_CHANGESTATUS			2				// Button ver�ndert ProgControl (Programm-Status)
#endif


