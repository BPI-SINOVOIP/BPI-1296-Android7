package com.rtk.mediabrowser;

import android.app.Fragment;
import android.app.FragmentTransaction;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.Toast;
import com.realtek.DLNA_DMP_1p5.DLNABrowser;
import com.realtek.DLNA_DMP_1p5.UPNP_DMP_RES_TYPE;
import com.realtek.DataProvider.FileFilterType;
import com.realtek.Utils.BDParser;
import com.realtek.Utils.MbwType;

import static com.rtk.mediabrowser.VideoPreViewFragment.VIDEO_PREVIEW_ENABLE;

//import com.rtk.mediabrowser.PlayService_tsb.RepeatStatus;


public class PreViewActivity extends AbstractFBViewActivity implements
        VideoPreViewFragment.OnGetVideoInfoListener
{
	//private RelativeLayout mMainview=null;	
    private VideoPreViewFragment mVideoPreViewFragment=null;
    private AudioPreViewFragment mAudioPreViewFragment=null;
    private PhotoPreViewFragment mPhotoPreViewFragment=null;
    
    private static final boolean DEBUG = true;
    private static final String MY_TAG = "PreViewActivity";
    private boolean m_Pause = true;
    BroadcastReceiver mBroadcastReceiver = null;
    BackgroundTask mBackTask = null;

    private static void log(String s){
        if(DEBUG)
            Log.d(MY_TAG, s);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void onGetVideoInfo(int width, int height) {
        if(DEBUG) {
            String s = String.format("w:%d,h:%d", width, height);
            Log.d(MY_TAG, s);
        }
        if(mGenericContentViewFragment!=null){
            mGenericContentViewFragment.notifyVideoSize(width, height);
        }
    }

    public class PreViewHandler extends Handler {
        public static final int SELECT_CHANGE = 0;

        public void handleMessage(Message msg) {
            if (msg.what == SELECT_CHANGE) {
                if (m_Pause) return;
                if (mBrowserType == MbwType.TYPE_DEVICE_DMS) {
                    if(mBackTask!=null) {
                        mBackTask.cancel(true);
                        mBackTask = null;
                    }
                    mBackTask = new BackgroundTask();
                    mBackTask.execute(msg.arg1);
                }else {
                    setPreView(msg.arg1);
                }
            }
        }
    }

    private PreViewHandler m_PreViewHandler =null;
 	public void Child_Init()// Init about View
	{	
		Log.d(MY_TAG,"Child_Init()");	
		m_NumOfColumns = 1;
		m_NumOfRows =9;
		m_ViewType = GenericContentViewFragment.LIST_VIEW;
	    //You should set init variable which will be use after setContentView() such like ViewType, numofcolumn, because setContentView will instance the element of layout file(Fragment).	
        setContentView(R.layout.pre_view);
    	mVideoPreViewFragment =(VideoPreViewFragment)(getFragmentManager().findFragmentById(R.id.VideoPreView_fragment));	
    	mPhotoPreViewFragment =(PhotoPreViewFragment)(getFragmentManager().findFragmentById(R.id.PhotoPreView_fragment));	
    	mAudioPreViewFragment =(AudioPreViewFragment)(getFragmentManager().findFragmentById(R.id.AudioPreView_fragment));
	}
    @Override 
    public void onStop()
    {
        if (m_PreViewHandler !=null)
        {
            m_PreViewHandler.removeCallbacksAndMessages(null);
            m_PreViewHandler = null;
        }
        super.onStop();
    }
    @Override
    public void onResume()
    {
       m_Pause = false;//It should be set before call super. 
       super.onResume();
    } 
    @Override
    public void onPause()
    {
        m_Pause = true;//It should be set before call super.
        super.onPause();
        Log.d(MY_TAG,"onPause");
        if (m_PreViewHandler !=null)
        {
            m_PreViewHandler.removeCallbacksAndMessages(null);
        }
    }
    public void Child_onItemSelected(int position)
    {
        stopPreView();
        if (m_Pause == false)
        {
            if (m_PreViewHandler == null)
            {
                m_PreViewHandler = new PreViewHandler();
            }
	        Message msg = Message.obtain(m_PreViewHandler, PreViewHandler.SELECT_CHANGE, position, 0, null);
            m_PreViewHandler.removeCallbacksAndMessages(null);
            m_PreViewHandler.sendMessageDelayed(msg,1000);
        }
    }
	public boolean Child_OptionHandle()
	{
		return false;//not taken, use default insterad. 
	}
    public int Child_ViewType()
    {
        return VIEW_TYPE_PREVIEW;
    }
    private void stopPreView()
    {
		mVideoPreViewFragment.m_MyInformPreView.StopPreView();
		mAudioPreViewFragment.m_MyInformPreView.StopPreView();
		mPhotoPreViewFragment.m_MyInformPreView.StopPreView();
    }

    class BackgroundTask extends AsyncTask<Integer, Void, DataProviderStruct> {
        ProgressDialog progDailog;
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
//            progDlg = new ProgressDialog(PreViewActivity.this);
//            progDlg.setIndeterminate(false);
//            progDlg.setProgressStyle(ProgressDialog.STYLE_SPINNER);
//            progDlg.setCancelable(false);
//            progDlg.show();
        }

        @Override
        protected DataProviderStruct doInBackground(Integer... ints) {
            int position = ints[0];
            DataProviderStruct struct = new DataProviderStruct();
            struct.position = position;
            struct.GetSize = mDataProvider.GetSize();
            int FileType = FileFilterType.DEVICE_FILE_NONE;
            if (struct.position>=0 && struct.position <struct.GetSize)
            {
                struct.GetFileTypeAt = mDataProvider.GetFileTypeAt(position);
                FileType = struct.GetFileTypeAt;
            }

            if (FileType == FileFilterType.DEVICE_FILE_VIDEO) {
                if (VIDEO_PREVIEW_ENABLE) {
                    struct.GetSizeAt = mDataProvider.GetSizeAt(position);
                    if(isCancelled() || struct.GetSizeAt<0) {
                        struct.isFailed = true;
                        return struct;
                    }
                    struct.GetDataAt = mDataProvider.GetDataAt(position);
                }
            } else if (FileType == FileFilterType.DEVICE_FILE_AUDIO) {
                struct.GetDataAt = mDataProvider.GetDataAt(position);
                if(isCancelled() || struct.GetDataAt==null || struct.GetDataAt.length()==0) {
                    struct.isFailed = true;
                    return struct;
                }
            } else if (FileType == FileFilterType.DEVICE_FILE_PHOTO) {
                struct.GetSizeAt = mDataProvider.GetSizeAt(position);
                if(isCancelled() || struct.GetSizeAt<0) {
                    struct.isFailed = true;
                    return struct;
                }
                if (mBrowserType == MbwType.TYPE_DEVICE_DMS) {
                    struct.GetMediaInfoDate = mDataProvider.GetMediaInfo(position, UPNP_DMP_RES_TYPE.UPNP_DMP_RES_DATE);
                    struct.GetMediaInfoResolution = mDataProvider.GetMediaInfo(position, UPNP_DMP_RES_TYPE.UPNP_DMP_RES_RESOLUTION);
                }
                struct.GetDataAt = mDataProvider.GetDataAt(position);
            } else if (FileType == FileFilterType.DEVICE_FILE_DEVICE) {
                struct.GetTitleAt = mDataProvider.GetTitleAt(position);
            } else if (FileType == FileFilterType.DEVICE_FILE_DLNA_DEVICE) {
                struct.GetTitleAt = mDataProvider.GetTitleAt(position);
            } else if (FileType == FileFilterType.DEVICE_FILE_SMB_DEVICE) {
                struct.GetTitleAt = mDataProvider.GetTitleAt(position);
            } else if (FileType == FileFilterType.DEVICE_FILE_PLAYLIST) {
                struct.GetTitleAt = mDataProvider.GetTitleAt(position);
            }

            return struct;
        }

        @Override
        protected void onPostExecute(DataProviderStruct struct) {
            super.onPostExecute(struct);
//            if(progDlg!=null) {
//                progDlg.dismiss();
//                progDlg = null;
//            }
            if (m_Pause) return;
            if(struct.isFailed) {
//                Toast.makeText(PreViewActivity.this, R.string.no_file, Toast.LENGTH_SHORT).show();
                return;
            }
            int FileType = struct.GetFileTypeAt;
            Log.d(MY_TAG,"setPreView position:"+struct.position+" FileType:"+FileType);
            boolean FragmentChange = false;
            String BDDVDPath = null;

            //Adjust A/V/P pre view fragment
            //getFragmentManager().enableDebugLogging(true);
            FragmentTransaction ft = getFragmentManager().beginTransaction();
            ft.setCustomAnimations(android.R.animator.fade_in,android.R.animator.fade_out);
            if(FileType == FileFilterType.DEVICE_FILE_VIDEO)
            {
                if(VIDEO_PREVIEW_ENABLE)
                    FragmentChange |= ShowFGM(ft, mVideoPreViewFragment);
                else
                    FragmentChange |= HideFGM(ft, mVideoPreViewFragment);

                FragmentChange |= HideFGM(ft, mAudioPreViewFragment);
                mAudioPreViewFragment.m_MyInformPreView.StopPreView();

                FragmentChange |= HideFGM(ft, mPhotoPreViewFragment);
                mPhotoPreViewFragment.m_MyInformPreView.StopPreView();
            }
            else if(FileType == FileFilterType.DEVICE_FILE_AUDIO)
            {
                FragmentChange |= ShowFGM(ft, mAudioPreViewFragment);

                FragmentChange |= HideFGM(ft, mVideoPreViewFragment);
                mVideoPreViewFragment.m_MyInformPreView.StopPreView();

                FragmentChange |= HideFGM(ft, mPhotoPreViewFragment);
                mPhotoPreViewFragment.m_MyInformPreView.StopPreView();
            }
            else if(FileType == FileFilterType.DEVICE_FILE_PHOTO||FileType == FileFilterType.DEVICE_FILE_DEVICE||FileType == FileFilterType.DEVICE_FILE_DLNA_DEVICE||FileType == FileFilterType.DEVICE_FILE_SMB_DEVICE||FileType == FileFilterType.DEVICE_FILE_PLAYLIST)
            {
                FragmentChange |= ShowFGM(ft, mPhotoPreViewFragment);
                FragmentChange |= HideFGM(ft, mVideoPreViewFragment);
                mVideoPreViewFragment.m_MyInformPreView.StopPreView();

                FragmentChange |= HideFGM(ft, mAudioPreViewFragment);
                mAudioPreViewFragment.m_MyInformPreView.StopPreView();
            }
            else if (FileType == FileFilterType.DEVICE_FILE_DIR)
            {
                if (VIDEO_PREVIEW_ENABLE && ((BDDVDPath = IsBDDVD(struct.position))!=null)) {
                    FragmentChange |= ShowFGM(ft, mVideoPreViewFragment);
                } else {
                    FragmentChange |= HideFGM(ft, mVideoPreViewFragment);
                    mVideoPreViewFragment.m_MyInformPreView.StopPreView();
                }

                FragmentChange |= HideFGM(ft, mAudioPreViewFragment);
                mAudioPreViewFragment.m_MyInformPreView.StopPreView();

                FragmentChange |= HideFGM(ft, mPhotoPreViewFragment);
                mPhotoPreViewFragment.m_MyInformPreView.StopPreView();
            }
            else
            {
                FragmentChange |= HideFGM(ft, mVideoPreViewFragment);
                mVideoPreViewFragment.m_MyInformPreView.StopPreView();

                FragmentChange |= HideFGM(ft, mAudioPreViewFragment);
                mAudioPreViewFragment.m_MyInformPreView.StopPreView();

                FragmentChange |= HideFGM(ft, mPhotoPreViewFragment);
                mPhotoPreViewFragment.m_MyInformPreView.StopPreView();
            }
            if (FragmentChange)
            {
                Log.d(MY_TAG,"FragmentTransaction commit.");
                ft.commitAllowingStateLoss();
                getFragmentManager().executePendingTransactions();
            }
            if(FileType == FileFilterType.DEVICE_FILE_VIDEO)
            {
                if (VIDEO_PREVIEW_ENABLE) {
                    Bundle bundle = null;
                    long size =struct.GetSizeAt;
                    if (size >0) {
                        if(bundle==null) bundle = new Bundle();
                        String SSize = getSizeString(size);
                        log("file size="+SSize);
                        bundle.putString("Size", SSize);
                    }
                    mVideoPreViewFragment.m_MyInformPreView.MediaChange(struct.GetDataAt,mBrowserType,FileType, bundle);
                }
            }
            else if(FileType == FileFilterType.DEVICE_FILE_AUDIO)
            {
                Bundle bundle = null;
                mAudioPreViewFragment.m_MyInformPreView.MediaChange(struct.GetDataAt,mBrowserType,FileType, bundle);
            }
            else if(FileType == FileFilterType.DEVICE_FILE_PHOTO)
            {
                Bundle bundle = null;
                long size =struct.GetSizeAt;
                if(size>0){
                    if(bundle==null) bundle = new Bundle();
                    String SSize = getSizeString(size);
                    log("file size=" + SSize);
                    bundle.putString("Size", SSize);
                }
                if (mBrowserType==MbwType.TYPE_DEVICE_DMS)
                {
                    if(bundle==null) bundle = new Bundle();
                    String Date = struct.GetMediaInfoDate;
                    String Resolution = struct.GetMediaInfoResolution;
                    bundle.putString("Date", Date!= null?Date:getString(R.string.mb_unknown));
                    bundle.putString("Resolution", Resolution != null ? Resolution : getString(R.string.mb_unknown));
                }
                mPhotoPreViewFragment.m_MyInformPreView.MediaChange(struct.GetDataAt,mBrowserType,FileType,bundle);
            }
            else if(FileType == FileFilterType.DEVICE_FILE_DEVICE)
            {
                mPhotoPreViewFragment.m_MyInformPreView.MediaChange(struct.GetTitleAt,mBrowserType,FileType);
            }
            else if(FileType == FileFilterType.DEVICE_FILE_DLNA_DEVICE)
            {
                mPhotoPreViewFragment.m_MyInformPreView.MediaChange(struct.GetTitleAt,mBrowserType,FileType);
            }
            else if(FileType == FileFilterType.DEVICE_FILE_SMB_DEVICE)
            {
                mPhotoPreViewFragment.m_MyInformPreView.MediaChange(struct.GetTitleAt,mBrowserType,FileType);
            }
            else if(FileType == FileFilterType.DEVICE_FILE_PLAYLIST)
            {
                mPhotoPreViewFragment.m_MyInformPreView.MediaChange(struct.GetTitleAt,mBrowserType,FileType);
            }
            else if((FileType == FileFilterType.DEVICE_FILE_DIR)&&
                    (BDDVDPath != null))
            {
                mVideoPreViewFragment.m_MyInformPreView.MediaChange(BDDVDPath,mBrowserType,FileType);
            }

        }
    }
    
	private void setPreView(int position) {
		int FileType = FileFilterType.DEVICE_FILE_NONE;
		boolean FragmentChange = false;
        String BDDVDPath = null;
		if (position>=0 && position <mDataProvider.GetSize())
		{
			FileType = mDataProvider.GetFileTypeAt(position);
		}
		Log.d(MY_TAG,"setPreView position:"+position+" FileType:"+FileType);
		//Adjust A/V/P pre view fragment
        //getFragmentManager().enableDebugLogging(true);
        FragmentTransaction ft = getFragmentManager().beginTransaction();
		ft.setCustomAnimations(android.R.animator.fade_in,android.R.animator.fade_out);
	    if(FileType == FileFilterType.DEVICE_FILE_VIDEO)
		{
            if(VIDEO_PREVIEW_ENABLE)
                FragmentChange |= ShowFGM(ft, mVideoPreViewFragment);
            else
                FragmentChange |= HideFGM(ft, mVideoPreViewFragment);

		    FragmentChange |= HideFGM(ft, mAudioPreViewFragment);
			mAudioPreViewFragment.m_MyInformPreView.StopPreView();

		    FragmentChange |= HideFGM(ft, mPhotoPreViewFragment);
			mPhotoPreViewFragment.m_MyInformPreView.StopPreView();
		}
		else if(FileType == FileFilterType.DEVICE_FILE_AUDIO)
		{
			FragmentChange |= ShowFGM(ft, mAudioPreViewFragment);

		    FragmentChange |= HideFGM(ft, mVideoPreViewFragment);
			mVideoPreViewFragment.m_MyInformPreView.StopPreView();

		    FragmentChange |= HideFGM(ft, mPhotoPreViewFragment);
			mPhotoPreViewFragment.m_MyInformPreView.StopPreView();
		}
		else if(FileType == FileFilterType.DEVICE_FILE_PHOTO||FileType == FileFilterType.DEVICE_FILE_DEVICE||FileType == FileFilterType.DEVICE_FILE_DLNA_DEVICE||FileType == FileFilterType.DEVICE_FILE_SMB_DEVICE||FileType == FileFilterType.DEVICE_FILE_PLAYLIST)
		{
			FragmentChange |= ShowFGM(ft, mPhotoPreViewFragment);
		    FragmentChange |= HideFGM(ft, mVideoPreViewFragment);
			mVideoPreViewFragment.m_MyInformPreView.StopPreView();

		    FragmentChange |= HideFGM(ft, mAudioPreViewFragment);
			mAudioPreViewFragment.m_MyInformPreView.StopPreView();
		}
        else if (FileType == FileFilterType.DEVICE_FILE_DIR)
        {
            if (VIDEO_PREVIEW_ENABLE && ((BDDVDPath = IsBDDVD(position))!=null)) {
                FragmentChange |= ShowFGM(ft, mVideoPreViewFragment);
            } else {
                FragmentChange |= HideFGM(ft, mVideoPreViewFragment);
                mVideoPreViewFragment.m_MyInformPreView.StopPreView();
            }

		    FragmentChange |= HideFGM(ft, mAudioPreViewFragment);
			mAudioPreViewFragment.m_MyInformPreView.StopPreView();

		    FragmentChange |= HideFGM(ft, mPhotoPreViewFragment);
			mPhotoPreViewFragment.m_MyInformPreView.StopPreView();
        }
		else
		{
		    FragmentChange |= HideFGM(ft, mVideoPreViewFragment);
			mVideoPreViewFragment.m_MyInformPreView.StopPreView();

		    FragmentChange |= HideFGM(ft, mAudioPreViewFragment);
			mAudioPreViewFragment.m_MyInformPreView.StopPreView();

		    FragmentChange |= HideFGM(ft, mPhotoPreViewFragment);
			mPhotoPreViewFragment.m_MyInformPreView.StopPreView();
		}
		if (FragmentChange)
		{
			Log.d(MY_TAG,"FragmentTransaction commit.");
			ft.commitAllowingStateLoss();
            getFragmentManager().executePendingTransactions();
		}
	    if(FileType == FileFilterType.DEVICE_FILE_VIDEO)
		{
            Bundle bundle = null;
            long size =mDataProvider.GetSizeAt(position);
            if (size >0) {
                if(bundle==null) bundle = new Bundle();
                String SSize = getSizeString(size);
                log("file size="+SSize);
                bundle.putString("Size", SSize);
            }
            mVideoPreViewFragment.m_MyInformPreView.MediaChange(mDataProvider.GetDataAt(position),mBrowserType,FileType, bundle);
		}
		else if(FileType == FileFilterType.DEVICE_FILE_AUDIO)
		{
            Bundle bundle = null;
//            long size =mDataProvider.GetSizeAt(position);
//            if(size>0){
//                if(bundle==null) bundle = new Bundle();
//                String SSize = getSizeString(size);
//                log("file size="+SSize);
//                bundle.putString("Size", SSize);
//            }

            mAudioPreViewFragment.m_MyInformPreView.MediaChange(mDataProvider.GetDataAt(position),mBrowserType,FileType, bundle);
		}
		else if(FileType == FileFilterType.DEVICE_FILE_PHOTO)
		{
            Bundle bundle = null;
            long size =mDataProvider.GetSizeAt(position);
            if(size>0){
                if(bundle==null) bundle = new Bundle();
                String SSize = getSizeString(size);
                log("file size=" + SSize);
                bundle.putString("Size", SSize);
            }
            if (mBrowserType==MbwType.TYPE_DEVICE_DMS)
            {
                if(bundle==null) bundle = new Bundle();
                String Date = mDataProvider.GetMediaInfo(position,UPNP_DMP_RES_TYPE.UPNP_DMP_RES_DATE);
                String Resolution = mDataProvider.GetMediaInfo(position,UPNP_DMP_RES_TYPE.UPNP_DMP_RES_RESOLUTION);
                bundle.putString("Date", Date!= null?Date:getString(R.string.mb_unknown));
                bundle.putString("Resolution", Resolution != null ? Resolution : getString(R.string.mb_unknown));
            }
            mPhotoPreViewFragment.m_MyInformPreView.MediaChange(mDataProvider.GetDataAt(position),mBrowserType,FileType,bundle);
		}
		else if(FileType == FileFilterType.DEVICE_FILE_DEVICE)
		{
			mPhotoPreViewFragment.m_MyInformPreView.MediaChange(mDataProvider.GetTitleAt(position),mBrowserType,FileType);
		}
		else if(FileType == FileFilterType.DEVICE_FILE_DLNA_DEVICE)
		{
			mPhotoPreViewFragment.m_MyInformPreView.MediaChange(mDataProvider.GetTitleAt(position),mBrowserType,FileType);
		}
		else if(FileType == FileFilterType.DEVICE_FILE_SMB_DEVICE)
		{
			mPhotoPreViewFragment.m_MyInformPreView.MediaChange(mDataProvider.GetTitleAt(position),mBrowserType,FileType);
		}
 		else if(FileType == FileFilterType.DEVICE_FILE_PLAYLIST)
        {
            mPhotoPreViewFragment.m_MyInformPreView.MediaChange(mDataProvider.GetTitleAt(position),mBrowserType,FileType);
        }
        else if((FileType == FileFilterType.DEVICE_FILE_DIR)&&
                (BDDVDPath != null))
        {
			mVideoPreViewFragment.m_MyInformPreView.MediaChange(BDDVDPath,mBrowserType,FileType);
        }

	}

    private String getSizeString(long size) {
        String SSize;
        int unit = 0;
        while((size /1024)>=1 && unit<=3)
        {
            unit = unit +1;
            size = size /1024;
        }
        if (unit ==0)//Byte
            SSize = String.format("%d B", size);
        else if (unit ==1)//KB
            SSize = String.format("%d KB", size);
        else if (unit ==2)//MB
            SSize = String.format("%d MB", size);
        else//GB
            SSize = String.format("%d GB", size);
        return SSize;
    }

    private boolean ShowFGM(FragmentTransaction ft, Fragment Fgm)
	{
		Log.d(MY_TAG,"Try to ShowFGM: "+Fgm.toString());
		if (Fgm.isHidden())
		{
			ft.show(Fgm);
			Log.d(MY_TAG,"ShowFGB: "+Fgm.toString());
			return true;
		}
		return false;
	}
	private boolean HideFGM(FragmentTransaction ft, Fragment Fgm)
	{
		Log.d(MY_TAG,"Try to HideFGM: "+Fgm.toString());
		//Wen: Don't why isVisible() is not work in preview just start, so I change to use isHidden instead.
		if (!Fgm.isHidden())
		{
			ft.hide(Fgm);
			Log.d(MY_TAG,"HideFGM: "+Fgm.toString());
			return true;
		}
		return false;
	}

    private String IsBDDVD(int position)
    {
        String FilePath = null;
        if (mBrowserType != MbwType.TYPE_DEVICE_DMS)
        {
            String path= null;
            path = mDataProvider.GetDataAt(position);
            BDParser BD = new BDParser(path);
            if (BD.getType() != BDParser.NOT_BD_DVD)
            {
                FilePath = BD.getPlayEntry();
            }
        }
        return FilePath;
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        int keyCode = event.getKeyCode();
        int action = event.getAction();
        if(action == KeyEvent.ACTION_DOWN){
            switch(keyCode) {
                case KeyEvent.KEYCODE_MEDIA_NEXT:
                case KeyEvent.KEYCODE_MEDIA_PREVIOUS:
                    if(mVideoPreViewFragment!=null && mVideoPreViewFragment.isPlaying())
                        return true;
                    break;
            }
        }
        return super.dispatchKeyEvent(event);
    }
}
