/*
 * Copyright (C) 2013 Realtek Semiconductor Corp.
 * Author: stanely
 * History:
 *      2013.12.20 : First draft, we won't use this code since there is no way to add Google Account.
 */

package com.realtek.realtekinitialsettings;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ActionBar;
import android.app.ActivityManager;
import android.app.Dialog;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnKeyListener;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.MotionEvent;
import android.view.View;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;
import android.view.LayoutInflater;
import android.view.View.OnClickListener;
import android.view.Gravity;
import android.view.Window;
import android.widget.TextView;
import android.widget.ImageView;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.Toast;
import android.accounts.AccountManager;
import android.accounts.Account;
import android.accounts.AccountAuthenticatorActivity;
import android.accounts.AccountManagerFuture;
import android.accounts.AccountManagerCallback;
import android.accounts.OperationCanceledException;
import android.accounts.AuthenticatorException;
import android.util.Log;

import java.util.List;
import java.util.ArrayList;
import java.io.IOException;

/**
 *   Realtek InitialAccountSettingsActivitY is used to show the initial setting functionalities.
 */
//public class InitialAccountSettingsActivity extends Activity {
public class InitialAccountSettingsActivity extends AccountAuthenticatorActivity {
	public static String TAG = "InitialAccountSettingsActivity";
	private MenuArrayAdapterEx mAdapter;
	private ImageButton mSkipIcon;
	private ImageButton mLoginIcon;
	private TextView mLoginFailedInfo;
	private View mCurrentFocusView;
	private String mAccount;
	private String mPassword;
	private Dialog mAccountLoginDialog;
	
	private final Handler handler = new Handler();  
	
	/**
	 *  Override this function
	*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
		final Activity cbt = this;
		
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
		//ActionBar actionBar = getActionBar();
		//actionBar.hide();

        setContentView(R.layout.activity_initial_account_settings);

		mLoginFailedInfo = (TextView) findViewById(R.id.login_failed_info_text);

		mSkipIcon = (ImageButton) findViewById(R.id.skip_icon);
		mSkipIcon.setOnClickListener(new OnClickListener() {  // register the listener for the icon's click event
			@Override
			public void onClick(View view) {
				goToHomeApp();
			}
		});

		mLoginIcon = (ImageButton) findViewById(R.id.login_icon);
		mLoginIcon.setOnClickListener(new OnClickListener() {  // register the listener for the icon's click event
			@Override
			public void onClick(View view) {
				//showAccountLoginDialog();
/*				
				// TEST
				final AccountManager accMgr;
				final Account[] accounts;
				final Account account;
				final AccountManagerFuture<Bundle> amf;
				//final MainActivity cbt = this;
				String authTokenType;
				
				accMgr = AccountManager.get(cbt);
				authTokenType = "com.google";
				accounts = accMgr.getAccountsByType(authTokenType);
				account = accounts[0];
				Log.d("stanely", "account = " + account.name + ", type = " + account.type);

				amf = accMgr.getAuthToken(account, authTokenType, null, true,
					new AccountManagerCallback<Bundle>() {

						@Override
						public void run(AccountManagerFuture<Bundle> arg0) {

							try {
								Bundle result;
								Intent i;
								String token;

								result = arg0.getResult();
								if (result.containsKey(AccountManager.KEY_INTENT)) {
									i = (Intent)result.get(AccountManager.KEY_INTENT);
									if (i.toString().contains("GrantCredentialsPermissionActivity")) {
										// Will have to wait for the user to accept
										// the request therefore this will have to
										// run in a foreground application
										cbt.startActivity(i);
									} else {
										cbt.startActivity(i);
									}

								} else {
									token = (String)result.get(AccountManager.KEY_AUTHTOKEN);


									// Remember to invalidate the token if the web service rejects it
									// if(response.isTokenInvalid()){
									//    accMgr.invalidateAuthToken(authTokenType, token);
									// }

								}
							} catch (OperationCanceledException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							} catch (AuthenticatorException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							} catch (IOException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}

						}
					}, handler);
*/
			}
		});

		mCurrentFocusView = mSkipIcon;
		
		showAccountLoginDialog();
    }

	/**
	 *  Override this function
	*/
    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
		
		//doAccountRemove(); // TEST, remove old one.
    }

	/**
	 *  Override this function
	*/
    @Override
    protected void onResume() {
        super.onResume();
    }

	/**
	 *  Override this function
	*/
    @Override
    protected void onPause() {
        super.onPause();
    }

	/**
	 *  Override this function in order to move the focus between listview and imageview.
	*/
