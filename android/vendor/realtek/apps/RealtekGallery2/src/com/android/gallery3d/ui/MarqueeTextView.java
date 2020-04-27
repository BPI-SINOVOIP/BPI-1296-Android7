package com.android.gallery3d.ui;


import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.text.TextUtils.TruncateAt;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MarqueeTextView extends TextView {

	static final String LOG_TAG = "MarqueeTextView";
	boolean focused = false;
	int 	m_SelectedStyle=0;
	int 	m_UnselectedStyle=0;
	int 	m_SelectedStyleWithoutFocus=0;
	int 	m_UnselectedStyleWithoutFocus=0;
	//int 	m_FocusedFontSize=0;
	//int 	m_FocusedGravity=0;
	//float 	m_OrgTextSize = 0;
	//int 	m_OrgGravity = 0;
	Context m_Context;
	boolean m_UpdateViewOnFocusChange=false;
    int mFocusBgColor = 0x00000000;
    int mNormalBgColor = 0x00000000;
    View mParentListView = null;
	public MarqueeTextView(Context context) {
		super(context);
		m_Context = context;
            setMarqueeRepeatLimit(-1);
            setEllipsize(TruncateAt.MARQUEE);
	}

	public MarqueeTextView(Context context, AttributeSet attrs) {
		super(context, attrs);
		//Init(context, attrs);
		m_Context = context;
            setMarqueeRepeatLimit(-1);
            setEllipsize(TruncateAt.MARQUEE);
	}

	public MarqueeTextView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		//Init(context, attrs);
		m_Context = context;
            setMarqueeRepeatLimit(-1);
            setEllipsize(TruncateAt.MARQUEE);
	}
	/*
	private void Init(Context context, AttributeSet attrs){
		setFocusableInTouchMode(true);
		TypedArray a = context.obtainStyledAttributes(attrs,
		com.rtk.mediabrowser.R.styleable.MarqueeTextView);
		m_OrgTextSize = getTextSize();
		m_OrgGravity = getGravity();
		
		final int N = a.getIndexCount();
		for (int i = 0; i < N; ++i)
		{
			int attr = a.getIndex(i);
			switch (attr)
			{
			    case com.rtk.mediabrowser.R.styleable.MarqueeTextView_SelectedStyle:
			        m_SelectedStyle = a.getInt(attr, -1);
					Log.d(LOG_TAG,"Get SelectedStyle="+m_SelectedStyle);
			        break;
			    case com.rtk.mediabrowser.R.styleable.MarqueeTextView_UnselectedStyle:
			        m_UnselectedStyle = a.getInt(attr, -1);
					Log.d(LOG_TAG,"Get UnselectedStyle="+m_UnselectedStyle);
			        break;
			    case com.rtk.mediabrowser.R.styleable.MarqueeTextView_FocusedFontSize:
			        m_FocusedFontSize = a.getDimensionPixelSize(attr, m_FocusedFontSize);
					Log.d(LOG_TAG,"Get FocusedFontSize="+m_FocusedFontSize);
			        break;
			    case com.rtk.mediabrowser.R.styleable.MarqueeTextView_FocusedGravity:
			        m_FocusedGravity = a.getInt(attr,-1);
					Log.d(LOG_TAG,"Get FocusedGravity="+m_FocusedGravity);
			        break;
			    case com.rtk.mediabrowser.R.styleable.MarqueeTextView_UpdateViewOnFocusChange:
			        m_UpdateViewOnFocusChange = a.getBoolean(attr,false);
					Log.d(LOG_TAG,"Get UpdateViewOnFocusChange="+m_UpdateViewOnFocusChange);
			        break;
			}
		}
		a.recycle();		
	}
	*/
	/*
    @Override
    protected void onFocusChanged(boolean focused, int direction, Rect previouslyFocusedRect) {
		super.onFocusChanged(focused, direction, previouslyFocusedRect);
		Log.d(LOG_TAG,"onFocusChanged_0: "+focused);
		
	}
    @Override
    protected void onDraw(Canvas canvas) {
		Log.d(LOG_TAG,"onDraw isFocused:"+isFocused()+"isSelected:"+isSelected());
		super.onDraw(canvas);
	}
	*/
	public void setSelectedChangeStyle(int SelectedStyle, int UnselectedStyle){
		m_SelectedStyle = SelectedStyle;
		m_UnselectedStyle = UnselectedStyle;
		m_UpdateViewOnFocusChange = true;
	}

    public void setSelectedChangeStyle(int SelectedStyle, int UnselectedStyle, int bgColor){
		m_SelectedStyle = SelectedStyle;
		m_UnselectedStyle = UnselectedStyle;
		if(m_SelectedStyleWithoutFocus==0)
			m_SelectedStyleWithoutFocus = SelectedStyle;
		if(m_UnselectedStyleWithoutFocus==0)
			m_UnselectedStyleWithoutFocus = UnselectedStyle;
		m_UpdateViewOnFocusChange = true;
        mFocusBgColor = bgColor;
	}

    public void setSelectedChangeStyleWithoutFocus(int SelectedStyle, int UnselectedStyle, int bgColor){
		m_SelectedStyleWithoutFocus = SelectedStyle;
		m_UnselectedStyleWithoutFocus = UnselectedStyle;
		m_UpdateViewOnFocusChange = true;
        mNormalBgColor = bgColor;
	}

	public void unsetSelectedChangeStyle(){
		m_UpdateViewOnFocusChange = false;
	}

	@Override
    protected void drawableStateChanged() {
        boolean isFocused = isFocused();
		
        if(mParentListView != null && mParentListView instanceof android.widget.ListView && mParentListView.isFocused()) {
            isFocused = true;
        }
		
        View view = (View)getParent();
        while(view!=null && isFocused==false) {
            if(view instanceof android.widget.ListView) {
                if(view.isFocused()) {
                    isFocused = true;
                }
                break;
            }
            view = (View)view.getParent();
        }
//		Log.d(LOG_TAG,"drawableStateChanged listview isFocused:"+isFocused+", isSelected:"+isSelected());

		if (m_UpdateViewOnFocusChange)
		{
			if (isSelected())
			{
                    //setTextSize(m_FocusedFontSize);
                    //setGravity(m_FocusedGravity);
                    if(isFocused) {
                        setTextAppearance(m_Context, m_SelectedStyle);
                        super.setBackgroundColor(mFocusBgColor);
                    }
                    else {
                        setTextAppearance(m_Context, m_SelectedStyleWithoutFocus);
                        super.setBackgroundColor(mNormalBgColor);
                    }
			}
			else
			{
                    //setTextSize(m_OrgTextSize);
                    //setGravity(m_OrgGravity);
                    if(isFocused)
                        setTextAppearance(m_Context, m_UnselectedStyle);
                    else
                        setTextAppearance(m_Context, m_UnselectedStyleWithoutFocus);
                    super.setBackgroundColor(mNormalBgColor);
			}
		}
		super.drawableStateChanged();
	}
	@Override
	public void setTextAppearance(Context context, int resid) {
    	TypedArray appearance =
            context.obtainStyledAttributes(resid,
                                           com.android.internal.R.styleable.TextView);
		int gravity;
		//gravity = appearance.getInt(com.android.internal.R.styleable.TextView_gravity, -1);
		//Log.d(LOG_TAG,"SetGravity: "+gravity);
		//if (gravity !=-1)
		//{
		//	setGravity(gravity);
		//}
		appearance.recycle();
		super.setTextAppearance(context,resid);
	}
	public boolean setFocused(boolean focused) {
		return this.focused = focused;
	}

	public boolean isFocused() {
		return focused;
	}
	
	public void setParentListView(View v) {
		mParentListView = v;
	}

}
