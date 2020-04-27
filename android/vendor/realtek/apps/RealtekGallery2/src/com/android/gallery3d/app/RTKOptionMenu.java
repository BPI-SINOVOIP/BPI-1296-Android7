package com.android.gallery3d.app;


import java.io.IOException;
import org.xmlpull.v1.XmlPullParserException;
import com.android.gallery3d.ui.MarqueeTextView;
import android.widget.PopupWindow;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.content.Context;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;
import android.content.res.XmlResourceParser;
import java.util.ArrayList;
import java.util.Iterator;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.ImageView;
import android.widget.TextView;
import com.android.gallery3d.R;

import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.AdapterView.OnItemClickListener;
import android.view.View.OnKeyListener;
import android.view.View.OnTouchListener;
import android.view.MotionEvent;
import android.graphics.drawable.BitmapDrawable;

public class RTKOptionMenu extends PopupWindow
{
    private static final String TAG = "RTKOptionMenu";
    private boolean DBG = false;
    private PopupWindow m_PopWindow = null;
    private View m_Popview = null;
    private ListView m_ListView = null;
    private OptionMenuListAdapter m_ListAdapter = null;
    private int m_Width;
    private int m_Height;
    private int m_x;
    private int m_y;
    private PopupWindow.OnDismissListener m_Listener=null;
    //Should call those API sequentially
    // Construct -->SetDefaultFocis-->setItemClickListener-->Show-->Hide-->Show
    public RTKOptionMenu(int X, int Y, int Width, int Height, XmlResourceParser in, View ContentView, PopupWindow.OnDismissListener listener)
    {
        super(ContentView, Width, Height);
        m_x = X;
        m_y = Y;
        m_Width = Width;
        m_Height = Height;
        m_Popview = ContentView;
        m_Listener = listener;
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
        super.setTouchInterceptor(new OnTouchListener()
        {

            public boolean onTouch(View view, MotionEvent event)
            {

                if (event.getAction() == MotionEvent.ACTION_OUTSIDE)
                {

                    Hide();
                    return true;
                }

                return false;
            }
        });

        // set onDismissListener
        if(m_Listener!=null)
            setOnDismissListener(m_Listener);
        m_ListView.setSelection(0);
    }
/*
    //return value
    //	true: find the item outside desirded to modifiy
    //  false: Can not find the item
    public boolean setDefaultFocus(String Str, boolean focus)
    {
        int i;
        for (i=0; i<m_ListAdapter.selectionList.size(); i++)
        {
            if (Str == m_ListAdapter.selectionList.get(i).Name)
            {
                m_ListAdapter.selectionList.get(i).Selected = focus;
                m_ListAdapter.notifyDataSetChanged();
                return true;
            }
        }
        return false;
    }
*/
    public void setItemClickListener(OnItemClickListener listener)
    {
        m_ListView.setOnItemClickListener(listener);
    }

    public void setListAdapter(XmlResourceParser in)
    {
        m_ListAdapter = new OptionMenuListAdapter(in);
        m_ListView.setAdapter(m_ListAdapter);
    }

    public void setOnKeyListener(OnKeyListener listener)
    {
        m_ListView.setOnKeyListener(listener);
    }

    public void Show()
    {
        if(DBG) Log.d(TAG,"Show option menu");
        super.setFocusable(true);
        super.setOutsideTouchable(true);
        super.showAtLocation(m_Popview, Gravity.LEFT| Gravity.TOP, m_x, m_y);
        m_ListView.requestFocusFromTouch();
    }

    public void Hide()
    {
        if(DBG) Log.d(TAG,"Hide option menu");
        super.setFocusable(false);
        super.setOutsideTouchable(false);
        super.dismiss();
    }
    public String getItemof(int position)
    {
        if(position<0)
        {
            return null;
        }
        return m_ListAdapter.getItemName(position);
    }

    public void setSelectionList(ArrayList<String> list)
    {
        if (m_ListAdapter != null)
            m_ListAdapter.setSelectionList(list);
    }

    public int getSelectedItemPosition()
    {
        return m_ListView.getSelectedItemPosition();

    }

