//**************************************************************************//
//File: units.cpp															//
//Verändert die Erstellung von Objekten je nach								//
//Art der Einheit und enthält die KI, sowie die Kollisionsabfrage.			//
//**************************************************************************//

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <map>
using namespace std;
#include "linalg.h"
#include "units.h"
#include "main.h"
#include "audio.h"
#include "levelloader.h"


//===========G L O B A L S=========================================================================
// Variabeln für die Bewegung des Spielerschiffes
signed int	PlayerShipEnergy = 0;			// Energie des PlayerShip(Extern)
DWORD		ZurueckNeigenTimer;				// Timer für das langsame zurückneigen des Schiffes
float		PlayerShipPosX, PlayerShipPosY;	// Aktuelle Position des PlayerShip (Extern)
float       PlayerShipvx, PlayerShipvy;     // Aktuelle Geschw. Komponenten des PlayerShip
int         PlayerShipState = 2;            // Schiffstatus (Normal=0, Invuln=1, Diving=2, Dived=3, Rediving=4)
float		iRocketTimer = 100;				// Zeit bis wieder eine Rakete verschossen werden darf
int			ChosenWeaponType = UD_STDROCKET;// Momentan gewählte Waffe
float		WeaponShotSpeed = RT_STDROCKET; // Schussgeschwindigkeit der aktuellen Waffe
// Variablen für das Scrolling
float		ScrollX		= 0;				// Ortsvektor zum Ursprung des Bildschirmkoordinatensystems
float		ScrollY		= 0;				// Ortsvektor zum Ursprung des Bildschirmkoordinatensystems
// Variabeln für den Highscore
int			Score = 0;						// Bereits erreichte Punktzahl
// Audio Manager
//AudioManager Am2;
//===========P R O T O S==========================================================================
//Prüft, ob das Objekt zerstört(noch nicht gelöscht) wurde und erstellt Explosion
bool CheckIfDestroyed(LPOBJEKT lpObjekt);
//Berechnet den Geschwindigkeitsvektor für die diagonale Bewegung
void InitVectorDiagonal(LPOBJEKT lpObjekt);
//Führt die diagonale Bewegung aus
void MoveDiagonal(LPOBJEKT lpObjekt);
//Führt das Scrolling aus
void ScrollWorld(void);
//Berechnet den Betrag eines Vektors
float VectorLength(float v[2]);
//bla
float angle(float v1[2], float v2[2]);
//bla
float angle_360_cw(float v[2]);
//Setzt die Scroll-Position auf (0/0)
void ResetScroll(void);
//Liefert die erreichte Punktzahl
int GetScore(void);
//Setzt die Punktzahl auf 0
void ResetScore(void);
// Velocity-Vektor für ein &bullet rausfinden, das nächstes Ziel mit Geschw. v0 treffen soll
void GetTargetVector(OBJEKT &Objekt, double v0);
// Berechnet und setztAnfangsposition für eine Drone
void SetDroneSpawnPos(LPOBJEKT drone);
// Updatet Drone
void UpdateDrone(LPOBJEKT drone);
// Berechnet und setzt Anfangsposition für eine Shield
void SetShieldSpawnPos(LPOBJEKT shield);
// Updatet Shield
void UpdateShield(LPOBJEKT shield);
// Momentanes Bild fuer HRocket ermitteln
int get_akt_bild_HRocket(LPOBJEKT objekt);
// Objekt auf Playership zielen lassen (mit Vorhaltewinkel)
void AimAtPlayership(LPOBJEKT obj);
//Findet Zwischenwinkel für ein &Objekt zum nächstliegenden Ziel
float GetTargetDeltaAngle(OBJEKT &Objekt, double v0);

//===========F U N K T I O N E N===================================================================

// Diejenigen Felder von der PU Einheiten setzen, die immer gleich sind
void InitGenericPU(LPOBJEKT lpObjekt)
{
    lpObjekt->groups[0] = GD_ENEMY;
    lpObjekt->energie = 1;
    lpObjekt->schaden = 0;
    lpObjekt->kollision = true;
    lpObjekt->s = 0;
    lpObjekt->LifeTime = 10000;
    return;
}

