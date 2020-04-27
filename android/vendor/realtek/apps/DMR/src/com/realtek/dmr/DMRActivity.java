package com.realtek.dmr;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 * 
 * @see SystemUiHider
 */
public class DMRActivity extends Activity {
	private static String TAG = DMRActivity.class.getName();
	private Button mBtnStart;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_dmr);

		mBtnStart = (Button) findViewById(R.id.btnStart);

		mBtnStart.setOnClickListener(new Button.OnClickListener() {
			@Override
			public void onClick(View v) {
				Log.v(TAG, "The Start is clicked");
				Intent i = new Intent(DMRActivity.this, DMRService.class);
				startService(i);
				Toast.makeText(DMRActivity.this, "Starting DMR ...", Toast.LENGTH_SHORT).show();
			}
		});

		Button buttonStop = (Button) findViewById(R.id.btnStop);

		buttonStop.setOnClickListener(new Button.OnClickListener() {
			@Override
			public void onClick(View v) {
				Log.v(TAG, "The Stop is clicked");
				Intent i = new Intent(DMRActivity.this, DMRService.class);
				stopService(i);
				Toast.makeText(DMRActivity.this, "Stopping DMR ...", Toast.LENGTH_SHORT).show();
			}
		});
	}

	@Override
	protected void onResume() {
		super.onResume();
		if(mBtnStart!=null)
			mBtnStart.callOnClick();
	}
}
