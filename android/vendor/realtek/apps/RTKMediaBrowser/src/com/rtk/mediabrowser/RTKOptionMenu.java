package com.rtk.mediabrowser;

import java.io.IOException;

import com.realtek.Utils.UseRtMediaPlayer;
import org.xmlpull.v1.XmlPullParserException;
import com.realtek.Utils.MarqueeTextView;
import android.widget.PopupWindow;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.content.Context;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;
import android.content.res.XmlResourceParser;
import java.util.ArrayList;
import java.util.Iterator;

import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.ImageView;

import android.widget.AdapterView.OnItemClickListener;
import android.view.View.OnKeyListener;
import android.view.View.OnTouchListener;
import android.view.MotionEvent;
import android.graphics.drawable.BitmapDrawable;

public class RTKOptionMenu extends PopupWindow
{
    private static final String TAG = "RTKOptionMenu";
	private PopupWindow m_PopWindow = null;
	private View m_Popview = null;
	private ListView m_ListView = null;
	private OptionMenuListAdapter m_ListAdapter = null;
	private int m_Width;
	private int m_Height;
	private int m_x;
	private int m_y;
	//Should call those API sequentially
	// Construct -->SetDefaultFocus-->setItemClickListener-->Show-->Hide-->Show
	public RTKOptionMenu(int X, int Y, int Width, int Height, XmlResourceParser in, View ContentView)
	{
		super(ContentView, Width, Height);
		m_x = X;
		m_y = Y;
		m_Width = Width;
		m_Height = Height;
		m_Popview = ContentView;
		m_ListView = (ListView)m_Popview.findViewById(R.id.option_list);
		m_ListAdapter = new OptionMenuListAdapter(in);
		m_ListView.setAdapter(m_ListAdapter);
		init();
	  		
	}

	//Pls don't call this construct via outside code, it's called by inside. 
	//public RTKOptionMenu(View contentView, int width, int height)
	private void init()
	{
		super.setAnimationStyle(R.style.PopupAnimation);
		super.setBackgroundDrawable(new BitmapDrawable());
		super.setTouchInterceptor(new OnTouchListener() {

			public boolean onTouch(View view, MotionEvent event) {

				if (event.getAction() == MotionEvent.ACTION_OUTSIDE) {

					Hide();
					return true;
				}

				return false;
			}
		});
	}
    //return value
	//	true: find the item outside desirded to modifiy
	//  false: Can not find the item        
	public boolean setDefaultFocus(String Str, boolean focus)
	{
		int i;
		for (i=0; i<m_ListAdapter.selectionList.size(); i++)
		{
			if (Str.equals(m_ListAdapter.selectionList.get(i).Name))
			{
				m_ListAdapter.selectionList.get(i).Selected = focus;
				m_ListAdapter.notifyDataSetChanged();
                                m_ListView.setSelection(i);
				return true;
			}
		}
		return false;
	}
	public void setItemClickListener(OnItemClickListener listener)
	{
		m_ListView.setOnItemClickListener(listener);
	}
	public void setOnKeyListener(OnKeyListener listener)
	{
		m_ListView.setOnKeyListener(listener);
	}
    public boolean isInTouchMode() {
        return m_ListView.isInTouchMode();
    }
	public void Show()
	{
		Log.d(TAG,"Show option menu");
		super.setFocusable(true);
		super.setOutsideTouchable(true);
		super.showAtLocation(m_Popview, Gravity.LEFT| Gravity.TOP, m_x, m_y);
	}
	public void Hide()
	{
		Log.d(TAG,"Hide option menu");
		super.setFocusable(false);
		super.setOutsideTouchable(false);
		super.dismiss();
	}

	public void refreshItem(int position, boolean b){
		if(m_ListAdapter!=null) {
			m_ListAdapter.selectionList.get(position).bool = b;
			m_ListAdapter.notifyDataSetInvalidated();
            m_ListView.requestFocusFromTouch();
			m_ListView.setSelection(position);
		}
	}
	public String getItemof(int position)
	{
		return m_ListAdapter.getItemName(position);
	}
	//OptionMenuListAdapter
    class OptionMenuListAdapter extends BaseAdapter 
	{
		private XmlPullParserFactory factory;	
		private XmlPullParser mXpp;
		ArrayList<selection> selectionList = null;

		class selection
		{
			String Group;
			String Name;
			boolean Selected;
			boolean GroupEnd;
			public boolean bool;
		}
	
