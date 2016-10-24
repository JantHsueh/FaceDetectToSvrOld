package com.firs.cn;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;


import com.firs.facedetecttosvr.MyApp;

public class MyDataBaseAdapter {
	// 用于打印log
	private static final String TAG = "MyDataBaseAdapter";

	// 表中一条数据的名称
	public static final String KEY_ID = "_id";

	// 表中一条数据的内容
	public static final String KEY_NUM = "num";

	// 表中一条数据的id
	public static final String KEY_DATA = "data";

	// 数据库名称为data
	private static final String DB_NAME = "facedetect.db";

	
	/*
	//用户信息数据库表名
	private static final String	DB_USER_INFO_TABLE		= "user_info";
	
	public static final String	USER_NICK		= "user_nick";
	public static final String	USER_NAME		= "user_name";
	public static final String	USER_SEX		= "user_sex";
	public static final String	USER_PHONE_NUM		= "user_phone_num";
	*/


	//记录数据库表名
	private static final String DB_RECORD_TABLE = "record_info";

	public static final String TIME_RECORD = "time_record";
	public static final String SCORE_RECORD = "score_record";
	public static final String FACE_NAME = "face_name";
	public static final String FACE_ID = "face_id";
	public static final String FACE_DETECT = "facedetect";
	public static final String REMARKS2 = "remarks2";
	public static final String OTHERSNAME = "othersname";

	
	/*
	//记录数据库表名
	private static final String	DB_CHECK_TYPE_TABLE	= "check_type";
	
	public static final String	WIFI_TYPE	= "wifi";
	public static final String	GPS_TYPE	= "gps";
	public static final String	CELL_TYPE	= "cell";
	public static final String	SVR_TYPE	= "svr";
	*/


	// 数据库版本
	private static final int DB_VERSION = 1;

	// 本地Context对象
	private Context mContext = null;

	
	/*
	//创建用户信息表
	private static final String	DB_USER_INFO_CREATE		= "CREATE TABLE " + DB_USER_INFO_TABLE 
			+ " ("+KEY_ID + " INTEGER PRIMARY KEY," + USER_NICK + " TEXT,"+ USER_NAME + " TEXT," + USER_SEX + " TEXT,"
			+ USER_PHONE_NUM + " TEXT)";
	*/


	//创建记录信息表
	private static final String DB_RECORD_INFO_CREATE = "CREATE TABLE " + DB_RECORD_TABLE
			+ " (" + TIME_RECORD + " TEXT," + SCORE_RECORD + " TEXT," + FACE_NAME + " TEXT,"
			+ FACE_ID + " TEXT," + FACE_DETECT + " TEXT," + REMARKS2 + " TEXT," + OTHERSNAME + " TEXT)";

	
		/*
	//创建刷脸类型表
	private static final String	DB_CHECK_TYPE_CREATE		= "CREATE TABLE " + DB_CHECK_TYPE_TABLE 
			+ " ("+KEY_ID + " INTEGER PRIMARY KEY," + WIFI_TYPE + " TEXT,"+ GPS_TYPE + " TEXT," + CELL_TYPE + " TEXT,"
			+ SVR_TYPE + " TEXT)";
			*/

	// 执行open（）打开数据库时，保存返回的数据库对象

	private SQLiteDatabase mSQLiteDatabase = null;

	// 由SQLiteOpenHelper继承过来
	private DatabaseHelper mDatabaseHelper = null;


	private static class DatabaseHelper extends SQLiteOpenHelper {
		/* 构造函数-创建一个数据库 */
		DatabaseHelper(Context context) {

			//当调用getWritableDatabase() 
			//或 getReadableDatabase()方法时
			//则创建一个数据库
			super(context, DB_NAME, null, DB_VERSION);


		}

		/* 创建一个表 */
		@Override

		public void onCreate(SQLiteDatabase db) {

			// 数据库没有表时创建一个
			//db.execSQL(DB_USER_INFO_CREATE);
			//创建记录表
			db.execSQL(DB_RECORD_INFO_CREATE);
			//创建类型表
			//db.execSQL(DB_CHECK_TYPE_CREATE);
		}