/*
	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {
		
		if(event.getAction() == KeyEvent.ACTION_DOWN) {
			switch(event.getKeyCode()) {
				case KeyEvent.KEYCODE_DPAD_UP:
				break;
				case KeyEvent.KEYCODE_DPAD_DOWN:
				break;
			}
		}
	
		return super.dispatchKeyEvent(event);
	}
*/

	/**
	 *  Override this function in order to move the focus between listview and imageview.
	*/
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {

		switch(keyCode) {
			case KeyEvent.KEYCODE_DPAD_LEFT:
            	Log.d(TAG,"onKeyDown: LEFT");
				return true;
			case KeyEvent.KEYCODE_DPAD_RIGHT:
            	Log.d(TAG,"onKeyDown: RIGHT");
				return true;
			case KeyEvent.KEYCODE_DPAD_UP:
            	Log.d(TAG,"onKeyDown: UP");
				if(mCurrentFocusView == mSkipIcon)  { // no need to do UP in mSkipIcon
					return true;
				} else if (mCurrentFocusView == mLoginIcon) {
					mSkipIcon.setFocusable(true);
					mSkipIcon.requestFocus();
					mCurrentFocusView = mSkipIcon;
					return true;	
				}
				break;
			case KeyEvent.KEYCODE_DPAD_DOWN:
            	Log.d(TAG,"onKeyDown: DOWN");
				if(mCurrentFocusView == mSkipIcon) {
					mLoginIcon.setFocusable(true);
					mLoginIcon.requestFocus();
					mCurrentFocusView = mLoginIcon;
				}
				return true;
			default:
				break;
		}		

		return super.onKeyDown(keyCode, event);
	}

	/**
     *   Show the dialog to let user input the user account and password.
	 *  
     */
	private boolean showAccountLoginDialog() {
		mAccountLoginDialog = new Dialog(InitialAccountSettingsActivity.this, R.style.AccountDialogTheme);
		mAccountLoginDialog.setContentView(R.layout.dialog_account);

		// adjust the attributes of dialog in order to fit the design
		Window dialogWin = mAccountLoginDialog.getWindow();
		WindowManager.LayoutParams lp = dialogWin.getAttributes();
		//dialogWin.setGravity(Gravity.LEFT | Gravity.TOP);
		dialogWin.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
		//lp.x = 460;
		//lp.y = 300;
		lp.width = 1000;
		lp.height = 480;
		dialogWin.setAttributes(lp);
		
		mAccountLoginDialog.setOnKeyListener(dialogOnKeyListener);

		// set title
		TextView dialog_account_login_title = (TextView) mAccountLoginDialog.findViewById(R.id.dialog_account_login_title);
		dialog_account_login_title.setText(getResources().getString(R.string.hint_login_icon_text));
	
		// set password display to "*", not "." 
		EditText dialog_password_input = (EditText) mAccountLoginDialog.findViewById(R.id.dialog_password_input);
		dialog_password_input.setTransformationMethod(new AsteriskPasswordTransformationMethod());
	
		mAccountLoginDialog.show();		
		return true;
	}

	/**
     *   Use user account and password to login
	 *  
     */	
	private boolean doAccountLogin(String email, String password) {
		//Account
		Account account = new Account(email, "com.google");  

		Bundle userdata = new Bundle();   
		userdata.putString("SERVER", "SERVER_TEST");  
		//AccountManager  
		AccountManager am = AccountManager.get(this);  

		if (am.addAccountExplicitly(account, password, userdata) == true) {  // add account successfully.  
			Bundle result = new Bundle();  
			result.putString(AccountManager.KEY_ACCOUNT_NAME, email);  
			result.putString(AccountManager.KEY_ACCOUNT_TYPE, "com.google");  
			setAccountAuthenticatorResult(result);  
			
			mAccountLoginDialog.cancel();	
			// exit the RealtekInitialSettings app and go to Home.
			Intent intent = new Intent();
			intent.setAction(Intent.ACTION_MAIN);
			intent.addCategory(Intent.CATEGORY_HOME);
			startActivity(intent);			
			finish();  			
		} else {
			// remove the dialog.
			mAccountLoginDialog.cancel();	
			mCurrentFocusView = mLoginIcon;
			mLoginIcon.requestFocus();
			return false;
		}
		return true;
	}

	/**
     *   Remove account.
	 *  
     */		
	private boolean doAccountRemove() {
		AccountManager am = AccountManager.get(this);
		Account[] accounts = am.getAccounts();
		for (int index = 0; index < accounts.length; index++) {
			am.removeAccount(accounts[index], null, null);
		}		
		return true;
	}
	
    /**
     *  Helper function to go to Home page - Launcher app
    */
    private void goToHomeApp() {
		// exit the RealtekInitialSettings app and go to Home.
		Intent intent = new Intent();
		intent.setAction(Intent.ACTION_MAIN);
		intent.addCategory(Intent.CATEGORY_HOME);
		startActivity(intent);
		finish();
	}	

    /**
     *  An interface to register in order to monitor onKey() function.
    */	
	private OnKeyListener dialogOnKeyListener = new OnKeyListener() {
		@Override
		public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) {
			if(keyCode == KeyEvent.KEYCODE_ENTER && event.getAction() == KeyEvent.ACTION_DOWN) {
				EditText accountText = (EditText) mAccountLoginDialog.findViewById(R.id.dialog_account_input);
				mAccount = accountText.getText().toString();
				//accountText.requestFocus();

				EditText passwordText = (EditText) mAccountLoginDialog.findViewById(R.id.dialog_password_input);
				mPassword = passwordText.getText().toString();

				Log.d("stanely", "currentfocus = " + mAccountLoginDialog.getCurrentFocus() + ", accountText = " + accountText + ", passwordText = " + passwordText);

				// move the focus
				if(accountText == mAccountLoginDialog.getCurrentFocus()) {
					accountText.clearFocus();
					passwordText.requestFocus();
				} else {
					// We have account and possworkd, let's do registration this account.
					Log.d(TAG, "onKey[1], mAccount = " + mAccount + ", mPassword = " + mPassword);
					if(mAccount != null & mPassword != null) {
						Log.d(TAG, "onKey[2], mAccount = " + mAccount + ", mPassword = " + mPassword);

						doAccountLogin(mAccount, mPassword);
					}
				}

				return true;
			} else 
				return false;
		}
	};

}
