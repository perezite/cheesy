package org.libsdl.app;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.media.SoundPool;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;

public class Sound {
    static Activity ParentActivity;
    static SoundPool soundPool = null;
    static List<Integer> SoundIds = new ArrayList<Integer>();

    public static void init(Activity activity) {
		try {
			ParentActivity = activity;
			soundPool = new SoundPool(20,AudioManager.STREAM_MUSIC, 0);
			Log.e("SDL", "Successfully initialized sound");
		} catch (Exception e) {
			Log.e("SDL", "Failed to initialize sound");
		}
    }

    public static int create(String assetPath) {
        int soundId = -1;

        try {
            AssetManager assetManager = ParentActivity.getAssets();
            AssetFileDescriptor sound1Descriptor = assetManager.openFd(assetPath);
            soundId = soundPool.load(sound1Descriptor, 1);
            SoundIds.add(soundId);
        } catch (Exception e) {
            return -1;
        }

        return soundId;
    }

    public static int play(int soundId) {
        try {
            int res = soundPool.play(soundId, 1, 1, 0, 0, 1);
			if (res == 0)
				return -1;
        } catch (Exception e) {
            return -1;
        }

        return 0;
    }

    public static int release(int soundId) {
        try {
            soundPool.unload(soundId);
            SoundIds.remove(new Integer(soundId));
        } catch (Exception e) {
            return -1;
        }
        return 0;
    }

    public static int setVolume(int soundId, float volume) {
        try {
            soundPool.setVolume(soundId, volume, volume);
        } catch (Exception e) {
            return -1;
        }

        return 0;
    }

    public static void release() {
        for (Integer i : SoundIds)
            release(i);
    }
}
