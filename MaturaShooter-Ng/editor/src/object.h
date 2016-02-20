#ifndef __OBJECT_H
#define __OBJECT_H

#include <wx/dc.h>
#include <string>

using namespace std;


typedef struct EDOBJEKT      // Editor-Objekt
{
public:
    // Objekt-Daten
    float				x,y;			// Position des Objekts in Weltkoordinaten
    float				zx,zy;			// Zielpunkt (Weltkoordinaten) [Anfangspunkt = (x, y)]
    float				v[2];			// Geschwindigkeitsvektor
    int					energie;		// Menge an Schaden, den das Objekt einstecken kann
    int					schaden;		// Menge an Schaden, den das Objekt verursacht (pro Frame)
    char                 _class[64];    // Editorklasse des Objekts
    int					typ;			// Welche Art Objekt
    int					zustand;		// Zustand des Objektes
    LONG				LifeTime;		// Lebenszeit des Objektes (In Millisekunden)
    float				s;			    // Bewegungsgeschwindigkeit des Objektes
    bool				kollision;		// Kollisionsabfrage für Objekt ein/aus
    bool				animiert;		// Automatisches Updaten der Animationsstufe ein/aus
    float               parallax_ratio; // Parallax-Rate des Objekts (Je groesser, desto langsamer scrollt das Objekt)
    // Sprite Daten
    char                path[64];       // Pfad zu Bilddatei
    // LPSPRITE 			bild;			// Sprite Struktur mit Grafiken
    int		            breite, hoehe, zeile; // Breite/Höhe einer Animationsstufe, aktuelle Animationszeile
    int					akt_bild;		// Die aktuell angezeigte Animationsstufe
    signed int			timer;			// Zeitmesser für Animation
    DWORD				AnimLifetime;	// Zeit bis zur nächsten Animationsstufe
    // Editor-Stuff
    wxBitmap *bmp;                      // Zur diesem Objekt assoziierte Bitmap im Editor
    bool selected;                      // Wurde im Editor selektiert
    bool activated;                     // ist aktiviert

EDOBJEKT();
~EDOBJEKT();
}EDOBJEKT, *LPEDOBJEKT;


class EditorTile            // Editor-Tile
{
public:
    EditorTile();
    EditorTile(const char *filename, float _x, float _y, float _w, float _h);
    ~EditorTile();
    int SetBitmap(const char *filename);
    int Draw(wxDC *dc);

    void SetX(float x);
    void SetY(float y);
    void SetW(int w);
    void SetH(int h);

    string GetFilename(void);
private:
    float x, y;           // Position
    int w, h;           // Width/Height
    string filename;
    wxBitmap *bmp;
    bool selected;
};

#endif
