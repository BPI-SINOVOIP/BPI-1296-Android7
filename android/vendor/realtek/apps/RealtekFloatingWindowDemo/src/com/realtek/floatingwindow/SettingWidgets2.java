package com.realtek.floatingwindow;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;

import android.widget.LinearLayout;
import android.content.Context;
import android.util.AttributeSet;
import android.widget.Button;
import android.widget.Toast;
import android.widget.TextView;
import android.widget.Spinner;
import android.widget.SimpleAdapter;
import android.widget.CheckBox;
import android.widget.ArrayAdapter;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.os.Bundle;
import android.view.View;
import android.webkit.MimeTypeMap;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;

import android.util.Log;

public class SettingWidgets2 extends LinearLayout {

    private static final String TAG     = "SettingWidgets2";
    private static final String sRoot   = "/storage";

    public static final String KEY_ENABLED          = "enabled";
    public static final String KEY_RTMEDIAPLAYER    = "rtmediaplayer";
    public static final String KEY_FILE_PATH        = "filepath";
    public static final String KEY_VIEWTYPE         = "viewType";
    public static final String KEY_SIZE             = "windowSize";

    public static final int TYPE_SURFACEVIEW    = 0;
    public static final int TYPE_TEXTUREVIEW    = 1;

    public static final int WINDOW_HD = 1;
    public static final int WINDOW_SD = 0;

    /*
    public static final String KEY_RECORD       = "record";
    public static final String KEY_RECORD_PATH  = "recordPath";

    public static final int TYPE_NOPREVIEW      = 2;

    private boolean bCanRecord;
    private String mToken;
    */

    private CheckBox mSourceCheckBox;
    private CheckBox mRTMediaPlayerCheckBox;
    private Button mFileButton;
    private Spinner mViewTypeSpinner;
    private Spinner mWindowSizeSpinner;

    /*
    private CheckBox mRecordCheckBox;
    */

    private TextView mInfoTextView;

    private Context mContext;

    private String mPlayFilePath="";

    // file selector related..
    private ArrayList<String> rootPath = new ArrayList<>();

    public SettingWidgets2(Context context){
        super(context);
        mContext = context;
    }

    public SettingWidgets2(Context context, AttributeSet attrs){
        super(context, attrs);
        mContext = context;
    }

    public SettingWidgets2(Context context, AttributeSet attrs, int defStyleAttr){
        super(context, attrs, defStyleAttr);
        mContext = context;
    }

    public SettingWidgets2(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes){
        super(context, attrs, defStyleAttr, defStyleRes);
        mContext = context;
    }

