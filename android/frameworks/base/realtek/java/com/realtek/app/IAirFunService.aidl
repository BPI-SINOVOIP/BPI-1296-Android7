package com.realtek.app;

interface IAirFunService {
	String openAndDownloadImage(String url);
	String[] extractMetaData(String url);
	int getDownloadStatus();
}