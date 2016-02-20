//**************************************************************************//
//File: sprite_func.h														//
//Enth�lt die Funktionen f�r die Verwaltung und Anzeige der Sprites			//
//**************************************************************************//


// DEFINES==================================================================//

#define ABSTAND 0								//Abstand f�r Schatten beim Markierungsrahmen
#define RGB16BIT(r,g,b) ((b%32) + ((g%64) << 6) + ((r%32) << 11))

//==========================================================================//



// FUNKTIONEN===============================================================//

//Malt einen Text auf eine Surface
int male_text_16bit(int x, int y, UCHAR rot, UCHAR gruen, UCHAR blau,
                        LPDIRECTDRAWSURFACE7 lpDDSurf, const char *Text);
//==========================================================================//