package com.firs.facedetecttosvr;


import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.Window;
import android.widget.TextView;

import com.firs.cn.MyDataBaseAdapter;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;

public class OtherSetPage extends Activity {
	
	TextView text_name;
	
	public static MyDataBaseAdapter database;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);//去掉标题栏
		//getActionBar().setBackgroundDrawable(this.getResources().getDrawable(R.drawable.action_bar_bg));
		setContentView(R.layout.main_tab_settings);
		//getActionBar().setBackgroundDrawable(this.getResources().getDrawable(R.drawable.action_bar_bg));
		//compareresult cpmrest = new compareresult();
		
		text_name = (TextView)findViewById(R.id.textName);
		
		SharedPreferences sharedPreferences = getSharedPreferences("useraccount", this.MODE_PRIVATE);
		String useraccount = sharedPreferences.getString("account", "admin");
		
		text_name.setText(useraccount);
		
		//创建数据库
        database = new MyDataBaseAdapter(this);
		
		//取得数据库对象 
        database.open();

	}
	@Override
    protected void onDestroy() {
        super.onDestroy();
        
        database.close();

    }
	
	public void set_server(View v)
	{
		Intent intent = new Intent(OtherSetPage.this,SetServerIP.class);
		startActivity(intent);
		//OtherSetPage.this.finish();
	}
	
	public void set_param(View v)
	{
		Intent intent = new Intent(OtherSetPage.this,SettingPage.class);
		startActivity(intent);
		//OtherSetPage.this.finish();
	}
	public void Check_record(View v)
	{
		Intent intent = new Intent(OtherSetPage.this,ShowRecordListPage.class);
		startActivity(intent);
		//OtherSetPage.this.finish();
	}
	
	public void ShowPicture(View v)
	{
		Intent intent = new Intent(OtherSetPage.this,GridViewActivity.class);
		startActivity(intent);
		//OtherSetPage.this.finish();
	}
	
	public void clear_record(View v)
	{
		Dialog dialog = new AlertDialog.Builder(this)
		.setTitle("提示")//设置标题
		.setMessage("是否要清除所有记录?")//设置内容
		.setPositiveButton("确定",//设置确定按钮
		new DialogInterface.OnClickListener() 
		{
			public void onClick(DialogInterface dialog, int whichButton)
			{
				//点击“确定”
				database.clearRecordInfoData();
				File dir=new File(MyApp.FACE_PATH);
				File[] lst=dir.listFiles();
				for (File f:lst){
					f.delete();
				}
				dir=new File(MyApp.FACE_TEMP_PATH);
				lst=dir.listFiles();
				for (File f:lst){
					f.delete();
				}
				//initData();
	            //listView.setAdapter(new MyAdapter());
			}
		}).setNeutralButton("取消", 
		new DialogInterface.OnClickListener() 
		{
			public void onClick(DialogInterface dialog, int whichButton)
			{
				//
			}
		}).create();//创建按钮
	
		// 显示对话框
		dialog.show();
		//OtherSetPage.this.finish();
	}
	
	public void back_mainpage(View v)
	{
		Dialog dialog = new AlertDialog.Builder(this)
		.setTitle("提示")//设置标题
		.setMessage("是否要退出?")//设置内容
		.setPositiveButton("确定",//设置确定按钮
		new DialogInterface.OnClickListener() 
		{
			public void onClick(DialogInterface dialog, int whichButton)
			{
				//点击“确定”
				Intent in = new Intent();
		         setResult(101, in );
				 OtherSetPage.this.finish();
			}
		}).setNeutralButton("取消", 
		new DialogInterface.OnClickListener() 
		{
			public void onClick(DialogInterface dialog, int whichButton)
			{
				//
			}
		}).create();//创建按钮
	
		// 显示对话框
		dialog.show();
	}
	
	public void about_page(View v)
	{
		Intent intent = new Intent(OtherSetPage.this,AboutPage.class);
		startActivity(intent);
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
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		//getMenuInflater().inflate(R.menu.login, menu);
		return true;
	}

}
