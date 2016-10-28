package com.firs.facedetecttosvr;


import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.firs.cn.MyDataBaseAdapter;
import com.firs.view.CalendarView;
import com.firs.view.ExcelUtils;

public class ShowRecordListPage extends Activity implements OnClickListener{
	//日期组件
	private ImageButton calendar_Left,calendar_Right;
	private TextView calendar_Center1;
	private CalendarView calendar;
	public Date date;
	public String CalendarDate=null,SQLtimes=null,input=null;//日历点击的日期
	//创建listview
	private ListView listView;
	private List<Map<String, String>> items;
	private Map<String, String> map = null;
	private HashMap<String, String> Allmap =new  HashMap<String, String>();
	private List<Map<String, String>> Allitems=new ArrayList<Map<String, String>>();//输入关键字的查询集合
	private List<Map<String, String>>  getAll=null;
	private final static String DATETIME = "datetime";
	private final static String SCORE = "score";
	private final static String NAME = "name";
	private final static String STATUS = "status";
	private int   listCount = 23;
	//右侧
	private ImageView imageView1,imageView2;

	public static MyDataBaseAdapter database;
	//导出excel数据
	private Button bt1,search_bt;
	private EditText search_input;
	private File file;
	private String[] title = { "姓名", "日期", "状态", "相识度"};
	private String[] saveData;//输入的数据源
	ArrayList<Integer> numberList=new ArrayList<Integer>();
	List<String> beanList=new ArrayList<String>();

