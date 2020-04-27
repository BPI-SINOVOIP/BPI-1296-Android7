package com.realtek.addon.dialog;

public class RTKPopupWindowItemModel {
	
	public String label;
	public OPTION_TAG tag;
	public boolean bIsEnable;
	
	public RTKPopupWindowItemModel(String l, OPTION_TAG t, boolean bEn){
		label=l;
		tag=t;
		bIsEnable=bEn;
	}
}
