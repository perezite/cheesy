package com.example.simulo.androidnativeaudio;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.media.SoundPool;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Toast;

import java.io.IOException;

public class MainActivity extends AppCompatActivity implements OnTouchListener {
    SoundPool soundPool;
    int explosionId = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        setVolumeControlStream(AudioManager.STREAM_MUSIC);
        soundPool = new SoundPool(20, AudioManager.STREAM_MUSIC, 0);

        try {
            AssetManager assetManager = this.getAssets();
            AssetFileDescriptor descriptor = assetManager.openFd("ding.ogg");
            explosionId = soundPool.load(descriptor, 1);
        } catch (IOException e) {
            Toast.makeText(this, "failed to load sound file", Toast.LENGTH_LONG).show();
        }
    }

    public boolean onTouch(View v, MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_UP) {
            if (explosionId != -1) {
                soundPool.play(explosionId, 1, 1, 0, 0, 1);
            }
        }
        return true;
    }

    public void playSound(View view) {
        if (explosionId != -1) {
            soundPool.play(explosionId, 1, 1, 0, 0, 1);
        }
    }

    public void playMusic(View view) {
    }
}