//-----------Initialisiert die Einheiten-----------------------------------------------------------
void InitUnit(LPOBJEKT lpObjekt)
{
    // Standard-Eigenschafen festlegen (Werden nachher je nach Objekt geändert)
    lpObjekt->akt_bild = 0;
    lpObjekt->animiert = true;
    lpObjekt->AnimLifetime = ANIM_LIFETIME;
    lpObjekt->kollision = false;
    lpObjekt->LifeTime = 0;
    lpObjekt->zustand = OZ_GO;
    lpObjekt->timer = GetTickCount();
    lpObjekt->bild = NULL;

    switch (lpObjekt->typ)
    {
        //----G E G N E R-----
    case UD_PLAYERSHIP:
    {
        lpObjekt->bild = &SpriteArray[SD_PLAYERSHIP];
        lpObjekt->groups[0] = GD_SHIP;
        lpObjekt->akt_bild = 3;
        lpObjekt->animiert = false;
        lpObjekt->energie = 1000;
        lpObjekt->schaden = 50;
        lpObjekt->kollision = true;
        lpObjekt->s = 4.0f;
        lpObjekt->LifeTime = 10;
        lpObjekt->timer = GetTickCount();
        lpObjekt->x = (float)((float)RESOLUTION_X / 2.0);
        lpObjekt->y = float(ScrollY) + ((float(RESOLUTION_Y) - lpObjekt->bild->hoehe) - float(RESOLUTION_Y)/10.0);
        lpObjekt->v[0] = 0.0;
        lpObjekt->v[1] = 0.0;
        PlayerShipPosX = lpObjekt->x;
        PlayerShipPosY = lpObjekt->y;
        ZurueckNeigenTimer = GetTickCount();
        return;
    } //Ende UD_PLAYERSHIP
    case UD_STONE64:
    {
        lpObjekt->bild = &SpriteArray[SD_STONE64];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 50;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->s = 1.0;
        lpObjekt->v[0] = 0;
        lpObjekt->v[1] = lpObjekt->s;
        lpObjekt->LifeTime = 10000;
        return;
    } //Ende UD_STONE64
    case UD_MONSTER1A:
    {
        lpObjekt->bild = get_Sprite(lpObjekt->typ);
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 200;
        lpObjekt->schaden = 30;
        lpObjekt->kollision = true;
        lpObjekt->s = 1.0;
        AimAtPlayership(lpObjekt);
        InitVectorDiagonal(lpObjekt);
        lpObjekt->LifeTime = 10000;
        return;
    } //Ende UD_MONSTER1A
    case UD_MONSTER1B:
    {
        lpObjekt->bild = &SpriteArray[SD_MONSTER1B];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->zx = 300.0;
        lpObjekt->zy = ScrollY;
        lpObjekt->energie = 300;
        lpObjekt->schaden = 30;
        lpObjekt->kollision = true;
        lpObjekt->s = 2;
        AimAtPlayership(lpObjekt);
        InitVectorDiagonal(lpObjekt);
        lpObjekt->LifeTime = 10000;
        return;
    } //Ende UD_MONSTER1B
    case UD_MONSTER1C:
    {
        lpObjekt->bild = &SpriteArray[SD_MONSTER1C];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 400;
        lpObjekt->schaden = 40;
        lpObjekt->kollision = true;
        lpObjekt->s = 2.5;
        AimAtPlayership(lpObjekt);
        InitVectorDiagonal(lpObjekt);
        lpObjekt->LifeTime = 10000;
        return;
    } //Ende UD_MONSTER1C
    case UD_SCHUSS:
    {
        lpObjekt->bild = &SpriteArray[SD_SCHUSS];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 35;
        lpObjekt->kollision = true;
        lpObjekt->s = 3;
        InitVectorDiagonal(lpObjekt);
        lpObjekt->LifeTime = 2500;
        return;
    } //Ende UD_MONSTER1B
    case UD_EXTRA:
    {
        lpObjekt->bild = &SpriteArray[SD_EXTRA];
        lpObjekt->groups[0] = GD_MISC;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 0;
        lpObjekt->kollision = true;
        lpObjekt->s = 0;
        lpObjekt->LifeTime = 10000;
        return;
    } //Ende UD_EXTRA

    case UD_SPOREA:
    {
        lpObjekt->bild = &SpriteArray[SD_SPOREA];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 80;
        lpObjekt->schaden = 30;
        lpObjekt->kollision = true;
        lpObjekt->s = 1.5;
        lpObjekt->animiert = true;
        InitVectorDiagonal(lpObjekt);
        lpObjekt->LifeTime = 10000;
        return;
    } // Ende UD_SPOREA

    case UD_POD:
    {
        lpObjekt->bild = &SpriteArray[SD_POD];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 200;
        lpObjekt->schaden = 1;
        lpObjekt->kollision = true;
        lpObjekt->s = 0.5;
        lpObjekt->animiert = true;
        InitVectorDiagonal(lpObjekt);
        lpObjekt->LifeTime = 10000;
        return;
    } // Ende UD_POD

    case UD_RUSHER:
    {
        lpObjekt->bild = &SpriteArray[SD_RUSHER];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 100;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->s = 3.5;
        lpObjekt->v[0] = 0;
        lpObjekt->v[1] = lpObjekt->s;
        lpObjekt->zx = 0;
        lpObjekt->zy = 0;
        lpObjekt->animiert = true;
        lpObjekt->LifeTime = 10000;
        return;
    }

    case UD_SASTER96:
    {
        lpObjekt->bild = &SpriteArray[SD_SASTER96];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 100;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->s = 0.2;
        lpObjekt->v[0] = lpObjekt->zx*lpObjekt->s;
        lpObjekt->v[1] = lpObjekt->zy*lpObjekt->s;
        lpObjekt->zx = 0;
        lpObjekt->zy = 0;
        lpObjekt->animiert = true;
        lpObjekt->LifeTime = 30000;
        return;
    }

    case UD_SASTER64:
    {
        // Feldmissbrauch: zx/zy werden durchgereicht als Kompenenten des Geschw. Vektors
        lpObjekt->bild = &SpriteArray[SD_SASTER64];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 200;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->s = 0.5;
        lpObjekt->v[0] = lpObjekt->zx*lpObjekt->s;
        lpObjekt->v[1] = lpObjekt->zy*lpObjekt->s;
        lpObjekt->zx = 0;
        lpObjekt->zy = 0;
        lpObjekt->animiert = true;
        lpObjekt->LifeTime = 10000;
        return;
    }

    case UD_SASTER32:
    {
        // Feldmissbrauch: zx/zy werden durchgereicht als Kompenenten des Geschw. Vektors
        lpObjekt->bild = &SpriteArray[SD_SASTER32];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 100;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->s = 0.7;
        lpObjekt->v[0] = lpObjekt->zx*lpObjekt->s;
        lpObjekt->v[1] = lpObjekt->zy*lpObjekt->s;
        lpObjekt->zx = 0;
        lpObjekt->zy = 0;
        lpObjekt->animiert = true;
        lpObjekt->LifeTime = 10000;
        return;
    }

    case UD_GASTER96:
    {
        lpObjekt->bild = &SpriteArray[SD_GASTER96];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 100;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->s = 0.5;
        lpObjekt->v[0] = lpObjekt->zx*lpObjekt->s;
        lpObjekt->v[1] = lpObjekt->zy*lpObjekt->s;
        lpObjekt->zx = 0;
        lpObjekt->zy = 0;
        lpObjekt->animiert = true;
        lpObjekt->LifeTime = 10000;
        return;
    }

    case UD_MASTER32:
    {
        // Feldmissbrauch: zx/zy werden durchgereicht als Kompenenten des Geschw. Vektors
        lpObjekt->bild = &SpriteArray[SD_MASTER32];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 50;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->s = 1.0;
        lpObjekt->v[0] = lpObjekt->zx*lpObjekt->s;
        lpObjekt->v[1] = lpObjekt->zy*lpObjekt->s;
        lpObjekt->zx = 0;
        lpObjekt->zy = 0;
        lpObjekt->animiert = true;
        lpObjekt->LifeTime = 10000;
        return;
    }

    case UD_MASTER64:
    {
        // Feldmissbrauch: zx/zy werden durchgereicht als Kompenenten des Geschw. Vektors
        lpObjekt->bild = &SpriteArray[SD_MASTER64];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 100;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->s = 0.75;
        lpObjekt->v[0] = lpObjekt->zx*lpObjekt->s;
        lpObjekt->v[1] = lpObjekt->zy*lpObjekt->s;
        lpObjekt->zx = 0;
        lpObjekt->zy = 0;
        lpObjekt->animiert = true;
        lpObjekt->LifeTime = 10000;
        return;
    }

    case UD_MASTER96:
    {
        lpObjekt->bild = &SpriteArray[SD_MASTER96];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 50;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->s = 0.5;
        lpObjekt->v[0] = lpObjekt->zx*lpObjekt->s;
        lpObjekt->v[1] = lpObjekt->zy*lpObjekt->s;
        lpObjekt->zx = 0;
        lpObjekt->zy = 0;
        lpObjekt->animiert = true;
        lpObjekt->LifeTime = 10000;
        return;
    }

    case UD_GSHOOTL:
    case UD_GSHOOTR:
    case UD_WALLHR:
    case UD_WALLHL:
    {
        if (lpObjekt->typ == UD_WALLHL)
        {
            lpObjekt->bild = &SpriteArray[SD_WALLHUGGERL];
        }
        if (lpObjekt->typ == UD_WALLHR)
        {
            lpObjekt->bild = &SpriteArray[SD_WALLHUGGERR];
        }
        if (lpObjekt->typ == UD_GSHOOTL)
        {
            lpObjekt->bild = &SpriteArray[SD_GSHOOTL];
        }
        if (lpObjekt->typ == UD_GSHOOTR)
        {
            lpObjekt->bild = &SpriteArray[SD_GSHOOTR];
        }
        // Feldmissbrauch: zx = obere Ziel-y-Koordinate; zy = untere Ziel-y-Koordinate
        static float biasup = 50.0, biasdown = 50.0;
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->AnimLifetime = (DWORD)200.0;
        lpObjekt->zx = lpObjekt->y - biasup;
        lpObjekt->zy = lpObjekt->y + biasdown;
        lpObjekt->s = 2.0;
        lpObjekt->v[0] = 0;
        lpObjekt->v[1] = lpObjekt->s;
        lpObjekt->energie = 100;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->animiert = true;
        lpObjekt->LifeTime = 10000;
        return;
    }

    case UD_HOMING:
    {
        lpObjekt->bild = &SpriteArray[SD_HOMING];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->energie = 300;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->s = 0.0;
        lpObjekt->zx = 0;
        lpObjekt->zy = 0;
        lpObjekt->v[0] = 0.0;
        lpObjekt->v[1] = 0.0;
        lpObjekt->animiert = true;
        lpObjekt->LifeTime = 10000;
        return;
    }

    case UD_HOMPROJ:
    {
        lpObjekt->bild = &SpriteArray[SD_HOMPROJ];
        lpObjekt->groups[0] = GD_ENEMY;
        lpObjekt->s = 3.0;
        lpObjekt->zx = lpObjekt->s*lpObjekt->zx;        // Feldmissbrauch: zx, zy werden genutzt, um
        lpObjekt->zy = lpObjekt->s*lpObjekt->zy;        // Geschw. Vektor zu uebergeben
        lpObjekt->v[0] = lpObjekt->zx;                  // Feldmissbrauch: zx, zy werden genutzt, um
        lpObjekt->v[1] = lpObjekt->zy;                  // Geschw. Vektor zu uebergeben
        lpObjekt->akt_bild = get_akt_bild_HRocket(lpObjekt); // Korrektes Bild einstellen
        lpObjekt->animiert = false;
        lpObjekt->energie = 300;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->LifeTime = 10000;
        return;
    }

    //-----B L O C K-S--------
    case UD_BLOCK1:
    {
        lpObjekt->bild = &SpriteArray[SD_BLOCK1];
        lpObjekt->groups[0] = GD_BLOCK;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 1000;
        lpObjekt->kollision = true;
        lpObjekt->s = 0;
        lpObjekt->animiert = false;
        lpObjekt->LifeTime = 20000;
        return;
    } // Ende UD_BLOCK1

    // Obsolet: Sollte Tile sein!
    case UD_BACK64A:
    {
        lpObjekt->bild = &SpriteArray[SD_BACK64A];
        lpObjekt->groups[0] = GD_BLOCK;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 0;
        lpObjekt->kollision = false;
        lpObjekt->s = 0;
        lpObjekt->animiert = false;
        lpObjekt->LifeTime = 20000;
        lpObjekt->parallax_ratio = 1.5;
        return;
    } // Ende UD_BLOCK1

    //-----E F F E K T E--------------
    case UD_EXP16:
    case UD_EXP32:
    case UD_EXP64:
    {
        switch (lpObjekt->typ)
        {
        case UD_EXP16:
        {
            lpObjekt->bild = &SpriteArray[SD_EXP16];
            break;
        }
        case UD_EXP32:
        {
            lpObjekt->bild = &SpriteArray[SD_EXP32];
            break;
        }
        case UD_EXP64:
        {
            lpObjekt->bild = &SpriteArray[SD_EXP64];
            break;
        }
        }
        lpObjekt->groups[0] = GD_MISC;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 0;
        lpObjekt->kollision = false;
        lpObjekt->s = 0;
        lpObjekt->LifeTime = 500;
        return;
    } //Ende UD_EXP64

    case UD_DUST:
    {
        lpObjekt->bild = &SpriteArray[SD_DUST];
        lpObjekt->groups[0] = GD_MISC;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 0;
        lpObjekt->kollision = false;
        // Zuefällige Geschwindigkeit zw. 0 und 5
        float r = (float)(rand()%5000);
        lpObjekt->s = 0.001*r;
        lpObjekt->animiert = true;
        lpObjekt->LifeTime = 10000;
        // Zufälligen Geschwindigkeitsvektor für das Partikel generieren (In Bogenmass)
        int	  alpha_grad = rand()%360;					// Zufälliger Winkel zw. 0°-360°
        float alpha_rad = (float)(Pi() * alpha_grad)/180;	// Umrechnung von Grad in Radian
        lpObjekt->v[0] = lpObjekt->s * cos(alpha_rad);
        lpObjekt->v[1] = lpObjekt->s * sin(alpha_rad);
        return;
    } // Ende UD_DUST

    //-----W A F F E N--------
    case UD_STDROCKET:
    {
        lpObjekt->bild = &SpriteArray[SD_STDROCKET];
        lpObjekt->groups[0] = GD_SHIP;
        lpObjekt->akt_bild = 1;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 50;
        lpObjekt->kollision = true;
        lpObjekt->s = 8;
        lpObjekt->LifeTime = 1900;
        return;
    } //Ende UD_STDROCKET
    case UD_DUALROCKET:
    {
        lpObjekt->bild = &SpriteArray[SD_DUALROCKET];
        lpObjekt->groups[0] = GD_SHIP;
        lpObjekt->akt_bild = 1;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->s = 8;
        lpObjekt->LifeTime = 1900;
        return;
    }
    case UD_DHROCKET:
    {
        lpObjekt->bild = &SpriteArray[SD_DHROCKET];
        lpObjekt->groups[0] = GD_SHIP;
        lpObjekt->akt_bild = 1;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 40;
        lpObjekt->kollision = true;
        lpObjekt->s = 8.0;
        lpObjekt->LifeTime = 10000;
        GetTargetVector(*lpObjekt, lpObjekt->s);
        //InitVectorDiagonal(lpObjekt);
        return;
    }
    case UD_HROCKET:
    {
        /*char buf[128];
        float v[2] = {0,1};
        float alpha = angle_360_cw(v);
        sprintf(buf, "%f", alpha);
        MessageBox(NULL, buf, "test", NULL);*/
        lpObjekt->bild = &SpriteArray[SD_HROCKET];
        lpObjekt->groups[0] = GD_SHIP;
        lpObjekt->akt_bild = 0;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 50;
        lpObjekt->kollision = true;
        lpObjekt->s = 6;
        lpObjekt->LifeTime = 1900;
        lpObjekt->animiert = false;
        lpObjekt->v[0] = 0.0 * lpObjekt->s;
        lpObjekt->v[1] = -1.0 * lpObjekt->s;
        lpObjekt->zy = float(GetTickCount());                       // Feldmissbrauch: zy ist der Update-Timer für TargetDeltaAngle
        lpObjekt->zx = GetTargetDeltaAngle(*lpObjekt, lpObjekt->s); // Feldmissbrauch: zx ist Zwischenwinkel zwischen
        // momentanem Vel.-Vector und Ziel-Vel.Vector
        lpObjekt->akt_bild = get_akt_bild_HRocket(lpObjekt);
        return;
    }
    case UD_TRROCKETCOMP:               // Dummy-Objekt, das drei TripleMissile's erstellt
    {
        lpObjekt->bild = &SpriteArray[SD_DUMMY];
        lpObjekt->groups[0] = GD_SHIP;
        lpObjekt->animiert = false;
        lpObjekt->kollision = false;
        lpObjekt->s = 8;
        float dx = 24;                      // Abstand der Raketen zueinander
        float vx = 1;                       // x-Komponente der äusseren Raketen
        CreateObjekt(lpObjekt->x - dx, lpObjekt->y, -vx, -sqrt(lpObjekt->s*lpObjekt->s-vx*vx), UD_TRROCKET);
        CreateObjekt(lpObjekt->x + 0.0, lpObjekt->y, 0, -lpObjekt->s, UD_TRROCKET);
        CreateObjekt(lpObjekt->x + dx, lpObjekt->y, vx, -sqrt(lpObjekt->s*lpObjekt->s-vx*vx), UD_TRROCKET);
        lpObjekt->zustand = OZ_DEAD;        // Dieses Dummy Objekt zerstören
        return;
    }
    case UD_TRROCKET:
    {
        // Argumentmissbrauch: zx, zy sind als Geschw.-Komponenten des Objekts zu interpretieren
        lpObjekt->bild = &SpriteArray[SD_TRROCKET];
        lpObjekt->groups[0] = GD_SHIP;
        lpObjekt->akt_bild = 1;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->s = 8;
        lpObjekt->v[0] = lpObjekt->zx;
        lpObjekt->v[1] = lpObjekt->zy;
        lpObjekt->LifeTime = 10000;
        return;
    }

    case UD_LASERCOMP:               // Dummy-Objekt, das drei Laserteile erstellt
    {
        lpObjekt->bild = &SpriteArray[SD_DUMMY];
        lpObjekt->groups[0] = GD_SHIP;
        lpObjekt->animiert = false;
        lpObjekt->kollision = false;
        lpObjekt->s = 8;
        float dy = 50.0;                      // Abstand der Laser zueinander
        float dx = 6.0;
        /*        CreateObjekt(lpObjekt->x + dx, lpObjekt->y - dy, 0, 0, UD_LASER1);
                CreateObjekt(lpObjekt->x + dx, lpObjekt->y - 2*dy, 0, 0, UD_LASER2);
                CreateObjekt(lpObjekt->x + dx, lpObjekt->y - 3*dy, 0, 0, UD_LASER3);*/
        CreateObjekt(lpObjekt->x + dx, lpObjekt->y - dy, 0, 0, UD_LASER4);
        lpObjekt->zustand = OZ_DEAD;        // Dieses Dummy Objekt zerstören
        //     MessageBox(NULL, "foo", "foo", NULL);
        return;
    }

    case UD_LASER1:
    case UD_LASER2:
    case UD_LASER3:
    {
        if (lpObjekt->typ == UD_LASER1)
            lpObjekt->bild = &SpriteArray[SD_LASER1];
        if (lpObjekt->typ == UD_LASER2)
            lpObjekt->bild = &SpriteArray[SD_LASER2];
        if (lpObjekt->typ == UD_LASER3)
            lpObjekt->bild = &SpriteArray[SD_LASER1];
        // TODO:
        // Wenn UD_LASER3 als Sprite SD_LASER3 bekommt, stuerzt das Programm ab..
        lpObjekt->groups[0] = GD_SHIP;
        lpObjekt->akt_bild = 1;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->LifeTime = 105;
        return;
    }
    case UD_LASER4:
    {
        lpObjekt->bild = &SpriteArray[SD_LASER1];
        lpObjekt->groups[0] = GD_SHIP;
        lpObjekt->akt_bild = 1;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 100;
        lpObjekt->kollision = true;
        lpObjekt->v[0] = 0.0;
        lpObjekt->v[1] = -30.0; // 30.0
        lpObjekt->LifeTime = 1500;
        return;
    }

    //-----D R O N E N / S H I E L D S--------------
    case UD_DRONE:
    {
        lpObjekt->bild = &SpriteArray[SD_DRONE];
        lpObjekt->groups[0] = GD_SHIP;
        lpObjekt->energie = 200;
        lpObjekt->schaden = 50;
        lpObjekt->kollision = true;
        lpObjekt->v[0] = 0.0;               // Winkel der Drone
        lpObjekt->v[1] = 0;                 // Radius der Drone
        lpObjekt->s = 0.0005;               // Winkelgeschwindigkeit der Drone
        lpObjekt->LifeTime = 10000;
        SetDroneSpawnPos(lpObjekt);
        UpdateDrone(lpObjekt);
        // Feldmissbrauch: zx ist als Schusstimer für die Drone zu interpretieren
        lpObjekt->zx = 100;
        return;
    }

    case UD_SHIELD1:
    {
        // Feldmissbrauch: zx wird beim Übergeben von CreateObjekt als Startwinkel betrachtet
        lpObjekt->bild = &SpriteArray[SD_SHIELD1];
        lpObjekt->groups[0] = GD_SHIP;
        lpObjekt->energie = 1;
        lpObjekt->schaden = 400;
        lpObjekt->kollision = true;
        lpObjekt->v[0] = 0.0;               // Winkel des Shields
        lpObjekt->v[1] = 0;                 // Radius des Shields
        lpObjekt->s = 0.0010;               // Winkelgeschwindigkeit der Drone
        lpObjekt->LifeTime = 10000;
        SetShieldSpawnPos(lpObjekt);
        UpdateShield(lpObjekt);
        return;
    }

    case UD_PUDIVE:
    {
        lpObjekt->bild = &SpriteArray[SD_PUDIVE];
        InitGenericPU(lpObjekt);
        return;
    }
    case UD_PUINVULN:
    {
        lpObjekt->bild = &SpriteArray[SD_PUINVULN];
        InitGenericPU(lpObjekt);
        return;
    }
    case UD_PULASER:
    {
        lpObjekt->bild = &SpriteArray[SD_PULASER];
        InitGenericPU(lpObjekt);
        return;
    }
    case UD_PUDUALROCKET:
    {
        lpObjekt->bild = &SpriteArray[SD_PUWEAPON];
        InitGenericPU(lpObjekt);
        return;
    }
    case UD_PUARMOR:
    {
        lpObjekt->bild = &SpriteArray[SD_PUARMOR];
        InitGenericPU(lpObjekt);
        return;
    }
    case UD_PUSPEED:
    {
        lpObjekt->bild = &SpriteArray[SD_PUSPEED];
        InitGenericPU(lpObjekt);
        return;
    }
    case UD_PULIFE:
    {
        lpObjekt->bild = &SpriteArray[SD_PULIFE];
        InitGenericPU(lpObjekt);
        return;
    }
    case UD_PUHROCKET:
    {
        lpObjekt->bild = &SpriteArray[SD_PUHROCKET];
        InitGenericPU(lpObjekt);
        return;
    }

    default:
    {
        return;
    }
    } //Ende switch(lpObjekt->typ)
}

