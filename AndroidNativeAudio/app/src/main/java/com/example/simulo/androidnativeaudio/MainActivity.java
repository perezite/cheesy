package com.example.simulo.androidnativeaudio;

import android.media.MediaPlayer;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.media.SoundPool;
import android.view.View;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements MediaPlayer.OnCompletionListener {
    SoundPool soundPool;
    int sound1Id = -1;
    int sound2Id = -1;

    MediaPlayer player1;
    boolean isPlayer1Prepared = false;

    protected void initializeSound() {
        soundPool = new SoundPool(20, AudioManager.STREAM_MUSIC, 0);

        try {
            AssetManager assetManager = this.getAssets();
            AssetFileDescriptor sound1Descriptor = assetManager.openFd("ding.ogg");
            sound1Id = soundPool.load(sound1Descriptor, 1);
            AssetFileDescriptor sound2Descriptor = assetManager.openFd("losing.wav");
            sound2Id = soundPool.load(sound2Descriptor, 1);
        } catch (Exception e) {
            Toast.makeText(this, "failed to load sound file", Toast.LENGTH_LONG).show();
        }
    }

    protected void initializeMusic() {
        try {
            player1 = new MediaPlayer();
            AssetManager assetManager = this.getAssets();
            AssetFileDescriptor assetDescriptor = assetManager.openFd("ukulele.ogg");
            player1.setDataSource(assetDescriptor.getFileDescriptor(),
                    assetDescriptor.getStartOffset(), assetDescriptor.getLength());
            player1.prepare();
            isPlayer1Prepared = true;
            player1.setOnCompletionListener(this);
        } catch (Exception e) {
            Toast.makeText(this, "failed to load music file", Toast.LENGTH_LONG).show();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        setVolumeControlStream(AudioManager.STREAM_MUSIC);
        initializeSound();
        initializeMusic();
    }

    public void playSound1(View view) {
        if (sound1Id != -1) {
            soundPool.play(sound1Id, 1, 1, 0, 0, 1);
        }
    }

    public void playSound2(View view) {
        if (sound2Id != -1) {
            soundPool.play(sound2Id, 1, 1, 0, 0, 1);
        }
    }

    public void playMusic1(View view) {
        if (player1.isPlaying())
            return;
        try {
            synchronized (this) {
                if (!isPlayer1Prepared)
                    player1.prepare();
                player1.start();
            }
        } catch (Exception e) {
            Toast.makeText(this, "playing music failed", Toast.LENGTH_LONG).show();
        }
    }

    public void stopMusic1(View view) {
        player1.stop();
        synchronized (this) {
            isPlayer1Prepared = false;
        }
    }

    public void onCompletion(MediaPlayer player) {
        if (player == player1) {
            synchronized (this) {
                isPlayer1Prepared = false;
            }
        }
    }
}
