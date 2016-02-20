#ifndef LEVELREADER_H
#define LEVELREADER_H

#include "object.h"

typedef struct FILE_OBJEKT      // File-Objekt
{
// Objekt-Daten
   float				x,y;			// Position des Objekts in Weltkoordinaten
   float				zx,zy;			// Zielpunkt (Weltkoordinaten) [Anfangspunkt = (x, y)]
   float				v[2];			// Geschwindigkeitsvektor
   int					energie;		// Menge an Schaden, den das Objekt einstecken kann
   int					schaden;		// Menge an Schaden, den das Objekt verursacht (pro Frame)
   char                 _class[64];     // Editorklasse des Objekts
   int					typ;			// Welche Art Objekt
   int					zustand;		// Zustand des Objektes
   LONG					LifeTime;		// Lebenszeit des Objektes (In Millisekunden)
   float				s;			    // Bewegungsgeschwindigkeit des Objektes
   bool					kollision;		// Kollisionsabfrage für Objekt ein/aus
   bool					animiert;		// Automatisches Updaten der Animationsstufe ein/aus
   float                parallax_ratio; // Parallax-Rate des Objekts (Je groesser, desto langsamer scrollt das Objekt)
// Sprite Daten
   char                 path[64];       // Pfad zu Bilddatei
// LPSPRITE 			bild;			// Sprite Struktur mit Grafiken
   int		            breite, hoehe, zeile; // Breite/Höhe einer Animationsstufe, aktuelle Animationszeile
   int					akt_bild;		// Die aktuell angezeigte Animationsstufe
   signed int			timer;			// Zeitmesser für Animation
   DWORD				AnimLifetime;	// Zeit bis zur nächsten Animationsstufe
 //   wxPoint *point;
}FILE_OBJEKT, *LPFILE_OBJEKT;


int SaveObjekt(LPEDOBJEKT objekt);
int PrepareFileSaving(const char *filename,int num_to_save, int LevelSizeX, int LevelSizeY);
LPEDOBJEKT LoadObjekt(void);
int PrepareFileLoading(const char *filename, int &LevelSizeX, int &LevelSizeY);

#endif
