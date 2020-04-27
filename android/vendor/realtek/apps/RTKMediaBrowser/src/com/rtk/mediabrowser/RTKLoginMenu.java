package com.rtk.mediabrowser;

import android.widget.CheckBox;
import android.widget.PopupWindow;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.content.Context;
import android.widget.Button;
import android.widget.EditText;
import android.view.View.OnTouchListener;
import android.view.MotionEvent;
import android.graphics.drawable.BitmapDrawable;

public class RTKLoginMenu extends PopupWindow
{
	private static final String TAG = "RTKLoginMenu";
	private static final boolean DEBUG = false;
	private final CheckBox mCheckSaveLoginInfo;
	private View m_Popview = null;
	private EditText m_EditTextUser = null;
	private EditText m_EditTextPasswd = null;
	private int m_Width;
	private int m_Height;
	private int m_x;
	private int m_y;

	public RTKLoginMenu(int X, int Y, int Width, int Height, View ContentView)
	{
		super(ContentView, Width, Height);
		m_x = X;
		m_y = Y;
		m_Width = Width;
		m_Height = Height;
		m_Popview = ContentView;
		m_EditTextUser = (EditText)m_Popview.findViewById(R.id.LoginUserInput);
		m_EditTextPasswd= (EditText)m_Popview.findViewById(R.id.LoginPasswdInput);
		mCheckSaveLoginInfo = (CheckBox) m_Popview.findViewById(R.id.cbSaveLoginInfo);
		init();
	}
	private void init()
	{
		super.setAnimationStyle(R.style.PopupAnimation);
//		super.setBackgroundDrawable(new BitmapDrawable());
		super.setTouchInterceptor(new OnTouchListener(){
			public boolean onTouch(View view, MotionEvent event)
			{
				if (event.getAction() == MotionEvent.ACTION_OUTSIDE)
				{
					Hide();
					return true;
				}
				return false;
			}
		});
	}
	public void setClickListener(OnClickListener listener)
	{
		m_EditTextPasswd.setOnClickListener(listener);
	}
	public void Show()
	{
		log("Show login menu");
		super.setFocusable(true);
		super.setOutsideTouchable(true);
		super.showAtLocation(m_Popview, Gravity.LEFT| Gravity.TOP, m_x, m_y);
	}
	public void Hide()
	{
		log("Hide login menu");
		super.setFocusable(false);
		super.setOutsideTouchable(false);
		super.dismiss();
	}
	public View getContentView()
	{
		return m_Popview;
	}

	public void resetFields(){
		if(m_EditTextUser!=null)
			m_EditTextUser.setText("");
		if(m_EditTextPasswd!=null)
			m_EditTextPasswd.setText("");
		if(mCheckSaveLoginInfo!=null)
			mCheckSaveLoginInfo.setChecked(false);
	}

	public boolean getSaveLoginInfo(){
		boolean ret = false;
		if(mCheckSaveLoginInfo!=null)
			ret = mCheckSaveLoginInfo.isChecked();
		log("getSaveLoginInfo ret="+ret);
		return ret;
	}

	private void log(String s) {
		if(DEBUG)
			Log.d(TAG, s);
	}
}
