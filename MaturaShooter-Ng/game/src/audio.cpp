/*
#include "audio.h"
#include <stdio.h>
#include <string.h>


ISound* CurrentPlayingMusic = 0;
ISoundEngine* engine = createIrrKlangDevice();

AudioManager::AudioManager()
{
    return;

	if (!engine)
	{
	    // debug
	    MessageBox(NULL, "Could not startup engine\n", " ", NULL);
		printf("Could not startup engine\n");
		return; // error starting up the engine
	}
}


AudioManager::~AudioManager(void)
{
 //   this->engine->drop();
}


int AudioManager::SetMusic(char *filename)
{
    // Temp:
    return 0;

    if (CurrentPlayingMusic)
        CurrentPlayingMusic->drop();

    if(strcmp(filename, "") == 0)   // No music shall be played..
    {
        return 0;
    }

    CurrentPlayingMusic = engine->play2D(filename, true, false, true);
    if (!CurrentPlayingMusic)
    {
        return 1;
    }

    return 0;
}


int AudioManager::PlaySound(char *filename)
{
    // Temp:
    return 0;

    engine->play2D(filename, false);
}


int AudioManager::Shutdown(void)
{
    engine->drop();
}



// stop previous sound and remove it from memory
if (CurrentPlayingSound)
    CurrentPlayingSound->drop();
SoundEngine->stopAllSounds();

if (strcmp(LastPlayedSoundFile, filename))
    SoundEngine->removeAllSoundSources();

// play new sound
CurrentPlayingSound = SoundEngine->play2D(filename, LoopSound, false, true);
if (!CurrentPlayingSound)
{
    MusicTitleDisplay->value("");
    MusicTitleDisplay->redraw();
    fl_alert("Could not play sound %s", filename);
    return;
}
*/

