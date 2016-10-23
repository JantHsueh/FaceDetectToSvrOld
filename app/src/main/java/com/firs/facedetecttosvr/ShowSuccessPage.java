package com.firs.facedetecttosvr;


import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

import com.firs.cn.FaceNative;
import com.firs.cn.MyDataBaseAdapter;
import com.firs.cn.compareresult;
import com.firs.facedetecttosvr.R.id;

import android.R.integer;
import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.Display;
import android.view.Gravity;
import android.view.Menu;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class ShowSuccessPage extends Activity {
	
//	Button btn_save;
//	Button btn_unSave;
//	TextView text_score,text_name,text_time;
	//EditText edit_remark;
	String    str;
	long lTime;
	
	public static MyDataBaseAdapter database;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);//���ر��� 
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
		WindowManager.LayoutParams.FLAG_FULLSCREEN); // ����״̬��
		//getActionBar().setBackgroundDrawable(this.getResources().getDrawable(R.drawable.action_bar_bg));
		setContentView(R.layout.activity_show);
		compareresult cpmrest = new compareresult();
		
		Display currDisplay = getWindowManager().getDefaultDisplay();//��ȡ��Ļ��ǰ�ֱ���
//		btn_save = (Button)findViewById(R.id.btnSave);
//		btn_unSave = (Button)findViewById(R.id.btnUnSave);
//		btn_save.setWidth(currDisplay.getWidth()/4);
//		btn_unSave.setWidth(currDisplay.getWidth()/4);
		
//		text_score = (TextView)findViewById(R.id.textScore);
//		text_name = (TextView)findViewById(R.id.textName);
//		text_time = (TextView)findViewById(R.id.textTime);
		
		//edit_remark = (EditText)findViewById(R.id.editRemarks);
		
		FaceNative.getCompareResult(cpmrest);//��ȡ����ȶԽ��
//		text_score.setText("���ƶ�: "+cpmrest.score/1000000+" %");
//		text_name.setText("ע������: "+cpmrest.name);
		
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		lTime = (System.currentTimeMillis()/1000)*1000;
 		Date    curDate = new Date(lTime);//��ȡ��ǰʱ��       
 		str = formatter.format(curDate);
		//database.insertRecordInfoData(str, smilar.toString(), "wifi", "no");
//		text_time.setText("�ȶ�ʱ�䣺"+str);
		
		ImageView image1 = (ImageView) findViewById(R.id.imageView1);
		ImageView image2 = (ImageView) findViewById(R.id.imageView2);
	    Bitmap bitmap = getLoacalBitmap("/data/data/com.firs.facedetecttosvr/temp/facePic_temp.jpg"); //�ӱ���ȡͼƬ
	    Bitmap bitmap2 = getLoacalBitmap("/data/data/com.firs.facedetecttosvr/temp/"+cpmrest.faceId+".jpg"); //�ӱ���ȡͼƬ
	    image1.setImageBitmap(bitmap);	//����Bitmap
	    image2.setImageBitmap(bitmap2);	//����Bitmap
	    
	    copyFile("/data/data/com.firs.facedetecttosvr/temp/facePic_temp.jpg",MyApp.FACE_PATH+Long.toString(lTime)+".jpg");
	     
	   //�������ݿ�
        database = new MyDataBaseAdapter(this);
		
		//ȡ�����ݿ���� 
        database.open();
	}
	
	 @Override
	    protected void onDestroy() {
	        super.onDestroy();
	        
	        database.close();

	    }
	
	public void unsaveRecord()
	{
		ShowSuccessPage.this.finish();
	}
	
	public void  saveRecord() {
		Toast toast;
		boolean bSave = true;
		compareresult cpmrest = new compareresult();
		FaceNative.getCompareResult(cpmrest);
		 MyApp.log("Long.toString(lTime)"+Long.toString(lTime));
		 //�������ݿ� 
		
		database.insertRecordInfoData(Long.toString(lTime), Integer.toString(cpmrest.score/1000000), cpmrest.name,  Integer.toString(cpmrest.faceId),
				"facePic_temp.jpg","","others");
		copyFile("/data/data/com.firs.facedetecttosvr/temp/"+cpmrest.faceId+".jpg",MyApp.FACE_PATH+Long.toString(lTime)+cpmrest.faceId+".jpg");
		if(true == bSave){
			toast= Toast.makeText(getApplicationContext(), "��¼���汣��ɹ�!", Toast.LENGTH_SHORT);
		}else {
			toast= Toast.makeText(getApplicationContext(), "��¼����ʧ��!", Toast.LENGTH_SHORT);
		}
		toast.setGravity(Gravity.CENTER, 0, 0);
		toast.show();
		setResult(1);
		ShowSuccessPage.this.finish();
	}
	/**
	* ���ر���ͼƬ
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
	public static  void copyFile(String LaiYuanLuJing, String MuDiLuJing)
	  {
	    FileInputStream source;
	    FileOutputStream target;
	    try
	    {
	      File f1 = new File(LaiYuanLuJing);
	      File f2 = new File(MuDiLuJing.substring(0,MuDiLuJing.lastIndexOf("/")));
	      if(!f2.exists())
	      {
	        f2.mkdirs();
	      }
	      f2 = new File(MuDiLuJing);
	      source = new FileInputStream(f1);
	      target = new FileOutputStream(f2);
//	      BufferedInputStream bin = new BufferedInputStream(source);
//	      BufferedOutputStream bout = new BufferedOutputStream(target);
	      byte[] b=new byte[1024*5];
	      int data;
	      while ( (data = source.read(b)) != -1)
	      {
	        target.write(b,0,data);
//	        bout.write(data,0,data);
	      }
	      source.close();
	      target.close();
	    }
	    catch (Exception ex)
	    {
	      System.out.print(ex.getMessage());
	    }
	  }
	
	public void  goBack (View v){
		//ShowSuccessPage.this.finish();
		saveRecord();//�����¼  ��������ʾ
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
