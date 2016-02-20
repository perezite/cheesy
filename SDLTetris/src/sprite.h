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
	int		anz_anim;						//Anzahl Animationen des Sprites

	int		breite, hoehe, zeile;			//Breite/Höhe einer Animationsstufe, aktuelle Animationszeile
    bool    *hardmask;                      // Zeiger auf das hardmask-Array
	SDL_Surface *Bild;  			        //Surface mit Bilddaten
}SPRITE, *LPSPRITE;

#define RGB16BIT(r,g,b) ((b%32) + ((g%64) << 6) + ((r%32) << 11))
//=========G L O B A L S=========================================================================
// Globales Sprite-Array
extern SPRITE	SpriteArray[22];
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


