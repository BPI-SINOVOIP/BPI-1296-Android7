package com.android.launcher2;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;

import com.realtek.launcher.R;

public class LauncherStorageWidgetHostView extends
        LauncherAppWidgetSelfUpdateHostView {
    
    private static final String TAG="StorageWidget";
    
    private ImageView mBackgroundImageView=null;
    private ImageView mAnimationImageView=null;
    
    private int mDrawableNoUsb=-1;
    private int mDrawableScanUsb=-1;
    private int mDrawableUsbCrash=-1;
    private int mDrawableUsb=-1;
    
    //private MediaScanStatusMonitor mMonitor;
    //private ArrayList<String> mExtStorageList=new ArrayList<String>();
    
    private int[] mAnimationDrawables;
    private int mAnimationDrawableIndex=0;
    
    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            doSelfUpdate();
        }
    };
    
    public LauncherStorageWidgetHostView(Context context, Launcher launcher) {
        super(context, launcher);
        inflateRootView(R.layout.wl_storage, "storage widget");
        
        // prepare drawable ID, make it readable
        
        mBackgroundImageView = (ImageView)mView.findViewById(R.id.background_image);
        mAnimationImageView = (ImageView)mView.findViewById(R.id.animation_image);
        
        mDrawableNoUsb=R.drawable.wi_storage_usb_no;
        mDrawableScanUsb=R.drawable.wi_storage_usb_scan;
        mDrawableUsb=R.drawable.wi_storage_usb;
        mDrawableUsbCrash=R.drawable.wi_storage_usb_crash;
        
        mAnimationDrawables = new int[8];
        mAnimationDrawables[0]=R.drawable.ic_launcher_usb_1;
        mAnimationDrawables[1]=R.drawable.ic_launcher_usb_2;
        mAnimationDrawables[2]=R.drawable.ic_launcher_usb_3;
        mAnimationDrawables[3]=R.drawable.ic_launcher_usb_4;
        mAnimationDrawables[4]=R.drawable.ic_launcher_usb_5;
        mAnimationDrawables[5]=R.drawable.ic_launcher_usb_6;
        mAnimationDrawables[6]=R.drawable.ic_launcher_usb_7;
        mAnimationDrawables[7]=R.drawable.ic_launcher_usb_8;
        
        // handle receiver
//        mMonitor = new MediaScanStatusMonitor();
//        IntentFilter filter = new IntentFilter();
//        filter.addAction(Intent.ACTION_MEDIA_SCANNER_STARTED);
//        filter.addAction(Intent.ACTION_MEDIA_SCANNER_FINISHED);
//        filter.addDataScheme("file");
//        context.registerReceiver(mMonitor, filter);
        
        doSelfUpdate();
    }
    
    private void registerUpdateWidgetMessage(long delay){
        Message message;
        String obj = "update widget";
        message = handler.obtainMessage(1,obj);
        if(delay>0)
            handler.sendMessageDelayed(message, delay);
        else
            handler.sendMessage(message);
    }
    
    private void doSelfUpdate(){
        //Log.d("Storage","\033[0;33;33m doSelfUpdate "+mLauncher.mMonitor+"\033[m");
        doUpdate();
        if(mLauncher.mMonitor != null && mLauncher.mMonitor.isScanning())
            registerUpdateWidgetMessage(500);
        else
            registerUpdateWidgetMessage(2000);
    }
    
    private void doUpdate(){
        //Log.i(TAG,"do update");
        // get storage status
        mLauncher.updateExtStorageList();
        
        int imageID=mDrawableNoUsb;
        
        if(mLauncher.mExtStorageList.size()>0){
            if(mLauncher.mMonitor != null && mLauncher.mMonitor.isScanning())
                imageID=mDrawableScanUsb;
            else
                imageID=mDrawableUsb;
        }else{
            imageID=mDrawableNoUsb;
        }
        
        mBackgroundImageView.setImageResource(imageID);
        
        // check and draw animation
        if(mLauncher.mExtStorageList.size()>0 && mLauncher.mMonitor != null && mLauncher.mMonitor.isScanning()){
            if(mAnimationImageView.getVisibility()==View.INVISIBLE){
                mAnimationImageView.setVisibility(View.VISIBLE);
                mAnimationDrawableIndex=0;
            }
            mAnimationImageView.setImageResource(mAnimationDrawables[mAnimationDrawableIndex]);
            mAnimationDrawableIndex++;
            mAnimationDrawableIndex = mAnimationDrawableIndex % mAnimationDrawables.length;
        }else{
            mAnimationImageView.setVisibility(View.INVISIBLE);
        }
    }
    
    class MediaScanStatusMonitor extends BroadcastReceiver{
        
        public int scanningCount=0;
        
        @Override
        public void onReceive(Context arg0, Intent arg1) {
            
            String action = arg1.getAction();
            Uri mData = arg1.getData();
            String uriPath = mData.getPath();
            
            Log.i(TAG,"onReceive:"+arg1.getAction()+" uriPath:"+uriPath);
            
            if(action.equals(Intent.ACTION_MEDIA_SCANNER_STARTED))
                scanningCount=scanningCount+1;
            
            if(action.equals(Intent.ACTION_MEDIA_SCANNER_FINISHED))
                scanningCount=scanningCount-1;
        }
        
        public boolean isScanning(){
            return scanningCount>0?true:false;
        }
    }
}
