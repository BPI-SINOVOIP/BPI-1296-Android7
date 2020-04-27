package com.rtk.mediabrowser;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.res.Resources;
import android.graphics.BitmapFactory;
import android.media.ExifInterface;
import android.os.*;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import com.realtek.DataProvider.FileFilterType;
import com.realtek.Utils.InformPreViewMsg;
import com.realtek.Utils.MbwType;
import com.realtek.bitmapfun.util.CommonFragmentWithImageWorker;
import com.realtek.bitmapfun.util.ImageCache;
import com.realtek.bitmapfun.util.ImageFetcher;
import com.realtek.bitmapfun.util.ReturnSizes;
import com.realtek.bitmapfun.util.HeifReader;

import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;

public class PhotoPreViewFragment extends CommonFragmentWithImageWorker
{
	public static final String IMAGE_CACHE_DIR = "images";
	private static final boolean DEBUG = true;
	private ReturnSizes mReturnSizes;
	private String      TAG ="PhotoPreViewFragment" ;
	private ImageView   m_ImageView_photo;
	private TextView    m_TextView_media_size;
	private TextView    m_TextView_media_time;
	private TextView mTextViewMediaResolution;
	private int m_ImageH;
	private int m_ImageW;
	private boolean m_Playing = false;
	private boolean m_Pause = true;

	private static Handler m_MyHandler;//Handle play/stop preview information
	public InformPreView m_MyInformPreView;//It will be used to inform File change/ stop playback by activity whom create this fragment.


