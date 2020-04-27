package com.mediacodec;

import java.io.File;
import java.util.*;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import android.Manifest;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.webkit.MimeTypeMap;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.*;
import android.content.Intent;
import android.content.SharedPreferences;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.TextView;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.support.v4.content.ContextCompat;
import android.support.v4.app.ActivityCompat;
import android.content.pm.PackageManager;

public class MainActivity extends Activity implements OnClickListener {
    public static final String TAG = "MainActivity";
    static final public String sRoot = "/storage"; 
    static final public String sParent = "..";
    private Button btnRecGrid1 = null;
    private Button btnRecGrid2 = null;
    private Button btnRecGrid4 = null;
    private Button btnRecGrid9 = null;
    private Button btnRecGrid16 = null;
    private Button btnOK = null;
    private TextView textView = null;
    private TextView textView2 = null;
    private EditText detText = null;
    private EditText detText2 = null;
    private AutoCompleteTextView listText[] = new AutoCompleteTextView[16];
    private CheckBox chbRepeat = null;
    private CheckBox chbAudioDynamic = null;
    private CheckBox chbFreeRun = null;
    private CheckBox chbTunnelMode = null;
    private CheckBox chbVideoOnly = null;
    private CheckBox chbPerformance = null;
    private CheckBox chbRender = null;
    private ArrayList<String> playList = null;
    private SharedPreferences playListRecord = null;
    Spinner mSpin = null;
    private ArrayList<File> fileList = new ArrayList<>();
    private ArrayList<String> mFileNameList = new ArrayList<>();
    private ArrayList<String> rootPath = new ArrayList<>();
    public String selectedfilepath = null;
    private final int PERMISSION_CONTACTS = 0;