	public ShowRecordListPage() {
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);//去掉标题栏
		setContentView(R.layout.activity_list);
		//创建数据库
		database = new MyDataBaseAdapter(this);
		//取得数据库对象
		database.open();
		initView();
		if(CalendarDate==null || CalendarDate.equals("")){
			Date data=new Date();
			SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd");
			CalendarDate=formatter.format(data);
			//Toast.makeText(ShowRecordListPage.this ,"当前日期："+CalendarDate, Toast.LENGTH_LONG).show();
		}


	}

	private void initView() {
		calendar_Left = (ImageButton)findViewById(R.id.calendar_Left);
		calendar_Right = (ImageButton)findViewById(R.id.calendar_Right);
		calendar_Center1 = (TextView)findViewById(R.id.calendar_Center1);
		calendar = (CalendarView)findViewById(R.id.calendar);
		listView = (ListView)findViewById(R.id.listView1);

		imageView1 = (ImageView)findViewById(R.id.imageView1);
		imageView2 = (ImageView)findViewById(R.id.imageView2);

		search_input=(EditText)findViewById(R.id.search_input);//搜索输入框
		search_bt=(Button)findViewById(R.id.search_bt);//搜索按钮
		bt1=(Button)findViewById(R.id.bt1);

		String[] YearAndMonth = calendar.getYearAndmonth().split("-");  //获取日历中年月 ya[0]为年，ya[1]为月（格式大家可以自行在日历控件中改）
		calendar_Center1.setText(YearAndMonth[0]+YearAndMonth[1]);

		search_bt.setOnClickListener(this);
		calendar_Left.setOnClickListener(this);
		calendar_Right.setOnClickListener(this);
		bt1.setOnClickListener(this);
		calendar.setOnItemClickListener(new calendarItemClick());

		listView.setClickable(true);
		listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int position, long arg3) {
				Object o = listView.getItemAtPosition(position);
				final ImageView isCheck= (ImageView) arg1.findViewById(R.id.isCheck);
				isCheck.setImageResource(R.drawable.bt_n);
				numberList.add(position);//点击 的某一项添加到数组


				showItemInfo(items.get(position).get(DATETIME));
			}
		});
	}
	private void showItemInfo(String datetime) {
		try {
			SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			String str = Long.toString(formatter.parse(datetime).getTime());
			Cursor cur = database.fetchCheckRecordInfoData(str);
			if(cur != null && cur.getCount() == 1) {
				Bitmap bitmap = getLoacalBitmap(MyApp.FACE_PATH+cur.getString(0)+".jpg"); //从本地取图片
				Bitmap bitmap2 = getLoacalBitmap(MyApp.FACE_PATH + cur.getString(0) + cur.getShort(3)+".jpg"); //从本地取图片
				imageView1.setImageBitmap(bitmap2);	//设置数据库Bitmap
				imageView2.setImageBitmap(bitmap);	//设置照相存入的Bitmap

			}
		} catch (ParseException e) {e.printStackTrace();}//获取当前时间

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
	public  boolean deleteFile(String fileName){
		File file = new File(fileName);
		if(file.isFile() && file.exists()){
			file.delete();
			return true;
		}else{
			return false;
		}
	}

	//单击事件
	@Override
	public void onClick(View v) {
		switch (v.getId()) {
			case R.id.calendar_Left:
				//点击上一月 同样返回年月
				String leftYearAndmonth = calendar.clickLeftMonth();
				String[] YearAndMonth = leftYearAndmonth.split("-");
				calendar_Center1.setText(YearAndMonth[0]+YearAndMonth[1]);
				break;
			case R.id.calendar_Right:
				//点击下一月
				String rightYearAndmonth = calendar.clickRightMonth();
				String[] YearAndMonth1 = rightYearAndmonth.split("-");
				calendar_Center1.setText(YearAndMonth1[0]+YearAndMonth1[1]);
				break;
			case R.id.bt1://导出数据
				file = new File(getSDPath() + "/RunVision");
				makeDir(file);//创建文件
				ExcelUtils.initExcel(file.toString() + "/worlk.xls", title);//创建表
				//写入数据信息     1. 数据集合   2.SDCARD路径    getAll/items
				ExcelUtils.writeObjListToExcel(beanList, getSDPath() + "/RunVision/worlk.xls", this);//"/Family/bill.xls"
				break;
			case R.id.search_bt://条件查询
				input=search_input.getText().toString();
				if(input!=null&&!input.equals("")&&!input.isEmpty()){//输入有内容
					searchToInput();
				}else{
					Toast.makeText(ShowRecordListPage.this ,"请输入查询关键字", Toast.LENGTH_LONG).show();
				}
				break;
			default:
				break;
		}

	}
//	//搜索关键字 查询数据库
	private void searchToInput() {
		getAll = new ArrayList<Map<String, String>>();
        //遍历items数据，看每一个item（map对象）中是否包含输入的这个值。如果有，则提取出来放入getAll的集合中。填充到适配器
		for(int i=0;i<items.size();i++) {
			if (items.get(i).containsValue(input)) {
				getAll.add(items.get(i));//点击日历时，清空集合
			} else {
			}
			if(CalendarDate.equals(SQLtimes)) {//时间是否是点击的时间
				if (getAll != null) {
					listView.setVisibility(View.VISIBLE);
					listView.setAdapter(new MyAdapter(getAll));
					new MyAdapter(getAll).notifyDataSetChanged();
				}
			}




		}


	}


	//日期的点击事件
	class calendarItemClick implements com.firs.view.CalendarView.OnItemClickListener{
		@Override
		public void OnItemClick(Date date) {
//			if(getAll!=null) {
//				getAll.clear();//清除搜索数据
//			}
			ShowRecordListPage.this.date=date;
			SimpleDateFormat  fomt=new SimpleDateFormat("yyyy-MM-dd");
			CalendarDate=fomt.format(date);//根据日期去条件查询当天的记录
			Date nowdata=new Date(); //今天
			String tady=fomt.format(date);
			Toast.makeText(ShowRecordListPage.this, "您选择的日期是："+CalendarDate, 0).show();
			if(!CalendarDate.equals(SQLtimes)){
				listView.setVisibility(View.GONE);
			}else{
				listView.setVisibility(View.VISIBLE);
				initData();
				//new MyAdapter().notifyDataSetChanged();
			}

		}

	}
	public void goBack(View v){
		this.finish();
	}
	//导出数据

	public String getSDPath() {
		//判断sdk路径是否存在，存在则输出器路径
		File sdDir = null;
		boolean sdCardExist = Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED);
		if (sdCardExist) {
			sdDir = Environment.getExternalStorageDirectory();
		}
		String dir = sdDir.toString();
		return dir;
	}
	public static void makeDir(File dir) {
		if (!dir.getParentFile().exists()) {
			makeDir(dir.getParentFile());
		}
		dir.mkdir();
		//判断文件是否存在
	}

	@Override
	protected void onResume() {
		super.onResume();
		initData();
		//listView.setAdapter(new MyAdapter());
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		database.close();
	}
	class MyAdapter extends BaseAdapter{
		private ViewHolder01 mHolder01;
		private Map<Integer, View> viewMap;
		private LayoutInflater inflater;
		private List<Map<String, String>> items;
		 public MyAdapter(List<Map<String, String>> items) {
			viewMap = new HashMap<Integer, View>();
			inflater = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			 this.items=items;
		}
		@Override
		public int getCount() {
			return items.size();
		}
		@Override
		public Object getItem(int position) {
			return items.get(position);
		}
		@Override
		public long getItemId(int position) {
			return position;
		}
		@Override
		public View getView(int position, View view, ViewGroup arg2) {
			view = viewMap.get(position);

			if (view == null) {
				view = getview(view,position);
			} else {
				getTagHolder(view,position);
			}
			setItemInfo(position);//上数据

			return view;
		}

		private View getview(View view, int position){
			mHolder01 = new ViewHolder01();
			view = inflater.inflate(R.layout.mylist_item, null);
			mHolder01.isCheck= (ImageView)view.findViewById(R.id.isCheck);
			mHolder01.textView01 = (TextView) view.findViewById(R.id.item01_txt);
			mHolder01.textView02 = (TextView) view.findViewById(R.id.item02_txt);
			mHolder01.textView03 = (TextView) view.findViewById(R.id.item03_txt);
			mHolder01.textView04 = (TextView) view.findViewById(R.id.item04_txt);

			view.setTag(mHolder01);
			viewMap.put(position, view);
			return view;
		}

		private void getTagHolder(View view ,int position){
			mHolder01 = (ViewHolder01) view.getTag();
		}

		private void setItemInfo(int position){
			mHolder01.textView01.setText(items.get(position).get(NAME));
			String[] str=items.get(position).get(DATETIME).split(" ");
			mHolder01.textView02.setText(str[1]);
			mHolder01.textView03.setText(items.get(position).get(STATUS));
			mHolder01.textView04.setText(items.get(position).get(SCORE));

		}

		class ViewHolder01 {
			private TextView textView01;
			private TextView textView02;
			private TextView textView03;
			private TextView textView04;
			private ImageView isCheck;
		}
	}
	//处理记录listview
	private void initData(){
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		String str;
		Cursor cur = database.fetchRecordInfoData();

		if(cur != null)
		{
			int cou = cur.getCount();
			MyApp.log("record count[]"+cou);
			int colCount = cur.getColumnCount();

			if(items != null)
			{
				items.clear();
				//log("------------items.clear()");
			}
			else
			{
				items =  new ArrayList<Map<String,String>>();
			}
			cur.moveToLast();

			for (int i = 0; i < cou; i++) {
				map = new HashMap<String, String>();
				Date    curDate = new Date(Long.parseLong(cur.getString(0)));//获取当前时间
				str = formatter.format(curDate);//数据库查询到的数据 日期
				String[] times=str.split(" ");
				SQLtimes=times[0];
				//times[0] 年份 如果日期选择的年份==数据库查到的年份，显示到界面
				//Toast.makeText(ShowRecordListPage.this, CalendarDate+"数据库日期是："+times[0], Toast.LENGTH_LONG).show();
				//2016.10.23
				Allmap.put(DATETIME, str);
				Allmap.put(SCORE, cur.getString(1));
				Allmap.put(NAME, cur.getString(2));
				Allmap.put(STATUS, "验证通过");
				Allitems.add(Allmap);
				if(CalendarDate.equals(SQLtimes)){//条件查询数据库
					map.put(DATETIME, str);
					map.put(SCORE, cur.getString(1));
					map.put(NAME, cur.getString(2));
					map.put(STATUS, "验证通过");
					items.add(map);
					beanList.add(cur.getString(2).toString()+str.toString()+"验证通过"+cur.getString(1).toString());
					listView.setAdapter(new MyAdapter(items));

				}
				//2016.10.25修改了一行代码
				if(!cur.isFirst()){
					cur.moveToPrevious();
				}

			}
		}
	}

