package com.firs.facedetecttosvr;

import com.firs.cn.FaceNative;

import android.R.integer;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.text.Editable;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.Menu;
import android.view.View;
import android.view.Window;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;

public class SetServerIP extends Activity {
	
	EditText edit_svrip;
	EditText edit_port;
	Button  bnt_setServer;
	Button bnt_return;
	
	private Thread newThread;         //����һ�����߳�
	public static ProgressBar  progressBar;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);//ȥ��������
		//getActionBar().setBackgroundDrawable(this.getResources().getDrawable(R.drawable.action_bar_bg));
		setContentView(R.layout.activity_setsvr);
		bnt_setServer = (Button)findViewById(R.id.btn_setserverip);
		bnt_return = (Button)findViewById(R.id.btn_return);
		edit_svrip = (EditText)findViewById(R.id.edit_server_ip);
		edit_port = (EditText)findViewById(R.id.edit_port);
		
		SharedPreferences sharedPreferences = getSharedPreferences("serversettings", this.MODE_PRIVATE);
		//2016.10.13    update (183.62.134.182)to(116.205.1.86)
		String serverip = sharedPreferences.getString("serverip", "116.205.1.86");//before��183.62.134.182��
		int port = sharedPreferences.getInt("port", 32108);
		edit_svrip.setText(serverip);
		edit_port.setText(Integer.toString(port));
		
		Editable ea = edit_svrip.getText();
		edit_svrip.setSelection(ea.length());
		
		progressBar=new ProgressBar(this,null,android.R.attr.progressBarStyleLarge);
		progressBar.setIndeterminateDrawable(getResources().getDrawable(R.drawable.progressbar));
		Display currDisplay = getWindowManager().getDefaultDisplay();//��ȡ��Ļ��ǰ�ֱ���
		int round = currDisplay.getWidth()/4;
        int displayWidth = currDisplay.getWidth()/2 - round/2;
        int displayHeight = currDisplay.getHeight()/4 - round/2-5;
        addContentView(progressBar, new LayoutParams(round,round));
        
        bnt_setServer.setWidth(round);
        bnt_return.setWidth(round);
        
		progressBar.setX(displayWidth);
		progressBar.setY(displayHeight);
		progressBar.setVisibility(View.INVISIBLE);

	}
	private Handler handler = new Handler()
	{
		Toast toast;
		boolean bSave;
		
		public void handleMessage(Message msg)
		{
			switch (msg.what){
				case 1:
					SharedPreferences settings = getSharedPreferences("serversettings",SetServerIP.MODE_PRIVATE);
					Editor editor = settings.edit();//��ȡ�༭��
					editor.putString("serverip", edit_svrip.getText().toString());
					editor.putInt("port", Integer.valueOf(edit_port.getText().toString()));
					bSave = editor.commit();//�ύ�޸�
					//Toast.makeText(this, "��������ɹ�!",
					//Toast.LENGTH_SHORT).show();
					if(true == bSave)
					{
						toast= Toast.makeText(getApplicationContext(), "���������óɹ�!", Toast.LENGTH_SHORT);
					}else {
						toast= Toast.makeText(getApplicationContext(), "��������ʧ��!", Toast.LENGTH_SHORT);
					}
					toast.setGravity(Gravity.CENTER, 0, 0);
					toast.show();
					break;
				case 2:
					toast= Toast.makeText(getApplicationContext(), "������ip����ʧ��,����ϵ����Ա��", Toast.LENGTH_SHORT);
					toast.setGravity(Gravity.CENTER, 0, 0);
					toast.show();
					break;
					
			}
			progressBar.setVisibility(View.INVISIBLE);
			bnt_setServer.setEnabled(true);
			super.handleMessage(msg);
		}

	};
	
	public void set_return(View v)
	{
		SetServerIP.this.finish();
	}
	//������ð�ť  
	public void  set_Server(View v) {
		bnt_setServer.setEnabled(false);
		progressBar.setVisibility(View.VISIBLE);
		//���ı��������ֵ���浽   ����
		FaceNative.SetServerIP(edit_svrip.getText().toString().getBytes(), Integer.valueOf(edit_port.getText().toString()), 1);
		new Thread() {
		@Override
		public void run() {
		     //����д�����߳���Ҫ���Ĺ���
			//timeout 60s
			long start_time = System.currentTimeMillis()/1000;
			while(true)
			{	//1.����ɹ�
				if(FaceNative.getServerSockFlag() == 1)
				{
					Message message = new Message();
					message.what = 1;
					handler.sendMessage(message);
					break;
				}
				//2.����ʱ���ӳ�ʱ
				if(System.currentTimeMillis()/1000 - start_time >= 30)
				{
					Message message = new Message();
					message.what = 2;
					handler.sendMessage(message);
					break;
				}
				 try {
				        Thread.sleep(1000);
				       } catch (InterruptedException e) {
				        e.printStackTrace();
			       }
			}
			
		   }
	   }.start();

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		//getMenuInflater().inflate(R.menu.login, menu);
		return true;
	}

}
