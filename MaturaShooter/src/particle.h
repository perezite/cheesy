//file particle.h

#ifndef __PARTICLE_H_
#define __PARTICLE_H_

#include <list>
#include "Grafik.h"
#include "Objekt.h"

// Für verkettete Listen
using namespace std;

//==========D E F I N E S========================================================================
#define MAX_PARTICLES		20				// Anzahl Partikel pro Partikelsystem
#define PI					3,1415927		// Die Zahl PI
//=========T Y P E D E F S======================================================================
// Struktur für die Partikel
typedef struct PARTICLE
{
	float				x, y;			// Position des Partikels in Weltkoordinaten
	float				v[2];			// Geschwindigkeitsvektor				
}PARTICLE, *LPPARTICLE;
// Struktur für ein Partikelsystem
typedef struct PSYSTEM
{
	float				x, y;			// Mittelpunkt des Partikelsytems in Weltkoordinaten
	PARTICLE			Particles[MAX_PARTICLES];// Array mit Partikeln
	LONG				LifeTime;		// Lebenszeit des Systems
}PSYSTEM, *LPPSYSTEM;
//==========P R O T O T Y P E N==================================================================
//Erstellt ein Partikelsystem mit Ursprung (x,y) und Partikel-Geschwindigkeit s
void CreateParticleSystem(float x, float y, float s);
//Aktualisiert die verk. Liste für die Partikel-Systeme
void ParticleSystemListUpdate(void);
// Löscht die gesamte Partikel-Liste
void ClearParticleSystemList(void);


#endif
