package com.example.hdmirxdemo;

import android.content.Context;
import android.widget.BaseAdapter;
import android.view.View;
import android.view.ViewGroup;
import android.view.LayoutInflater;
import android.util.Log;

import java.util.List;
import java.util.Map;

public class SchRecAdapter extends BaseAdapter {

    private static final boolean DEBUG=false;
    private static final String TAG="SchRecAdapter";

    List<SchRecItem2> mItemList;
    private LayoutInflater mInflater;
    private Context mContext;

    public SchRecAdapter(Context context, List<SchRecItem2> itemList) {
        mContext = context;
        mInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        mItemList = itemList;
    }

    @Override
    public int getCount(){
        return mItemList.size();
    }

    @Override
    public Object getItem(int position){
        return mItemList.get(position);
    }

    @Override
    public long getItemId(int position){
        return 0;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent){

        if(DEBUG){
            Log.d(TAG, "getView position:"+position+" view:"+convertView+" parent:"+parent);
        }

        SchRecItem2 item = mItemList.get(position);
        View rst = null;
        if(convertView == null){
            SchRecListViewItem view = (SchRecListViewItem) mInflater.inflate(R.layout.sch_rec_list_view_item, null);
            view.setParameters(item.getBundle());
            ViewHolder vh = new ViewHolder();
            vh.mView = view;
            view.setTag(vh);
            rst = view;
        }else{
            if(convertView instanceof SchRecListViewItem){
                //Log.i(TAG, "convertView is a SchRecListViewItem");
            }
            ViewHolder vh = (ViewHolder) convertView.getTag();
            vh.mView.setParameters(item.getBundle());
            rst = convertView;
        }
        //((SchRecListViewItem)rst).updateButton(false);
        return rst;
    }

    class ViewHolder {
        SchRecListViewItem mView;
    }
}