//----------------------------------------------------------------------------------------------
void BrainStone(LPOBJEKT objekt)
{
    objekt->y += objekt->v[1];
    objekt->LifeTime -= dwFrameTime;
}

//----------------------------------------------------------------------------------------------
void BrainWallhugger(LPOBJEKT lpObjekt)
{
    // Richtung umkehren, falls noetig
    if (lpObjekt->y < lpObjekt->zx)             // Feldmissbrauch: xz ist untere Bewegungsgrenze fuer Wallhugger
    {
        lpObjekt->s = fabs(lpObjekt->s);
    }
    if (lpObjekt->y > lpObjekt->zy)
    {
        lpObjekt->s = -fabs(lpObjekt->s);
    }

    // Schuesse erzeugen in Intervallen
    if (lpObjekt->akt_bild == lpObjekt->bild->anz_anim)
    {
        char buf[128];
        lpObjekt->akt_bild = 1;
        sprintf(buf, "%d %d", lpObjekt->akt_bild, lpObjekt->bild->anz_anim);
        float y = lpObjekt->y + (lpObjekt->bild->hoehe/2.0);
        if (lpObjekt->typ == UD_WALLHL or lpObjekt->typ == UD_GSHOOTL)
        {
            float x = lpObjekt->x + lpObjekt->bild->breite;
            CreateObjekt(x, y, x+RESOLUTION_X, y, UD_SCHUSS);
        }
        if (lpObjekt->typ == UD_WALLHR or lpObjekt->typ == UD_GSHOOTR)
        {
            static float shotwidth = 32.0;
            float x = lpObjekt->x - shotwidth;
            CreateObjekt(x, y, x-RESOLUTION_X, y, UD_SCHUSS);
        }
    }

    lpObjekt->v[1] = lpObjekt->s;

    if (lpObjekt->typ == UD_WALLHL or lpObjekt->typ == UD_WALLHR)
    {
        MoveDiagonal(lpObjekt);
    }

    return;
}