    public void refreshItem(int position, String str)
    {
        if(m_ListAdapter!=null && m_ListView!=null)
        {
            m_ListAdapter.RefreshItem(position, str);
            m_ListAdapter.notifyDataSetInvalidated();
            m_ListView.setSelection(position);
        }
    }

    static public String FindMultiString(Context context, String KeyName)
    {
        String OKName;
        if (KeyName.equals("Slideshow"))
            OKName=context.getString(R.string.slideshow);
        else if (KeyName.equals("Slideshow timing"))
            OKName=context.getString(R.string.slideshow_timing);
        else if (KeyName.equals("Slideshow repeat mode"))
            OKName=context.getString(R.string.slideshow_repeat_mode);
        else if (KeyName.equals("Background music"))
            OKName=context.getString(R.string.background_music);
        else if (KeyName.equals("EXIF info"))
            OKName=context.getString(R.string.exif_info);
        else if (KeyName.equals("Subtitle"))
            OKName=context.getString(R.string.subtitle);
        else if (KeyName.equals("Audio"))
            OKName=context.getString(R.string.audio);
        else if (KeyName.equals("3D"))
            OKName=context.getString(R.string.movie_3d);
        else if (KeyName.equals("3Dto2D"))
            OKName=context.getString(R.string.movie_3dto2d);
        else if (KeyName.equals("Video RAW"))
            OKName=context.getString(R.string.video_raw);
        else if (KeyName.equals("Aspect Ratio"))
            OKName=context.getString(R.string.aspect_ratio);
        else if (KeyName.equals("OFF"))
            OKName=context.getString(R.string.setting_off);
        else if (KeyName.equals("Offset"))
            OKName=context.getString(R.string.offset);
        else if (KeyName.equals("Size"))
            OKName=context.getString(R.string.size);
        else if (KeyName.equals("Sync"))
            OKName=context.getString(R.string.sync);
        else if (KeyName.equals("Text Encoding"))
            OKName=context.getString(R.string.text_encoding);
        else if (KeyName.equals("Color"))
            OKName=context.getString(R.string.color);
        else if (KeyName.equals("White"))
            OKName=context.getString(R.string.white);
        else if (KeyName.equals("Yellow"))
            OKName=context.getString(R.string.yellow);
        else if (KeyName.equals("Green"))
            OKName=context.getString(R.string.green);
        else if (KeyName.equals("Black"))
            OKName=context.getString(R.string.black);
        else if (KeyName.equals("Red"))
            OKName=context.getString(R.string.red);
        else if (KeyName.equals("Blue"))
            OKName=context.getString(R.string.blue);
        else if (KeyName.equals("Root Menu"))
            OKName=context.getString(R.string.root_menu);
        else if (KeyName.equals("Title Menu"))
            OKName=context.getString(R.string.title_menu);
        else if (KeyName.equals("BD_Title"))
            OKName=context.getString(R.string.bd_title_info);
        else if (KeyName.equals("BD_Chapter"))
            OKName=context.getString(R.string.bd_chapter_info);
        else if (KeyName.equals("Transition effect"))
            OKName=context.getString(R.string.transition_effect);
        else if (KeyName.equals("Property"))
            OKName=context.getString(R.string.property);
        else if (KeyName.startsWith("Save Subtitle"))
        {
            OKName=context.getString(R.string.save_subtitle);
            if(KeyName.endsWith("On"))
            {
                OKName+=": " + context.getString(R.string.setting_on);
            }
            else if(KeyName.endsWith("Off"))
            {
                OKName+=": " + context.getString(R.string.setting_off);
            }
        }
        else
            OKName = KeyName;
        return OKName;
    }

    public void setSelectedItem(int position)
    {
        m_ListView.requestFocusFromTouch();
        m_ListView.setSelection(position);
    }

    //OptionMenuListAdapter
    class OptionMenuListAdapter extends BaseAdapter
    {
        private XmlPullParserFactory factory;
        private XmlPullParser mXpp;
        ArrayList<selection> selectionList = null;

        class selection
        {
            String Name;
        }

