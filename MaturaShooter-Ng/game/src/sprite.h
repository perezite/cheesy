//**************************************************************************//
//File: sprite.h														    //
//Enthält Funktionen für die Grafikdarstellungen							//
//**************************************************************************//

#ifndef __SPRITE_H_
#define __SPRITE_H_

#include "dxinit.h"

//=========T Y P E D E F S=======================================================================
// Sprite-Struktur
typedef struct SPRITE
{
	int		anz_anim;						// Anzahl Animationen des Sprites

	int		breite, hoehe, zeile;			// Breite/Höhe einer Animationsstufe, aktuelle Animationszeile
    bool    *hardmask;                      // Zeiger auf das hardmask-Array
	SDL_Surface *Bild;  			        // Surface mit Bilddaten
}SPRITE, *LPSPRITE;

#define RGB16BIT(r,g,b) ((b%32) + ((g%64) << 6) + ((r%32) << 11))
//=========G L O B A L S=========================================================================
// Globales Sprite-Array
extern SPRITE	SpriteArray[65];
//=========D E F I N E S=========================================================================

// Die Sprite-Konstanten für die verschiedenen Objekttypen
#define SD_PLAYERSHIP		0
#define SD_STDROCKET		1
#define	SD_STONE64			2
#define	SD_MONSTER1A		3
#define SD_MONSTER1B		4
#define	SD_EXP16			5
#define	SD_EXP32			7
#define	SD_EXP64			8
#define SD_EXTRA			9
#define SD_SCHUSS			10
// Hintergrund-Sprites (Verzierungen)
#define	SD_BLOCK1			11
#define	SD_BACK64A			12
// Menü-Sprites
#define SD_STARTEN			13
#define SD_BEENDEN			14
#define SD_ANFAENGER		15
#define SD_ERFAHREN			16
#define SD_PROFI			17
#define SD_CURSOR			18
#define SD_HIGHSCORE		19
// Andere Sprites
#define	SD_BACKGROUND800	20
#define SD_LOGO				21

#define SD_DHROCKET         22
#define SD_TRROCKET         23
#define SD_DUMMY            24
#define SD_DRONE            25
#define SD_MONSTER1C        26
#define SD_SPOREA           27
#define SD_POD              28
#define SD_DUST             29
#define SD_RUSHER           30
#define SD_SASTER96         31
#define SD_SASTER64         32
#define SD_SASTER32         33
#define SD_GASTER96         35
#define SD_GASTER64         36
#define SD_GASTER32         37
#define SD_SHIELD1          38
#define SD_PUDIVE           39
#define SD_PUINVULN         40
#define SD_PULASER          41
#define SD_PUMISSIL         42
#define SD_PUARMOR          43
#define SD_PUSPEED          44
#define SD_PUWEAPON         45
#define SD_PULIFE           46
#define SD_PUHROCKET        47
#define SD_HROCKET          48
#define SD_DUALROCKET       49
#define SD_LASER1           50
#define SD_LASER2           51
#define SD_LASER3           52
#define SD_PSHIPINVULN      53
#define SD_PSHIPDIVING      54
#define SD_PSHIPDIVED       55
#define SD_WALLHUGGERL      56
#define SD_WALLHUGGERR      57
#define SD_GSHOOTR          58
#define SD_GSHOOTL          59
#define SD_MASTER96         60
#define SD_MASTER64         61
#define SD_MASTER32         62
#define SD_HOMING           63
#define SD_HOMPROJ          64

//=========P R O T O S===========================================================================
/*int AddColorKey(LPDIRECTDRAWSURFACE7 surface, DWORD low, DWORD high);
// Initialisiert sämtliche Sprites mit Grafiken
int GlobalSpriteInit(void);*/
// Blabla
int GlobalSpriteInit(void);
// Füllt eine SPRITE Struktur
int CreateSprite(LPSPRITE sprite, const char file_name[128], int anz_anim,
				int breite, int hoehe, int zeile);
// Malt ein Sprite
int MaleSprite(SDL_Surface *screen, LPSPRITE sprite, long ziel_x, long ziel_y, int akt_anim, int zeile, bool color_keyed);
//int MaleSprite(LPDIRECTDRAWSURFACE7 lpddsBack, LPSPRITE sprite, long ziel_x, long ziel_y, int akt_anim, int zeile, bool color_keyed);

#endif


