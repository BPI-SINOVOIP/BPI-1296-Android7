package com.realtek.transcode;

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
import android.widget.*;
import android.content.Intent;
import android.content.IntentFilter;
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
import android.content.BroadcastReceiver;
import android.content.Context;
//import android.support.v4.app.Fragment;
import android.content.pm.PackageManager;

public class MainActivity extends Activity implements OnClickListener {
    public static final String TAG = "MainActivity";
    private static final int ENCODER_INSTANCES_LIMIT = 4;
    private static final int OTHER_TEXT_LIMIT = 3;
    private static final int RESOLUTION_SET = 2;
    private Button btnRecGrid1 = null;
    private Button btnRecGrid2 = null;
    private Button btnRecGrid4 = null;
    private Button btnRecGrid9 = null;
    private Button btnMonitor = null;
    private AutoCompleteTextView listText[] = new AutoCompleteTextView[ENCODER_INSTANCES_LIMIT];
    private EditText listTargetText[] = new EditText[ENCODER_INSTANCES_LIMIT];
    private EditText listOtherText[] = new EditText[OTHER_TEXT_LIMIT];
    private EditText resolText[] = new EditText[RESOLUTION_SET];
    private ArrayList<String> playList = null;
    private ArrayList<String> targetList = null;
    private SharedPreferences playListRecord = null;
    private Spinner spnFR = null;
    private Spinner spnBR = null;
    private Spinner spnFI = null;
    private Spinner spnFormat = null;
    private Spinner spnExit = null;
    private Spinner spnTrack = null;
    private Spinner spnPass = null;
    private Spinner spnResol = null;
    private Spinner spnSampleRate = null;
    private Spinner spnChannel = null;
    private Spinner spnDeinterlace = null;
    private Spinner spnBitrateMode = null;
    private Spinner spnProfile = null;
    private Spinner spnLevel = null;
    private Spinner spnStreamOption = null;
    private ArrayAdapter<String> FrameRateList;
    private ArrayAdapter<String> BitRateList;
    private ArrayAdapter<String> FrameIntervalList;
    private ArrayAdapter<String> FormatList;
    private ArrayAdapter<String> ExitList;
    private ArrayAdapter<String> TrackList;
    private ArrayAdapter<String> PassList;
    private ArrayAdapter<String> ResolList;
    private ArrayAdapter<String> SampleRateList;
    private ArrayAdapter<String> ChannelList;
    private ArrayAdapter<String> DeinterlaceList;
    private ArrayAdapter<String> BitrateModeList;
    private ArrayAdapter<String> ProfileList;
    private ArrayAdapter<String> LevelList;
    private ArrayAdapter<String> StreamOptionList;
    private String[] FrameRate = {"Original", "24", "30", "60", "10", "12", "15", "20", "25", "50"};
    private String[] BitRate = {"AUTO", "3000000", "6000000", "9000000"};
    private String[] FrameInterval = {"1", "0", "3", "5", "10"};
    private String[] OutputFormat = {"MPEG2TS", "MP4"};
	//John
	private String[] OutputFileEx={"ts","mp4"};
	private Map<String,String> OutputFileExMap = new HashMap<String,String>();
    private String[] ExitType = {"Terminated", "KeepGoing"};
    private String[] Track = {"Video + Audio", "VideoOnly", "AudioOnly"};
    private String[] Pass = {"AAC", "ByPass"};
    private String[] Resol = {"720P", "1080P", "Original"};
    private String[] SampleRate = {"48000", "96000", "88200", "64000", "44100", "32000", "24000", "22050", "16000", "12000", "11025", "8000"};
    private String[] Channel = {"2", "3", "4", "5", "6", "1"};
    private String[] Deinterlace = {"AUTO_DEINT", "FORCE_DEINT", "FORCE_PROGRESSIVE"};
    private String[] BitrateMode = {"Disable", "Variable", "Constant", "VariableSkipFrames", "ConstantSkipFrames"};
    private String[] Profile = {"AVCProfileHigh", "AVCProfileBaseline", "AVCProfileMain"};
    private String[] Level = {"AVCLevel42", "AVCLevel1", "AVCLevel11", "AVCLevel12", "AVCLevel13", "AVCLevel1b", "AVCLevel2", "AVCLevel21", "AVCLevel22", "AVCLevel3", "AVCLevel31", "AVCLevel32", "AVCLevel4", "AVCLevel41"};
    private String[] StreamOption = {"OFF", "ON"};
    private String selectedFrameRate = "Original";
    private int selectedBitRate = -1;
    private int selectedFrameInterval = 10;
    private String selectedOutoutFormat = "MPEG2TS";
    private String selectedExit = "Terminated";
    private String selectedTrack = "ALL";
    private String selectedPass = "AAC";
    private String selectedResol = "Original";
    private int selectedSampleRate = 48000;
    private int selectedChannel = 2;
    private int selectedDeinterlace = 0;
    private int selectedBitrateMode = 0;
    private int selectedProfile = 1;
    private int selectedLevel = 1;
    private int selectedStream = 0;
    Spinner mSpin = null;
    private ArrayList<File> fileList = new ArrayList<>();
    private ArrayList<String> mFileNameList = new ArrayList<>();
    private final int READ_WRITE_CONTACTS = 0;
    private ArrayList<String> rootPath = new ArrayList<>();
    private boolean firstClick = true;
    static final public String sRoot = "/storage"; 
    static final public String sParent = "..";
	
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
        btnMonitor = (Button) findViewById(R.id.btnMonitor);