//----------------------------------------------------------------------------------------------
void BrainPU(LPOBJEKT lpObjekt)
{
    // Prüfen, ob das Spielerschiff dieses Objekt berührt hat
    bool bHit = false;
    if (((PlayerShipPosX + 64) > lpObjekt->x) && (PlayerShipPosX < (lpObjekt->x + 32)) &&
            ((PlayerShipPosY + 64) > lpObjekt->y) && (PlayerShipPosY < (lpObjekt->y + 32)))
    {
        bHit = true;
        lpObjekt->zustand = OZ_DEAD;
    }

    // Falls Beruehrung: Aktion ausfuehren
    if (bHit == true)
    {
        switch (lpObjekt->typ)
        {
        case UD_EXTRA:
        {
            Score += 2000;
            return;
        }
        /*           case UD_PU:
                   {
                       ChosenWeaponType = UD_DHROCKET;
                       WeaponShotSpeed = RT_DHROCKET;
                       return;
                   }*/
        case UD_PUARMOR:
        {
            int nshields = 8;
            float dphi = 2*Pi()/((float)nshields);
            for (int i = 0; i < nshields; i++)
            {
                CreateObjekt(0, 0, dphi*(float)i, 0, UD_SHIELD1);
            }
            return;
        }
        case UD_PUHROCKET:
        {
            ChosenWeaponType = UD_HROCKET;
            WeaponShotSpeed = RT_HROCKET;
            return;
        }
        case UD_PUDUALROCKET:
        {
            ChosenWeaponType = UD_DUALROCKET;
            WeaponShotSpeed = RT_DUALROCKET;
            return;
        }
        case UD_PULASER:
        {
            ChosenWeaponType = UD_LASERCOMP;
            WeaponShotSpeed = RT_LASERCOMP;
            return;
        }
        case UD_PUINVULN:
        {
            PlayerShipState = 1;
            return;
        }
        case UD_PUDIVE:
        {
            PlayerShipState = 2;
            return;
        }
        }
    }

    lpObjekt->LifeTime -= dwFrameTime;
    return;
}

//----------------------------------------------------------------------------------------------
void BrainMonster(LPOBJEKT objekt, int ammo_type)
{
    if (objekt->zustand == OZ_GO)
        MoveDiagonal(objekt);
    // Mit einer kleinen Wahrscheinlichkeit einen Schuss erzeugen
    if (ammo_type != 0 and (rand()%200)==1)
        CreateObjekt(objekt->x + 32, objekt->y + 65, PlayerShipPosX, PlayerShipPosY - 100, ammo_type);
    objekt->LifeTime -= dwFrameTime;
}

//----------------------------------------------------------------------------------------------
void BrainGeneric(LPOBJEKT objekt, int ammo_type)
{
    if (objekt->zustand == OZ_GO)
    {
        objekt->x += objekt->v[0];
        objekt->y += objekt->v[1];
    }
    objekt->LifeTime -= dwFrameTime;
}

//----------- foo ------------------------------------------------------------------------------
void CreatePlayerShipRocket(void)
{
    // Eine Rakete abschiessen kostet Punkte
    Score -= 20;
    if (Score < 0)
        Score = 0;
    // Rakete mit Sound erstellen
    CreateObjekt(PlayerShipPosX + 24, PlayerShipPosY - 18, 0, 0, ChosenWeaponType);
    iRocketTimer = 0;
//    Am2.PlaySound("snd/sound.wav");
}

//----------Berechne aktuelles Bild fuer HRocket aus Geschw.-Vektor-----------------------------
int get_akt_bild_HRocket(LPOBJEKT objekt)
{
    float v2[2] = {objekt->v[0], objekt->v[1]};
    v2[1] = -v2[1];
    float phi = angle_360_cw(v2);
    float z = phi+((5.0/8.0)*Pi());
    float n = (2.0/8.0)*Pi();
    float rat = floor(z/n);
    int num = ((int)rat)%8;
//    char buffer[128]; sprintf(buffer, "%d %f", num, phi);
//    MessageBox(NULL, buffer, " ", NULL);
    return num;
}

