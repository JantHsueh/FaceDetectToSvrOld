package com.firs.facedetecttosvr;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

public class NewSettingPage extends Activity {
    private ImageView m1_im1,m1_im2;
    private ImageView m2_im1,m2_im2;
    private ImageView m3_im1,m3_im2,m3_im3,m3_im1_1,m3_im2_2,m3_im3_3;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_new_setting_page);
		initView();
	}
	private void initView() {
		m1_im1=(ImageView)findViewById(R.id.m1_im1);
		m1_im2=(ImageView)findViewById(R.id.m1_im2);
		m2_im1=(ImageView)findViewById(R.id.m2_im1);
		m2_im2=(ImageView)findViewById(R.id.m2_im2);
		m3_im1=(ImageView)findViewById(R.id.m3_im1);
		m3_im2=(ImageView)findViewById(R.id.m3_im2);
		m3_im1=(ImageView)findViewById(R.id.m3_im1);
		m3_im1_1=(ImageView)findViewById(R.id.m3_im1_1);
		m3_im2_2=(ImageView)findViewById(R.id.m3_im2_2);
		m3_im3_3=(ImageView)findViewById(R.id.m3_im3_3);
	}

   public void goBack(View v){
	   NewSettingPage.this.finish(); 
   }

}
