package com.realtek.addon.database.helper;

import com.realtek.addon.helper.ConstantsDef;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class DBOpenHelper extends SQLiteOpenHelper {
	
	private static final int DATABASE_VERSION = 1;
	public DBOpenHelper(Context context) {
		super(context, ConstantsDef.DB_FILE_NAME, null, DATABASE_VERSION);
		
	}

	@Override
	public void onCreate(SQLiteDatabase database) {
		
//		database.execSQL(
//				"create table "+ConstantsDef.DB_TABLE_NAME+
//				"("+
//				//"ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"+
//				ConstantsDef.DB_KEY_APPPKGNAME+" VARCHAR PRIMARY KEY NOT NULL, "+	// we choice package name as database key
//				ConstantsDef.DB_KEY_CATEGORY+" INT"+
//				")");
		
		database.execSQL(
				"create table "+ConstantsDef.DB_TABLE_NAME+
				"("+
				"ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"+
				ConstantsDef.DB_KEY_CATEGORY+" INT,"+
				ConstantsDef.DB_KEY_APPPKGNAME+" VARCHAR"+
				")");
	}

	@Override
	public void onUpgrade(SQLiteDatabase arg0, int arg1, int arg2) {
	}
	
	@Override
	public void onOpen(SQLiteDatabase db) {
		super.onOpen(db);
	}
	
	@Override
	public synchronized void close() {
        super.close();
    }

}
