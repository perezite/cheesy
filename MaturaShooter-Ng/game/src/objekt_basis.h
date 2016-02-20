//file objekt_basis.h

#ifndef __OBJEKT_BASIS_H_
#define __OBJEKT_BASIS_H_

#include <list>
#include <map>
#include "sprite.h"

// Für verkettete Listen
using namespace std;

//==========T Y P E D E F S========================================================================
typedef struct OBJEKT
{
// Objekt-Daten
   float				x,y;			// Position des Objekts in Weltkoordinaten
   float				zx,zy;			// Zielpunkt (Weltkoordinaten) [Anfangspunkt = (x, y)] (x)
   float				v[2];			// Geschwindigkeitsvektor (x)
   int					energie;		// Menge an Schaden, den das Objekt einstecken kann
   int					schaden;		// Menge an Schaden, den das Objekt verursacht (pro Frame)
   int					typ;			// Welche Art Objekt
   int					zustand;		// Zustand des Objektes
   LONG					LifeTime;		// Lebenszeit des Objektes (In Millisekunden)
   float				s;			    // Bewegungsgeschwindigkeit des Objektes
   bool					kollision;		// Kollisionsabfrage für Objekt ein/aus
   bool					animiert;		// Automatisches Updaten der Animationsstufe ein/aus
   float                parallax_ratio; // Parallax-Rate des Objekts (Je groesser, desto langsamer scrollt das Objekt)
   map<int, int>        groups;          // Zum Objekt assoziierte Gruppen
// Sprite-Animations Daten
   LPSPRITE				bild;			// Sprite Struktur mit Grafiken (x)
   int					zeile;			// Die aktuell angezeigt Zeile
   int					akt_bild;		// Die aktuell angezeigte Animationsstufe
   signed int			timer;			// Zeitmesser für Animation (x)
   DWORD				AnimLifetime;	// Zeit bis zur nächsten Animationsstufe
}OBJEKT, *LPOBJEKT;
//==========E X T E R N A L S========================================================================
extern list<OBJEKT> UnitList;									// Objekt-Liste
//==========F U N K T I O N E N====================================================================

int ListUpdate();
int UpdateObjectSprite(LPOBJEKT objekt);
int InsertObjekt(OBJEKT objekt);
int CreateObjekt(float x, float y, float zx, float zy, int typ);
int CreateInactiveObjekt(float x, float y, int typ);
void ClearUnitList(void);
int LoadLevelFromFile(char path[128]);

#endif