    private Spinner spnSource = null;
    private String[] Source = {"U-Disk", "Samba", "HDMI-Rx", "USB-Camera"};
    private ArrayAdapter<String> SourceList;
    private int sourceflag = 0;
    private SmbUtils smbUtils = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate");
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.main);

        btnRecGrid1 = (Button) findViewById(R.id.btnRecGrid1);
        btnRecGrid2 = (Button) findViewById(R.id.btnRecGrid2);
        btnRecGrid4 = (Button) findViewById(R.id.btnRecGrid4);
        btnRecGrid9 = (Button) findViewById(R.id.btnRecGrid9);
        btnRecGrid16 = (Button) findViewById(R.id.btnRecGrid16);
        textView = (TextView) findViewById(R.id.txtView);
        textView2 = (TextView) findViewById(R.id.txtView2);
        detText = (EditText) findViewById(R.id.edtText);
        detText2 = (EditText) findViewById(R.id.edtText2);
        
        listText[0] = (AutoCompleteTextView) findViewById(R.id.contText1);
        listText[1] = (AutoCompleteTextView) findViewById(R.id.contText2);
        listText[2] = (AutoCompleteTextView) findViewById(R.id.contText3);
        listText[3] = (AutoCompleteTextView) findViewById(R.id.contText4);
        listText[4] = (AutoCompleteTextView) findViewById(R.id.contText5);
        listText[5] = (AutoCompleteTextView) findViewById(R.id.contText6);
        listText[6] = (AutoCompleteTextView) findViewById(R.id.contText7);
        listText[7] = (AutoCompleteTextView) findViewById(R.id.contText8);
        listText[8] = (AutoCompleteTextView) findViewById(R.id.contText9);
        listText[9] = (AutoCompleteTextView) findViewById(R.id.contText10);
        listText[10] = (AutoCompleteTextView) findViewById(R.id.contText11);
        listText[11] = (AutoCompleteTextView) findViewById(R.id.contText12);
        listText[12] = (AutoCompleteTextView) findViewById(R.id.contText13);
        listText[13] = (AutoCompleteTextView) findViewById(R.id.contText14);
        listText[14] = (AutoCompleteTextView) findViewById(R.id.contText15);
        listText[15] = (AutoCompleteTextView) findViewById(R.id.contText16);

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED
            || ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.CAMERA}, PERMISSION_CONTACTS);
        }
        else {
            ArrayAdapter<String> dataList = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, mFileNameList);
            for(AutoCompleteTextView actv : listText){
                actv.setThreshold(0);
                actv.setAdapter(dataList);
                actv.setOnClickListener(this);
            }
        }
        
        btnOK = (Button) findViewById(R.id.btnOK);
        chbRepeat = (CheckBox) findViewById(R.id.chbRepeat);
        chbAudioDynamic = (CheckBox) findViewById(R.id.chbAudioDynamic);
        chbFreeRun = (CheckBox) findViewById(R.id.chbFreeRun);
        chbTunnelMode = (CheckBox) findViewById(R.id.chbTunnelMode);
        chbVideoOnly = (CheckBox) findViewById(R.id.chbVideoOnly);
        chbPerformance = (CheckBox) findViewById(R.id.chbPerformance);
        chbRender = (CheckBox) findViewById(R.id.chbRender);
        playList = new ArrayList<String>();
        playListRecord = getSharedPreferences("RealtekPIP", 0);
        String temp = playListRecord.getString("PLAYLIST", "");
        if(temp != "")
        {
            String[] playlists = new String[16];
            playlists = temp.split(",");
            for(int i=0;i<16; i++)
            {
                if(playlists[i] != "")
                {
                    Log.d(TAG, "Set Source " + i + " Path " + playlists[i]);
                    listText[i].setText(playlists[i]);
                }
            }
        }
        

	smbUtils = SmbUtils.getInstance(this);
	// Select source path
	spnSource = (Spinner) findViewById(R.id.spnSource);
        SourceList = new ArrayAdapter<String>(this, R.layout.spinner, Source);
        spnSource.setAdapter(SourceList);
        spnSource.setOnItemSelectedListener(new SpinnerListener());

        chbRepeat.setChecked(false);
        chbAudioDynamic.setChecked(false);
        chbFreeRun.setChecked(false);
        chbTunnelMode.setChecked(false);
        chbVideoOnly.setChecked(false);
        chbPerformance.setChecked(false);
        chbRender.setChecked(true);
        btnRecGrid1.setOnClickListener(this);
        btnRecGrid2.setOnClickListener(this);
        btnRecGrid4.setOnClickListener(this);
        btnRecGrid9.setOnClickListener(this);
        btnRecGrid16.setOnClickListener(this);
        btnOK.setOnClickListener(this);
                
    }

    private void getAllVideoFileList(){
        getUSBPath(rootPath);
        fileList.clear();
	for (String path : rootPath) {
	    if (path == null) return;
            listAllFiles(path, fileList);
	}
        //Collections.sort(fileList, new FileTypeComparator());
        mFileNameList.clear();
        for(File f : fileList){
            //String mimeType = getMimeType(f.getPath());
//            log(String.format("fUri=%s, mime=%s", f.getPath(), mimeType));
            //if(mimeType!=null && mimeType.startsWith("video/")) 
            {
                log("f="+f.getPath());
                mFileNameList.add(f.getPath());
            }
        }
    }
    @Override
    public void onRequestPermissionsResult(int requestCode,
            String permissions[], int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode) {
            case PERMISSION_CONTACTS:
                if (grantResults.length > 0
                    && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    ArrayAdapter<String> dataList = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, mFileNameList);
                    for(AutoCompleteTextView actv : listText){
                        actv.setThreshold(0);
                        actv.setAdapter(dataList);
                        actv.setOnClickListener(this);
                    }
                } else {
                    MainActivity.this.finish();
                }
                break;
            default:
                break;
        }
    }

    private void getUSBPath(ArrayList<String> rootPath) {
	rootPath.clear();
        List<StorageUtils.StorageInfo> storageList = StorageUtils.getStorageList(this);
        for(StorageUtils.StorageInfo info : storageList){
	    Log.d("StorageInfo",String.format("removable=%s, path=%s", info.removable, info.path));
            if(info.removable && !info.isSdCard) {
                rootPath.add(info.path);
            }
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

    public void listAllFiles(String directoryName, ArrayList<File> files) {
        File directory = new File(directoryName);

        // get all the files from a directory
        File[] fList = directory.listFiles();
        for (File file : fList) {
            if (file.isFile()) {
                files.add(file);
            } else if (file.isDirectory()) {
                listAllFiles(file.getAbsolutePath(), files);
            }
        }
    }

    private class FileTypeComparator implements Comparator<File> {
        @Override
        public int compare(File f1, File f2) {
            String s1 = f1.getName();
            String s2 = f2.getName();
            int s1Idx = s1.indexOf(".");
            int s2Idx = s2.indexOf(".");
            if(s1Idx<0) return 1;
            if(s2Idx<0) return -1;
            return s1.substring(s1Idx).compareTo(s2.substring(s2Idx));
        }
    }

    private void log(String s) {
        Log.d(TAG, s);
    }

//    @Override
//    protected void onResume() {
//        // TODO Auto-generated method stub
//        super.onResume();
//        Intent intent = new Intent();
//        intent.setClass(MainActivity.this, DecodeActivity.class);
//        intent.putExtra("DATA_INT", 2);
//        intent.putExtra("LIMIT_INT", 1);
//        intent.putExtra("LIMIT_AUD_INT", 1);
//        intent.putExtra("REPEAT", 1);        
//        startActivity(intent);
//        MainActivity.this.finish();
//    }

    @Override
    protected void onPause() {
        // TODO Auto-generated method stub
        super.onPause();
    }
    
    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();

        SmbUtils.deleteInstance();
        try{
            Thread.sleep(100);
        }catch(InterruptedException e){}      
    }
	
    	private void openCurDir(final AutoCompleteTextView auto,String curPath,final String root) {
		final List<Map<String,Object>> listItem = new ArrayList<Map<String,Object>> ();
		final List<Map<String,Object>> lfolders = new ArrayList<Map<String,Object>> ();
		final List<Map<String,Object>> lfiles = new ArrayList<Map<String,Object>> ();
	
	    	if (curPath.equals(root)) {
	        	listItem.clear();
			String[] arrrootPath = new String[rootPath.size()];
			for (int i=0; i<rootPath.size(); i++) {
				arrrootPath[i] = rootPath.get(i);
				String rootPathName = arrrootPath[i].substring(root.length()+1,arrrootPath[i].length());
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
			    			//String mimeType = getMimeType(file[i].getPath());
			    			//if(mimeType!=null && mimeType.startsWith("video/")) 
                            {
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
	    	SimpleAdapter adapter = new SimpleAdapter(this, listItem, R.layout.filedialog,new String[]{"name","image"},
							new int[]{R.id.filedialogitem_name,R.id.filedialogitem_img});
	    	final AlertDialog.Builder builder = new Builder(MainActivity.this);
            	builder.setTitle("Add Video Path");
            	builder.setIcon(R.drawable.ic_launcher);
	    	builder.setAdapter(adapter, new DialogInterface.OnClickListener() {	
			@Override
			public void onClick(DialogInterface dialog, int which) {
		    	// TODO Auto-generated method stub
		    		if ((Boolean) listItem.get(which).get("isDire")) {
	                		openCurDir(auto,(String)listItem.get(which).get("path"),root);
		    		}else {
					auto.setText((String)listItem.get(which).get("path"));
		    		}
		    	}
	        });
	    	builder.show();	
    	}
    
    @Override
    public void onClick(View v) {
        if(v instanceof AutoCompleteTextView){
            AutoCompleteTextView actv = (AutoCompleteTextView) v;
			switch(sourceflag) {
				case 0:
					if (rootPath.size() == 0) {
	    					Toast.makeText(MainActivity.this, "No U-disk available!", Toast.LENGTH_SHORT).show();
					} else {
						openCurDir(actv, sRoot, sRoot);
					}
					break;
				case 1:
					if(new File(SmbUtils.MOUNT_ROOT).exists()) {
						rootPath.clear();
						File [] mntSamba = new File(SmbUtils.MOUNT_ROOT).listFiles();
						if(mntSamba.length > 0) {
							for(File tmp : mntSamba) {
								rootPath.add(tmp.getPath());
							}
							openCurDir(actv, SmbUtils.MOUNT_ROOT, SmbUtils.MOUNT_ROOT);
						} else {
							Toast.makeText(this, "No samba server available!", Toast.LENGTH_SHORT).show();
						}
					} else {
						Toast.makeText(this, "No samba server available!", Toast.LENGTH_SHORT).show();
					}
					break;
				case 2:
					actv.setText("SRC");
					break;
				case 3:
					actv.setText("CAM");
					break;
				default:
					actv.setText("/");
			}
            return;
        }
            // TODO Auto-generated method stub        
            Intent intent = new Intent();
            intent.setClass(MainActivity.this, DecodeActivity.class);
            int intGridNum = 0;
            int intVideoGridNum = 0;
            int intAudioGridNum = 0;
            switch (v.getId()) {
            case R.id.btnRecGrid1:
                intGridNum = 1;
                intVideoGridNum = 1;
                intAudioGridNum = 1;
                break;
            case R.id.btnRecGrid2:
                intGridNum = 2;
                intVideoGridNum = 2;
                intAudioGridNum = 1;
                break;
    
            case R.id.btnRecGrid4:
                intGridNum = 4;
                intVideoGridNum = 4;
                intAudioGridNum = 1;
                break;
    
            case R.id.btnRecGrid9:
                intGridNum = 9;
                intVideoGridNum = 9;
                intAudioGridNum = 1;
                break;
    
            case R.id.btnRecGrid16:
                intGridNum = 16;
                intVideoGridNum = 16;
                intAudioGridNum = 1;
                break;
    
            case R.id.btnOK:
                intGridNum = 16;
                if(detText.getText().toString().trim().length() == 0 || detText2.getText().toString().trim().length() == 0)
                    return;
                intVideoGridNum = Integer.parseInt(detText.getText().toString());
                if (intVideoGridNum < 0)
                    intVideoGridNum = 1;
                else if (intVideoGridNum > 16)
                    intVideoGridNum = 16;

                intAudioGridNum = Integer.parseInt(detText2.getText().toString());
                if (intAudioGridNum < 0)
                    intAudioGridNum = 1;
                else if (intAudioGridNum > 16)
                    intAudioGridNum = 16;
                break;
            }

            if(chbAudioDynamic.isChecked())
                intAudioGridNum = intVideoGridNum;
            if(chbVideoOnly.isChecked())
                intAudioGridNum = 0;

            intent.putExtra("DATA_INT", intGridNum);
            intent.putExtra("LIMIT_INT", intVideoGridNum);
            intent.putExtra("LIMIT_AUD_INT", intAudioGridNum);

            if(chbRepeat.isChecked())
                intent.putExtra("REPEAT", 1);
            else
                intent.putExtra("REPEAT", 0);

            if(chbAudioDynamic.isChecked())
                intent.putExtra("AUDIOCLICK", 1);
            else
                intent.putExtra("AUDIOCLICK", 0);

            if(chbFreeRun.isChecked())
                intent.putExtra("FREERUN", 1);
            else
                intent.putExtra("FREERUN", 0);

            if(chbTunnelMode.isChecked())
                intent.putExtra("TUNNELMODE", 1);
            else
                intent.putExtra("TUNNELMODE", 0);

            if(chbPerformance.isChecked())
                intent.putExtra("PERFORMANCE", 1);
            else
                intent.putExtra("PERFORMANCE", 0);

            if(chbRender.isChecked())
                intent.putExtra("RENDER", 1);
            else
                intent.putExtra("RENDER", 0);
            StringBuilder sb = new StringBuilder();
            playList.clear();
            for(int i=0;i<16; i++)
            {
                String tmp = listText[i].getText().toString();
                playList.add(tmp);
                sb.append(tmp).append(",");
            }
            
            playListRecord.edit().putString("PLAYLIST", sb.toString()).commit();
            intent.putStringArrayListExtra("LIST", playList);
            
            Log.d(TAG, "onClick start DecodeActivity");
            startActivity(intent);
            //MainActivity.this.finish();
    }


	public class SpinnerListener implements OnItemSelectedListener {
		public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
			
			switch(position) {
				case 0:	//U-Disk
					sourceflag = 0;
					getUSBPath(rootPath);
					break;
				case 1:	//Samba
					sourceflag = 1;
					smbUtils.createSmbLoginDialog();
					break;
				case 2:	//HDMI-Rx
					sourceflag = 2;
					break;
				case 3:	//USB-Camera
					sourceflag = 3;
					break;
				default:
			
			}
		}
		public void onNothingSelected(AdapterView<?> parent) {
			
		}
	}
}
