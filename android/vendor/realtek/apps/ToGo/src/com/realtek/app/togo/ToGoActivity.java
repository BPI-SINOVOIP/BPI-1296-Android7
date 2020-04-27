package com.realtek.app.togo;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;
import android.util.Log;
import android.content.ServiceConnection;
import android.content.ComponentName;
import android.os.IBinder;
import android.content.Context;
import android.content.Intent;
import com.realtek.app.IToGoManager;
import android.os.RemoteException;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileDescriptor;
import java.io.IOException;
import java.util.Date;
import java.text.SimpleDateFormat;
import android.util.Log;
import android.os.Environment;
import android.os.ParcelFileDescriptor;

public class ToGoActivity extends Activity {
	private final static String TAG = "ToGoActivity";
	private Button btBindService;
	private Button btUnbindService;
	private Button btQueryall;
	private Button btInitCamera;
	private Button btInitFile;
	private Button btInitDtv;
	private Button btConfigOutToFile;
	private Button btStartClient0;
	private Button btStartClient1;
	private Button btStopClient0;
	private Button btStopClient1;
	private ToGoConnection mConn;
	private IToGoManager mToGoMgr;
	private int mToGoHandle[];
	private int mState[];	// 0-IDLE, 1-INITED, 2-RUN, 3-STOP

	private void initVariables() {
		mToGoHandle = new int [2];
		mToGoHandle[0] = mToGoHandle[1] = -1;

		mState = new int [2];
		mState[0] = mState[1] = 0;
	}

