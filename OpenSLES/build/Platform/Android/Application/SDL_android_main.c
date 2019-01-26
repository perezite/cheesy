/*
    SDL_android_main.c, placed in the public domain by Sam Lantinga  3/13/14
*/

#include <SDL2/SDL_internal.h>

/* Include the SDL main definition header */
#include <SDL2/SDL_main.h>

#include "SDL_android_main.h"

/*******************************************************************************
                 Functions called by JNI
*******************************************************************************/
#include <jni.h>

AAssetManager *g_assetManager;

/* The application code for the main function */
#undef main
extern int main(int argc, char* args[]);

/* Called before SDL_main() to initialize JNI bindings in SDL library */
extern void SDL_Android_Init(JNIEnv* env, jclass cls);

/* This prototype is needed to prevent a warning about the missing prototype for global function below */
JNIEXPORT int JNICALL Java_org_libsdl_app_SDLActivity_nativeInit(JNIEnv* env, jclass cls, jobject array, jobject jAssetManager);

AAssetManager* getAssetManager()
{
	return g_assetManager;
}

/* Start up the SDL app */
// https://stackoverflow.com/questions/10941802/cant-access-aassetmanager-in-native-code-passed-from-java-in-wallpaperservice
JNIEXPORT int JNICALL Java_org_libsdl_app_SDLActivity_nativeInit(JNIEnv* env, jclass cls, jobject array, jobject jAssetManager)
{
    int i;
    int argc;
    int status;
    int len;
    char** argv;

	/* This interface could expand with ABI negotiation, callbacks, etc. */
    SDL_Android_Init(env, cls);

    SDL_SetMainReady();

	// retrieve the asset manager
	g_assetManager = NULL;
	g_assetManager = AAssetManager_fromJava(env, jAssetManager);

    /* Prepare the arguments. */

    len = (*env)->GetArrayLength(env, array);
    argv = SDL_stack_alloc(char*, 1 + len + 1);
    argc = 0;
    /* Use the name "app_process" so PHYSFS_platformCalcBaseDir() works.
       https://bitbucket.org/MartinFelis/love-android-sdl2/issue/23/release-build-crash-on-start
     */
    argv[argc++] = SDL_strdup("app_process");
    for (i = 0; i < len; ++i) {
        const char* utf;
        char* arg = NULL;
        jstring string = (*env)->GetObjectArrayElement(env, array, i);
        if (string) {
            utf = (*env)->GetStringUTFChars(env, string, 0);
            if (utf) {
                arg = SDL_strdup(utf);
                (*env)->ReleaseStringUTFChars(env, string, utf);
            }
            (*env)->DeleteLocalRef(env, string);
        }
        if (!arg) {
            arg = SDL_strdup("");
        }
        argv[argc++] = arg;
    }
    argv[argc] = NULL;

    /* Run the application. */
	status = main(argc, argv);

    /* Release the arguments. */

    for (i = 0; i < argc; ++i) {
        SDL_free(argv[i]);
    }
    SDL_stack_free(argv);
    /* Do not issue an exit or the whole application will terminate instead of just the SDL thread */
    /* exit(status); */

    return status;
}

/* vi: set ts=4 sw=4 expandtab: */
