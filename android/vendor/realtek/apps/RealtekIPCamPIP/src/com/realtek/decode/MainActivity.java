package com.realtek.decode;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;

public class MainActivity extends Activity implements OnClickListener {

	public static final String TAG = "MainActivity";
	private Button btnRecGrid1 = null;
	private Button btnRecGrid2 = null;
	private Button btnRecGrid3 = null;
	private Button btnRecGrid4 = null;
	
    private CheckBox chbSearchI = null;
    
    private SharedPreferences playListRecord = null;
	private ArrayList<String> playList = new ArrayList<String>();
	private EditText listText[] = new EditText[4];
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		btnRecGrid1 = (Button) findViewById(R.id.btnRecGrid1);
		btnRecGrid2 = (Button) findViewById(R.id.btnRecGrid2);
		btnRecGrid3 = (Button) findViewById(R.id.btnRecGrid3);
		btnRecGrid4 = (Button) findViewById(R.id.btnRecGrid4);
        
		listText[0] = (EditText) findViewById(R.id.sourceText1);
		listText[1] = (EditText) findViewById(R.id.sourceText2);
		listText[2] = (EditText) findViewById(R.id.sourceText3);
		listText[3] = (EditText) findViewById(R.id.sourceText4);
        
        chbSearchI = (CheckBox) findViewById(R.id.chbSearchI);
        if(chbSearchI != null) {
            chbSearchI.setChecked(false);
        }
        if(btnRecGrid1 != null) {
            btnRecGrid1.setOnClickListener(this);
        }
        if(btnRecGrid2 != null) {
            btnRecGrid2.setOnClickListener(this);
        }
        if(btnRecGrid3 != null) {
            btnRecGrid3.setOnClickListener(this);
        }
        if(btnRecGrid4 != null) {
            btnRecGrid4.setOnClickListener(this);
        }
        playListRecord = getSharedPreferences("IPCamPIP", 0);
        String temp = playListRecord.getString("PLAYLIST", "");
        if(!temp.equals("")) {
            String[] playlists = temp.split(",");
            for (int i = 0; i < 4; i++) {
                if (!playlists[i].equals("")) {
                    Log.d(TAG, "Set Source " + i + " Path " + playlists[i]);
                    listText[i].setText(playlists[i]);
                }
            }
        }
	}
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
    @Override
    public void onClick(View v) {
        Intent intent = new Intent();
        intent.setClass(MainActivity.this, DecodeActivity.class);
        int intGridNum = 0;
        
        switch (v.getId()) {
        	
        	case R.id.btnRecGrid1:
        		intGridNum = 1;
        		break;
        	case R.id.btnRecGrid2:
        		intGridNum = 2;
        		break;
        	case R.id.btnRecGrid3:
        		intGridNum = 3;
        		break;
        	case R.id.btnRecGrid4:
        		intGridNum = 4;
        		break;
        }
        StringBuilder sb = new StringBuilder();
        playList.clear();
        for(int i=0; i<4; i++) {
            String tmp = listText[i].getText().toString();
            playList.add(tmp);
            sb.append(tmp).append(",");
        }
        
        SharedPreferences.Editor editor = playListRecord.edit();       
        if(editor != null) {
            editor.putString("PLAYLIST", sb.toString()).commit();
        }
        intent.putStringArrayListExtra("DATA_URL", playList);        
        
        intent.putExtra("DATA_INT", intGridNum);
        
        if(chbSearchI.isChecked())
            intent.putExtra("SEARCH_I", 1);
        else
            intent.putExtra("SEARCH_I", 0);
        Log.d(TAG, "onClick start DecodeActivity");
        startActivity(intent);
    }
}
