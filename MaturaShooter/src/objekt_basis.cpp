//**************************************************************************//
//File: objekt_basis.cpp  													//
//Enthält Funktionen für die Objekte										//
//**************************************************************************//

#include <stdio.h>										// Für File-Funktionen
#include "Objekt.h"	
#include "WinMain.h"	
#include "Grafik.h"	
//===========E X T E R N A L S===================================================================
extern LPDIRECTDRAWSURFACE7	lpddsBack;					// Backbuffer
extern HWND hWnd;
//==========G L O B A L S========================================================================
list<OBJEKT> UnitList;									// Objekt-Liste
//===========P R O T O T Y P E N=================================================================
// Fügt ein Objekt direkt in die Liste ein
int InsertObjekt(OBJEKT objekt);
// Aktualisiert das Sprite eines Objektes
int UpdateSprite(LPOBJEKT objekt);
// Führt eine Kollisionsabfrage für jedes Element der Liste mit jedem anderen aus
void GlobalCollisionCheck(void);
// Löscht die gesamte Objektliste
void ClearUnitList(void);
//===========F U N K T IO N E N==================================================================
//-----------Aktualisiert die verketteten Listen-------------------------------------------------
int ListUpdate()
{
	list<OBJEKT>::iterator iter1;				// Iterator für UnitList

	// Prüfen, ob zu löschende oder zerstörte Objekte in der UnitList
	for (iter1 = UnitList.begin(); iter1 != UnitList.end(); iter1++)
	{
		// Falls Objekte Tot
		if ((*iter1).zustand == OZ_DEAD)		// Muss am Anfang stehen, da Objekt sonst immer wieder auf OZ_DESTROYED gestellt wird
		{
			list<OBJEKT>::iterator iterTemp1;	// Temporär-Iterator zum Löschen
			// Objektstatus auf zerstört -> Objekt löschen
			iterTemp1 = iter1;						
			++iterTemp1;						// Temp-Iterator auf nächstes Element
			UnitList.erase(iter1);				// Zerstörtes / abgelaufenes Element entfernen	
			iter1 = iterTemp1;					// Iterator auf nächstes Element(ist in iterTemp gespeichert)
			continue;
		}
		// Falls Objekt zerstört (Noch nicht tot)
		if ((*iter1).energie <= 0)
		{
			// Objekt zerstört->Status auf zerstört setzen
			(*iter1).zustand = OZ_DESTROYED;
		}
		// Falls Lebenszeit des Objekts abgelaufen
		if ((*iter1).LifeTime <= 0 && (*iter1).LifeTime != NULL)
		{
			(*iter1).zustand = OZ_DEAD;			// Lebenszeit abgelaufen -> Im nächsten Frame löschen lassen
		}
	}

	// Verhalten der Objekte (KI) aufrufen
	for (iter1 = UnitList.begin(); iter1 != UnitList.end(); iter1++)
	{
		UnitKI(&(*iter1));						// Verhalten (KI) der Objekte aufrufen
	}


	// Sprites malen
	for (iter1 = UnitList.begin(); iter1 != UnitList.end(); iter1++)
	{
			UpdateSprite(&(*iter1));			// Sprite des Objektes updaten und malen
	}

	// Gesamte Liste auf Kollisionen prüfen
	GlobalCollisionCheck();
	return 0;
}
//---------Fügt ein neues Objekt in die Liste ein und setzt dessen Eigenschaften-----------------
int CreateObjekt(float x, float y,   // Position
                float zx, float zy, // Zielkoordinaten des Objekts
						int typ)
{
	OBJEKT objekt;
	objekt.x            =  x;					// Startposition x
	objekt.y            =  y;					// Startposition y
	objekt.zx           = zx;					// Zielposition z
	objekt.zy           = zy;					// Zielposition z
	objekt.s			=  1;					// Geschwindigkeit 
	objekt.LifeTime		=  0;					
	objekt.zeile		= 0;
	objekt.AnimLifetime = 50;
	objekt.timer		= GetTickCount();
	objekt.akt_bild     = 0;
	objekt.kollision	= false;				// Kollisionsabfrage standardmässig aus
	objekt.typ			= typ; 
	objekt.bild			= NULL;
	// Objekt je nach Einheitentyp noch genauer einstellen
	InitUnit(&objekt);
	// Objekt in UnitList einfügen
	UnitList.push_front(objekt);
	return 0;		
} //CreateObjekt
//----------Fügt ein Objekt manuell in die Liste ein-------------------------------------------
int InsertObjekt(OBJEKT objekt)
{
	UnitList.push_front(objekt);
	return 0;
}
//----------Aktualisiert das Sprite eines Objekts----------------------------------------------
int UpdateSprite(LPOBJEKT objekt)
{
	//Prüfen, ob die Zeit bis zum nächsten Update schon überschritten ist
	if (objekt->animiert == true)
	{
		if ((GetTickCount() - objekt->timer) > objekt->AnimLifetime)
		{
			if (objekt->akt_bild < objekt->bild->anz_anim)
			{
				objekt->akt_bild++;
			}
			else
			{
				objekt->akt_bild = 0;
			}
			//Die aktuelle Zeit im Objekt speichern (Im Sprite-Teil) (Nur, wenn die Animationsstufe geändert wurde)
			objekt->timer = GetTickCount();
		}
	}

	// Umrechnung der Welt- in Bildschirmkoordinaten (Vektorkomponentenweise)
	int BildschirmX = (int)((objekt->x) + (-ScrollX));
	int BildschirmY = (int)((objekt->y) + (-ScrollY));
	MaleSprite(GetBackBuffer(), objekt->bild, BildschirmX, BildschirmY, objekt->akt_bild, objekt->zeile, true);
	return 0;
} //UpdateObjekt
//----------Führt eine Kollisionsabfrage für jedes Element der Liste mit seinen Nachfolgern aus--
void GlobalCollisionCheck(void)
{
	list<OBJEKT>::iterator objekt1;				// passenden Iterator definieren
	// Jedes Objekt einmal durchgehen
	for (objekt1 = UnitList.begin(); objekt1 != UnitList.end(); objekt1++)
	{
		// Nur mit jedem Nachfolger-Element prüfen
		list<OBJEKT>::iterator objekt2;
		objekt2 = objekt1;
		objekt2++;
		for (;objekt2 != UnitList.end(); objekt2++)
		{
			if ((*objekt1).kollision == true && (*objekt2).kollision == true)
			{
				// Bounding-Box Kollisionsabfrage
				int obj1_x2 = (*objekt1).x + (*objekt1).bild->breite;
				int obj1_y2 = (*objekt1).y + (*objekt1).bild->hoehe;
				int obj2_x2 = (*objekt2).x + (*objekt2).bild->breite;
				int obj2_y2 = (*objekt2).y + (*objekt2).bild->hoehe;
				// Die eigentliche Kollisionsabfrage
				if (((*objekt1).x < obj2_x2) && ((*objekt1).y < obj2_y2) && (obj1_x2 > (*objekt2).x) && (obj1_y2 > (*objekt2).y))
				{
					// Lebensenergie beider Einheiten runtersetzen
					(*objekt1).energie -= ((*objekt2).schaden)/2;
					(*objekt2).energie -= ((*objekt1).schaden)/2;
				}
			}
		}
	}
}
//----------Löscht die gesamte Objektliste--------------------------------------------------------
void ClearUnitList(void)
{
	UnitList.clear();
}