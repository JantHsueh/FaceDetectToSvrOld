package com.firs.facedetecttosvr;


import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.FileInputStream;
import java.io.FileNotFoundException;

public class ShowFailPage extends Activity {
	
	TextView text_failshow;
	long lTime;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);//隐藏标题 
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
		WindowManager.LayoutParams.FLAG_FULLSCREEN); // 隐藏状态栏
		//getActionBar().setBackgroundDrawable(this.getResources().getDrawable(R.drawable.action_bar_bg));
		setContentView(R.layout.activity_failshow);
		//compareresult cpmrest = new compareresult();
		//text_failshow = (TextView)findViewById(R.id.textShowFail);
		//FaceNative.getCompareResult(cpmrest);
//		if(FaceNative.getCompareFlag() == 2)
//		{
//			text_failshow.setText("比对失败,服务器找不到相似人脸!");
//		}
//		else
//		{
//			text_failshow.setText("比对超时，请检查网络再重新比对!");
//		}
		lTime = System.currentTimeMillis();
		ImageView image1 = (ImageView) findViewById(R.id.imageView1);	
	    Bitmap bitmap = getLoacalBitmap("/data/data/com.firs.facedetecttosvr/temp/facePic_temp.jpg"); //从本地取图片	  
	    image1.setImageBitmap(bitmap);	//设置Bitmap
	    ShowSuccessPage.copyFile("/data/data/com.firs.facedetecttosvr/temp/facePic_temp.jpg",MyApp.FACE_PATH+Long.toString(lTime)+".jpg");
	}
	
	public void unsaveRecord(View v)
	{
		ShowFailPage.this.finish();
	}
	
	/**
	* 加载本地图片
	* http://bbs.3gstdy.com
	* @param url
	* @return
	*/
	public static Bitmap getLoacalBitmap(String url) {
	     try {
	          FileInputStream fis = new FileInputStream(url);
	          return BitmapFactory.decodeStream(fis);
	     } catch (FileNotFoundException e) {
	          e.printStackTrace();
	          return null;
	     }
	}
	
	
	
	public void  goBack (View v){
		this.finish();
		//saveRecord();//保存记录  弹出框提示
	}
    public void  toHistory (View v){
    	Intent intent = new Intent(this,ShowRecordListPage.class);
		startActivity(intent);
		this.finish();
	}
    public void  toSetting (View v){
    	Intent intent = new Intent(this,NewSettingPage.class);
		startActivity(intent);
		this.finish();
}
}