//----------Die KI der Einheiten----------------------------------------------------------------
void UnitKI(LPOBJEKT lpObjekt)
{
    // Erst prüfen, ob Objekt zerstört wurde, falls ja->Funktion abbrechen
    if (CheckIfDestroyed(lpObjekt) == true)
        return;
    // Je nach Objekttyp KI aufrufen
    switch (lpObjekt->typ)
    {
        // Das Schiff des Spielers
    case UD_PLAYERSHIP:
    {
        lpObjekt->energie = 1000;

        // Korrektes Sprite einstellen
        switch (PlayerShipState)
        {
            static DWORD divetimer = 0;
        case 0:
        {
            lpObjekt->bild = &SpriteArray[SD_PLAYERSHIP];
            lpObjekt->kollision = true;
            break;
        }
        case 1:
        {
            static DWORD invulntimer = GetTickCount();
            lpObjekt->bild = &SpriteArray[SD_PSHIPINVULN];
            lpObjekt->kollision = false;
            if ((GetTickCount() - invulntimer) >= 2000)
            {
                lpObjekt->akt_bild = 4;
                PlayerShipState = 0;
                invulntimer = GetTickCount();
            }
            break;
        }
        case 2:
        {
            lpObjekt->bild = &SpriteArray[SD_PSHIPDIVING];
            lpObjekt->animiert = true;
            lpObjekt->kollision = false;
            if (lpObjekt->akt_bild >= lpObjekt->bild->anz_anim)
            {
                PlayerShipState = 3;
                divetimer = GetTickCount();
                //    MessageBox(NULL, "","",NULL);
            }
            break;
        }
        case 3:
        {
            lpObjekt->bild = &SpriteArray[SD_PSHIPDIVED];
            lpObjekt->animiert = false;
            lpObjekt->kollision = false;
            lpObjekt->akt_bild = 0;
            char buf[128];
            sprintf(buf, "%d", lpObjekt->akt_bild);
            if ((GetTickCount() - divetimer) >= 2000)
            {
                lpObjekt->akt_bild = 4;
                PlayerShipState = 0;
            }
            //            MessageBox(NULL, buf, "foo", NULL);
            break;
        }
        }

        // Aktuelle Position und Energie des PlayerShip in externe Variabeln speichern
        PlayerShipPosX = lpObjekt->x;
        PlayerShipPosY = lpObjekt->y;
        PlayerShipEnergy = lpObjekt->energie;
        PlayerShipvx = lpObjekt->v[0];
        PlayerShipvy = lpObjekt->v[1];

        // Das Schiff mit dem Vektor des vorigen Frames bewegen
        MoveDiagonal(lpObjekt);

        // Den Bewegungsvektor verkleinern (Schiff wird langsamer, wenn nicht beschleunigt)
        if (VectorLength(lpObjekt->v) > 0)
        {
            lpObjekt->v[0] = lpObjekt->v[0]*0.8;
            lpObjekt->v[1] = lpObjekt->v[1]*0.9;
        }

        // Schiff nach links bewegen
        if (GetAsyncKeyState(VK_LEFT))
        {
            lpObjekt->v[0] -= 1;
            if (GetTickCount() - lpObjekt->timer > 75 && lpObjekt->akt_bild > 0)
            {
                lpObjekt->akt_bild--;
                lpObjekt->timer = GetTickCount();
            }
        }
        // Schiff nach rechts bewegen
        if (GetAsyncKeyState(VK_RIGHT))
        {
            lpObjekt->v[0] += 1;
            if (GetTickCount() - lpObjekt->timer > 75 && lpObjekt->akt_bild < lpObjekt->bild->anz_anim)
            {
                lpObjekt->akt_bild++;
                lpObjekt->timer = GetTickCount();
            }
        }
        // Schiff nach oben bewegen
        if (GetAsyncKeyState(VK_UP))
        {
            lpObjekt->v[1] -= 0.5f;
        }
        // Schiff nach unten bewegen
        if (GetAsyncKeyState(VK_DOWN))
        {
            lpObjekt->v[1] += 0.5f;
        }

        // Prüfen, ob der Bewegungsvektor zu lang ist und ggf. auf maximale Länge kürzen
        if (VectorLength(lpObjekt->v) >= lpObjekt->s)
        {
            lpObjekt->v[0] = ((lpObjekt->v[0])/(VectorLength(lpObjekt->v)))*lpObjekt->s;
            lpObjekt->v[1] = ((lpObjekt->v[1])/(VectorLength(lpObjekt->v)))*lpObjekt->s;
        }

        // Prüfen, ob Schiff nicht mehr im Bildschirm-Bereich und korrigieren falls nötig
        // Linker Rand
        if (lpObjekt->x < 0)
            lpObjekt->x = 0;
        // Rechter Rand
        if ((lpObjekt->x + lpObjekt->bild->breite) > RESOLUTION_X)
            lpObjekt->x = (float)(RESOLUTION_X - lpObjekt->bild->breite);
        // Oberer Rand
        if ((lpObjekt->y) < (ScrollY))
            lpObjekt->y = ScrollY;
        // Unterer Rand
        if ((lpObjekt->y + lpObjekt->bild->hoehe) > (ScrollY + RESOLUTION_Y))
            lpObjekt->y = ((float)(ScrollY + RESOLUTION_Y)) - (lpObjekt->bild->hoehe);
        // Position etwas nach oben setzen (Raumschiff bewegt sich nach oben mit dem Scrolling)
        lpObjekt->y -= 1;

        // Langsames zurückneigen des Schiffes, wenn es sich nicht nach links/rechts bewegt
        if (abs((int)lpObjekt->v[0]) < 0.2) // Zurückneigen nur, falls fast keine Bewegung in X-Richtung
        {
            if ((GetTickCount() - ZurueckNeigenTimer) > 100)
            {
                if (lpObjekt->akt_bild < 3)
                    lpObjekt->akt_bild++;
                if (lpObjekt->akt_bild > 3)
                    lpObjekt->akt_bild--;
                ZurueckNeigenTimer = GetTickCount();
            }
        }

        /*        // Rakete erstellen, falls CTRL gedrückt und genug Zeit seit letzer Rakete verstrichen
                iRocketTimer += fFrameTime;
                if (iRocketTimer > WeaponShotSpeed)
                    iRocketTimer = WeaponShotSpeed;
                if (GetAsyncKeyState(VK_CONTROL) && iRocketTimer >= WeaponShotSpeed)
                {
                    // Eine Rakete abschiessen kostet Punkte
                    Score -= 20;
                    if (Score < 0)
                        Score = 0;
                    // Rakete mit Sound erstellen
                    CreateObjekt(lpObjekt->x + 24, lpObjekt->y - 18, 0, 0, ChosenWeaponType);
                    iRocketTimer = 0;
        //            PlayAudio("snd/sound.wav", false);
                    Am2.PlaySound("snd/sound.wav");
                }*/

        // TEMPORÄR: Wenn Enter gedrückt: Drone erzeugen
        static bool shield_exists = false;
        if (GetAsyncKeyState(VK_RETURN) and shield_exists == false)
        {
            ;
        }

        return;
    } // Ende UD_PLAYERSHIP

    // Ein Meteorit
    case UD_STONE64:
    {
        //BrainStone(lpObjekt);
        BrainStone(lpObjekt);
        return;
    } // Ende UD_STONE64

    // Das erste Monster (Ein Wurm)
    case UD_MONSTER1A:
    {
        BrainMonster(lpObjekt, 0);
        return;
    } // Ende UD_MONSTER1A

    // Das zweiter Monster (Eine Schnecke)
    case UD_MONSTER1B:
    {
        BrainMonster(lpObjekt, UD_SCHUSS);
        return;
    } // Ende UD_MONSTER1B

    // Das dritte Monster
    case UD_MONSTER1C:
    {
        BrainMonster(lpObjekt, UD_SCHUSS);
        return;
    } // Ende UD_MONSTER1B

    // Spore A
    case UD_SPOREA:
    {
        if (rand()%40==1)
        {
            lpObjekt->zx = PlayerShipPosX;
            lpObjekt->zy = PlayerShipPosY;
            InitVectorDiagonal(lpObjekt);
        }
        BrainMonster(lpObjekt, 0);
        return;
    }

    // Pod
    case UD_POD:
    {
        BrainMonster(lpObjekt, 0);
        return;
    }

    // Rusher
    case UD_RUSHER:
    {
        BrainGeneric(lpObjekt, 0);
        return;
    }

    case UD_SASTER32:
    case UD_SASTER64:
    case UD_SASTER96:
    case UD_MASTER32:
    case UD_MASTER64:
    case UD_MASTER96:
    case UD_GASTER32:
    case UD_GASTER64:
    case UD_GASTER96:
    {
        BrainGeneric(lpObjekt, 0);
        return;
    }

    case UD_GSHOOTL:
    case UD_GSHOOTR:
    case UD_WALLHR:
    case UD_WALLHL:
    {
        BrainWallhugger(lpObjekt);
        return;
    }

    case UD_HOMING:
    {
        BrainMonster(lpObjekt, 0);
        return;
    }

    case UD_HOMPROJ:
    {
        MoveDiagonal(lpObjekt);
        return;
    }

    // Der Schuss von Gegner Nr.2 (UD_MONSTER1B)
    case UD_SCHUSS:
    {
        if (lpObjekt->zustand == OZ_GO)
            MoveDiagonal(lpObjekt);
        lpObjekt->LifeTime -= dwFrameTime;
        return;
    } // Ende UD_SCHUSS

    // 2000 Extra-Punkte
    case UD_EXTRA:
    {
        // Prüfen, ob das Spielerschiff dieses Objekt berührt hat und wenn ja-->
        // Extra-Punkte berechnen
        if (((PlayerShipPosX + 64) > lpObjekt->x) && (PlayerShipPosX < (lpObjekt->x + 32)) &&
                ((PlayerShipPosY + 64) > lpObjekt->y) && (PlayerShipPosY < (lpObjekt->y + 32)))
        {
            Score += 2000;
            lpObjekt->zustand = OZ_DEAD;
        }
        lpObjekt->LifeTime -= dwFrameTime;
        return;
    } // Ende UD_EXTRA

    //------B L O C K S-------------------
    // Ein grosses Hindernis
    case UD_BLOCK1:
    {
        // Damit Objekt nie zerstört wird (+ um Variabel-Überläufe zu verhindern)
        lpObjekt->energie = 1;
        lpObjekt->LifeTime -= dwFrameTime;
        return;
    } //Ende UD_BLOCK1

    // Ein Stein im Hintergrund
    case UD_BACK64A:
    {
        // Damit Objekt nie zerstört wird (+ um Variabel-Überläufe zu verhindern)
        lpObjekt->energie = 1;
        lpObjekt->LifeTime -= dwFrameTime;
        return;
    } // Ende UD_BACK64A

    //------E F F E K T E-------------------------
    // Die verschiedenen Explosionen (Verschieden gross)
    case UD_EXP16:
    case UD_EXP32:
    case UD_EXP64:
    {
        lpObjekt->LifeTime -= dwFrameTime;
        return;
    } // Ende UD_EXP16/32/64

    case UD_PUDIVE:
    case UD_PUINVULN:
    case UD_PULASER:
    case UD_PUDUALROCKET:
    case UD_PUARMOR:
    case UD_PUSPEED:
    case UD_PULIFE:
    case UD_PUHROCKET:
    {
        BrainPU(lpObjekt);
        return;
    }

    case UD_DUST:
    {
        lpObjekt->LifeTime -= dwFrameTime;
        VECTOR2 g;                  // Gravitationsbeschleunigung
        g.x = 0.0f;
        g.y = 0.002f;
        VECTOR2 add;
        add.x = g.x*fFrameTime;
        add.y = g.y*fFrameTime;
        lpObjekt->v[0] += add.x;
        lpObjekt->v[1] += add.y;
        MoveDiagonal(lpObjekt);
        return;
    }

    //------W A F F E N-------------------
    // Die Standard-Rakete
    case UD_STDROCKET:
    {
        lpObjekt->y -= lpObjekt->s;
        lpObjekt->LifeTime -= dwFrameTime;
        return;
    } // Ende UD_STDROCKET
    // Homing Rakete
    case UD_DHROCKET:
    {
        MoveDiagonal(lpObjekt);
        lpObjekt->LifeTime -= dwFrameTime;
        return;
    } // Ende UD_STDROCKET
    // Dual Rakete
    case UD_DUALROCKET:
    {
        lpObjekt->y -= lpObjekt->s;
        lpObjekt->LifeTime -= dwFrameTime;
        return;
    }
    // Playership Homing Rakete
    case UD_HROCKET:
    {
        /*        static int infinity = 100000;
                static DWORD timer = GetTickCount();
                MoveDiagonal(lpObjekt);
                lpObjekt->LifeTime -= dwFrameTime;
                if(GetTickCount() - timer > infinity)
                {
                    GetTargetVector(*lpObjekt, lpObjekt->s);
                    //Richtiges Bild einstellen
                    lpObjekt->akt_bild = get_akt_bild_HRocket(lpObjekt);
                    timer = GetTickCount();
                }

                return;*/
        MoveDiagonal(lpObjekt);
        static DWORD update = 450;                      // Update-Intervall fuer TargetDeltaAngle
        lpObjekt->LifeTime -= dwFrameTime;
        if ((GetTickCount() - DWORD(lpObjekt->zy)) > update)
//        if(1 == 1)
        {
            lpObjekt->zx = GetTargetDeltaAngle(*lpObjekt, lpObjekt->s);
            lpObjekt->zy = float(GetTickCount());
        }
        static float omega = 0.0020;                    // Winkelgeschw. der Raketen
        static float tolerance = fFrameTime*2.0*omega;  // Minimaler Zwischenwinkel zx, bei dem noch rotiert werden soll
        float alpha = fFrameTime*omega*sgn(lpObjekt->zx); // Rotation in diesem Frame
        if (fabs(lpObjekt->zx) > tolerance)             // Feldmissbrauch: zx ist aktueller Zwischenwinkel
        {
            Rotate2D(lpObjekt->v, alpha, lpObjekt->v, lpObjekt->s);
            lpObjekt->akt_bild = get_akt_bild_HRocket(lpObjekt);
            lpObjekt->zx -= alpha;
        }
        return;
    }

    case UD_TRROCKET:
    {
        MoveDiagonal(lpObjekt);
        lpObjekt->LifeTime -= dwFrameTime;
        return;
    }

    case UD_LASER1:
    case UD_LASER2:
    case UD_LASER3:
    {
        static float infinity = 1000000;
        lpObjekt->energie = (int)infinity;
        lpObjekt->LifeTime -= dwFrameTime;
        lpObjekt->y -= 1.0;
        return;
    }
    case UD_LASER4:
    {
        MoveDiagonal(lpObjekt);
        lpObjekt->LifeTime -= dwFrameTime;
        lpObjekt->y -= 1.0;
        return;
    }

    //------D R O N E N / S H I E L D S--------------

    case UD_DRONE:
    {
        UpdateDrone(lpObjekt);
        lpObjekt->y -= 1;
        // HRocket erstellen, falls CTRL gedrückt und genug Zeit seit letzer Rakete verstrichen
        // Feldmissbrauch: zx ist als Schusstimer der Drone zu interpretieren
        lpObjekt->zx += fFrameTime;
        if (iRocketTimer > RT_DHROCKET)
            iRocketTimer = RT_DHROCKET;
        if (GetAsyncKeyState(VK_CONTROL) && lpObjekt->zx >= RT_DHROCKET)
        {
            // HRocket mit Sound erstellen
            CreateObjekt(lpObjekt->x + 24, lpObjekt->y - 36, 0, 0, UD_DHROCKET);
            lpObjekt->zx = 0;
//				PlaySoundOnce(SoundArray[1], 0, DSBVOLUME_MAX);
        }
        return;
    }

    case UD_SHIELD1:
    {
        /*        char buf[128];
                sprintf(buf, "%d", lpObjekt->energie);
                MessageBox(NULL, buf, "update", NULL);*/
        UpdateShield(lpObjekt);
        lpObjekt->y -= 1;

        return;
    }

    } // Ende switch(lpObjekt->typ)

} // Ende UnitKI()

