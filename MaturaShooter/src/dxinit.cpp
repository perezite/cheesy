// File: dxinit.cpp

#include "dxinit.h"
#include "Grafik.h"
#include <windows.h>
//===============P R O T O T Y P E N==============================================================
int InitFullscreenApp(HWND *hWnd, HINSTANCE hInstance, int nCmdShow, int res_x, int res_y, int ColorDepth);
int InitClipper(int Width, int Height);
void ClearScreen(void);
LPDIRECTDRAWSURFACE7 GetBackBuffer(void);
LPDIRECTDRAWSURFACE7 GetPrimarySurface(void);
LPDIRECTDRAW7 GetDDraw7(void);
bool QuitApp(char ErrorString[128]);
extern LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM  lParam);
//===============G L O B A L S====================================================================
LPDIRECTDRAW			lpdd;						// DirectDraw 1-Interface
LPDIRECTDRAW7			lpdd7;						// DirectDraw 7-Interface

DDSURFACEDESC2			ddsd;						// Surface-Desc		

LPDIRECTDRAWSURFACE7	lpddsPrimary;				// Primäre Surface. Immer Sichtbar
LPDIRECTDRAWSURFACE7	lpddsBack;					// Backbuffer

LPDIRECTDRAWCLIPPER		lpddClipper;				// Clipper
HRESULT					ddrval;
DDBLTFX					ddbltfx;
RECT					fill_area;
RECT					rect_quelle;
RECT					rect_ziel;
//===============F U N K T I O N E N==============================================================
//---------Erstellt ein DirectX Vollbild-Fenster--------------------------------------------------
int InitFullscreenApp(HWND *hWnd, HINSTANCE hInstance, int nCmdShow, int res_x, int res_y, int ColorDepth)
{
	// WINDOWS-FENSTER INITIALISIEREN
	// Fensterklasse vorbereiten
	WNDCLASS myclass;
	myclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;	
	myclass.lpfnWndProc = WndProc;
	//Extra-Speicher brauchen wir nicht
	myclass.cbClsExtra = 0;
	myclass.cbWndExtra = 0;
	//Das Instance-Handle speichern
	myclass.hInstance = hInstance; // Haben wir ja von WinMain() bekommen
	//Icon und Cursor laden. (Default-Werte)
	myclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	myclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//Hintergrundfarbe festlegen(Schwarz)
	myclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//Ein Menu haben/brauchen wir nicht (es wird eine Vollbild-Ahnwendung. Würde also nix bringen)
	myclass.lpszMenuName = NULL;
	//Name der Klasse. Ist willkürlich gewählt
	myclass.lpszClassName = "CLASSTYP1";
	// Klasse registrieren lassen
	if(RegisterClass(&myclass) == 0)
	{
		// Irgendwas ist schiefgelaufen!
		return 1;
	}
	// Fenster erstellen ...
	*hWnd=CreateWindow("CLASSTYP1","game.exe",WS_POPUP | WS_VISIBLE,0,0,res_x,
						res_y,NULL,NULL,hInstance,NULL);
	// ... und anzeigen
	ShowWindow(*hWnd, nCmdShow);
	UpdateWindow(*hWnd);
	// DIRECT DRAW INITIALISIEREN
	// Basis-Objekt vom Typ LPDIRECTDRAW erstellen
	if(FAILED(DirectDrawCreate(NULL, &lpdd, NULL)))
		return 1;
	// Durch das Basis-Objekt das DirectDraw-Hauptobjekt in der Version 7 erstellen
	if(FAILED(lpdd->QueryInterface(IID_IDirectDraw7, (LPVOID *)&lpdd7)))
		return 1;
	// Basis-Objekt freigeben (Wird nicht mehr gebraucht)
	lpdd->Release();
	lpdd=NULL;
	// KOOPERATIONSLEVEL ZWISCHEN DIRECTDRAW UND WINDOWS SETZEN 
	DWORD dwFlags=DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT;
	if(FAILED(lpdd7->SetCooperativeLevel(*hWnd, dwFlags)))
		return 1;
	// Auflösung und Farbtiefe einstellen
	if(FAILED(lpdd7->SetDisplayMode(res_x, res_y, ColorDepth, 0, 0)))
		return 1;
	// PRIMARY SURFACE ERSTELLEN
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ddsd.dwFlags= DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;	
	ddsd.dwBackBufferCount = 1;
	if(FAILED(lpdd7->CreateSurface(&ddsd, &lpddsPrimary, NULL)))
		return 1;
	// BACK BUFFER VON DER PRIMARY SURFACE HOLEN
	DDSCAPS2				ddscaps;
	ZeroMemory(&ddscaps, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	lpddsPrimary->GetAttachedSurface(&ddscaps, &lpddsBack);
	// DDBLTFX-STRUKTUR FÜLLEN
	ddbltfx.dwSize = sizeof(DDBLTFX);
	ddbltfx.dwFillColor = RGB(0,0,0);
	SetRect(&fill_area, 0, 0, res_x, res_y);
	// CLIPPER SETZEN
	if (InitClipper(res_x, res_y) != 1)
		return 1;
	// INITIALISIERTE OBJEKTE ZURÜCKGEBEN
	return 0;;
}
//----------Liefert den BackBuffer----------------------------------------------------------------
LPDIRECTDRAWSURFACE7 GetBackBuffer(void)
{
	return lpddsBack;
}
//----------Liefert die Primary Surface-----------------------------------------------------------
LPDIRECTDRAWSURFACE7 GetPrimarySurface(void)
{
	return lpddsPrimary;
}
//----------Liefert das DD-Hauptobjekt------------------------------------------------------------
LPDIRECTDRAW7 GetDDraw7(void)
{
	return lpdd7;
}
//----------Setzt den Clipper---------------------------------------------------------------------
int InitClipper(int Width, int Height)
{
	int result = lpdd7->CreateClipper(0, &lpddClipper, NULL);
	if (result != DD_OK)
		return 0;

	typedef struct CLIPLIST
	{
		RGNDATAHEADER rdh;
		RECT rgndata;
	}CLIPLIST;

	CLIPLIST cliplist;
	cliplist.rdh.dwSize = sizeof (RGNDATAHEADER);
	cliplist.rdh.iType = RDH_RECTANGLES;
	cliplist.rdh.nCount = 1;
	cliplist.rdh.nRgnSize = sizeof(cliplist.rgndata);
	cliplist.rdh.rcBound.left = 0;
	cliplist.rdh.rcBound.top = 0;
	cliplist.rdh.rcBound.right = Width;
	cliplist.rdh.rcBound.bottom = Height;
	cliplist.rgndata.left = 0;
	cliplist.rgndata.top = 0;
	cliplist.rgndata.right = Width;
	cliplist.rgndata.bottom = Height;

	result = lpddClipper->SetClipList((LPRGNDATA) &cliplist, 0);
	if (result != DD_OK)
	{
		return 0;
	}

	result = lpddsBack->SetClipper(lpddClipper);
	if (result != DD_OK)
	{
				
		return 0;
	}
return 1;
} //InitClipper()
//------------Überzieht den Bildschirm mit Schwarz------------------------------------------------
void ClearScreen(void)
{
		lpddsBack->Blt(&fill_area, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
}
//-----------Zerstört alle Objekte und beendet das Programm---------------------------------------
bool QuitApp(char ErrorString[128])
{
		if (lpddClipper != NULL)
		{
			lpddClipper->Release();
			lpddClipper = NULL;
		}
		if (lpddsBack != NULL)
		{
			lpddsBack->Release();
			lpddsBack=NULL;
		}
		if (lpddsPrimary != NULL)
		{
			lpddsPrimary->Release();
			lpddsPrimary=NULL;
		}
		if (lpdd7 != NULL)
		{
			lpdd7->Release();
			lpdd7=NULL;
		}
		PostQuitMessage(1);
		return true;
} //Ende: QuitApp