    public void init(){

        mSourceCheckBox = (CheckBox) findViewById(R.id.source_checkbox);
        mRTMediaPlayerCheckBox = (CheckBox) findViewById(R.id.rtmediaplayer_checkbox);
        mViewTypeSpinner = (Spinner) findViewById(R.id.view_type_spinner);
        mWindowSizeSpinner = (Spinner) findViewById(R.id.window_size_spinner);
        mFileButton = (Button) findViewById(R.id.file_select_button);
        mInfoTextView = (TextView) findViewById(R.id.info_textview);

        mInfoTextView.setText("Info:");

        mRTMediaPlayerCheckBox.setText("Use RTMediaPlayer");
        mSourceCheckBox.setText("File playback");
        mFileButton.setOnClickListener(new ButtonClickListener());

        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
                mContext,
                R.array.view_type_item,
                android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mViewTypeSpinner.setAdapter(adapter);

        adapter = ArrayAdapter.createFromResource(
                mContext,
                R.array.window_size_item_2,
                android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mWindowSizeSpinner.setAdapter(adapter);
    }

    public void setInfoText(String text){
        if(text == null) {text = "";}
        mInfoTextView.setText("Info: "+text);
    }

    public void setLabelText(String sourceText /*, String recordText*/ ){
        mSourceCheckBox.setText(sourceText);
    }

    public void setEnabled(boolean enable){
        super.setEnabled(enable);

        mSourceCheckBox.setEnabled(enable);
        mRTMediaPlayerCheckBox.setEnabled(enable);
        mFileButton.setEnabled(enable);
        mViewTypeSpinner.setEnabled(enable);
        mWindowSizeSpinner.setEnabled(enable);
    }

    public Bundle getSettingConfig(){
        Bundle bundle = new Bundle();

        boolean enabled = mSourceCheckBox.isChecked();
        boolean rtEnabled = mRTMediaPlayerCheckBox.isChecked();
        int viewType = mViewTypeSpinner.getSelectedItemPosition();
        int windowSize = mWindowSizeSpinner.getSelectedItemPosition();

        if(mPlayFilePath==null)
            mPlayFilePath="";

        bundle.putBoolean(KEY_ENABLED,enabled);
        bundle.putBoolean(KEY_RTMEDIAPLAYER,rtEnabled);
        bundle.putInt(KEY_VIEWTYPE,viewType);
        bundle.putInt(KEY_SIZE,windowSize);
        bundle.putString(KEY_FILE_PATH,mPlayFilePath);

        /*
        boolean record = mRecordCheckBox.isChecked();
        bundle.putBoolean(KEY_RECORD,record);
        if(record){
            String path = 
                CameraHelper.getOutputMediaFile(CameraHelper.MEDIA_TYPE_VIDEO,mToken).toString();
            bundle.putString(KEY_RECORD_PATH,path);
        }
        */

        return bundle;
    }

    /**
     * file selector click listener.
     */ 
    class ButtonClickListener implements View.OnClickListener {
        @Override
        public void onClick(View v) {
            openCurDir(sRoot);
        }
    }

    private void getUSBPath(ArrayList<String> rootPath) {
        rootPath.clear();
        List<StorageUtils.StorageInfo> storageList = StorageUtils.getStorageList(mContext);
        for(StorageUtils.StorageInfo info : storageList){
            Log.d(TAG,String.format("removable=%s, path=%s", info.removable, info.path));
            if(info.removable && !info.isSdCard) {
                rootPath.add(info.path);
            }
        }
    }

    private void openCurDir(String curPath){
        final List<Map<String,Object>> listItem = new ArrayList<Map<String,Object>> ();
        final List<Map<String,Object>> lfolders = new ArrayList<Map<String,Object>> ();
        final List<Map<String,Object>> lfiles = new ArrayList<Map<String,Object>> ();
        getUSBPath(rootPath);
        if (rootPath.size() == 0) {
            Toast.makeText(mContext, "Do not detect U disk!", Toast.LENGTH_SHORT).show();
        }else {
            if (curPath.equals(sRoot)) {
                listItem.clear();
                String[] arrrootPath = new String[rootPath.size()];
                for (int i=0; i<rootPath.size(); i++) {
                    arrrootPath[i] = rootPath.get(i);
                    String rootPathName = arrrootPath[i].substring(9,arrrootPath[i].length());
                    //Map<String, Object> map = new HashMap<String, Object>();
                    Map map = new HashMap();	
                    map.put("name", rootPathName);
                    map.put("image", R.drawable.filedialog_root);
                    map.put("path", arrrootPath[i]);
                    map.put("isDire", true);
                    listItem.add(map);	

                }
            }else {
                File f = new File(curPath);
                File[] file = f.listFiles();
                Map map1 = new HashMap();
                map1.put("name", "..");
                map1.put("image", R.drawable.filedialog_folder_up);
                map1.put("path", f.getParent());
                map1.put("isDire", true);
                listItem.add(map1);	

                if(file != null) {
                    for (int i = 0;i < file.length;i++){
                        if (file[i].isDirectory()){
                            Map map = new HashMap();
                            map.put("name", file[i].getName());
                            map.put("image", R.drawable.filedialog_folder);
                            map.put("path", file[i].getPath());
                            map.put("isDire", file[i].isDirectory());
                            lfolders.add(map);

                        }else if (file[i].isFile()){
                            String mimeType = getMimeType(file[i].getPath());
                            if(mimeType!=null && mimeType.startsWith("video/")) {
                                Map map = new HashMap();
                                map.put("name", file[i].getName());
                                map.put("image", R.drawable.filedialog_video);
                                map.put("path", file[i].getPath());
                                map.put("isDire", file[i].isDirectory());
                                lfiles.add(map);	

                            }
                        }

                    }
                    listItem.addAll(lfolders);
                    listItem.addAll(lfiles);
                }
            }

            SimpleAdapter adapter = new SimpleAdapter(mContext,
                listItem,
                R.layout.filedialog,
                new String[]{"name","image"},
                new int[]{R.id.filedialogitem_name,R.id.filedialogitem_img});

            final AlertDialog.Builder builder = new Builder(mContext);
            builder.setAdapter(adapter, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        if ((Boolean) listItem.get(which).get("isDire")) {
                            openCurDir((String)listItem.get(which).get("path"));
                        }else{
                            mPlayFilePath = (String)listItem.get(which).get("path");
                            Log.d(TAG, "file:"+mPlayFilePath);
                            mInfoTextView.setText("Info: "+mPlayFilePath);
                        }
                    }
                });

            builder.show();
        }
    }

    public static String getMimeType(String url) {
        String type = null;
        String extension = UrlToExtensionUtils.getFileExtensionFromUrl(url);
        if (extension != null) {
            type = MimeTypeMap.getSingleton().getMimeTypeFromExtension(extension);
        }
        return type;
    }
}
