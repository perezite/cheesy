//**************************************************************************//
//File: units.h																//
//Verändert die Erstellung von Objekten je nach								//
//Art der Einheit und enthält die KI.										//
//Enthält ausserdem die Sprite-Inits										//
//**************************************************************************//

#ifndef __UNITS_H_
#define __UNITS_H_

#include "Global.h"
#include "Objekt.h"
#include "Grafik.h"
#include "Sounds.h"
//========E X T E R N A L S======================================================================
// Für UD_Playership:
extern signed int		PlayerShipEnergy;					// Energie des PlayerShip
extern int				PlayerShipPosX, PlayerShipPosY;		// Aktuelle Position des PlayerShip
// Für Scrolling
extern float			ScrollX;							// Momentane Scroll-Position (X) 
extern float			ScrollY;							// Momentane Scroll-Postionn (Y) 
//========D E F I N E S==========================================================================
// Objekt-Zustands-Defines
#define OZ_UNDEF		0									// Nicht definierter Zustand
#define OZ_STOP			1									// Ruhezustand
#define OZ_GO			2									// Bewegung
#define OZ_ATTACK		3									// Angriff
#define OZ_DESTROYED	4									// Zerstört, d.h. Explodiert gerade
#define OZ_DEAD			5									// Muss im nächsten Frame gelöscht werden	
// Einheiten-Defines
#define UD_UNDEFOBJ		0									// Nicht definiertes Objekt
#define UD_PLAYERSHIP	1									// Das Schiff des Spielers
#define UD_STONE64		2									// Grosser Stein
#define UD_MONSTER1A	5									// Wurm Nr.1
#define UD_MONSTER1B	6									// Wurm Nr.2
#define UD_SCHUSS		7									// Schuss von Wurm Nr.2
#define UD_EXTRA		8									// Extra-Punkte
// Block-Defines
#define UD_BLOCK1		1001								// Block Nr.1
#define UD_BACK64A		1501								// Hintergrundblock Nr.1
// Spezialeffekte-Defines
#define UD_UNDEFGFX		2000								// Undefinerte Spezialgrafik
#define UD_EXP16		2001								// Explosion: 16 x 16
#define UD_EXP32		2002								// Explosion: 32 x 32
#define UD_EXP64		2003								// Explosion: 64 x 64
// Waffen-Defines
#define UD_UNDEFWEAPON	3000								// Nicht definierte Waffe
#define UD_STDROCKET	3001								// Standard-Rakete
// Andere Defines
#define RT_STDROCKET	200									// Schussgeschw. für StdRocket
//========F U N K T I O N E N====================================================================
// Stellt fakultative Felder der Objekt-Strutkur je nach Einheiten-Art ein
void InitUnit(LPOBJEKT lpObjekt);
// Führt KI für das jeweilige Objekt aus														
void UnitKI(LPOBJEKT lpObjekt);	
// Führt das Scrolling aus						
void ScrollWorld(void);
// Setzt die Scroll-Position auf (0/0)
void ResetScroll(void);	
//Liefert die erreichte Punktzahl
int GetScore(void);	
//Setzt die Punktzahl auf 0
void ResetScore(void);								

#endif