		OptionMenuListAdapter(XmlResourceParser in)
		{
            Log.d(TAG,"OptionMenuListAdapter");
			try{
				factory = XmlPullParserFactory.newInstance();
				mXpp = in;
				ParseXML();
				printList();	
			} catch (XmlPullParserException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		private void ParseXML()throws XmlPullParserException,IOException
		{
            Log.d(TAG,"ParseXML");
			selection item = null;
			int eventType = mXpp.getEventType();
            Log.d(TAG,"ParseXML init eventType="+eventType);
			while (eventType != XmlPullParser.END_DOCUMENT)
			{
				String ItemName = null;
				String GroupName = null;
				String Name = null;
				switch(eventType)
				{
					case XmlPullParser.START_DOCUMENT:
						selectionList = new ArrayList<>();
					break;
					case XmlPullParser.START_TAG:
						Name = mXpp.getName();
						Log.d(TAG,"START_TAG Name: "+Name);
						if (Name.equals("Group"))
						{
							Log.d(TAG,"ParseXML Attribute count="+mXpp.getAttributeCount()+" Name= "+mXpp.getAttributeName(0)+ " Value= "+mXpp.getAttributeValue(0));
							GroupName = mXpp.getAttributeValue(0);
            				Log.d(TAG,"ParseXML Get New Group: "+GroupName);
						}
						else if(Name.equals("Item"))
						{
							item = new selection();
							item.Group = GroupName;
							item.GroupEnd = false;
							int attr_count = mXpp.getAttributeCount();
							int i;
							for (i=0; i<attr_count; i++)
							{
								if (mXpp.getAttributeName(i).equals("Name"))
								{
									item.Name = mXpp.getAttributeValue(i);
								}
								else if (mXpp.getAttributeName(i).equals("Focus"))
								{
									item.Selected = mXpp.getAttributeValue(i).equals("true");
								}
								else if (mXpp.getAttributeName(i).equals("GroupEnd"))
								{
									item.GroupEnd = mXpp.getAttributeValue(i).equals("true");
								}
								else
								{
									Log.d(TAG,"Got unknow item attribute:"+mXpp.getAttributeName(i));
								}
								if("mb_use_rt_player".equals(item.Name)){
									item.bool = UseRtMediaPlayer.getInstance(getContentView().getContext()).getUseRtPlayer();
								}
							}
            				Log.d(TAG,"ParseXML Get New Item: "+item.Name+"selected: "+item.Selected);
							selectionList.add(item);
						}
					break;
					case XmlPullParser.END_TAG:
						Name = mXpp.getName();
						Log.d(TAG,"END_TAG Name: "+Name);
						if (Name.equals("Group"))
						{
							GroupName = null;
						}
						else if(Name.equals("Item"))
						{
						}
					break;
				}
				eventType = mXpp.next();
            	Log.d(TAG,"ParseXML next eventType="+eventType);
			}
		}

		private void printList()
		{
            Log.d(TAG,"printList");
			Iterator<selection> it = selectionList.iterator();
			while(it.hasNext())
			{
				selection CurItem = it.next();
				Log.d(TAG,"selection Group: "+CurItem.Group+" Name: "+CurItem.Name+ "selected: "+CurItem.Selected);
			}
		}
		//Wen: Update specific item
        @Override
        public int getCount() {
            Log.d(TAG,"getCount: "+selectionList.size());
            return selectionList.size();
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
		public String getItemName(int position){
			return selectionList.get(position).Name;
		}
		class ViewHolder {
			ImageView selection_img;
			MarqueeTextView tv;
			ImageView group_end;
		}
        @Override
        public View getView(int position, View convertView, ViewGroup parent) 
        {
            ViewHolder holder;
            Log.d(TAG,"getView position :"+position);             
            if (convertView == null) 
            {   
                Log.d(TAG,"getview:convertView==null :"+position); 
                // if it's not recycled, initialize some attributes
	            Context context = parent.getContext();
	            LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                convertView = inflater.inflate(R.layout.item_of_option_menu, null);
				holder = new ViewHolder();
            	holder.selection_img = (ImageView)(convertView.findViewById(R.id.selection_img));
            	holder.group_end = (ImageView)(convertView.findViewById(R.id.group_end));
            	holder.tv = (MarqueeTextView)(convertView.findViewById(R.id.selection_text));
				holder.tv.setSelectedChangeStyle(R.style.OPTSelectedText,R.style.OPTUnselectedText);
				convertView.setTag(holder);
            } 
            else 
            {
                holder = (ViewHolder)convertView.getTag();
            }
			RefreshView(parent.getContext(), position, holder);
            return convertView;
		}
       	public void RefreshView(Context context, int position, ViewHolder holder)
		{
            String Name = FindMultiString(context, selectionList.get(position).Name);
			if("mb_use_rt_player".equals(selectionList.get(position).Name)){
				if(selectionList.get(position).bool)
					Name+=" On";
				else
					Name+=" Off";
			}
			holder.tv.setText(Name);
			if (selectionList.get(position).GroupEnd == true)
			{
				holder.group_end.setVisibility(View.VISIBLE);
			}
			else
			{
				holder.group_end.setVisibility(View.INVISIBLE);
				holder.group_end.setMaxHeight(0);
			}
		}
        private String FindMultiString(Context context, String KeyName)
        {
            String OKName;
				if(KeyName.equals("mb_select_dir"))
					OKName="Select Dir";
				else if(KeyName.equals("mb_select_file"))
					OKName="Select File";
                else if (KeyName.equals("mb_thumbnailview"))
                    OKName=context.getString(R.string.mb_thumbnailview);
                else if (KeyName.equals("mb_preview"))
                    OKName=context.getString(R.string.mb_preview);
                else if (KeyName.equals("mb_all_video"))
                    OKName=context.getString(R.string.mb_all_video);
                else if (KeyName.equals("mb_dvd_bd"))
                    OKName=context.getString(R.string.mb_dvd_bd);
                else if (KeyName.equals("mb_date_view"))
                    OKName=context.getString(R.string.mb_date_view);
                else if (KeyName.equals("mb_all_photo"))
                    OKName=context.getString(R.string.mb_all_photo);
                else if (KeyName.equals("mb_sort_by_name_a_z"))
                    OKName=context.getString(R.string.mb_sort_by_name_a_z);
                else if (KeyName.equals("mb_sort_by_name_z_a"))
                    OKName=context.getString(R.string.mb_sort_by_name_z_a);
                else if (KeyName.equals("mb_recently_play"))
                    OKName=context.getString(R.string.mb_recently_play);
                else if (KeyName.equals("mb_recently_add_new_old"))
                    OKName=context.getString(R.string.mb_recently_add_new_old);
                else if (KeyName.equals("mb_recently_add_old_new"))
                    OKName=context.getString(R.string.mb_recently_add_old_new);
				else if(KeyName.equals("mb_use_rt_player"))
					OKName = "Use RT Player";
                else
                    OKName = KeyName;
            return OKName;
        }
	}
}
