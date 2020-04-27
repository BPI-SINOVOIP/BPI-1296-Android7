package com.example.RTKMiracastSink;
 
import android.content.Context;
import android.util.AttributeSet;
import android.widget.VideoView;
 
public class CustomVideoView extends VideoView {
 
 public CustomVideoView(Context context) {
  super(context);
 }
 
 public CustomVideoView(Context context, AttributeSet attrs) {
  super(context, attrs);
 }
 
 public CustomVideoView(Context context, AttributeSet attrs, int defStyle) {
  super(context, attrs, defStyle);
 }
  
 @Override
 protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) 
 {
    int width = getDefaultSize(0, widthMeasureSpec);
    int height = getDefaultSize(0, heightMeasureSpec);
    setMeasuredDimension(width , height);
 }
}