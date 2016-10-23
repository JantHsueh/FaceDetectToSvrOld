package com.firs.cn;

import android.os.AsyncTask;
import android.util.Log;

import com.firs.facedetecttosvr.MyApp;

public class RecognizeTask extends AsyncTask<Integer , FacePosition, Boolean> {
    public final static String TAG = "recognizeTask";
    private final static int[] DEFAULT_POSITION = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    private final static int DEFAULT_DELAY = 100;
    private RecognizeListener recognizeListener = null; 
    private int mWidth = 240;
    private int mHeight = 320;
    private boolean isFront = false;
    private static boolean isWriteOne = false; 
    private static byte[] picDataOne;
    private static byte[] picDataOther;

    public RecognizeTask(int width, int height, boolean isFront) {
        // 初始化算法库
       // FaceNative.initFaceLib();
       
        this.mWidth = width;
        this.mHeight = height;
        this.isFront = isFront;
    }

    public void writePicData(byte[] picData) {
        if (isWriteOne) {
            isWriteOne = false;
            picDataOne = picData;
        } else {
            isWriteOne = true;
            picDataOther = picData;
        }
    }

    private static byte[] getPicData() {
        if (isWriteOne) {
            return picDataOther;
        } else {
            return picDataOne;
        }
    }

    @Override
    protected Boolean doInBackground(Integer... params) {
        //log("doInBackground()");默认位置、立场
        int[] position = DEFAULT_POSITION;
        byte[] picData = null;
        FacePosition pos = new FacePosition(DEFAULT_POSITION);
        while (!isCancelled()) {
             //log("MyApp.faceRec.recognizeFace(rType)");
            picData = getPicData();
            if (picData == null) {
                continue;
            }
            //log("picData:" + (picData != null? picData.length : "null"));
            position = FaceNative.recognizeFace(mWidth, mHeight, isFront, picData.clone());
            picData = null;
            //log("position ret:" + position[0] + " left: " + position[1] + " top: " + position[2] + " right: " + position[3] + " bottom: " + position[4] + " rotate: " + position[5]);
            // 获取人脸坐标
            if (isValidPosition(position)) {
                // 发送人脸坐标更新信息
                pos.fromIntArray(position);
            } else {
                pos.fromIntArray(DEFAULT_POSITION);
            }
            
            publishProgress(pos);
            //log("publishProgress(pos)");
            try {
                Thread.sleep(DEFAULT_DELAY);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            //log(" Thread.sleep end ");
        }
        log("mThread.run() end");
        return true;
    }

    @Override
    protected void onProgressUpdate(FacePosition... values) {
        super.onProgressUpdate(values);
        //log("onProgressUpdate()");
        //log(values[0].toString());
        // 原始图片和显示图片左右反转，转换坐标后为
        recognizeListener.updateFacePosition(values[0]);
    }

    @Override
    protected void onPostExecute(Boolean result) {
        log("onPostExecute()");
        super.onPostExecute(result);
        recognizeListener.recognizeClose(result);
    }

    @Override
    protected void onCancelled() {
        // 释放识别库
       //FaceNative.releaseFaceLib();
    }

    /**
     * 判断是否有效的人脸坐标
     * @param position
     * @return
     */
    private boolean isValidPosition(int[] position) {
        if (position == null || position.length < 5) {
            //log("getFacePosition() invalid position data");
            return false;
        }        
        if ((position[0] == 0 && position[2] == position[0]) || (position[1] == 0 && position[3] == position[1])) {
            //log("getFacePosition() invalid position");
            return false;
        }
        return true;
    }

    // 添加log
    private void log(String msg) {
        if (MyApp.APP_DEBUG) {
            Log.e(TAG, msg);
        }
    }

    public void setRecognizeListener(RecognizeListener recognizeListener) { 
        this.recognizeListener = recognizeListener; 
    } 

    public static interface RecognizeListener { 
        void recognizeClose(boolean result);
        void updateFacePosition(FacePosition pos);
    } 
}
