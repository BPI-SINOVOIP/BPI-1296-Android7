package com.realtek.addon.database.helper;

import java.util.ArrayList;

import com.realtek.addon.helper.ConstantsDef;
import com.realtek.addon.helper.DebugHelper;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;

public class DBManager {
	
	public Context context;
	public DBOpenHelper helper;
	public SQLiteDatabase db;
	
	public DBManager(Context c){
		
		context=c;
		helper=new DBOpenHelper(context);
		db = helper.getWritableDatabase();
		
	}
	
	/**
	 * Check if given app is already in given category db list
	 * @param category
	 * @param name
	 * @return
	 */
	public boolean checkExistence(int category, String name){
		
		String sql= "select "+ConstantsDef.DB_KEY_APPPKGNAME+
				" from "+ConstantsDef.DB_TABLE_NAME+
				" where "+ConstantsDef.DB_KEY_CATEGORY+" ='"+category+"'"+
				" AND "+ConstantsDef.DB_KEY_APPPKGNAME+" ='"+name+"';";
		
		DebugHelper.dump("select:"+sql);
		Cursor cursor = db.rawQuery(sql, null);
		int count=cursor.getCount();
		DebugHelper.dump("check existence of cat:"+category+" , name="+name+" count:"+count);
		cursor.close();
		if(count>0)
			return true;
		else
			return false;
	}
	
	/**
	 * Insert a app into given category
	 * @param category :app category
	 * @param name :app package name
	 * @return true: insert success<p>false: entry already exist.
	 */
	public boolean insert(int category, String name){
		
		ContentValues cv = new ContentValues();
		cv.put(ConstantsDef.DB_KEY_CATEGORY, category);
		cv.put(ConstantsDef.DB_KEY_APPPKGNAME, name);
		if(!checkExistence(category, name)){
			long ret=db.insert(ConstantsDef.DB_TABLE_NAME, "", cv);
			DebugHelper.dump("[DBManager] insert rst="+ret);
			return true;
		}else{
			DebugHelper.dump("[DBManager] insert entry already exist!");
			return false;
		}
	}
	
	/**
	 * Delete entire category
	 * @param category :category is going to be deleted
	 */
	public void delete(int category){
		String sql="delete from "+ConstantsDef.DB_TABLE_NAME+" "+
				"where "+ConstantsDef.DB_KEY_CATEGORY+"='"+category+"';";
		DebugHelper.dump("delete:"+sql);
		db.execSQL(sql);
	}
	
	/**
	 * Delete given app in every category, usually called when an app is uninstalled
	 * @param name :app package name
	 */
	public void delete(String name){
		String sql="delete from "+ConstantsDef.DB_TABLE_NAME+" "+
				"where "+ConstantsDef.DB_KEY_APPPKGNAME+"='"+name+"';";
		DebugHelper.dump("delete:"+sql);
		db.execSQL(sql);
	}
	
	/**
	 * Delete given app under given category
	 * @param category
	 * @param name
	 */
	public void delete(int category, String name){
		String sql="delete from "+ConstantsDef.DB_TABLE_NAME+" "+
				"where "+ConstantsDef.DB_KEY_CATEGORY+"='"+category+"' "+
				"AND "+ConstantsDef.DB_KEY_APPPKGNAME+"='"+name+"';";
		
		DebugHelper.dump("delete:"+sql);
		db.execSQL(sql);
	}
	
	/**
	 * find app list under given category, and save to strArr, this API would not check existence of app stored in database
	 * @param category :app category
	 * @param strArr :ArrayList to save app list under given category
	 */
	public void select(int category, ArrayList<String> strArr){
		String sql= "select "+ConstantsDef.DB_KEY_APPPKGNAME+
				" from "+ConstantsDef.DB_TABLE_NAME+
				" where "+ConstantsDef.DB_KEY_CATEGORY+" ='"+category+"';";
		DebugHelper.dump("select:"+sql);
		Cursor cursor = db.rawQuery(sql, null);
		int count=cursor.getCount();
		cursor.moveToFirst();
		for(int i=0;i<count;i++){
			DebugHelper.dump("select "+cursor.getString(0));
			strArr.add(cursor.getString(0));
			cursor.moveToNext();
		}
		cursor.close();
	}
	
	/*
	public void addFavorite(AppInfo appInfo){
		String pkgName=appInfo.info.packageName;
		insert(ConstantsDef.FAV, pkgName);
	}
	
	public void addVideo(AppInfo appInfo){
		String pkgName=appInfo.info.packageName;
		insert(ConstantsDef.VIDEO, pkgName);
	}
	
	public void removeFavorite(AppInfo appInfo){
		
	}
	
	public void removeVideo(AppInfo appInfo){
		
	}
	*/
}
