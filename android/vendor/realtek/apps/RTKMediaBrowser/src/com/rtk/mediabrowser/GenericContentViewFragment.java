package com.rtk.mediabrowser;
import android.app.Activity;
import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.*;
import android.widget.AbsListView.OnScrollListener;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemSelectedListener;
import com.realtek.DataProvider.AbstractDataProvider;
import com.realtek.DataProvider.FileFilterType;
import com.realtek.Utils.MarqueeTextView;
import com.realtek.Utils.MbwType;
import com.realtek.bitmapfun.util.*;


public class GenericContentViewFragment extends CommonFragmentWithImageWorker
{
    private ReturnSizes mReturnSizes;
    private RtkFileBrowserGridView    mGridView=null;
    private RtkListView    mListView=null;
    private AbsListView    mAbstractView=null;
    private String TAG = "GenericContentViewFrag";
    private static final boolean DEBUG = true;
    public AbstractViewAdapter mAbstractViewAdapter=null;
    public DirectionControl directionControl =null;
	public final static int  GRID_VIEW =0;
	public final static int  LIST_VIEW =1;
    
    
    private ImageView mImageView;
    public static final String IMAGE_CACHE_DIR = "images";
	public int m_NumOfColumns = 0;
	public int m_NumOfRows = 0;
    public int m_NumOfItemsPerPage = 0;    
	public int m_ViewType = 0;
    public boolean mStopLoadImage = false;
    public boolean mIgnoreKeyEvent = false;

    public interface UiListener
    {
        
        public AbstractDataProvider getCurrentDataProvider();
		public int getCurrentMbwType();
        public void onItemSelected(int position);
        public void onItemClicked (int position);
        public boolean onKeyClicked(View view,int keyCode,KeyEvent event,int position,int iconNum,int firstVisibleItem,int lastVisibleItem);
		public int GetNumOfColumns();
		public int GetNumOfRows();
		public int GetViewType();
        public void LoadingBusy(int state);
        
    }

    private View getViewByPosition(int pos, ListView listView) {
        final int firstListItemPosition = listView.getFirstVisiblePosition();
        final int lastListItemPosition = firstListItemPosition + listView.getChildCount() - 1;

        if (pos < firstListItemPosition || pos > lastListItemPosition ) {
            return listView.getAdapter().getView(pos, null, listView);
        } else {
            final int childIndex = pos - firstListItemPosition;
            return listView.getChildAt(childIndex);
        }
    }

    private static final int VIDEO_4K_WIDTH = 3840;
    private static final int VIDEO_4K_HEIGHT = 2156;
    private static boolean sIs4KVideo = false;

    public void notifyVideoSize(int width, int height) {
        if (mAbstractView == mListView && mListView != null) {
            if (DEBUG) {
                String s = String.format("w:%d,h:%d", width, height);
                Log.d(TAG, s);
            }
            if (width >= VIDEO_4K_WIDTH && height >= VIDEO_4K_HEIGHT) {
//                Log.d(TAG, ">=4K Video");
                sIs4KVideo = true;
                int pos = mListView.getSelectedItemPosition();
                View itemView = getViewByPosition(pos, mListView);
                mAbstractViewAdapter.getView(pos, itemView, mListView);
            }
        }
    }
    
    private UiListener mUiCallback;
    private class AbstractViewLoadingControl implements LoadingControl
    {

    	@Override
    	public void startLoading() {
    		mUiCallback.LoadingBusy(1);
    	}


    	@Override
    	public void stopLoading() {
    		mUiCallback.LoadingBusy(0);
    	}
    }
    private AbstractViewLoadingControl gv_loadingcontrol=new AbstractViewLoadingControl();
    
    
    
    @Override
    public void onResume()
    {
        mStopLoadImage = false;
        super.onResume();
        Log.d(TAG, "onResume:");
    }
    
    @Override
    public void onDestroyView()
    {
    	
    	super.onDestroyView();
    }    
   
    @Override
    public void onDestroy()
    {
        super.onDestroy();
        Log.d(TAG, "onDestroy:");
    } 
    @Override
    public void onDetach()
    {
        super.onDetach();
        Log.d(TAG, "onDetach:");
    } 
    
