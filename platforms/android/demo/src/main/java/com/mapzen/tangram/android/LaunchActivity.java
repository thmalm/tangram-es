package com.mapzen.tangram.android;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;

public class LaunchActivity extends AppCompatActivity {

    private static final int START_ACTIVITY_DELAY = 1000; // ms
    private Handler handler = new Handler();
    int cnt = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.launch);
    }

    @Override
    protected void onResume() {
        super.onResume();

        handler.postDelayed(launchRunnable, START_ACTIVITY_DELAY);
    }

    @Override
    protected void onPause() {
        super.onPause();

        handler.removeCallbacks(launchRunnable);
    }

    private Runnable launchRunnable = new Runnable() {

        @Override
        public void run() {

            Log.d("LaunchActivity", "Map activity launch #" + cnt++);
            Intent intent = new Intent(LaunchActivity.this, MainActivity.class);
            intent.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
            startActivity(intent);
        }
    };
}
