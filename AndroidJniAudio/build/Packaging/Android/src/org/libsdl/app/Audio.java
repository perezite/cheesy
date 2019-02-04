package org.libsdl.app;

import android.app.Activity;
import android.media.AudioManager;

public class Audio {
	private static boolean isInit = false;

    public static int isInit() {
		try {
			return isInit ? 1 : 0;
		} catch (Exception e) {
			return -1;
		}
	}
	
	public static void init(Activity activity) {
        activity.setVolumeControlStream(AudioManager.STREAM_MUSIC);
        Sound.init(activity);
        Music.init(activity);
		isInit = true;
    }

    public static void release() {
        Sound.release();
        Music.release();
		isInit = false;
    }
}
