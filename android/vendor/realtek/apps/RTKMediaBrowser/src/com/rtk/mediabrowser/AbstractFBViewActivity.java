package com.rtk.mediabrowser;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.*;
import android.content.res.Configuration;
import android.content.res.XmlResourceParser;
import android.graphics.Point;
import android.graphics.drawable.AnimationDrawable;
import android.net.ConnectivityManager;
import android.net.EthernetManager;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.View;
import android.widget.*;
import android.widget.AdapterView.OnItemClickListener;
import com.realtek.DLNA_DMP_1p5.DLNABrowser;
import com.realtek.DataProvider.*;
import com.realtek.Utils.*;
import com.realtek.rtksmb.SmbBrowser;
import com.realtek.rtksmb.SmbUserMgr;
import com.rtk.mediabrowser.GenericContentViewFragment.UiListener;
import jcifs.smb.SmbAuthException;
import jcifs.smb.SmbException;

import java.io.*;
import java.util.ArrayList;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;
import android.app.AlertDialog;
import android.app.Instrumentation;

//import com.rtk.mediabrowser.PlayService_tsb.RepeatStatus;
import android.database.Cursor;
public abstract class AbstractFBViewActivity extends Activity implements UiListener
{
    private static final boolean DEBUG = true;
    public static final String MEDIA_BROWSER_USE_RT_MEDIA_PLAYER = "MEDIA_BROWSER_USE_RT_MEDIA_PLAYER";
    private static final String EXTRA_DVDPLAYER_EXTENSION = "MEDIA_BROWSER_EXTRA_DVDPLAYER_EXTENSION";
    private final ReentrantLock mLock = new ReentrantLock();
	private TextView info_window_left=null;
	private TextView info_window_right=null;

	private MimeTypes mMimeTypes=null;
	private XmlResourceParser mMimeTypeXml;

	private RelativeLayout mMainview=null;	
    public GenericContentViewFragment mGenericContentViewFragment=null;
    
    private int       mMediaType              = MbwType.TYPE_DEVICE_PHOTO;
    private boolean mDisableDLNA = false;
    private boolean mDisableSAMBA = false;
    //Top mediaInfo & icon
	private ImageView mTopMediaInfoImageView =null;
    private TextView  mTopMediaInfoTextView =null;

    //Top mediaPathInfo & icon
    private TextView  mTopMediaPathInfoTextView = null;
	private ImageView mTopMediaPathInfoImageView = null;
	private ImageView mTopMediaPathLine0ImageView = null;
	private ImageView mTopMediaPathLine1ImageView = null;
    private String mRootPath    = null;
    private Path_Info mPathInfo=null;
    
	public int m_NumOfColumns = 1;
	public int m_NumOfRows = 8;
	public int m_ViewType =-1;	
    private static final String TAG = "AbstractFBViewActivity";
    private final int m_DebugLevel = 2;//0~3: less msg ~ most msg
         
    public AbstractDataProvider mDataProvider=null;
    public int mBrowserType = 0;
    
    private int mPosition = 1;
    private int mState = 0x0;
    private static final int S_CREATE =0x1;
    private static final int S_RESTART = 0x2;
    private static final int S_START= 0x4;
    private static final int S_RESUME = 0x8;
    private static final int S_PAUSE = 0x16;
    private static final int S_STOP = 0x32;
	private RTKOptionMenu  mPop = null;
	private int mSelectedPosition = -1;
	private RTKLoginMenu mLogin = null;	
	private Boolean mIsLogined = false;
    private EditText m_LoginUserInput = null;
    private EditText m_LoginPasswdInput = null;
    private static final String PLAYLIST_FILE = "/tmp/RTMediaBrowserPlaylist.txt";
    private static final String PLAYLIST_FILE_TITLE = "/tmp/RTMediaBrowserPlaylist_title.txt";
    private DataBaseHelper mDB = null;
    
    private final Handler All_Dev_DataProvider_handler = new MyHandler();
	private static final int UPDATE_ALL_DEV_LIST =1000;
    private int mAllDevQueryAbort =0;
    private boolean mFirst_SMB_QUERY_ALL_DOMAIN_DONE = false;
    private boolean mNetworkConnected = true;
    private boolean mStopUpdateContentList = false;

    private static final int MOVIE_REQUEST_CODE = 0;
    //public static final int PHOTO_REQUEST_CODE = 1;
    //public static final int MUSIC_REQUEST_CODE = 2;
    // how about BD menu?

    private BackgroundTask mBackTask = null;
    
    private static final boolean SINGLE_MUSIC_PLAYBACK = false;
    private boolean mIsGetContentMode = false;
    private String mQueryString;
    public boolean mIsStartMusic = false;

    class MyHandler extends Handler{
        public void handleMessage(Message msg) {
            if (msg.what == AllDeviceDataProvider.SMB_QUERY_DOMAIN_DONE)
            {
                Log.d(TAG, "SMB_QUERY_DOMAIN_DONE GetSize(SMB) = " + mDataProvider.GetSize(FileFilterType.DEVICE_FILE_SMB_DEVICE));
                //mAllDevQueryAbort = mAllDevQueryAbort & (~AllDeviceDataProvider.SMB_QUERY_DOMAIN_ABORT);
                if (!mFirst_SMB_QUERY_ALL_DOMAIN_DONE)
                {
                    GotNewItem = GotNewItem|AllDeviceDataProvider.UPDATE_SMB;
                }
            }
            else if (msg.what == AllDeviceDataProvider.DMS_QUERY_DONE)
            {
                Log.d(TAG, "DMS_QUERY_DONE GetSize(DMS) = " + mDataProvider.GetSize(FileFilterType.DEVICE_FILE_DLNA_DEVICE));
                if ((mAllDevQueryAbort & AllDeviceDataProvider.DMS_QUERY_ABORT)==AllDeviceDataProvider.DMS_QUERY_ABORT)
                {
                    //As DLNA lib require, we have to renew DLNA lib when network disconnect-->connect.
                    DLNABrowser.DelInstance();
                    DLNABrowser.GetInstance();
                    mAllDevQueryAbort = mAllDevQueryAbort & (~AllDeviceDataProvider.DMS_QUERY_ABORT);
                }
                GotNewItem = GotNewItem|AllDeviceDataProvider.UPDATE_QUERY_DMS;
            }
            else if (msg.what == AllDeviceDataProvider.USB_QUERY_DONE)
            {
                Log.d(TAG, "USB_QUERY_DONE GetSize(USB) = " + mDataProvider.GetSize(FileFilterType.DEVICE_FILE_DEVICE));
                GotNewItem = GotNewItem|AllDeviceDataProvider.UPDATE_QUERY_USB;
            }
            else if (msg.what == AllDeviceDataProvider.SMB_QUERY_ALL_DOMAIN_DONE)
            {
                Log.d(TAG, "SMB_QUERY_ALL_DOMAIN_DONE GetSize(SMB) = " + mDataProvider.GetSize(FileFilterType.DEVICE_FILE_SMB_DEVICE));
                mFirst_SMB_QUERY_ALL_DOMAIN_DONE= true;
                //mAllDevQueryAbort = mAllDevQueryAbort & (~AllDeviceDataProvider.SMB_QUERY_DOMAIN_ABORT);
                GotNewItem = GotNewItem|AllDeviceDataProvider.UPDATE_QUERY_SMB;
            }
            else if (msg.what == UPDATE_ALL_DEV_LIST)
            {
                if (GotNewItem!=0)
                {
                    try{
                        if (mLock.tryLock(100,TimeUnit.MILLISECONDS))
                        {
                            if (mSelectedPosition>=0)
                            {
                                UpdateItemView();
                                Log.d(TAG, "[0]Try to check in next 5 sec.");
                                sendEmptyMessageDelayed(UPDATE_ALL_DEV_LIST,5000);
                            }
                            else//The Activity is just start, Just Update list, don't call UpdateItemView();
                            {
                                mDataProvider.UpdateData(GotNewItem);//Update new data to list, and ask query again.
                                GotNewItem =0;
                                Log.d(TAG, "GetSize(All) =" + mDataProvider.GetSize());
                                if (mDataProvider.GetSize()>0)
                                {
                                    if(mGenericContentViewFragment!=null)
                                    {
                                        mGenericContentViewFragment.RefreshAbstractView(0);
                                    }
                                    Log.d(TAG, "[1]Try to check in next 5 sec.");
                                    sendEmptyMessageDelayed(UPDATE_ALL_DEV_LIST,5000);
                                }
                                else
                                {
                                    Log.d(TAG, "[2]Try to check in next 2 sec.");
                                    sendEmptyMessageDelayed(UPDATE_ALL_DEV_LIST,2000);
                                }
                            }
                        }
                        else
                        {
                            Log.d(TAG, "Can not lock, try to update in next 1 sec.");
                            sendEmptyMessageDelayed(UPDATE_ALL_DEV_LIST,1000);//try to update in next 1 sec.
                        }
                    }
                    catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    finally{
                        if (mLock.isHeldByCurrentThread()){mLock.unlock();}
                    }
                }
                else
                {
                    if (mDataProvider.GetSize()==0)
                    {
                        Log.d(TAG, "[3]no update, try to check in next 0.1 sec.");
                        sendEmptyMessageDelayed(UPDATE_ALL_DEV_LIST,100);
                    }
                    else
                    {
                        Log.d(TAG, "[4]no update, try to check in next 5 sec.");
                        sendEmptyMessageDelayed(UPDATE_ALL_DEV_LIST,5000);
                    }
                }
                if (msg.arg1==1&& msg.obj!=null)
                {
                    if (mDataProvider.GetItemIndex((String)msg.obj)== -1)
                    {
                        //send msg in immediately
                        Log.d(TAG,"UPDATE_ALL_DEV_LIST[+] can not get:"+ msg.obj);
                        removeMessages(UPDATE_ALL_DEV_LIST);
                        sendMessageDelayed(Message.obtain(msg),100);
                    }
                    else
                    {
                        Log.d(TAG,"UPDATE_ALL_DEV_LIST[+] get:"+ msg.obj);
                    }
                }
                if (msg.arg1==-1&& msg.obj!=null)
                {
                    if (mDataProvider.GetItemIndex((String)msg.obj)>-1)
                    {
                        //send msg in immediately
                        Log.d(TAG,"UPDATE_ALL_DEV_LIST[-] can not get:"+ msg.obj);
                        removeMessages(UPDATE_ALL_DEV_LIST);
                        sendMessageDelayed(Message.obtain(msg),300);
                    }
                    else
                    {
                        Log.d(TAG,"UPDATE_ALL_DEV_LIST[-] get:"+ msg.obj);
                    }
                }

            }
            else if (msg.what == AllDeviceDataProvider.DMS_QUERY_ABORT)
            {
                Log.d(TAG,"DMS_QUERY_ABORT");
                //Delete and new DLNABrowser instance
                if ((mAllDevQueryAbort & AllDeviceDataProvider.DMS_QUERY_ABORT) ==0)
                {
                    DLNABrowser.DelInstance();
                    DLNABrowser.GetInstance();
                }
                GotNewItem = GotNewItem | AllDeviceDataProvider.UPDATE_QUERY_DMS;
                mAllDevQueryAbort = mAllDevQueryAbort|AllDeviceDataProvider.DMS_QUERY_ABORT;
            }
            else if (msg.what == AllDeviceDataProvider.SMB_QUERY_DOMAIN_ABORT)
            {
                Log.d(TAG,"SMB_QUERY_DOMAIN_ABORT");
                //Delete and new SmbBrowser instance
                //SmbBrowser.DelInstance();
                //SmbBrowser.GetInstance();
                GotNewItem = GotNewItem|AllDeviceDataProvider.UPDATE_QUERY_SMB;
                //mAllDevQueryAbort = mAllDevQueryAbort|AllDeviceDataProvider.SMB_QUERY_DOMAIN_ABORT;
            }
        }
    }

