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

public class RTKVScrollMenu extends PopupWindow
{
    private static final String TAG = "RTKVScrollMenu";
    private boolean DBG = false;
    private PopupWindow m_PopWindow = null;
    private View m_Popview = null;
    private ListView m_ListView = null;
    private TextView mTitle;
    private OptionMenuListAdapter m_ListAdapter = null;
    private int m_Width;
    private int m_Height;
    private int mGravity = Gravity.CENTER;
    public int m_x;
    public int m_y;
    //Should call those API sequentially
    // Construct -->SetDefaultFocis-->setItemClickListener-->Show-->Hide-->Show
    public RTKVScrollMenu(int X, int Y, int Width, int Height, String title, XmlResourceParser in, View ContentView)
    {
        super(ContentView, Width, Height);
        m_x = X;
        m_y = Y;
        m_Width = Width;
        m_Height = Height;
        m_Popview = ContentView;
        m_ListView = (ListView)m_Popview.findViewById(R.id.vscroll_list);
        mTitle = (TextView)m_Popview.findViewById(R.id.menu_title);
        String titleName = RTKOptionMenu.FindMultiString(ContentView.getContext(), title);
        mTitle.setText(titleName);
        mTitle.setY(0);
        m_ListAdapter = new OptionMenuListAdapter(in, this);
        m_ListView.setAdapter(m_ListAdapter);
        init();

    }

    //Pls don't call this construct via outside code, it's called by inside.
    //public RTKOptionMenu(View contentView, int width, int height)
    private void init()
    {
        super.setAnimationStyle(R.style.Animation_OnScreenHint);
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
    }

    public void setItemClickListener(OnItemClickListener listener)
    {
        m_ListView.setOnItemClickListener(listener);
    }

    public void setOnKeyListener(OnKeyListener listener)
    {
        m_ListView.setOnKeyListener(listener);
    }

    public void Show(boolean focus)
    {
        if(DBG) Log.d(TAG,"Show vscroll menu");
        setFocusable(focus);
        super.setOutsideTouchable(true);
        super.showAtLocation(m_Popview, Gravity.LEFT| Gravity.TOP, m_x, m_y);
        requestFocusFromTouch();

        View listItem = m_ListAdapter.getView(0, null, m_ListView);
        if(listItem!=null) {
            listItem.measure(0, 0);
            int itemHeight = m_ListView.getDividerHeight() + listItem.getMeasuredHeight();
            m_ListView.setSelectionFromTop(getSelectedItemPosition(), itemHeight);
        }
    }
    public void Hide()
    {
        if(DBG) Log.d(TAG,"Hide vscroll menu");
        setFocusable(false);
        super.setOutsideTouchable(false);
        super.dismiss();
    }

    public void setAlpha(float alpha)
    {
        ViewGroup bg_vscroll = (ViewGroup)m_Popview.findViewById(R.id.bg_vscroll);
        if(bg_vscroll!=null)
            bg_vscroll.setAlpha(alpha);
    }

    public String getItemof(int position)
    {
        if(position<0)
        {
            return null;
        }
        return m_ListAdapter.getItemName(position);
    }

    public void update(boolean bUpdateListView)
    {
        super.update();
    }

    public int getSelectedItemPosition()
    {
        return m_ListView.getSelectedItemPosition();

    }

    public int getCount()
    {
        if (m_ListAdapter != null)
            return m_ListAdapter.getCount();
        else
            return 0;
    }

    public void setSelectionList(ArrayList<String> list)
    {
        if (m_ListAdapter != null)
            m_ListAdapter.setSelectionList(list);
    }

    public void modifySelectionList(int loc, String selection)
    {
        if (m_ListAdapter != null)
            m_ListAdapter.modifySelectionList(loc, selection);
    }

    public void setGravity(int gravity)
    {
        mGravity = gravity;
    }

    public void requestFocusFromTouch()
    {
        m_ListView.requestFocusFromTouch();
    }

    public void setSelectedItem(int position)
    {
        m_ListView.setSelection(position);
    }

    @Override
    public void setFocusable(boolean focusable)
    {
        super.setFocusable(focusable);
        if(m_ListView!=null)
        {
            m_ListView.setFocusable(focusable);
        }
    }

    //OptionMenuListAdapter
    class OptionMenuListAdapter extends BaseAdapter
    {
        private XmlPullParserFactory factory;
        private XmlPullParser mXpp;
        ArrayList<selection> selectionList = null;
        private PopupWindow mWindow = null;

        class selection
        {
            String Name;
            boolean Selected;
        }

        OptionMenuListAdapter(XmlResourceParser in, PopupWindow window)
        {
            if(DBG) Log.d(TAG,"OptionMenuListAdapter");
            try
            {
                factory = XmlPullParserFactory.newInstance();
                mWindow = window;
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
        public void modifySelectionList(int loc, String selection)
        {
            selection item = new selection();
            item.Name = selection;
            selectionList.set(loc, item);
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
                                else if (mXpp.getAttributeName(i).equals("Focus"))
                                {
                                    item.Selected = mXpp.getAttributeValue(i).equals("true")?true:false;
                                }
                                else
                                {
                                    if(DBG) Log.d(TAG,"Got unknow item attribute:"+mXpp.getAttributeName(i));
                                }
                            }
                            if(DBG) Log.d(TAG,"ParseXML Get New Item: "+item.Name+"selected: "+item.Selected);
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
                if(DBG) Log.d(TAG," Name: "+CurItem.Name+ "selected: "+CurItem.Selected);
            }
        }
        //Wen: Update specific item
        @Override
        public int getCount()
        {
            if (selectionList == null)
                return 0;
            if(DBG) Log.d(TAG,"getCount: "+selectionList.size());
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
            if (selectionList == null || position>=selectionList.size())
            {
                return "";
            }
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
            if ( selectionList == null || position>=selectionList.size() )
                return null;
            ViewHolder holder;
            if(DBG) Log.d(TAG,"getView position :"+position);
            if (convertView == null)
            {
                if(DBG) Log.d(TAG,"getview:convertView==null :"+position);
                // if it's not recycled, initialize some attributes
                Context context = parent.getContext();
                LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                convertView = inflater.inflate(R.layout.item_of_vscroll_basic, null);
                holder = new ViewHolder();
                //holder.selection_img = (ImageView)(convertView.findViewById(R.id.selection_img));
                holder.tv = (MarqueeTextView)(convertView.findViewById(R.id.selection_text));
                holder.tv.setGravity(mGravity);
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
            if (selectionList == null || position>=selectionList.size())
                return;
            String Name = RTKOptionMenu.FindMultiString(context, selectionList.get(position).Name);
            holder.tv.setText(Name);
            holder.tv.setSelectedChangeStyle(R.style.OPTFocusSelectedText, R.style.OPTFocusUnselectedText, 0xffffffff);
            holder.tv.setSelectedChangeStyleWithoutFocus(R.style.OPTSelectedText, R.style.OPTUnselectedText, 0x00000000);
        }

    }
}
