/**
 * Copyright © 2013-2023 firs Incorporated. All rights reserved.
 * 版权所有：飞瑞斯科技公司
 * developer：邹丰
 * data：2013-12-18
 */

package com.firs.facedetecttosvr;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.PreviewCallback;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Display;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.firs.cn.FaceNative;
import com.firs.cn.FacePosition;
import com.firs.cn.RecognizeTask;
import com.firs.cn.RecognizeTask.RecognizeListener;
import com.firs.view.FullScreenDialogFragment;

import java.io.IOException;
import java.util.Iterator;
import java.util.List;


public class MainActivity extends Activity implements View.OnClickListener,
		SurfaceHolder.Callback, PreviewCallback, RecognizeListener,
		SeekBar.OnSeekBarChangeListener{
	private final static String TAG = "MainActivity";
	// 图像相关参数
	public final static int DEFAULT_ROTATE_VALUE = 90;
	private static int cameraIndex = 0;
	private static int cameraFrontIndex = -1;
	private static int cameraBackIndex = -1;
	private static boolean isFront = true;
	// 人脸框
	public static DrawCaptureRect mDraw = null;
	private Camera camera;
	private SurfaceView sView;
	private SurfaceHolder surfaceHolder;
	private Button btnSetting = null;
	private Button btnCamera= null;
	private Button btnSend = null;
	private static Toast mToast;
	// SurfaceView窗口大小
	private int sWidth = 0;
	private int sHeight = 0;
	private float wRate = 0;
	private float hRate = 0;
	private int width = 0;
	private int height = 0;
	// 是否在浏览中
	private boolean isPreview = false;
	private boolean isShowMenu = true;
	private RecognizeTask mTask;
	private int iWith = 0,iHeight=0;
	private int iFaceSize = 0;
	private long lbuttonShowTime = 0;
	private SeekBar mSeekBarDef;
	public static ProgressBar  progressBar;
	//view 四个边距
	private View v_h_top,v_h_buttom,v_w_lift,v_w_right;
	@SuppressWarnings("deprecation")
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);//隐藏标题
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN); // 隐藏状态栏
		setContentView(R.layout.activity_main);
		mDraw = new DrawCaptureRect(MainActivity.this, getResources().getColor(R.color.face_rect));// 添加人脸框
		initView();
	}
	private void initView() {
		sView = (SurfaceView) findViewById(R.id.surfaceView);// 获取界面中SurfaceView组件
		surfaceHolder = sView.getHolder();// 获得SurfaceView的SurfaceHolder
		surfaceHolder.addCallback(this);// 为surfaceHolder添加一个回调监听器
		surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);// 设置该SurfaceView自己不维护缓冲
		sView.setKeepScreenOn(true);
		addContentView(mDraw, new LayoutParams(LayoutParams.WRAP_CONTENT,LayoutParams.WRAP_CONTENT));//画图
		mDraw.setVisibility(View.INVISIBLE);
		btnSetting = (Button) this.findViewById(R.id.btnSetting);
		btnSetting.setOnClickListener(this);
		btnCamera = (Button) this.findViewById(R.id.btnCamera);
		btnSend = (Button) this.findViewById(R.id.imgViewMiddle);
		btnSend.setOnClickListener(this);
		sView.setOnClickListener(this);
		isShowMenu = true;
		getFrontCameraIndex();//获取前置摄像头设备号
		if (cameraFrontIndex >= 0) {//1.默认显示前置相机cameraFrontIndex >= 0  2.//默认显示后置相机 cameraBackIndex >= 0   false
			cameraIndex = cameraFrontIndex;
			isFront = true;//false
		}
		mSeekBarDef = (SeekBar) findViewById(R.id.seekBar1);
		mSeekBarDef.setOnSeekBarChangeListener(this);
		progressBar=new ProgressBar(this,null,android.R.attr.progressBarStyleLarge);
		progressBar.setIndeterminateDrawable(getResources().getDrawable(R.drawable.progressbar));
		Display currDisplay = getWindowManager().getDefaultDisplay();//获取屏幕当前分辨率
		iWith = currDisplay.getWidth();//屏幕大小
		iHeight=currDisplay.getHeight();
		//Toast.makeText(this, (iHeight*1/10)*2+"==00000", 10).show();
		int round = currDisplay.getWidth()/4;
		int displayWidth = currDisplay.getWidth()/2 - round/2;
		int displayHeight = currDisplay.getHeight()/4 - round/2-5;
		addContentView(progressBar, new LayoutParams(round,round));
		progressBar.setX(displayWidth);
		progressBar.setY(displayHeight);
		progressBar.setVisibility(View.INVISIBLE);
	}

	@Override
	protected void onResume() {
		super.onResume();
		setFaceQuality(); //获取摄像头设备索引
		//getFrontCameraIndex();//切换摄像头
		initCamera(); // 打开摄像头
		surfaceCreated(surfaceHolder);//设置目标界面
		initFaceDetect();//记录人脸坐标，算法库，进行识别      (识别类型为1对1识别recType = userId)
		mTask = new RecognizeTask(width, height, isFront);//异步操作   摄像头可用240  320  true
		mTask.setRecognizeListener(this);//重写接口方法
		mTask.execute(0);

	}
	@Override
	protected void onPause() {
		super.onPause();
		// 停止显示Camera图像
		if (surfaceHolder != null) {
			surfaceDestroyed(surfaceHolder);
		}
		// 释放摄像头
		releaseCamera();
		closeToast();
		closeFaceDetect();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		FaceNative.releaseFaceLib();
		FaceNative.setThreadExit();
	}


	void setFaceQuality(){
		SharedPreferences sharedPreferences = getSharedPreferences("setting", this.MODE_PRIVATE);
		int quality = Integer.valueOf(sharedPreferences.getString("quality", "0"));
		if(1 == quality){
			iFaceSize = iWith*6/30;//屏幕大小*6/30  为1/5
		}else if(2 == quality){
			iFaceSize = iWith*10/30;
		} else{
			iFaceSize = iWith*8/30;
		}
	}
	// 初始化Camera
	@SuppressLint("NewApi")
	private void initCamera() {
		log("initCamera()");
		try {
			if (!isPreview) {
				//打开摄像头    前/后
				camera = Camera.open(cameraIndex);
				//*******************************************设置旋转90度
				camera.setDisplayOrientation(DEFAULT_ROTATE_VALUE);
				//传入保存当前图像像素的字节数组。在Camera对象上，有3种不同的方式使用这个回调：
				camera.setPreviewCallback(this);
				if (camera != null) {
					Camera.Parameters parameters = camera.getParameters();
					mSeekBarDef.setProgress(parameters.getZoom());
					// 设置分辨率  （获取相机分辨率）
					List<Camera.Size> list = parameters.getSupportedPreviewSizes();
					//迭代器
					Iterator<Camera.Size> its = list.iterator();
					int minWidth = 0;
					Camera.Size size = null;

					while (its.hasNext()) {
						size = (Camera.Size) its.next();
						log("size width:" + size.width + " height:" + size.height);
						if (size.width / 4 != size.height / 3) {
							continue;
						}
						if (minWidth != 0 && minWidth < size.width) {
							continue;
						}
						log("size width:" + size.width + " height:" + size.height);
						minWidth = size.width;
						if (320 == size.width && 240 == size.height) {
							width = 320;
							height = 240;
							break;
						} else if (640 == size.width && 480 == size.height) {
							width = 640;
							height = 480;
						}
					}
					//parameters.setPictureSize(height, width);
					//2016.10.17
//	                height= height-(iHeight*1/10)*2;
//	                width=width-(iWith*1/8)*2;

					parameters.setPreviewSize(width, height);//设置图片尺寸
					camera.setParameters(parameters);

					//Size cSize = camera.getParameters().getPreviewSize();
					//camera.enableShutterSound(true);
					// 通过SurfaceView显示取景画面
					camera.setPreviewDisplay(surfaceHolder);
					camera.setPreviewCallback(this);

					List<String> focusModes = parameters.getSupportedFocusModes();
					if (focusModes.contains(Camera.Parameters.FOCUS_MODE_AUTO)) {//自动对焦
						camera.setAutoFocusMoveCallback(null);
					}

					// 开始预览
					camera.startPreview();
					isPreview = true;
					// 自动对焦
					camera.autoFocus(new AutoFocusCallback() {
						@Override
						public void onAutoFocus(boolean success, Camera camera) {
							if(success){
								camera.cancelAutoFocus();//只有加上了这一句，才会自动对焦。
							}
						}
					});

				}
			}
		} catch (RuntimeException ex)
		{
			showToast(R.string.txtCameraBusy);
			ex.printStackTrace();
			// 释放摄像头
			releaseCamera();
		} catch (Exception e) {
			showToast(R.string.txtCameraFail);
			e.printStackTrace();
			// 释放摄像头
			releaseCamera();
		}
	}


	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		try {
			// 设置显示目标界面
			if (camera != null) {
				camera.setPreviewDisplay(holder);

			}
		} catch (IOException exception) {
			exception.printStackTrace();
		}
	}

	@Override
	public void onStopTrackingTouch(SeekBar seekBar) {

	}


	@Override
	public void onStartTrackingTouch(SeekBar seekBar) {

	}


	@Override
	public void onProgressChanged(SeekBar seekBar, int progress,
								  boolean fromUser) {
		//Log.d(TAG, "seekid:"+seekBar.getId()+", progess"+progress);
		switch(seekBar.getId()) {
			case R.id.seekBar1:{
				// 设置“与系统默认SeekBar对应的TextView”的值
				//mTvDef.setText(getResources().getString(R.string.text_def)+" : "+String.valueOf(seekBar.getProgress()));
				int i = seekBar.getProgress();

				try{
					Camera.Parameters  p = camera.getParameters();
					if(p.isZoomSupported())
					{
						p.setZoom(i/3);
						camera.setParameters(p);
					}
					//Log.d(TAG, "Is support Zoom " + p.isZoomSupported());
				}catch (Exception e) {
					e.printStackTrace();
				}
				break;
			}
			default:
				break;
		}
	}

	@SuppressLint("NewApi")
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
							   int height) {
		//log("surfaceChanged() width:" + width + " height:" + height);
		sWidth = width;
		sHeight = height;
		if (camera != null) {
			camera.stopPreview();
			isPreview = false;
			camera.setPreviewCallback(this);
			camera.setAutoFocusMoveCallback(null);
			camera.startPreview();
			isPreview = true;
		}
		// 打开摄像头
		initCamera();
	}



	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		// 释放摄像头
		releaseCamera();
	}

	@Override
	public void onPreviewFrame(byte[] data, Camera camera) {
		if (data != null && mTask != null && !mTask.isCancelled()) {
			//log("onPreviewFrame()+++++++++++++++++++save");
			mTask.writePicData(data.clone());
		}

		if((lbuttonShowTime != 0)&&(System.currentTimeMillis() -lbuttonShowTime > 5000))
		{
			lbuttonShowTime = 0;
		}
	}
	//获取前置摄像头设备号
	private void getFrontCameraIndex() {
		//log("Camera.getNumberOfCameras()="+ Camera.getNumberOfCameras());
		for (int i = 0; i < Camera.getNumberOfCameras(); i++) {
			CameraInfo info = new CameraInfo();
			Camera.getCameraInfo(i, info);
			if (info.facing == CameraInfo.CAMERA_FACING_FRONT) {//这就是前置摄像头，亲。
				cameraFrontIndex = i;
			}
			if(info.facing == CameraInfo.CAMERA_FACING_BACK) {
				cameraBackIndex = i;
			}
		}
	}



	// 释放摄像头图像显示
	public void releaseCamera() {
		// 释放摄像头
		if (camera != null) {
			camera.setPreviewCallback(null);
			if (isPreview) {
				camera.stopPreview();
				isPreview = false;
			}
			camera.release();
			camera = null;
		}
	}

	/**
	 * 初始化识别状态
	 */
	public void initFaceDetect() {
		//btnStart.setEnabled(false);
		//btnStop.setEnabled(true);
		mDraw.setLocation((float)0, (float)0, (float)0, (float)0,0,(float)0,(float)0,(float)0,(float)0,(float)0,(float)0, 0);
		mDraw.setVisibility(View.VISIBLE);
		if (mTask != null) {
			mTask.cancel(true);

		}
		// 初始化算法库
		FaceNative.initFaceLib();



	}

	/**
	 * 停止识别状态
	 */
	public void closeFaceDetect() {
		//btnStart.setEnabled(true);
		//btnStop.setEnabled(false);
		mDraw.setLocation((float)0, (float)0, (float)0, (float)0,0,(float)0,(float)0,(float)0,(float)0,(float)0,(float)0, 0);
		mDraw.setVisibility(View.INVISIBLE);
		if (mTask != null) {
			mTask.cancel(true);
		}
		//FaceNative.releaseFaceLib();
	}

	public void click_Surface(View v)
	{

	}
	public void OtherSetPage(View v) {
		Intent intent = new Intent();
		//intent.setAction("com.firs.cn.RESULT");
		intent.setClass(MainActivity.this, OtherSetPage.class);
		startActivityForResult(intent, 101);
	}
	@SuppressLint("NewApi")
	@Override
	public void onClick(View view) {
		Toast toast;
		log("onClick()"+view.getId()+"id"+R.id.btn1tomore);
		switch (view.getId()) {

			case R.id.btnSetting:{//设置
				Intent intent = new Intent();
				// 通过Intent跳转界面
				//intent.setAction("com.firs.cn.RESULT");
				// 通过类名跳转界面
				intent.setClass(MainActivity.this, OtherSetPage.class);
				//startActivity(intent);

				startActivityForResult(intent, 101);
			}

			case R.id.surfaceView:
			{
				lbuttonShowTime = System.currentTimeMillis();
				break;
			}
			case R.id.imgViewMiddle:{//点击拍摄
				ridi();
			}
			break;
			default:
				break;
		}
	}

	private void ridi() {
		//满足条件  1.按钮可点击   2.双眼睁开
		if((btnSend.isEnabled() == true) && (FaceNative.getAuth() == 1)&&(FaceNative.getPictureFaceFlag() == 1))
		{
			stopFace();
			btnSend.setEnabled(false);
			progressBar.setVisibility(View.VISIBLE);
			//进行人脸比对   发送信息到服务器。
			FaceNative.sendPicture();
			new Thread() {
				@Override
				public void run() {

					//这里写入子线程需要做的工作
					//timeout 60s
					long start_time = System.currentTimeMillis()/1000;
					while(true)
					{
						if(FaceNative.getCompareFlag() == 3||FaceNative.getCompareFlag() == 4)
						{
							//bSave = true;
							Message message = new Message();
							message.what = 1;
							handler.sendMessage(message);
							break;
						}
						else if(FaceNative.getCompareFlag() == 2){
							Message message = new Message();
							message.what = 2;
							handler.sendMessage(message);
							break;
						}


						if(System.currentTimeMillis()/1000 - start_time >= 60)
						{
							//bSave = false;
							Message message = new Message();
							message.what = 0;
							handler.sendMessage(message);
							break;
						}
						try {
							Thread.sleep(1000);
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}


				}
			}.start();
		}
		else if(FaceNative.getPictureFaceFlag() == 0)//人脸抓拍失败，不如侧脸时
		{
			Message message = new Message();
			message.what = 3;
			handler.sendMessage(message);
		}
		else if(FaceNative.getAuth() != 1)//没有登录
		{
			Message message = new Message();
			message.what = 4;
			handler.sendMessage(message);
		}

	}


	private Handler handler = new Handler()
	{
		Toast toast;

		public void handleMessage(Message msg)
		{
			progressBar.setVisibility(View.INVISIBLE);
			btnSend.setEnabled(true);
			switch (msg.what)
			{
				case 0:
					//拍摄成功，接收注册人脸   1. 成功      2.失败
					if(FaceNative.getCompareFlag() == 1){
						Intent intent = new Intent(MainActivity.this,ShowSuccessPage.class);
						startActivityForResult(intent, 1);
						break;
					}
					Intent intent = new Intent(MainActivity.this,ShowFailPage.class);
					startActivityForResult(intent, 1);
					break;
				case 1:
					Intent intent1 = new Intent(MainActivity.this,ShowSuccessPage.class);
					startActivityForResult(intent1, 1);
					break;
				case 2:
					Intent intent11 = new Intent(MainActivity.this,ShowFailPage.class);
					startActivityForResult(intent11, 1);
					break;
				case 3://人脸检测抓拍失败
					toast= Toast.makeText(getApplicationContext(), "人脸检测抓拍失败！", Toast.LENGTH_SHORT);
					toast.setGravity(Gravity.CENTER, 0, 0);
					toast.show();
					break;
				case 4://未登录
					toast= Toast.makeText(getApplicationContext(), "请先登录服务器!", Toast.LENGTH_SHORT);
					toast.setGravity(Gravity.CENTER, 0, 0);
					toast.show();
					break;

			}
			super.handleMessage(msg);
		}
	};

	@Override
	public void recognizeClose(boolean result) {
	}

	@Override
	public void updateFacePosition(FacePosition pos) {
		int iVal = 0;
		//log("updateFacePosition(FacePosition pos):" + pos.toString());
		if(mDraw != null) {
			//人脸数据信息对象
			FacePosition fp = getFaceLocation(pos);
			mDraw.setLocation(fp.getmLeft(), fp.getmTop(), fp.getmRight(), fp.getmBottom(),
					fp.getmEyeStatus(), fp.getmLEX(), fp.getmLEY(), fp.getmREX(),
					fp.getmREY(), fp.getmMX(), fp.getmMY(), fp.getmResult());
			mDraw.invalidate();

			if(1 == FaceNative.getPictureFaceFlag())//获取人脸正确， 正面人脸
			{
				try{
					Camera.Parameters  p = camera.getParameters();
					if(p.isZoomSupported())//当中要想设置其焦距必须对其进行检查，只有2.2或者之后的才支持
					{
						if(fp.getmREX() - fp.getmLEX()<iFaceSize)//两眼之间的间距<人脸
						{
							iVal = mSeekBarDef.getProgress()+3;
							mSeekBarDef.setProgress(iVal);
						}
						else if(fp.getmREX() - fp.getmLEX()>iWith/2)//两眼之间的间距>一半的屏幕
						{
							iVal = mSeekBarDef.getProgress()-9;
							if(iVal <0)
							{
								iVal = 0;
							}
							mSeekBarDef.setProgress(iVal);
						}
						//log("0000000000x:"+fp.getmLeft()+"camera:"+p.getZoom()+"  x-x:"+(fp.getmREX() - fp.getmLEX()));
						//p.setZoom(i/3);
						//camera.setParameters(p);
					}
					// Log.d(TAG, "Is support Zoom " + p.isZoomSupported());
				}catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}

	/**
	 * 人脸框类
	 *
	 * @author zoufeng
	 */
	class DrawCaptureRect extends View {
		private int mcolorfill;
		private int mEyeStatus;
		private int mRadius = 10;
		private float mleft, mtop, mright, mbuttom,mLEX,mLEY,mREX,mREY,mMX,mMY;
		private int mState;

		public DrawCaptureRect(Context context, int colorfill) {
			super(context);
			this.mcolorfill = colorfill;
			this.mleft = 0;
			this.mtop = 0;
			this.mright = 0;
			this.mbuttom = 0;
			this.mEyeStatus = 0;
			this.mLEX = 0;
			this.mLEY = 0;
			this.mREX = 0;
			this.mREY = 0;
			this.mMX = 0;
			this.mMY = 0;
		}

		/**
		 * 人脸框坐标
		 *
		 * @param left
		 * @param top
		 * @param right
		 * @param buttom
		 */
		public void setLocation(float left, float top, float right, float buttom,
								int eyeStatus, float lex, float ley, float rex, float rey, float mx, float my, int state) {
			this.mleft = left;
			this.mtop = top;
			this.mright = right;
			this.mbuttom = buttom;
			this.mEyeStatus = eyeStatus;
			this.mLEX = lex;
			this.mLEY = ley;
			this.mREX = rex;
			this.mREY = rey;
			this.mMX = mx;
			this.mMY = my;
			this.mState = state;
		}

		@SuppressLint("DrawAllocation")
		@Override
		protected void onDraw(Canvas canvas) {
//        	(iHeight*1/10)*2;
//           (iWith*1/8)*2;
			if (mState > 0) {
				Paint mpaint = new Paint();
				mpaint.setColor(mcolorfill);
				mpaint.setStyle(Paint.Style.STROKE);
				mpaint.setStrokeWidth(5.0f);

				if(isFront == false)
				{
					float left = mleft;
					float top = mtop;
					mleft = sWidth - mright;//屏幕宽-人脸方形外边界的右侧宽=左侧边宽+方形宽
					mright = mright-left+mleft;//右边=左边
					mbuttom = mtop - top + mbuttom;//人脸方形外的高 +底部=方形+底部
					mtop  = sHeight- mbuttom;//屏幕高-方形及底部=顶部

					mLEY =  sHeight - mLEY;
					mREY =  sHeight - mREY;
					mMY  =  sHeight - mMY;
				}

				// 画人脸框
				canvas.drawRect(new RectF(mleft+180, mtop+220, mright+80, mbuttom), mpaint);
				// 画左眼
				canvas.drawCircle(mLEX+150, mLEY+180, mRadius, mpaint);
				// 画右眼
				canvas.drawCircle(mREX+150, mREY+180, mRadius, mpaint);
				// 画嘴
				canvas.drawCircle(mMX+150, mMY+150, mRadius, mpaint);

				mpaint.setTextSize(30);
				mpaint.setStyle(Paint.Style.STROKE);
				mpaint.setStrokeWidth(2.0f);

				// 输出眼睛状态
				// canvas.drawText(getEyeStatus(mEyeStatus), 30, 30, mpaint);
				super.onDraw(canvas);
			}

		}


		/**
		 * 获取眼睛状态
		 * @param status 状态值
		 * @return
		 */
		public String getEyeStatus(int status) {
			switch (status) {
				case -1: {
					return "检测失败";
				}
				case 0: {
					return "完全闭眼";
				}
				case 1: {
					return "双眼睁开";
				}
				case 2: {
					return "左眼睁开";
				}
				case 3: {
					return "右眼睁开";
				}
				default: {
					return "";
				}
			}
		}
	}

	/**
	 * 横竖屏坐标转换
	 * @param fp
	 * @return
	 */
	public FacePosition getFaceLocation(FacePosition fp) {
		FacePosition lp = new FacePosition();
		lp.copyFromObject(fp);
		wRate = (float)sWidth   / (float) height;//宽比高的度数
		hRate = (float)sHeight  / (float) width ;//高比宽的度数
		//Toast.makeText(this,  fp.getmRotate()+"=========", 2).show();
		switch (fp.getmRotate()) {//旋转度数
			case 0: {
				lp.setmLeft(fp.getmLeft() * wRate);//上下左右的旋转比例度数
				lp.setmTop(fp.getmTop() * hRate);
				lp.setmRight(fp.getmRight() * wRate);
				lp.setmBottom(fp.getmBottom() * hRate);
				lp.setmLEX(fp.getmLEX() * wRate);
				lp.setmLEY(fp.getmLEY() * hRate);
				lp.setmREX(fp.getmREX() * wRate);
				lp.setmREY(fp.getmREY() * hRate);
				lp.setmMX(fp.getmMX() * wRate);
				//嘴巴的高度有偏差，高度另算
//            int titleHeight=title_main.getHeight();
//            float cha=(float)sHeight -(float)titleHeight;
//            hRate = cha/ (float) width;//高比宽的度数
				lp.setmMY(fp.getmMY() * hRate);
				break;
			}
			case 90: {
				lp.setmLeft((height - fp.getmBottom()) * wRate);
				lp.setmTop((width - fp.getmRight()) * hRate);
				lp.setmRight((height - fp.getmTop()) * wRate);
				lp.setmBottom((width - fp.getmLeft()) * hRate);
				lp.setmLEX((height - fp.getmLEY()) * wRate);
				lp.setmLEY((width - fp.getmLEX()) * hRate);
				lp.setmREX((height - fp.getmREY()) * wRate);
				lp.setmREY((width - fp.getmREX()) * hRate);
				lp.setmMX((height - fp.getmMY()) * wRate);
				lp.setmMY((width - fp.getmMX()) * hRate);
				break;
			}
			case 180: {
				//2016.10.15  旋转度   180度，位置偏眼睛下。  所以修改Y轴坐标     高hRate=（(屏幕总高-TabBar高度)/人脸方形框的宽+1.1）    ）
//        	int titleHeight=title_main.getHeight();
//        	hRate = (((float)sHeight-(float)titleHeight )/ (float) width+(float)1.1);//高比宽的度数
				lp.setmLeft((height - fp.getmRight()) * wRate);
				lp.setmTop((width - fp.getmBottom()) * hRate);
				lp.setmRight((height - fp.getmLeft()) * wRate);
				lp.setmBottom((width -fp.getmTop()) * hRate);
				lp.setmLEX(fp.getmLEX() * wRate);
				lp.setmLEY((width - fp.getmLEY()) * hRate);
				lp.setmREX(fp.getmREX() * wRate);
				lp.setmREY((width - fp.getmREY()) * hRate);
				lp.setmMX(fp.getmMX() * wRate);
				lp.setmMY((width - fp.getmMY()) * hRate);
				break;
			}
			case 270: {
				lp.setmLeft(fp.getmTop() * wRate);
				lp.setmTop(fp.getmLeft() * hRate);
				lp.setmRight(fp.getmBottom() * wRate);
				lp.setmBottom(fp.getmRight() * hRate);
				lp.setmLEX(fp.getmLEY() * wRate);
				lp.setmLEY(fp.getmLEX() * hRate);
				lp.setmREX(fp.getmREY() * wRate);
				lp.setmREY(fp.getmREX() * hRate);
				lp.setmMX(fp.getmMY() * wRate);
				lp.setmMY(fp.getmMX() * hRate);
				break;
			}
			default: {
				break;
			}

		}
		return lp;
	}

	@Override
	public boolean onMenuOpened(int featureId, Menu menu) {
		//log("onMenuOpened()");
		if (isShowMenu) {
			return super.onMenuOpened(featureId, menu);
		} else {
			return false;
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		//log("onCreateOptionsMenu()");
		//getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		//log("onOptionsItemSelected()");
		switch (item.getItemId()) {
			case R.id.action_switch_camera:{
				closeFaceDetect();
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				int index = cameraIndex;
				// 切换相机
				if (cameraIndex == cameraFrontIndex) {
					if (cameraBackIndex != -1) {
						isFront = false;

						cameraIndex = cameraBackIndex;
					} else {
						log("后置相机不可用");
					}
				} else {
					if (cameraFrontIndex != -1) {
						isFront = true;

						cameraIndex = cameraFrontIndex;
					} else {
						log("前置相机不可用");
					}
				}
				// 判断相机是否切换
				if (index != cameraIndex) {
					if (camera != null) {

						releaseCamera();
						onResume();
					} else {
						onResume();
					}
				}
				break;
			}
        /*
        case R.id.action_login : {
        	Intent intent = new Intent(MainActivity.this,Login.class);
    		startActivity(intent);
    		break;
        }*/
			case R.id.action_setsvrip : {
				Intent intent = new Intent(MainActivity.this,SetServerIP.class);
				startActivity(intent);
				break;
			}
			case R.id.action_showlist : {
				Intent intent = new Intent(MainActivity.this,ShowRecordListPage.class);
				startActivity(intent);
				break;
			}
			case R.id.action_setting : {
				Intent intent = new Intent(MainActivity.this,SettingPage.class);
				startActivity(intent);
				break;
			}
			case R.id.action_close : {
				closeToast();
				this.onPause();
				this.onDestroy();
				this.finish();
				break;
			}
			default :
				break;
		}
		return super.onOptionsItemSelected(item);
	}




	/**
	 * 显示提示消息
	 * @param context
	 * @param duration
	 */
	public void showToast(int rMemo) {
		LayoutInflater inflater = getLayoutInflater();
		View layout = inflater.inflate(R.layout.dialog_info,
				(ViewGroup) findViewById(R.id.llToast));
		TextView text = (TextView) layout.findViewById(R.id.tvTextToast);
		text.setText(rMemo);
		if (mToast == null) {
			mToast = new Toast(getApplicationContext());
			mToast.setDuration(Toast.LENGTH_SHORT);
			mToast.setView(layout);
		} else {
			mToast.setDuration(Toast.LENGTH_SHORT);
			mToast.setView(layout);
		}
		mToast.show();
	}

	//取消提示消息
	public void closeToast() {
		if (mToast != null) {
			mToast.cancel();
		}
	}
	void startFace()
	{
		// 打开摄像头
		initCamera();
		surfaceCreated(surfaceHolder);

		initFaceDetect();
		// 开始识别,(识别类型为1对1识别recType = userId)
		mTask = new RecognizeTask(width, height, isFront);
		mTask.setRecognizeListener(this);
		mTask.execute(0);
	}
	void stopFace()
	{
		if (surfaceHolder != null) {
			surfaceDestroyed(surfaceHolder);
		}

		// 释放摄像头
		releaseCamera();
		closeToast();
		//closeFaceDetect();

		if (mTask != null) {
			mTask.cancel(true);
		}
		closeFaceDetect();
	}
	public  void onetoMore(View v){
		Toast toast;
		toast= Toast.makeText(getApplicationContext(), "1:1功能尚未开通，请关注官方消息！", Toast.LENGTH_SHORT);
		toast.setGravity(Gravity.CENTER, 0, 0);
		toast.show();
	}
	//切换摄像头    前/后置
	public void cameraChange(View v){
		closeFaceDetect();//停止识别
		try {
			Thread.sleep(10);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		int index = cameraIndex;
		// 切换相机
		if (cameraIndex == cameraFrontIndex) {
			if (cameraBackIndex != -1) {
				isFront = false;

				cameraIndex = cameraBackIndex;
			} else {
				log("后置相机不可用");
			}
		} else {
			if (cameraFrontIndex != -1) {
				isFront = true;

				cameraIndex = cameraFrontIndex;
			} else {
				log("前置相机不可用");
			}
		}
		// 判断相机是否切换
		if (index != cameraIndex) {
			if (camera != null) {
				releaseCamera();
				onResume();
			} else {
				onResume();
			}
		}
	}
	// 打印log
	public void log(String msg) {
		if (MyApp.APP_DEBUG) {
			//Common.writeLog(TAG, System.currentTimeMillis() + msg);
		}
	}


	//*****************************************************
	private String name="test", pwd="123456";
	private boolean bSave = false;//是否有刷身份证
	@Override
	protected void onActivityResult( int requestCode, int resultCode, Intent data )
	{
		switch ( resultCode ) {
			case 101 :
				this.finish();
				//System.out.println(data.getExtras().getString( "result" ));
				//tv1.setText( data.getExtras().getString( "result" ));
				break;

			default :
				break;
		}

	}
	public void  goBack (View v){
		this.finish();
	}
	public void  toHistory (View v){
		Intent intent = new Intent(this,ShowRecordListPage.class);
		startActivity(intent);

	}
	//设置页面
	public void  toSetting (View v){
		getDiaLog();
	}
	/**
	 * 弹出框
	 */
	private void getDiaLog() {
		Intent intent = new Intent(this,NewSettingPage.class);
		startActivity(intent);
//		AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this,R.style.SampleThemeLight);//R.style.SampleThemeLight
//		View view = LayoutInflater.from(MainActivity.this).inflate(R.layout.activity_new_setting_page, null);
//		RelativeLayout ry = (RelativeLayout) view.findViewById(R.id.new_setting_page_Ry);
//		ry.getBackground().setAlpha(250);//0~255透明度值
//		builder.setView(view);
//		final Dialog dialog = builder.create();
//		Window window = dialog.getWindow();
//		WindowManager.LayoutParams lp = window.getAttributes();
//		lp.alpha = 0.7f;// 设置透明度为0.3
//		window.setAttributes(lp);
//		dialog.show();
//		dialog.setCanceledOnTouchOutside(false);
//
//		View.OnClickListener listener = new View.OnClickListener() {
//			@Override
//			public void onClick(View view) {
//				MainActivity.this.finish();
//			}
//		};
//		commit.setOnClickListener(listener);
//		FullScreenDialogFragment fullScreenDialogFragment = new FullScreenDialogFragment();
//		fullScreenDialogFragment.show(getFragmentManager(), "FullScreenDialogFragment");

	}













}

