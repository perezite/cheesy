//**************************************************************************//
//File: sprite.cpp														    //
//Enthält Funktionen für die Grafikdarstellungen							//
//**************************************************************************//

#include "Global.h"
#include "sprite.h"

//=========E X T E R N A L S=======================================================================

SPRITE	SpriteArray[22];

//=========F U N K T I O N E N=====================================================================
/*
//---------Initialisiert sämtliche Sprites mit Grafiken--------------------------------------------
int GlobalSpriteInit(void)
{
	// Unit-Sprites
	if(CreateSprite(&SpriteArray[SD_PLAYERSHIP], "graphics\\Ship.bmp", 7, 64, 64, 0))
        return 0;
}
*/


//---------Initialisiert sämtliche Sprites mit Grafiken--------------------------------------------
int GlobalSpriteInit(void)
{
	// Unit-Sprites
	if(CreateSprite(&SpriteArray[SD_PLAYERSHIP], "graphics\\Ship.bmp", 7, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_STDROCKET], "graphics\\Missile2.bmp", 2, 16, 16, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_STONE64], "graphics\\Stone64.bmp", 24, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_EXP16], "graphics\\Exp16.bmp", 10, 16, 16, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_EXP32], "graphics\\Exp32.bmp", 10, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_EXP64], "graphics\\Exp64.bmp", 10, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_MONSTER1A], "graphics\\Monster1A.bmp", 16, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_MONSTER1B], "graphics\\Monster1B.bmp", 16, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_EXTRA], "graphics\\Extra.bmp", 8, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_SCHUSS], "graphics\\Schuss.bmp", 8, 16, 16, 0))
		QuitApp("Fehler");
	// Block-Sprites
	if(CreateSprite(&SpriteArray[SD_BLOCK1], "graphics\\Block1.bmp", 1, 353, 97, 0))
		QuitApp("Fehler");
	// Background-Block Sprites
	if(CreateSprite(&SpriteArray[SD_BACK64A], "graphics\\Back64A.bmp", 1, 64, 64, 0))
		QuitApp("Fehler");
	// Menü-Sprites
	if(CreateSprite(&SpriteArray[SD_STARTEN], "graphics\\starten.bmp", 1, 300, 30, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_BEENDEN], "graphics\\beenden.bmp", 1, 300, 30, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_ANFAENGER], "graphics\\anfaenger.bmp", 1, 300, 30, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_ERFAHREN], "graphics\\erfahren.bmp", 1, 300, 30, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_PROFI], "graphics\\profi.bmp", 1, 300, 30, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_CURSOR], "graphics\\cursor.bmp", 1, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_HIGHSCORE], "graphics\\highscore.bmp", 1, 300, 30, 0))
		QuitApp("Fehler");
	// Andere Sprites
	if(CreateSprite(&SpriteArray[SD_BACKGROUND800], "graphics\\background800.bmp", 1, 800, 600, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_LOGO], "graphics\\logo.bmp", 1, 640, 141, 0))
		QuitApp("Fehler");
	return 0;
}


//---------Füllt eine SPRITE Struktur--------------------------------------------------------
int CreateSprite(LPSPRITE sprite, const char file_name[128], int anz_anim,
				int breite, int hoehe, int zeile)
{
	sprite->breite = breite;
	sprite->hoehe = hoehe;
	sprite->zeile = zeile;

	//Testen, ob zu viele Animationsstufen vorhanden sind
	if (anz_anim > MAX_BILDER)
		return 1;
	else
		sprite->anz_anim = anz_anim - 1;

	//Bitmap in Surface der Sprite Struktur laden
	sprite->Bild = NULL;
    sprite->Bild = SDL_LoadBMP(file_name);
    if (sprite->Bild == NULL)
    {
        printf("Can't load file %s: %s\n", file_name, SDL_GetError());
        return 1;
    }

	//Color Key für die Surface der Sprite-Struktur setzen
    SDL_PixelFormat *Format = sprite->Bild->format;
/*    SDL_SetColorKey(sprite->Bild, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Format, 255, 0, 255));*/
    SDL_SetColorKey(sprite->Bild, SDL_SRCCOLORKEY | SDL_RLEACCEL , SDL_MapRGB(Format, 255, 0, 255));

	return 0;
}


//---------Malt das Sprite-------------------------------------------------------------------

int MaleSprite(SDL_Surface *screen, LPSPRITE sprite, long ziel_x, long ziel_y, int akt_anim, int zeile, bool color_keyed)
{
	SDL_Rect rect_quelle;
	SDL_Rect rect_ziel;

    rect_ziel.x = ziel_x; rect_ziel.y = ziel_y; rect_ziel.w = sprite->breite; rect_ziel.h = sprite->hoehe;
//    SetRect(&rect_ziel, ziel_x, ziel_y, ziel_x + sprite->breite, ziel_y + sprite->hoehe);
	// Nur der Teil der Offscreen Surface, der der aktuelle Animationsstufe entspricht, soll
	// kopiert werden
    rect_quelle.x = akt_anim * sprite->breite; rect_quelle.y = zeile*sprite->hoehe; rect_quelle.w = sprite->breite;
    rect_quelle.h = sprite->hoehe;
/*	SetRect(&rect_quelle, akt_anim * sprite->breite,
			zeile * sprite->hoehe,
			akt_anim * sprite->breite + sprite->breite,
			zeile * sprite->hoehe + sprite->hoehe);*/

/*	if (color_keyed == true)
		lpddsBack->Blt(&rect_ziel, sprite->lpddsBild, &rect_quelle, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
	else
		lpddsBack->Blt(&rect_ziel, sprite->lpddsBild, &rect_quelle, DDBLT_WAIT, NULL);*/
    SDL_BlitSurface(sprite->Bild, &rect_quelle, screen, &rect_ziel);

	return 0;
} //MaleSprite


