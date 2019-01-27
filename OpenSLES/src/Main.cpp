#include "../build/Platform/Android/Application/SDL_android_main.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <android/asset_manager.h>
#include <assert.h>
#include <string>

SLObjectItf		engineObject;
SLEngineItf		engine;
SLObjectItf		outputMixObject;
AAssetManager*	assetManager;

struct Player
{
	SLObjectItf		playerObject;
	SLPlayItf		player;
};


SLresult init()
{
	SLresult result	= slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
	assert(result == SL_RESULT_SUCCESS);

	result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
	assert(result == SL_RESULT_SUCCESS);

	result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engine);
	assert(result == SL_RESULT_SUCCESS);

	result = (*engine)->CreateOutputMix(engine, &outputMixObject, 0, NULL, NULL);
	assert(result == SL_RESULT_SUCCESS);

	result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
	assert(result == SL_RESULT_SUCCESS);

	return result == SL_RESULT_SUCCESS;
}

void SLAPIENTRY play_callback(SLPlayItf player, void* cotext, SLuint32 event) 
{
	if (event & SL_PLAYEVENT_HEADATEND)
		(*player)->SetPlayState(player, SL_PLAYSTATE_STOPPED);
}

Player* createSound(std::string assetPath)
{
	off_t start;
	off_t length;

	// open asset
	AAsset* asset = AAssetManager_open(assetManager, assetPath.c_str(), AASSET_MODE_UNKNOWN);
	assert(asset != NULL);
	int fileDescriptor = AAsset_openFileDescriptor(asset, &start, &length);
	assert(fileDescriptor > 0);
	AAsset_close(asset);

	// configure audio source
	SLDataLocator_AndroidFD sourceDataLocator = { SL_DATALOCATOR_ANDROIDFD, fileDescriptor, start, length };
	SLDataFormat_MIME mimeFormat = { SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED };
	SLDataSource audioSource = { &sourceDataLocator, &mimeFormat };

	// configure audio sink
	SLDataLocator_OutputMix outputDataLocator = { SL_DATALOCATOR_OUTPUTMIX, outputMixObject };
	SLDataSink audioSink = { &outputDataLocator, NULL };

	// create the audio player
	Player* playerInstance = new Player();
	const SLInterfaceID interfaceIds[1] = { SL_IID_PLAY };
	const SLboolean interfaceRequirements[1] = { SL_BOOLEAN_TRUE };
	SLresult result = (*engine)->CreateAudioPlayer(engine,
		&playerInstance->playerObject, &audioSource, &audioSink, 1, interfaceIds, interfaceRequirements);

	// release the player
	result = (*playerInstance->playerObject)->Realize(playerInstance->playerObject, SL_BOOLEAN_FALSE);
	assert(SL_RESULT_SUCCESS == result);

	// get the player interface
	result = (*playerInstance->playerObject)->GetInterface(
		playerInstance->playerObject, SL_IID_PLAY, &playerInstance->player);
	assert(SL_RESULT_SUCCESS == result);

	// register callback when playback ends
	(*playerInstance->player)->RegisterCallback(playerInstance->player, play_callback, NULL);
	(*playerInstance->player)->SetCallbackEventsMask(playerInstance->player, SL_PLAYEVENT_HEADATEND);

	return playerInstance;
}

void play(Player* playerInstance)
{
	if (playerInstance != NULL) {
		(*playerInstance->player)->SetPlayState(playerInstance->player, SL_PLAYSTATE_STOPPED);
		(*playerInstance->player)->SetPlayState(playerInstance->player, SL_PLAYSTATE_PLAYING);
	}
}

void destroy(Player* playerInstance)
{
	(*playerInstance->playerObject)->Destroy(playerInstance->playerObject);
}

void cleanup()
{
	if (outputMixObject != NULL)
		(*outputMixObject)->Destroy(outputMixObject);

	if (engineObject != NULL)
		(*engineObject)->Destroy(engineObject);
}

int main(int argc, char* args[])
{
	assetManager = getAssetManager();

	init();
	Player* player = createSound("ding.ogg");
	play(player);
	
	while(true)
	{
	}

	destroy(player);
	cleanup();
}