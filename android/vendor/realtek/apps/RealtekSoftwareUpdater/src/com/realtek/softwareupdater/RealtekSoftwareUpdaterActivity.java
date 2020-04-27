/*
 * Copyright (C) 2014 Realtek Semiconductor Corp.
 * Author: stanely
 * History:
 *      2014.04.16 : First draft.
 */

package com.realtek.softwareupdater;

import android.annotation.TargetApi;
import android.app.*;
import android.content.DialogInterface;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Environment;
import android.os.SystemProperties;
import android.os.AsyncTask;
import android.os.RecoverySystem;
import android.view.View;
import android.view.Window;
import android.view.KeyEvent;
import android.view.View.OnClickListener;
import android.view.View.OnHoverListener;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.ImageView;
import android.widget.ImageButton;
import android.widget.ArrayAdapter;
import android.widget.AdapterView;
import android.widget.AbsListView;
import android.widget.Toast;
import android.graphics.Color;
import android.graphics.Typeface;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AbsListView.OnScrollListener;
import android.util.Log;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiManager;
import java.util.ArrayList;
import java.io.File;
import java.io.BufferedReader;
import java.io.BufferedInputStream;
import java.io.InputStreamReader;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.lang.Runtime;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.nio.channels.FileChannel;
import java.lang.Integer;
import android.os.SystemProperties;
import com.realtek.xml.*;

/**
 *   Realtek RealtekSoftwareUpdaterActivity is used to show the OTA functionalities.
 */
public class RealtekSoftwareUpdaterActivity extends Activity implements OnScrollListener {
    public static String TAG = "RealtekSoftwareUpdaterActivity";

    public static final int MAX_ITEMS_PER_PAGE = 7;  // display 7 items per page.
    private ListView mSoftwareMenuList; 
    private static String[] mSupportSoftwareArray;
    private MenuArrayAdapter mAdapter;
    private static ImageView mListViewHeader;
    private static ImageView mListViewFooter;
    private int mCurrentFocusIdx; 
    private int mSupportSoftwareCount;
    private Handler mSWHandler;
    private ArrayList<String> mSoftwareFiles;
    //private ArrayList<String> mSoftwareFilesInFullPath;
    private ArrayList<RecoverySoftwareItem> mSoftwareFilesInFullPath;
    private Thread mThread;
    private Handler mRefreshHandler;
    public ArrayList<RSSItemField> mXMLItems;

    public static final float LOCAL_SW_VERSION = 1;  // TODO: should be changed time by time.
    public static int RECOVERY_SOFTWARE_TYPE_USB = 0;
    public static int RECOVERY_SOFTWARE_TYPE_SDCARD = 1;
    public static int RECOVERY_SOFTWARE_TYPE_OTA = 2;

	private static String mOTAXMLUrl = "http://172.21.98.109/appliance.xml";  // xml link on server side.



    /**
     *  Override this function
    */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Remove app title 
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        // Remove status bar and navigation bar

        // Below method only funcion with Android 4.0 and lower.
        //getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        // Below method only funcion with Android 4.1 and higher.
        View decorView = getWindow().getDecorView();
        int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
        decorView.setSystemUiVisibility(uiOptions);

        setContentView(R.layout.activity_software_updater);

        // header
        mListViewHeader = (ImageView) findViewById(R.id.menu_list_header_image);

        // footer
        mListViewFooter = (ImageView) findViewById(R.id.menu_list_footer_image);

	Log.d(TAG, "[OTA]rtk.vmx.boot prop: " + SystemProperties.getInt("rtk.vmx.boot",0));

        displayHeaderAndFooter();

        mSoftwareFiles = new ArrayList<String> ();
        //mSoftwareFilesInFullPath = new ArrayList<String> ();
        mSoftwareFilesInFullPath = new ArrayList<RecoverySoftwareItem> ();
        mSoftwareMenuList = (ListView) findViewById(R.id.software_menu);
        mSupportSoftwareArray = getResources().getStringArray(R.array.string_array_support_software_list);  // use pre-defined string array for display.
        mSupportSoftwareCount = mSupportSoftwareArray.length;

