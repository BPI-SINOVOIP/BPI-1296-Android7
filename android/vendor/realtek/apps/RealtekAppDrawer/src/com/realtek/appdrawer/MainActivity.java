package com.realtek.appdrawer;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.util.Log;

import com.realtek.app.adapter.AppAdapter;
import com.realtek.app.adapter.ViewHolder;

public class MainActivity extends Activity implements OnItemClickListener{
	
	private AppAdapter mAdapter;
	private ListView mListView;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mListView = (ListView) findViewById(R.id.app_list);
        //mAdapter = new AppAdapter(this);
        //mListView.setAdapter(mAdapter);
        //mListView.setOnItemClickListener(this);
    }

    protected void onResume (){
        super.onResume();
        Log.d("App","onResume");
        mAdapter = new AppAdapter(this);
        mListView.setAdapter(mAdapter);
        mListView.setOnItemClickListener(this);
    }
    
    @Override
	public void onItemClick(AdapterView<?> parent, 
			View view,
			int position,
			long id) {
		ViewHolder holder = (ViewHolder) view.getTag();
		ApplicationInfo app = holder.app;
		launchInstalledActivity(app.packageName, this);
	}
    
    protected void launchInstalledActivity(String packageName, Context context){
		final PackageManager packageManager = context.getPackageManager();
		Intent intent=packageManager.getLaunchIntentForPackage(packageName);
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(intent);
	}
}
