//**************************************************************************//
//File: main.cpp:   														//
//Diese Datei enthält die eigentliche Andwendung und ruft Unterfunktionen	//
//aus den Header Dateien auf												//
//**************************************************************************//

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "dxinit.h"
#include "sprite.h"
#include "objekt_basis.h"
#include "main.h"
#include "Global.h"
#include "Grafik.h"
#include "Objekt.h"
#include "tiles.h"
#include "audio.h"
#include "units.h"
//	if(Sprite(&SpriteArray[SD_PLAYERSHIP], "graphics\\Ship.bmp", 7, 64, 64, 0))
TileManager Tm("graphics\\tile.txt");
//AudioManager Am;
//LPSPRITE blubsprite;
//Tile tile;
//(string filename, int breite, int hoehe, float x, float y)
//#include "Sounds.h"
//==========T Y P E D E F S==================================================================
// Struktur für einen Highscore-Eintrag (Score)
typedef struct SCORE
{
	int tag, monat, jahr;
	int minute,stunde;
	int score;
}SCORE;
// Stuktur für einen Menüpunkt (Button)
typedef struct BUTTON
{
	bool active;								// true, falls der Button angeklickt wurde
	LPSPRITE Sprite;							// Grafik für den Button
	int x, y;									// Position des Buttons
	int type;									// Typ des Buttons
	int action;									// Aktion falls angeklickt
}BUTTON, *LPBUTTON;
//==========E X T E R N A L S================================================================
int							nVirtKey;			// Gedrückte Taste
float						fFrameTime = 0;		// Zeit für den letzen Framedurchlauf (in float)
DWORD						dwFrameTime = 0;
float						Fps = 0;			// Frames per Second
//==========G L O B A L S====================================================================

// Programm - Kontrolle (Welcher Programmteil, welches Menü)
DWORD						ProgControl = PC_INITGAME;
DWORD						WindowControl = WC_ENABLED;
// Variabeln für das Menü
const int					AnzButtons = 4;		// Anzahl vorhandener Buttons
BUTTON						Buttons[4];			// Die verschiedenen Buttons im Menü
bool						bClicked;			// true, falls im Menü geklickt wurde
// Variabeln für die Anzeige und Speicherung des Highscores
SCORE						Highscore[9];		// Hierhin werden die Highscore-Daten gespeichert
// Framebremse
DWORD						dwStartTime;		// Startzeit der Hauptschleife in DWORD
float						fStartTime;			// Startzeit der Hauptschleife in float
// Variabeln für den Spielablauf (Neue Gegner, Spielende)
DWORD						GegnerZeit1 = GetTickCount();// Werden gebraucht, um in bestimmten
DWORD						GegnerZeit2;		// Zeitabständen Gegner einzufügen
DWORD						GegnerArtZeit1 = GetTickCount();// Werden gebraucht, um in bestimmten Zeitabständen
DWORD						GegnerArtZeit2;		// die Schwierigkeit zu
												// erhöhen (es kommen immer schwerere Gegner)
