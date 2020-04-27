package com.rtk.mediabrowser;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Region;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.animation.LayoutAnimationController;
import android.view.ViewDebug;
import android.view.View;
import android.view.ViewGroup;
import android.widget.GridView;
import android.widget.ImageView;

import java.util.ArrayList;

public class RtkFileBrowserGridView extends GridView {
    private static final String LOG_TAG = "RtkFileBrowserGridView";
    public boolean isOnMeasure = false;
    public RtkFileBrowserGridView(Context context) {
        super(context);
    }

    public RtkFileBrowserGridView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public RtkFileBrowserGridView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }
	@Override
    public void adjustForBottomFadingEdge(View childInSelectedRow,
            int topSelectionPixel, int bottomSelectionPixel) {
        // Some of the newly selected item extends below the bottom of the
        // list
        if (childInSelectedRow.getBottom() > bottomSelectionPixel) {

            // Find space available above the selection into which we can
            // scroll upwards
            int spaceAbove = childInSelectedRow.getTop() - topSelectionPixel;

            // Find space required to bring the bottom of the selected item
            // fully into view
            int spaceBelow = childInSelectedRow.getBottom() - bottomSelectionPixel;
            //int offset = Math.min(spaceAbove, spaceBelow);
			int offset = Math.min(spaceAbove, childInSelectedRow.getHeight());
			Log.i(LOG_TAG,"adjustForBottomFadingEdge offset="+offset);	
 
            // Now offset the selected item to get it into view
            offsetChildrenTopAndBottom(-offset);
        }
    }
    @Override  
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {  
        isOnMeasure = true;  
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);  
    }  
  
    @Override  
    protected void onLayout(boolean changed, int l, int t, int r, int b) {  
        isOnMeasure = false;  
        super.onLayout(changed, l, t, r, b);  
    }
}
