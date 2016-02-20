//**************************************************************************//
//File: sprite.cpp														    //
//Enthält Funktionen für die Grafikdarstellungen							//
//**************************************************************************//

#include "Global.h"
#include "sprite.h"
#include <iostream>
#include <fstream>
using std::cout;
using std::cin;

//=========E X T E R N A L S=======================================================================

SPRITE	SpriteArray[65];

//=========F U N K T I O N E N=====================================================================


/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}


void drawpixel(SDL_Surface *screen, int x, int y, int R, int G, int B)
{
    Uint32 yellow = SDL_MapRGB(screen->format, R, G, B);
    SDL_LockSurface(screen);
    for(int i = 0; i < 20; i++)
        putpixel(screen, x, y, yellow);
    SDL_UnlockSurface(screen);
}


/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}


int create_hardmask(LPSPRITE sprite, Uint8 R, Uint8 G, Uint8 B)
{
    Uint32 colorkey = SDL_MapRGB(sprite->Bild->format, R, G, B);
    SDL_LockSurface(sprite->Bild);
    int breite = sprite->breite;
    int hoehe = sprite->hoehe;
    int anz_anim = sprite->anz_anim + 1;                 // anz_anim+1 wegen bescheuerter Indizierung

    sprite->hardmask = (bool*)malloc(sizeof(bool)*breite*hoehe*anz_anim);

    bool *hardmask = sprite->hardmask;

    for(int k = 0; k < anz_anim; k++)
    {
        for(int i = 0; i < hoehe; i++)
        {
            for(int j = 0; j < breite; j++)
            {
                Uint32 color = getpixel(sprite->Bild, j+k*sprite->breite, i);
                if(color == colorkey)
                    hardmask[i*breite+j+(k*breite*hoehe)] = false;
                else
                    hardmask[i*breite+j+(k*breite*hoehe)] = true;
            }
        }
    }

    SDL_UnlockSurface(sprite->Bild);

    return 0;
}

/*
int print_hardmask(LPSPRITE sprite, char filename[128])
{
    bool *hardmask = sprite->hardmask;
    std:ofstream outfile1(filename);
    int breite = sprite->breite;
    int hoehe = sprite->hoehe;
    int anz_anim = sprite->anz_anim + 1;                 // anz_anim+1 wegen bescheuerter Indizierung

    // Ausgeben
    for(int k = 0; k < anz_anim; k++)
    {
        for(int j = 0; j < hoehe; j++)
        {
            for(int i = 0; i < breite; i++)
            {
                outfile1 << hardmask[j*breite+i+(k*breite*hoehe)];
            }
            outfile1 << "\n";
        }
        outfile1 << "\n---------------\n";
    }

    outfile1.close();

}*/


//---------Initialisiert sämtliche Sprites mit Grafiken--------------------------------------------
int GlobalSpriteInit(void)
{
	// Unit-Sprites
	if(CreateSprite(&SpriteArray[SD_PLAYERSHIP], "graphics\\Ship.bmp", 7, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_PSHIPINVULN], "graphics\\ShipInvuln.bmp", 7, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_PSHIPDIVING], "graphics\\ShipDiving.bmp", 7, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_PSHIPDIVED], "graphics\\ShipDived.bmp", 1, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_STDROCKET], "graphics\\Missile2.bmp", 2, 16, 16, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_DHROCKET], "graphics\\hmissile.bmp", 8, 16, 16, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_HROCKET], "graphics\\HRocket.bmp", 8, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_DUALROCKET], "graphics\\dualmissile.bmp", 2, 16, 16, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_TRROCKET], "graphics\\trmissile.bmp", 4, 8, 16, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_LASER1], "graphics\\laser1.bmp", 1, 2, 160, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_LASER2], "graphics\\laser4.bmp", 1, 2, 96, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_LASER3], "graphics\\laser4.bmp", 1, 2, 48, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_DUMMY], "graphics\\dummy.bmp", 1, 16, 16, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_DRONE], "graphics\\drone.bmp", 16, 32, 32, 0))
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
	if(CreateSprite(&SpriteArray[SD_MONSTER1C], "graphics\\Monster1C.bmp", 16, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_RUSHER], "graphics\\rusher.bmp", 24, 64, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_EXTRA], "graphics\\Extra.bmp", 8, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_SCHUSS], "graphics\\Schuss.bmp", 8, 16, 16, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_SPOREA], "graphics\\SporeA.bmp", 8, 16, 16, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_POD], "graphics\\pod.bmp", 24, 96, 96, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_DUST], "graphics\\dust.bmp", 16, 4, 4, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_SASTER96], "graphics\\SAster96.bmp", 25, 96, 96, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_SASTER64], "graphics\\SAster64.bmp", 24, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_SASTER32], "graphics\\SAster32.bmp", 16, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_MASTER96], "graphics\\MAster96.bmp", 25, 96, 96, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_MASTER64], "graphics\\MAster64.bmp", 24, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_MASTER32], "graphics\\MAster32.bmp", 16, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_GASTER96], "graphics\\GAster96.bmp", 25, 96, 96, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_GASTER64], "graphics\\GAster64.bmp", 24, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_GASTER32], "graphics\\GAster32.bmp", 16, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_WALLHUGGERL], "graphics\\wallhuggerl.bmp", 14, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_WALLHUGGERR], "graphics\\wallhuggerr.bmp", 14, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_GSHOOTL], "graphics\\gshootl.bmp", 8, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_GSHOOTR], "graphics\\gshootr.bmp", 8, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_HOMING], "graphics\\Homing.bmp", 16, 64, 64, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_HOMPROJ], "graphics\\HomProjc.bmp", 8, 16, 16, 0))
		QuitApp("Fehler");
	// Block-Sprites
	if(CreateSprite(&SpriteArray[SD_BLOCK1], "graphics\\Block1.bmp", 1, 217, 224, 0))
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
    // Shields
	if(CreateSprite(&SpriteArray[SD_SHIELD1], "graphics\\shield1.bmp", 8, 16, 16, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_PUDIVE], "graphics\\PUDive.bmp", 8, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_PUINVULN], "graphics\\PUInvuln.bmp", 8, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_PULASER], "graphics\\PULaser.bmp", 8, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_PULIFE], "graphics\\PULife.bmp", 1, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_PUMISSIL], "graphics\\PUMissil.bmp", 8, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_PUARMOR], "graphics\\PUArmor.bmp", 8, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_PUSPEED], "graphics\\PUSpeed.bmp", 8, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_PUWEAPON], "graphics\\PUWeapon.bmp", 8, 32, 32, 0))
		QuitApp("Fehler");
	if(CreateSprite(&SpriteArray[SD_PUHROCKET], "graphics\\PUHrocket.bmp", 8, 32, 32, 0))
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
//	sprite->Bild->flags = NULL;

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

    SDL_PixelFormat *Format = sprite->Bild->format;
	//Color Key für die Surface der Sprite-Struktur setzen
    SDL_SetColorKey(sprite->Bild, SDL_SRCCOLORKEY | SDL_RLEACCEL , SDL_MapRGB(Format, 255, 0, 255));
    // hardmask generieren
    create_hardmask(sprite, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B);

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


