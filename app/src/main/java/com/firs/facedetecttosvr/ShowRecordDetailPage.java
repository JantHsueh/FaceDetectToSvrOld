package com.firs.facedetecttosvr;


import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

import com.firs.cn.FaceNative;
import com.firs.cn.MyDataBaseAdapter;
import com.firs.cn.compareresult;
import com.firs.facedetecttosvr.R.id;

import android.R.integer;
import android.os.Bundle;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.Menu;
import android.view.View;
import android.view.Window;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

public class ShowRecordDetailPage extends Activity {
	TextView text_score,text_name,text_time;
	String    str,str1;
	public static MyDataBaseAdapter database;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);//ȥ��������
		setContentView(R.layout.activity_record_detail);
		Display currDisplay = getWindowManager().getDefaultDisplay();//��ȡ��Ļ��ǰ�ֱ���
		((Button)findViewById(R.id.btnSave)).setWidth(currDisplay.getWidth()/4);
		((Button)findViewById(R.id.btnUnSave)).setWidth(currDisplay.getWidth()/4);
        database = new MyDataBaseAdapter(this);//�������ݿ�
		
		//ȡ�����ݿ���� 
        database.open();
		
		text_score = (TextView)findViewById(R.id.textScore);
		text_name = (TextView)findViewById(R.id.textName);
		text_time = (TextView)findViewById(R.id.textTime);
		
		//edit_remark = (EditText)findViewById(R.id.editRemarks);
		
		Intent myIntent= getIntent(); // gets the previously created intent
		String datetime = myIntent.getStringExtra("DATETIME"); // will return "FirstKeyValue"
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
 		
		try {
			str = Long.toString(formatter.parse(datetime).getTime());
		} catch (ParseException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}//��ȡ��ǰʱ��       
		 Cursor cur = database.fetchCheckRecordInfoData(str);
		if(cur != null && cur.getCount() == 1)
        {
			MyApp.log("cur.count:"+cur.getCount()+ "timelong"+str);
			text_score.setText("���ƶ�: "+cur.getString(1)+" %");
			text_name.setText("ע������: "+cur.getString(2));
			
			Date    curDate = new Date(Long.parseLong(cur.getString(0)));//��ȡ��ǰʱ��       
	 		str1 = formatter.format(curDate);
			text_time.setText("�ȶ�ʱ�䣺"+str1);
			//edit_remark.setText(cur.getString(5));
			
			ImageView image1 = (ImageView) findViewById(R.id.imageView1);
			ImageView image2 = (ImageView) findViewById(R.id.imageView2);
		    Bitmap bitmap = getLoacalBitmap(MyApp.FACE_PATH+cur.getString(0)+".jpg"); //�ӱ���ȡͼƬ
		    Bitmap bitmap2 = getLoacalBitmap(MyApp.FACE_PATH + cur.getString(0) + cur.getShort(3)+".jpg"); //�ӱ���ȡͼƬ
		    image1.setImageBitmap(bitmap);	//����Bitmap
		    image2.setImageBitmap(bitmap2);	//����Bitmap
        } else
		{
		 	text_score.setText("���ƶ�: ");
			text_name.setText("ע������: "+"");
			
			//SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");       
	 		Date    curDate1 = new Date(System.currentTimeMillis());//��ȡ��ǰʱ��       
	 		str1 = formatter.format(curDate1);
			//database.insertRecordInfoData(str, smilar.toString(), "wifi", "no");
			text_time.setText("�ȶ�ʱ�䣺"+str1);
			
		}
	
	}
	 @Override
	    protected void onPause() {
        super.onPause();
	    }
	 @Override
	    protected void onDestroy() {
	        super.onDestroy();
	        
	        database.close();

	    }
	
	public void deleteRecord(View v)
	{
		Dialog dialog = new AlertDialog.Builder(this)
		.setTitle("��ʾ")//���ñ���
		.setMessage("�Ƿ�Ҫɾ���ü�¼?")//��������
		.setPositiveButton("ȷ��",//����ȷ����ť
		new DialogInterface.OnClickListener() 
		{
			public void onClick(DialogInterface dialog, int whichButton)
			{
				Cursor cur = database.fetchCheckRecordInfoData(str);
				if(cur != null && cur.getCount() == 1)
		        {
					deleteFile(MyApp.FACE_PATH+cur.getString(0)+".jpg");
					deleteFile(MyApp.FACE_PATH + cur.getString(0) + cur.getShort(3)+".jpg");
		        }
				//�����ȷ����
				database.deleteRecordInfoData(str);				
				ShowRecordDetailPage.this.finish();
			}
		}).setNeutralButton("ȡ��", 
		new DialogInterface.OnClickListener()
		{
			public void onClick(DialogInterface dialog, int whichButton)
			{
				//
			}
		}).create();//������ť
	
		// ��ʾ�Ի���
		dialog.show();
	}
	public void returnBack(View v)
	{
		
		ShowRecordDetailPage.this.finish();
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
	public  boolean deleteFile(String fileName){  
	   File file = new File(fileName);  
	   if(file.isFile() && file.exists()){  
		       file.delete();  
	      return true;
	     }else{  
			  return false;
		  }  
	  }


}
