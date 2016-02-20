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
//#include "Sounds.h"
//========E X T E R N A L S======================================================================
// Für UD_Playership:
extern signed int		PlayerShipEnergy;					// Energie des PlayerShip
extern float			PlayerShipPosX, PlayerShipPosY;		// Aktuelle Position des PlayerShip
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
#define UD_MONSTER1C    10                                  // Wurm Nr.3
#define UD_SCHUSS		7									// Schuss von Wurm Nr.2
#define UD_EXTRA		8									// Extra-Punkte
#define UD_DRONE        9                                   // Drone
#define UD_SPOREA       11                                  // Spore A
#define UD_POD          12                                  // Pod
#define UD_RUSHER       13                                  // Rusher
#define UD_SASTER96     14                                  // Asteroid 1
#define UD_SASTER64     15                                  // Asteroid 2
#define UD_SASTER32     16                                  // Asteroid 3
#define UD_GASTER96     17                                  // GAsteroid 1
#define UD_GASTER64     18                                  // GAsteroid 2
#define UD_GASTER32     19                                  // GAsteroid 3
#define UD_WALLHL       20                                  // Wallhugger links
#define UD_WALLHR       21                                  // Wallhugger rechts
#define UD_GSHOOTL      22                                  // gShoot links
#define UD_GSHOOTR      23                                  // gShoot rechts
#define UD_MASTER96     24                                  // MAsteroid 1
#define UD_MASTER64     25                                  // MAsteroid 2
#define UD_MASTER32     26                                  // MAsteroid 3
#define UD_HOMING       27                                  // Homing
#define UD_HOMPROJ      28                                  // Homing Projektil
// Block-Defines
#define UD_BLOCK1		1001								// Block Nr.1
#define UD_BLOCK2       1002                                // Block Nr.2
#define UD_BACK64A		1501								// Hintergrundblock Nr.1
// Spezialeffekte-Defines
#define UD_UNDEFGFX		2000								// Undefinerte Spezialgrafik
#define UD_EXP16		2001								// Explosion: 16 x 16
#define UD_EXP32		2002								// Explosion: 32 x 32
#define UD_EXP64		2003								// Explosion: 64 x 64
#define UD_DUST         2004                                // Explosionstrümmer eines Asteroiden
// Shields
#define UD_SHIELD1      4001                                // Shield 1
#define UD_PUDIVE       4002
#define UD_PUINVULN     4003
#define UD_PULASER      4004
#define UD_PUDUALROCKET 4005
#define UD_PUARMOR      4006
#define UD_PUSPEED      4007
#define UD_PULIFE       4008
#define UD_PUHROCKET    4010
// Waffen-Defines
#define UD_UNDEFWEAPON	3000								// Nicht definierte Waffe
#define UD_STDROCKET	3001								// Standard-Rakete
#define UD_DHROCKET     3002                                // Dronen Homing-Rocket
#define UD_HROCKET      3003                                // Playership Homing-Rocket
#define UD_TRROCKETCOMP 3004                                // Verbund aus 3 Triple-Missile Raketen
#define UD_TRROCKET     3005                                // Einzelne TripleMissile Rakete
#define UD_DUALROCKET   3006                                // Dual Rakete
#define UD_LASERCOMP    3007                                // Verbund aus den 3 Lasern-Teilen
#define UD_LASER1       3008                                // Laser Teil 1
#define UD_LASER2       3009                                // Laser Teil 2
#define UD_LASER3       3010                                // Laser Teil 3
#define UD_LASER4       3011
// Schussgeschw. Defines
#define RT_STDROCKET	100									// Schussgeschw. für StdRocket
#define RT_DHROCKET     500                                 // Schussgeschw. für Dronen HRocket
#define RT_TRROCKETCOMP 200                                 // Schussgeschw. für TripleRocket-Verbund
#define RT_HROCKET      200                                 // Schussgeschw. fuer Playership HRocket
#define RT_DUALROCKET   100                                 // Schussgeschw. Dual Rakete
#define RT_LASERCOMP    85                                  // Schussgeschw. Laser
// Gruppen Defines
#define GD_ENEMY        0
#define GD_SHIP         1
#define GD_BLOCK        2
#define GD_POWERUP      3
#define GD_MISC         4
//========F U N K T I O N E N====================================================================
// Stellt fakultative Felder der Objekt-Strutkur je nach Einheiten-Art ein
void InitUnit(LPOBJEKT lpObjekt);
// Führt KI für das jeweilige Objekt aus
void UnitKI(LPOBJEKT lpObjekt);
// Führt das Scrolling aus
void ScrollWorld(void);
// Setzt die Scroll-Position auf (0/0)
void ResetScroll(void);
// Liefert die erreichte Punktzahl
int GetScore(void);
// Setzt die Punktzahl auf 0
void ResetScore(void);
// Initialisiert Geschwindigkeitsvektor
void InitVectorDiagonal(LPOBJEKT lpObjekt);
// foo
void MoveDiagonal(LPOBJEKT lpObjekt);
// foo
void CreatePlayerShipRocket(void);
// foo
void SetChosenWeaponType(int wt);

#endif
