//**************************************************************************//
//File: sound.cpp															//
//Enthält die Funktionen für die Sound-Ausgabe								//
//**************************************************************************//

#include "sound.h"

//=========E X T E R N A L S=====================================================================
LPDIRECTSOUNDBUFFER SoundArray[2];
//=========G L O B A L S=========================================================================
LPDIRECTSOUND	      lpDS;
//=========P R O T O S===========================================================================
// Wave-Daten laden (Paramter 2-4: Grösse, Format, eigentliche Daten, werden durch interne Funktionen gefüllt)
int LoadWave(const char *fileName, LPWAVEFORMATEX format, DWORD *lpLength, PBYTE *lpData);
// Wave-Format auslesen (Wird von LoadWave() aufgerufen)
int LoadWaveFormat(HMMIO hmmio, LPWAVEFORMATEX lpFormat);
// Wave-Länge und Wave-Daten auslesen (Wird von LoadWave() aufgerufen)
int LoadWaveDataAndLength(HMMIO hmmio, DWORD *lpLength, PBYTE *lpData);
//=========F U N K T I O N E N ==================================================================
// Initialisiert alle Sound Buffer von SoundArray[]
int GlobalSoundInit(void)
{  
	  if (MakeSoundBuffer("sound.wav", &SoundArray[1]))
	  {
			MessageBox(NULL, "Error", "CreateSoundBuffer()", NULL);
			return 1;
	  }
	  if (MakeSoundBuffer("game.wav", &SoundArray[2]))
	  {
			MessageBox(NULL, "Error", "CreateSoundBuffer()", NULL);
			return 1;
	  }

	return 0;
}

// DirectSound-Objekte initialisieren
int InitDSound(HWND hWnd)
{
	HRESULT dsrval;
	// 1. Schritt: DirectSound Interface erstellen
	dsrval = DirectSoundCreate(NULL, &lpDS, NULL);
	if(FAILED(dsrval))
	{
		MessageBox(NULL, "Error", "DirectSoundCreate()", NULL);
		return 1;
	}
	// 2. Schritt: Coop-Level festlegen
	dsrval = lpDS->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
	if(FAILED(dsrval))
	{
		MessageBox(NULL, "Error", "SetCooperativeLevel()", NULL);
		return 1;
	}
	return 0;
}

// DirectSound-Objekte freigeben
int	QuitDSound(void)
{
	if(lpDS!= NULL)
	  lpDS->Release();

	for (int i=0; i<ANZ_SOUNDS; i++) 
	{
	   if(SoundArray[i]!= NULL)
			SoundArray[i]->Release();
	}

	return 0;
}

