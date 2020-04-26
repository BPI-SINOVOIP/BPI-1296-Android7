package com.android.BluetoothRTK3ddService;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothProfile;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnFocusChangeListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.NumberPicker;
import android.widget.NumberPicker.OnValueChangeListener;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;
import android.util.Log;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import android.view.WindowManager;
import android.view.Gravity;
import android.view.Display;


public class BluetoothRTK3ddActivity extends Activity implements OnClickListener{        
	private static final String TAG = "BluetoothRTK3ddActivity";    
	private static final boolean DBG = true;    
	Button btn_sendData, btn_resetData;    
	EditText et_delay;    
	Builder dialog_ValidateInput;    
	private static boolean shutterUpdateState = false, isDutyCycleChanged = false;
	private BluetoothRTK3ddProfile          m3ddProfile;

		@Override
		protected void onCreate(Bundle savedInstanceState) {
		m3ddProfile = new BluetoothRTK3ddProfile(this);
		super.onCreate(savedInstanceState);        
		setContentView(R.layout.activity_3d_sync_layout);
		setParentLayout();
		btn_sendData = (Button)findViewById(R.id.button1);
		btn_resetData = (Button)findViewById(R.id.button3);
		btn_sendData.setOnClickListener(this);
		btn_resetData.setOnClickListener(this);
		et_delay = (EditText)findViewById(R.id.editText5);
		//et_delay.setOnFocusChangeListener(this);
		et_delay.addTextChangedListener(
		new TextWatcher() 
		{
			@Override            
			public void onTextChanged(CharSequence s, int start, int before, int count) 
			{
			}            
			@Override            
			public void beforeTextChanged(CharSequence s, int start, int count, int after) 
			{
			}            

			@Override            
			public void afterTextChanged(Editable s) 
			{
				int val_delay;                
				String tmpStr;                
				try {                    
					tmpStr = et_delay.getText().toString();                    
					if(tmpStr.equals(""))                        
						return;                    
					    val_delay = Integer.parseInt(tmpStr);                
					} catch (Exception ee) {                    
					Log.e(TAG, "Error reading right close offset value. Reset to default", ee);                    
					  et_delay.setText(R.string.delay_def);                    
					  et_delay.setTextColor(Color.RED);                    
					  return;                
					}                
					if (val_delay >= 0)                    
						et_delay.setTextColor(Color.BLACK);                
					Log.d(TAG, "val_delay:" + val_delay);                
					if (val_delay < 0 || val_delay > 65535) {                    
						dialog_ValidateInput = new AlertDialog.Builder(BluetoothRTK3ddActivity.this);                    
						dialog_ValidateInput.setTitle("Invalid Input!")                        
							.setIcon(android.R.drawable.ic_dialog_alert)                        
							.setMessage("Incorrect input value entered for Delay param. "+                        
							"Enter value between 0 to 65535")                        
							.setNeutralButton("OK", new DialogInterface.OnClickListener() {                            
							public void onClick(DialogInterface dialog, int which) {                                
								et_delay.setText(R.string.delay_def);                                
								et_delay.setTextColor(Color.RED);                            
								}                        
							}
								)                        
									.setOnCancelListener(new OnCancelListener() 
									{                            
									public void onCancel(DialogInterface dialog) {                                
										et_delay.setText(R.string.delay_def);                                
										et_delay.setTextColor(Color.RED);                            
										}                        
									}).show();                    
									return;                
									}            

			}        
			}
		);    
		}            


		protected void onPause() {        
			if (DBG) {            
				Log.d(TAG, "onPause");        
		    }                
			super.onPause();    
		}    

		protected void onResume() {        
			if (DBG) {            
				Log.d(TAG, "onPause");        
			}        
			super.onResume();    
		}    

		@Override    
		public void onDestroy() {        
		    super.onDestroy();    
		}    

		@Override    
		protected void onStop(){        
		    super.onStop();        
		    if (DBG) {            
			   Log.d(TAG, "onStop");        
			}    
		}                

		private boolean isValidNum(String in) {        
			boolean ret;        
			try {            
				int t = Integer.parseInt(in);            
				ret = true;       
			} 
			catch (NumberFormatException e) {            
				ret = false;        
			}        
			return (ret);    
		}    

		private boolean validateOffsetValues() {        
			if(isValidNum(et_delay.getText().toString())){            
				return true;        
			}        
			return false;    
		}

		private void set_glass_delay(int threeD_delay){
			if(m3ddProfile == null) {
				Log.d(TAG, "Not connected to Proxy yet");
				return;
		    }

			//set dongle to 3D glass delay time
			byte[] mDelayTime = new byte[2];
			mDelayTime[0] = (byte) (threeD_delay&0x00FF);
			mDelayTime[1] = (byte) ((threeD_delay&0xFF00)>>8);
			String delaytime = String.format("%02X:%02X",mDelayTime[1],mDelayTime[0]);
			Log.d(TAG, "set_glass_delay delaytime = "+delaytime);
			m3ddProfile.GenericCommand(mDelayTime,2,2);
	  }

	  public void setParentLayout(){
       WindowManager m = getWindowManager(); 
        Display d = m.getDefaultDisplay();
          
        android.view.WindowManager.LayoutParams p = getWindow().getAttributes();
        p.height = (int) (d.getHeight() * 0.3);
        p.width = (int) (d.getWidth() * 0.3);
        p.alpha = 1.0f;
        p.dimAmount = 0.0f;
         
        getWindow().setAttributes(p);
        getWindow().setGravity(Gravity.CENTER);
    }

		@Override    
		public void onClick(View v) {    	        
		
			if(v == btn_sendData) {
			        Log.d(TAG, "onClick btn_sendData");
					int trd_delay = Integer.parseInt(et_delay.getText().toString());
					set_glass_delay(trd_delay);
			}       
			/**TODO:Reset Shutter Data:         
			* ========>>>: set default value<<<===========         
			* et_leftOpenOffset = 0         * et_leftCloseOffset = 65535         
			* et_rightOpenOffset = 0         * et_rightCloseOffset = 65535        
			* et_delay = 0         * np_dutyCycle = 100        
			*/        
			else if(v == btn_resetData) {
				et_delay.setText(getString(R.string.delay_def));        
			}
		}
}

