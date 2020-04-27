package com.realtek.addon.helper;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;

import android.annotation.TargetApi;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.os.Environment;
import android.view.View;
import android.view.View.MeasureSpec;

public class BitmapHelper {
	
	public static Bitmap drawableToBitmap (Drawable drawable) {
	    if (drawable instanceof BitmapDrawable) {
	        return ((BitmapDrawable)drawable).getBitmap();
	    }
	    
	    if(drawable.getIntrinsicHeight()<0 || drawable.getIntrinsicWidth()<0)
	    	return null;
	    
	    Bitmap bitmap = Bitmap.createBitmap(drawable.getIntrinsicWidth(), drawable.getIntrinsicHeight(), Config.ARGB_8888);
	    Canvas canvas = new Canvas(bitmap); 
	    drawable.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
	    drawable.draw(canvas);

	    return bitmap;
	}
	
	public static Bitmap createBitmapFromView(View v) {
		
		int w=v.getLayoutParams().width;
		int h=v.getLayoutParams().height;
		
		DebugHelper.dump("View w="+w+" h="+h);
		
		Bitmap b = Bitmap.createBitmap(v.getLayoutParams().width,
				v.getLayoutParams().height, Bitmap.Config.ARGB_8888);
		Canvas c = new Canvas(b);
		v.measure(MeasureSpec.makeMeasureSpec(v.getLayoutParams().width,
				MeasureSpec.EXACTLY), MeasureSpec.makeMeasureSpec(
				v.getLayoutParams().height, MeasureSpec.EXACTLY));
		v.layout(0, 0, v.getMeasuredWidth(), v.getMeasuredHeight());
		v.draw(c);

		return b;
	}
	
	public static boolean writeBitmap2PngFile(String path, Bitmap mBitmap){
		
		if(mBitmap==null)
			return false;
		
		// auto append external storage path
		String extStoragePath=Environment.getExternalStorageDirectory().getAbsolutePath();
		
		File myDrawFile = new File(extStoragePath+"/"+path);
		DebugHelper.dump("Bitmap2File -> write Bitmap to file:"+myDrawFile.getAbsolutePath());
		
		try {
			BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(myDrawFile));
			mBitmap.compress(Bitmap.CompressFormat.PNG, 90, bos);
			bos.flush();
			bos.close();

		} catch (Exception e) {
			e.printStackTrace();
			DebugHelper.dump("Bitmap2File -> Save file error!");
			return false;
		}
		DebugHelper.dump("Bitmap2File -> Save file ok!");
		return true;
	}
	
	@TargetApi(19) 
	public static void setBitmapPremultiplied(Bitmap bitmap){
		if(Build.VERSION.SDK_INT>=19){
			bitmap.setPremultiplied(true);
		}
	}
	
	
	public static Bitmap applyAlpha(Bitmap bitmap, int alphaLvl){
		
		// range of alpha value returned from ColorPickerPreference is 0-255
		
		//float af=(float)alphaLvl/100f;
		//alphaLvl=(int)(af*255);
		
		// COMMENT: for backward compatibility
		//alphaLvl=0xff-alphaLvl;
		
		//DebugHelper.dump("applyAlpha alphaLvl="+alphaLvl);
		Bitmap mutableBitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true);
		// RTKCOMMENT: add for kitkat
		setBitmapPremultiplied(mutableBitmap);
		
		Canvas canvas = new Canvas(mutableBitmap);
		int colour = (alphaLvl & 0xFF) << 24;
		canvas.drawColor(colour, android.graphics.PorterDuff.Mode.DST_OUT);
		return mutableBitmap;
		
		/*
		Canvas canvas = new Canvas(pbitmap);
		Paint p=new Paint();
		p.setAlpha(alphaLvl);
		canvas.drawBitmap(pbitmap, 0, 0, p);
		*/
		/*
		Bitmap mutableBitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true);
		Canvas canvas = new Canvas(mutableBitmap);
		int colour = (alphaLvl & 0xFF) << 24;
		canvas.drawColor(colour, android.graphics.PorterDuff.Mode.DST_OUT);
		
		return mutableBitmap;
		*/
	}
}