//-----------Prüft, ob das Objekt zerstört(noch nicht gelöscht) wurde und erstellt Explosion------
bool CheckIfDestroyed(LPOBJEKT lpObjekt)
{
    if (lpObjekt->zustand == OZ_DESTROYED)
    {
        // Nicht bei jedem Objekt ist die Todesanimation gleich->Selektion
        switch (lpObjekt->typ)
        {
        case UD_PLAYERSHIP:
        {
            lpObjekt->zustand = OZ_DEAD;
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP64);
            SetProgControl(PC_SHOWDEATH);
            return true;
        }
        case UD_STONE64:
        {
            Score += 100*3;
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP64);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }
        case UD_MONSTER1A:
        {
            Score += 200*3;
//				CreateParticleSystem(lpObjekt->x, lpObjekt->y, 2);
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP32);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }
        case UD_MONSTER1B:
        {
            Score += 300*3;
//				CreateParticleSystem(lpObjekt->x, lpObjekt->y, 2);
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP32);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }
        case UD_MONSTER1C:
        {
            Score += 300*3;
//				CreateParticleSystem(lpObjekt->x, lpObjekt->y, 2);
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP32);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }
        case UD_POD:
        {
            Score += 400*3;
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP64);
            // Sporen erstellen
            float spore_height = 16, spore_width = 16;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    float bias = (float)(rand()%10 + 5);
                    float x = lpObjekt->x+i*(spore_width+bias);
                    float y = lpObjekt->y+j*(spore_height+bias);
                    CreateObjekt(x, y, PlayerShipPosX, PlayerShipPosY, UD_SPOREA);
                }
            }
            /*            for(int i = 0; i < 20; i++)
                        {
                            CreateObjekt(lpObjekt->x+lpObjekt->bild->breite/2.0, lpObjekt->y+lpObjekt->bild->hoehe/2.0, 0, 0, UD_DUST);
                        }*/
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }

        case UD_HOMING:
        {
            int nmissiles = 8;
            static float r = 40.0;
            float width = lpObjekt->bild->breite/2.0;
            float height = lpObjekt->bild->hoehe/2.0;
            float dphi = 2*Pi()/((float)nmissiles);
            for (int i = 0; i < nmissiles; i++)
            {
                float phi = dphi*(float)i;
                CreateObjekt(lpObjekt->x + cos(phi)*r + height, lpObjekt->y + sin(phi)*r + width, cos(phi), sin(phi), UD_HOMPROJ);
            }
            CreateObjekt(lpObjekt->zx, lpObjekt->zy, 0, 0, UD_EXP32);

            lpObjekt->zustand = OZ_DEAD;

            return true;
        }

        case UD_SPOREA:
        {
            Score += 10*3;
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP16);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }

        case UD_RUSHER:
        {
            Score += 20*3;
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP16);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }

        case UD_SASTER96:
        {
            Score += 20*3;
            float biasx = 64 + 10;                      // Verschiebung der erzeugten Steine zueinander
            float len = 1/sqrt(2.0);                   // Komponentenlängen für diag.-Vektoren
            float hwidth = lpObjekt->bild->breite/2.0, hheight = lpObjekt->bild->hoehe/2.0;
            CreateObjekt(lpObjekt->x + hwidth, lpObjekt->y + hheight, 0, 1.0, UD_SASTER64);
            CreateObjekt(lpObjekt->x + hwidth - biasx, lpObjekt->y + hheight, -len, len, UD_SASTER64);
            CreateObjekt(lpObjekt->x + hwidth + biasx, lpObjekt->y + hheight, len, len, UD_SASTER64);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }

        case UD_SASTER64:
        {
            Score += 20*3;
            float biasx = 32 + 10;                      // Verschiebung der erzeugten Steine zueinander
            float len = 1/sqrt(2.0);                    // Komponentenlängen für diag.-Vektoren
            float hwidth = lpObjekt->bild->breite/2.0, hheight = lpObjekt->bild->hoehe/2.0;
            CreateObjekt(lpObjekt->x + hwidth, lpObjekt->y + hheight, 0, 1.0, UD_SASTER32);
            CreateObjekt(lpObjekt->x + hwidth - biasx, lpObjekt->y + hheight, -len, len, UD_SASTER32);
            CreateObjekt(lpObjekt->x + hwidth + biasx, lpObjekt->y + hheight, len, len, UD_SASTER32);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }

        case UD_SASTER32:
        {
            Score += 20*3;
            // Staub erstellen
            for (int i = 0; i < 5; i++)
            {
                CreateObjekt(lpObjekt->x+lpObjekt->bild->breite/2.0, lpObjekt->y+lpObjekt->bild->hoehe/2.0, 0, 0, UD_DUST);
            }
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }

        case UD_MASTER96:
        {
            Score += 20*3;
            float biasx = 64 + 10;                  // Verschiebung der erzeugten Steine zueinander
            float len = 1/sqrt(2.0);                   // Komponentenlängen für diag.-Vektoren
            float hwidth = lpObjekt->bild->breite/2.0, hheight = lpObjekt->bild->hoehe/2.0;
            CreateObjekt(lpObjekt->x + hwidth, lpObjekt->y + hheight, 0, 1.0, UD_MASTER64);
            CreateObjekt(lpObjekt->x + hwidth - biasx, lpObjekt->y + hheight, -len, len, UD_MASTER64);
            CreateObjekt(lpObjekt->x + hwidth + biasx, lpObjekt->y + hheight, len, len, UD_MASTER64);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }

        case UD_MASTER64:
        {
            Score += 20*3;
            float biasx = 32 + 10;                      // Verschiebung der erzeugten Steine zueinander
            float len = 1/sqrt(2.0);                    // Komponentenlängen für diag.-Vektoren
            float hwidth = lpObjekt->bild->breite/2.0, hheight = lpObjekt->bild->hoehe/2.0;
            CreateObjekt(lpObjekt->x + hwidth, lpObjekt->y + hheight, 0, 1.0, UD_MASTER32);
            CreateObjekt(lpObjekt->x + hwidth - biasx, lpObjekt->y + hheight, -len, len, UD_MASTER32);
            CreateObjekt(lpObjekt->x + hwidth + biasx, lpObjekt->y + hheight, len, len, UD_MASTER32);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }

        case UD_MASTER32:
        {
            Score += 20*3;
            // Staub erstellen
            for (int i = 0; i < 5; i++)
            {
                CreateObjekt(lpObjekt->x+lpObjekt->bild->breite/2.0, lpObjekt->y+lpObjekt->bild->hoehe/2.0, 0, 0, UD_DUST);
            }
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }

        case UD_SCHUSS:
        {
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP16);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }
        case UD_STDROCKET:
        {
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP16);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }
        case UD_DHROCKET:
        {
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP16);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }
        case UD_DUALROCKET:
        {
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP16);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }
        case UD_TRROCKET:
        {
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP16);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }
        case UD_LASER1:
        case UD_LASER2:
        case UD_LASER3:
        {
            return false; // Objekt unzerstoerbr
        }
        case UD_DRONE:
        {
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP16);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }
        case UD_SHIELD1:
        {
            /*            char buf[128];
                        sprintf(buf, "%d", lpObjekt->energie);
                        MessageBox(NULL, buf, "foo", NULL);*/
            CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP16);
            lpObjekt->zustand = OZ_DEAD;
            return true;
        }
        default:
        {
            return false;
        }
        }
    }
    return false;
}

