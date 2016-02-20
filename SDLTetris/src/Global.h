//**************************************************************************//
//File: Global.h															//
//Enthält globale Typedefs und Defines										//
//**************************************************************************//

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#define RESOLUTION_X 800
#define RESOLUTION_Y 600
#define COLOR_DEPTH 32
#define COLOR_KEY_R 255
#define COLOR_KEY_G 0
#define COLOR_KEY_B 255
#define COLOR_KEY_HIGH RGB(COLOR_KEY_R,COLOR_KEY_G,COLOR_KEY_B)
#define COLOR_KEY_LOW RGB(COLOR_KEY_R,COLOR_KEY_G,COLOR_KEY_B)

#define ANIM_LIFETIME 50						//Update-Zeit für ein Sprite-Frame

#define MAX_BILDER 100							//Maximale Anzahl an Animationsstufen

//==========================================================================//
#endif




