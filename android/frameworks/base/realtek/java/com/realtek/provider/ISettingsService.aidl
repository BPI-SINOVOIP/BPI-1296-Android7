package com.realtek.provider;

interface ISettingsService {
	String getString(String table, String name);
	boolean putString(String table, String name, String value);
	int getInt(String table, String name);
	boolean putInt(String table, String name, int value);
	long getLong(String table, String name);
	boolean putLong(String table, String name, long value);
	float getFloat(String table, String name);
	boolean putFloat(String table, String name, float value);
	String getProperty(String prop, String def);
	boolean setProperty(String prop, String value);
    String wifiCmd(in List<String> cmd);
}
