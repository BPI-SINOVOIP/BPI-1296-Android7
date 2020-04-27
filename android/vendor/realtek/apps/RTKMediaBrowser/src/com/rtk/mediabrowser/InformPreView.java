package com.rtk.mediabrowser;

import android.os.Handler;
import android.os.Message;
import android.os.Bundle;
import com.realtek.Utils.InformPreViewMsg;
public class InformPreView
{
	private Handler OutSideHandler;
	public InformPreView(Handler handler)
	{
		OutSideHandler = handler;
	}
    public void MediaChange(String path, int BWType, int FileType, Bundle bundle)
	{
		Message msg = Message.obtain(OutSideHandler, InformPreViewMsg.MEDIA_PLAY, BWType, FileType, path);
		if(bundle!=null)
        	msg.setData(bundle);
		OutSideHandler.removeCallbacksAndMessages(null);
		OutSideHandler.sendMessageDelayed(msg,500);//Delay 1500ms to ask preview.
	}
    
	public void MediaChange(String path, int BWType, int FileType)
	{
		Message msg = Message.obtain(OutSideHandler, InformPreViewMsg.MEDIA_PLAY, BWType, FileType, path);
		OutSideHandler.removeCallbacksAndMessages(null);
		OutSideHandler.sendMessageDelayed(msg,500);//Delay 1500ms to ask preview.
	}
	public void StopPreView()
	{
		OutSideHandler.removeCallbacksAndMessages(null);
		OutSideHandler.sendEmptyMessage(InformPreViewMsg.PLAY_STOP);
	}
}
