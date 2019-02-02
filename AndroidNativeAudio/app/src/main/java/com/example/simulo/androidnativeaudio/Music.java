package com.example.simulo.androidnativeaudio;

import android.app.Activity;

import java.util.ArrayList;
import java.util.List;

public class Music {
    static Activity ParentActivity;
    static List<MusicPlayer> players = new ArrayList<>();

    static void init(Activity activity) {
        ParentActivity = activity;
    }

    public static int loadAsync(String assetPath)
    {
        int musicId;

        try {
            MusicPlayer player = new MusicPlayer(assetPath, ParentActivity);
            player.loadAsync();
            players.add(player);
            musicId = players.indexOf(player);
        } catch (Exception e) {
            return -1;
        }

        return musicId;
    }

    public static boolean isLoadCompleted(int musicId) {
        return players.get(musicId).isLoadCompleted();
    }

    public static int play(int musicId) {
        try {
            players.get(musicId).play();
        } catch (Exception e) {
            return -1;
        }

        return 0;
    }

    public static int stop(int musicId) {
        try {
            players.get(musicId).stop();
        } catch (Exception e) {
            return -1;
        }

        return 0;
    }

    public static int setLooping(int musicId, boolean isLooping) {
        try {
            players.get(musicId).setLooping(isLooping);
        } catch (Exception e) {
            return -1;
        }

        return 0;
    }

    public static boolean isLooping(int musicId) {
        boolean result;
        try {
            result = players.get(musicId).isLooping();
        } catch (Exception e) {
            return false;
        }

        return result;
    }

    public static int setVolume(int musicId, float volume) {
        try {
            players.get(musicId).setVolume(volume);
        } catch (Exception e) {
            return -1;
        }

        return 0;
    }

    public static int release(int musicId) {
        try {
            players.get(musicId).release();
            players.remove(musicId);
        } catch (Exception e) {
            return -1;
        }

        return 0;
    }

    public static void release() {
        for (MusicPlayer player : players)
            player.release();
    }
}