    public class DataProviderHandler extends Handler
    {
		public void handleMessage(Message msg) { 
		    if (msg.what == SmbBrowser.SmbMountOkay)
		    {   
				if(mGenericContentViewFragment!=null)
				{
					mGenericContentViewFragment.RefreshAbstractView(0);
    	            mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
				}
		        Log.d(TAG, "SmbMountOkay");
		    }
		    else if (msg.what == SmbBrowser.SmbMountFailed)
		    {
                //Have to show Login ask ID/PW 
		        Log.d(TAG, "SmbMountFailed");
                NewLoginMenu();//Show Login UI to ask ID/PW.
		    }
		    else if (msg.what == SmbBrowser.SmbUmountOkay)
		    {
		        Log.d(TAG, "SmbUmountOkay");
		    }
		    else if (msg.what == SmbBrowser.SmbUmountFailed)
		    {
		        Log.d(TAG, "SmbUmountFailed");
		    }
            else if (msg.what == SmbBrowser.SmbAuthFailed)
            {
                NewLoginMenu();//Show Login UI to ask ID/PW.
            }
            else if (msg.what == AbstractDataProvider.FILE_LIST_CREATE_DONE)
            {
                if(mStopUpdateContentList==false && mGenericContentViewFragment != null)
				{
                    //if (mDataProvider.GetSize()==0 || mBrowserType == MbwType.TYPE_DEVICE_DMS)//DMS dataprovider doesn't have double buffer, so GetSize() will just return current size of list.  
                    if (mDataProvider.GetSize()==0
                            || mBrowserType==MbwType.TYPE_DEVICE_SMB)
                    {
                        mDataProvider.UpdateData(0);//UpdateData() will ask Dataprovider update background list buffer to foreground. 
					    mGenericContentViewFragment.RefreshAbstractView(mPathInfo.getLastLevelFocus());
                    }
                    else
                    {
                        UpdateItemView();
                    }
				}
                if (mBrowserType == MbwType.TYPE_DEVICE_PHOTO ||
                    mBrowserType == MbwType.TYPE_DEVICE_VIDEO || 
                    mBrowserType == MbwType.TYPE_DEVICE_AUDIO ||
                    mBrowserType == MbwType.TYPE_DEVICE_QUERY)
                {
                    if (mDataProvider.GetSize()==0)
                    {
                        Toast.makeText(getApplicationContext(),getString(R.string.no_db_file), Toast.LENGTH_LONG).show();
                    } 
                }
		        Log.d(TAG, "Create file list DONE");
                if(mBrowserType==MbwType.TYPE_DEVICE_QUERY&& mDataProvider.GetSize()==1){
                    onItemClicked(0);
                }
            }
            else if (msg.what == AbstractDataProvider.FILE_LIST_CREATE_PORTION)
            {
               if(mStopUpdateContentList==false && mGenericContentViewFragment != null)
               {
                    if (mDataProvider.GetSize()==0)
                    {
                        mDataProvider.UpdateData(0);
					    mGenericContentViewFragment.RefreshAbstractView(mPathInfo.getLastLevelFocus()<msg.arg2?mPathInfo.getLastLevelFocus():0);
                    }
                    else
                    {
                        UpdateItemView();
                    }
               } 
		       Log.d(TAG, "Create file list PORTION ["+msg.arg1+"/"+msg.arg2+"]");
            } 
            else if (msg.what == AbstractDataProvider.FILE_LIST_CREATE_ABORT)
            {
		        log("Create file list ABORT");
                Object obj = msg.obj;
                if(obj instanceof SmbException){
                    SmbException e = (SmbException)obj;
                    if(e instanceof SmbAuthException){
                        log("smb login failed: "+e.getMessage());
//                        Toast.makeText(AbstractFBViewActivity.this, e.getMessage(), Toast.LENGTH_SHORT).show();
                        if (mDataProvider.GetSize()<0)//Login failed.
                        {
                            mDataProviderHandler.sendEmptyMessageDelayed(SmbBrowser.SmbAuthFailed,500);
                        }
                    } else {
                        log("smb exception");
                        Toast.makeText(AbstractFBViewActivity.this, e.getMessage(), Toast.LENGTH_SHORT).show();
                    }
                }

            }
		}
    }
	private DataProviderHandler mDataProviderHandler = null;
	private int GotNewItem = 0;
	
	//loading icon
    private Handler loading_icon_system_handle    = null;
	private AnimationDrawable ad = null;
	private ImageView topLoadingIcon =null;

    //Broadcast Receiver
    private Handler mInternal_Handler = null;
    private final int CHECK_CUR_PATH_IS_VALID = 1;
    private final int KEEP_CHECK_CUR_PATH_IS_VALID = 2;
    private IntentFilter mIntentFilter = null;
    private MyBroadcastReceiver mReceiver = null;

    class MyBroadcastReceiver extends BroadcastReceiver {
        private final Handler handler;
        public MyBroadcastReceiver(Handler handler){
            this.handler = handler;
        }
        public void onReceive(Context context, Intent intent) {
            if ((mState & (S_PAUSE | S_STOP)) == 0) {
                final String action = intent.getAction();
                final String Data = intent.getDataString();
                final String Type = intent.getType();
                final String DataScheme = intent.getScheme();
                if (m_DebugLevel >= 2) {
                    Log.d(TAG, "<BroadcastReceiver> action:" + action);
                    Log.d(TAG, "<BroadcastReceiver> Component:" + intent.getComponent());
                    Log.d(TAG, "<BroadcastReceiver> Categories:" + intent.getCategories());
                    Log.d(TAG, "<BroadcastReceiver> Data:" + Data);
                    Log.d(TAG, "<BroadcastReceiver> Type:" + Type);
                    Log.d(TAG, "<BroadcastReceiver> DataScheme:" + DataScheme);
                }
                if (action.equals("android.intent.action.MEDIA_REMOVED")
                        || action.equals("android.intent.action.MEDIA_BAD_REMOVAL")) {
                    //Delay 3 sec to check CurrentPathIsValid()
                    //mInternal_Handler.sendEmptyMessageDelayed(CHECK_CUR_PATH_IS_VALID, 3000);
                    //if Data equals file:///dev/null, then ignore
                    if (!"file:///dev/null".equals(Data)
                            && !CurrentPathIsValid(Data)) {
                        BackToMediaBrowserHome();
                    }
                } else if (action.equals(Intent.ACTION_MEDIA_UNMOUNTED) || action.equals(Intent.ACTION_MEDIA_BAD_REMOVAL)) {
                    if (mBrowserType == MbwType.TYPE_DEVICE_ALL) {
                        Message Newmsg = handler.obtainMessage(UPDATE_ALL_DEV_LIST, -1, 0, Data);
                        handler.sendMessage(Newmsg);
                    }
                } else if (action.equals(Intent.ACTION_MEDIA_MOUNTED)) {
                    if (mBrowserType == MbwType.TYPE_DEVICE_ALL) {
                        Message Newmsg = handler.obtainMessage(UPDATE_ALL_DEV_LIST, 1, 0, Data);
                        handler.sendMessage(Newmsg);
                    }
                    if ((mBrowserType == MbwType.TYPE_DB_PHOTO) || (mBrowserType == MbwType.TYPE_DB_VIDEO)) {
                        BackToMediaBrowserHome();
                    }
                } else if (action.equals(ConnectivityManager.CONNECTIVITY_ACTION) ||
                        action.equals(ConnectivityManager.INET_CONDITION_ACTION) ||
                        action.equals(EthernetManager.NETWORK_STATE_CHANGED_ACTION)) {
                    boolean noConnection = !hasNetwork();
                    if (!CurrentPathIsValid(null)) {
                        BackToMediaBrowserHome();
                    } else if (noConnection){
                        if (mBrowserType == MbwType.TYPE_DEVICE_DMS
                        || mBrowserType == MbwType.TYPE_DEVICE_SMB){
                            BackToMediaBrowserHome();
                        } else if(mBrowserType == MbwType.TYPE_DEVICE_ALL) {
                            if(mDataProvider!=null) {
                                if(mDataProvider instanceof AllDeviceDataProvider) {
                                    AllDeviceDataProvider allDeviceDataProvider = (AllDeviceDataProvider)mDataProvider;
                                    allDeviceDataProvider.clearNetworkItems();
                                    log("allDeviceDataProvider.clearNetworkItems()");
                                } else {
                                    log("mDataProvider isn't instanceof AllDeviceDataProvider");
                                }
                                if(mDataProvider.GetSize()>0) {
                                    if (mGenericContentViewFragment != null) {
                                        mGenericContentViewFragment.RefreshAbstractView(0);
                                    }
                                }
                            }
                        }
                    } else { //has network
                        if ((mBrowserType == MbwType.TYPE_DEVICE_DMS) && DLNABrowser.GetInstance().isRestarted() ) {
                            goBackToBrowseAllFiles();
                        }
                        All_Dev_DataProvider_handler.sendEmptyMessageDelayed(UPDATE_ALL_DEV_LIST,100);
                    }
                }
            } else {
                Log.v(TAG, "Got Broadcast, but do not thing because we are in State:" + mState);
            }
        }
    }

    private void goBackToBrowseAllFiles() {
        NewDB();
        String oldViewType = mDB.QueryDate(MbwType.TYPE_DEVICE_ALL, DataBaseHelper.VIEW_TYPE);
        ComponentName componetName = null;
        Intent intent= new Intent();
        Bundle bundle = new Bundle();
        if (oldViewType.equals(DataBaseHelper.PREVIEW)){
            componetName = new ComponentName("com.rtk.mediabrowser", "com.rtk.mediabrowser.PreViewActivity");
            bundle.putInt("CurPosition", 1);
        } else {
            componetName = new ComponentName("com.rtk.mediabrowser", "com.rtk.mediabrowser.ThumbnailViewActivity");
            bundle.putInt("CurPosition", 0);
        }
        bundle.putInt("browserType", MbwType.TYPE_DEVICE_ALL);
        bundle.putInt("MediaType", FileFilterType.DEVICE_FILE_DIR|FileFilterType.DEVICE_FILE_PHOTO|FileFilterType.DEVICE_FILE_VIDEO|FileFilterType.DEVICE_FILE_AUDIO|FileFilterType.DEVICE_FILE_PLAYLIST);
        bundle.putBoolean("DisableDLAN",mDisableDLNA);
        bundle.putBoolean("DisableSAMBA",mDisableSAMBA);
        intent.putExtras(bundle);
        intent.setComponent(componetName);
        startActivity(intent);
        overridePendingTransition(0,0);
    }

    private void debugIntent(Intent intent, String tag) {
        Log.v(tag, "action: " + intent.getAction());
        Log.v(tag, "component: " + intent.getComponent());
        Bundle extras = intent.getExtras();
        if (extras != null) {
            for (String key: extras.keySet()) {
                Log.v(tag, "key [" + key + "]: " +
                        extras.get(key));
            }
        }
        else {
            Log.v(tag, "no extras");
        }
    }

    private void NewDB()
    {
        if (mDB==null){ mDB = new DataBaseHelper(this);} 
    }	
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
       	Log.v(TAG,"onCreate");
        if (m_DebugLevel >=3)
        {
		    //DisplayMetric : A structure describing general information about a display, such as its size, density, and font scaling. 
            DisplayMetrics dm = new DisplayMetrics();
            getWindowManager().getDefaultDisplay().getMetrics(dm);
            Log.d(TAG,"System Width: "+dm.widthPixels+" Height: "+dm.heightPixels);
        }

        Intent intent= getIntent();
        if(intent==null) return;
        if(intent!=null && intent.getAction()!=null) {
            final String action = intent.getAction();
            if (Intent.ACTION_GET_CONTENT.equals(action)) {
                mIsGetContentMode = true;
            }else if(Intent.ACTION_SEARCH.equals(action)){
                log("get action search");
            }else if(Intent.ACTION_VIEW.equals(action)){
                log("get action view");
            }
        }
        mQueryString = intent.getStringExtra("queryString");
        mBrowserType = intent.getIntExtra("browserType", 0);
        mMediaType = intent.getIntExtra("MediaType", 0);
        mPosition = intent.getIntExtra("CurPosition",1);
        if (mBrowserType == MbwType.TYPE_DEVICE_ALL)
        {
            mDisableDLNA = intent.getBooleanExtra("DisableDLAN",false);
            mDisableSAMBA = intent.getBooleanExtra("DisableSAMBA",false);
        }
        //mLoginUser = intent.getStringExtra("Username");
        //mLoginPasswd = intent.getStringExtra("Passwd");
		Log.v(TAG,"BrowserType="+mBrowserType+" MediaType="+mMediaType);
        if (!mDisableDLNA) {
            if (DLNABrowser.isLoaded()){
                DLNABrowser.GetInstance().setRestarted(false);
            }
        }
		mPathInfo = null;
        mPathInfo = intent.getParcelableExtra("PathInfo");
       	if (mPathInfo == null)
		{
        	mPathInfo = new Path_Info();  
        	if(mBrowserType == MbwType.TYPE_DEVICE_ALL||
			   mBrowserType == MbwType.TYPE_DB_PHOTO||
			   mBrowserType == MbwType.TYPE_DB_AUDIO||
			   mBrowserType == MbwType.TYPE_DB_VIDEO)
       		{	
       			mRootPath     ="NONE";
       		}
       		else
       		{
       			Log.d(TAG,"You should provide PathInfo");
       		}	
       		mPathInfo.addLevelInfo(mRootPath);
		}
        
        //GetMimeType
        mMimeTypeXml = getResources().getXml(R.xml.mimetypes);
        mMimeTypes=Util.GetMimeTypes(mMimeTypeXml);
        
