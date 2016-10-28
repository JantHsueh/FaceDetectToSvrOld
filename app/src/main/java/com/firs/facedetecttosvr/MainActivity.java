package com.firs.facedetecttosvr;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.firs.facedetecttosvr.getidcardinfor.BluetoothActivity;
import com.firs.facedetecttosvr.getidcardinfor.CaptureActivity;
import com.firs.facedetecttosvr.getidcardinfor.ImageActivity;
import com.firs.facedetecttosvr.getidcardinfor.RealTimeActivity;

public class MainActivity extends Activity {



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
