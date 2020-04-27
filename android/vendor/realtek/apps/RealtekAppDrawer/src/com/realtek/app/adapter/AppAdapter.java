package com.realtek.app.adapter;

import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.realtek.app.provider.InstalledAppInfoProvider;
import com.realtek.appdrawer.R;

public class AppAdapter extends BaseAdapter {
	
	private static final String TAG = "AppAdapter";
	
	private Context mContext;
	LayoutInflater mInflater;
	List<ResolveInfo> mData;
	private PackageManager mPackageManager;
	
	public AppAdapter(Context c){
		mContext = c;
		mPackageManager = mContext.getPackageManager();
		loadAllApps(c);
		mInflater = (LayoutInflater)c.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	}
	
	public void loadAllApps(Context c){
		
		Log.d(TAG,"loadAllApps");
		
		final Intent mainIntent = new Intent(Intent.ACTION_MAIN, null);
        mainIntent.addCategory(Intent.CATEGORY_LAUNCHER);

        mData = mPackageManager.queryIntentActivities(mainIntent, 0);
        
        if(mData!=null){
        	for(int i=0;i<mData.size();i++){
        		Log.d(TAG, "App:"+mData.get(i));
        	}
        }
	}

	@Override
	public int getCount() {
		return mData.size();
	}

	@Override
	public Object getItem(int arg0) {
		return mData.get(arg0);
	}

	@Override
	public long getItemId(int arg0) {
		return arg0;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		
		if(convertView==null){
			convertView=mInflater.inflate(R.layout.app_row, null);
			TextView text=(TextView)convertView.findViewById(R.id.text);
			ImageView image = (ImageView)convertView.findViewById(R.id.image);
			
			ApplicationInfo app = mData.get(position).activityInfo.applicationInfo;
			
			String label= (String)mPackageManager.getApplicationLabel(app);
			text.setText(label);
			
			// Icon
			Drawable icon=InstalledAppInfoProvider.getFullResIcon(app.packageName, app.icon, mContext);
			image.setImageDrawable(icon);
			
			ViewHolder holder = new ViewHolder();
			holder.text = text;
			holder.image = image;
			holder.app = app;
			convertView.setTag(holder);
		}else{
			ViewHolder holder = (ViewHolder) convertView.getTag();
			ApplicationInfo app = mData.get(position).activityInfo.applicationInfo;
			
			String label= (String)mPackageManager.getApplicationLabel(app);
			holder.text.setText(label);
			// Icon
			Drawable icon=InstalledAppInfoProvider.getFullResIcon(app.packageName, app.icon, mContext);
			holder.image.setImageDrawable(icon);
			
			// update app field
			holder.app = app;
		}
		return convertView;
	}
}