        mAdapter = new MenuArrayAdapter(this, R.layout.menu_list_item, mSupportSoftwareArray);
        mSoftwareMenuList.setAdapter(mAdapter);
        mSoftwareMenuList.setOnItemSelectedListener(new ListViewItemSelectedListener());
        mSoftwareMenuList.setOnItemClickListener(new ListViewItemClickListener());

        mCurrentFocusIdx = 0;

        //mSWHandler = new Handler();
        //mSWHandler.postDelayed(softwareSearchRunnable, 100);  // executed after 100ms
        doUSBSearch();
        doOTASearch();

    }

    /**
     *  Override this function
    */
    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);

        mSoftwareMenuList.setChoiceMode(ListView.CHOICE_MODE_SINGLE);
        mSoftwareMenuList.requestFocus();
        mSoftwareMenuList.setSelection(0);
        mSoftwareMenuList.setItemChecked(0, true);

        // refresh the software list
        mRefreshHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                switch(msg.what) {
                    case 0:
                        Log.d(TAG, "Inside handleMessage(0)");
                        break;
                    case 1:
                        Log.d(TAG, "Inside handleMessage(1)");
                        String[] tmp = new String[1];
                        mSupportSoftwareArray = mSoftwareFiles.toArray(tmp);
                        Log.d(TAG, "Inside handleMessage(1), size: " + mSoftwareFiles.size());

                        for(int i = 0; i < mSoftwareFiles.size(); i++) {
                            Log.d(TAG, "Inside hanldeMessage(1), f = " + mSoftwareFiles.get(i));
                        }

                        mSupportSoftwareCount = mSupportSoftwareArray.length;
                        mAdapter = new MenuArrayAdapter(getApplicationContext(), R.layout.menu_list_item, mSupportSoftwareArray);
                        mSoftwareMenuList.setAdapter(mAdapter);
                        mAdapter.notifyDataSetChanged();

                        break;
                    default:
                        break;
                }
                super.handleMessage(msg);
            }
        };

        mThread = new Thread(new Runnable() {
            private int cnt = 2;

            @Override
            public void run() {
                while(true) {
                    try {
                        Thread.sleep(500);  // sleep 500ms
                        cnt--;
                        Message msg = new Message();
                        if(cnt > 0) {
                            msg.what = 0;  // wait 500ms*2
                        } else {
                            msg.what = 1;  // done
                            mRefreshHandler.sendMessage(msg);
                            break;
                        }
                        mRefreshHandler.sendMessage(msg);
                    } catch (Exception e) {
                        e.printStackTrace(); 
                    }
                }
            }
        });
        mThread.start();
    }

    /**
     *  Runnable instance to execute the available software searching.
    */
    final Runnable softwareSearchRunnable = new Runnable() {
        public void run() {
            //search path: USB, OTA.

            // USB
            doUSBSearch();            

            // OTA 
            doOTASearch();

        }
    };

    private void doOTASearch() {
        Log.d(TAG, "[OTA]doOTASearch()");
		if(SystemProperties.getInt("rtk.vmx.boot",0)==1){
			Log.d(TAG, "[OTA]VMX");
			mOTAXMLUrl = "http://192.168.1.100/appliance.xml";  // xml link on server side.
		}
		
        new RetrieveFeedAsync().execute(mOTAXMLUrl);
    }

    private void doUSBSearch() {
        mSoftwareFiles.clear();
        mSoftwareFilesInFullPath.clear();

        File[] sSearchPath = new File("/storage").listFiles();

        // search USB
        for(File folder: sSearchPath) {
            Log.d(TAG, "folder: " + folder);

            if(folder == null || folder.isDirectory()==false) {
                continue;
            }

            File[] files = folder.listFiles();
            if(files == null || files.length == 0) {
                continue;
            }
            Log.d(TAG, "length of file: " + files.length);
                
            for(File file: files) {
                Log.d(TAG, "search file1: " + file.toString());
                //String patternZip = "(.*).zip";

				String patternZip = "(.*)rtk(.*).zip";  // simple filename checking: we expect rtk_____.zip
 
 				if(SystemProperties.getInt("rtk.vmx.boot",0)==1){
					patternZip = "(.*)install(.*).img";  // simple filename checking: we expect install_____.img			
				}
 
                if(file.isFile() && file.toString().matches(patternZip)) {   // .zip
                    //mSoftwareFiles.add(file.toString());  // full path and name
                    //mSoftwareFilesInFullPath.add(file.toString());  // full path and name
                    mSoftwareFilesInFullPath.add(new RecoverySoftwareItem(file.toString(), file.getName(), RECOVERY_SOFTWARE_TYPE_USB));  // full path and name
                    mSoftwareFiles.add("USB: " + file.getName());
                    Log.d(TAG, "search file2: " + file.toString());
               }
            }
        }

    }


    /**
     *  Override this function
    */
    @Override
    protected void onResume() {
        super.onResume();

        if(!mThread.isAlive()) {
            mThread.start();
        }
    }

    /**
     *  Override this function
    */
    @Override
    protected void onPause() {
        super.onPause();
        if(mSWHandler != null) {
            mSWHandler.removeCallbacks(softwareSearchRunnable);
        }

        if(mThread.isAlive()) {
            mThread.stop();
        }
    }

    /**
     *  Override this function in order to move the focus between listview and imageview.
    */
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {

        switch(keyCode) {
            case KeyEvent.KEYCODE_DPAD_LEFT:
                Log.d(TAG,"onKeyDown: LEFT, mCurrentFocusIdx = " + mCurrentFocusIdx);
                break;
            case KeyEvent.KEYCODE_DPAD_RIGHT:
                Log.d(TAG,"onKeyDown: RIGHT, mCurrentFocusIdx = " + mCurrentFocusIdx);
                break;
            case KeyEvent.KEYCODE_DPAD_UP:
                Log.d(TAG,"onKeyDown: UP");
                break;
            case KeyEvent.KEYCODE_DPAD_DOWN:
                Log.d(TAG,"onKeyDown: DOWN");
                return true;
            default:
                break;
        }       

        return super.onKeyDown(keyCode, event);
    }

    /**
     *  Register to monitor the state of scrolling in ListView.
    */

    @Override
    public void onScroll(AbsListView view, int firstVisibleItem, int visibleItemCount, int totalItemCount) {
        Log.d(TAG, "firstVisibleItem = " + firstVisibleItem + ", visibleItemCount = " + visibleItemCount);
        Log.d(TAG, "totalItemCount = " + totalItemCount);
        //Log.d(TAG, "onScroll, view = " + view);
    
    }

    @Override
    public void onScrollStateChanged(AbsListView view, int scrollState) {
        Log.d(TAG, "onScrollStateChanged = " + scrollState);
    }

    /**
     *  Helper function to determinie whether it's time to show header and footer image.
    */
    private void displayHeaderAndFooter() {

        // Header
        if(mCurrentFocusIdx <= 0) {  
            mListViewHeader.setVisibility(View.INVISIBLE); 
        } else {
            mListViewHeader.setVisibility(View.VISIBLE); 
        }

        // Footer
        if(mSupportSoftwareCount > RealtekSoftwareUpdaterActivity.MAX_ITEMS_PER_PAGE && mCurrentFocusIdx < mSupportSoftwareCount - 1) {
            mListViewFooter.setVisibility(View.VISIBLE); 
        } else {
            mListViewFooter.setVisibility(View.INVISIBLE); 
        }

    }

    /**
     *  Helper function to show the highlight focus on the selected item on listview when user moving the focus to 
     *  right NextIcon.
     *  Parameter:
     *      highlight: 
     *          on = highlight
     *          off = no highlight
    */
    private boolean displayHighlightInItem(boolean highlight) {
        int idx = mCurrentFocusIdx - mSoftwareMenuList.getFirstVisiblePosition();
        View v = mSoftwareMenuList.getChildAt(idx);  // idx is visible index on the listview which displayed currently.

        if(v == null)
            return false;

        TextView t = (TextView) v.findViewById(R.id.display_item);

        if(t == null)
            return false;

        if(highlight) {  // highlight
            t.setBackgroundResource(R.drawable.list_bg_focus);
            t.setWidth(1086);
        } else {  // no highlight
            t.setBackgroundResource(R.drawable.list_bg_none);
            t.setWidth(934);
            mAdapter.notifyDataSetChanged();
        }
        return true;
    }

    /**
     *  Helper class to change the focus/unfocus image for each item.
    */
    private class ListViewItemSelectedListener implements OnItemSelectedListener {

        @Override
        public void onItemSelected(AdapterView<?> parent, View view, int position , long id)
        {
            Log.d(TAG,"onItemSelected:" + position);
            //Log.d(TAG,"onItemSelected, getFirstVisiblePosition:" + mSoftwareMenuList.getFirstVisiblePosition());
            //Log.d(TAG,"onItemSelected, getChildCount:" + mSoftwareMenuList.getChildCount());
            mCurrentFocusIdx = position;
            displayHeaderAndFooter();  // keep updating
        }

        @Override
        public void onNothingSelected(AdapterView<?> parent)
        {
            Log.d(TAG, "onNothingSelected:");
        }
    }

    /**
     *  Helper class to monitor the item click event.
    */
    private class ListViewItemClickListener implements OnItemClickListener {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            Log.d(TAG, "onItemClick, position = " + position + ", id = " + id);
            final File dst;
			
			boolean needReboot=true;

            mCurrentFocusIdx = position;

            // write recovery file to factory
            try {
                    // Here we take prefix as indicator, the logic section inside RecoverySystem.installPackage
                    // will read the prefix in order to know the source from which medium.        
                    if(mSoftwareFilesInFullPath.get(position).getType() == RECOVERY_SOFTWARE_TYPE_USB) {
						if(SystemProperties.getInt("rtk.vmx.boot",0)==1){
							dst = new File("/cache/" + mSoftwareFilesInFullPath.get(position).getShortFilename());
						}else{
							dst = new File("/usb/" + mSoftwareFilesInFullPath.get(position).getShortFilename());
						}
                    } else if(mSoftwareFilesInFullPath.get(position).getType() == RECOVERY_SOFTWARE_TYPE_OTA) {
                        dst = new File("/cache/" + mSoftwareFilesInFullPath.get(position).getShortFilename());
                    } else if(mSoftwareFilesInFullPath.get(position).getType() == RECOVERY_SOFTWARE_TYPE_SDCARD) {
                        dst = new File("/sdcard/" + mSoftwareFilesInFullPath.get(position).getShortFilename());
                    } else {
                        Log.d(TAG, "Unknown update.zip source type.");
                        dst = new File("/unknown/" + mSoftwareFilesInFullPath.get(position).getShortFilename());
                    }


                if(mSoftwareFilesInFullPath.get(position).getType() == RECOVERY_SOFTWARE_TYPE_OTA) {  // start downloading file from server
                    new DownloadFileFromURL().execute(mSoftwareFilesInFullPath.get(position).getFilename(), mSoftwareFilesInFullPath.get(position).getShortFilename());
                }

				if(SystemProperties.getInt("rtk.vmx.boot",0)==1){
					needReboot=false;
					if(mSoftwareFilesInFullPath.get(position).getType() == RECOVERY_SOFTWARE_TYPE_USB) {  // start copy file from USB
                    new CopyFileFromUSB().execute(mSoftwareFilesInFullPath.get(position).getFilename(), mSoftwareFilesInFullPath.get(position).getShortFilename());
                }
				}
				
                if(mSoftwareFilesInFullPath.get(position).getType() != RECOVERY_SOFTWARE_TYPE_OTA && needReboot) {  //  OTA path we will reboot once the downloading is done.
                    AlertDialog.Builder builder = new AlertDialog.Builder(RealtekSoftwareUpdaterActivity.this);
                    builder.setMessage(String.format(getString(R.string.confirm_message), dst));
                    builder.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int id) {
                            Log.d(TAG, "Go upgrade, dst = " + dst);
                            //go upgrade
                            new AsyncTask<Void, Void, Void>() {
                                ProgressDialog progressDialog = null;
                                @Override
                                protected Void doInBackground(Void... params) {
                                    try {
                                        RecoverySystem.installPackage(getApplicationContext(), dst);  // will reboot
                                    } catch (Exception e){Log.d(TAG, "Error: " + e);}
                                    return null;
                                }

                                @Override
                                protected void onPreExecute() {
                                    //show progress bar
                                    progressDialog = new ProgressDialog(RealtekSoftwareUpdaterActivity.this);
                                    progressDialog.setMessage(getString(R.string.wait_reboot));
                                    progressDialog.setCancelable(false);
                                    progressDialog.show();
                                }

                                @Override
                                protected void onPostExecute(Void result) {
                                    if (progressDialog != null)
                                        progressDialog.dismiss();
                                }
                            }.execute((Void)null);
                        }
                    });
                    builder.setNegativeButton(android.R.string.cancel, new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int arg1) {
                            //cancel upgrade
                            Log.d(TAG, "Cancel upgrade ...");
                        }
                    });
                    builder.show();
                }
                
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private class CopyFilesTask extends AsyncTask<File, Integer, Long> {

        protected Long doInBackground(File... files) {
            Log.d(TAG, "CopyFilesTask - doInBackground()");
            File src = files[0];
            File dst = files[1];

            try {
                copyFile(src, dst);
            } catch (IOException e) {
                e.printStackTrace();
            }

            return 1L;
        }

        protected void onProgressUpdate(Integer... progress) {
            Log.d(TAG, "CopyFilesTask - onProgressUpdate(), progress = " + progress[0]);
        }

        protected void onPostExecute(Long result) {
            Log.d(TAG, "CopyFilesTask - onPostExecute");
        }

        public void copyFile(File src, File dst) throws IOException {
            FileInputStream inStream = new FileInputStream(src);
            FileOutputStream outStream = new FileOutputStream(dst);
            FileChannel inChannel = inStream.getChannel();
            FileChannel outChannel = outStream.getChannel();
            inChannel.transferTo(0, inChannel.size(), outChannel);
            inStream.close();
            outStream.close();
        }    
    }

    public class RecoverySoftwareItem {
        private String mFilename;
        private String mShortFilename;
        private int mType;

        public RecoverySoftwareItem(String name, String shortname, int type) {
            mFilename = name;
            mShortFilename = shortname;
            mType = type;
        }

        public void setFilename(String name) {
            mFilename = name;
        }

        public void setShortFilename(String name) {
            mShortFilename = name;
        }

        public void setType(int type) {
            mType = type;
        }

        public String getFilename() {
            return mFilename;
        }

        public String getShortFilename() {
            return mShortFilename;
        }

        public int getType() {
            return mType;
        }
    }

    public void prepareOTAItem() {
        RecoverySoftwareItem item;

        for(int i = 0; i < mXMLItems.size(); i++) {
            if(mXMLItems.get(i) == null) 
                continue;

            Log.d(TAG, "OTA search file1: " + mXMLItems.get(i).swURL + ": " + mXMLItems.get(i).type);

			String patternZip = "(.*)rtk(.*).zip";  // simple filename checking: we expect rtk_____.zip
			
			if(SystemProperties.getInt("rtk.vmx.boot",0)==1){
				patternZip = "(.*)install(.*).img";  // simple filename checking: we expect install_____.img
			}

            String[] tmpShortFilename = mXMLItems.get(i).swURL.split("/");  // http://172.21.85.211:8080/rtk-update_incremental_package.zip
 
            if(mXMLItems.get(i).swURL.matches(patternZip) && compareNewerVersion(mXMLItems.get(i))) {   // .zip
                item = new RecoverySoftwareItem(mXMLItems.get(i).swURL, tmpShortFilename[3], RECOVERY_SOFTWARE_TYPE_OTA);  // full path and name
                mSoftwareFilesInFullPath.add(item);
                mSoftwareFiles.add("OTA: " + tmpShortFilename[3]);
                Log.d(TAG, "OTA search file2: " + tmpShortFilename[3]);
           }
        }
    }

    /*
     * Rule: Find the oldVersion of software available on the server side, then compare the newVersion > oldVersion, if so this is the newer software.
    */
    public boolean compareNewerVersion(RSSItemField item) {
        if(LOCAL_SW_VERSION == Float.parseFloat(item.oldVersion) && Float.parseFloat(item.newVersion) > Float.parseFloat(item.oldVersion)) {
            return true;
        } else {
            return false;
        }
    }

    public void RetrieveFeedCallback() {
        Log.d(TAG, "RetrieveFeedCallback()");
        prepareOTAItem();
    }

    public class RetrieveFeedAsync extends AsyncTask<String, Integer, String> {
        public ArrayList<RSSItemField> item;

        @Override
        protected String doInBackground(String... params) {
            HttpURLConnection connection = null;
            InputStream input = null;
            String content = "";
            
            try {
                URL url = new URL(params[0]);
                connection = (HttpURLConnection) url.openConnection();
                connection.setRequestMethod("GET");
                connection.setConnectTimeout(15*1000);
                connection.setReadTimeout(15*1000);
                connection.connect();
                
                if (connection.getResponseCode() != HttpURLConnection.HTTP_OK) {
                    Log.d(TAG, "Server returned HTTP " + connection.getResponseCode()
                            + " " + connection.getResponseMessage());
                    return null;
                }
                input = new BufferedInputStream(connection.getInputStream());
                byte data[] = new byte[8192];
                int count;
                if ((count = input.read(data)) != -1) {
                    content = new String(data, 0, count);   // get xml file now
                }

                // parsing
                XmlParser p = new XmlParser();
                item = p.parseXmlResponse(content);
                Log.d(TAG, "Ending!!!" + "size = " + item.size());
            }
            catch (Exception e) {
                Log.d(TAG, "err:"+e.getMessage());
                return null;
            }
            finally {
                try {
                    if (input != null)
                        input.close();
                }
                catch (IOException ignored) {
                }
                if (connection != null)
                    connection.disconnect();
                if(item != null) { 
                    for(int i = 0; i < item.size(); i++) {
                        if(item.get(i) == null) continue;
                        if(item.get(i).product != null) Log.d(TAG, item.get(i).product);
                        if(item.get(i).type != null) Log.d(TAG, item.get(i).type);
                        if(item.get(i).oldVersion != null) Log.d(TAG, item.get(i).oldVersion);
                        if(item.get(i).newVersion != null) Log.d(TAG, item.get(i).newVersion);
                        if(item.get(i).region != null) Log.d(TAG, item.get(i).region);
                        if(item.get(i).swURL != null) Log.d(TAG, item.get(i).swURL);
                        if(item.get(i).swReleaseNotes != null) Log.d(TAG, item.get(i).swReleaseNotes);
                    }
                }
            }
          
            return "SUCCESS";
        }

        @Override
        protected void onPostExecute(String result) {
            super.onPostExecute(result);  

            if(item != null) {
                mXMLItems = item;
                RetrieveFeedCallback();  // to notify the job is done.
            }
        }

        @Override
        protected void onProgressUpdate(Integer... values) {
            super.onProgressUpdate(values);
        }   
    }
    
    public class DownloadFileFromURL extends AsyncTask<String, String, String> {
        ProgressDialog progressDialog;
        int progressValue;
//        int displayLogControl = 5;
        File dst;

        @Override
        protected String doInBackground(String... f_url) {
            int count;
            try {
                URL url = new URL(f_url[0]);
                URLConnection conection = url.openConnection();
                conection.connect();

                int lenghtOfFile = conection.getContentLength();
                Log.d(TAG, "downloading length of file: " + lenghtOfFile);

                // download the file
                InputStream input = new BufferedInputStream(url.openStream(), 8192);

                // output stream
                Log.d(TAG, "downloading filename: " + "/cache/" + f_url[1]);
                OutputStream output = new FileOutputStream("/cache/" + f_url[1]);
                dst = new File("/cache/" + f_url[1]);

                byte data[] = new byte[1024];

                long total = 0;

                while ((count = input.read(data)) != -1) {
                    total += count;
                    progressValue = (int) ((total * 100) / lenghtOfFile);

//                    if(progressValue % displayLogControl == 0)
//                        Log.d(TAG, "downloading progress: " + progressValue);

                    // writing data to file
                    output.write(data, 0, count);

                    // show progress
                    progressDialog.setProgress(progressValue);
                }

                // flushing output
                output.flush();

                // closing streams
                output.close();
                input.close();

            } catch (Exception e) {
                Log.e("Error: ", e.getMessage());
                e.printStackTrace();
            }

            return null;
        }

        @Override
        protected void onPreExecute() {
            progressDialog = new ProgressDialog(RealtekSoftwareUpdaterActivity.this, 0);
            //progressDialog.show(RealtekSoftwareUpdaterActivity.this, "ProgressDialog", "Wait!");
            progressDialog.setTitle(R.string.software_downloading);
            progressDialog.setCancelable(true);  // could be cancel by RETURN key.
            progressDialog.setMax(100);
            progressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
            progressDialog.show();
            progressValue = 0;
        }


        @Override
        protected void onPostExecute(String result) {
            super.onPostExecute(result);  
            progressDialog.dismiss();

            try {
                if(progressValue != 0) {
                    Thread.sleep(1000);
                    RecoverySystem.installPackage(getApplicationContext(), dst);  // For OTA case, we call reboot here.
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
        }
    }

		public class CopyFileFromUSB extends AsyncTask<String, String, String> {
        ProgressDialog progressDialog;
        int progressValue;
        File dst;

        @Override
        protected String doInBackground(String... f_path) {        
        int count;
        try{
            File f = new File(f_path[0]);
            long lengthOfFile = f.length();
			
            // input file
            Log.d(TAG, "input file: " + f_path[0]);
            InputStream input = new FileInputStream(f_path[0]);

            //out file
            Log.d(TAG, "out filename: " + "/cache/"+f_path[1]);


            OutputStream output = new FileOutputStream("/cache/" +f_path[1]);
            dst = new File("/cache/" + f_path[1]);

            byte data[] = new byte[1024];

            long total =0;
            while((count=input.read(data))!=-1){
                total += count;
                progressValue = (int) ((total * 100) / lengthOfFile);
                output.write(data,0,count);
                progressDialog.setProgress(progressValue);			
           }
            // flushing output
            output.flush();
            // closing streams
            output.close();
            input.close();

            } catch (Exception e) {
                Log.e("Error: ", e.getMessage());
                e.printStackTrace();
            }
        return null;
          
        }
        @Override
        protected void onPreExecute() {
            progressDialog = new ProgressDialog(RealtekSoftwareUpdaterActivity.this, 0);
            //progressDialog.show(RealtekSoftwareUpdaterActivity.this, "ProgressDialog", "Wait!");
            progressDialog.setTitle(R.string.software_downloading);
            progressDialog.setCancelable(true);  // could be cancel by RETURN key.
            progressDialog.setMax(100);
            progressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
            progressDialog.show();
            progressValue = 0;
        }

        @Override
        protected void onPostExecute(String result) {
            super.onPostExecute(result);  
            progressDialog.dismiss();

            try {
                if(progressValue != 0) {
                    Thread.sleep(1000);
                    RecoverySystem.installPackage(getApplicationContext(), dst);  // For OTA case, we call reboot here.
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
        }
		
    }

	
 }
