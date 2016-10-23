package com.firs.cn;

public class FaceNative {
    static {
        System.loadLibrary("stlport_shared");
        System.loadLibrary("jpeg");
        System.loadLibrary("face_identify");
    }

    /**
     * 初始化人脸识别算法库 return 返回执行结果
     */
    public static native void initFaceLib();

    /**
     * 释放初始化人脸识别算法库 return 返回执行结果
     */
    public static native void releaseFaceLib();

    /**
     * 
     * @param width
     * @param height
     * @param imgData
     * @param isFront 是否前置相机
     * @return int[] {int ret, left, top , right, bottom, rotate}
     *                              // =============== nEyeStatusThreshold>=0 返回睁闭眼分析结果=========================
                                // nEyeStatus==-1 检测失败;
                                // nEyeStatus==0  完全闭眼;
                                // nEyeStatus==1  双眼睁开;
                                // nEyeStatus==2  左眼睁开;
                                // nEyeStatus==3  右眼睁开;
     */
    public static native int[] recognizeFace(int width, int height, boolean isFront, byte[] imgData);
    //public static native int SendLoginMsg(byte[] username,byte[] pwd);
    public static native void initTcp();
    public static native void UserAuth(byte[] username, byte[] pwd);
    public static native void SetServerIP(byte[] serverip, int iPort, int ipChanged);
    public static native int getAuth();
    public static native int getCompareFlag();
    public static native int getPictureFaceFlag();
    public static native void sendPicture();
    public static native void setThreadExit();
    public static native compareresult getCompareResult(compareresult cmprest);
    public static native int getServerSockFlag();
    public static native void SetScore(int iScore);

    
}
