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

void play(std::string assetPath) 
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
	SLDataLocator_AndroidFD sourceDataLocator = { SL_DATALOCATOR_ANDROIDFD, fileDescriptor, start, length};
	SLDataFormat_MIME mimeFormat = { SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED };
	SLDataSource audioSource = { &sourceDataLocator, &mimeFormat };

	// configure audio sink
	SLDataLocator_OutputMix outputDataLocator = { SL_DATALOCATOR_OUTPUTMIX, outputMixObject };
	SLDataSink audioSink = { &outputDataLocator, NULL };
}

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

int main(int argc, char* args[])
{
	assetManager = getAssetManager();

	init();
	play("orchestral.ogg");
}