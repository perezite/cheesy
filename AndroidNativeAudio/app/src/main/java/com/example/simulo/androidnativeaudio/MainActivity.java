package com.example.simulo.androidnativeaudio;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

public class MainActivity extends AppCompatActivity {
    int sound1Id = -1;
    int sound2Id = -1;

    int music1Id = -1;
    int music2Id = -1;
    int music3Id = -1;
    int music4Id = -1;

    boolean fadeMusic2 = false;
    float music2Volume = 1f;
    boolean music2VolumeIncreasing = false;

    Thread updateThread = null;
    long lastUpdate = System.currentTimeMillis();

    private Runnable updateRunner = new Runnable() {
        public void run() {
            while (true)
                update();
        }
    };

    protected void fadeMusic2(float deltaSeconds) {
        if (music2Volume <= 0f) {
            music2VolumeIncreasing = true;
            music2Volume = 0f;
        } else if (music2Volume >= 1f) {
            music2VolumeIncreasing = false;
            music2Volume = 1f;
        }

        float deltaVolume = deltaSeconds / 5f;
        music2Volume += music2VolumeIncreasing ? deltaVolume : -deltaVolume;

        Music.setVolume(music2Id, music2Volume);
    }

    protected void update() {
        long deltaMillis = System.currentTimeMillis() - lastUpdate;
        lastUpdate = System.currentTimeMillis();
        float deltaSeconds = deltaMillis / 1000f;

        if (fadeMusic2)
            fadeMusic2(deltaSeconds);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Audio.init(this);

        sound1Id = Sound.loadAsync("ding.ogg");
        sound2Id = Sound.loadAsync("losing.wav");

        music1Id = Music.loadAsync("ukulele.ogg");
        music2Id = Music.loadAsync("idea.wav");
        music3Id = Music.loadAsync("losing.wav");
        music4Id = Music.loadAsync("losing.wav");

        updateThread = new Thread(updateRunner);
        updateThread.start();
    }

    protected void onPause() {
        super.onPause();

        if (isFinishing()) {
            Audio.release();
        }
    }

    public void playSound1(View view) {
        if (Sound.isLoadCompleted(sound1Id))
            Sound.play(sound1Id);
    }

    public void playSound2(View view) {
        if (Sound.isLoadCompleted(sound2Id))
            Sound.play(sound2Id);
    }

    public void playMusic1(View view) {
        if (Music.isLoadCompleted(music1Id))
            Music.play(music1Id);
    }

    public void stopMusic1(View view) {
        if (Music.isLoadCompleted(music1Id))
            Music.stop(music1Id);
    }

    public void playMusic2(View view) {
        if (Music.isLoadCompleted(music2Id))
            Music.play(music2Id);
    }

    public void stopMusic2(View view) {
        if (Music.isLoadCompleted(music2Id))
            Music.stop(music2Id);
    }

    public void fadeMusic2(View view) {
        fadeMusic2 = true;
    }

    public void playMusic3(View view) {
        if (Music.isLoadCompleted(music3Id)) {
            Music.setLooping(music3Id, true);
            Music.play(music3Id);
        }
    }

    public void stopMusic3(View view) {
        if (Music.isLoadCompleted(music3Id))
            Music.stop(music3Id);
    }

    public void startMusic4(View view) {
        if (Music.isLoadCompleted(music4Id))
            Music.play(music4Id);
    }
}