	File getOutputMediaFile() {
        // To be safe, you should check that the SDCard is mounted
        // using Environment.getExternalStorageState() before doing this.

        String state = Environment.getExternalStorageState();
        if (!Environment.MEDIA_MOUNTED.equals(state)) {
            return null;
        }

        File mediaStorageDir = new File(Environment.getExternalStoragePublicDirectory(
            Environment.DIRECTORY_DCIM), "TestToGo");
        // This location works best if you want the created images to be shared
        // between applications and persist after your app has been uninstalled.

        // Create the storage directory if it does not exist
        if (! mediaStorageDir.exists()) {
            if (! mediaStorageDir.mkdirs()) {
                Log.e(TAG, "Failed to create directory for pictures/video.");
                return null;
            }
        }

        // Create a media file name
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        File mediaFile;

        mediaFile = new File(mediaStorageDir.getPath() + File.separator +
            "VID_"+ timeStamp + ".ts");

        return mediaFile;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        initVariables();

        btBindService = (Button)findViewById(R.id.btBindService);
        btBindService.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.v(TAG, "Bind Service.");

				mConn = new ToGoConnection();
				Intent intent = new Intent("com.realtek.app.togo.ToGoAppService");
				bindService(intent, mConn, Context.BIND_AUTO_CREATE);

				Toast.makeText(getApplicationContext(), "Bind Service", Toast.LENGTH_SHORT).show();
			}
        });
        
        btUnbindService = (Button)findViewById(R.id.btUnbindService);
        btUnbindService.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.v(TAG, "Unbind Service.");

				unbindService(mConn);

				Toast.makeText(getApplicationContext(), "Unbind Service", Toast.LENGTH_SHORT).show();
			}    	
        });
        
        btQueryall = (Button)findViewById(R.id.btQueryall);
        btQueryall.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.v(TAG, "Queryall.");
				Toast.makeText(getApplicationContext(), "Queryall", Toast.LENGTH_SHORT).show();
			}    	
        });
        
        btInitCamera = (Button)findViewById(R.id.btInitCamera);
        btInitCamera.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.v(TAG, "Init Camera.");
				
				if (mToGoMgr == null) {
					Log.e(TAG, "mToGoMgr==NULL.");
					return;
				}
				
				try {
					if ((mToGoHandle[0] = mToGoMgr.init(0, 0)) < 0) {
						Log.e(TAG, "init() NG.");
						return;
					}
				} catch (RemoteException e) {
					Log.e(TAG, "RemoteException!");
					return;
				}
				
				Log.v(TAG, "init OK.m.ToGoHandle=" + mToGoHandle[0]);
				mState[0] = 1;
				
				Toast.makeText(getApplicationContext(), "Init Camera", Toast.LENGTH_SHORT).show();
			}    	
        });
        
        btInitFile = (Button)findViewById(R.id.btInitFile);
        btInitFile.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.v(TAG, "Init File.");
				
				if (mToGoMgr == null) {
					Log.e(TAG, "mToGoMgr==NULL.");
					return;
				}
				
				try {
					if ((mToGoHandle[0] = mToGoMgr.init(2, -1)) < 0) {
						Log.e(TAG, "init() NG.");
						return;
					}
				} catch (RemoteException e) {
					Log.e(TAG, "RemoteException!");
					return;
				}
				
				Log.v(TAG, "init OK.m.ToGoHandle=" + mToGoHandle[0]);
				mState[0] = 1;
				
				Toast.makeText(getApplicationContext(), "Init File", Toast.LENGTH_SHORT).show();
			}    	
        });
        
        btInitDtv = (Button)findViewById(R.id.btInitDtv);
        btInitDtv.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.v(TAG, "Init DTV.");
				
				if (mToGoMgr == null) {
					Log.e(TAG, "mToGoMgr==NULL.");
					return;
				}
				
				try {
					if ((mToGoHandle[0] = mToGoMgr.init(3, -1)) < 0) {
						Log.e(TAG, "init() NG.");
						return;
					}
				} catch (RemoteException e) {
					Log.e(TAG, "RemoteException!");
					return;
				}
				
				Log.v(TAG, "init OK.m.ToGoHandle=" + mToGoHandle[0]);
				mState[0] = 1;
				
				Toast.makeText(getApplicationContext(), "Init DTV", Toast.LENGTH_SHORT).show();
			}    	
        });
        
        btConfigOutToFile = (Button)findViewById(R.id.btConfigOutToFile);
        btConfigOutToFile.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.v(TAG, "Config Out to File.");
				
				if ((mToGoMgr == null) || (mToGoHandle[0] < 0)) {
					Log.e(TAG, "mToGoMgr==NULL || mToGoHandle<0.");
					return;
				}

				if (mState[0] != 1) {
					Log.e(TAG, "state ISNOT INITED.state=" + mState[0]);
					return;
				}
				
				File outputFile = getOutputMediaFile();
                Log.d(TAG, "File name: " + outputFile.toString());

                ParcelFileDescriptor pfd = null;
                try {
                    pfd = ParcelFileDescriptor.open(new File(outputFile.toString()), ParcelFileDescriptor.MODE_CREATE|ParcelFileDescriptor.MODE_READ_WRITE);
                    if (pfd != null) {
                        Log.d(TAG, "get pfd OK.");
                    }
                    else {
                        Log.e(TAG, "get pfd NG.");
                        return;
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                    return;
                }

                try {
                	mToGoMgr.setOutputFD(mToGoHandle[0], pfd);
                } catch (RemoteException e) {
					Log.e(TAG, "RemoteException!");
					return;
				}
				
				Toast.makeText(getApplicationContext(), "Config Out to File", Toast.LENGTH_SHORT).show();
			}    	
        });
        
        btStartClient0 = (Button)findViewById(R.id.btStartClient0);
        btStartClient0.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.v(TAG, "Start Client 0.");
				
				if ((mToGoMgr == null) || (mToGoHandle[0] < 0)) {
					Log.e(TAG, "mToGoMgr==NULL || mToGoHandle<0.");
					return;
				}

				if (mState[0] != 1) {
					Log.e(TAG, "state ISNOT INITED.state=" + mState[0]);
					return;
				}
				
				try {
					if (mToGoMgr.start(mToGoHandle[0]) < 0) {
						Log.e(TAG, "start() NG.");
						return;
					}
				} catch (RemoteException e) {
					Log.e(TAG, "RemoteException!");
					return;
				}

				mState[0] = 2;
				
				Toast.makeText(getApplicationContext(), "Start Client 0", Toast.LENGTH_SHORT).show();
			}    	
        });
        
        btStartClient1 = (Button)findViewById(R.id.btStartClient1);
        btStartClient1.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.v(TAG, "Start Client 1.");
				Toast.makeText(getApplicationContext(), "Start Client 1", Toast.LENGTH_SHORT).show();
			}    	
        });
        
        btStopClient0 = (Button)findViewById(R.id.btStopClient0);
        btStopClient0.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.v(TAG, "Stop Client 0.");
				
				if ((mToGoMgr == null) || (mToGoHandle[0] < 0)) {
					Log.e(TAG, "mToGoMgr==NULL || mToGoHandle<0.");
					return;
				}

				if (mState[0] != 2) {
					Log.e(TAG, "state ISNOT RUN.state=" + mState[0]);
					return;
				}

				try {
					mToGoMgr.stop(mToGoHandle[0]);
					mToGoMgr.uninit(mToGoHandle[0]);
				} catch (RemoteException e) {
					Log.e(TAG, "RemoteException!");
					return;
				}

				mState[0] = 0;
				
				Toast.makeText(getApplicationContext(), "Stop Client 0", Toast.LENGTH_SHORT).show();
			}    	
        });
        
        btStopClient1 = (Button)findViewById(R.id.btStopClient1);
        btStopClient1.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.v(TAG, "Stop Client 1.");
				Toast.makeText(getApplicationContext(), "Stop Client 1", Toast.LENGTH_SHORT).show();
			}    	
        });
    }

    class ToGoConnection implements ServiceConnection {

		@Override
		public void onServiceConnected(ComponentName arg0, IBinder arg1) {
			// TODO Auto-generated method stub
			Log.v(TAG, "connected.");
			mToGoMgr = IToGoManager.Stub.asInterface(arg1);
		}

		@Override
		public void onServiceDisconnected(ComponentName arg0) {
			// TODO Auto-generated method stub
			Log.v(TAG, "disconnected.");
			mToGoMgr = null;
		}
		
	}

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.to_go, menu);
        return true;
    }
    
}
