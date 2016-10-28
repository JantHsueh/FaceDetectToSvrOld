package com.firs.facedetecttosvr;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

/**
 * 1、使用设备（usb）刷身份证的方式，来获取身份证上的信息和人脸图片
 * 2、抓拍人脸
 * 3、将身份证上的人脸和抓拍到的人脸进行识别
 */
public class RealTimeActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_real_time);
    }
}
