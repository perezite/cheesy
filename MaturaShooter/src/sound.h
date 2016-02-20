//**************************************************************************//
//File: sound.h																//
//Enthält die Funktionen für die Sound-Ausgabe								//
//**************************************************************************//

#ifndef __SOUND_H_
#define __SOUND_H_

#include <windows.h>
#include <dsound.h>
#include <mmsystem.h>

//=========D E F I N E S=========================================================================
#define ANZ_SOUNDS			1
//=========G L O B A L S=========================================================================
extern LPDIRECTSOUNDBUFFER SoundArray[2];
//=========P R O T O S===========================================================================
// Initialisiert alle Sound Buffer von SoundArray[]
int GlobalSoundInit(void);
// DirectSound-Objekte initialisieren
int InitDSound	(HWND hWnd);
// DirectSound-Objekte freigeben
int	QuitDSound	(void);
// Secondary SoundBuffer erstellen und Wave-Datei aus der Datei fileName in den Buffer laden
int	MakeSoundBuffer(const char *fileName, LPDIRECTSOUNDBUFFER *lpDSB);
// Sound einmal abspielen
void PlaySoundOnce(LPDIRECTSOUNDBUFFER lpDSB, int pan, int vol);
// Sound wiederholt abspielen (Letzter Paramter: true = starten, false = stoppen)
void PlaySoundLoop(LPDIRECTSOUNDBUFFER lpDSB, int pan, int vol, BOOL bStartStop);
#endif