        OptionMenuListAdapter(XmlResourceParser in)
        {
            if(DBG) Log.d(TAG,"OptionMenuListAdapter");
            try
            {
                factory = XmlPullParserFactory.newInstance();
                mXpp = in;
                if (mXpp != null)
                {
                    ParseXML();
                    printList();
                }
            }
            catch (XmlPullParserException e)
            {
                e.printStackTrace();
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }
        }

        public void setSelectionList(ArrayList<String> list)
        {
            selectionList = new ArrayList<selection>();
            for (int i = 0; i < list.size(); i++)
            {
                selection item = new selection();
                item.Name = list.get(i);
                selectionList.add(item);
            }
            notifyDataSetChanged();
        }

        private void ParseXML()throws XmlPullParserException,IOException
        {
            if(DBG) Log.d(TAG,"ParseXML");
            selection item = null;
            int eventType = mXpp.getEventType();
            if(DBG) Log.d(TAG,"ParseXML init eventType="+eventType);
            while (eventType != XmlPullParser.END_DOCUMENT)
            {
                String ItemName = null;
                String GroupName = null;
                String Name = null;
                switch(eventType)
                {
                    case XmlPullParser.START_DOCUMENT:
                        selectionList = new ArrayList<selection>();
                        break;
                    case XmlPullParser.START_TAG:
                        Name = mXpp.getName();
                        if(Name.equals("Item"))
                        {
                            item = new selection();

                            int attr_count = mXpp.getAttributeCount();
                            int i;
                            for (i=0; i<attr_count; i++)
                            {
                                if (mXpp.getAttributeName(i).equals("Name"))
                                {
                                    item.Name = mXpp.getAttributeValue(i);
                                }
                                else
                                {
                                    if(DBG) Log.d(TAG,"Got unknow item attribute:"+mXpp.getAttributeName(i));
                                }
                            }
                            if(DBG) Log.d(TAG,"ParseXML Get New Item: " );
                            selectionList.add(item);
                        }
                        break;
                    case XmlPullParser.END_TAG:
                        Name = mXpp.getName();
                        if(DBG) Log.d(TAG,"END_TAG Name: "+Name);
                        if(Name.equals("Item"))
                        {
                        }
                        break;
                }
                eventType = mXpp.next();
                if(DBG) Log.d(TAG,"ParseXML next eventType="+eventType);
            }
        }
        private void printList()
        {
            if(DBG) Log.d(TAG,"printList");
            Iterator<selection> it = selectionList.iterator();
            while(it.hasNext())
            {
                selection CurItem = it.next();
                if(DBG) Log.d(TAG," Name: "+CurItem.Name );
            }
        }
        //Wen: Update specific item
        @Override
        public int getCount()
        {
            if (selectionList == null)
                return 0;
//            if(DBG) Log.d(TAG,"getCount: "+selectionList.size());
            return selectionList.size();
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
        public String getItemName(int position)
        {
            return selectionList.get(position).Name;
        }
        class ViewHolder
        {
            ImageView selection_img;
            MarqueeTextView tv;
        }
        @Override
        public View getView(int position, View convertView, ViewGroup parent)
        {
            if (selectionList == null)
                return null;
            ViewHolder holder;
//            if(DBG) Log.d(TAG,"getView position :"+position);
            if (convertView == null)
            {
                if(DBG) Log.d(TAG,"getview:convertView==null :"+position);
                // if it's not recycled, initialize some attributes
                Context context = parent.getContext();
                LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                convertView = inflater.inflate(R.layout.item_of_option_menu, null);
                holder = new ViewHolder();
                holder.selection_img = (ImageView)(convertView.findViewById(R.id.selection_img));
                holder.tv = (MarqueeTextView)(convertView.findViewById(R.id.selection_text));
                holder.tv.setSelectedChangeStyle(R.style.OPTSelectedText, R.style.OPTUnselectedText);
                holder.tv.setParentListView(m_ListView);
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
            if (selectionList == null)
                return;
            String Name = FindMultiString(context, selectionList.get(position).Name);
            holder.tv.setText(Name);
        }

        public void RefreshItem(int position, String str)
        {
            if(selectionList!=null)
            {
                selectionList.get(position).Name = str;
            }
        }

    }
}