		/* 升级数据库 */
		@Override

		public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
			if (newVersion == 2) {
				//db.execSQL(DB_CHECK_TYPE_CREATE);
			}
			//db.execSQL("DROP TABLE IF EXISTS "+DB_USER_INFO_TABLE);
			db.execSQL("DROP TABLE IF EXISTS " + DB_RECORD_TABLE);
			Log.e("onUpgrade", "DROP TABLE IF EXISTS ...");
			onCreate(db);
		}
	}

	/* 构造函数-取得Context */
	public MyDataBaseAdapter(Context context) {

		mContext = context;
	}


	// 打开数据库，返回数据库对象

	public void open() throws SQLException {

		mDatabaseHelper = new DatabaseHelper(mContext);
		mSQLiteDatabase = mDatabaseHelper.getWritableDatabase();
	}


	// 关闭数据库

	public void close() {
		mDatabaseHelper.close();
	}


	/*

	//插入一条用户信息数据
	public long insertUserInfoData(String userNick, String userName, String userSex, String userphomeNum)
	{
		ContentValues initialValues = new ContentValues();
		initialValues.put(KEY_ID, 0);
		initialValues.put(USER_NICK, userNick);
		initialValues.put(USER_NAME, userName);
		initialValues.put(USER_SEX, userSex);
		initialValues.put(USER_PHONE_NUM, userphomeNum);

		return mSQLiteDatabase.insert(DB_USER_INFO_TABLE, KEY_ID, initialValues);
	}
	//查询用户信息
	public Cursor fetchUserInfoData() throws SQLException
	{

		Cursor mCursor =

		mSQLiteDatabase.query(true, DB_USER_INFO_TABLE,
				new String[] { KEY_ID, USER_NICK, USER_NAME, USER_SEX, USER_PHONE_NUM },
				KEY_ID + "=" + 0,
				null, null, null, null, null);

		if (mCursor != null)
		{
			mCursor.moveToFirst();
		}
		return mCursor;

	}
	//更新用户信息
	public boolean updateUserInfoData(String userNick, String userName, String userSex, String userphomeNum)
	{
		ContentValues args = new ContentValues();
		args.put(USER_NICK, userNick);
		args.put(USER_NAME, userName);
		args.put(USER_SEX, userSex);
		args.put(USER_PHONE_NUM, userphomeNum);

		return mSQLiteDatabase.update(DB_USER_INFO_TABLE, args, KEY_ID + "=" + 0, null) > 0;
	}

	//插入一条类型信息数据
	public long insertCheckTypeInfoData()
	{
		ContentValues initialValues = new ContentValues();
		initialValues.put(KEY_ID, 0);
		initialValues.put(WIFI_TYPE, "0");
		initialValues.put(GPS_TYPE, "0");
		initialValues.put(CELL_TYPE, "0");
		initialValues.put(SVR_TYPE, "0");

		return mSQLiteDatabase.insert(DB_CHECK_TYPE_TABLE, KEY_ID, initialValues);
	}
	
	//获取类型信息
	public Cursor fetchCheckTypeInfoData() throws SQLException
	{

		Cursor mCursor =

		mSQLiteDatabase.query(true, DB_CHECK_TYPE_TABLE,
				new String[] { KEY_ID, WIFI_TYPE, GPS_TYPE, CELL_TYPE, SVR_TYPE },
				KEY_ID + "=" + 0,
				null, null, null, null, null);

		if (mCursor != null)
		{
			mCursor.moveToFirst();
		}
		return mCursor;

	}
	
	//更类型信息户信息
	public boolean updateCheckTypeInfoData(String wifiType, String gpsType, String cellTyep, String svrType)
	{
		ContentValues args = new ContentValues();
		args.put(WIFI_TYPE, wifiType);//暂时只更新wifi
		//args.put(GPS_TYPE, "0");
		//args.put(CELL_TYPE, "0");
		//args.put(SVR_TYPE, "0");

		return mSQLiteDatabase.update(DB_CHECK_TYPE_TABLE, args, KEY_ID + "=" + 0, null) > 0;
	}
	
	*/
	//FACE_DETECT + "TEXT," + REMARKS + "TEXT," + OTHERS + "TEXT)";
	//插入一条记录信息数据
	public long insertRecordInfoData(String time, String score, String name, String faceid, String faceDetect

			, String remarks, String others) {

		ContentValues initialValues = new ContentValues();
		initialValues.put(TIME_RECORD, time);
		initialValues.put(SCORE_RECORD, score);
		initialValues.put(FACE_NAME, name);
		initialValues.put(FACE_ID, faceid);
		initialValues.put(FACE_DETECT, faceDetect);
		initialValues.put(REMARKS2, remarks);
		initialValues.put(OTHERSNAME, others);
		MyApp.log("insertRecordInfoData");


		return mSQLiteDatabase.insert(DB_RECORD_TABLE, null, initialValues);
	}

	//删除所有记录信息数据
	public void clearRecordInfoData() {
		mSQLiteDatabase.execSQL("delete from " + DB_RECORD_TABLE);

	}

	//查询记录信息
	public Cursor fetchRecordInfoData() throws SQLException {

		Cursor mCursor =

				mSQLiteDatabase.query(true, DB_RECORD_TABLE,
						//new String[] { TIME_RECORD, SCORE_RECORD, FACE_NAME, FACE_ID },
						new String[]{TIME_RECORD, SCORE_RECORD, FACE_NAME, FACE_ID, FACE_DETECT, REMARKS2, OTHERSNAME},
						null,
						null, null, null, null, null);

		if (mCursor != null) {

			mCursor.moveToFirst();
		}
		return mCursor;

	}

	//获取类型信息
	public Cursor fetchCheckRecordInfoData(String datetime) throws SQLException {

		Cursor mCursor =

				mSQLiteDatabase.query(true, DB_RECORD_TABLE,
						new String[]{TIME_RECORD, SCORE_RECORD, FACE_NAME, FACE_ID, FACE_DETECT, REMARKS2, OTHERSNAME},
						TIME_RECORD + "=" + datetime,
						null, null, null, null, null);

		if (mCursor != null) {

			mCursor.moveToFirst();
		}
		return mCursor;

	}


	//删除一条数据 
	public boolean deleteRecordInfoData(String datetime) {
		return mSQLiteDatabase.delete(DB_RECORD_TABLE, TIME_RECORD + "=" + datetime, null) > 0;
	}

	public void deleteTable(String tablename) {
		mSQLiteDatabase.execSQL("DROP TABLE " + tablename);
	}
	/*
	public void createUserinfoTable() {
		mSQLiteDatabase.execSQL(DB_USER_INFO_CREATE);
	}
	*/
	
	
/*
	//删除一条数据 
	public boolean deleteData(long rowId)
	{
		return mSQLiteDatabase.delete(DB_TABLE, KEY_ID + "=" + rowId, null) > 0;
	}

	通过Cursor查询所有数据 
	public Cursor fetchAllData()
	{
		return mSQLiteDatabase.query(DB_TABLE, new String[] { KEY_ID, KEY_NUM, KEY_DATA }, null, null, null, null, null);
	}

	查询指定数据 
	public Cursor fetchData(long rowId) throws SQLException
	{

		Cursor mCursor =

		mSQLiteDatabase.query(true, DB_TABLE, new String[] { KEY_ID, KEY_NUM, KEY_DATA }, KEY_ID + "=" + rowId, null, null, null, null, null);

		if (mCursor != null)
		{
			mCursor.moveToFirst();
		}
		return mCursor;

	}

	更新一条数据 
	public boolean updateData(long rowId, int num, String data)
	{
		ContentValues args = new ContentValues();
		args.put(KEY_NUM, num);
		args.put(KEY_DATA, data);

		return mSQLiteDatabase.update(DB_TABLE, args, KEY_ID + "=" + rowId, null) > 0;
	}
	*/

}