//-------------Berechnet den Geschwindigkeitsvektor von der aktuellen Position zur Zielposition----
void InitVectorDiagonal(LPOBJEKT lpObjekt)
{
    float vStrich[2];							// Vektor von (x,y) zu (zx, zy) [Vektor v-"strich"]
    float BetragvStrich;						// |vStrich|
    float a;									// Quotient der Beträge |vStrich| und |lpObjekt->v|

    vStrich[0] = lpObjekt->zx - lpObjekt->x;	// Der Vektor von der Objekt-Position
    vStrich[1] = lpObjekt->zy - lpObjekt->y;	// zur Zielposition (Hat noch die falsche Länge)

    // Betrag von Vector1
    BetragvStrich = sqrt((vStrich[0]*vStrich[0])+(vStrich[1]*vStrich[1]));
    // Quotient berechnen (|lpObjekt->Vector| = lpObjekt->s)
    a = (lpObjekt->s)/BetragvStrich;
    // Komponenten von Vector2 berechnen
    lpObjekt->v[0] = a * vStrich[0];
    lpObjekt->v[1] = a * vStrich[1];

}

//-----------Führt die diagonale Bewegung aus-------------------------------------------------------
void MoveDiagonal(LPOBJEKT lpObjekt)
{
    lpObjekt->x += lpObjekt->v[0];
    lpObjekt->y += lpObjekt->v[1];

    return;
}

//-----------Berechnet Geschw. Vektor, damit bullet target mit Geschw. v0 trifft--------------------
VECTOR2 AimTarget(OBJEKT &bullet, OBJEKT &target, double v0)
{
    float theight = target.bild->hoehe;
    float twidth  = target.bild->breite;
    float bheight = bullet.bild->hoehe;
    float bwidth  = bullet.bild->breite;
    double th = 0;
    VECTOR2 NullVec;
    NullVec.x = 0;
    NullVec.y = 0;
    VECTOR2 vt, pt;
    vt.x = target.v[0];
    vt.y = target.v[1];
    pt.x = target.x+(twidth/2.0-bwidth/2.0)-bullet.x;
    pt.y = target.y+(theight/2.0-bheight/2.0)-bullet.y;

    double disc = 4*dotP(pt, pt)*dotP(vt, vt)+4*v0*v0*dotP(pt, pt);
    if (disc < 0)
        return NullVec;
    double num1 = 2*dotP(pt, vt)+sqrt(disc);
    double den1 = 2*v0*v0;
    double num2 = 2*dotP(pt, vt)-sqrt(disc);
    double den2 = den1;
    double res1 = num1/den1;
    double res2 = num2/den2;
    th = -1.0;
    if (res1 > 0)
        th = res1;
    if (res2 > 0 and res2 < res1)
        th = res2;
    if (th < 0)
        return NullVec;
    //VECTOR2 ph = VecAdd(pt, ScalarMult(th, vt));
    //ph = ScalarMult(1.0/th, ph);
    VECTOR2 ph = VecAdd(pt, ScalarMult(th, vt));
    VECTOR2 d = ScalarMult(1.0/th, ph);
    return d;
}


// Helper function: Checks whether obj is in a certain group
bool IsGroup(OBJEKT &obj, int group)
{
    for (int i = 0; i < (signed int)obj.groups.size(); i++)
    {
        if (obj.groups[i] == group)
        {
            return true;
        }
    }
    return false;
}

//Findet Zwischenwinkel zwischen v1 und v2, wenn v1 zu v2 rotieren soll
//(inkl. Vorzeichen)
float ComputeDeltaAngle(float v1[2], float v2[2])
{
    float anglev1 = angle_360_ccw(v1);
    float anglev2 = angle_360_ccw(v2);
    float delta = anglev2 - anglev1;        // "Anfangswinkel minus Endwinkel"
    if (delta > Pi())                       // .. dann ist in die andere Richtung um den Komplementär-
        // winkel rotiert weniger weit
    {
        delta = -(2*Pi() - delta);
    }

    return delta;
}