	//private ImageInfoTask m_ImageInfoTask;
	//Photo Exif API
	private void setJPEGInfo(String path)
	{
		Log.d(TAG,"setJPEGInfo");
		ExifInterface exif = null;
		try {
			exif = new ExifInterface(path);
		} catch (IOException e) {
			e.printStackTrace();
		}
		if(exif==null) return;
		String date = exif.getAttribute(ExifInterface.TAG_DATETIME);
		//String date = exif.getAttribute(ExifInterface.TAG_EXPOSURE_TIME);
		date = dateFormat(date);
		m_TextView_media_time.setText(date);
	}
	private void setBitmapInfo(String path)
	{
		Log.d(TAG,"setBitmapInfo");
            if(path.toLowerCase().endsWith("heic") || path.toLowerCase().endsWith("heif")) {
                HeifReader heifReader = new HeifReader(getActivity(), path, null);
                heifReader.getThumbnail();
                m_ImageH = heifReader.getHeight();
                m_ImageW = heifReader.getWidth();
            }
            else {
                int orientation = 0;;
                if(path.toLowerCase().indexOf("jpeg")>=0 || path.toLowerCase().indexOf("jpg")>=0) {
                    try {
                        ExifInterface exif = new ExifInterface(path);
                        orientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_NORMAL);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                BitmapFactory.Options option = new BitmapFactory.Options();
                option.inJustDecodeBounds = true;
                //option.inPurgeable = true;
                //option.inInputShareable = true;
                BitmapFactory.decodeFile(path, option);
                if(orientation==ExifInterface.ORIENTATION_ROTATE_90 || orientation==ExifInterface.ORIENTATION_ROTATE_270) {
                    m_ImageH = option.outWidth;
                    m_ImageW = option.outHeight;
                }
                else {
                    m_ImageH = option.outHeight;
                    m_ImageW = option.outWidth;
                }
                option = null;
            }
            mTextViewMediaResolution.setText(String.format("%d X %d",m_ImageW, m_ImageH));
	}

	//Device Info API
	private void setDeviceTextView(int FileType, String path)
	{
		if (FileType==FileFilterType.DEVICE_FILE_DEVICE)
			m_TextView_media_time.setText(getString(R.string.mb_storage));
		else if (FileType==FileFilterType.DEVICE_FILE_DLNA_DEVICE)
			m_TextView_media_time.setText(getString(R.string.mb_dlna));
		else//(FileType==FileFilterType.DEVICE_FILE_SMB_DEVICE)
			m_TextView_media_time.setText(getString(R.string.mb_samba));
		mTextViewMediaResolution.setText(path);
	}
	private void setTextColor(int MBWType)
	{
		Resources r = getResources();
		if (MBWType == MbwType.TYPE_DB_PHOTO)
		{
			m_TextView_media_time.setTextColor(r.getColor(R.color.BWTitlePhotoBlue));
			m_TextView_media_size.setTextColor(r.getColor(R.color.BWTitlePhotoBlue));
			mTextViewMediaResolution.setTextColor(r.getColor(R.color.BWTitlePhotoBlue));
		}
		else
		{
			m_TextView_media_time.setTextColor(r.getColor(R.color.BWTitleGreen));
			m_TextView_media_size.setTextColor(r.getColor(R.color.BWTitleGreen));
			mTextViewMediaResolution.setTextColor(r.getColor(R.color.BWTitleGreen));
		}
	}

	private String dateFormat(String date)
	{
		final String NO_DATE_STRING = "----:--:-- --:--:--";
		if(date == null)
			return NO_DATE_STRING;
		log("dateFormat process: "+date);
		SimpleDateFormat df_pattern1 = new SimpleDateFormat("yyyy:MM:dd HH:mm:ss");
		SimpleDateFormat df_pattern2 = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss");
		SimpleDateFormat df_pattern3 = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ssZ");
		SimpleDateFormat df_des = new SimpleDateFormat("HH:mm EEEE,dd MMMM yyyy");
		java.util.Date d = null;
		try {
			d = df_pattern1.parse(date);
		} catch (ParseException e) {
			e.printStackTrace();
		}
		//if df_pattern1 has parse exception, then, use df_pattern2 to parse
		if(d==null){
			try{
				d = df_pattern2.parse(date);
			} catch (ParseException e) {
				e.printStackTrace();
			}
		}
		//if df_pattern2 has parse exception, then, use df_pattern3 to parse
		if(d==null){
			try{
				d = df_pattern3.parse(date);
			} catch (ParseException e) {
				e.printStackTrace();
			}
		}
		if(d==null)
			return NO_DATE_STRING;
		String formateDate1 = null;
		String formateDate2 = null;
		formateDate1 = df_des.format(d);
		formateDate2 = d.getHours()+":"+d.getMinutes()+" "+d.getDay()+","+d.getDate()+" "+d.getMonth()+" "+d.getYear();
		log("we get date:" + formateDate1 + "   " + formateDate2);
		return formateDate1;
	}

	private void log(String s) {
		if(DEBUG)
			Log.d(TAG, s);
	}

	class InitDiskCacheTask extends AsyncTask<Void, Void, Void> {
		ProgressDialog progDialog;
		@Override
		protected void onPreExecute() {
			super.onPreExecute();
			progDialog = new ProgressDialog(getActivity());
			progDialog.setIndeterminate(false);
			progDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
			progDialog.setCancelable(true);
			progDialog.show();
		}

		@Override
		protected Void doInBackground(Void... params) {
//			synchronized (mDiskCacheLock) {
			if(getActivity()==null) {
				log("InitDiskCacheTask getActivity()==null");
				return null;
			}
			log("locked in InitDiskCacheTask");

			mReturnSizes =  new ReturnSizes(604,420);//It should sync with fragement_photo_preview.xml
			mImageWorker = ImageFetcher.getInstance(getActivity(), null, mReturnSizes.getWidth(),
					mReturnSizes.getHeight());
			return null;
		}

		@Override
		protected void onPostExecute(Void aVoid) {
			super.onPostExecute(aVoid);
			if (getActivity()==null) { // or call isFinishing() if min sdk version < 17
				return;
			} else {
				if(Build.VERSION.SDK_INT>=Build.VERSION_CODES.JELLY_BEAN_MR1)
					if(getActivity().isDestroyed()) return;
					else
					if(getActivity().isFinishing()) return;
			}
			if(progDialog !=null) {
				progDialog.dismiss();
			}
		}
	}

	//Photo decode API
	protected void initImageWorker()
	{
		Log.d(TAG,"initImageWorker");
		new InitDiskCacheTask().execute();
	}
	private void loadimage(ImageView view, String path)
	{
		ImageFetcher.getInstance().loadImage(path, view);
	}
	private void cancelDecode(ImageView view)
	{
		if(view !=null)
		{
			mImageWorker.cancelWork(view);
		}
	}

	@Override
	public void onResume()
	{
		super.onResume();
		Log.d(TAG,"onResume:");
		m_Pause = false;
	}

	@Override
	public void onDestroyView()
	{
		super.onDestroyView();
		Log.d(TAG,"onDestroyView");
	}

	@Override
	public void onPause()
	{
		super.onPause();
		m_Pause = true;
		Log.d(TAG,"onPause");
		//Do PLAY_STOP work immediately instead of send command to handler.
		m_MyHandler.removeCallbacksAndMessages(null);
		cancelDecode(m_ImageView_photo);
		m_ImageView_photo.setVisibility(View.INVISIBLE);
		//m_MyHandler.sendEmptyMessage(InformPreViewMsg.PLAY_STOP);
	}

	@Override
	public void onAttach(Activity activity) {
		super.onAttach(activity);
		Log.d(TAG,"onAttach");
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
							 Bundle savedInstanceState)//call by system
	{
		Log.d(TAG,"onCreateView inflater");

		return inflater.inflate(R.layout.fragment_photo_preview, container, false);
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) //call immediately after onCreateView
	{
		super.onViewCreated(view, savedInstanceState);
		Log.d(TAG, "onViewCreated view");

		m_ImageView_photo 	  = (ImageView)(view.findViewById(R.id.photo));
		m_TextView_media_size = (TextView)(view.findViewById(R.id.media_size));
		m_TextView_media_time = (TextView)(view.findViewById(R.id.media_time));
		mTextViewMediaResolution = (TextView) view.findViewById(R.id.media_resolution);
		m_MyHandler = new Handler(){
			@Override
			public void handleMessage(Message msg)
			{
				switch(msg.what)
				{
					case InformPreViewMsg.MEDIA_PLAY:
						if ((m_Pause == false) && (msg.obj instanceof String))
						{
							m_Playing = true;
							Log.d(TAG,"Try to preview: "+(String)msg.obj);
							setTextColor(msg.arg1);
							if(msg.arg2 == FileFilterType.DEVICE_FILE_PHOTO)
							{
								//Play photo
								String path = (String)msg.obj;
								if (msg.arg1==MbwType.TYPE_DEVICE_DMS)
								{
									if (msg.peekData()!=null)
									{
										Bundle bundle = msg.getData();
										String Date = bundle.getString("Date");
										String Resolution = bundle.getString("Resolution");
										String size = bundle.getString("Size");
										if (Date != null)
											m_TextView_media_time.setText(Date);
										if (Resolution != null)
											mTextViewMediaResolution.setText(Resolution);
										if (size != null)
											m_TextView_media_size.setText(size);
									}
								}
								else
								{
									//Log.d(TAG, "Length="+path.length()+"indexof(.jpg)="+path.indexOf(".jpg"));
									if (((path.length()-4) == path.toLowerCase().indexOf(".jpg")) ||
											((path.length()-5) == path.toLowerCase().indexOf(".jpeg")) )
										setJPEGInfo(path);
									else
										m_TextView_media_time.setText("----:--:-- --:--:--");

									setBitmapInfo(path);
								}
                                        /*
                                        if (m_ImageInfoTask!=null)
                                        {
                                            m_ImageInfoTask.cancel(true);
                                        }
                                        m_ImageInfoTask = new ImageInfoTask(m_TextView_media_time,m_TextView_media_size);
                                        m_ImageInfoTask.execute(path);
                                        */
								loadimage(m_ImageView_photo,path);
								m_ImageView_photo.setVisibility(View.VISIBLE);

								//Toast.makeText(getActivity(),getString(R.string.mb_image_over_supporting), Toast.LENGTH_LONG).show();
							}
							else if (msg.arg2 == FileFilterType.DEVICE_FILE_DEVICE)
							{
								m_ImageView_photo.setImageResource(R.drawable.icon_allfile_pre_usb);
								m_ImageView_photo.setVisibility(View.VISIBLE);
								setDeviceTextView(msg.arg2,(String)msg.obj);
							}
							else if (msg.arg2 == FileFilterType.DEVICE_FILE_DLNA_DEVICE)
							{
								m_ImageView_photo.setImageResource(R.drawable.icon_allfile_pre_upnp);
								m_ImageView_photo.setVisibility(View.VISIBLE);
								setDeviceTextView(msg.arg2,(String)msg.obj);
							}
							else if (msg.arg2 == FileFilterType.DEVICE_FILE_PLAYLIST)
							{
								m_ImageView_photo.setImageResource(R.drawable.icon_default_audio);
							}
							else //(msg.arg2 == FileFilterType.DEVICE_FILE_SMB_DEVICE)
							{
								m_ImageView_photo.setImageResource(R.drawable.icon_allfile_pre_network);
								m_ImageView_photo.setVisibility(View.VISIBLE);
								setDeviceTextView(msg.arg2,(String)msg.obj);
							}
						}
						else
						{
							Log.e(TAG,"Can not load media now m_Pause="+m_Pause);
						}
						break;
					case InformPreViewMsg.PLAY_STOP:
						Log.d(TAG, "InformPreViewMsg.PLAY_STOP, m_Playing="+m_Playing);
						if (m_Playing) {
							m_Playing = false;
							//Clean photo preview and info
							Log.d(TAG, "InformPreViewMsg.PLAY_STOP");
							cancelDecode(m_ImageView_photo);
							m_ImageView_photo.setVisibility(View.INVISIBLE);
							m_TextView_media_time.setText("");
							mTextViewMediaResolution.setText("");
							m_TextView_media_size.setText("");
						}
						break;
					case InformPreViewMsg.PROGRESS_CHANGED:
						break;
				}
				super.handleMessage(msg);
			}
		};
		initImageWorker();
		m_MyInformPreView = new InformPreView(m_MyHandler);
	}
        /*
        private class ImageInfoTask extends AsyncTask<String, void, void> {
            private final WeakReference<TextView> TimeViewRef;
            private final WeakReference<TextView> SizeViewRef;
            
            public ImageInfoTask(TextView TimeView, TextView SizeView)
            {
                TimeViewRef = new WeakReference<TextView> TimeView;
                SizeViewRef = new WeakReference<TextView> SizeView;
            }
            @Override
            protected void onCancelled()
            {
                super.onCancelled();
            }
            @Override 
            protected void doInBackground(String... params)
            {
            }
            @Override
            protected void onPostExecute()
            {
            }
            private TextView getAttachedTimeView(){
                final TextView view = TimeViewRef.get();
                return view;
            }
            private TextView getAttachedSizeView(){
                final TextView view = SizeViewRef.get();
                return view;
            }
        } 
        */
}
