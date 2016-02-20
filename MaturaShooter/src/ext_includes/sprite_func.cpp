//**************************************************************************//
//File: sprite_func.cpp														//
//Enthält die Funktionen für die Verwaltung und Anzeige der Sprites			//
//**************************************************************************//

#include <ddraw.h>
#include <windows.h>
#include "../Global.h"
#include "sprite_func.h"

//////////////////////////////////////////////////////////////////////////////

int male_text_16bit(int x, int y, UCHAR rot, UCHAR gruen, UCHAR blau,
                        LPDIRECTDRAWSURFACE7 lpDDSurf, const char *Text)
   {
   char           Token;         // Aktuelles Zeichen im Text
   int            TextLaenge;    // Laenge des Textes
   int            Zeilenbreite;  // Zeilenbreite
   USHORT         *Vram;         // Speicheradresse der Surface
   DDSURFACEDESC2 ddsd;          // SurfaceDesv Struktur

   // Text zu hoch oder zu tief fuer Anzeige
   if ( y<8 || y> RESOLUTION_Y )
      return TRUE;

   TextLaenge = strlen(Text);
   ZeroMemory(&ddsd, sizeof(ddsd));
   ddsd.dwSize = sizeof(ddsd);

   lpDDSurf->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR |
                  DDLOCK_WAIT, NULL);
   Zeilenbreite = (int)(ddsd.lPitch >> 1);
   Vram         = (USHORT *)ddsd.lpSurface;
   USHORT Farbe = RGB16BIT(rot,gruen,blau);
   
   // Text durchlaufen
   for (int i=0; i<TextLaenge; i++)
      {
      Token = Text[i];
      // Zu weit links kann korrigiert werden...
      if (x<0) {
         x += 10;
         continue;
         }
      // ...zu weit rechts nicht!
      else if (x>(RESOLUTION_X-12))
         {
         lpDDSurf->Unlock(NULL);    // nicht vergessen!!!
         return 1;
         }

      // Welcher Buchstabe soll gemalt werden
      switch(Token)
	      {	
	      case 'A': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+6 + y*Zeilenbreite] = Farbe;
            Vram[x+6 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            // Mittelstrich
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            x += 8; // Position des Cursors weiterschieben
		      } break;
         case 'a': 
   		   {
            // Linke Seite
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'B': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Rechte Striche
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
            } break;
         case 'b': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Oben und unten
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'C': 
   		   {
            // Linker Bogen
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
            // Oben und Unten
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Rechte Seite
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            x += 7; // Position des Cursors weiterschieben
            } break;
         case 'c': 
   		   {
            // Linker Strich
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            // Oben und unten
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            x += 5; // Position des Cursors weiterschieben
		      } break;
         case 'D': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Oben und Unten
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Rechter Bogen
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            x += 7; // Position des Cursors weiterschieben
            } break;
         case 'd': 
   		   {
            // Linker Strich
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            // Oben und unten
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'E': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
            } break;
         case 'e': 
   		   {
            // Linker Strich
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            // Oben und Rechts
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            // Mitte
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'F': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
            } break;
         case 'f': 
   		   {
            // Linker Strich
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            x += 5; // Position des Cursors weiterschieben
		      } break;
         case 'G': 
   		   {
            // Linker Bogen
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
            // Oben und Unten
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Rechte Seite
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            x += 7; // Position des Cursors weiterschieben
            } break;
         case 'g': 
   		   {
            // Linker Strich
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            // Oben und unten
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y+1)*Zeilenbreite] = Farbe;
            // Ganz unten
            Vram[x + (y+2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y+2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y+2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y+2)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'H': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Querstrich
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;

            x += 6; // Position des Cursors weiterschieben
            } break;
         case 'h': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Bogen
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'I': 
   		   {
            // Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            x += 2; // Position des Cursors weiterschieben
            } break;
         case 'i': 
   		   {
            // Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            x += 2; // Position des Cursors weiterschieben
		      } break;
         case 'J': 
   		   {
            // Fuss
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
            } break;
         case 'j': 
   		   {
            // Strich
            Vram[x + (y+1)*Zeilenbreite] = Farbe;
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            x += 3; // Position des Cursors weiterschieben
		      } break;
         case 'K': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Schraegstriche
            Vram[x+5 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;
            x += 7; // Position des Cursors weiterschieben
            } break;
         case 'k': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Schraegstriche
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'L': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Unterer Strich
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
            } break;
         case 'l': 
   		   {
            // Linker Strich
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            x += 3; // Position des Cursors weiterschieben
		      } break;
         case 'M': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+6 + y*Zeilenbreite] = Farbe;
            Vram[x+6 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+6 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+6 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+6 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+6 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+6 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+6 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+6 + (y-8)*Zeilenbreite] = Farbe;
            // Mitte
            Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
            x += 8; // Position des Cursors weiterschieben
            } break;
         case 'm': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            // Mittlerer Strich
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+8 + y*Zeilenbreite] = Farbe;
            Vram[x+8 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+8 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+8 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+8 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+8 + (y-5)*Zeilenbreite] = Farbe;
            // Obere Bögen
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+6 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+7 + (y-6)*Zeilenbreite] = Farbe;
            x += 10; // Position des Cursors weiterschieben
		      } break;
         case 'N': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+5 + y*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;
            // Schraegstrich
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
            x += 7; // Position des Cursors weiterschieben
            } break;
         case 'n': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            // Oberer Strich
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'O': 
   		   {
            // Linker Bogen
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
            // Oben und Unten
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Rechte Seite
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            x += 7; // Position des Cursors weiterschieben
            } break;
         case 'o': 
   		   {
            // Linker Strich
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            // Oben und Unten
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'P': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
            } break;
         case 'p': 
   		   {
            // Linker Strich
            Vram[x + (y+2)*Zeilenbreite] = Farbe;
            Vram[x + (y+1)*Zeilenbreite] = Farbe;
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            // Oben und unten
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'Q': 
   		   {
            // Linker Bogen
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
            // Oben und Unten
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Rechte Seite
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            // Q-Strich
            Vram[x+5 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            x += 7; // Position des Cursors weiterschieben
            } break;
         case 'q': 
   		   {
            // Linker Strich
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            // Oben und unten
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + (y+2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y+1)*Zeilenbreite] = Farbe;
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'R': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            // Schraegstrich
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
            } break;
         case 'r': 
   		   {
            // Linker Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            // Oberer Strich
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            x += 4; // Position des Cursors weiterschieben
		      } break;
         case 'S': 
   		   {
            // Linke Striche
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            // Rechte Striche
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
            } break;
         case 's': 
   		   {
            // Linke Seite
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            // Rechte Seite
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'T': 
   		   {
            // Mittelstrich
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            // Querstrich
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
            } break;
         case 't': 
   		   {
            // Strich
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            // Querstrich
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            x += 4; // Position des Cursors weiterschieben
		      } break;
         case 'U': 
   		   {
            // Linke Seite
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Unten
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            // Rechte Seite
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;
            x += 7; // Position des Cursors weiterschieben
            } break;
         case 'u': 
   		   {
            // Linke Seite
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            // Rechte Seite
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            // Unten
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'V': 
   		   {
            // Linke Seite
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Rechte Seite
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;
            x += 7; // Position des Cursors weiterschieben
            } break;
         case 'v': 
   		   {
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'W': 
   		   {
            // Linke Seite
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Mitte
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
            // Rechte Seite
            Vram[x+6 + y*Zeilenbreite] = Farbe;
            Vram[x+6 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+7 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+7 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+7 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+7 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+8 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+8 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+8 + (y-8)*Zeilenbreite] = Farbe;
            x += 10; // Position des Cursors weiterschieben
            } break;
         case 'w': 
   		   {
            // Linke Seite
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            // Mitte
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
            // Rechte Seite
            Vram[x+6 + y*Zeilenbreite] = Farbe;
            Vram[x+6 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+7 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+7 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+7 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+8 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+8 + (y-6)*Zeilenbreite] = Farbe;
            x += 10; // Position des Cursors weiterschieben
		      } break;
         case 'X': 
   		   {
            // Schraegstrich links
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Schraegstrich links
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
            } break;
         case 'x': 
   		   {
            // Schraegstrich links
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            // Schraegstrich links
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'Y': 
   		   {
            // Schraegstrich links
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            // Mitte
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            // Schraegstrich links
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
            } break;
         case 'y': 
   		   {
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + (y+1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y+2)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case 'Z': 
   		   {
            // Querstriche
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+5 + y*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;
            // Schraegstrich
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            x += 7; // Position des Cursors weiterschieben
            } break;
         case 'z': 
   		   {
            // Oben und Unten
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            // Schraegstrich
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '0': 
   		   {
            // Linker Bogen
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            // Oben und Unten
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            // Rechte Seite
            Vram[x+3 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-7)*Zeilenbreite] = Farbe;
            x += 5; // Position des Cursors weiterschieben
		      } break;
         case '1': 
   		   {
            // Strich
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            x += 4; // Position des Cursors weiterschieben
		      } break;
         case '2': 
   		   {
            // Linker Punkt
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+4 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Schraegbogen
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '3': 
   		   {
            // Linke Striche
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Rechte Striche
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '4': 
   		   {
            // Querstrich
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            // Schraegstrich
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-7)*Zeilenbreite] = Farbe;
            // Hochstrich
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '5': 
   		   {
            // Rechte Striche
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Linke Striche
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '6': 
   		   {
            // Rechte Striche
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Linker Strich
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '7': 
   		   {
            // Querstrich
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
            // Schraegstrich
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-7)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '8': 
   		   {
            // Linke Striche
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Rechte Striche
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '9': 
   		   {
            // Linke Striche
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            // Querstriche
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            Vram[x+3 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            // Rechter Strich
            Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '<': 
   		   {
            Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '>': 
   		   {
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '-': 
   		   {
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            x += 4; // Position des Cursors weiterschieben
		      } break;
         case '+': 
   		   {
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '*': 
   		   {
            // Querstrich
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            // Rest
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '/': 
   		   {
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            x += 4; // Position des Cursors weiterschieben
		      } break;
         case '=': 
   		   {
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
            x += 5; // Position des Cursors weiterschieben
		      } break;
         case '!': 
   		   {
            // Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            x += 2; // Position des Cursors weiterschieben
		      } break;
         case '?': 
   		   {
            // Bogen oben
            Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            // Schraegstrich
            Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + y*Zeilenbreite] = Farbe;
            x += 6; // Position des Cursors weiterschieben
		      } break;
         case '.': 
   		   {
            // Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            x += 2; // Position des Cursors weiterschieben
		      } break;
         case ',': 
   		   {
            // Strich
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y+1)*Zeilenbreite] = Farbe;
            x += 2; // Position des Cursors weiterschieben
		      } break;
         case ';': 
   		   {
            // Strich
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y+1)*Zeilenbreite] = Farbe;
            x += 2; // Position des Cursors weiterschieben
		      } break;
         case ':': 
   		   {
            // Strich
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + y*Zeilenbreite] = Farbe;
            x += 2; // Position des Cursors weiterschieben
		      } break;
         case '(': 
   		   {
            // Bogen
            Vram[x+2 + (y+2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y+1)*Zeilenbreite] = Farbe;
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
            x += 4; // Position des Cursors weiterschieben
		      } break;
         case ')': 
   		   {
            // Strich
            // Bogen
            Vram[x + (y+2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y+1)*Zeilenbreite] = Farbe;
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            x += 4; // Position des Cursors weiterschieben
		      } break;
         case '[': 
   		   {
            // Strich
            Vram[x+1 + (y+2)*Zeilenbreite] = Farbe;
            Vram[x + (y+2)*Zeilenbreite] = Farbe;
            Vram[x + (y+1)*Zeilenbreite] = Farbe;
            Vram[x + y*Zeilenbreite] = Farbe;
            Vram[x + (y-1)*Zeilenbreite] = Farbe;
            Vram[x + (y-2)*Zeilenbreite] = Farbe;
            Vram[x + (y-3)*Zeilenbreite] = Farbe;
            Vram[x + (y-4)*Zeilenbreite] = Farbe;
            Vram[x + (y-5)*Zeilenbreite] = Farbe;
            Vram[x + (y-6)*Zeilenbreite] = Farbe;
            Vram[x + (y-7)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;            
            x += 3; // Position des Cursors weiterschieben
		      } break;
         case ']': 
   		   {
            // Strich
            Vram[x + (y+2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y+2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y+1)*Zeilenbreite] = Farbe;
            Vram[x+1 + y*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
            Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
            Vram[x + (y-8)*Zeilenbreite] = Farbe; 
            x += 3; // Position des Cursors weiterschieben
		      } break;
         default: { x += 4; } break;
         } // switch
      } // for
   lpDDSurf->Unlock(NULL);
   return 1;
   } // xxxMale_Text

//==========================================================================//