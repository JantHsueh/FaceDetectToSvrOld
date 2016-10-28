package com.firs.facedetecttosvr;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

public class MainActivity extends AppCompatActivity {




    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    }

    @Override
    protected void onStart() {
        super.onStart();
    }



    /**
     * 跳转到Bluetooth界面
     */
    public void startBluetoothActivity(View v) {
        Intent intent = new Intent(MainActivity.this, BluetoothActivity.class);
        startActivity(intent);
    }

    /**
     * 跳转到CaptureActivity界面
     */
    public void startCaptureActivity(View v) {
        Intent intent = new Intent(MainActivity.this, CaptureActivity.class);
        startActivity(intent);
    }

    /**
     * 跳转到ImageActivity界面
     */
    public void startImageActivity(View v) {
        Intent intent = new Intent(MainActivity.this, ImageActivity.class);
        startActivity(intent);
    }

    /**
     * 跳转到RealTimeActivity界面
     */
    public void startRealTimeActivity(View v) {
        Intent intent = new Intent(MainActivity.this, RealTimeActivity.class);
        startActivity(intent);
    }

}
