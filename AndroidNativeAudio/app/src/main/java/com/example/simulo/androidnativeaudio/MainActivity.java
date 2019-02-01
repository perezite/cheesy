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
    MediaPlayer player2;
    MediaPlayer player3;
    boolean isPlayer1Prepared = false;
    boolean isPlayer2Prepared = false;
    boolean isPlayer3Prepared = false;

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

    protected void createMusic(String assetPath, MediaPlayer player) {
        try {
            AssetManager assetManager = this.getAssets();
            AssetFileDescriptor assetDescriptor = assetManager.openFd(assetPath);
            player.setDataSource(assetDescriptor.getFileDescriptor(),
                    assetDescriptor.getStartOffset(), assetDescriptor.getLength());
            player.prepare();
            player.setOnCompletionListener(this);
        } catch (Exception e) {
            Toast.makeText(this, "failed to load music file", Toast.LENGTH_LONG).show();
        }
    }

    protected void initializeMusic() {
        player1 = new MediaPlayer();
        createMusic("ukulele.ogg", player1);
        isPlayer1Prepared = true;

        player2 = new MediaPlayer();
        createMusic("idea.wav", player2);
        isPlayer2Prepared = true;

        player3 = new MediaPlayer();
        createMusic("losing.wav", player3);
        isPlayer3Prepared = true;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        setVolumeControlStream(AudioManager.STREAM_MUSIC);
        initializeSound();
        initializeMusic();
    }

    protected void playMusic(MediaPlayer player, boolean isPrepared) {
        if (player.isPlaying())
            return;
        try {
            synchronized (this) {
                if (!isPrepared)
                    player.prepare();
                player.start();
            }
        } catch (Exception e) {
            Toast.makeText(this, "playing music failed", Toast.LENGTH_LONG).show();
        }
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
        playMusic(player1, isPlayer1Prepared);
    }

    public void stopMusic1(View view) {
        player1.stop();
        synchronized (this) {
            isPlayer1Prepared = false;
        }
    }

    public void playMusic2(View view) {
        playMusic(player2, isPlayer2Prepared);
    }

    public void stopMusic2(View view) {
        player2.stop();
        synchronized (this) {
            isPlayer2Prepared = false;
        }
    }

    public void playMusic3(View view) {
        player3.setLooping(true);
        playMusic(player3, isPlayer3Prepared);
    }

    public void stopMusic3(View view) {
        player3.stop();
        synchronized (this) {
            isPlayer3Prepared = false;
        }
    }

    public void onCompletion(MediaPlayer player) {
        if (player == player1) {
            synchronized (this) {
                isPlayer1Prepared = false;
            }
        } else if (player == player2) {
            synchronized (this) {
                isPlayer2Prepared = false;
            }
        } else if (player == player3) {
            synchronized (this) {
                isPlayer3Prepared = false;
            }
        }
    }
}