DWORD						NeuerGegnerZeit = 2100; // Zeit in Millisekunden, bis ein neuer Gegner erscheint
DWORD						Spielzeit = 105000;	// Anzahl Millisekunden, bis das Spiel beendet ist
DWORD						GespielteZeit = 0;	// Bereits gespielte Zeit in Millisekunden
int							Schwierigkeit = 1;	// Der Schwierigkeitsgrad (von 1 bis 3)
// Variabeln für PC_SHOWEXP
signed long					DeathTimer = 1500;	// Todesanimation des Schiffes wird 1,5 Sek. angezeigt
//===========P R O T O T Y P E N=============================================================
// DIE VERSCHIEDENEN SPIELABSCHNITTE
// Programm initialisieren
void InitGame(void);
// Menü Initialisieren
void LoadMenu(void);
// Menü anzeigen
void ShowMenu(void);
// Das eigentliche Spiel
void MainLoop(void);
// Laden der Highscore-Liste
void LoadHighscore(void);
// Zeigen des von Load Higscore geladenen Highscores (Wird von LoadHighscore aufgerufen)
void ShowHighscore(void);
// Zeigt die Punktzahl an, falls man in den Highscore gekommen ist
void ShowScore(void);
// Zeigt an, dass man nicht in den Highscore gekommen ist
void ShowNoScore(void);
// Fügt einen neuen Score in die Highscore-Datei ein
void SaveScore(int NewScore);
// Initialisiert die Spiel-Objekte und Variabeln neu
void ResetGame(void);
// ANDERE FUNKTIONEN
// Setzt den aktuellen Programmteil
void SetProgControl(int NewProgControl);
//===========F U N K T I O N E N=============================================================
//-----------Der Event-Handler---------------------------------------------------------------
int WndProc(SDL_Event event)
{
    bool done = false;
    // check for messages
    switch (event.type)
    {
        // exit if the window is closed
        case SDL_QUIT:
        {
            done = true;
//          QuitDSound();
            break;
        }

        // check for keypresses
        case SDL_KEYDOWN:
        {
            // exit if ESCAPE is pressed
/*            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                done = true;
            }*/
            switch(event.key.keysym.sym)
            {
            case SDLK_0:
            {
                bClicked = true;
                break;
            }
            case SDLK_9:
            {
                bClicked = false;
                break;
            }
            case SDLK_LCTRL:
            {
                //MessageBox(NULL, "test", "test", NULL);
                CreatePlayerShipRocket();
                break;
            }
            default:
            {
                break;
            }
            }

            break;
        }
    }


	return done;
}
//----------W I N M A I N - Abschitt-------------------------------------------------------------
int main ( int argc, char** argv )
{
    bool done = false;

	while(!done)
	{
		dwStartTime = GetTickCount();
		fStartTime = (float)GetTickCount();

        // *** message processing loop ***
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            done = WndProc(event);
        }

        // *** Programmabschnitte ausfuehren ***
		switch(ProgControl)
		{
		case PC_INITGAME:
			{
				InitGame();
				break;
			}
		case PC_LOADMENU:
			{
				LoadMenu();
				break;
			}
		case PC_SHOWMENU:
			{
				ShowMenu();
				break;
			}
		case PC_LOADHIGHSCORE:
			{
				LoadHighscore();
				break;
			}
		case PC_SHOWHIGHSCORE:
			{
				ShowHighscore();
				break;
			}
		case PC_SHOWSCORE:
			{
				ShowScore();
				break;
			}
		case PC_SHOWNOSCORE:
			{
				ShowNoScore();
				break;
			}
		case PC_SAVESCORE:
			{
				SaveScore(GetScore());
				break;
			}
		case PC_RESETGAME:
			{
				ResetGame();
				break;
			}
		case PC_SHOWDEATH:
			{
				if (DeathTimer > 0)
				{
					MainLoop();
					// Sonst kann der Timer durch Druck auf ESC abgebrochen werden
					ProgControl = PC_SHOWDEATH;
					DeathTimer -= dwFrameTime;
				}
				if (DeathTimer <= 0)
				{
					ProgControl = PC_SAVESCORE;
					DeathTimer = 1500;
				}
				break;
			}
		case PC_MAINLOOP:
			{
				if (WindowControl == WC_ENABLED)
					MainLoop();
				break;
			}
        case PC_ENDPROG:
            {
                return (0);
            }
		}
		// Am Schluss noch die Durchlaufzeit speichern
		dwFrameTime = GetTickCount() - dwStartTime;
    }// Ende while(1)