// Secondary SoundBuffer erstellen und Wave-Daten aus der Datei fileName in den Buffer laden
int	MakeSoundBuffer(const char *fileName, LPDIRECTSOUNDBUFFER *lpDSB)
{
	DSBUFFERDESC         dsbd;          // SB Description
	BYTE                *pDSBuffData;   // SB Daten Adresse
	WAVEFORMATEX         waveFormat;    // Wave Format
	DWORD                dwDataLength;  // Länge der Wave Daten
	PBYTE                pbyWaveDaten;  // Eigentliche Wave Daten
	HRESULT              dsrval;        // Rückgabewert

	pbyWaveDaten = NULL;

	if(LoadWave(fileName, &waveFormat, &dwDataLength, &pbyWaveDaten))
	{
		MessageBox(NULL, "Error", "LoadWave()", NULL);
		return 1;
	}

	ZeroMemory(&dsbd,sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME |
				  DSBCAPS_CTRLFREQUENCY | DSBCAPS_STATIC;
	dsbd.dwBufferBytes = dwDataLength;	//Länge der Wave-Daten
	dsbd.lpwfxFormat = &waveFormat;		//Format der Wave-Daten

	dsrval = lpDS->CreateSoundBuffer(&dsbd, lpDSB, NULL);
	if (FAILED(dsrval))
	{
		MessageBox(NULL, "Error", "CreateSoundBuffer()", NULL);
		return 1;
	}

	// Sound Buffer verriegeln um Daten zu speichern
	dsrval = (*lpDSB)->Lock(0,dwDataLength,(LPVOID *)&pDSBuffData,
						   &dwDataLength,NULL,0,0);
	if (FAILED(dsrval))
	{
		MessageBox(NULL, "Error", "Lock()", NULL);
		return 1;
	}

	// Kopieren der Sounddaten in den Sound Buffer
	memcpy(pDSBuffData,pbyWaveDaten,dwDataLength);
	// Freigeben der Sounddaten (Werden jetzt nicht mehr gebraucht)
	free(pbyWaveDaten);

	// Sound Buffer entriegeln
	dsrval = (*lpDSB)->Unlock(pDSBuffData,dwDataLength,NULL,0);
	if (FAILED(dsrval))
	{
		MessageBox(NULL, "Error", "Unlock()", NULL);
		return 1;
	}

	return 0;
}

// Wave-Daten laden (Paramter 2-4: Grösse, Format, eigentliche Daten, werden durch interne Funktionen gefüllt)
int LoadWave(const char *fileName, LPWAVEFORMATEX lpFormat, DWORD *lpLength, PBYTE *lpData)
{
	HMMIO hmmio;
	// 1. Datei öffnen
	hmmio = mmioOpen((char*)((LPCTSTR)fileName), NULL, MMIO_READ);
	if(!hmmio)
	{
		MessageBox(NULL, "Error", "mmioOpen()", NULL);
		return 1;
	}
	// 2. Daten für WAVEFORMATEX Struktur aus Datei lesen
	if(LoadWaveFormat(hmmio, lpFormat))
	{
		MessageBox(NULL, "Error", "LoadWaveFormat()", NULL);
		return 1;
	}
	// 3. Wave-Datenlänge und Wave Daten aus Datei lesen
	if(LoadWaveDataAndLength(hmmio, lpLength, lpData))
	{
		MessageBox(NULL, "Error", "LoadWaveDataAndLength()", NULL);
		return 1;
	}
	// 4. Datei schliessen
	mmioClose(hmmio, 0);
	return 0;
}

// Wave-Format auslesen und *format damit füllen (Wird von LoadWave() aufgerufen)
int LoadWaveFormat(HMMIO hmmio, LPWAVEFORMATEX lpFormat)
{
	MMCKINFO mmWave;
	MMCKINFO mmFmt;
	MMRESULT mmResult;

	ZeroMemory(&mmWave, sizeof(mmWave));
	ZeroMemory(&mmFmt, sizeof(mmFmt));

	// Pointer an den Anfang des Wave Files setzen
	mmResult = mmioSeek(hmmio, 0, SEEK_SET);

	mmWave.fccType = mmioStringToFOURCC("WAVE", 0);
	mmFmt.ckid = mmioStringToFOURCC("fmt", 0);

	// Finde den Anfang des WAVE-Chunks
	mmResult = mmioDescend(hmmio,&mmWave,NULL,MMIO_FINDRIFF);
	if(FAILED(mmResult))
	   return 1;

	// Finde den Anfang des FMT-Subchunks im WAVE-Chunk
	// und setze dort Pointer-Position
	mmResult = mmioDescend(hmmio,&mmFmt,&mmWave,MMIO_FINDCHUNK);
	if(FAILED(mmResult))
	   return 1;

	// Lies von Pointer-Position in Param1 die Daten in Param2
	// über eine Länge von Param3.
	mmResult = mmioRead(hmmio,(HPSTR)lpFormat,sizeof(WAVEFORMATEX));
	if(FAILED(mmResult))
	   return 1;

	return 0;
}


// Wave-Länge und Wave-Daten auslesen (Wird von LoadWave() aufgerufen)
int LoadWaveDataAndLength(HMMIO hmmio, DWORD *lpLength, PBYTE *lpData)
{
	MMCKINFO mmWave;
	MMCKINFO mmData;
	MMRESULT mmResult;

	ZeroMemory(&mmWave, sizeof(mmWave));
	ZeroMemory(&mmData, sizeof(mmData));

	// Pointer an den Anfang des Wave Files setzen
	mmResult = mmioSeek(hmmio, 0, SEEK_SET);

	// Finde den Anfang des WAVE-Chunks
	// und setze dort Pointer-Position
	mmWave.fccType = mmioStringToFOURCC("WAVE", 0);
	mmData.ckid    = mmioStringToFOURCC("data", 0);

	mmResult = mmioDescend(hmmio,&mmWave,NULL,MMIO_FINDRIFF);
	if(FAILED(mmResult))
	{
		MessageBox(NULL, "Error", "mmioDescend()", NULL);
		return 1;
	}
	// Finde den Anfang des Data-Subchunks im WAVE-Chunk
	// und setze dort Pointer-Position
	mmResult = mmioDescend(hmmio,&mmData,&mmWave,MMIO_FINDCHUNK);
	if(FAILED(mmResult))
	{
		MessageBox(NULL, "Error", "mmioDescend()", NULL);
		return 1;
	}
	// Länge der Wave Daten speichern
	*lpLength = mmData.cksize;

	// Speicher für die Daten-Variable reservieren
	*lpData = (PBYTE)malloc(*lpLength);
	if(!*lpData)
	{
		MessageBox(NULL, "Error", "malloc()", NULL);
		return 1;
	}

	ZeroMemory(*lpData,*lpLength);

	// Wave Daten in die Variable lesen
	mmResult = mmioRead(hmmio,(HPSTR)*lpData,*lpLength);
	if(FAILED(mmResult))
	{
		MessageBox(NULL, "Error", "mmioRead()", NULL);
		return 1;
	}
	return 0;

}
// Sound einmal abspielen
void PlaySoundOnce(LPDIRECTSOUNDBUFFER lpDSB, int pan, int vol)
{
	if(lpDSB) 
	{
		lpDSB->SetPan(pan);
		lpDSB->SetVolume(vol);
		lpDSB->Play(0,0,0); 
	}
}

// Sound wiederholt abspielen (Letzter Paramter: true = starten, false = stoppen)
void PlaySoundLoop(LPDIRECTSOUNDBUFFER lpDSB, int pan, int vol, BOOL bStartStop)
{
	DWORD ret;
	lpDSB->GetStatus(&ret);
	
	if( (lpDSB) && (bStartStop) ) 
	{
		if(ret != DSBSTATUS_LOOPING) 
		{
			lpDSB->SetPan(pan);
			lpDSB->SetVolume(vol);
			lpDSB->Play(0,0,DSBPLAY_LOOPING);
		}
	}
	else if(lpDSB)
		lpDSB->Stop();
}

