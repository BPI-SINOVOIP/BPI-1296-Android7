package com.rtk.mediabrowser;


import android.util.Log;
//import com.rtk.mediabrowser.PlayService_tsb.RepeatStatus;


public class ThumbnailViewActivity extends AbstractFBViewActivity
{
	//private RelativeLayout mMainview=null;	
    
    private static final String MY_TAG = "ThumbnailViewActivity";

 	public void Child_Init()
	{
		Log.d(MY_TAG,"Child_Init()");	
		m_NumOfColumns = 5;
		m_NumOfRows =3;
		m_ViewType = GenericContentViewFragment.GRID_VIEW; 
	    //You should set init variable which will be use after setContentView() such like ViewType, numofcolumn, because setContentView will instance the element of layout file(Fragment).	
        setContentView(R.layout.thumbnail_view);
	}
    public void Child_onItemSelected(int position)
    {
    }
	public boolean Child_OptionHandle()
	{
		return false;//not taken, use default insterad. 
	}
    public int Child_ViewType()
    {
        return VIEW_TYPE_THUMBNAIL;
    }
}
