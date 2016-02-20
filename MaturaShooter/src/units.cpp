//**************************************************************************//
//File: units.cpp															//
//Verändert die Erstellung von Objekten je nach								//
//Art der Einheit und enthält die KI, sowie die Kollisionsabfrage.			//
//**************************************************************************//

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "units.h"
//===========G L O B A L S=========================================================================
// Variabeln für die Bewegung des Spielerschiffes
signed int	PlayerShipEnergy = 0;			// Energie des PlayerShip(Extern)
DWORD		ZurueckNeigenTimer;				// Timer für das langsame zurückneigen des Schiffes
int			PlayerShipPosX, PlayerShipPosY;	// Aktuelle Position des PlayerShip (Extern)						
float		iRocketTimer = 100;				// Zeit bis wieder eine Rakete verschossen werden darf
int			ChosenWeaponType = UD_STDROCKET;// Momentan gewählte Waffe
float		WeaponShotSpeed = RT_STDROCKET; // Schussgeschwindigkeit der aktuellen Waffe
// Variablen für das Scrolling
float		ScrollX		= 0;				// Ortsvektor zum Ursprung des Bildschirmkoordinatensystems
float		ScrollY		= 0;				// Ortsvektor zum Ursprung des Bildschirmkoordinatensystems
// Variabeln für den Highscore
int			Score = 0;						// Bereits erreichte Punktzahl
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
//Setzt die Scroll-Position auf (0/0)
void ResetScroll(void);	
//Liefert die erreichte Punktzahl
int GetScore(void);
//Setzt die Punktzahl auf 0
void ResetScore(void);
//===========F U N K T I O N E N===================================================================
//-----------Initialisiert die Einheiten-----------------------------------------------------------
void InitUnit(LPOBJEKT lpObjekt)
{
	// Standard-Eigenschafen festlegen (Werden nachher je nach Objekt geändert)
	lpObjekt->akt_bild = 0;
	lpObjekt->animiert = true;
	lpObjekt->AnimLifetime = ANIM_LIFETIME;
	lpObjekt->kollision = false;
	lpObjekt->LifeTime = NULL;
	lpObjekt->zustand = OZ_GO;
	lpObjekt->timer = GetTickCount();
	lpObjekt->bild = NULL;

	switch (lpObjekt->typ)
	{
	//----G E G N E R-----
	case UD_PLAYERSHIP:
		{
			lpObjekt->bild = &SpriteArray[SD_PLAYERSHIP];
			lpObjekt->akt_bild = 3;
			lpObjekt->animiert = false;
			lpObjekt->energie = 1000;
			lpObjekt->schaden = 50;
			lpObjekt->kollision = true;
			lpObjekt->s = 4;
			lpObjekt->timer = GetTickCount();
		//	lpObjekt->x = ScrollX + (float)(RESOLUTION_X / 2);
		//	lpObjekt->x = x;
		//	lpObjekt->y = ScrollY + ((float)(RESOLUTION_Y - lpObjekt->bild->hoehe));
		//	lpObjekt->y = y;
			ZurueckNeigenTimer = GetTickCount();
			return;
		} //Ende UD_PLAYERSHIP
	case UD_STONE64:
		{
			lpObjekt->bild = &SpriteArray[SD_STONE64];
			lpObjekt->energie = 50;
			lpObjekt->schaden = 100;
			lpObjekt->kollision = true;
			lpObjekt->s = 1;
			lpObjekt->LifeTime = 10000;
			return;
		} //Ende UD_STONE64
	case UD_MONSTER1A:
		{
			lpObjekt->bild = &SpriteArray[SD_MONSTER1A];
			lpObjekt->energie = 200;
			lpObjekt->schaden = 30;
			lpObjekt->kollision = true;
			lpObjekt->s = 2;
			InitVectorDiagonal(lpObjekt);
			lpObjekt->LifeTime = 10000;
			return;
		} //Ende UD_MONSTER1A
	case UD_MONSTER1B:
		{
			lpObjekt->bild = &SpriteArray[SD_MONSTER1B];
			lpObjekt->energie = 300;
			lpObjekt->schaden = 30;
			lpObjekt->kollision = true;
			lpObjekt->s = 2;
			InitVectorDiagonal(lpObjekt);
			lpObjekt->LifeTime = 10000;
			return;
		} //Ende UD_MONSTER1B
	case UD_SCHUSS:
		{
			lpObjekt->bild = &SpriteArray[SD_SCHUSS];
			lpObjekt->energie = 1;
			lpObjekt->schaden = 35;
			lpObjekt->kollision = true;
			lpObjekt->s = 4;
			InitVectorDiagonal(lpObjekt);
			lpObjekt->LifeTime = 2500;
			return;
		} //Ende UD_MONSTER1B
	case UD_EXTRA:
		{
			lpObjekt->bild = &SpriteArray[SD_EXTRA];
			lpObjekt->energie = 1;
			lpObjekt->schaden = 0;
			lpObjekt->kollision = true;
			lpObjekt->s = 0;
			lpObjekt->LifeTime = 3000;
			return;
		} //Ende UD_EXTRA
	//-----B L O C K-S--------
	case UD_BLOCK1:
		{
			lpObjekt->bild = &SpriteArray[SD_BLOCK1];
			lpObjekt->energie = 1;
			lpObjekt->schaden = 1000;
			lpObjekt->kollision = true;
			lpObjekt->s = 0;
			lpObjekt->animiert = false;
			return;
		} // Ende UD_BLOCK1
	case UD_BACK64A:
		{
			lpObjekt->bild = &SpriteArray[SD_BACK64A];
			lpObjekt->energie = 1;
			lpObjekt->schaden = 0;
			lpObjekt->kollision = false;
			lpObjekt->s = 0;
			lpObjekt->animiert = false;
			return;
		} // Ende UD_BLOCK1
	//-----E F F E K T E--------------
	case UD_EXP16:
	case UD_EXP32:
	case UD_EXP64:
		{
			switch(lpObjekt->typ)
			{
				case UD_EXP16: {lpObjekt->bild = &SpriteArray[SD_EXP16];break;}
				case UD_EXP32: {lpObjekt->bild = &SpriteArray[SD_EXP32];break;}
				case UD_EXP64: {lpObjekt->bild = &SpriteArray[SD_EXP64];break;}
			}
			lpObjekt->energie = 1;
			lpObjekt->schaden = 0;
			lpObjekt->kollision = false;
			lpObjekt->s = 0;
			lpObjekt->LifeTime = 500;
			return;
		} //Ende UD_EXP64
	//-----W A F F E N--------
	case UD_STDROCKET:
		{
			lpObjekt->bild = &SpriteArray[SD_STDROCKET];
			lpObjekt->akt_bild = 1;
			lpObjekt->energie = 1;
			lpObjekt->schaden = 50;
			lpObjekt->kollision = true;
			lpObjekt->s = 8;
			lpObjekt->LifeTime = 1900;	
			return;
		} //Ende UD_STDROCKET
	default:
		{
			return;
		}
	} //Ende switch(lpObjekt->typ)
}
//----------Die KI der Einheiten----------------------------------------------------------------
void UnitKI(LPOBJEKT lpObjekt)
{
	// Erst prüfen, ob Objekt zerstört wurde, falls ja->Funktion abbrechen
	if (CheckIfDestroyed(lpObjekt) == true)
		return;
	// Je nach Objekttyp KI aufrufen
	switch(lpObjekt->typ)
	{
	// Das Schiff des Spielers
	case UD_PLAYERSHIP:
		{
			// Aktuelle Position und Energie des PlayerShip in externe Variabeln speichern
			PlayerShipPosX = lpObjekt->x;
			PlayerShipPosY = lpObjekt->y;
			PlayerShipEnergy = lpObjekt->energie;

			// Das Schiff mit dem Vektor des vorigen Frames bewegen
			MoveDiagonal(lpObjekt);

			// Den Bewegungsvektor verkleinern (Schiff wird langsamer, wenn nicht beschleunigt)
			if (VectorLength(lpObjekt->v) > 0)
			{
				lpObjekt->v[0] = lpObjekt->v[0]*0.8;
				lpObjekt->v[1] = lpObjekt->v[1]*0.9;
			}

			// Schiff nach links bewegen
			if(GetAsyncKeyState(VK_LEFT))
			{
				lpObjekt->v[0] -= 1;
				if (GetTickCount() - lpObjekt->timer > 75 && lpObjekt->akt_bild > 0)
				{
					lpObjekt->akt_bild--;
					lpObjekt->timer = GetTickCount();
				}
			}
			// Schiff nach rechts bewegen
			if(GetAsyncKeyState(VK_RIGHT))
			{
				lpObjekt->v[0] += 1;
				if (GetTickCount() - lpObjekt->timer > 75 && lpObjekt->akt_bild < lpObjekt->bild->anz_anim)
				{
					lpObjekt->akt_bild++;
					lpObjekt->timer = GetTickCount();
				}
			}
			// Schiff nach oben bewegen
			if(GetAsyncKeyState(VK_UP))
			{
				lpObjekt->v[1] -= 0.5f;
			}
			// Schiff nach unten bewegen
			if(GetAsyncKeyState(VK_DOWN))
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
			if (abs(lpObjekt->v[0]) < 0.2) // Zurückneigen nur, falls fast keine Bewegung in X-Richtung
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

			// Rakete erstellen, falls CTRL gedrückt und genug Zeit seit letzer Rakete verstrichen
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
				CreateObjekt(lpObjekt->x + 24, lpObjekt->y - 20, 0, 0, ChosenWeaponType);
				iRocketTimer = 0;
				PlaySoundOnce(SoundArray[1], 0, DSBVOLUME_MAX);
			}

			return;
		} // Ende UD_PLAYERSHIP

	// Ein Meteorit
	case UD_STONE64:
		{
			lpObjekt->y += lpObjekt->s;
			lpObjekt->LifeTime -= dwFrameTime;
			return;
		} // Ende UD_STONE64

	// Das erste Monster (Ein Wurm)
	case UD_MONSTER1A:
		{
			if (lpObjekt->zustand == OZ_GO)
				MoveDiagonal(lpObjekt);
			return;
		} // Ende UD_MONSTER1A

	// Das zweiter Monster (Eine Schnecke)
	case UD_MONSTER1B:
		{
			if (lpObjekt->zustand == OZ_GO)
				MoveDiagonal(lpObjekt);
			// Mit einer kleinen Wahrscheinlichkeit einen Schuss erzeugen
			if((rand()%200)==1)
				CreateObjekt(lpObjekt->x + 32, lpObjekt->y + 65, PlayerShipPosX, PlayerShipPosY - 100, UD_SCHUSS);
			return;
		} // Ende UD_MONSTER1B

	// Der Schuss von Gegner Nr.2 (UD_MONSTER1B)
	case UD_SCHUSS:
		{
			if (lpObjekt->zustand == OZ_GO)
				MoveDiagonal(lpObjekt);
			return;
		} // Ende UD_EXTRA

	// 2000 Extra-Punkte
	case UD_EXTRA:
		{
			// Prüfen, ob das Spielerschiff dieses Objekt berührt hat und wenn ja-->
			// Extra-Punkte berechnen
			if (((PlayerShipPosX + 64) > lpObjekt->x) && (PlayerShipPosX < (lpObjekt->x + 32)) && ((PlayerShipPosY + 64) > lpObjekt->y) && (PlayerShipPosY < (lpObjekt->y + 32)))
			{
				Score += 2000;
				lpObjekt->zustand = OZ_DEAD;
			}
			return;
		} // Ende UD_EXTRA

	//------B L O C K S-------------------
	// Ein grosses Hindernis
	case UD_BLOCK1:
		{
			// Damit Objekt nie zerstört wird (+ um Variabel-Überläufe zu verhindern)
			lpObjekt->energie = 1;
			return;
		} //Ende UD_BLOCK1

	// Ein Stein im Hintergrund
	case UD_BACK64A:
		{
			// Damit Objekt nie zerstört wird (+ um Variabel-Überläufe zu verhindern)
			lpObjekt->energie = 1;
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

	//------W A F F E N-------------------
	// Die Standard-Rakete
	case UD_STDROCKET:
		{			
			lpObjekt->y -= lpObjekt->s;
			lpObjekt->LifeTime -= dwFrameTime;
			return;
		} // Ende UD_STDROCKET
	} // Ende switch(lpObjekt->typ)
} // Ende UnitKI()
//-----------Prüft, ob das Objekt zerstört(noch nicht gelöscht) wurde und erstellt Explosion------
bool CheckIfDestroyed(LPOBJEKT lpObjekt)
{
	if (lpObjekt->zustand == OZ_DESTROYED)
	{
		// Nicht bei jedem Objekt ist die Todesanimation gleich->Selektion
		switch(lpObjekt->typ)
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
				CreateParticleSystem(lpObjekt->x, lpObjekt->y, 2);
				CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP32);
				lpObjekt->zustand = OZ_DEAD;
				return true;
			}
		case UD_MONSTER1B:
			{
				Score += 300*3;
				CreateParticleSystem(lpObjekt->x, lpObjekt->y, 2);
				CreateObjekt(lpObjekt->x, lpObjekt->y, 0, 0, UD_EXP32);
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
		default: {return false;}
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
//-----------Berechnet den Betrag eines Vektors---------------------------------------------------
float VectorLength(float v[2])
{
	return sqrt(pow(v[0],2)+pow(v[1],2));
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
