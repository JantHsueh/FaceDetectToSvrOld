package com.firs.facedetecttosvr;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.Editable;
import android.view.Display;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.firs.cn.FaceNative;

public class Login extends Activity {
	EditText edit_account;
	EditText edit_pwd;
	CheckBox cb_pwd;
	Button  bnt_login;
	Button  bnt_setting;
	TextView  bnt_register;
	RelativeLayout viewRayout;
	String serverip;
	int port;
	boolean bSave = false;
	private Thread newThread;         //声明一个子线程
	public static ProgressBar  progressBar;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);//去掉标题栏
		setContentView(R.layout.activity_login);

		viewRayout = (RelativeLayout)findViewById(R.id.RelativeLayout02);

		edit_account = (EditText)findViewById(R.id.login_edit_account);
		edit_pwd = (EditText)findViewById(R.id.login_edit_pwd);
		cb_pwd = (CheckBox)findViewById(R.id.login_cb_savepwd);
		bnt_login = (Button)findViewById(R.id.login_btn_login);
		bnt_setting = (Button)findViewById(R.id.btn_set);
		bnt_register = (TextView)findViewById(R.id.btn_register);

		progressBar=new ProgressBar(this,null,android.R.attr.progressBarStyleLarge);

		progressBar.setIndeterminateDrawable(getResources().getDrawable(R.drawable.progressbar));
		Display currDisplay = getWindowManager().getDefaultDisplay();//获取屏幕当前分辨率
		int round = currDisplay.getWidth()/4;
		int displayWidth = currDisplay.getWidth()/2 - round/2;
		int displayHeight = currDisplay.getHeight()/4 - round/2-5;
		addContentView(progressBar, new LayoutParams(round,round));
		progressBar.setX(displayWidth);
		progressBar.setY(displayHeight);
		progressBar.setVisibility(View.INVISIBLE);

		// LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
		LinearLayout.LayoutParams layoutParams = (android.widget.LinearLayout.LayoutParams) viewRayout.getLayoutParams();
		//layoutParams.setMargins(15,currDisplay.getHeight()/4,15,0);//4个参数按顺序分别是左上右下
		layoutParams.topMargin = currDisplay.getHeight()*2/5;
		layoutParams.height = currDisplay.getHeight()/3+15;
		viewRayout.setLayoutParams(layoutParams);


		bnt_login.setWidth(round);
		bnt_setting.setWidth(round);
		bnt_register.setWidth(round);

		/*
		SharedPreferences sharedPreferences = getSharedPreferences("serversettings", this.MODE_PRIVATE);
		serverip = sharedPreferences.getString("serverip", "192.168.1.1");
		port = sharedPreferences.getInt("port", 8080);
		*/

		SharedPreferences sharedPreferences = getSharedPreferences("useraccount", this.MODE_PRIVATE);
		String useraccount = sharedPreferences.getString("account", "test");
		String pwd = sharedPreferences.getString("pwd", "123456");
		String saveflag = sharedPreferences.getString("saveflag", "0");

		edit_account.setText(useraccount);
		edit_pwd.setText(pwd);
		if(saveflag.equals("0"))
		{
			cb_pwd.setChecked(true);
		}
		else{
			cb_pwd.setChecked(false);
		}

		Editable ea = edit_account.getText();
		edit_account.setSelection(ea.length());

		SharedPreferences sharedPreferences2 = getSharedPreferences("serversettings", this.MODE_PRIVATE);
		//2016.10.14      befor  183.62.134.182            修改  服务地址为116.205.1.86
		String serverip = sharedPreferences2.getString("serverip", "116.205.1.86");
		int port = sharedPreferences2.getInt("port", 32108);
		FaceNative.SetServerIP(serverip.getBytes(), port, 0);
		//log("create MyApp  ................");
		//FaceNative.initTcp();

		SharedPreferences sharedPreferences3= getSharedPreferences("setting", this.MODE_PRIVATE);
		int sorce = Integer.valueOf(sharedPreferences3.getString("score", "60"));
		FaceNative.SetScore(sorce);

	}
	@Override
	protected void onResume() {
		//Log.e("11","onResume()");
		super.onResume();
		FaceNative.initTcp();
	}

	@Override
	protected void onPause() {
		super.onPause();
		if(false == bSave)
		{
			FaceNative.setThreadExit();
		}
		//Log.e("11","onPause()");
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

	        /*if(false == bSave)
	        {
	        	FaceNative.setThreadExit();
	        }
	        */
		// Log.e("11","onDestroy()");
	}

	private Handler handler = new Handler()
	{
		Toast toast;
		public void handleMessage(Message msg)
		{
			switch (msg.what)
			{
				case 1:

					//验证通过，保存用户名和密码
					if(cb_pwd.isChecked() &&(true == bSave))
					{
						//save account and pwd
						SharedPreferences settings = getSharedPreferences("useraccount",Login.MODE_PRIVATE);
						Editor editor = settings.edit();//获取编辑器
						editor.putString("account", edit_account.getText().toString());
						editor.putString("pwd", edit_pwd.getText().toString());
						editor.putString("saveflag", "0");
						editor.commit();//提交修改


					}
					//没有记住密码
					if(!cb_pwd.isChecked())
					{
						//save account and pwd
						SharedPreferences settings = getSharedPreferences("useraccount",Login.MODE_PRIVATE);
						Editor editor = settings.edit();//获取编辑器
						editor.putString("account", edit_account.getText().toString());
						editor.putString("pwd", "");
						editor.putString("saveflag", "1");
						editor.commit();//提交修改
					}
					progressBar.setVisibility(View.INVISIBLE);

					bnt_login.setFocusable(true);
					bnt_login.setClickable(true);

					if(FaceNative.getAuth() == 1)
					{
						Intent intent = new Intent(Login.this,MainActivity.class);
						startActivity(intent);
						Login.this.finish();
						toast= Toast.makeText(getApplicationContext(), "登录成功!", Toast.LENGTH_SHORT);
					}else {
						if(FaceNative.getAuth() == -1)
						{
							toast= Toast.makeText(getApplicationContext(), "登录失败!用户名和密码不匹配。", Toast.LENGTH_SHORT);
						}
						else if(FaceNative.getAuth() == 2)
						{
							toast= Toast.makeText(getApplicationContext(), "登录超时失败!请重新登录。", Toast.LENGTH_SHORT);
						}
						else {
							toast= Toast.makeText(getApplicationContext(), "服务器连接失败!请重新登录。", Toast.LENGTH_SHORT);
						}
					}
					toast.setGravity(Gravity.CENTER, 0, 0);
					toast.show();
					break;
			}
			super.handleMessage(msg);
		}

	};
	public void userRegister(View v)
	{
		Toast toast;
		toast= Toast.makeText(getApplicationContext(), "注册功能暂时还没开通，请关注官方消息。", Toast.LENGTH_SHORT);
		toast.setGravity(Gravity.CENTER, 0, 0);
		toast.show();
	}
	public void setServerIp(View v)//设置
	{
		Intent intent = new Intent(Login.this,SetServerIP.class);
		startActivity(intent);
	}
	public void saveAccountPwd(View v) {

		bnt_login.setFocusable(false);
		bnt_login.setClickable(false);
		progressBar.setVisibility(View.VISIBLE);
		/*
		try {
	        Thread.sleep(1000);
	       } catch (InterruptedException e) {
	        // TODO Auto-generated catch block
	        e.printStackTrace();
       }
		*/
		/*
		if(FaceNative.getAuth() == 1)
		{
			Message message = new Message();
			message.what = 1;
			handler.sendMessage(message);
			bSave = true;
			return ;
		}
		*/
		FaceNative.UserAuth(edit_account.getText().toString().getBytes(), edit_pwd.getText().toString().getBytes());

		new Thread() {
			@Override
			public void run() {

				//这里写入子线程需要做的工作
				//timeout 30s
				long start_time = System.currentTimeMillis()/1000;
				while(true)
				{
					//Log.e("1111111111111111","System.currentTimeMillis()/1000 - start_time");
					if(FaceNative.getAuth() == 1)
					{
						bSave = true;
						break;
					}

					if(FaceNative.getAuth() == 2)
					{
						bSave = false;
						break;
					}

					if(System.currentTimeMillis()/1000 - start_time >= 30)
					{
						bSave = false;
						break;
					}
					try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
				Message message = new Message();
				message.what = 1;
				handler.sendMessage(message);

			}
		}.start();
	}



}
