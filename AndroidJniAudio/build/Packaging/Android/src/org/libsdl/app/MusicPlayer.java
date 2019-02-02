package org.libsdl.app;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.MediaPlayer;

import java.io.IOException;

public class MusicPlayer implements MediaPlayer.OnCompletionListener {
    private MediaPlayer mediaPlayer = null;
    private boolean isPrepared = false;

    public MusicPlayer(String assetPath, Activity parentActivity) throws IOException {
        AssetManager assetManager = parentActivity.getAssets();
        AssetFileDescriptor assetDescriptor = assetManager.openFd(assetPath);

        mediaPlayer = new MediaPlayer();
        mediaPlayer.setDataSource(assetDescriptor.getFileDescriptor(),
                assetDescriptor.getStartOffset(), assetDescriptor.getLength());
        mediaPlayer.setOnCompletionListener(this);
        mediaPlayer.prepare();
        isPrepared = true;
    }

    public void play() throws IOException {
        if (mediaPlayer.isPlaying())
            return;

        synchronized (this) {
            if (!isPrepared) {
                mediaPlayer.prepare();
                isPrepared = true;
            }
        }

        mediaPlayer.start();
    }

    public void stop() {
        mediaPlayer.stop();
        synchronized (this) {
            isPrepared = false;
        }
    }

    public boolean isLooping() {
        return mediaPlayer.isLooping();
    }

    public void setLooping(boolean looping) {
        mediaPlayer.setLooping(looping);
    }

    public void setVolume(float volume) {
        mediaPlayer.setVolume(volume, volume);
    }

    public void release() {
        mediaPlayer.setLooping(false);
        mediaPlayer.stop();
        mediaPlayer.release();
        synchronized (this) {
            isPrepared = false;
        }
    }

    @Override
    public void onCompletion(MediaPlayer completedMediaPlayer) {
        if (mediaPlayer == completedMediaPlayer) {
            synchronized (this) {
                isPrepared = false;
            }
        }
    }
}