        //setListView
        init();
        mState = S_CREATE;
        new InitSharedPrefTask().execute();
    }

    class InitSharedPrefTask extends AsyncTask<Void, Void, Void> {
        ProgressDialog progDlg;
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            progDlg = new ProgressDialog(AbstractFBViewActivity.this);
            progDlg.setIndeterminate(false);
            progDlg.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            progDlg.setCancelable(false);
            progDlg.show();
            log("InitSharedPrefTask progDlg.show()");
        }

        @Override
        protected Void doInBackground(Void... params) {
            UseRtMediaPlayer.getInstance(AbstractFBViewActivity.this).getUseRtPlayer();
            return null;
        }

        @Override
        protected void onPostExecute(Void aVoid) {
            super.onPostExecute(aVoid);
            if(progDlg !=null) {
                log("InitSharedPrefTask progDlg.dismiss()");
                progDlg.dismiss();
            }
        }
    }
  
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        return true;
    }
    
    @Override
    public void onStop()
    {
        super.onStop();
        Log.v(TAG,"onStop");
        FreeReceiverAndInternalHandler(); 
        mState = S_STOP;
    }
    @Override
    public void onDestroy()
    {
    	super.onDestroy();
       	Log.v(TAG, "onDestroy");
		if (mPop!= null)
		{
			if (mPop.isShowing())
				mPop.dismiss();
			mPop = null;
		}
        System.gc();
    }
    //onStart() call NewReceiverAndInternalHandler()
    //onResume() call NewReceiverAndInternalHandler()
    //onPause() call PauseReceiverAndInternalHandler()
    //onStop() call FreeReceiverAndInternalHandler()
    private void PauseReceiverAndInternalHandler()
    {
        if (mInternal_Handler != null)
        {
            mInternal_Handler.removeCallbacksAndMessages(null);
        }
        if (mReceiver != null)
        {
            unregisterReceiver(mReceiver);
        }
    }
    private void FreeReceiverAndInternalHandler()
    {
        if (mInternal_Handler != null)
        {
            mInternal_Handler.removeCallbacksAndMessages(null);
            mInternal_Handler = null;
        }
        if (mReceiver != null)
        {
            //unregisterReceiver(mReceiver);
        	mReceiver = null;
        }
    } 
    private void NewReceiverAndInternalHandler()
    {
        //Receiver
        if (mReceiver == null)
        {
            mReceiver = new MyBroadcastReceiver(All_Dev_DataProvider_handler);
        }
        mIntentFilter = new IntentFilter();
        //mIntentFilter.addAction(Intent.ACTION_MEDIA_EJECT);
        mIntentFilter.addAction(Intent.ACTION_MEDIA_UNMOUNTED);
        mIntentFilter.addAction(Intent.ACTION_MEDIA_REMOVED);
        mIntentFilter.addAction(Intent.ACTION_MEDIA_BAD_REMOVAL);
        mIntentFilter.addAction(Intent.ACTION_MEDIA_MOUNTED);
        mIntentFilter.addDataScheme("file");
        registerReceiver(mReceiver, mIntentFilter);
        mIntentFilter = new IntentFilter();
        //For Network 
            //WiFi
        mIntentFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
        mIntentFilter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
            //Ethernet
        mIntentFilter.addAction(EthernetManager.NETWORK_STATE_CHANGED_ACTION);
        mIntentFilter.addAction(EthernetManager.ETHERNET_STATE_CHANGED_ACTION);
            //Network
        mIntentFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
        mIntentFilter.addAction(ConnectivityManager.INET_CONDITION_ACTION);
        registerReceiver(mReceiver, mIntentFilter);
        //Internal Handler
        if (mInternal_Handler != null)
        {
            mInternal_Handler.removeCallbacksAndMessages(null);
        }
        else
        {
            mInternal_Handler = new Handler()
            {
            	@Override  
                public void handleMessage(Message msg)  
                {
                    if ((mState &(S_PAUSE|S_STOP)) ==0)
                    {
                        switch (msg.what)  
                        {  
                          case CHECK_CUR_PATH_IS_VALID: 
                                if (!CurrentPathIsValid(null))
                                {
                                    BackToMediaBrowserHome();
                                }
                        	  break; 
                          case KEEP_CHECK_CUR_PATH_IS_VALID:
                                if (!CurrentPathIsValid(null))
                                {
                                    BackToMediaBrowserHome();
                                }
                                else
                                {
                                    if (msg.arg1>0)
                                    {
                                        Message Newmsg = obtainMessage(KEEP_CHECK_CUR_PATH_IS_VALID, msg.arg1 -1,0);
                                        sendMessageDelayed(Newmsg, 3000);
                                    }
                                }
                         default:
                        	 break;
                        } 
                    }
                    else
                    {
                        Log.v(TAG,"Internal Handler got Message, but do not thing because we are in STATE:"+mState);
                    } 
                  
                  super.handleMessage(msg);  
                }  
            };
        }
    
    } 
    @Override
    public void onResume() {
        super.onResume();
       	Log.v(TAG,"onResume");
        //Refresh db list
        if(mBrowserType==MbwType.TYPE_DB_VIDEO || mBrowserType==MbwType.TYPE_DB_PHOTO) {
            getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
            if (mDataProvider!=null && mDataProvider.GetSize() > 0) {
                mGenericContentViewFragment.RefreshAbstractView(0);
                mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
            }
        } else if(mBrowserType == MbwType.TYPE_DEVICE_ALL){
            All_Dev_DataProvider_handler.sendEmptyMessageDelayed(UPDATE_ALL_DEV_LIST,100);
        }
        if (mState == S_PAUSE)//If come from onStart(), we don't need to info child, because it have call in onStart().
        {
            NewReceiverAndInternalHandler();
        }
        if (mDataProvider!=null && mDataProvider.GetSize()>0)
        {
            if(mIsStartMusic) {
                //For refresh list when get back from music.
                String musicPath = null;
                ContentResolver contentResolver = getContentResolver();
                Uri queryUri = Uri.parse("content://com.android.music.ContentProvider.MusicUrlPathprovider/urlpath");
                Cursor cursor = contentResolver.query(queryUri, null, null, null, "id desc");
                while(cursor.moveToNext()){
                    int id = cursor.getInt(cursor.getColumnIndex("id"));
                    musicPath = cursor.getString(cursor.getColumnIndex("path"));
                    break;
                }
                int index = -1;
                if(musicPath != null) {
                    index = mDataProvider.GetItemIndex(musicPath);
                }
                //Log.d(TAG,"MUSIC Result musicPath="+musicPath+" index="+index);
                if(index!=-1){
                    mGenericContentViewFragment.RefreshAbstractView(index);
                }
                mIsStartMusic = false;
            }
            mGenericContentViewFragment.RefreshAbstractView(mPathInfo.getLastLevelFocus());
            Child_onItemSelected(mPathInfo.getLastLevelFocus());//Info Child we got focus to trigger preview. 
        }
        Message msg = mInternal_Handler.obtainMessage(KEEP_CHECK_CUR_PATH_IS_VALID, 3,0);
        mInternal_Handler.sendMessageDelayed(msg, 3000);

        mState = S_RESUME;
    }
	
    @Override
    public void onRestart() {
        super.onRestart();
        Log.v(TAG, "onRestart");
        mState = S_RESTART;
    }

    @Override
    public void onStart() {
        super.onStart();
        Log.v(TAG, "onStart");
        if (mState == S_CREATE) {
            if(!(mBrowserType==MbwType.TYPE_DB_VIDEO || mBrowserType==MbwType.TYPE_DB_PHOTO)) {
                getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
                if (mDataProvider.GetSize() > 0) {
                    mGenericContentViewFragment.RefreshAbstractView(0);
                    mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
                }
            }
        }
        NewReceiverAndInternalHandler();
        mState = S_START;
    }

    @Override 
    public void onConfigurationChanged(Configuration newConfig)
    {
        super.onConfigurationChanged(newConfig);
        Log.d(TAG, "Configuration Change :" + newConfig);
        Log.d(TAG, "Configuration Change keyboard: " + newConfig.keyboard);
        Log.d(TAG,"Configuration Change keyboardHidden: "+newConfig.keyboardHidden);

    }
    @Override
    public void onPause() {
        super.onPause();
        Log.v(TAG, "onPause");
        if (mDB != null)//unsaved change should be commit to persistent data in onPause().
        {
            mDB.close();
            mDB = null;
        }
        PauseReceiverAndInternalHandler();
        if (mDataProvider != null)
        {
            mDataProvider.StopQuery();
        }
		if (All_Dev_DataProvider_handler!=null)
		{
			All_Dev_DataProvider_handler.removeCallbacksAndMessages(null);
		}
		if (mDataProviderHandler!=null)
		{
			mDataProviderHandler.removeCallbacksAndMessages(null);
			mDataProviderHandler = null;
		}
        mState = S_PAUSE;
    }
    @Override
    public void onLowMemory()
    {
        super.onLowMemory();
        System.gc();
       	Log.v(TAG,"onLowMemory");
    }
    @Override
    public boolean dispatchKeyEvent(KeyEvent event)
    {
        int keyCode = event.getKeyCode();
        int action = event.getAction();
		log("dispatchKeyEvent Key code=" + keyCode);
        if(mGenericContentViewFragment.mIgnoreKeyEvent) {
            log("mIgnoreKeyEvent==true, pass dispatchKeyEvent");
            return true;
        }
		mGenericContentViewFragment.SetFocus(getCurrentFocus());
    	boolean retval = false;
        if(action == KeyEvent.ACTION_DOWN){
            switch(keyCode){
                case KeyEvent.KEYCODE_BACK:
                    retval = onBackClicked();
                    break;
                case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
                    retval = onPlayClicked();
                    break;
                case KeyEvent.KEYCODE_MENU:
                {
                    Log.d(TAG,"KeyEvent.KEYCODE_MENU");
                    if(mPop == null)
                    {
                        NewOptionMenu();
                        if (mPop!=null && mPop.isInTouchMode()) {
                            Instrumentation ins = new Instrumentation();
                            ins.setInTouchMode(false);
                        }
                        if(mPop!=null) {
                            if (mPosition == 0) {
                                mPop.setDefaultFocus("mb_thumbnailview", true);
                            } else {
                                mPop.setDefaultFocus("mb_preview", true);
                            }
                            mPop.Show();
                        }
                    }
                    else
                    {
                        if (mPop.isShowing())
                        {
                            mPop.Hide();
                        }
                        else
                        {
                            if (mPop.isInTouchMode()) {
                                Instrumentation ins = new Instrumentation();
                                ins.setInTouchMode(false);
                            }
                            mPop.Show();
                        }
                    }
                    retval = true;
                    break;
                }
            }
        }

    	if(!retval)
    	{	
    		retval =  super.dispatchKeyEvent(event);
    	}
    	
    	return retval;
    }
    

    private boolean CurrentPathIsValid(String ChangeIOPath)
    {
        boolean rlt = false;
	    if (mBrowserType == MbwType.TYPE_DEVICE_USB)
        {
            if (ChangeIOPath != null)
            {
                //ChangeIOPath may be content:///dev/null
                if(ChangeIOPath.contains("/storage")) {
                    String Path = ChangeIOPath.substring(ChangeIOPath.indexOf("/storage"));
                    String CurPath = mPathInfo.getLastLevelPath();
                    if (!CurPath.contains(Path)) {
                        rlt = true;
                    }
                }
            }
            else
            {
                File file = new File(mPathInfo.getLastLevelPath());
                Log.d(TAG,"File:"+mPathInfo.getLastLevelPath()+", exists:"+file.exists()+", CanRead:"+file.canRead());
                rlt = file.exists() && file.canRead(); 
                //rlt = IsexistInMountPath(mPathInfo.getLastLevelPath());
            }
        }
        else if (mBrowserType == MbwType.TYPE_DEVICE_DMS||mBrowserType == MbwType.TYPE_DEVICE_SMB)
        {
                rlt = hasNetwork();
                //rlt = true;
        }
        else if (mBrowserType == MbwType.TYPE_DEVICE_ALL)
        {
            if (!hasNetwork())
            {
                mDataProvider.SetSystemState(AbstractDataProvider.NETWORK_DISCONNECT);
            }
            else
            {
                mDataProvider.ClearSystemState(AbstractDataProvider.NETWORK_DISCONNECT);
            }
            rlt = true;
        }
        else//DB
        {
            rlt = ChangeIOPath == null || !ChangeIOPath.contains("/storage");
        }
        return rlt;
    }
    public boolean IsexistInMountPath(String CheckingPath)
    {
        File file = new File("/proc/mounts");
        boolean rlt = false;
        if (file.canRead())
        {
            BufferedReader reader = null;
            try {
                reader = new BufferedReader(new FileReader(file));
                String text;
                int index = 0;
                while ((text = reader.readLine()) != null) {
                    String[] segs = text.split(" ");
                    if (segs.length > 1)
                    {
                        if (segs[0].indexOf("fuse") >= 0 && segs[1].indexOf("storage") >= 0 && segs[1].indexOf("emulated") == -1 && segs[1].indexOf("mmcblk") == -1)
                        {
                          //Check CheckingPath is match or not
                          Log.d(TAG,"mount Path:"+segs[1]+"CheckingPath:"+CheckingPath);
                          if (CheckingPath.indexOf(segs[1])>-1)
                          {
                            rlt = true;
                            break;
                          }
                        }
                    }
                }
            }catch (IOException e) {
                Log.d(TAG, "IOException", e);
            } finally {
                try {
                    if (reader != null) {
                        reader.close();
                    }
                } catch (IOException e) {
                    Log.d(TAG, "IOException", e);
                }
            }
        }
        return rlt;
    }

    private boolean hasNetwork() {
        mNetworkConnected = Util.hasNetwork(this);
        return mNetworkConnected;
    }

    private void BackToMediaBrowserHome()
    {
        if(mIsGetContentMode) {
            finish();
            return;
        }
	    ComponentName componetName = new ComponentName("com.rtk.mediabrowser",
	                "com.rtk.mediabrowser.MediaBrowser");
	    Intent intent = new Intent();
        intent.setComponent(componetName);
		intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
	    startActivity(intent);
        overridePendingTransition(0,0);
    }
    
    private void init()
    { 
        Child_Init();

		if(m_ViewType == GenericContentViewFragment.GRID_VIEW)
		{
        	mGenericContentViewFragment = (GenericContentViewFragment)(getFragmentManager().findFragmentById(R.id.genericview_grid_fragment));
		}
		else
		{
        	mGenericContentViewFragment = (GenericContentViewFragment)(getFragmentManager().findFragmentById(R.id.genericview_list_fragment));
		}
        //Set media dependent View
        mMainview = (RelativeLayout)findViewById(R.id.background);
		mMainview.setFocusable(false);
        mTopMediaInfoTextView = (TextView)findViewById(R.id.topMediaInfo);
        mTopMediaInfoTextView.setFocusable(false);
        mTopMediaInfoImageView = (ImageView)findViewById(R.id.topMediaInfoBg);
        mTopMediaInfoImageView.setFocusable(false);
        setMediaDependentView();
        mTopMediaPathInfoTextView = (TextView)findViewById(R.id.topPathInfo);
        mTopMediaPathInfoTextView.setFocusable(false);
        mTopMediaPathInfoImageView = (ImageView)findViewById(R.id.topPathIcon);
        mTopMediaPathInfoImageView.setFocusable(false);
        mTopMediaPathLine0ImageView = (ImageView)findViewById(R.id.TitleLine0);
        mTopMediaPathLine0ImageView.setFocusable(false);
        mTopMediaPathLine1ImageView = (ImageView)findViewById(R.id.TitleLine1);
        mTopMediaPathLine1ImageView.setFocusable(false);
        setTopMediaPathInfoTextView();
        
        loading_icon_system_handle = new Handler()
        {
        	private int loading_num = 0;
        	@Override  
            public void handleMessage(Message msg)  
            {
        		
            	switch (msg.what)  
                {  
                  case 1:
                	  loading_num++;      	  
                	  break;  
                  case 0:
                	  loading_num--;
                	  break;
                 default:
                	 break;
                }  
            	if(loading_num > 0)
            	{
            		topLoadingIcon.setVisibility(View.VISIBLE);
            		ad.start();
            	}
            	else if(loading_num == 0)
            	{
            		ad.stop();
            		topLoadingIcon.setVisibility(View.INVISIBLE);
            	}
            	else
            		Log.e(TAG,"loading icon error");
              super.handleMessage(msg);  
            }  
        	
        };
        
        topLoadingIcon=(ImageView)findViewById(R.id.topLoadingIcon);
        topLoadingIcon.setBackgroundResource(R.drawable.load_anim_list);
        topLoadingIcon.setVisibility(View.INVISIBLE);
        ad = (AnimationDrawable)topLoadingIcon.getBackground();
        
    }

    private void setTopMediaPathInfoTextView()
    {
    	if(mTopMediaPathInfoImageView != null && mTopMediaPathInfoTextView != null && mPathInfo.getLastLevelPath()!=null)
		{
			if (mBrowserType == MbwType.TYPE_DEVICE_USB||mBrowserType == MbwType.TYPE_DEVICE_DMS||mBrowserType == MbwType.TYPE_DEVICE_SMB)
			{
				mTopMediaPathInfoTextView.setVisibility(View.VISIBLE);
				mTopMediaPathInfoImageView.setVisibility(View.VISIBLE);
				mTopMediaPathLine0ImageView.setVisibility(View.VISIBLE);
				mTopMediaPathLine1ImageView.setVisibility(View.VISIBLE);
    			mTopMediaPathInfoTextView.setText(mPathInfo.getLastLevelPath());
				Log.d(TAG,"mPathInfo.getLastLevelPath:"+mPathInfo.getLastLevelPath());
				if (mBrowserType == MbwType.TYPE_DEVICE_USB)
					mTopMediaPathInfoImageView.setImageResource(R.drawable.icon_usb_path);
				else
					mTopMediaPathInfoImageView.setImageResource(R.drawable.icon_network_path);
					
			}
			else
			{
				//Set path related view to invisible
				mTopMediaPathInfoTextView.setVisibility(View.INVISIBLE);
				mTopMediaPathInfoImageView.setVisibility(View.INVISIBLE);
				mTopMediaPathLine0ImageView.setVisibility(View.INVISIBLE);
				mTopMediaPathLine1ImageView.setVisibility(View.INVISIBLE);
			}
		} else if(mBrowserType == MbwType.TYPE_DEVICE_QUERY) {
    	    if(mTopMediaPathInfoTextView!=null) {
                mTopMediaPathInfoTextView.setVisibility(View.VISIBLE);
                mTopMediaPathInfoTextView.setText("Search: "+mQueryString);
            }
        }
    }
    
    private void setMediaDependentView()
    {
        if(mMainview != null && mTopMediaInfoTextView != null && mTopMediaInfoImageView != null)
        {
			switch(mBrowserType)
			{
				case MbwType.TYPE_DEVICE_PHOTO:
					mMainview.setBackgroundResource(R.drawable.bg);
            		mTopMediaInfoTextView.setText(getString(R.string.mb_photo));
					mTopMediaInfoImageView.setImageResource(R.drawable.button_photo_up);
				break;
				case MbwType.TYPE_DEVICE_VIDEO:
					mMainview.setBackgroundResource(R.drawable.bg);
            		mTopMediaInfoTextView.setText(getString(R.string.mb_video));
					mTopMediaInfoImageView.setImageResource(R.drawable.button_video_up);
				break;
				case MbwType.TYPE_DEVICE_AUDIO:
					mMainview.setBackgroundResource(R.drawable.bg);
            		mTopMediaInfoTextView.setText(getString(R.string.mb_music));
					mTopMediaInfoImageView.setImageResource(R.drawable.button_allfile_up);
				break;
				case MbwType.TYPE_DEVICE_ALL:
				case MbwType.TYPE_DEVICE_USB:
				case MbwType.TYPE_DEVICE_DMS:
				case MbwType.TYPE_DEVICE_SMB:
					mMainview.setBackgroundResource(R.drawable.bg);
            		mTopMediaInfoTextView.setText(getString(R.string.mb_allfile));
					mTopMediaInfoImageView.setImageResource(R.drawable.button_allfile_up);
				break;
			}
        }    
    }
    
    private void getDataProvider(String path, int SortMode)
    {
        mStopUpdateContentList = false;
		Log.d(TAG,Thread.currentThread().getStackTrace()[2].getMethodName()+"() Path:"+path);
        if(mPathInfo.getLastLevelPath() != null || mMimeTypeXml!=null ) 
        {   
            mDataProvider = null;
			if(mBrowserType == MbwType.TYPE_DEVICE_PHOTO)
			{
                if (mDataProviderHandler == null)
                {
			        mDataProviderHandler = new DataProviderHandler();
                }
                if (mDataProvider != null)
                {
                    mDataProvider.StopQuery();
                }
				if(mPathInfo.getLastLevel()==0){//mMediaType = FileFilterType.DEVICE_FILE_PHOTO_DATE or FileFilterType.DEVICE_FILE_PHOTO 
				    mDataProvider = new DBFileDataProvider(mMediaType, -1, SortMode, mMimeTypes, this, mDataProviderHandler);
                }
                else{//PHOTO DATE second level
					mDataProvider = new DBFileDataProvider(path, -1, mMimeTypes, this, mDataProviderHandler);
                }
                
			}
			else if(mBrowserType == MbwType.TYPE_DEVICE_VIDEO)	
			{
                if (mDataProviderHandler == null)
                {
			        mDataProviderHandler = new DataProviderHandler();
                }
                if (mDataProvider != null)
                {
                    mDataProvider.StopQuery();
                }
				mDataProvider = new DBFileDataProvider(mMediaType, -1, SortMode, mMimeTypes, this, mDataProviderHandler);
			}
			else if(mBrowserType == MbwType.TYPE_DEVICE_AUDIO)	
			{
                if (mDataProviderHandler == null)
                {
			        mDataProviderHandler = new DataProviderHandler();
                }
                if (mDataProvider != null)
                {
                    mDataProvider.StopQuery();
                }
				mDataProvider = new DBFileDataProvider(mMediaType, -1, SortMode, mMimeTypes, this, mDataProviderHandler);
			}
			else if(mBrowserType == MbwType.TYPE_DEVICE_QUERY)
            {
                if (mDataProviderHandler == null)
                {
                    mDataProviderHandler = new DataProviderHandler();
                }
                if (mDataProvider != null)
                {
                    mDataProvider.StopQuery();
                }
                if(mQueryString !=null){
                    mDataProvider = new QueryDataProvider(mMediaType, mQueryString, -1, SortMode, mMimeTypes, this, mDataProviderHandler);
                }
            }
			else if(mBrowserType == MbwType.TYPE_DEVICE_ALL)	
			{
                mFirst_SMB_QUERY_ALL_DOMAIN_DONE= false;
                GotNewItem = 0;//When we new mDataProvider, should clean GotNewItem because that may have old info.;
                int type = AllDeviceDataProvider.UPDATE_QUERY_USB;
                if (!mDisableDLNA)
                    type = type|AllDeviceDataProvider.UPDATE_QUERY_DMS;
                if (!mDisableSAMBA)
                    type = type|AllDeviceDataProvider.UPDATE_QUERY_SMB;

				mDataProvider = new AllDeviceDataProvider(All_Dev_DataProvider_handler,getApplicationContext(),type);
        		All_Dev_DataProvider_handler.sendEmptyMessageDelayed(UPDATE_ALL_DEV_LIST,50);
			}
			else if(mBrowserType == MbwType.TYPE_DEVICE_USB)
			{
                if (mDataProviderHandler == null)
                {
			        mDataProviderHandler = new DataProviderHandler();
                }
                if (mDataProvider != null)
                {
                    mDataProvider.StopQuery();
                }
				mDataProvider = new DeviceFileDataProvider(path,mMediaType,-1,SortMode,mMimeTypes, mDataProviderHandler);
			}	
			else if(mBrowserType == MbwType.TYPE_DEVICE_DMS)
			{
                if (mDataProviderHandler == null)
                {
			        mDataProviderHandler = new DataProviderHandler();
                }
                if (mDataProvider != null)
                {
                    mDataProvider.StopQuery();
                }
			 	mDataProvider = new DLNAFileDataProvider(path, mMediaType,-1,SortMode,mMimeTypes,mDataProviderHandler);
			}	
			else if(mBrowserType == MbwType.TYPE_DEVICE_SMB)
			{
                if (mDataProviderHandler == null)
                {
			        mDataProviderHandler = new DataProviderHandler();
                }
                if (mDataProvider != null)
                {
                    mDataProvider.StopQuery();
                }
                String user=Const.GUEST;
                String password="";
                String[] infos = tryGetSmbLoginInfo(path);
                if(infos!=null) {
                    if (infos.length >= 1)
                        user = infos[0];
                    if (infos.length >= 2)
                        password = infos[1];
                }

				mDataProvider = new SMBFileDataProvider(path, mMediaType,-1,SortMode,mMimeTypes, getApplicationContext(), mDataProviderHandler,user,password);
			}
          	//mDataProvider.sortListByType();
            //Log.d(TAG,"getDataProvider:size:"+mDataProvider.GetSize());
        }
    }

    private String[] tryGetSmbLoginInfo(String smbPath) {
        log(String.format("tryGetSmbLoginInfo smbPath=%s", smbPath));
        SmbUserMgr mgr = SmbUserMgr.getInstance(this);
        log("getUserInfo smbPath=" + smbPath);
        String userInfo = mgr.getUserInfo(smbPath);
        if (userInfo != null) {
//                            log("getUserInfo userInfo="+userInfo);
            return userInfo.split(SmbUserMgr.USERINFO_SPLIT);
        }
        return null;
    }

  	//If you use this abstract view, you should implement below API. 
	public abstract void Child_Init();//SubClass should do something about onCreate() in this API, instead of onCreate().BECAUSE that have some cooperat thing here.
	public abstract void Child_onItemSelected(int position);
	public abstract boolean Child_OptionHandle();
    public final static int VIEW_TYPE_THUMBNAIL = 0;
    public final static int VIEW_TYPE_PREVIEW = 1;
    public abstract int Child_ViewType();

	//Don't forget include layout/media_info.xml
	
    public AbstractDataProvider getCurrentDataProvider()
    {
        return mDataProvider;
    }
	public int getCurrentMbwType()
	{
		return mBrowserType;
	}
    public void onItemSelected(int position)
    {
    	Log.d(TAG,"ItemSelected   position is :"+position);
		Child_onItemSelected(position);
    	mPathInfo.setLastLevelFocus(position);
		mSelectedPosition = position;
    }

    class BackgroundTask extends AsyncTask<Integer, Void, DataProviderStruct> {
        ProgressDialog progDlg;
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            progDlg = new ProgressDialog(AbstractFBViewActivity.this);
            progDlg.setIndeterminate(false);
            progDlg.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            progDlg.setCancelable(true);
            progDlg.show();
            log("BackgroundTask show progDlg,"+Thread.currentThread().getId());
            onItemClickedPreAction();
        }

        @Override
        protected DataProviderStruct doInBackground(Integer... integers) {
            return onItemClickedGetModel(integers[0]);
        }

        @Override
        protected void onPostExecute(DataProviderStruct stru) {
            if(progDlg !=null) {
                log("BackgroundTask dismiss progDlg"+Thread.currentThread().getId());
                progDlg.dismiss();
                progDlg = null;
            }
            if(stru.isFailed) {
                Toast.makeText(AbstractFBViewActivity.this, R.string.no_file, Toast.LENGTH_SHORT).show();
                return;
            } else {
                onItemClickedPostAction(stru);
            }
            super.onPostExecute(stru);
        }
    }

    private void onItemClickedPreAction() {
        log("onItemClickedPreAction");
//Don't ask stop query in some Browser type, because we need query work complete done for creating playlist(ex:USB, DB video....), should ask don't update forground list and UI instead. But
        if (mBrowserType == MbwType.TYPE_DEVICE_ALL) {
            mDataProvider.StopQuery();
        }
        mStopUpdateContentList = true;
        mGenericContentViewFragment.stopGridViewDecode();
        if (All_Dev_DataProvider_handler != null) {
            All_Dev_DataProvider_handler.removeCallbacksAndMessages(null);
        }
        if (mDataProviderHandler != null) {
            mDataProviderHandler.removeCallbacksAndMessages(null);
        }
    }

    private DataProviderStruct onItemClickedGetModel(int pos) {
        log("onItemClickedGetModel");
        mPathInfo.setLastLevelFocus(pos);
        pos = mPathInfo.getLastLevelFocus();

        DataProviderStruct stru = new DataProviderStruct();
        if (mDataProvider == null) {
            stru.isFailed = true;
            log("mDataProvider is null");
            return null;
        }
        stru.GetSize = mDataProvider.GetSize();
        Log.v(TAG, "onItemClicked LastFocus:" + pos + "DataProvider size:" + mDataProvider.GetSize());
        if (mDataProvider != null && pos >= 0 && pos < mDataProvider.GetSize()) {
            Log.v(TAG, "focus: " + pos + "File Type:" + mDataProvider.GetFileTypeAt(pos));
        } else {
            log("mDataProvider or position is invalid");
        }
        if (pos >= 0 && pos < stru.GetSize) {
            stru.GetFileTypeAt = mDataProvider.GetFileTypeAt(pos);
            stru.GetDataAt = mDataProvider.GetDataAt(pos);
            if(mBrowserType == MbwType.TYPE_DEVICE_DMS
                    && (stru.GetDataAt==null || stru.GetDataAt.length()==0)){
                stru.GetSizeAt = mDataProvider.GetSizeAt(pos);
                if(stru.GetSizeAt==-1) {    //server is down
                    stru.isFailed = true;
                    return stru;
                }
                stru.GetDataAt = mDataProvider.GetDataAt(pos);
            }

            switch (stru.GetFileTypeAt) {
                case FileFilterType.DEVICE_FILE_PHOTO:
                    stru.GetPlayList = mDataProvider.GetPlayList(FileFilterType.DEVICE_FILE_PHOTO);
                    while (stru.GetPlayList == null) {
                        try {
                            Thread.sleep(500);
                        } catch (InterruptedException ex) {
                            Thread.currentThread().interrupt();
                        }
                        stru.GetPlayList = mDataProvider.GetPlayList(FileFilterType.DEVICE_FILE_PHOTO);
                    }
                    stru.GetNameList = mDataProvider.GetNameList(FileFilterType.DEVICE_FILE_PHOTO);
                    break;
                case FileFilterType.DEVICE_FILE_VIDEO:
                    stru.GetPlayList = mDataProvider.GetPlayList(FileFilterType.DEVICE_FILE_VIDEO);
                    while (stru.GetPlayList == null) {
                        try {
                            Thread.sleep(500);
                        } catch (InterruptedException ex) {
                            Thread.currentThread().interrupt();
                        }
                        stru.GetPlayList = mDataProvider.GetPlayList(FileFilterType.DEVICE_FILE_VIDEO);
                    }
                    stru.GetNameList = mDataProvider.GetNameList(FileFilterType.DEVICE_FILE_VIDEO);
                    break;
                case FileFilterType.DEVICE_FILE_AUDIO:
                    stru.GetPlayList = mDataProvider.GetPlayList(FileFilterType.DEVICE_FILE_AUDIO);
                    while (stru.GetPlayList == null) {
                        try {
                            Thread.sleep(500);
                        } catch (InterruptedException ex) {
                            Thread.currentThread().interrupt();
                        }
                        stru.GetPlayList = mDataProvider.GetPlayList(FileFilterType.DEVICE_FILE_AUDIO);
                    }
                    stru.GetNameList = mDataProvider.GetNameList(FileFilterType.DEVICE_FILE_AUDIO);
                    break;
                case FileFilterType.DEVICE_FILE_DIR:
                case FileFilterType.DEVICE_FILE_DLNA_DEVICE:
                    stru.GetTitleAt = mDataProvider.GetTitleAt(pos);
                    break;
            }

        } else {
            stru.isFailed = true;
        }
        return stru;
    }

    private void grantReadPermission(Context ctx, Intent intent, String[] urls) {
        if(urls==null) return;
        String packageName = intent.getComponent().getPackageName();
        Log.d(TAG, "grantReadPermission packageName="+packageName);
        for(String url : urls) {
            Uri uri = Uri.parse(url);
            ctx.grantUriPermission(packageName, uri, Intent.FLAG_GRANT_READ_URI_PERMISSION);
        }
    }

    private void onItemClickedPostAction(DataProviderStruct stru){
        log("onItemClickedPostAction, stru.GetFileTypeAt="+stru.GetFileTypeAt);
        if(stru.isFailed) {
            log("onItemClickedGetModel is failed");
            return;
        }
        if(mIsGetContentMode){
            Toast.makeText(this, getResources().getString(R.string.press_option_to_select_folder), Toast.LENGTH_LONG).show();
        }
        switch (stru.GetFileTypeAt)
        {
            case FileFilterType.DEVICE_FILE_APK:
            {
                Intent intent = new Intent(Intent.ACTION_VIEW);
                String filePath = stru.GetDataAt;
                if(!filePath.startsWith("http://"))
                    filePath = "file://"+filePath;
                intent.setDataAndType(Util.getUri(this, filePath), "application/vnd.android.package-archive");
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK
                        | Intent.FLAG_ACTIVITY_NO_ANIMATION
                        | Intent.FLAG_GRANT_READ_URI_PERMISSION
                );
                startActivity(intent);
            }
            break;
            case FileFilterType.DEVICE_FILE_PHOTO:
            {
                String filePath = stru.GetDataAt;
                if(!filePath.startsWith("http://"))
                    filePath = "file://"+filePath;
                Uri uri = Util.getUri(this, filePath);
                Intent intent = new Intent();
                ComponentName comp = new ComponentName("com.android.gallery3d", "com.android.gallery3d.app.GalleryActivity");
                intent.setComponent(comp);
                if (mBrowserType == MbwType.TYPE_DEVICE_DMS||mBrowserType == MbwType.TYPE_DEVICE_SMB)
                    intent.putExtra("SourceFrom","Network");
                else
                    intent.putExtra("SourceFrom","Local");
                intent.setAction(Intent.ACTION_VIEW);
                intent.setDataAndType(uri, "image/*");
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK
                        | Intent.FLAG_ACTIVITY_CLEAR_TASK
                        | Intent.FLAG_ACTIVITY_NO_ANIMATION);
                ArrayList<String> playlist = stru.GetPlayList;
                while(playlist == null){
                    try{
                        Thread.sleep(500);
                    }catch (InterruptedException ex){
                        Thread.currentThread().interrupt();
                    }
                    playlist = stru.GetPlayList;
                }
                ArrayList<String> playlist_title = stru.GetNameList;
                int positionInPlaylist = -1;
                int size = playlist.size();
                filePath = uri.toString();
                for (int i = 0; i <size ; i++)
                {
                    if (filePath.equals(playlist.get(i)))
                    {
                        positionInPlaylist = i;
                        break;
                    }
                }
                Log.v(TAG,"size of playlist="+size+" position="+positionInPlaylist);
                if (size >100)
                {
                    PlaylistToFile(PLAYLIST_FILE, playlist);
                    intent.putExtra("PlayListAddress", PLAYLIST_FILE);
                    PlaylistToFile(PLAYLIST_FILE_TITLE, playlist_title);
                    intent.putExtra("PlayListTitleAddress", PLAYLIST_FILE_TITLE);
                }
                else
                {
                    String[] playlistArray = new String[playlist.size()];
                    intent.putExtra("PlayListAddress","Intent");
                    intent.putExtra("playlist", playlist.toArray(playlistArray));
                    String[] playlistTitleArray = new String[playlist_title.size()];
                    intent.putExtra("PlayListTitleAddress","Intent");
                    intent.putExtra("playlistTitle", playlist_title.toArray(playlistTitleArray));
                }
                intent.putExtra("index-hint", positionInPlaylist);
                intent.putExtra("slideshow", false);
                //playlist.clear();
                //playlist_title.clear();
                Log.v(TAG,"PlayList Create Done.");
                startActivity(intent);
            }
            break;
            case FileFilterType.DEVICE_FILE_VIDEO:
            {
                String filePath = stru.GetDataAt;
                if(!filePath.startsWith("http://")) {
                    filePath = "file://" + filePath;
                }
                Uri uri = Util.getUri(this, filePath);
                final Intent intent = new Intent();
                ComponentName comp = new ComponentName("com.android.gallery3d", "com.android.gallery3d.app.MovieActivity");
                intent.setComponent(comp);
                if (mBrowserType == MbwType.TYPE_DEVICE_DMS||mBrowserType == MbwType.TYPE_DEVICE_SMB)
                    intent.putExtra("SourceFrom","Network");
                else
                    intent.putExtra("SourceFrom","Local");
                if(mBrowserType==MbwType.TYPE_DEVICE_DMS){
                    intent.putExtra(EXTRA_DVDPLAYER_EXTENSION, "forceSeek forcerange");
                }
                intent.setAction(Intent.ACTION_VIEW);
                intent.setData(uri);
                intent.putExtra(MEDIA_BROWSER_USE_RT_MEDIA_PLAYER, UseRtMediaPlayer.getInstance(this).getUseRtPlayer());
                //intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
                // Blocked until MovieActivity returned.
                intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK| Intent.FLAG_ACTIVITY_NO_ANIMATION);
                ArrayList<String> playlist = stru.GetPlayList;
                while(playlist == null){
                    try{
                        Thread.sleep(500);
                    }catch (InterruptedException ex){
                        Thread.currentThread().interrupt();
                    }
                    playlist = stru.GetPlayList;
                }