//    Am.Shutdown();

    return(0);
}// Ende WinMain
//===========D I E  S P I E L A B S C H N I T T E================================================
//-----------Laden der Spielwelt-----------------------------------------------------------------
void InitGame(void)
{
//    Am.SetMusic("snd/game.mp3");
	// Zufallsgenerator initialisieren
	srand(GetTickCount());
	// Fenster erstellen und DirectX initialisieren
	InitFullscreenApp(RESOLUTION_X, RESOLUTION_Y, COLOR_DEPTH);
	// Sämtliche Sprites laden
	GlobalSpriteInit();
	//CreateObjekt(0, 0, 0, 0, UD_PLAYERSHIP);
	LoadLevelFromFile("realtest.lvl");
	ResetGame();
	// Game-Inits fertig->Menü initialisieren
	ProgControl = PC_LOADMENU;
}
//----------Haupt-Schleife (Das eigentliche Spiel)----------------------------------------------
void MainLoop(void)
{
	// Cursor verbergen
//	ShowCursor(false);

	// Hintergrund-Musik abspielen
//	PlaySoundLoop(SoundArray[2], 0, DSBVOLUME_MAX, true);

	// Zuerst den Bildschirm löschen
	ClearScreen();

	//---Gegner einfügen (Je nach Spielabschnitt)---
	// Zeitnehmer updaten
	GegnerZeit2 = GetTickCount();
	// Falls genügend Zeit verstrichen --> Einen Gegner des aktuellen Typs einfügen
/*	if ((GegnerZeit2 - GegnerZeit1) > (NeuerGegnerZeit/Schwierigkeit))
	{
		GegnerZeit1 = GetTickCount();	// Zeitzähler zurücksetzen, damit Zählung von vorne anfängt
		int i = rand()%RESOLUTION_X;	// Zufällige Position für Gegner generieren
		// Gegner einfügen (Je nach Zeitabschnitt ein anderer Gegnertyp)
		if (GespielteZeit > 0 && GespielteZeit < 30000)
			CreateObjekt(i, ScrollY - 40, 0, 1.0, UD_MONSTER1A);
		if (GespielteZeit > 30000 && GespielteZeit < 60000)
			CreateObjekt(i, ScrollY - 40, PlayerShipPosX, PlayerShipPosY - 100, UD_MONSTER1C);
		if (GespielteZeit > 60000 && GespielteZeit < 100000)
			CreateObjekt(i, ScrollY - 40, PlayerShipPosX, PlayerShipPosY - 100, UD_MONSTER1B);
	}*/
	// Etwa alle 10 Sekunden einen Extra-Score Container einfügen
	if (rand()%700==1)
		CreateObjekt(rand()%RESOLUTION_X, ScrollY - 32, 0, 0, UD_EXTRA);
	// Variable der bereits gespielten Zeit raufsetzen
	GespielteZeit += dwFrameTime;

	// Scrolling durchführen
	ScrollWorld();

	// Hintergrund malen
//	MaleSprite(GetSDLScreen(), &SpriteArray[SD_BACKGROUND800], 0, 0, 0, 0, false);

	// Objekte und Partikelsysteme updaten und malen
	Tm.Update();
//	tile.SetSprite(&blubsprite);
//    tile.Draw();
	ListUpdate();
//	MaleSprite(GetSDLScreen(), blubsprite, 300, 300, 0, 0, true);
//	ParticleSystemListUpdate();

	// Status-Informationen anzeigen
/*	char buffer[128];
	sprintf(buffer, "Energie: %d", PlayerShipEnergy);
	MaleText(buffer, 700, 10, 255, 255, 0);
	sprintf(buffer, "FPS: %f", Fps);
	MaleText(buffer, 700, 30, 255, 255, 0);
	sprintf(buffer, "Verbleibende Zeit: %d", 105-((DWORD)(GespielteZeit/1000)));
	MaleText(buffer, 20, 10, 255, 255, 0);
	sprintf(buffer, "Punktzahl: %d", GetScore());
	MaleText(buffer, 20, 30, 255, 255, 0);
*/
	// Das Bild zuletzt noch FLIPPEN
    FlipScreen();

	// Framebremse
	while ((GetTickCount() - dwStartTime) < 14);			// Maximum ~75 fps
	{
	    FlipScreen();
		if(GetAsyncKeyState(VK_ESCAPE))
		{
			//ProgControl = PC_LOADMENU;
			ProgControl = PC_ENDPROG;                       // TEMP: kill program
                                                            // instead of showing the
                                                            // menu, which doesn't work yet
		}
	}

    if(GetAsyncKeyState(VK_ESCAPE))
    {
        //ProgControl = PC_LOADMENU;
        ProgControl = PC_ENDPROG;                           // TEMP: kill program
                                                            // instead of showing the
                                                            // menu, which doesn't work yet
    }

	// Durchlaufzeit der Hauptschleife in float speichern
	dwFrameTime = GetTickCount() - dwStartTime;
	// Durchlaufzeit der Hauptschleife in DWORD speichern
	fFrameTime = GetTickCount() - fStartTime;
	// Frames per Second speichern
	Fps = 1000.0/fFrameTime;
	// Falls Spielzeit überschritten --> Variabeln und Objekte resetten und in Highscore-Bildschirm wechseln
	if (GespielteZeit >= Spielzeit)
			ProgControl = PC_SAVESCORE;

}
//----------Menü-Struktur laden-------------------------------------------------------------------
void LoadMenu(void)
{
	// Button: "Spiel starten"
	Buttons[0].Sprite = &SpriteArray[SD_STARTEN];
	Buttons[0].x = 250;
	Buttons[0].y = 100;
	Buttons[0].active = false;
	Buttons[0].type = BD_CHANGESTATUS;
	Buttons[0].action = 0;
	// Button: Schwierigkeitsgrad
	Buttons[1].Sprite = &SpriteArray[SD_ANFAENGER];
	Buttons[1].x = 250;
	Buttons[1].y = 150;
	Buttons[1].active = false;
	Buttons[1].type = BD_INT;
	Buttons[1].action = 0;
	// Button: Highscore
	Buttons[2].Sprite = &SpriteArray[SD_HIGHSCORE];
	Buttons[2].x = 250;
	Buttons[2].y = 200;
	Buttons[2].active = false;
	Buttons[2].type = BD_CHANGESTATUS;
	Buttons[2].action = 2;
	// Button: "Spiel beenden"
	Buttons[3].Sprite = &SpriteArray[SD_BEENDEN];
	Buttons[3].x = 250;
	Buttons[3].y = 250;
	Buttons[3].active = false;
	Buttons[3].type = BD_CHANGESTATUS;
	Buttons[3].action = 1;

	// Menü geladen --> Menü jetzt anzeigen
	//ProgControl = PC_SHOWMENU;
	ProgControl = PC_RESETGAME;      // TEMP: skip the start menu, since it doesn't work
                                    // properly yet
}
//----------Menü anzeigen-------------------------------------------------------------------------
void ShowMenu(void)
{
	ShowCursor(false);
	POINT		point;
	GetCursorPos(&point);

	// ---Maus-Eingaben abfangen---
	if (GetAsyncKeyState(VK_RETURN))
		ProgControl = PC_MAINLOOP;
	// Falls auf die Maus geklickt wurde: Alle Buttons mit Mausposition vergleichen
	if (bClicked)
	{
		for (int i = 0; i < AnzButtons; i++)
		{
			RECT rect;
			rect.left = Buttons[i].x;
			rect.right = rect.left + Buttons[i].Sprite->breite;
			rect.top = Buttons[i].y;
			rect.bottom = rect.top + Buttons[i].Sprite->hoehe;
			if ((point.x > rect.left) && (point.x < rect.right) && (point.y > rect.top) && (point.y < rect.bottom))
				Buttons[i].active = true;
		}
	}

	// ---Auf Klicks reagieren---
	for (int i = 0; i < AnzButtons; i++)
	{
        // Since the Menu doesn't work properly in this version of the game
	    // we just skip it
		if (Buttons[i].active == true)
		{

			// Button-Typ: BT_CHANGESTATUS
			if (Buttons[i].type == BD_CHANGESTATUS)
			{
				// action == 0 --> Spiel starten / Fortfahren
				if (Buttons[i].action == 0)
				{
					ShowCursor(false);
					ProgControl = PC_MAINLOOP;
				}
				// action == 1 --> Programm beenden
				if (Buttons[i].action == 1)
				{
					PostQuitMessage(0);
				}
				// action == 2 --> Highscore anzeigen
				if (Buttons[i].action == 2)
				{
					ProgControl = PC_LOADHIGHSCORE;
				}
			}

			// Button-Typ: BT_INT
			if (Buttons[i].type == BD_INT)
			{
				// action == 0 --> Schwierigkeitsgrad raufstellen
				if (Buttons[i].action == 0)
				{
					if (Schwierigkeit == 1)
					{
						Schwierigkeit = 2;
						Buttons[i].Sprite = &SpriteArray[SD_ERFAHREN];
					}
					else if (Schwierigkeit == 2)
					{
						Schwierigkeit = 3;
						Buttons[i].Sprite = &SpriteArray[SD_PROFI];
					}
					else if (Schwierigkeit == 3)
					{
						Schwierigkeit = 1;
						Buttons[i].Sprite = &SpriteArray[SD_ANFAENGER];
					}
					Sleep(100);		// Kleine Verzögerung
				}
			}

			// Es wurde nun auf den Button reagiert --> Button wieder deaktivieren
			Buttons[i].active = false;
		}
	}

    // TEMP: Since the menu doesn't work properly yet, we just skip it
    ShowCursor(false);
	ProgControl = PC_MAINLOOP;

	// ---Bildschirm mit Schwarz überziehen---
	ClearScreen();

    // TEMP: Skip the menu
    FlipScreen();
    return;

	// ---Hintergrund anzeigen---
	MaleSprite(GetSDLScreen(), &SpriteArray[SD_BACKGROUND800], 0, 0, 0, 0, false);
	MaleSprite(GetSDLScreen(), &SpriteArray[SD_LOGO], 100, 400, 0, 0, true);

	// ---Das Menü anzeigen---
	// Alle Buttons des aktuellen Menüs malen
	for (int i = 0; i < AnzButtons; i++)
		MaleSprite(GetSDLScreen(), Buttons[i].Sprite, Buttons[i].x, Buttons[i].y, 0, 0, true);

	// ---Mauscursor malen---
	MaleSprite(GetSDLScreen(), &SpriteArray[SD_CURSOR], point.x, point.y, 0, 0, true);

	// ---Das Bild zuletzt noch FLIPPEN---
//	while((GetPrimarySurface())->Flip(NULL, DDFLIP_WAIT) != DD_OK);
    FlipScreen();

	return;
}
//----------Laden der Highscore-Liste-------------------------------------------------------------
void LoadHighscore(void)
{
	// Highscore-Datei öffnen
	FILE *file = fopen("highscore.dat", "rb");
	SCORE Score;
	// Alle Scores nacheinander auslesen
	for(int i = 0; i < 9; i++)
	{
		fread(&Score, sizeof(SCORE), 1, file);
		Highscore[i] = Score;
	}
	// Datei schliessen
	fclose(file);
	// Highscore wurde geladen --> Er kann jetzt angezeigt werden
	ProgControl = PC_SHOWHIGHSCORE;
}
//--------Zeigen des von Load Higscore geladenen Highscores (Wird von LoadHighscore aufgerufen)---
void ShowHighscore()
{
	// ---Bildschirm mit Schwarz überziehen---
	ClearScreen();
	// ---Hintergrund anzeigen---
	MaleSprite(GetSDLScreen(), &SpriteArray[SD_BACKGROUND800], 0, 0, 0, 0, false);
	// ---Alle Scores des Highscore-Arrays anzeigen---
	MaleText("Platzierung", 191, 50, 255, 255, 0);
	MaleText("Punkte", 280, 50, 255, 255, 0);
	MaleText("Datum", 395, 50, 255, 255, 0);
	MaleText("Zeit", 540, 50, 255, 255, 0);
	char buffer[128];
	for(int i = 0; i < 9; i++)
	{
		sprintf(buffer, "%d", (9-i));
		MaleText(buffer, 191, (450-(i * 50)) + 40, 255, 255, 0);
		sprintf(buffer, "%d", Highscore[i].score);
		MaleText(buffer, 280, (450-(i * 50)) + 40, 255, 255, 0);
		sprintf(buffer, "%d.%d.%d", Highscore[i].tag, Highscore[i].monat, Highscore[i].jahr);
		MaleText(buffer, 395, (450-(i * 50)) + 40, 255, 255, 0);
		sprintf(buffer, "%d:%d", Highscore[i].stunde, Highscore[i].minute);
		MaleText(buffer, 540, (450-(i * 50)) + 40, 255, 255, 0);
	}
	// Meldung anzeigen "Mit ESC zurück ins Hauptmenü"
	MaleText("Mit Escape zurück ins Hauptmenü", 280, 560, 255, 255, 0);
	//---Das Bild zuletzt noch FLIPPEN---
//	while((GetPrimarySurface())->Flip(NULL, DDFLIP_WAIT) != DD_OK);
    FlipScreen();
	//---Falls Escape --> Status wechseln---
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		ProgControl = PC_SHOWMENU;
	}

	// Skip the highscore, since it doesn't work properly yet
	ProgControl = PC_RESETGAME;
}
//-------------Zeigt die Punktzahl an, falls man in den Highscore gekommen ist--------------------
void ShowScore(void)
{
	// ---Bildschirm mit Schwarz überziehen---
	ClearScreen();

	// ---Hintergrund anzeigen---
	MaleSprite(GetSDLScreen(), &SpriteArray[SD_BACKGROUND800], 0, 0, 0, 0, false);

	//---Erreichte Punktzahl anzeigen---
	MaleText("Gratulation!!!", 250, 250, 255, 255, 0);
	MaleText("Du bist in den Highscore gekommen mit", 250, 280, 255, 255, 0);
	char buffer[128];
	sprintf(buffer, "%d Punkten", GetScore());
	MaleText(buffer, 250, 310, 255, 255, 0);
	MaleText("Weiter mit Enter...", 250, 400, 255, 255, 0);

	//---Das Bild zuletzt noch FLIPPEN---
//	while((GetPrimarySurface())->Flip(NULL, DDFLIP_WAIT) != DD_OK);
    FlipScreen();

	//---Falls Enter --> Status wechseln---
	if (GetAsyncKeyState(VK_RETURN))
	{
		ProgControl = PC_RESETGAME;					//Spiel-Objekte und -Variablen resetten
	}

	// Skip the score, since it doesn't work properly yet
	ProgControl = PC_RESETGAME;
}
//----------Zeigt an, dass man nicht in den Highscore gekommen ist--------------------------------
void ShowNoScore(void)
{
    // ---Bildschirm mit Schwarz überziehen---
	ClearScreen();

	// ---Hintergrund anzeigen---
	MaleSprite(GetSDLScreen(), &SpriteArray[SD_BACKGROUND800], 0, 0, 0, 0, false);

	//---Erreichte Punktzahl anzeigen---
	MaleText("Deine Punktzahl war nicht gross genug,", 250, 250, 255, 255, 0);
	MaleText("um in den Highscore zu kommen! Mehr Glück beim nächsten mal!", 250, 280, 255, 255, 0);
	MaleText("Weiter mit Enter...", 250, 400, 255, 255, 0);

	//---Das Bild zuletzt noch FLIPPEN---
//	while((GetPrimarySurface())->Flip(NULL, DDFLIP_WAIT) != DD_OK);
    FlipScreen();

	//---Falls Enter --> Status wechseln---
	if (GetAsyncKeyState(VK_RETURN))
	{
		ProgControl = PC_RESETGAME;					//Spiel-Objekte und -Variablen resetten
	}

	// Skip the no score, since it doesn't work properly yet
	ProgControl = PC_RESETGAME;
}
//----------Fügt einen neuen Score in die Highscore-Datei ein-------------------------------------
void SaveScore(int NewScore)
{
	// 1. SCHRITT: ALTE HIGHSCORE-LISTE LADEN
	// Highscore-Liste in Array laden
	FILE *file = fopen("highscore.dat", "rb");
	SCORE OldHighscore[9];
	SCORE NewHighscore[9];
	SCORE ScoreRead;				// In diese Variable werden die einzelnen Scores geladen
	// Nacheinander alle zehn Scores in das Array laden
	for (int i = 0; i < 9; i++)
	{
		// Einen Score laden
		fread(&ScoreRead, sizeof(SCORE), 1, file);
		// Diesen Score an entsprechender Stelle im Array speichern
		OldHighscore[i] = ScoreRead;
	}
	// 2. SCHRITT: NEUEN SCORE IN DIE GELADENE LISTE EINFÜGEN (FALLS GROSS GENUG)
	// Array durchgehen, bis Wert gefunden wird, der kleiner ist als neuer Score
	int ScorePos = -1;
	for (int i = 0; i < 9; i++)
	{
		if (NewScore > OldHighscore[i].score)
			ScorePos = i;
	}
	// Falls ScorePos jetzt immer noch == -1, war neue Punktzahl zu klein um in den Highscore zu kommen
	// in diesem Fall ändert sich nichts am Highscore und das Programm geht zurück ins Menü
	if (ScorePos == -1)
	{
		ProgControl = PC_SHOWNOSCORE;
		return;
	}
	// Alle Scores die kleiner als der neue Score um eins zurück verschieben im neuen Score (Ausser der erste Eintrag im
	// alten Highscore, der wird gar nicht kopiert [er fällt also aus dem Highscore raus])
	for(int i = 0; i <= ScorePos; i++)
	{
		if (i != 0) // Erster Score des alten Higscores darf nicht in neue Liste kommen
		{
			NewHighscore[i-1] = OldHighscore[i];
		}
	}
	// Alle Scores die grösser sind als neuer Score an gleiche Stelle in neuem Highscore verschieben
	for(int i = ScorePos+1; i < 9; i++)
	{
		NewHighscore[i] = OldHighscore[i];
	}
	// Momentane Zeit sowie übergebene Punktzahl an passender Stelle im neuen Highscore speichern
	time_t ZeitUnformatiert;
	tm *ZeitFormatiert;
	ZeitUnformatiert = time(0);
	ZeitFormatiert = localtime(&ZeitUnformatiert);
	NewHighscore[ScorePos].tag = ZeitFormatiert->tm_mday;
	NewHighscore[ScorePos].monat = ZeitFormatiert->tm_mon+1;
	NewHighscore[ScorePos].jahr = ZeitFormatiert->tm_year + 1900;
	NewHighscore[ScorePos].minute = ZeitFormatiert->tm_min;
	NewHighscore[ScorePos].stunde = ZeitFormatiert->tm_hour;
	NewHighscore[ScorePos].score = NewScore;

	// 3. SCHRITT: NEUE LISTE SPEICHERN
	// Alte Datei schliessen
	fclose(file);
	// Alte HighscoreDaten überschreiben
	file = fopen("highscore.dat", "wb");
	for(int i = 0; i < 9; i++)
	{
		fwrite(&NewHighscore[i], sizeof(SCORE), 1, file);
	}
	// Datei schliessen
	fclose(file);
	// Erreichte Punktzahl anzeigen
	ProgControl = PC_SHOWSCORE;
}
//----------Initialisiert die Spiel-Objekte und Variabeln neu-------------------------------------
void ResetGame(void)
{
	// Alle Variabeln und Objekte auf Anfangswerte setzen
	ClearUnitList();
//	ClearParticleSystemList();
	GegnerZeit1 = GetTickCount();
	GegnerArtZeit1 = GetTickCount();
	GespielteZeit = 0;
	NeuerGegnerZeit = 1500;
	// Schiff in den Level einfügen
	//CreateObjekt(float(RESOLUTION_X)/(2.0), 0, 0, 0, UD_PLAYERSHIP);
	CreateObjekt(0,-400,0,0, UD_PLAYERSHIP);
	// Andere Objekte einfügen
	CreateObjekt(0, -200, 0, 0, UD_BLOCK1);
	CreateObjekt(736, 0, 0, 0, UD_BACK64A);
	CreateObjekt(300, 0, 0, 0, UD_EXTRA);
	ResetScore();
	ResetScroll();
	srand(GetTickCount());
    // Waffentyp zurücksetzen
    SetChosenWeaponType(UD_STDROCKET);
	// Jetzt den Highscore anzeigen
	//ProgControl = PC_LOADHIGHSCORE;
	ProgControl = PC_MAINLOOP;
}
//----------Setzt den aktuellen Programmteil----------------------------------------------------
void SetProgControl(int NewProgControl)
{
	ProgControl = NewProgControl;
}
