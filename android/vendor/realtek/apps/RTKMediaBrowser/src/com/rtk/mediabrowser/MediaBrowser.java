package com.rtk.mediabrowser;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.*;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.res.Configuration;
import android.database.Cursor;
import android.net.ConnectivityManager;
import android.net.EthernetManager;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.SystemProperties;
import android.provider.BaseColumns;
import android.provider.MediaStore;
import android.speech.RecognizerIntent;
import android.text.TextUtils;
import android.util.Log;
import android.view.*;
import android.widget.*;
import android.widget.AdapterView.OnItemClickListener;
import com.realtek.DLNA_DMP_1p5.DLNABrowser;
import com.realtek.DataProvider.FileFilterType;
import com.realtek.Utils.MarqueeTextView;
import com.realtek.Utils.MbwType;
import com.realtek.Utils.UseRtMediaPlayer;
import com.realtek.bitmapfun.util.ImageFetcher;
import com.realtek.rtksmb.SmbBrowser;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static com.rtk.mediabrowser.AbstractFBViewActivity.MEDIA_BROWSER_USE_RT_MEDIA_PLAYER;

public class MediaBrowser extends Activity 
{
	private static final boolean DEBUG = true;
	//public static TvManager mTV;
	private String TAG="MediaBrowser";
	private GridView mGridView=null;
	private GridViewAdapter mGridViewAdapter = null;
	
	private static int GetOutMB = 0;	
	private static int LastFocus = 0;	
	private boolean USBPlugIn = false;
	
	
	  //Top mediaInfo
    private TextView  mBrowserTypeTextView    =null;
    private String    mBrowserTypeString      = null;
   
    private DataBaseHelper mDB = null;
	private boolean  mDisableDLNA = false;
	private boolean  mDisableSAMBA = false;
	private static final int REQUEST_PICK_FOLDER = 123;
	private static final int SPEECH_REQUEST_CODE = 124;
	private ImageView mVoiceBtn;

	private void NewDB()
    {
        if (mDB==null){ mDB = new DataBaseHelper(this);} 
    }