//--------Findet Zwischenwinkel für ein &Objekt zum nächstliegenden Ziel---------------------
float GetTargetDeltaAngle(OBJEKT &Objekt, double v0)
{
//    bool smart = false;
    double mindist2 = 0.0;            // Kleinste gefundene Distanz im Quadrat
    list<OBJEKT>::iterator miniter = UnitList.begin();
    bool match = false;

    // Objektliste nach nahen Gegnern durchsuchen
    for (list<OBJEKT>::iterator iter = UnitList.begin(); iter != UnitList.end(); iter++)
    {
        if (IsGroup((*iter), GD_ENEMY) == false)
        {
            continue;
        }
        if (((*iter).y) > PlayerShipPosY)
        {
            continue;
        }
        double dx = Objekt.x - (*iter).x;
        double dy = Objekt.y - (*iter).y;
        double dist2 = dx*dx+dy*dy;
        if (dist2 < mindist2 || match == false)
        {
            mindist2 = dist2;
            miniter = iter;
            match = true;
        }
    }


    if (match == false)
    {
        return 0.0;
    }

    // Zwischenwinkel berechnen
    float v1[2];                        // Ziel-Vektor
    float zx = (*miniter).x + (1.0/2.0)*(*miniter).bild->breite - (1.0/2.0)*Objekt.bild->hoehe;
    float zy = (*miniter).y + (1.0/2.0)*(*miniter).bild->hoehe - (1.0/2.0)*Objekt.bild->hoehe;
    v1[0] = zx - Objekt.x;
    v1[1] = zy - Objekt.y;
//    float angle1 = angle_360_cw(v1);
//    float angle2 = angle_360_cw(Objekt.v);
    float delta = ComputeDeltaAngle(Objekt.v, v1);
    char buf[128];
    sprintf(buf, "%f (%f %f) (%f %f)", delta, v1[0], v1[1], Objekt.v[0], Objekt.v[1]);
//    MessageBox(NULL, buf, "bla", NULL);
    return (-delta);                    // Minus-Zeichen, für Konsistenz mit Weltkoordinatensystem

//    float delta = angle(Objekt.v, v1);
    /*
        // Geschw.-Vektor des Objekts auf Ziel setzen mit Vorhaltewinkel (falls Objekt 'schlau')
        if(smart)
        {
            VECTOR2 v = AimTarget(Objekt, (*miniter), Objekt.s);
            Objekt.v[0] = v.x;
            Objekt.v[1]= v.y;
            return;
        }
    */
}


//--------Findet Velocity-Vector für ein &Objekt zum nächstliegenden Ziel mit Vorhaltewinkel----------
void GetTargetVector(OBJEKT &Objekt, double v0)
{
    bool smart = true;
    double mindist2 = 0.0;            // Kleinste gefundene Distanz im Quadrat
    list<OBJEKT>::iterator miniter = UnitList.begin();
    bool match = false;

    // Objektliste nach nahen Gegnern durchsuchen
    for (list<OBJEKT>::iterator iter = UnitList.begin(); iter != UnitList.end(); iter++)
    {
        /*if ((*iter).typ != UD_MONSTER1A && (*iter).typ != UD_MONSTER1B
                && (*iter).typ != UD_STONE64)
            continue;*/
        if (IsGroup((*iter), GD_ENEMY) == false)
        {
            continue;
        }
        if (((*iter).y/*+(*iter).bild->hoehe*/) > PlayerShipPosY)
        {
            continue;
        }
        double dx = Objekt.x - (*iter).x;
        double dy = Objekt.y - (*iter).y;
        double dist2 = dx*dx+dy*dy;
        if (dist2 < mindist2 || match == false)
        {
            mindist2 = dist2;
            miniter = iter;
            match = true;
        }
    }


    if (match == false)
    {
        /*        Objekt.zx = 300;
                Objekt.zy = PlayerShipPosY - 1000;*/
//        MessageBox(NULL, "foo", "foo", NULL);
        Objekt.v[0] = 0.0f;
        Objekt.v[1] = -v0;
        return;
    }

    // Objekt auf gefundenes Ziel setzen
    Objekt.zx = (*miniter).x;
    Objekt.zy = (*miniter).y;
    InitVectorDiagonal(&Objekt);

    // Geschw.-Vektor des Objekts auf Ziel setzen mit Vorhaltewinkel (falls Objekt 'schlau')
    if (smart)
    {
        VECTOR2 v = AimTarget(Objekt, (*miniter), Objekt.s);
        Objekt.v[0] = v.x;
        Objekt.v[1]= v.y;
        return;
    }

    //Richtiges Bild für bullet einstellen
//    Objekt.akt_bild = get_akt_bild_HRocket(&Objekt);
}

//-----------Berechnet und setzt Anfangsposition für eine Drone-------------------------------------
void SetDroneSpawnPos(LPOBJEKT drone)
{
    drone->v[0] = 0.0;
    drone->v[1] = 70.0;
}

//----------Updatet den Winkel der Drone-----------------------------------------------------------
void SetNewDroneAngle(LPOBJEKT drone)
{
    // Feldmissbrauch: v[0] = Dronenwinkel, v[1] = Dronenradius
    if (drone->v[0] > Pi()/4.0)
    {
        drone->v[0] = Pi()/4.0;
        drone->s = -fabs(drone->s);       // Kreisrichtung umkehren
    }
    if (drone->v[0] < -Pi()/4.0)
    {
        drone->v[0] = -Pi()/4.0;
        drone->s = fabs(drone->s);        // Kreisrichtung umkehren
    }

    // Dronenkoordinaten updaten
    float dangle = drone->s*fFrameTime;
    drone->v[0] = drone->v[0] + dangle;

}

//-----------Platziert Drone an richtigem Platz und setzt Velocity Vec.----------------------------
void UpdateDrone(LPOBJEKT drone)
{
    // Neuen Dronen-Winkel berechnen
    SetNewDroneAngle(drone);

    // Umrechnen von Polarkoordinaten in kartesische. Drone an richtige Stelle neben dem
    // Schiff verschieben.
    float shipwidth = 64, shipheight = 64;
    float hshipwidth = shipwidth/2.0, hshipheight = shipheight/2.0; // Halbe Höhe/Breite
    float shipx = PlayerShipPosX, shipy = PlayerShipPosY;
    float hdronewidth = drone->bild->breite/2.0, hdroneheight = drone->bild->hoehe/2.0;
//    float dronex = drone->x, droney = droney;
    float phi = drone->v[0], r = drone->v[1];
    float dx = cos(phi)*r, dy = sin(phi)*r;
    drone->x = (shipx + hshipwidth) + (dx - hdronewidth);
    drone->y = (shipy + hshipheight) + (dy - hdroneheight);
}

//-----------Berechnet und setzt Anfangsposition für eine Shield-------------------------------------
void SetShieldSpawnPos(LPOBJEKT shield)
{
    // Feldmissbrauch: v[0] = Winkel, v[1] = Radius
    // Feldmissbrauch: Von CreateObjekt übergebenes zx wird als Startwinkel interpretiert
    shield->v[0] = shield->zx;
    shield->v[1] = 70.0;
}

//----------Updatet den Winkel und Radius des Shield-------------------------------------------------
void SetNewShieldAngleAndRadius(LPOBJEKT shield)
{
    // Feldmissbrauch: v[0] = Winkel, v[1] = Radius

    static float maxrad = 70.0, minrad = 50.0;
    static float vrad = 0.005;            // Änderungsgeschw. des Radius

    /*    if(vrad < 0)
            MessageBox(NULL, "foo", "", NULL);*/

    // Änderungsgeschw. des Radius umkehren falls nötig
    if (shield->v[1] > maxrad)
    {
        shield->v[1] = maxrad;
        vrad = -fabs(vrad);
    }

    if (shield->v[1] < minrad)
    {
//        char buf[128];
//        sprintf(buf, "foobar: %f", vrad);
//        MessageBox(NULL, buf, "", NULL);
        shield->v[1] = minrad;
        vrad = fabs(vrad);
    }


    // Radius updaten
    float dradius = vrad*fFrameTime;
    shield->v[1] = shield->v[1] + dradius;

    // Winkel updaten
    float dangle = shield->s*fFrameTime;
    shield->v[0] = shield->v[0] + dangle;
}

//-----------Platziert Shield an richtigem Platz und setzt Velocity Vec.----------------------------
void UpdateShield(LPOBJEKT shield)
{
    // Neuen Shield-Winkel berechnen
    SetNewShieldAngleAndRadius(shield);

    // Umrechnen von Polarkoordinaten in kartesische. Drone an richtige Stelle neben dem
    // Schiff verschieben.
    float shipwidth = 64, shipheight = 64;
    float hshipwidth = shipwidth/2.0, hshipheight = shipheight/2.0; // Halbe Höhe/Breite
    float shipx = PlayerShipPosX, shipy = PlayerShipPosY;
    float hshieldwidth = shield->bild->breite/2.0, hshieldheight = shield->bild->hoehe/2.0;
//    float shieldx = shield->x, shieldy = shield->y;
    float phi = shield->v[0], r = shield->v[1];
    float dx = cos(phi)*r, dy = sin(phi)*r;
    shield->x = (shipx + hshipwidth) + (dx - hshieldwidth);
    shield->y = (shipy + hshipheight) + (dy - hshieldheight);
}

//-----------Lasse Objekt auf Playership los-------------------------------------------------------
void AimAtPlayership(LPOBJEKT obj)
{
    obj->zx = PlayerShipPosX;
    obj->zy = PlayerShipPosY - 200.0;
}

//-----------Führt das Scrolling aus---------------------------------------------------------------
void ScrollWorld(void)
{
    ScrollY -= 1;
}
//-----------Setzt die Scroll-Position auf (0/0)--------------------------------------------------
void ResetScroll(void)
{
    ScrollY = 0;
}
//-----Liefert die erreichte Punktzahl-------------------------------------------------------------
int GetScore(void)
{
    return Score;
}
//-----Setzt die Punktzahl auf 0-------------------------------------------------------------------
void ResetScore(void)
{
    Score = 0;
}
// foo
void SetChosenWeaponType(int wt)
{
    ChosenWeaponType = wt;
}