    @Override
    public void onPause()
    {
        mStopLoadImage = true;
        Log.d(TAG,"onPause");
        cancelWaitTask();
        stopGridViewDecode();
        super.onPause();
    }

    @Override
    public void onAttach(Activity activity) {      
		Log.d(TAG,"onAttach()");	
        super.onAttach(activity);       
        try{
            mUiCallback = (UiListener) activity; // check if the interface is implemented
        }catch(ClassCastException e){
            e.printStackTrace();
        }
		m_NumOfColumns = mUiCallback.GetNumOfColumns();
		m_NumOfRows = mUiCallback.GetNumOfRows();
		m_ViewType = mUiCallback.GetViewType();
		m_NumOfItemsPerPage = m_NumOfColumns *m_NumOfRows;
    }
    
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState)//call by system  
    {
        mStopLoadImage = false;
        Log.d(TAG, "onCreatedView inflater ViewType=" + m_ViewType);
        View RltView;
		if (m_ViewType==GRID_VIEW)
		{
        	RltView = inflater.inflate(R.layout.fragment_gridview, container, false);
		}
		else
		{
        	RltView = inflater.inflate(R.layout.fragment_listview, container, false);
		}
		return RltView;
    }
    
    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) //call immediately after onCreateView
    {
        super.onViewCreated(view, savedInstanceState);
                
        Log.d(TAG, "onViewCreated view");
      	 
		if (m_ViewType==GRID_VIEW)
		{
        	mGridView=(RtkFileBrowserGridView)(view.findViewById(R.id.gridview));
			mAbstractView = mGridView;
		}
		else
		{
        	mListView=(RtkListView)(view.findViewById(R.id.listview));
			mAbstractView = mListView;
		}
        setAbstractView();
      
    }
    
    public void RefreshAbstractView(int initPos)
    {
        Log.d(TAG,"RefreshAbstractView");
        stopGridViewDecode();

        mStopLoadImage = false;
        mAbstractView.setSelected(true);
        mAbstractViewAdapter.RefreshData();
        mAbstractViewAdapter.notifyDataSetChanged();
        //mAbstractView.invalidateViews();
        mAbstractView.requestFocusFromTouch();
        mAbstractView.setSelection(initPos);
        mAbstractView.requestFocus();
    	mUiCallback.onItemSelected(mAbstractView.getSelectedItemPosition());
    }

    private WaitResourceTask mWaitResourceTask;

    class WaitResourceTask extends AsyncTask<Void, Integer, Void> {

        private final Runnable mRunnable;
        private final Activity mCtx;

        public WaitResourceTask(Activity ctx, Runnable runnable) {
            mCtx = ctx;
            mRunnable = runnable;
        }

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            log("WaitResourceTask begin");
            mIgnoreKeyEvent = true;
        }

        @Override
        protected Void doInBackground(Void... voids) {
            while (mImageWorker != null && mImageWorker.isBusyOnVideoframeDecode()) {
                try {
                    Thread.sleep(50);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void aVoid) {
            log("WaitResourceTask end");
            mIgnoreKeyEvent = false;
            if(!isCancelled())
                mCtx.runOnUiThread(mRunnable);
            super.onPostExecute(aVoid);
        }
    }

    public void WaitResourceRelease(Activity activity, Runnable runnable)
    {
        cancelWaitTask();
        mWaitResourceTask = new WaitResourceTask(activity, runnable);
        mWaitResourceTask.execute();
    }

    private void cancelWaitTask(){
        if(mWaitResourceTask!=null){
            mWaitResourceTask.cancel(true);
            mWaitResourceTask = null;
        }
    }

    public void stopGridViewDecode() {
        Log.d(TAG, "stopGridViewDecode");
        //Cancel image decode request. 
        mStopLoadImage = true;//Bug:44560, GridView will call getView at any time, so change mStopLoadImage state to true ahead for blocking mImageWorker.loadImage when GridView call getView().
        if (mGridView != null) {
            int childCount = mGridView.getChildCount();
            for (int i = 0; i < childCount; i++) {
                View v = mGridView.getChildAt(i);
                if (v != null) {
                    cancelDecode(v);
                }
            }
        }
    }
    
    //Handle of android key
    /*
    private class AbstractViewOnKeyListener implements OnKeyListener
    {
        public boolean onKey(View view, int keyCode, KeyEvent event)
        {
        	Log.d(TAG,"AbstractViewOnKeyListener"+keyCode+"KeyEvent"+event);
        	if(event.getAction()==KeyEvent.ACTION_DOWN)
        	{
	        	int position = mAbstractView.getSelectedItemPosition();
	        	int iconNum=mAbstractView.getCount();
	        	int firstVisibleItem=mAbstractView.getFirstVisiblePosition();
	        	int lastVisibleItem=mAbstractView.getLastVisiblePosition();
	        	return mUiCallback.onKeyClicked(view,keyCode,event,position,iconNum,firstVisibleItem,lastVisibleItem);
        	}
			return false;       
        }
    }
    */
    public AbstractViewAdapter getAbstractViewAdapter()
    {
    	return mAbstractViewAdapter;
    }

    public class DirectionControl
    {
    	private AbstractViewAdapter adapter = null;
    	private int positionCurrent_allitems = 0;
    	private int positionCurrent_pageitem = 0;
		
		public int m_NumOfColumns = 5;
		public int m_NumOfRows = 3;
    	public int m_NumOfItemsPerPage = 15;    
		public int m_DownBoundOffset;	
    	DirectionControl(int NumOfColumns, int NumOfRows)
    	{
    		adapter = getAbstractViewAdapter();
    		positionCurrent_pageitem = mAbstractView.getSelectedItemPosition();
			m_NumOfColumns = NumOfColumns;
			m_NumOfRows = NumOfRows;
			m_NumOfItemsPerPage = m_NumOfColumns*m_NumOfRows;
			m_DownBoundOffset = 2; //selected row will bound on second row from bottom, when down key is pressed.
    		positionCurrent_allitems = positionCurrent_pageitem + adapter.moveLine*m_NumOfColumns + adapter.movePage*m_NumOfItemsPerPage;
    	}
    	public AbstractViewAdapter GetAdapter()
    	{
    		return adapter;
    	}
    	public void DataRefresh(int NewPosition)
		{
    		int OldPosition = mAbstractView.getSelectedItemPosition();
    		adapter.RefreshData();
    		adapter.notifyDataSetChanged();
			if (NewPosition == OldPosition)
			{
				Log.d(TAG,"DataRefresh Same position:"+mAbstractView.getSelectedItemPosition());
    			//mAbstractView.setSelection(mAbstractView.getSelectedItemPosition());
			}
			else
			{
    			mAbstractView.setSelection(NewPosition);
    	        mUiCallback.onItemSelected(mAbstractView.getSelectedItemPosition());
			}
		}
    }
    
     
    private class AbstractViewItemSelectedListener implements OnItemSelectedListener
    {
        @Override
        public void onItemSelected(AdapterView<?> parent, View view, int position , long id)
        {  
            Log.d(TAG,"onItemSelected position:"+position+" id:"+id);
    	    mUiCallback.onItemSelected(position);
            //reset sIs4KVideo when item changed
            sIs4KVideo = false;
        }
        @Override
        public void onNothingSelected(AdapterView<?> parent)
        {
            Log.d(TAG,"onNothingSelected:");
            //reset sIs4KVideo when item changed
            sIs4KVideo = false;
        }           
    }
    
    private class AbstractViewItemClickListener implements OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id) 
        {   
            mUiCallback.onItemClicked(position);
        }
    }
    private class AbstractViewScrollListener implements OnScrollListener  
	{
		@Override
		public void onScroll(AbsListView view, int firstVisibleItem, int visibleItemCount, int totalItemCount)
		{
			Log.d(TAG,"AbstractViewScrollListener onScroll");
		}
		public void onScrollStateChanged(AbsListView view, int scrollState)
		{
			Log.d(TAG,"AbstractViewScrollListener onScrollStateChanged state:"+scrollState);
		}
	}
    /* 
   	private class AbstractViewGenericMotionListener implements OnGenericMotionListener
	{
 		@Override
		public boolean onGenericMotion(View v, MotionEvent event)
		{
			boolean Rlt=false;
			if (0 != (event.getSource() & InputDevice.SOURCE_CLASS_POINTER)) 
			{
    			switch (event.getAction()) 
				{
      				case MotionEvent.ACTION_SCROLL:
					{
        				if (event.getAxisValue(MotionEvent.AXIS_VSCROLL) < 0.0f)
						{
							//Down
        					Log.d(TAG,"Mouse wheel SCROLL DOWN");
							directionControl.DownOperation();
						}
        				else
						{
							//Up
        					Log.d(TAG,"Mouse wheel SCROLL UP");
							directionControl.UpOperation();
						}
        				Rlt = true;
					}
					break;
					default:
					break;
    			}
  			}
  			return Rlt;
		}    
	} 
    */ 
    @Override
    protected void initImageWorker()
    {
        Log.d(TAG,"initImageWorker");
        new InitDiskCacheTask().execute();
    }

    class InitDiskCacheTask extends AsyncTask<Void, Void, Void> {
        ProgressDialog progDlg;
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            progDlg = new ProgressDialog(getActivity());
            progDlg.setIndeterminate(false);
            progDlg.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            progDlg.setCancelable(true);
            progDlg.show();
            log("InitDiskCacheTask progDlg.show()");
        }

        @Override
        protected Void doInBackground(Void... params) {
            if(getActivity()==null) return null;
            log("InitDiskCacheTask doInBackground start ");
            mReturnSizes =  new ReturnSizes(342, 230);//It should sync with item_of_gridview.xml
            mImageWorker = ImageFetcher.getInstance(getActivity(), null, mReturnSizes.getWidth(),
                    mReturnSizes.getHeight());
            log("InitDiskCacheTask doInBackground end");
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
            if(progDlg !=null) {
                log("InitDiskCacheTask progDlg.dismiss()");
                progDlg.dismiss();
            }
        }
    }

    private void log(String s) {
        if(DEBUG)
            Log.d(TAG, s);
    }

    public void cancelDecode(View view)
    {
        if(view !=null)
        {    
            ImageView imageView = (ImageView)view.findViewById(R.id.grid_img);
            mImageWorker.cancelWork(imageView);
        }
    }
   	public void SetFocus(View CurrentFocusView)
	{
		if (CurrentFocusView != mAbstractView)
		{
        	mAbstractView.requestFocus();
			Log.d(TAG,"SetFocus to GenericContentViewFragment");
		}
	}
	 
    
    private void setAbstractView()
    {
        Log.d(TAG,"setAbstractView");
       
        mAbstractView.setOnItemSelectedListener(new AbstractViewItemSelectedListener());//Handle of item is selected
        mAbstractView.setOnItemClickListener(new AbstractViewItemClickListener());//Handle of item is clicked
        mAbstractView.setOnScrollListener(new AbstractViewScrollListener());
        //mAbstractView.setOnGenericMotionListener(new AbstractViewGenericMotionListener());
        //mAbstractView.setOnKeyListener(new AbstractViewOnKeyListener());//Handle of android key
        mAbstractViewAdapter = new AbstractViewAdapter(m_NumOfColumns, m_NumOfRows,m_ViewType);
		if (m_ViewType == GRID_VIEW)
		{
//        	mGridView.setNumColumns(m_NumOfColumns);
		}
		/*
           try {
            Thread.sleep(1);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
		*/
        mAbstractView.setAdapter(mAbstractViewAdapter);
        mAbstractView.setFocusable(true);
        mAbstractView.requestFocus();
        mAbstractView.setSelection(0);
        mAbstractView.setSelected(true);
        mAbstractView.setClickable(true);
        mAbstractView.setChoiceMode(ListView.CHOICE_MODE_SINGLE);
       
        directionControl = new DirectionControl(m_NumOfColumns, m_NumOfRows);
        
        Log.d(TAG,"setAbstractView focusIndex:"+mAbstractView.getSelectedItemPosition());
        
      
    }
    
    
    
    class AbstractViewAdapter extends BaseAdapter 
    {        
    	private AbstractDataProvider dataProvider = null;
    	public int moveLine=0;
    	public int movePage=0;
    	public int positionCurrent=0;
    	public int countTotal = 0;
		public int m_NumOfColumns = 5;
		public int m_NumOfRows = 3;
		public int m_NumOfItemsPerPage = 15;
		public int m_ViewType = 0;
		public int m_MbwType =0;

    	AbstractViewAdapter(int NumOfColumns, int NumOfRows, int ViewType)
    	{
			m_NumOfColumns = NumOfColumns;
			m_NumOfRows = NumOfRows;
			m_ViewType = ViewType;
			m_NumOfItemsPerPage = m_NumOfColumns * m_NumOfRows;
			CalculateInternalVar();
    	}

    	public void RefreshData()
    	{
			Log.d(TAG,"RefreshData");
			CalculateInternalVar();
    	}

    	private void CalculateInternalVar()
		{
    		dataProvider = mUiCallback.getCurrentDataProvider();
    		if(dataProvider != null)
    		{
    			countTotal = dataProvider.GetSize();
    			Log.d(TAG, "countTotal is :"+countTotal);
    		}
            else
            {
                countTotal = 0;
            }
		}
    		
        
        @Override
        public int getCount() {
			//Log.d(TAG,"getCount:"+countTotal);
            return countTotal;
        }

        
        @Override
        public Object getItem(int arg0) {
            // TODO Auto-generated method stub
            return null;
        }

        @Override
        public long getItemId(int position) {
            // TODO Auto-generated method stub
            return 0;
        }

		class GridViewHolder {
			ImageView imageView;
			MarqueeTextView tv;
			ImageView FocusView;
			ImageView TypeimageView;
		}
		class ListViewHolder {
			ImageView FocusedimageView;
			ImageView FolderimageView;
			MarqueeTextView tv;
			ImageView TypeimageView;
		}
        @Override
        public View getView(int position, View convertView, ViewGroup parent) 
        {
            GridViewHolder GridViewholder;
            ListViewHolder ListViewholder;
			m_MbwType = mUiCallback.getCurrentMbwType();
            if (position >=countTotal)
            {
                Log.d(TAG,"[ERROR]getView position:"+position+" over counTotal:"+countTotal);
            }
			if (m_ViewType == GenericContentViewFragment.GRID_VIEW)					 {	
        		if (convertView == null) 
        		{   
        		    // if it's not recycled, initialize some attributes
        		    LayoutInflater inflater = LayoutInflater.from(getActivity());
        		    convertView = inflater.inflate(R.layout.item_of_gridview, null);
					GridViewholder = new GridViewHolder();
        			GridViewholder.imageView = (ImageView)(convertView.findViewById(R.id.grid_img));
        			GridViewholder.TypeimageView = (ImageView)(convertView.findViewById(R.id.grid_type_img));
        			GridViewholder.FocusView = (ImageView)(convertView.findViewById(R.id.focus_img));
        			GridViewholder.tv = (MarqueeTextView)(convertView.findViewById(R.id.grid_text));
					GridViewholder.tv.setSelectedChangeStyle(R.style.THUSelectedText,R.style.THUUnselectedText);
					if (m_MbwType == MbwType.TYPE_DB_VIDEO){
						GridViewholder.FocusView.setImageResource(R.drawable.selector_grid_video_focused_bar);
					}
					else if (m_MbwType == MbwType.TYPE_DB_PHOTO){
						GridViewholder.FocusView.setImageResource(R.drawable.selector_grid_photo_focused_bar);
					}
					else{
						GridViewholder.FocusView.setImageResource(R.drawable.selector_grid_allfile_focused_bar);
					}
					convertView.setTag(GridViewholder);

        		} 
        		else 
        		{
        		    GridViewholder = (GridViewHolder)convertView.getTag();
        		 }
                         if(((RtkFileBrowserGridView)parent).isOnMeasure) {
                             return convertView;
                         }
				RefreshGridView(position, GridViewholder);
			}
			else
			{
        		if (convertView == null) 
        		{   
        		    // if it's not recycled, initialize some attributes
        		    LayoutInflater inflater = LayoutInflater.from(getActivity());
        		    convertView = inflater.inflate(R.layout.item_of_listview, null);
					ListViewholder = new ListViewHolder();
        			ListViewholder.FocusedimageView = (ImageView)(convertView.findViewById(R.id.focused_img));
        			ListViewholder.FolderimageView = (ImageView)(convertView.findViewById(R.id.folder_img));
        			ListViewholder.TypeimageView = (ImageView)(convertView.findViewById(R.id.list_type_img));
        			ListViewholder.tv = (MarqueeTextView)(convertView.findViewById(R.id.list_text));
					ListViewholder.tv.setSelectedChangeStyle(R.style.PRESelectedText,R.style.PREUnselectedText);
					if (m_MbwType == MbwType.TYPE_DB_VIDEO)
						ListViewholder.FocusedimageView.setImageResource(R.drawable.selector_list_video_focused_bar);
					else if (m_MbwType == MbwType.TYPE_DB_PHOTO)
						ListViewholder.FocusedimageView.setImageResource(R.drawable.selector_list_photo_focused_bar);
					else
						ListViewholder.FocusedimageView.setImageResource(R.drawable.selector_list_allfile_focused_bar);
					convertView.setTag(ListViewholder);
        		} 
        		else 
        		{
        		    ListViewholder = (ListViewHolder)convertView.getTag();
        		}
				RefreshListView(position, ListViewholder);
			}
            return convertView;
		}
       	public void RefreshListView(int position, ListViewHolder holder)
		{
        	String path = null;
        	String Typepath = null;
        	//position = position + movePage*m_NumOfItemsPerPage+moveLine*m_NumOfColumns;
        	int type = mUiCallback.getCurrentDataProvider().GetFileTypeAt(position);
        	switch (type)
        	{ 
        	    case FileFilterType.DEVICE_FILE_DIR:
        	    {
        	   	  	holder.FolderimageView.setVisibility(View.VISIBLE);
        	        holder.FolderimageView.setImageResource(R.drawable.icon_allfile_folder_next);
        	        holder.TypeimageView.setImageResource(R.drawable.selector_list_folder);
        	    }
        	    break;  
        	    case FileFilterType.DEVICE_FILE_DEVICE:
        	    {
        	   	  	holder.FolderimageView.setVisibility(View.VISIBLE);
        	        holder.FolderimageView.setImageResource(R.drawable.icon_allfile_folder_next);
        	        holder.TypeimageView.setImageResource(R.drawable.selector_list_storage);
        	    }
        	    break; 
        	    case FileFilterType.DEVICE_FILE_DLNA_DEVICE:
        	    {
        	   	  	holder.FolderimageView.setVisibility(View.VISIBLE);
        	        holder.FolderimageView.setImageResource(R.drawable.icon_allfile_folder_next);
        	        holder.TypeimageView.setImageResource(R.drawable.selector_list_dms);
        	    }
        	    break; 
        	    case FileFilterType.DEVICE_FILE_SMB_DEVICE:
        	    {
        	   	  	holder.FolderimageView.setVisibility(View.VISIBLE);
        	        holder.FolderimageView.setImageResource(R.drawable.icon_allfile_folder_next);
        	        holder.TypeimageView.setImageResource(R.drawable.selector_list_smb);
        	    }
        	    break; 
        	    
        	    case  FileFilterType.DEVICE_FILE_AUDIO:
        	    case  FileFilterType.DEVICE_FILE_PLAYLIST:
        	    {
        	   	  	holder.FolderimageView.setVisibility(View.INVISIBLE);
        	        holder.TypeimageView.setImageResource(R.drawable.selector_list_audio);
        	    }
        	    break;    
        	    
        	    case  FileFilterType.DEVICE_FILE_VIDEO:
        	    {
        	   	  	holder.FolderimageView.setVisibility(View.INVISIBLE);
        	        holder.TypeimageView.setImageResource(R.drawable.selector_list_video);
        	    }
        	    break; 
        	    
        	    case  FileFilterType.DEVICE_FILE_PHOTO:
        	    {
        	   	  	holder.FolderimageView.setVisibility(View.INVISIBLE);
        	        holder.TypeimageView.setImageResource(R.drawable.selector_list_photo);
        	    }  
        	    break;

        	    case  FileFilterType.DEVICE_FILE_APK:
        	    {
        	   	  	holder.FolderimageView.setVisibility(View.INVISIBLE);
        	        holder.TypeimageView.setImageResource(R.drawable.selector_list_apk);
        	    }  
        	    break;
        	    
        	    default:
        	    {
        	   	  	holder.FolderimageView.setVisibility(View.INVISIBLE);
        	        holder.TypeimageView.setImageResource(R.drawable.selector_list_folder);
        	    }
        	    break;
            }
            holder.tv.setText(mUiCallback.getCurrentDataProvider().GetTitleAt(position));
            holder.tv.setEllipsize(TextUtils.TruncateAt.MARQUEE);
            if(sIs4KVideo) {
                if(DEBUG) Log.d(TAG, "workaround for 4K video");
//                holder.tv.setEllipsize(TextUtils.TruncateAt.MIDDLE);
                holder.tv.setMarqueeRepeatLimit(1);
            } else {
                holder.tv.setMarqueeRepeatLimit(-1);
            }
		}	
       	public void RefreshGridView(int position, GridViewHolder holder)
		{
        	String path = null;
        	String Typepath = null;
        	//position = position + movePage*m_NumOfItemsPerPage+moveLine*m_NumOfColumns;
        	int type = mUiCallback.getCurrentDataProvider().GetFileTypeAt(position);
        	holder.imageView.setImageBitmap(null);
        	switch (type)
        	{ 
        	    case FileFilterType.DEVICE_FILE_DIR:
        	    {
        	        holder.imageView.setBackgroundResource(R.drawable.icon_allfile_grid_d);
        	        path = "android.resource://" + R.drawable.icon_allfile_grid_d;
					if (m_MbwType == MbwType.TYPE_DEVICE_USB||m_MbwType == MbwType.TYPE_DEVICE_DMS||m_MbwType == MbwType.TYPE_DEVICE_SMB){
			   	  		holder.TypeimageView.setImageResource(R.drawable.selector_grid_folder);
					}
                    else{
			   	  	    holder.TypeimageView.setImageURI(null);
                    }
			   	  	//holder.FocusView.setImageResource(R.drawable.selector_grid_file_focus);
        	    }
        	    break;  
        	    case FileFilterType.DEVICE_FILE_DEVICE:
        	    {
        	        holder.imageView.setBackgroundResource(R.drawable.icon_allfile_usb);
        	        path = "android.resource://" + R.drawable.icon_allfile_usb;
			   	  	holder.TypeimageView.setImageURI(null);
			   	  	//holder.FocusView.setImageResource(R.drawable.selector_grid_file_focus);
        	    }
        	    break; 
        	    
        	    case FileFilterType.DEVICE_FILE_DLNA_DEVICE:
        	    {
        	       	holder.imageView.setBackgroundResource(R.drawable.icon_allfile_upnp);
        	       	path = "android.resource://" + R.drawable.icon_allfile_upnp;
			   	  	holder.TypeimageView.setImageURI(null);
			   	  	//holder.TypeimageView.setImageResource(R.drawable.selector_grid_dms);
			   	  	//holder.FocusView.setImageResource(R.drawable.selector_grid_file_focus);
        	    }
        	    break; 
        	    case FileFilterType.DEVICE_FILE_SMB_DEVICE:
        	    {
        	        holder.imageView.setBackgroundResource(R.drawable.icon_allfile_network);
        	       	path = "android.resource://" + R.drawable.icon_allfile_network;
			   	  	holder.TypeimageView.setImageURI(null);
			   	  	//holder.TypeimageView.setImageResource(R.drawable.selector_grid_smb);
			   	  	//holder.FocusView.setImageResource(R.drawable.selector_grid_file_focus);
        	    }
        	    break; 
        	    case  FileFilterType.DEVICE_FILE_AUDIO:
        	    case  FileFilterType.DEVICE_FILE_PLAYLIST:
        	    {
        	        holder.imageView.setBackgroundResource(R.drawable.icon_allfile_grid_m);
					path = mUiCallback.getCurrentDataProvider().GetDataAt(position);
					if (m_MbwType == MbwType.TYPE_DEVICE_USB||m_MbwType == MbwType.TYPE_DEVICE_DMS||m_MbwType == MbwType.TYPE_DEVICE_SMB){
			   	  		holder.TypeimageView.setImageResource(R.drawable.selector_grid_audio);
					}
                    else{
			   	  	    holder.TypeimageView.setImageURI(null);
                    }
			   	  	//holder.FocusView.setImageResource(R.drawable.selector_grid_audio_focus);
        	    }
        	    break;    
        	    
        	    case  FileFilterType.DEVICE_FILE_VIDEO:
        	    {
        	        holder.imageView.setBackgroundResource(R.drawable.icon_allfile_grid_v);
//        	        path = GetVideoThumbnailPath( mUiCallback.getCurrentDataProvider().GetDataAt(position) );
					path = mUiCallback.getCurrentDataProvider().GetDataAt(position);
					if (m_MbwType == MbwType.TYPE_DEVICE_USB||m_MbwType == MbwType.TYPE_DEVICE_DMS||m_MbwType == MbwType.TYPE_DEVICE_SMB){
			   	  		holder.TypeimageView.setImageResource(R.drawable.selector_grid_video);
					}
                    else{
			   	  	    holder.TypeimageView.setImageURI(null);
                    }
			   	  	//holder.FocusView.setImageResource(R.drawable.selector_grid_video_focus);
        	    }
        	    break; 
        	    
        	    case  FileFilterType.DEVICE_FILE_PHOTO:
        	    {
        	        holder.imageView.setBackgroundResource(R.drawable.icon_allfile_grid_p);
        	        path = mUiCallback.getCurrentDataProvider().GetDataAt(position);
					if (m_MbwType == MbwType.TYPE_DEVICE_USB||m_MbwType == MbwType.TYPE_DEVICE_DMS||m_MbwType == MbwType.TYPE_DEVICE_SMB){
			   	  		holder.TypeimageView.setImageResource(R.drawable.selector_grid_photo);
					}
                    else{
			   	  	    holder.TypeimageView.setImageURI(null);
                    }
			   	  	//holder.FocusView.setImageResource(R.drawable.selector_grid_photo_focus);
        	    }  
        	    break;
        	    
        	    case  FileFilterType.DEVICE_FILE_APK:
        	    {
        	        holder.imageView.setBackgroundResource(R.drawable.icon_allfile_grid_apk);
        	        path = "android.resource://" + R.drawable.icon_allfile_grid_apk;
					if (m_MbwType == MbwType.TYPE_DEVICE_USB||m_MbwType == MbwType.TYPE_DEVICE_DMS||m_MbwType == MbwType.TYPE_DEVICE_SMB){
			   	  		holder.TypeimageView.setImageResource(R.drawable.selector_grid_apk);
					}
                    else{
			   	  	    holder.TypeimageView.setImageURI(null);
                    }
			   	  	//holder.FocusView.setImageResource(R.drawable.selector_grid_photo_focus);
        	    }  
        	    break;

        	    default:
        	    {
        	        holder.imageView.setBackgroundResource(R.drawable.icon_allfile_grid_d);
        	        path = "android.resource://" + R.drawable.icon_allfile_grid_d;
			   	  	holder.TypeimageView.setImageResource(R.drawable.selector_grid_audio);
			   	  	//holder.FocusView.setImageResource(R.drawable.selector_grid_photo_focus);
        	    }
        	    break;
        	}
            if (!mStopLoadImage)
            {
                if(mImageWorker == null)      
                {
                    initImageWorker();
                }else {
		        if (path != null){
                	//mImageWorker.loadImage(path, holder.imageView, gv_loadingcontrol, type);//Give loading control handler to indicate status of picture decode. 
                	mImageWorker.loadImage(path, holder.imageView, null, type);//Don't show image decode loading icon while it's busy.
		        }
                }
            }
        	holder.tv.setText(mUiCallback.getCurrentDataProvider().GetTitleAt(position));
		}	
       
    }

	public String GetVideoThumbnailPath(String videoPath)
	{
		String thumbnail=null;
		String videoName=null;
		String photoName=null;
	
		if( videoPath.lastIndexOf(".") > 0 )
			videoName = videoPath.substring(0, videoPath.lastIndexOf("."));
		
		for( int i=0; i< mUiCallback.getCurrentDataProvider().GetSize(); i++)
		{
			if(mUiCallback.getCurrentDataProvider().GetMimeTypeAt(i)!=null &&
				mUiCallback.getCurrentDataProvider().GetMimeTypeAt(i).substring(0, 5).equals("image"))
			{
				photoName = mUiCallback.getCurrentDataProvider().GetDataAt(i).substring(0, mUiCallback.getCurrentDataProvider().GetDataAt(i).lastIndexOf("."));
				if(videoName.equals(photoName))
				{
					thumbnail=mUiCallback.getCurrentDataProvider().GetDataAt(i);
					break;
				}
				else if(thumbnail==null)
				{
					thumbnail=mUiCallback.getCurrentDataProvider().GetDataAt(i);
				}
			}
		}
		return thumbnail;
	}
}
