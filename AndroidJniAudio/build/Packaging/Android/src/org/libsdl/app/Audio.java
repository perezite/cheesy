package org.libsdl.app;

import android.app.Activity;
import android.media.AudioManager;

public class Audio {
    public static void init(Activity activity) {
        activity.setVolumeControlStream(AudioManager.STREAM_MUSIC);
        Sound.init(activity);
        Music.init(activity);
    }

    public static void release() {
        Sound.release();
        Music.release();
    }
}