//                log("uri="+uri);
//                log("playlist="+Arrays.toString(playlist.toArray()));
                ArrayList<String> playlist_title = stru.GetNameList;
                int positionInPlaylist = -1;
                int size = playlist.size();
                filePath = uri.toString();
                for (int i = 0; i <size ; i++)
                {
                    if (filePath.equals(playlist.get(i)))
                    {
                        positionInPlaylist = i;
                        break;
                    }
                }
                Log.v(TAG,"size of playlist="+size+" position="+positionInPlaylist);
                if ((size >40)||(mBrowserType == MbwType.TYPE_DEVICE_DMS)||(mBrowserType == MbwType.TYPE_DEVICE_SMB))
                {
                    PlaylistToFile(PLAYLIST_FILE, playlist);
                    intent.putExtra("PlayListAddress",PLAYLIST_FILE);
                    PlaylistToFile(PLAYLIST_FILE_TITLE, playlist_title);
                    intent.putExtra("PlayListTitleAddress", PLAYLIST_FILE_TITLE);
                }
                else
                {
                    String[] playlistArray = new String[playlist.size()];
                    intent.putExtra("PlayListAddress","Intent");
                    intent.putExtra("playlist", playlist.toArray(playlistArray));
                    String[] playlistTitleArray = new String[playlist_title.size()];
                    intent.putExtra("PlayListTitleAddress","Intent");
                    intent.putExtra("playlistTitle", playlist_title.toArray(playlistTitleArray));
                }
                intent.putExtra("index-hint", positionInPlaylist);
                //playlist.clear();
                //playlist_title.clear();
                // BUG_FIX: 42659
                // startActivity(intent);
                //If that have decode video frame work in ImageWorker, wait it done.
                mGenericContentViewFragment.stopGridViewDecode();//stop all task
                mGenericContentViewFragment.WaitResourceRelease(this, new Runnable() {
                    @Override
                    public void run() {
                        startActivityForResult(intent, MOVIE_REQUEST_CODE);
                    }
                });//block API

            }
            //Ask Playback
            break;
            case FileFilterType.DEVICE_FILE_AUDIO:
            {
                String filePath = stru.GetDataAt;
                if(!filePath.startsWith("http://"))
                    filePath = "file://"+filePath;
                filePath = Util.getUri(this, filePath).toString();
                Intent intent = new Intent();
                ComponentName comp = new ComponentName("com.android.music", "com.android.music.MediaPlaybackActivity");
                intent.setComponent(comp);
                if (!SINGLE_MUSIC_PLAYBACK)
                {
                    ArrayList<String> playlist = stru.GetPlayList;
                    while(playlist == null){
                        try{
                            Thread.sleep(500);
                        }catch (InterruptedException ex){
                            Thread.currentThread().interrupt();
                        }
                        playlist = stru.GetPlayList;
                    }
                    ArrayList<String> playlist_title = stru.GetNameList;
                    int positionInPlaylist = -1;
                    int size = playlist.size();
                    for (int i = 0; i <size ; i++)
                    {
                        if (mBrowserType == MbwType.TYPE_DEVICE_DMS)
                        {
                            if (filePath.regionMatches(0,playlist.get(i),0,filePath.length()))
                            {
                                positionInPlaylist = i;
                                break;
                            }
                        }
                        else if(mBrowserType == MbwType.TYPE_DEVICE_SMB)
                        {
                            if (filePath.regionMatches(0,playlist.get(i),0,filePath.length()))
                            {
                                positionInPlaylist = i;
                                break;
                            }
                        }
                        else
                        {
                            if (filePath.equals(playlist.get(i)))
                            {
                                positionInPlaylist = i;
                                break;
                            }
                        }
                    }
                    Log.v(TAG,"size of playlist="+playlist.size()+" position="+positionInPlaylist);
                    if (size >100)
                    {
                        PlaylistToFile(PLAYLIST_FILE, playlist);
                        String[] playlistArray = new String[playlist.size()];
                        grantReadPermission(this, intent, playlist.toArray(playlistArray));
                        intent.putExtra("PlayListAddress",PLAYLIST_FILE);
                        PlaylistToFile(PLAYLIST_FILE_TITLE, playlist_title);
                        intent.putExtra("PlayListTitleAddress", PLAYLIST_FILE_TITLE);
                    }
                    else
                    {
                        String[] playlistArray = new String[playlist.size()];
                        intent.putExtra("PlayListAddress","Intent");
                        intent.putExtra("playlist-url", playlist.toArray(playlistArray));
                        grantReadPermission(this, intent, playlist.toArray(playlistArray));
                        String[] playlistTitleArray = new String[playlist_title.size()];
                        intent.putExtra("PlayListTitleAddress","Intent");
                        intent.putExtra("playlistTitle", playlist_title.toArray(playlistTitleArray));
                    }
                    intent.putExtra("position", positionInPlaylist);
                    //playlist.clear();
                    //playlist_title.clear();
                }
                else
                {
                    ArrayList<String> playlist = new ArrayList<String>();
                    playlist.add(stru.GetDataAt);
                    String[] playlistArray = new String[playlist.size()];
                    intent.putExtra("PlayListAddress","Intent");
                    intent.putExtra("playlist-url", playlist.toArray(playlistArray));
                    grantReadPermission(this, intent, playlist.toArray(playlistArray));
                    playlist.clear();
                }
                if (mBrowserType == MbwType.TYPE_DEVICE_DMS||mBrowserType == MbwType.TYPE_DEVICE_SMB)
                    intent.putExtra("SourceFrom","Network");
                else
                    intent.putExtra("SourceFrom","Local");
                intent.putExtra(MEDIA_BROWSER_USE_RT_MEDIA_PLAYER, UseRtMediaPlayer.getInstance(this).getUseRtPlayer());
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK
                        | Intent.FLAG_ACTIVITY_CLEAR_TASK
                        | Intent.FLAG_ACTIVITY_NO_ANIMATION);
                mIsStartMusic = true;
                startActivity(intent);
            }
            //Ask Playback
            break;
            case FileFilterType.DEVICE_FILE_PLAYLIST:
            {
                Log.v(TAG,"Let's play music playlist.");
                ArrayList<String> playlist = new ArrayList<String>();
                Intent intent = new Intent();
                String filePath = stru.GetDataAt;
                if(!filePath.startsWith("http"))
                    filePath = "file://" + filePath;
                playlist.add(filePath);
                String[] playlistArray = new String[playlist.size()];
                intent.putExtra("PlayListAddress","Intent");
                intent.putExtra("playlist-url", playlist.toArray(playlistArray));
                //Log.d(TAG,"music play list:"+playlist.toArray(playlistArray)[0]);
                playlist.clear();
                ComponentName comp = new ComponentName("com.android.music", "com.android.music.MediaPlaybackActivity");
                intent.setComponent(comp);
                if (mBrowserType == MbwType.TYPE_DEVICE_DMS||mBrowserType == MbwType.TYPE_DEVICE_SMB)
                    intent.putExtra("SourceFrom","Network");
                else
                    intent.putExtra("SourceFrom","Local");
                intent.putExtra(MEDIA_BROWSER_USE_RT_MEDIA_PLAYER, UseRtMediaPlayer.getInstance(this).getUseRtPlayer());
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK
                        | Intent.FLAG_ACTIVITY_CLEAR_TASK
                        | Intent.FLAG_ACTIVITY_NO_ANIMATION);
                startActivity(intent);
            }
            break;
            case FileFilterType.DEVICE_FILE_DEVICE:
            {
                //String path = mDataProvider.GetTitleAt(mPathInfo.getLastLevelFocus())+"/";
                String path = stru.GetDataAt;
                mPathInfo.cleanLevelInfo();
                mPathInfo.addLevelInfo(path);
                Log.v(TAG," Go to USB:"+mPathInfo.getLastLevelPath());
                mBrowserType = MbwType.TYPE_DEVICE_USB;
                mMediaType = mMediaType|FileFilterType.DEVICE_FILE_APK;
                getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
                setTopMediaPathInfoTextView();
                    /*
                    if (mDataProvider.GetSize()>0)
                    {
            		    mGenericContentViewFragment.RefreshAbstractView(0);
                    }
                    */
                mGenericContentViewFragment.RefreshAbstractView(0);
                mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
            }
            break;
            case FileFilterType.DEVICE_FILE_SMB_DEVICE:
            {
                if (mNetworkConnected)
                {
                    String path = stru.GetDataAt;
                    //String path = "//Rtdomain/Rtcn5405-25-253/";
                    mPathInfo.cleanLevelInfo();
                    mPathInfo.addLevelInfo(path);
                    Log.v(TAG," Go to SMB:"+mPathInfo.getLastLevelPath());
                    mBrowserType = MbwType.TYPE_DEVICE_SMB;
                    getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
                    setTopMediaPathInfoTextView();
                        /*
                        if (mDataProvider.GetSize()>0)
                        {
            		        mGenericContentViewFragment.RefreshAbstractView(0);
                        }
                        */
                    mGenericContentViewFragment.RefreshAbstractView(0);
                    mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
                }
                else
                {
                    Toast.makeText(getApplicationContext(),getString(R.string.network_NG), Toast.LENGTH_LONG).show();
                }
            }
            break;
            case FileFilterType.DEVICE_FILE_DLNA_DEVICE:
            {
                if (mNetworkConnected)
                {
                    String DMSRootPath = DLNABrowser.GetInstance().GetPluginRootPath();
                    String path = DMSRootPath + stru.GetTitleAt + "/";
                    //String path = mDataProvider.GetDataAt(mPathInfo.getLastLevelFocus());//Wen: Should not use GetDataAt, it will get null string from DMP lib.
                    mPathInfo.cleanLevelInfo();
                    mPathInfo.addLevelInfo(path);
                    Log.v(TAG," Go to DMS:"+mPathInfo.getLastLevelPath());
                    mBrowserType = MbwType.TYPE_DEVICE_DMS;
                    getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
                    setTopMediaPathInfoTextView();
                        /*
                        if (mDataProvider.GetSize()>0)
                        {
            		        mGenericContentViewFragment.RefreshAbstractView(0);
                        }
                        */
                    mGenericContentViewFragment.RefreshAbstractView(0);
                    mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
                }
                else
                {
                    Toast.makeText(getApplicationContext(),getString(R.string.network_NG), Toast.LENGTH_LONG).show();
                }
            }
            break;
            case FileFilterType.DEVICE_FILE_DIR:
            {
                String path;
                if (mBrowserType == MbwType.TYPE_DEVICE_DMS)
                {
                    path    = mPathInfo.getLastLevelPath() + stru.GetTitleAt+"/";
                }
                else
                {
                    path    = stru.GetDataAt;
                }

                mPathInfo.addLevelInfo(path);

                Log.d(TAG,"mPathInfo.getLastLevelPath() is :"+mPathInfo.getLastLevelPath());
                if (mDataProvider != null) {
                    mDataProvider.StopQuery();
                }
                getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
                setTopMediaPathInfoTextView();
                    /*
                    if (mDataProvider.GetSize()>0)
                    {
            		    mGenericContentViewFragment.RefreshAbstractView(0);
                    }
                    */
                mGenericContentViewFragment.RefreshAbstractView(0);
                mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
            }
            break;
            case FileFilterType.DEVICE_FILE_PHOTO_DATE:
            {
                mPathInfo.addLevelInfo(stru.GetDataAt);
                getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_DATE_TAKEN_ASC);
                    /*
                    if (mDataProvider.GetSize()>0){
					    mGenericContentViewFragment.RefreshAbstractView(0);
                    }
                    */
                mGenericContentViewFragment.RefreshAbstractView(0);
                mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
            }
            break;
        }
    }

    public void onItemClicked(int position) {
        if (mBrowserType == MbwType.TYPE_DEVICE_DMS
                || mBrowserType == MbwType.TYPE_DEVICE_SMB) {
            if (mBackTask != null) {
                log("a BackTask exists");
                mBackTask.cancel(true);
                mBackTask = null;
            }
            mBackTask = new BackgroundTask();
            mBackTask.execute(position);
        } else {
            onItemClickedPreAction();
            DataProviderStruct stru = onItemClickedGetModel(position);
            onItemClickedPostAction(stru);
            mGenericContentViewFragment.mStopLoadImage = false;
        }
    }

    private boolean PlaylistToFile(String FileName, ArrayList<String> playlist)
    {
        try {
            //Delete old file and create new now 
            File file = new File(FileName);
            if (file.exists())
                file.delete();
            file.createNewFile();
            file.setReadable(true,false);

            //Add list to file
            String newline = "\r\n";
            int size = playlist.size();
            FileOutputStream fout = new FileOutputStream(file); 
            OutputStreamWriter outputStreamWriter = new OutputStreamWriter(fout);
            for (int i = 0; i<size; i++)
            {
                outputStreamWriter.write(playlist.get(i));
                outputStreamWriter.write(newline);
            }
            outputStreamWriter.flush();
            outputStreamWriter.close();
            fout.flush();
            fout.close();
        }
        catch (IOException e){
           Log.e(TAG,"PlaylistToFile Got exception: "+e.toString());
           return false;
        }
        return true;
    }
    private class OptionListViewItemClickListener implements OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id)
	   	{	
        	Log.d(TAG,"Option menu "+mPop.getItemof(position)+" is clicked");
			if (!Child_OptionHandle())//If it's not taken by child.
			{
                if(mPop.getItemof(position).equals("mb_select_dir")) {
                    mPop.Hide();
                    Intent intent = new Intent();
                    log("mPathInfo.getLastLevelPath()="+mPathInfo.getLastLevelPath());
                    intent.putExtra("SelectPath",mPathInfo.getLastLevelPath());
                    setResult(RESULT_OK, intent);
                    finish();
                } else if(mPop.getItemof(position).equals("mb_select_file")){
                    mPop.Hide();
                    Intent intent = new Intent();
                    String file = "";
                    if(mDataProvider!=null) {
                        file = mDataProvider.GetDataAt(mPathInfo.getLastLevelFocus());
                        log("mb_select_file file="+file);
                    }
                    intent.putExtra("SelectPath",file);
                    setResult(RESULT_OK, intent);
                    finish();
                }
                else if(mPop.getItemof(position).equals("mb_thumbnailview"))
				{
				    mPop.Hide();
                    if (Child_ViewType()!= VIEW_TYPE_THUMBNAIL)
                    {
                        NewDB();
                        mDB.UpdateChange(mBrowserType,DataBaseHelper.VIEW_TYPE,DataBaseHelper.THUMBNAIL);

				        ComponentName componetName = new ComponentName("com.rtk.mediabrowser",
	    		                    "com.rtk.mediabrowser.ThumbnailViewActivity");
				        Intent intent = new Intent();
				        Bundle bundle = new Bundle();
				        bundle.putInt("browserType", mBrowserType);
				        bundle.putInt("MediaType", mMediaType);
                                        bundle.putInt("CurPosition",position);
                        if (mBrowserType == MbwType.TYPE_DEVICE_ALL)
                        {
                            bundle.putBoolean("DisableDLAN",mDisableDLNA);
                            bundle.putBoolean("DisableSAMBA",mDisableSAMBA);
                        }
				        intent.putExtras(bundle);
				        intent.putExtra("PathInfo",mPathInfo);
				        intent.setComponent(componetName);
				        //intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK|Intent.FLAG_ACTIVITY_CLEAR_TASK);
				        //intent.setFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
				        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
				        //intent.setFlags(Intent.FLAG_ACTIVITY_PREVIOUS_IS_TOP);
				        startActivity(intent);
                        overridePendingTransition(0,0);
                    }
				}	
				else if(mPop.getItemof(position).equals("mb_preview"))
				{
					mPop.Hide();
                    if (Child_ViewType()!= VIEW_TYPE_PREVIEW)
                    {
                        NewDB();
                        mDB.UpdateChange(mBrowserType,DataBaseHelper.VIEW_TYPE, DataBaseHelper.PREVIEW);
				        ComponentName componetName = new ComponentName("com.rtk.mediabrowser",
	    		                    "com.rtk.mediabrowser.PreViewActivity");
				        Intent intent = new Intent();
				        Bundle bundle = new Bundle();
				        bundle.putInt("browserType", mBrowserType);
				        bundle.putInt("MediaType", mMediaType);
                                        bundle.putInt("CurPosition",position);
                        if (mBrowserType == MbwType.TYPE_DEVICE_ALL)
                        {
                            bundle.putBoolean("DisableDLAN",mDisableDLNA);
                            bundle.putBoolean("DisableSAMBA",mDisableSAMBA);
                        }
				        intent.putExtras(bundle);
				        intent.putExtra("PathInfo",mPathInfo);
				        intent.setComponent(componetName);
				        //intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK|Intent.FLAG_ACTIVITY_CLEAR_TASK);
				        //intent.setFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
				        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
				        //intent.setFlags(Intent.FLAG_ACTIVITY_PREVIOUS_IS_TOP);
				        startActivity(intent);
                        overridePendingTransition(0,0);
                    }
				}
				else if(mPop.getItemof(position).equals("mb_all_video"))
				{
					mPop.Hide();
                    NewDB();
                    mDB.UpdateChange(mBrowserType,DataBaseHelper.CATEGORY, DataBaseHelper.CATEGORY_ALL);
					mMediaType = FileFilterType.DEVICE_FILE_VIDEO;
					getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
					mGenericContentViewFragment.RefreshAbstractView(0);
    	            mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
				}
				else if(mPop.getItemof(position).equals("mb_dvd_bd"))
				{
					mPop.Hide();
                    NewDB();
                    mDB.UpdateChange(mBrowserType,DataBaseHelper.CATEGORY, DataBaseHelper.CATEGORY_BD);
					mMediaType = FileFilterType.DEVICE_FILE_VIDEO_ISO_ONLY;
					getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
					mGenericContentViewFragment.RefreshAbstractView(0);
    	            mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
				}
				else if(mPop.getItemof(position).equals("mb_date_view"))
				{
					mPop.Hide();
                    NewDB();
                    mDB.UpdateChange(mBrowserType,DataBaseHelper.CATEGORY, DataBaseHelper.CATEGORY_DATE);
					if(mPathInfo.getLastLevel()>0)
						mPathInfo.backToLastLevel();
					mMediaType = FileFilterType.DEVICE_FILE_PHOTO_DATE;
					getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_DAY_TAKEN_ASC);
					mGenericContentViewFragment.RefreshAbstractView(0);
    	            mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
				}
				else if(mPop.getItemof(position).equals("mb_all_photo"))
				{
					mPop.Hide();
                    NewDB();
                    mDB.UpdateChange(mBrowserType,DataBaseHelper.CATEGORY, DataBaseHelper.CATEGORY_ALL);
					if(mPathInfo.getLastLevel()>0)
						mPathInfo.backToLastLevel();
					mMediaType = FileFilterType.DEVICE_FILE_PHOTO;
					getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
					mGenericContentViewFragment.RefreshAbstractView(0);
    	            mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
				}
				else if(mPop.getItemof(position).equals("mb_sort_by_name_a_z"))
				{
					mPop.Hide();
					getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
					mGenericContentViewFragment.RefreshAbstractView(0);
    	            mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
				}
				else if(mPop.getItemof(position).equals("mb_sort_by_name_z_a"))
				{
					mPop.Hide();
					getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_DESC);
					mGenericContentViewFragment.RefreshAbstractView(0);
    	            mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
				}
				else if(mPop.getItemof(position).equals("mb_recently_play"))
				{
					mPop.Hide();
					getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_RECENTLY_PLAY_DESC);
					mGenericContentViewFragment.RefreshAbstractView(0);
    	            mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
				}
				else if(mPop.getItemof(position).equals("mb_recently_add_new_old"))
				{
					mPop.Hide();
					getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_DATE_MODIFIED_DESC);
					mGenericContentViewFragment.RefreshAbstractView(0);
    	            mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
				}
				else if(mPop.getItemof(position).equals("mb_recently_add_old_new"))
				{
					mPop.Hide();
					getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_DATE_MODIFIED_ASC);
					mGenericContentViewFragment.RefreshAbstractView(0);
    	            mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
				}
                else if(mPop.getItemof(position).equals("mb_use_rt_player"))
                {
                    //save setting
                    boolean b = UseRtMediaPlayer.getInstance(AbstractFBViewActivity.this).switchUseRtPlayer();
                    //refresh item
                    if(mPop!=null) {
                        mPop.refreshItem(position, b);
                    }
                    log("setUseRtPlayer " + b);
                }
			}
        }
    }


	private class OptionMenuKeyListener implements View.OnKeyListener
	{
		@Override
		public boolean onKey(View v, int keyCode, KeyEvent event) {
        if (keyCode ==  KeyEvent.KEYCODE_MENU&& 
                event.getAction() == KeyEvent.ACTION_DOWN) {
			mPop.Hide();
            return true;
        }
        else if (isMediaKey(keyCode)) {
            return true;
        }
        return false;
    }

    private boolean isMediaKey(int keyCode) {
        return keyCode == KeyEvent.KEYCODE_HEADSETHOOK
                || keyCode == KeyEvent.KEYCODE_MEDIA_PREVIOUS
                || keyCode == KeyEvent.KEYCODE_MEDIA_NEXT
                || keyCode == KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE
                || keyCode == KeyEvent.KEYCODE_MEDIA_PLAY
                || keyCode == KeyEvent.KEYCODE_MEDIA_PAUSE
                || keyCode == KeyEvent.KEYCODE_MEDIA_STOP;
    }

	}
	private void NewOptionMenu() {
        View ContentView = getLayoutInflater().inflate(R.layout.optionmenu,null);
        Display display = getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int w = getResources().getDimensionPixelSize(R.dimen.RTK_option_w);
        int h = getResources().getDimensionPixelSize(R.dimen.RTK_option_h);
//        int x = getResources().getDimensionPixelSize(R.dimen.RTK_option_x);
//        int y = getResources().getDimensionPixelSize(R.dimen.RTK_option_y);
        final int x = screenSize.x-w;
        final int y = (screenSize.y/2)-(h/2);
        if(mBrowserType == MbwType.TYPE_DB_PHOTO)
        {
            mPop = new RTKOptionMenu(x,y,w,h, getResources().getXml(R.xml.photodboption), ContentView);
        }
        else if(mBrowserType == MbwType.TYPE_DB_VIDEO)
        {
            Log.d(TAG,"NewOptionMenu VIDEO");
            mPop = new RTKOptionMenu(x,y,w,h, getResources().getXml(R.xml.videodboption), ContentView);
        }
        else if(mBrowserType == MbwType.TYPE_DB_AUDIO)
        {
            mPop = new RTKOptionMenu(x,y,w,h, getResources().getXml(R.xml.audiodboption), ContentView);
        }
        else
        {
            if(mIsGetContentMode)
                mPop = new RTKOptionMenu(x,y,w,h, getResources().getXml(R.xml.pick_file_option), ContentView);
            else
                mPop = new RTKOptionMenu(x,y,w,h, getResources().getXml(R.xml.allfileoption), ContentView);
        }
        //Set default focus(optional)

        //setItemClickListener
        mPop.setItemClickListener(new OptionListViewItemClickListener());
        mPop.setOnKeyListener(new OptionMenuKeyListener());
	}
        
       private class LoginKeyListener implements View.OnKeyListener
        {
            @Override
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                if (((keyCode ==  KeyEvent.KEYCODE_DPAD_CENTER) || (keyCode ==  KeyEvent.KEYCODE_ENTER))&&
                    event.getAction() == KeyEvent.ACTION_DOWN) {
		            String user = m_LoginUserInput.getText().toString();
                    String password = m_LoginPasswdInput.getText().toString();
                    mIsLogined = true;
                    SmbUserMgr smbUserMgr = SmbUserMgr.getInstance(AbstractFBViewActivity.this);
                    if(mLogin.getSaveLoginInfo()) {
                        smbUserMgr.saveUserInfo(SmbBrowser.getSmbPath(), user, password);
                    } else {
                        String path = SmbBrowser.getSmbPath();
                        if(!user.endsWith(Const.GUEST) && path.startsWith("smb://")) {
                            smbUserMgr.keepUserInfo(path, user, password);
                        }
                    }
                    mLogin.Hide();
                    getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
                    mGenericContentViewFragment.RefreshAbstractView(0);
                    mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.    
                    return true;
                }
            return false;
            }
        }


   /*	private class LoginClickListener implements OnClickListener
	{
		@Override
		public void onClick(View v)
		{	
			View m_Popview = mLogin.getContentView();
			EditText m_LoginUserInput = (EditText)m_Popview.findViewById(R.id.LoginUserInput);
 			EditText m_LoginPasswdInput = (EditText)m_Popview.findViewById(R.id.LoginPasswdInput);
			switch(v.getId())
			{
 				case R.id.LoginPasswdInput:
					mLoginUser = m_LoginUserInput.getText().toString();
					mLoginPasswd= m_LoginPasswdInput.getText().toString();
					mIsLogined = true;
                    if(mLogin.getSaveLoginInfo()) {
                        SmbUserMgr smbUserMgr = SmbUserMgr.getInstance(AbstractFBViewActivity.this);
                        smbUserMgr.saveUserInfo(SmbBrowser.getSmbPath(), mLoginUser, mLoginPasswd);
                    }
					mLogin.Hide();
                    getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
		            mGenericContentViewFragment.RefreshAbstractView(0);
    	            mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
					break;
				default: 
					break;
			}
		}
	}*/
	private void NewLoginMenu() 
	{
		View ContentView = getLayoutInflater().inflate(R.layout.loginmenu,null); 
		if(mLogin == null)
		{
            int x = getResources().getDimensionPixelSize(R.dimen.RTK_smblogin_x);
            int y = getResources().getDimensionPixelSize(R.dimen.RTK_smblogin_y);
            int w = getResources().getDimensionPixelSize(R.dimen.RTK_smblogin_w);
            int h = getResources().getDimensionPixelSize(R.dimen.RTK_smblogin_h);

			mLogin = new RTKLoginMenu(x,y,w,h, ContentView);
                        View m_Popview = mLogin.getContentView();
                        m_LoginUserInput = (EditText)m_Popview.findViewById(R.id.LoginUserInput);
                        m_LoginPasswdInput = (EditText)m_Popview.findViewById(R.id.LoginPasswdInput);
		      // 	mLogin.setClickListener(new LoginClickListener());
                        m_LoginPasswdInput.setOnKeyListener(new LoginKeyListener());
		}
        mLogin.resetFields();
		mLogin.Show();
	}
	
    private boolean onBackClicked()
    {
        boolean retval =false;
        mGenericContentViewFragment.stopGridViewDecode();
         
        if(mDataProvider == null)
        {
        	Log.d(TAG,"onBackClicked:mDataProvider==null");
        }	
        
        Log.d(TAG,"onBackClicked: Path Level = "+ mPathInfo.getLastLevel());
        
        /*
        if( mDataProvider != null &&
            mPathInfo.getLastLevelFocus() >=0 && 
            mPathInfo.getLastLevelFocus() <=mDataProvider.GetSize())*/
        if( mDataProvider != null)
        {
            if(mPathInfo.getLastLevel() >0)
            {   
                mPathInfo.backToLastLevel();
		        mDataProvider.StopQuery();
				if (mMediaType == FileFilterType.DEVICE_FILE_PHOTO_DATE)
				    getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_DAY_TAKEN_ASC);
                else
                    getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
                int i = mPathInfo.getLastLevelFocus();
			    mGenericContentViewFragment.RefreshAbstractView(0);
                mPathInfo.setLastLevelFocus(i);
                setTopMediaPathInfoTextView();
                /*
                if (mPathInfo.getLastLevelFocus()<mDataProvider.GetSize())
                {
                    mGenericContentViewFragment.RefreshAbstractView(mPathInfo.getLastLevelFocus());
                }
                */
                retval = true;
            }
        }
		if (!retval)
		{
			if(mBrowserType == MbwType.TYPE_DEVICE_ALL
                    ||mBrowserType == MbwType.TYPE_DB_PHOTO
                    ||mBrowserType == MbwType.TYPE_DB_AUDIO
                    ||mBrowserType == MbwType.TYPE_DB_VIDEO
                    ||mBrowserType == MbwType.TYPE_DEVICE_QUERY)
			{
                BackToMediaBrowserHome();
                retval = true;
			}
			else //MbwType.TYPE_DEVICE_USB, MbwType.TYPE_DEVICE_DMS and MbwType.TYPE_DEVICE_SMB
			{
			    if(mDataProvider!=null)
		            mDataProvider.StopQuery();
                mBrowserType = MbwType.TYPE_DEVICE_ALL;
                mPathInfo.cleanLevelInfo(); 
       		    mPathInfo.addLevelInfo("NONE");
                getDataProvider(mPathInfo.getLastLevelPath(), DBFileDataProvider.SortMode.SORT_BY_NAME_ASC);
			    mGenericContentViewFragment.RefreshAbstractView(0);
    	        mPathInfo.setLastLevelFocus(0);//It's workaround for listview, we ask listview select 0, but it will not select to 0 if list count is zero. However it's work in GridView.
	            mSelectedPosition = -1;
				retval = true;
			}
		}
        return retval;
    }
    private boolean onPlayClicked()
    {
        boolean retval =false;
		if ((mBrowserType != MbwType.TYPE_DEVICE_DMS)&&
            (FileFilterType.DEVICE_FILE_DIR == mDataProvider.GetFileTypeAt(mPathInfo.getLastLevelFocus())))
		{
			String path = mDataProvider.GetDataAt(mPathInfo.getLastLevelFocus());
            if(path==null) return false;
            BDParser BD = new BDParser(path);
            if (BD.getType() != BDParser.NOT_BD_DVD)
            {
                Intent intent = new Intent();
                ComponentName comp = new ComponentName("com.android.gallery3d", "com.android.gallery3d.app.MovieActivity");
                intent.setComponent(comp);
                intent.setAction(Intent.ACTION_VIEW);
                String playEntry = BD.getPlayEntry();
                Uri encodePlayEntry = Util.getUri(this, playEntry);
                log("onPlayClicked encodePlayEntry="+encodePlayEntry);
                intent.setData(encodePlayEntry);
                intent.putExtra(MEDIA_BROWSER_USE_RT_MEDIA_PLAYER, true);
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK
                        | Intent.FLAG_ACTIVITY_CLEAR_TASK
                        | Intent.FLAG_ACTIVITY_NO_ANIMATION);

                //collect playlist information
                ArrayList<String> playlist = mDataProvider.GetPlayList(FileFilterType.DEVICE_FILE_BD_FOLDER);
                ArrayList<String> playlist_title = mDataProvider.GetNameList(FileFilterType.DEVICE_FILE_BD_FOLDER);
//                log("onPlayClicked playlist=" + Arrays.toString(playlist.toArray()));
//                log("onPlayClicked playlist_title=" + Arrays.toString(playlist_title.toArray()));
                if (playlist == null) {
                    new AlertDialog.Builder(this)
                        .setTitle("Warning!")
                        .setMessage("BD DVD query is not done. Please try again later.")
                        .setPositiveButton("YES", null)
                        .show();
                    return true;
                }
                int positionInPlaylist = -1;
                int size = playlist.size();
                for (int i = 0; i < size; i++) {
                    if (encodePlayEntry.toString().equals(playlist.get(i))) {
                        positionInPlaylist = i;
                        break;
                    }
                }
                Log.v(TAG, "size of playlist=" + size + " position=" + positionInPlaylist);
                if ((size > 40) || (mBrowserType == MbwType.TYPE_DEVICE_DMS) || (mBrowserType == MbwType.TYPE_DEVICE_SMB)) {
                    PlaylistToFile(PLAYLIST_FILE, playlist);
                    intent.putExtra("PlayListAddress", PLAYLIST_FILE);
                    PlaylistToFile(PLAYLIST_FILE_TITLE, playlist_title);
                    intent.putExtra("PlayListTitleAddress", PLAYLIST_FILE_TITLE);
                } else {
                    String[] playlistArray = new String[playlist.size()];
                    intent.putExtra("PlayListAddress", "Intent");
                    intent.putExtra("playlist", playlist.toArray(playlistArray));
                    String[] playlistTitleArray = new String[playlist_title.size()];
                    intent.putExtra("PlayListTitleAddress", "Intent");
                    intent.putExtra("playlistTitle", playlist_title.toArray(playlistTitleArray));
                }
                intent.putExtra("index-hint", positionInPlaylist);

                startActivity(intent);
                retval = true;
            } 	
		}
        if (!retval)
        {
           onItemClicked(mPathInfo.getLastLevelFocus());
           retval = true; 
        }
        
        return retval;
    }

    public boolean onKeyClicked(View view, int keyCode, KeyEvent event,int position,int iconNum,int firstVisibleItem,int lastVisibleItem)
    {
        /*    	
    	switch(keyCode)
    	{
    	
    	case KeyEvent.KEYCODE_DPAD_RIGHT:
    	{
			if(m_ViewType == GenericContentViewFragment.GRID_VIEW)
			{
				try{
					if (mLock.tryLock(100,TimeUnit.MILLISECONDS))
					{
    					mGenericContentViewFragment.directionControl.RightOperation();
					}
				}
				catch (InterruptedException e) {
					e.printStackTrace();
				}
				finally{
					if (mLock.isHeldByCurrentThread()){mLock.unlock();}
				}
			}
			return true;
    	}
    	case KeyEvent.KEYCODE_DPAD_LEFT:
    	{
			if(m_ViewType == GenericContentViewFragment.GRID_VIEW)
			{
				try{
					if (mLock.tryLock(100,TimeUnit.MILLISECONDS))
					{
    					mGenericContentViewFragment.directionControl.LeftOperation();
					}
				}
				catch (InterruptedException e) {
					e.printStackTrace();
				}
				finally{
					if (mLock.isHeldByCurrentThread()){mLock.unlock();}
				}
			}
    		return true;
    	}
    	case KeyEvent.KEYCODE_DPAD_UP:
    	{
			try{
				if (mLock.tryLock(100,TimeUnit.MILLISECONDS))
				{
    				mGenericContentViewFragment.directionControl.UpOperation();
				}
			}
			catch (InterruptedException e) {
				e.printStackTrace();
			}
			finally{
				if (mLock.isHeldByCurrentThread()){mLock.unlock();}
			}
    		return true;
    	}
    	case KeyEvent.KEYCODE_DPAD_DOWN:
    	{
			try{
				if (mLock.tryLock(100,TimeUnit.MILLISECONDS))
				{
    				mGenericContentViewFragment.directionControl.DownOperation();
				}
			}
			catch (InterruptedException e) {
				e.printStackTrace();
			}
			finally{
				if (mLock.isHeldByCurrentThread()){mLock.unlock();}
			}
    		return true;
    	}
    	case KeyEvent.KEYCODE_PAGE_DOWN:
    	{
    		mGenericContentViewFragment.directionControl.PageDownOperation();
    		return true; 
    	}
    	case KeyEvent.KEYCODE_PAGE_UP:
    	{
    		mGenericContentViewFragment.directionControl.PageUpOperation();
    		return true;    		
    	}
    	default:
    		return false;
    	}
    */
     return false;
    }
    
	public int GetNumOfColumns()
	{
		return m_NumOfColumns;
	}

	public int GetNumOfRows()
	{
		return m_NumOfRows;
	}
	public int GetViewType()
	{
		Log.d(TAG, "GetViewType()=" + m_ViewType);
		return m_ViewType;
	}
    public void LoadingBusy(int state)
    {
        Message msg = new Message();
        msg.what=state;
       loading_icon_system_handle.sendMessage(msg); 
    }
	private void UpdateItemView()
	{
        if (mSelectedPosition>=0)
        {
	        String Name = mDataProvider.GetTitleAt(mSelectedPosition);
	        int Type = mDataProvider.GetFileTypeAt(mSelectedPosition);
	        Log.d(TAG, "UpdateItemView() Org idx:"+mSelectedPosition+" Org name:"+Name+" Type:"+Type);
	        int NewPosition = -1;
	        //Don't forget get all old info which you need before Update data. 
	        mDataProvider.UpdateData(GotNewItem);//Update new data to list, and ask query again.
	        GotNewItem =0;
	        //find out the selected index of UPDATED data list. 
	        //check same index 
	        Log.d(TAG,"UpdateItemView() 0");
	        if (mSelectedPosition < mDataProvider.GetSize())
	        {	
	        	Log.d(TAG,"UpdateItemView() 1");
	        	if(Name.equals(mDataProvider.GetTitleAt(mSelectedPosition)) && Type == mDataProvider.GetFileTypeAt(mSelectedPosition))
	        	{
	        		Log.d(TAG,"UpdateItemView() find it in same index");
	        		NewPosition = mSelectedPosition;
	        	}
	        }
	        if (NewPosition ==-1)
	        {
	        	//check from the same category 
	        	int StrPointer = FindStartPosition(Type);		
	        	int EndPointer = FindEndPosition(Type);	
	        	int i;
                if (StrPointer >=0)
                {
	                for (i=StrPointer;i<=EndPointer;i++)
	                {
	                	if (Name.equals(mDataProvider.GetTitleAt(i)))
	                	{
	                		NewPosition = i;
	                		Log.d(TAG,"UpdateItemView() find it in index:"+i);
	                	}
	                }
                    //Can not find the old item in new list, so just use first item of this category.
	                if (NewPosition == -1)
	                {
	                	//Could not find it in new list
	                	NewPosition = StrPointer;
	                	Log.d(TAG,"UpdateItemView() it's not in the list, use default");
	                }
                }
                //Can not find any item of the category, so just jump to first item. 
                if (NewPosition == -1)
                {
                    if (mDataProvider.GetSize()>0)
                    {
                        NewPosition = 0;
                    }
                }
	        }
	        mGenericContentViewFragment.directionControl.DataRefresh(NewPosition);
        }
        else// get in via mouse, and it will not have any selected item. So don't need to select any 
        {
		    mDataProvider.UpdateData(GotNewItem);//Update new data to list, and ask query again.
		    mGenericContentViewFragment.directionControl.DataRefresh(-1);
        }
	}
	private int FindStartPosition(int Type)
	{
		int Position = -1;
		if (Type ==	FileFilterType.DEVICE_FILE_DEVICE && mDataProvider.GetSize(FileFilterType.DEVICE_FILE_DEVICE)>0)
		{
			Position= 0;
		}
		else if (Type == FileFilterType.DEVICE_FILE_DLNA_DEVICE && mDataProvider.GetSize(FileFilterType.DEVICE_FILE_DLNA_DEVICE)>0)
		{
			Position= mDataProvider.GetSize(FileFilterType.DEVICE_FILE_DEVICE);
		}
		else if (Type == FileFilterType.DEVICE_FILE_SMB_DEVICE && mDataProvider.GetSize(FileFilterType.DEVICE_FILE_SMB_DEVICE)>0)
		{
			Position= (mDataProvider.GetSize(FileFilterType.DEVICE_FILE_DEVICE)+mDataProvider.GetSize(FileFilterType.DEVICE_FILE_DLNA_DEVICE));
		}
		return Position;
	}
	private int FindEndPosition(int Type)
	{
		int Position =-1;
		if (Type ==	FileFilterType.DEVICE_FILE_DEVICE && mDataProvider.GetSize(FileFilterType.DEVICE_FILE_DEVICE)>0)
		{
			Position= mDataProvider.GetSize(FileFilterType.DEVICE_FILE_DEVICE)-1;
		}
		else if (Type == FileFilterType.DEVICE_FILE_DLNA_DEVICE && mDataProvider.GetSize(FileFilterType.DEVICE_FILE_DLNA_DEVICE)>0)
		{
			Position= (mDataProvider.GetSize(FileFilterType.DEVICE_FILE_DEVICE)+mDataProvider.GetSize(FileFilterType.DEVICE_FILE_DLNA_DEVICE)-1);
		}
		else if (Type == FileFilterType.DEVICE_FILE_SMB_DEVICE && mDataProvider.GetSize(FileFilterType.DEVICE_FILE_SMB_DEVICE)>0)
		{
			Position= mDataProvider.GetSize()-1;
		}
		return Position;
	}

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        log("onActivityResult requestCode["+requestCode+"] resultCode["+resultCode+"]");
        switch(requestCode){
            case MOVIE_REQUEST_CODE:
                if(resultCode==Activity.RESULT_OK){
                    if(!hasNetwork()){
                        log("mBrowserType="+mBrowserType);
                        if(mBrowserType == MbwType.TYPE_DEVICE_DMS) {
                            if(DLNABrowser.isLoaded())
                                DLNABrowser.GetInstance().StopPreviousWork();
                            BackToMediaBrowserHome();
                            break;
                        }
                    }

                    if(mGenericContentViewFragment!=null){
                        String videoPath = data.getStringExtra("StopPosition");
                        //Toast.makeText(this,"Movie Result video:"+videoPath,Toast.LENGTH_SHORT).show();
                        int index = mDataProvider.GetItemIndex(videoPath);
                        if(index!=-1){
                            Instrumentation ins = new Instrumentation();
                            ins.setInTouchMode(false);
                            mGenericContentViewFragment.RefreshAbstractView(index);
                            //Toast.makeText(this,"Movie Result index:"+index,Toast.LENGTH_SHORT).show();
                        }
                    }
                }
                break;
            default:
                break;
        }
    }

    private void log(String s) {
        if(DEBUG)
            Log.d(TAG, s);
    }
}
