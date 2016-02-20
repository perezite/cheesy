//file objekt_basis.h

#ifndef __OBJEKT_BASIS_H_
#define __OBJEKT_BASIS_H_

#include <list>
#include "Grafik.h"

// Für verkettete Listen
using namespace std;

//==========T Y P E D E F S========================================================================
typedef struct OBJEKT
{
// Objekt-Daten
   float				x,y;			// Position des Objekts in Weltkoordinaten
   float				zx,zy;			// Zielpunkt (Weltkoordinaten) [Anfangspunkt = (x, y)]
   float				v[2];			// Geschwindigkeitsvektor
   int					energie;		// Menge an Schaden, den das Objekt einstecken kann
   int					schaden;		// Menge an Schaden, den das Objekt verursacht (pro Frame)
   int					typ;			// Welche Art Objekt
   int					zustand;		// Zustand des Objektes
   LONG					LifeTime;		// Lebenszeit des Objektes (In Millisekunden) 
   float				s;			// Bewegungsgeschwindigkeit des Objektes
   bool					kollision;		// Kollisionsabfrage für Objekt ein/aus
   bool					animiert;		// Automatisches Updaten der Animationsstufe ein/aus
// Sprite-Animations Daten
   LPSPRITE				bild;			// Sprite Struktur mit Grafiken
   int					zeile;			// Die aktuell angezeigt Zeile
   int					akt_bild;		// Die aktuell angezeigte Animationsstufe	
   signed int			timer;			// Zeitmesser für Animation
   DWORD				AnimLifetime;	// Zeit bis zur nächsten Animationsstufe
}OBJEKT, *LPOBJEKT;
//==========E X T E R N A L S========================================================================
extern list<OBJEKT> UnitList;									// Objekt-Liste
//==========F U N K T I O N E N====================================================================
// Fügt ein Objekt manuell in die Liste ein
int InsertObjekt(OBJEKT objekt);
// Fügt ein neues Objekt in die Liste ein und setzt dessen Eigenschaften automatisch
int CreateObjekt(float x, float y,		// Position
                float zx, float zy,		// Zielkoordinaten
                    int typ);			// Objekttyp
// Aktualisiert die verketteten Listen
int ListUpdate();
// Löscht die gesamte Objektliste
void ClearUnitList(void);
#endif
