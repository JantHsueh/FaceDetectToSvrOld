/**
 * Copyright © 2013-2023 firs Incorporated. All rights reserved. 
 * 版权所有：飞瑞斯科技公司
 * developer：邹丰
 * data：2013-12-18
 */

package com.firs.facedetecttosvr;
import android.app.Application;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class MyApplication extends Application {
    private final static String TAG = "MyApplication";
    public final static boolean APP_DEBUG = true;
    private final static String TEMP_FIFACE_CONFIG = "FiFaceConfig.ini";
    private final static String TEMP_MODEL_CONTEXT = "ModelContext.conf";
    private final static String TEMP_MODEL_PARAM = "ModelParam.conf";
    private final static String TEMP_PARA_OPT4 = "PARA_opt4.bin";
    private final static String TEMP_PROJ_OPT4 = "PROJ_opt4.bin";
    private final static String CONFIG_ROOT_PATH = "/mnt/sdcard/faciallib/";
    private final static String CONFIG_SUB_PATH = "/20130413/";
    final static String FACE_PATH = "/data/data/com.firs.facedetecttosvr/facePic/";
    final static String FACE_TEMP_PATH = "/data/data/com.firs.facedetecttosvr/temp/";
    
    @Override
    public void onCreate() {
        super.onCreate();
        /*
        SharedPreferences sharedPreferences = getSharedPreferences("serversettings", this.MODE_PRIVATE);
		String serverip = sharedPreferences.getString("serverip", "183.62.134.182");
		int port = sharedPreferences.getInt("port", 32108);
		
        FaceNative.SetServerIPActivity(serverip.getBytes(), port, 0);
        log("create MyApplication  ................");
        FaceNative.initTcp();
        */
        // 配置算法库配置文件
        moveConfigFiles();
        //创建保存人脸照片目录
        CheckPath();
    }
    
    public void CheckPath()
    {
    	File rootPath = new File(FACE_PATH);
        if (!rootPath.exists() || !rootPath.isDirectory()) {
            if (rootPath.mkdir()) {
            	Log.e(TAG,"创建目录成功!FACE_PATH:" + FACE_PATH);
            } else {
                Log.e(TAG,"创建目录失败。FACE_PATH:" + FACE_PATH);
            }
         }
        rootPath = new File(FACE_TEMP_PATH);
        if (!rootPath.exists() || !rootPath.isDirectory()) {
            if (rootPath.mkdir()) {
            	Log.e(TAG,"创建目录成功!FACE_TEMP_PATH:" + FACE_TEMP_PATH);
            } else {
                Log.e(TAG,"创建目录失败。FACE_TEMP_PATH:" + FACE_TEMP_PATH);
            }
         }
    }

    public void moveConfigFiles() {
        // 创建根目录
        File rootPath = new File(CONFIG_ROOT_PATH);
        if (!rootPath.exists() || !rootPath.isDirectory()) {
            if (rootPath.mkdir()) {
                moveConfigFile(R.raw.fifaceconfig_ini, CONFIG_ROOT_PATH + TEMP_FIFACE_CONFIG);
            } else {
                Log.e(TAG,"算法库配置目录创建失败。rootPath:" + CONFIG_ROOT_PATH);
            }
         }

        // 创建子目录
        String strSubPath = CONFIG_ROOT_PATH + CONFIG_SUB_PATH;
        File subPath = new File(strSubPath);
        if (!subPath.exists() || !subPath.isDirectory()) {
            if (subPath.mkdir()) {
                moveConfigFile(R.raw.modelcontext_conf, strSubPath + TEMP_MODEL_CONTEXT);
                moveConfigFile(R.raw.modelparam_conf, strSubPath + TEMP_MODEL_PARAM);
                moveConfigFile(R.raw.para_opt4_bin, strSubPath + TEMP_PARA_OPT4);
                moveConfigFile(R.raw.proj_opt4_bin, strSubPath + TEMP_PROJ_OPT4);
            } else {
                Log.e(TAG,"算法库配置目录创建失败。SubPath:"+ strSubPath);
            }
        }
    }

    // 移动raw资源文件到目标文件
    public boolean moveConfigFile(int rId, String path) {
        try {
            File fileFif = new File(path);
            if (!fileFif.exists()) {
                fileFif.createNewFile();
                OutputStream output = new FileOutputStream(fileFif);
                InputStream input = getResources().openRawResource(rId);
                int length = input.available();
                byte[] bts = new byte[length];
                input.read(bts);
                output.write(bts);
                input.close();
                output.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
            Log.e(TAG,"算法库配置文件创建失败file:"+ path);
        }
        return false;
    }

    // 打印log
    public static void log(String msg) {
        if (APP_DEBUG) Log.e(TAG, msg);
    }
}
