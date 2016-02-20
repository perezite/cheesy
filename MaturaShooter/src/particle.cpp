// File: particle.cpp

#include "particle.h"
#include <math.h>

//==========G L O B A L S========================================================================
list<PSYSTEM> ParticleSystemList;						// Liste mit Partikel-Systemen
//==========P R O T O T Y P E N==================================================================
// Erstellt ein Partikelsystem mit Ursprung (x,y) und Partikel-Geschwindigkeit speed
void CreateParticleSystem(float x, float y, float s);
// Aktualisiert die verk. Liste für die Partikel-Systeme
void UpdateParticleSystemList(void);
// Löscht die gesamte Partikel-Liste
void ClearParticleSystemList(void);
//===========F U N K T IO N E N==================================================================
//----------Erstellt ein Partikelsystem mit Ursprung (x,y) und Partikel-Geschw. s----------------
void CreateParticleSystem(float x, float y, float s)
{
	PSYSTEM ParticleSystem;
	// Ursprung des Partikelsystems setzen
	ParticleSystem.x = x;
	ParticleSystem.y = y;
	// Geschwindigkeitsvektoren und Position der einzelnen Partikel setzen
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		//---Partikel sind am Anfang alle im Mittelpunkt des Partikelsystems (Emitter)---
		ParticleSystem.Particles[i].x = x;
		ParticleSystem.Particles[i].y = y;		
		//---Geschwindigkeits-Vektor des Partikels setzen---
		// Zufälligen Geschwindigkeitsvektor für das Partikel generieren (In Radian)
		int	  alpha_grad = rand()%360;					// Zufälliger Winkel zw. 0°-360°
		float alpha_rad = (float)(PI * alpha_grad)/180;	// Umrechnung von Grad in Radian
		ParticleSystem.Particles[i].v[0] = s * cos(alpha_rad);
		ParticleSystem.Particles[i].v[1] = s * sin(alpha_rad);
	}
	// Lebenszeit des Partikelsystems setzen
	ParticleSystem.LifeTime = 1000;
	// Partikelsystem in Liste einfügen
	ParticleSystemList.push_back(ParticleSystem);
}
//----------Aktualisiert die verk. Liste für die Partikel-Systeme---------------------------------
void ParticleSystemListUpdate(void)
{
	// Iterator definieren
	list<PSYSTEM>::iterator iter;
	// Alle Partikelsysteme durchlaufen
	for(iter = ParticleSystemList.begin(); iter != ParticleSystemList.end(); iter++)
	{
		// Lebenszeit runtersetzen
		(*iter).LifeTime -= (LONG)dwFrameTime;
		// Falls Lebenszeit des Systems abgelaufen -> System löschen
		if (((*iter).LifeTime) < 0)
		{
			list<PSYSTEM>::iterator iterTemp;// Temporär-Iterator zum Löschen
			iterTemp = iter;						
			++iterTemp;						// Temp-Iterator auf nächstes Element
			ParticleSystemList.erase(iter);	// Zerstörtes / abgelaufenes Element entfernen	
			iter = iterTemp;				// Iterator auf nächstes Element(ist in iterTemp gespeichert)
			continue;
		}

		// Alle Partikel durchlaufen
		for (int i = 0; i < MAX_PARTICLES; i++)
		{
			// Aktuelles Partikel zeichnen (Koordinaten von Welt- in Bildschirmkoordinatensystem umgerechnet)
			MalePixel((*iter).Particles[i].x - ScrollX, (*iter).Particles[i].y - ScrollY, 255, 255, 0);
			// Geschwindigkeitsvektor zum aktuellen Ortsvektor des Partikels addieren
			(*iter).Particles[i].x += (*iter).Particles[i].v[0];
			(*iter).Particles[i].y += (*iter).Particles[i].v[1];
		}
	}
}
//----------Löscht die gesamte Partikel-Liste-----------------------------------------------------
void ClearParticleSystemList(void)
{
	ParticleSystemList.clear();
}