	//The BroadcastReceiver that listens for network broadcasts
	private final BroadcastReceiver myReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			final String action = intent.getAction();
			if(action.equals(ConnectivityManager.CONNECTIVITY_ACTION) ||
					action.equals(ConnectivityManager.INET_CONDITION_ACTION) ||
					action.equals(EthernetManager.NETWORK_STATE_CHANGED_ACTION)) {
				log("network changed connection="+Util.hasNetwork(MediaBrowser.this));
				tryReinitDLNA();
			}
		}
	};

	private void tryReinitDLNA() {
		if (!mDisableDLNA) {
			if (DLNABrowser.isLoaded()){
				DLNABrowser.GetInstance().StopPreviousWork();
				DLNABrowser.DelInstance();
			}
            DLNABrowser.GetInstance();
        }
	}

	private void log(String s) {
		if(DEBUG)
			Log.d(TAG, s);
	}

	@Override
    public void onCreate(Bundle savedInstanceState) 
	{
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        Log.d(TAG,"onCreate............");
		Util.setAppCtx(this.getApplicationContext());
        Intent intent= getIntent();
        setContentView(R.layout.activity_media_browser);
        if (SystemProperties.get("persist.rtk.mb.dlna.disable","false").equals("true"))
            mDisableDLNA = true;
        if (SystemProperties.get("persist.rtk.mb.samba.disable","false").equals("true"))
            mDisableSAMBA = true;
        Log.d(TAG, "DisableDLAN="+mDisableDLNA+", DisableSAMBA="+mDisableSAMBA);
        if (!mDisableDLNA){
            Thread thread = new Thread( new Runnable(){
                    public void run(){
                        DLNABrowser.GetInstance();//Wen: Pls don't remove this line, it's used to init DLNA SSDP ahead to query DMS.
                    }
                });
            thread.start();
        }
		ImageFetcher.getInstance(this);

        mGridView = (GridView) findViewById(R.id.mbw_menu_gridview);
        mGridView.setOnItemClickListener(new GridViewItemClickListener());
        mGridViewAdapter = new GridViewAdapter();
        mGridView.setAdapter(mGridViewAdapter);
        mGridView.setFocusable(true);
        mGridView.requestFocus();
        mGridView.setSelection(LastFocus);
        mGridView.setSelected(true);
        mGridView.setClickable(true);

        mVoiceBtn = (ImageView) findViewById(R.id.voice_action);
        if(mVoiceBtn!=null){
			final Intent voiceSearchIntent = new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);
			intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL,
					RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);

			PackageManager manager = this.getPackageManager();
			List<ResolveInfo> infos = manager.queryIntentActivities(voiceSearchIntent, 0);
			if (infos.size() > 0) {
				mVoiceBtn.setOnClickListener(new View.OnClickListener() {
					@Override
					public void onClick(View view) {
						// Start the activity, the intent will be populated with the speech text
						startActivityForResult(voiceSearchIntent, SPEECH_REQUEST_CODE);
					}
				});
				mVoiceBtn.setVisibility(View.VISIBLE);
			} else {
				log("No Activity found to handle Intent { act=android.speech.action.RECOGNIZE_SPEECH (has extras) }");
				mVoiceBtn.setVisibility(View.INVISIBLE);
			}
		}

        Log.d(TAG,"setGridView focusIndex:"+mGridView.getSelectedItemPosition());

		IntentFilter filter = new IntentFilter();
		filter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
		filter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
		//Ethernet
		filter.addAction(EthernetManager.NETWORK_STATE_CHANGED_ACTION);
		filter.addAction(EthernetManager.ETHERNET_STATE_CHANGED_ACTION);
		//Network
		filter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
		filter.addAction(ConnectivityManager.INET_CONDITION_ACTION);
		registerReceiver(myReceiver, filter);

		if(intent!=null){
			String action = intent.getAction();
			if(Intent.ACTION_GET_CONTENT.equals(action)){
				log("ACTION_GET_CONTENT");
				goBrowseAllFiles(intent);
			} else if(Intent.ACTION_VIEW.equals(action)){
				log("ACTION_VIEW");
				final Intent intentPlayer = new Intent();
				Uri intentUri = intent.getData();
				log("intent.getData()="+intentUri);
				Uri uri = Util.getUri(this, intentUri.toString());
				ContentResolver cr = this.getContentResolver();
				String mime = cr.getType(uri);
				if(mime.startsWith("audio")){
					ComponentName comp = new ComponentName("com.android.music", "com.android.music.MediaPlaybackActivity");
					intentPlayer.setComponent(comp);
					Util.grantReadPermission(this, intentPlayer, uri);
					intentPlayer.setAction(Intent.ACTION_VIEW);
				} else if (mime.startsWith("video")){
					ComponentName comp = new ComponentName("com.android.gallery3d", "com.android.gallery3d.app.MovieActivity");
					intentPlayer.setComponent(comp);
					Util.grantReadPermission(this, intentPlayer, uri);
					intentPlayer.putExtra("SourceFrom", "Local");
					intentPlayer.putExtra(MEDIA_BROWSER_USE_RT_MEDIA_PLAYER, UseRtMediaPlayer.getInstance(this).getUseRtPlayer());
					intentPlayer.setAction(Intent.ACTION_VIEW);
					intentPlayer.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK
							| Intent.FLAG_ACTIVITY_NO_ANIMATION);
				} else if (mime.startsWith("image")){
					ComponentName comp = new ComponentName("com.android.gallery3d", "com.android.gallery3d.app.GalleryActivity");
					intentPlayer.setComponent(comp);
					Util.grantReadPermission(this, intentPlayer, uri);
					intentPlayer.setAction(Intent.ACTION_VIEW);
					intent.setDataAndType(uri, "image/*");
					intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK
							| Intent.FLAG_ACTIVITY_CLEAR_TASK
							| Intent.FLAG_ACTIVITY_NO_ANIMATION);
				}
				intentPlayer.setData(uri);
				startActivity(intentPlayer);
				finish();
			}
		}
    }
    @Override 
    public void onStop()
    {
        super.onStop();
    }
	
    @Override
    public boolean dispatchKeyEvent(KeyEvent event)
    {
        boolean retval = false;
    	if( event.getKeyCode() == KeyEvent.KEYCODE_MENU && event.getAction() == KeyEvent.ACTION_DOWN)
        {
            retval = true;//bug 43947. Just take it and do not thing.
        } else if(event.getKeyCode()==KeyEvent.KEYCODE_SEARCH){
			if(event.getAction() == KeyEvent.ACTION_UP) {
				if (mVoiceBtn != null) {
					mVoiceBtn.callOnClick();
				}
			}
    		retval = true;
		}
        if (retval ==false)
        {
            retval = super.dispatchKeyEvent(event);
        } 
        return retval;
    }
	
	 private class GridViewItemClickListener implements OnItemClickListener
	 {
		 @Override
		 public void onItemClick(AdapterView<?> parent, View v, int position, long id) 
		 { 
			 /*	 
			 if(mBrowserType == 0)
		     {
				 File file = new File("/storage/media0/");
				 File[] files = file.listFiles(); 
				 int fileSize =files.length;
			 
				 files = null;
				 file=null;
			 
				 if(fileSize == 0)
				 {
					 Toast.makeText(getApplicationContext(), "Please insert external disk!",
             		     Toast.LENGTH_SHORT).show();
					 return;
				 }
		     }	 
			 */
			 LastFocus = position; 
			 if(position == 1)//photo
			 {
                 GetOutMB = 0;
                 NewDB();
                 String oldViewType = mDB.QueryDate(MbwType.TYPE_DB_PHOTO,DataBaseHelper.VIEW_TYPE);
                 ComponentName componetName = null;
                 Intent intent= new Intent();
                 Bundle bundle = new Bundle();
                 if (oldViewType.equals(DataBaseHelper.PREVIEW)){
				    componetName = new ComponentName("com.rtk.mediabrowser", "com.rtk.mediabrowser.PreViewActivity");
                                    bundle.putInt("CurPosition", 1);}
                 else{
				    componetName = new ComponentName("com.rtk.mediabrowser", "com.rtk.mediabrowser.ThumbnailViewActivity");
                                    bundle.putInt("CurPosition", 0);}
	        	    
				// Intent intent= new Intent();
				// Bundle bundle = new Bundle();
	        	 bundle.putInt("browserType", MbwType.TYPE_DB_PHOTO);
	        	 bundle.putInt("MediaType", FileFilterType.DEVICE_FILE_PHOTO);
	        	 intent.putExtras(bundle);
				 intent.setComponent(componetName);
				 //intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
				 //intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK|Intent.FLAG_ACTIVITY_CLEAR_TASK);
				 startActivity(intent);
                 overridePendingTransition(0,0);
			 }
			 else if(position == 3)//AUDIO
			 {
                 GetOutMB = 0;
				 ComponentName componetName = new ComponentName("com.rtk.mediabrowser",
	                        //"com.rtk.mediabrowser.ThumbnailViewActivity");
							"com.rtk.mediabrowser.PreViewActivity");
				 Intent intent= new Intent();
				 Bundle bundle = new Bundle();
	        	 bundle.putInt("browserType", MbwType.TYPE_DB_AUDIO);
	        	 bundle.putInt("MediaType", FileFilterType.DEVICE_FILE_AUDIO);
                         bundle.putInt("CurPosition", 1);
	        	 intent.putExtras(bundle);
				 intent.setComponent(componetName);
				 //intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
				 //intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK|Intent.FLAG_ACTIVITY_CLEAR_TASK);
				 startActivity(intent);
                 overridePendingTransition(0,0);
			 } 
			 else if(position == 0) //VIDEO
			 {
                 GetOutMB = 0;
                 NewDB();
                 String oldViewType = mDB.QueryDate(MbwType.TYPE_DB_VIDEO,DataBaseHelper.VIEW_TYPE);
                 ComponentName componetName = null;
                 Intent intent= new Intent();
                  Bundle bundle = new Bundle();
                 if (oldViewType.equals(DataBaseHelper.PREVIEW)){
				    componetName = new ComponentName("com.rtk.mediabrowser", "com.rtk.mediabrowser.PreViewActivity");
                                    bundle.putInt("CurPosition", 1);}
                 else{
				    componetName = new ComponentName("com.rtk.mediabrowser", "com.rtk.mediabrowser.ThumbnailViewActivity");
                                    bundle.putInt("CurPosition", 0);} 
				// Intent intent= new Intent();
				// Bundle bundle = new Bundle();
	        	 bundle.putInt("browserType", MbwType.TYPE_DB_VIDEO);
	        	 bundle.putInt("MediaType", FileFilterType.DEVICE_FILE_VIDEO);
	        	 intent.putExtras(bundle);
				 intent.setComponent(componetName);
				 //intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
				 //intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK|Intent.FLAG_ACTIVITY_CLEAR_TASK);
				 startActivity(intent);
                 overridePendingTransition(0,0);
			 } 
			 else if(position == 2) //All Dev
			 {
				 goBrowseAllFiles(null);
			 } 
		 }   	
		
	}

	private void goBrowseAllFiles(Intent reqIntent) {
		GetOutMB = 0;
		NewDB();
		String oldViewType = mDB.QueryDate(MbwType.TYPE_DEVICE_ALL, DataBaseHelper.VIEW_TYPE);
		ComponentName componetName = null;
		Intent intent= new Intent();
		Bundle bundle = new Bundle();
		if (oldViewType.equals(DataBaseHelper.PREVIEW)){
           componetName = new ComponentName("com.rtk.mediabrowser", "com.rtk.mediabrowser.PreViewActivity");
                           bundle.putInt("CurPosition", 1);}
        else{
           componetName = new ComponentName("com.rtk.mediabrowser", "com.rtk.mediabrowser.ThumbnailViewActivity");
                           bundle.putInt("CurPosition", 0);}
		// Intent intent= new Intent();
		// Bundle bundle = new Bundle();
		bundle.putInt("browserType", MbwType.TYPE_DEVICE_ALL);
		bundle.putInt("MediaType", FileFilterType.DEVICE_FILE_DIR|FileFilterType.DEVICE_FILE_PHOTO|FileFilterType.DEVICE_FILE_VIDEO|FileFilterType.DEVICE_FILE_AUDIO|FileFilterType.DEVICE_FILE_PLAYLIST);
		bundle.putBoolean("DisableDLAN",mDisableDLNA);
		bundle.putBoolean("DisableSAMBA",mDisableSAMBA);
		intent.putExtras(bundle);
		intent.setComponent(componetName);
		//intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		//intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK|Intent.FLAG_ACTIVITY_CLEAR_TASK);
		if(reqIntent==null)
			startActivity(intent);
		else {
			bundle.putBoolean("DisableDLAN", true);
			bundle.putBoolean("DisableSAMBA", true);
			intent.putExtras(bundle);
			intent.setAction(reqIntent.getAction());
			startActivityForResult(intent, REQUEST_PICK_FOLDER);
		}
		overridePendingTransition(0,0);
	}

	private void goBrowseVoiceSearch(String query) {
		GetOutMB = 0;
		NewDB();
		String oldViewType = mDB.QueryDate(MbwType.TYPE_DEVICE_ALL, DataBaseHelper.VIEW_TYPE);
		ComponentName componetName = null;
		Intent intent= new Intent();
		Bundle bundle = new Bundle();
		if (oldViewType.equals(DataBaseHelper.PREVIEW)){
			componetName = new ComponentName("com.rtk.mediabrowser", "com.rtk.mediabrowser.PreViewActivity");
			bundle.putInt("CurPosition", 1);}
		else{
			componetName = new ComponentName("com.rtk.mediabrowser", "com.rtk.mediabrowser.ThumbnailViewActivity");
			bundle.putInt("CurPosition", 0);}
		// Intent intent= new Intent();
		// Bundle bundle = new Bundle();
		bundle.putInt("browserType", MbwType.TYPE_DEVICE_QUERY);
		bundle.putInt("MediaType", FileFilterType.DEVICE_FILE_VIDEO|FileFilterType.DEVICE_FILE_AUDIO|FileFilterType.DEVICE_FILE_PLAYLIST);
		bundle.putBoolean("DisableDLAN",true);
		bundle.putBoolean("DisableSAMBA",true);
		bundle.putString("queryString", query);
		intent.putExtras(bundle);
		intent.setComponent(componetName);
		//intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		//intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK|Intent.FLAG_ACTIVITY_CLEAR_TASK);
		startActivity(intent);
		overridePendingTransition(0,0);
	}

	private class GridViewAdapter extends BaseAdapter
	 {
		 
		 public GridViewAdapter() 
		 {
		 }
	        
		 @Override
		 public int getCount() 
		 {
			 return 3;
		 }
		 
		 @Override
	     public Object getItem(int arg0) 
		 {
	            // TODO Auto-generated method stub
			 return null;
		 }

		 @Override
		 public long getItemId(int position) 
		 {
			 // TODO Auto-generated method stub
			 return 0;
		 }


		 @Override
		 public View getView(int position, View convertView, ViewGroup parent) 
		 {
			 ImageView imageView = null;
			 MarqueeTextView  textView  = null;
			 View itemView;
	            
			 Log.d(TAG,"position :"+position); 
	            
			 if (convertView == null) 
			 { 
				 // if it's not recycled, initialize some attributes
	             Context context = parent.getContext();
	             LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	             itemView = inflater.inflate(R.layout.item_of_mbw_menu_gridview, null);
	             imageView = (ImageView)(itemView.findViewById(R.id.mbw_menu_grid_img));
	             textView  = (MarqueeTextView)(itemView.findViewById(R.id.mbw_menu_grid_text));
				 textView.setSelectedChangeStyle(R.style.MBWSelectedText,R.style.MBWUnselectedText);
			 	 Log.d(TAG,"convertView is null"); 
	             
	             if(position==0)
	             {
	            	 imageView.setImageResource(R.drawable.selector_mbw_video);
	            	 textView.setText(getString(R.string.mb_video));
	             }
	             else if(position==1)
	             {
	            	 imageView.setImageResource(R.drawable.selector_mbw_photo);
	            	 textView.setText(getString(R.string.mb_photo));
	             }
	             else if(position==3)
	             {
	            	 imageView.setImageResource(R.drawable.selector_mbw_audio);
	            	 textView.setText(getString(R.string.mb_music));
	             }
	             else if(position==2)
	             {
	            	 imageView.setImageResource(R.drawable.selector_mbw_all);
	            	 textView.setText(getString(R.string.mb_allfile));
	             }
			 } 
	         else 
	         {
	        	 itemView = convertView;
	         }
			 
			 return itemView;
		 }
	
	 }
	  
	  @Override
	  public void onResume()
	  {
		  super.onResume();
		  Log.d(TAG, "------------OnResume--------");
		  if (!mDisableDLNA){
			  Thread thread = new Thread( new Runnable(){
				  public void run(){
					  tryReinitDLNA();//Wen: Pls don't remove this line, it's used to init DLNA SSDP ahead to query DMS.
				  }
			  });
			  thread.start();
		  }
	 }
	  
      @Override 
      public void onConfigurationChanged(Configuration newConfig)
      {
          super.onConfigurationChanged(newConfig);
          Log.d(TAG,"Configuration Change :"+newConfig);
          Log.d(TAG,"Configuration Change keyboard: "+newConfig.keyboard);
          Log.d(TAG,"Configuration Change keyboardHidden: "+newConfig.keyboardHidden);

      }
	  @Override
	  public void onPause()
	  {
		  Log.d(TAG,"onPause");
		  super.onPause();
          if (mDB != null)
          {
              mDB.close();
              mDB = null;
          }
	  } 
	  
	  @Override
	  public void onDestroy()
	  {
		  if(myReceiver!=null)
		  	unregisterReceiver(myReceiver);
          mGridView = null;
          mGridViewAdapter = null;
          if (GetOutMB==1)
          {
			SmbBrowser.DelInstance();
			if(DLNABrowser.isLoaded()) {
				DLNABrowser.GetInstance().StopPreviousWork();
				DLNABrowser.DelInstance();
			}
          }
          GetOutMB =1;
		  ImageFetcher.destroy();
		  
		  Log.d(TAG,"onDestroy");
		  super.onDestroy();	
		  Util.DestoryMimeTypes();
	  }
   @Override
   public void onLowMemory()
   {
       super.onLowMemory();
       //System.gc();
      	Log.v(TAG,"onLowMemory");
   }

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if(requestCode==REQUEST_PICK_FOLDER){
			log("onActivityResult REQUEST_PICK_FOLDER");
			if(resultCode==RESULT_OK) {
				Intent intent = new Intent();
				String selectPath = data.getStringExtra("SelectPath");
				log("selectPath="+selectPath);
				intent.putExtra("SelectPath", "file://"+selectPath);
				setResult(RESULT_OK, intent);
			}
			finish();
		} else if(requestCode==SPEECH_REQUEST_CODE){
			if(data==null) {
				return;
			}
			List<String> results = data.getStringArrayListExtra(
					RecognizerIntent.EXTRA_RESULTS);
			if(results==null) return;
			log("voice result="+ Arrays.toString(results.toArray()));
			if(results.size()==1){
				goBrowseVoiceSearch(results.get(0));
				return;
			}
			String[] querys = new String[results.size()];
			results.toArray(querys);
			AlertDialog.Builder b = new AlertDialog.Builder(this);
			b.setTitle("Select your query");
			final String[] types = querys;
			b.setItems(types, new DialogInterface.OnClickListener() {
				@Override
				public void onClick(DialogInterface dialog, int which) {
					dialog.dismiss();
					goBrowseVoiceSearch(types[which]);
				}
			});
			b.show();
		}
	}


}
