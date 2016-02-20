//**************************************************************************//
//File: grafik.cpp														    //
//Enthält Funktionen für die Grafikdarstellungen							//
//**************************************************************************//


#include "Grafik.h"


// VARIABELN================================================================//

extern LPDIRECTDRAW7			lpdd7;
extern LPDIRECTDRAWSURFACE7	lpddsBack;	

// FUNKTIONEN================================================================//

int add_color_key(LPDIRECTDRAWSURFACE7 surface, DWORD low, DWORD high)
{
	DDCOLORKEY key;
	ZeroMemory(&key, sizeof(key));
	key.dwColorSpaceHighValue = low;
	key.dwColorSpaceLowValue = high;
	surface->SetColorKey(DDCKEY_SRCBLT, &key);
	return 1;
}

// Füllt eine SPRITE Struktur
int init_sprite(LPSPRITE sprite, const char file_name[128], int anz_anim, 
				int breite, int hoehe, DWORD anim_speed)
{
	//Aktuelle Zeit im Sprite speichern
	sprite->timer = GetTickCount();

	//Animationsstufe auf Anfang
	sprite->akt_anim = 0;

	sprite->lpddsBild = NULL;
	//TODO: Breite, Hoehe direkt mit Surface ermitteln
	sprite->breite = breite;
	sprite->hoehe = hoehe;
	if (anz_anim > MAX_BILDER)
	{
		anz_anim = MAX_BILDER - 1;
	}
	sprite->anz_anim = anz_anim - 1;
	//Bitmap in Surface der LPSPRITE-Struktur laden
	sprite->lpddsBild = DDLoadBitmap(lpdd7, file_name, 0, 0);
	//Color Key für die Surface der Sprite-Struktur setzen
	add_color_key(sprite->lpddsBild, COLOR_KEY_LOW, COLOR_KEY_HIGH);
	return 1;
}//init_sprite

// Aktualisiert das Sprite eines Objektes
int update_sprite(LPSPRITE sprite, int objekt_timer, int objekt_akt_bild)
{
		//Prüfen, ob die Zeit bis zum nächsten Update schon überschritten ist
	if (GetTickCount() - objekt_timer > ANIM_LIFETIME)
	{
		if (objekt_akt_bild < sprite->anz_anim)
		{
		objekt_akt_bild++;
		}
		else
		{

		objekt_akt_bild = 0;
		}
	

	//Die aktuelle Zeit im Sprite speichern
	objekt_timer = GetTickCount();
	}

return 1;
} //update_sprite


// Malt das Sprite
int male_sprite(int akt_bild, LPSPRITE sprite, long ziel_x, long ziel_y, int zeile, bool color_keyed)
{
	RECT rect_quelle;
	RECT rect_ziel;
	
	//Die Position auf dem Bildschirm speichern
	sprite->pos_x = ziel_x;
	sprite->pos_y = ziel_y;
	SetRect(&rect_ziel, ziel_x, ziel_y, ziel_x + sprite->breite, ziel_y + sprite->hoehe);
	SetRect(&rect_quelle, akt_bild * sprite->breite, 
			zeile * sprite->hoehe,
			akt_bild * sprite->breite + sprite->breite, 
			zeile * sprite->hoehe + sprite->hoehe);

	if (color_keyed == true)
	{
	lpddsBack->Blt(&rect_ziel, sprite->lpddsBild, &rect_quelle, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
	}

	else
	{
	lpddsBack->Blt(&rect_ziel, sprite->lpddsBild, &rect_quelle, DDBLT_WAIT, NULL);
	}
	return 1;
} //male_sprite


//-------------------------------------------------------------------------------------------------

// Malt ein Pixel
int male_pixel_16bit(int x, int y, UCHAR rot,
                         UCHAR gruen, UCHAR blau,
                         LPDIRECTDRAWSURFACE7 lpDDSurf)
{
   int            Zeilenbreite;  // Zeilenbreite
   int            DDS_Breite;    // Breite der Surface
   int            DDS_Hoehe;     // Höhe der Surface
   USHORT         *Vram;         // Speicheradresse der Surface
   DDSURFACEDESC2 ddsd;          // SurfaceDesv Struktur

   ZeroMemory(&ddsd, sizeof(ddsd));
   ddsd.dwSize = sizeof(ddsd);

   lpDDSurf->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR |
                  DDLOCK_WAIT, NULL);
      Zeilenbreite = (int)(ddsd.lPitch >> 1);
      DDS_Breite   = ddsd.dwWidth;
      DDS_Hoehe    = ddsd.dwHeight;
      USHORT Farbe = RGB16BIT(rot,gruen,blau);

      // Prüfen, ob der Pixel überhaupt im sichtbaren Bereich
	  // der Surface gemalt werden soll!!!
      if ( x<0 || x>(DDS_Breite-1) || y<0 || y>(DDS_Hoehe-1) )
         return FALSE;
      
      Vram = (USHORT *)ddsd.lpSurface;
      Vram[x + y*Zeilenbreite] = Farbe;
   lpDDSurf->Unlock(NULL);
   return 1;
   } //male_pixel