//	@Override
//	public boolean onOptionsItemSelected(MenuItem item) {
//		//log("onOptionsItemSelected()");
//		switch (item.getItemId()) {
//
//			case R.id.deleteall : {
//				Dialog dialog = new AlertDialog.Builder(this)
//						.setTitle("提示")//设置标题
//						.setMessage("是否要清除所有记录?")//设置内容
//						.setPositiveButton("确定",//设置确定按钮
//								new DialogInterface.OnClickListener()
//								{
//									public void onClick(DialogInterface dialog, int whichButton)
//									{
//										//点击“确定”
//										database.clearRecordInfoData();
//										File dir=new File(MyApp.FACE_PATH);
//										File[] lst=dir.listFiles();
//										for (File f:lst){
//											f.delete();
//										}
//										dir=new File(MyApp.FACE_TEMP_PATH);
//										lst=dir.listFiles();
//										for (File f:lst){
//											f.delete();
//										}
//										initData();
//										listView.setAdapter(new MyAdapter());
//									}
//								}).setNeutralButton("取消",
//								new DialogInterface.OnClickListener()
//								{
//									public void onClick(DialogInterface dialog, int whichButton){
//									}
//								}).create();//创建按钮
//
//				// 显示对话框
//				dialog.show();
//
//				break;
//			}
//			default :
//				break;
//		}
//		return super.onOptionsItemSelected(item);
//	}



}
