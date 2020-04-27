package com.android.launcher2;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Build;
import android.view.View;
import android.util.Log;
import com.realtek.addon.helper.DebugHelper;

/**
 * Class added for realtek launcher.<br>
 * A view created in DragLayer to display focus frame.
 * 
 * @author bruce_huang
 *
 */
public class FocusView extends View {
    
    private static final int INNER_PADDING=2;
	
	public Launcher mLauncher;
	public DragLayer mDragLayer;
	//public Workspace mWorkspace;
	public View mBaseView;
	public Bitmap mBitmap;
	public Paint p;
	
	public int x;
	public int y;
	public int w;
	public int h;
	
	public FocusView(Launcher launcher, View baseView){
		super(launcher);
		
		mLauncher=launcher;
		mBaseView=baseView;
		
		mDragLayer=mLauncher.getDragLayer();
		//mWorkspace=mLauncher.getWorkspace();
		
		//int width=mDragLayer.getWidth();
		//int height=mDragLayer.getHeight();
		//DebugHelper.dump("FocusView, DragLayer width:"+width);
		//DebugHelper.dump("FocusView, DragLayer height:"+height);
		
		int[] basedLoc=new int[2];
		baseView.getLocationOnScreen(basedLoc);
		DebugHelper.dump("[FocusView] baseView location on Screen: x="+basedLoc[0]+" y="+basedLoc[1]);
		
		/*
		int[] basedLoc2=new int[2];
		baseView.getLocationInWindow(basedLoc2);
		DebugHelper.dump("[FocusView] baseView location on Window: x="+basedLoc2[0]+" y="+basedLoc2[1]);
		*/
		
		//int left=baseView.getLeft();
		//int top=baseView.getTop();		
		//DebugHelper.dump("[FocusView] baseView left="+left+" top="+top);
		
		int topOffset=getStatusBarHeightFromResource();
		
		// Calculate size and location of focus view
		x=basedLoc[0]-mLauncher.focusFrameThickness;
		y=basedLoc[1]-mLauncher.focusFrameThickness-topOffset;
		w=baseView.getWidth()+(mLauncher.focusFrameThickness*2);
		h=baseView.getHeight()+(mLauncher.focusFrameThickness*2);
		
		p=new Paint();
		p.setColor(Color.WHITE);
	}
	
	public int getStatusBarHeightFromResource() {
		  /*
	      int result = 0;
	      // only do this in kitkat build.
	      if(Build.VERSION.SDK_INT>=19) {
		      int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
		      if (resourceId > 0) {
		          result = getResources().getDimensionPixelSize(resourceId);
		      }
	      }
          */
          int statusBarHeight = getResources().getDimensionPixelSize(com.android.internal.R.dimen.status_bar_height);
          statusBarHeight = 0;
	      Log.d("LAUNCHER", "getStatusBarHeightFromResource statusBarHeight:"+statusBarHeight);
	      return statusBarHeight;
	}
	
	/*
	private int getStatusBarHeight(){
		DisplayMetrics dm = new DisplayMetrics();
		mLauncher.getWindowManager().getDefaultDisplay().getMetrics(dm);
		int topOffset=dm.heightPixels-mDragLayer.getMeasuredHeight();
		DebugHelper.dump("[FocusView] topOffset="+topOffset);
		return topOffset;
	}
	*/
	
	@Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        setMeasuredDimension(w, h);
    }
	
	@Override
    protected void onDraw(Canvas canvas) {
		canvas.drawARGB(255/5, 0xff, 0xff, 0xff);
		
		//p.setStrokeWidth(mLauncher.focusFrameThickness);
		canvas.drawRect(0, 0, w, mLauncher.focusFrameThickness+INNER_PADDING, p);
		canvas.drawRect(0, 0, mLauncher.focusFrameThickness+INNER_PADDING, h, p);
		canvas.drawRect(w-mLauncher.focusFrameThickness-INNER_PADDING, 0, w, h, p);
		canvas.drawRect(0, h-mLauncher.focusFrameThickness-INNER_PADDING, w, h, p);
		
		//canvas.drawRect(0, 0, w, h, p);
	}
	
	private void checkAndRemoveGarbageDragView(){
	    int dragLayerChildCount=mDragLayer.getChildCount();
	    for(int i=0;i<dragLayerChildCount;i++){
	        View v=mDragLayer.getChildAt(i);
	        if(v instanceof DragView){
	            mDragLayer.removeView(v);
	        }
	    }
	}

	public void show(){
		DebugHelper.dump("--------------> FocusView show...");
		checkAndRemoveGarbageDragView();

        if(mLauncher.mBootCompleted == false){
            Log.d("LAUNCHER","trying to show focus frame, but not boot completed, return");
            return;
        }

        Log.d("LAUNCHER" , "BOOTCOMPLETED draw focus frame");

		mDragLayer.addView(this);
		setTranslationX(x);
		setTranslationY(y);
	}
	
	public void remove(){
		mDragLayer.removeView(this);
	}
}