        listText[0] = (AutoCompleteTextView) findViewById(R.id.contText1);
        listText[1] = (AutoCompleteTextView) findViewById(R.id.contText2);
        listText[2] = (AutoCompleteTextView) findViewById(R.id.contText3);
        listText[3] = (AutoCompleteTextView) findViewById(R.id.contText4);

        listTargetText[0] = (EditText) findViewById(R.id.targetText1);
        listTargetText[1] = (EditText) findViewById(R.id.targetText2);
        listTargetText[2] = (EditText) findViewById(R.id.targetText3);
        listTargetText[3] = (EditText) findViewById(R.id.targetText4);

        listOtherText[0] = (EditText) findViewById(R.id.ipText);
        listOtherText[1] = (EditText) findViewById(R.id.portText);
        listOtherText[2] = (EditText) findViewById(R.id.repeatText);

        resolText[0] = (EditText) findViewById(R.id.widthText);
        resolText[1] = (EditText) findViewById(R.id.heightText);

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) 
        {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE}, READ_WRITE_CONTACTS);
        }
        else
        {
            //getAllVideoAudioFileList();
            ArrayAdapter<String> dataList = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, mFileNameList);
            for(AutoCompleteTextView actv : listText){
                actv.setThreshold(0);
                actv.setAdapter(dataList);
                actv.setOnClickListener(this);
            }
        }
        
        playListRecord = getSharedPreferences("RealtekEncoder", 0);
        String temp = playListRecord.getString("PLAYLIST", "");
        String temp2 = playListRecord.getString("TARGETLIST", "");
        String temp3 = playListRecord.getString("OTHER", "");
        String temp4 = playListRecord.getString("RESOLUTION", "");

        if(temp != "") {
            String[] playlists = new String[ENCODER_INSTANCES_LIMIT];
            playlists = temp.split(",");
            for (int i = 0; i < ENCODER_INSTANCES_LIMIT; i++) {
                if (playlists[i] != "") {
                    Log.d(TAG, "Set Source " + i + " Path " + playlists[i]);
                    listText[i].setText(playlists[i]);
                }
            }
        }

        if(temp2 != "")
        {
            String[] targetlists = new String[ENCODER_INSTANCES_LIMIT];
            targetlists = temp2.split(",");
            for(int i=0;i<ENCODER_INSTANCES_LIMIT; i++)
            {
                if(targetlists[i] != "")
                {
                    Log.d(TAG, "Set Source " + i + " Path " + targetlists[i]);
                    listTargetText[i].setText(targetlists[i]);
                }
            }
        }

        if(temp3 != "")
        {
            String[] otherlists = new String[OTHER_TEXT_LIMIT];
            otherlists = temp3.split(",");
            for(int i=0;i<OTHER_TEXT_LIMIT; i++)
            {
                if(otherlists[i] != "")
                {
                    Log.d(TAG, "Set Other Text " + i + " with " + otherlists[i]);
                    listOtherText[i].setText(otherlists[i]);
                }
            }
        }

        if(temp4 != "")
        {
            String[] resollists = new String[RESOLUTION_SET];
            resollists = temp4.split(",");
            for(int i=0;i<RESOLUTION_SET; i++)
            {
                if(resollists[i] != "")
                {
                    Log.d(TAG, "Set resolution Text " + i + " with " + resollists[i]);
                    resolText[i].setText(resollists[i]);
                }
            }
        }

        //John
		OutputFileExMap=this.setOutputFileExMap(OutputFormat,OutputFileEx);
		
        spnSampleRate = (Spinner) findViewById(R.id.spnSampleRate);
        SampleRateList = new ArrayAdapter<String>(this, R.layout.spinner, SampleRate);
        spnSampleRate.setAdapter(SampleRateList);
        spnSampleRate.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                selectedSampleRate = Integer.parseInt(adapterView.getSelectedItem().toString());
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });

        spnChannel = (Spinner) findViewById(R.id.spnChannel);
        ChannelList = new ArrayAdapter<String>(this, R.layout.spinner, Channel);
        spnChannel.setAdapter(ChannelList);
        spnChannel.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                selectedChannel = Integer.parseInt(adapterView.getSelectedItem().toString());
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });

        spnDeinterlace = (Spinner) findViewById(R.id.spnDeinterlace);
        DeinterlaceList = new ArrayAdapter<String>(this, R.layout.spinner, Deinterlace);
        spnDeinterlace.setAdapter(DeinterlaceList);
        spnDeinterlace.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                switch(adapterView.getSelectedItem().toString())
                {
                    case "AUTOD_EINT":
                        selectedDeinterlace = 0;
                        break;
                    case "FORCE_DEINT":
                        selectedDeinterlace = 1;
                        break;
                    case "FORCE_PROGRESSIVE":
                        selectedDeinterlace = 2;
                        break;    
                    default:
                        break;
                }
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });

        spnBitrateMode = (Spinner) findViewById(R.id.spnBitrateMode);
        BitrateModeList = new ArrayAdapter<String>(this, R.layout.spinner, BitrateMode);
        spnBitrateMode.setAdapter(BitrateModeList);
        spnBitrateMode.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                switch(adapterView.getSelectedItem().toString())
                {
                    case "Disable":
                        selectedBitrateMode = 0;
                        break;
                    case "Variable":
                        selectedBitrateMode = 1;
                        break;
                    case "Constant":
                        selectedBitrateMode = 2;
                        break;
                    case "VariableSkipFrames":
                        selectedBitrateMode = 3;
                        break;
                    case "ConstantSkipFrame":
                        selectedBitrateMode = 4;
                        break;
                    default:
                        break;
                }
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });

        spnProfile = (Spinner) findViewById(R.id.spnProfile);
        ProfileList = new ArrayAdapter<String>(this, R.layout.spinner, Profile);
        spnProfile.setAdapter(ProfileList);
        spnProfile.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                switch(adapterView.getSelectedItem().toString())
                {
                    case "AVCProfileBaseline":
                        selectedProfile = 1;
                        break;
                    case "AVCProfileHigh":
                        selectedProfile = 8;
                        break;
                    case "AVCProfileMain":
                        selectedProfile = 2;
                        break;
                    default:
                        break;
                }
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });

        spnLevel = (Spinner) findViewById(R.id.spnLevel);
        LevelList = new ArrayAdapter<String>(this, R.layout.spinner, Level);
        spnLevel.setAdapter(LevelList);
        spnLevel.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                switch(adapterView.getSelectedItem().toString())
                {
                    case "AVCLevel1":
                        selectedLevel = 1;
                        break;
                    case "AVCLevel11":
                        selectedLevel = 4;
                        break;
                    case "AVCLevel12":
                        selectedLevel = 8;
                        break;
                    case "AVCLevel13":
                        selectedLevel = 16;
                        break;
                    case "AVCLevel1b":
                        selectedLevel = 2;
                        break;
                    case "AVCLevel2":
                        selectedLevel = 32;
                        break;
                    case "AVCLevel21":
                        selectedLevel = 64;
                        break;
                    case "AVCLevel22":
                        selectedLevel = 128;
                        break;
                    case "AVCLevel3":
                        selectedLevel = 256;
                        break;
                    case "AVCLevel31":
                        selectedLevel = 512;
                        break;
                    case "AVCLevel32":
                        selectedLevel = 1024;
                        break;
                    case "AVCLevel4":
                        selectedLevel = 2048;
                        break;
                    case "AVCLevel41":
                        selectedLevel = 4096;
                        break;
                    case "AVCLevel42":
                        selectedLevel = 8192;
                        break;
                    default:
                        break;
                }
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });

        spnStreamOption = (Spinner) findViewById(R.id.spnStreamChoise);
        StreamOptionList = new ArrayAdapter<String>(this, R.layout.spinner, StreamOption);
        spnStreamOption.setAdapter(StreamOptionList);
        spnStreamOption.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                switch(adapterView.getSelectedItem().toString())
                {
                    case "ON":
                        selectedStream = 1;
                        break;
                    case "OFF":
                        selectedStream = 0;
                        break;
                    default:
                        break;
                }
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });

        spnResol = (Spinner) findViewById(R.id.spnResol);
        ResolList = new ArrayAdapter<String>(this, R.layout.spinner, Resol);
        spnResol.setAdapter(ResolList);
        spnResol.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                selectedResol = adapterView.getSelectedItem().toString();
                if(firstClick == true)
                    firstClick = false;
                else
                {
                    switch(selectedResol)
                    {
                        case "Original":
                            resolText[0].setText("0");
                            resolText[1].setText("0");
                            break;
                        case "4K":
                          resolText[0].setText("3840");
                          resolText[1].setText("2160");
                          break;
                        case "1080P":
                          resolText[0].setText("1920");
                           resolText[1].setText("1080");
                            break;
                        case "720P":
                            resolText[0].setText("1280");
                            resolText[1].setText("720");
                            break;
                        default:
                            resolText[0].setText("0");
                            resolText[1].setText("0");
                            break;
                    }
                }
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });

        spnPass = (Spinner) findViewById(R.id.spnPass);
        PassList = new ArrayAdapter<String>(this, R.layout.spinner, Pass);
        spnPass.setAdapter(PassList);
        spnPass.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                selectedPass = adapterView.getSelectedItem().toString();
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });

        spnTrack = (Spinner) findViewById(R.id.spnTrack);
        TrackList = new ArrayAdapter<String>(this, R.layout.spinner, Track);
        spnTrack.setAdapter(TrackList);
        spnTrack.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                selectedTrack = adapterView.getSelectedItem().toString();
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });
        
        spnFormat = (Spinner) findViewById(R.id.spnFormat);
        FormatList = new ArrayAdapter<String>(this, R.layout.spinner, OutputFormat);
        spnFormat.setAdapter(FormatList);
        spnFormat.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                selectedOutoutFormat = adapterView.getSelectedItem().toString();
				//John
				changeOutputFileEx(selectedOutoutFormat);
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });

        spnExit = (Spinner) findViewById(R.id.spnExit);
        ExitList = new ArrayAdapter<String>(this, R.layout.spinner, ExitType);
        spnExit.setAdapter(ExitList);
        spnExit.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                selectedExit= adapterView.getSelectedItem().toString();
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });

        spnFR = (Spinner) findViewById(R.id.spnFR);
        FrameRateList = new ArrayAdapter<String>(this, R.layout.spinner, FrameRate);
        spnFR.setAdapter(FrameRateList);
        spnFR.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                selectedFrameRate = adapterView.getSelectedItem().toString();
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });
        spnBR = (Spinner) findViewById(R.id.spnBR);
        BitRateList = new ArrayAdapter<String>(this, R.layout.spinner, BitRate);
        spnBR.setAdapter(BitRateList);
        spnBR.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                String temp = adapterView.getSelectedItem().toString();
                if(temp.compareTo("AUTO") == 0)
                    selectedBitRate = -1;
                else
                    selectedBitRate = Integer.parseInt(temp);
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });
        spnFI = (Spinner) findViewById(R.id.spnFI);
        FrameIntervalList = new ArrayAdapter<String>(this, R.layout.spinner, FrameInterval);
        spnFI.setAdapter(FrameIntervalList);
        spnFI.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
            public void onItemSelected(AdapterView adapterView, View view, int position, long id) {
                selectedFrameInterval = Integer.parseInt(adapterView.getSelectedItem().toString());
            }
            public void onNothingSelected(AdapterView arg0) {

            }
        });
        btnRecGrid1.setOnClickListener(this);
        btnRecGrid2.setOnClickListener(this);
        btnRecGrid4.setOnClickListener(this);
        btnRecGrid9.setOnClickListener(this);
        btnMonitor.setOnClickListener(this);

        IntentFilter iFilter = new IntentFilter(Intent.ACTION_SCREEN_OFF);
        this.registerReceiver(mReceiver, iFilter);
    }

    private void getAllVideoAudioFileList(){
	getUSBPath(rootPath);	
	fileList.clear();
	mFileNameList.clear();
	for(String path : rootPath) { 
	    if (path == null) 
		return;
	    listAllFiles(path, fileList);
	}
	for(File f : fileList) {
        String mimePath = f.getPath();
        log("f="+mimePath);
        mFileNameList.add(mimePath);
        /*
        String mimePath = f.getPath();
	    String mimeType = getMimeType(mimePath);
	    //Log.d("fUrl-MimeType",String.format("fUrl=%s, mime=%s", mimePath, mimeType));
	    if(mimeType!=null && (mimeType.startsWith("video/") || mimeType.startsWith("audio/"))) {
            log("f="+f.getPath());
            mFileNameList.add(mimePath);
	    }
        else if(mimeType==null)//Which original extractor not support
        {
            if(mimePath.endsWith(".aiff") || mimePath.endsWith(".AIFF")){
                log("f="+f.getPath());
                mFileNameList.add(mimePath);
	        }
        }
        */
	}
			
    }

    private void getUSBPath(ArrayList<String> rootPath) {
	rootPath.clear();
        List<StorageUtils.StorageInfo> storageList = StorageUtils.getStorageList(this);		
		//Debug	
	for(StorageUtils.StorageInfo info : storageList){
	    Log.d("StorageInfo",String.format("removable=%s, path=%s", info.removable, info.path));
        }
		//Debug
        for(StorageUtils.StorageInfo info : storageList){
			
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
	
	
	//John
	public Map<String,String> setOutputFileExMap(String[] outputFormatTemp,String[] outputFileExTemp)
	{
		if(outputFormatTemp.length<0||outputFileExTemp.length<0)
		{
			Log.d(TAG,"outPutFile formate Error");
		}
		Map<String,String> OutputFileExMapTemp = new HashMap<String,String>();
		for(int i=0;i<OutputFormat.length;i++)
		{
			OutputFileExMapTemp.put(outputFormatTemp[i],outputFileExTemp[i]);	
		}
		return OutputFileExMapTemp;	
	}
	
	//John
	public void changeOutputFileEx(String ouputFile)
	{
		String fileEx = OutputFileExMap.get(ouputFile);
		for(int i=0;i<listTargetText.length;i++)
		{
			String oldOutputFileName=listTargetText[i].getText().toString();
			int pointPosition=oldOutputFileName.lastIndexOf(".");
			if(pointPosition<0)
			{
				Log.d(TAG,"error OutputName,you could add a point at the end yourself");
			}
			Log.d(TAG,"oldOutputFileName: "+oldOutputFileName);
			String newOutputFileName=oldOutputFileName.substring(0,(oldOutputFileName.lastIndexOf(".")+1))+fileEx;
			Log.d(TAG,"newOutputFileName: "+newOutputFileName);
			listTargetText[i].setText(newOutputFileName);
		}
		
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
    }

    private void openCurDir(final AutoCompleteTextView auto,String curPath) {
	final List<Map<String,Object>> listItem = new ArrayList<Map<String,Object>> ();
	final List<Map<String,Object>> lfolders = new ArrayList<Map<String,Object>> ();
	final List<Map<String,Object>> lfiles = new ArrayList<Map<String,Object>> ();
	getUSBPath(rootPath);
	if (rootPath.size() == 0) {
	    Toast.makeText(MainActivity.this, "Do not detect U disk!", Toast.LENGTH_SHORT).show();
	}else {
	    if (curPath.equals(sRoot)) {
	        listItem.clear();
		String[] arrrootPath = new String[rootPath.size()];
		for (int i=0; i<rootPath.size(); i++) {
			arrrootPath[i] = rootPath.get(i);
			String rootPathName = arrrootPath[i].substring(9,arrrootPath[i].length());
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
	    SimpleAdapter adapter = new SimpleAdapter(this, listItem, R.layout.filedialog,new String[]{"name","image"},new int[]{R.id.filedialogitem_name,R.id.filedialogitem_img});
	    final AlertDialog.Builder builder = new Builder(MainActivity.this);
            builder.setTitle("Add Video Path");
            builder.setIcon(R.drawable.ic_launcher);
	    builder.setAdapter(adapter, new DialogInterface.OnClickListener() {	
		@Override
		public void onClick(DialogInterface dialog, int which) {
		    // TODO Auto-generated method stub
		    if ((Boolean) listItem.get(which).get("isDire")) {
	                openCurDir(auto,(String)listItem.get(which).get("path"));
		    }else {
			auto.setText((String)listItem.get(which).get("path"));
		    }
		    }
	        });
	    builder.show();
        }	
    }
    
    @Override
    public void onClick(View v) {
        if(v instanceof AutoCompleteTextView){
            AutoCompleteTextView actv = (AutoCompleteTextView) v;
            //actv.setText("/");
            openCurDir(actv,sRoot);
            return;
        }
            // TODO Auto-generated method stub        
            Intent intent = new Intent();
            intent.setClass(MainActivity.this, DecodeActivity.class);
            int intSourceNum = 0;
            switch (v.getId()) {
            case R.id.btnRecGrid1:
                intSourceNum = 1;
                break;
            case R.id.btnRecGrid2:
                intSourceNum = 2;
                break;
    
            case R.id.btnRecGrid4:
                intSourceNum = 3;
                break;

            case R.id.btnRecGrid9:
                intSourceNum = 4;
                break;

            case R.id.btnMonitor:
                intSourceNum = 0;
				Log.d(TAG,"Monitor log: set Monitor flag");
                break;
                default:
                break;
            }

            int track;
            if(selectedTrack.compareTo("AudioOnly") == 0)
                track = 1;
            else if(selectedTrack.compareTo("VideoOnly") == 0)
                track = 0;
            else 
                track = 2;

            intent.putExtra("DATA_INT", intSourceNum);
            intent.putExtra("TRACK_INT", track);
            intent.putExtra("OUTPUT_FORMAT", selectedOutoutFormat);
            intent.putExtra("EXIT_TYPE", selectedExit);
            intent.putExtra("FRAME_RATE", selectedFrameRate);
            intent.putExtra("BIT_RATE", selectedBitRate);
            intent.putExtra("FRAME_INTERVAL", selectedFrameInterval);
            intent.putExtra("AUDIO_MODE", selectedPass);
            intent.putExtra("WIDTH", Integer.parseInt(resolText[0].getText().toString()));
            intent.putExtra("HEIGHT", Integer.parseInt(resolText[1].getText().toString()));
            intent.putExtra("DE_INTERLACE", selectedDeinterlace);
            intent.putExtra("BITRATE_MODE", selectedBitrateMode);
            intent.putExtra("PROFILE", selectedProfile);
            intent.putExtra("LEVEL", selectedLevel);
            intent.putExtra("SAMPLE_RATE", selectedSampleRate);
            intent.putExtra("CHANNEL", selectedChannel);
            intent.putExtra("STREAMING", selectedStream);
            intent.putExtra("IP", listOtherText[0].getText().toString());
            intent.putExtra("PORT", Integer.parseInt(listOtherText[1].getText().toString()));
            intent.putExtra("REPEAT", Integer.parseInt(listOtherText[2].getText().toString()));

            StringBuilder sb = new StringBuilder();
            playList = new ArrayList<String>();
            for(int i=0;i<ENCODER_INSTANCES_LIMIT; i++)
            {
                String tmp = listText[i].getText().toString();
                playList.add(tmp);
                sb.append(tmp).append(",");
            }
            
            playListRecord.edit().putString("PLAYLIST", sb.toString()).commit();
            intent.putStringArrayListExtra("LIST", playList);

            sb = new StringBuilder();
            targetList = new ArrayList<String>();
            for(int i=0;i<ENCODER_INSTANCES_LIMIT; i++)
            {
                String tmp = listTargetText[i].getText().toString();
                targetList.add(tmp);
                sb.append(tmp).append(",");
            }

            playListRecord.edit().putString("TARGETLIST", sb.toString()).commit();
            intent.putStringArrayListExtra("LIST_TARGET", targetList);

            sb = new StringBuilder();
            targetList = new ArrayList<String>();
            for(int i=0;i<OTHER_TEXT_LIMIT; i++)
            {
                String tmp = listOtherText[i].getText().toString();
                targetList.add(tmp);
                sb.append(tmp).append(",");
            }

            playListRecord.edit().putString("OTHER", sb.toString()).commit();

            sb = new StringBuilder();
            targetList = new ArrayList<String>();
            for(int i=0; i<RESOLUTION_SET; i++)
            {
                String tmp = resolText[i].getText().toString();
                targetList.add(tmp);
                sb.append(tmp).append(",");
            }
            playListRecord.edit().putString("RESOLUTION", sb.toString()).commit();
            Log.d(TAG, "onClick start DecodeActivity");
            startActivity(intent);
            //MainActivity.this.finish();
    }

    private BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(Intent.ACTION_SCREEN_OFF)) {
                MainActivity.this.finish();
            }
        }
    };

    @Override
    public void onRequestPermissionsResult(int requestCode,
            String permissions[], int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode) {
            case READ_WRITE_CONTACTS: 
                if (grantResults.length > 0
                    && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    getAllVideoAudioFileList();
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

}
