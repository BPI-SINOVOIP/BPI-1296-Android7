package example.use.hdmirxinput;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.ContentValues;
import android.content.DialogInterface;
import android.media.tv.TvContract;
import android.media.tv.TvInputInfo;
import android.media.tv.TvInputManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.AsyncTask;
import android.util.Log;
import android.database.Cursor;
import android.widget.Toast;
import android.content.Context;
import android.content.ContentUris;
import java.io.IOException;
import android.widget.Button;
import android.view.View;
import android.widget.TextView;
import android.content.ContentProviderOperation;
import android.content.OperationApplicationException;
import java.util.List;
import java.util.ArrayList;
import example.use.hdmirxinput.R;
import android.content.SharedPreferences;

public class SetupActivity extends Activity implements View.OnClickListener {

    private static boolean DEBUG=false;
    private static final String TAG="ExampleTvInputSetupActivity";

    private TextView mLabel = null;
    private Button mOkButton = null;
    private TvInputManager mManager;
    private String mInputId;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mInputId = getIntent().getStringExtra(TvInputInfo.EXTRA_INPUT_ID);
        mLabel = (TextView) findViewById(R.id.hardware_id);
        mOkButton = (Button) findViewById(R.id.button_ok);
        mManager = (TvInputManager) getSystemService(Context.TV_INPUT_SERVICE);
        mOkButton.setOnClickListener(this);

        if(DEBUG) Log.d(TAG, "mInputId:"+mInputId);
        // do device search
        new TvInputSearchTask().execute((Void)null);
    }

    private String getHDMITvInputId(){
        TvInputInfo passThroughInput = null;
        List<TvInputInfo> list = mManager.getTvInputList();
        for(int i=0;i<list.size();i++){
            if(list.get(i).isPassthroughInput() && list.get(i).getType() == TvInputInfo.TYPE_HDMI){
                if(DEBUG) Log.d(TAG, " -- TvInputInfo: "+list.get(i));
                passThroughInput = list.get(i);
                break;
            }
        }
        if(DEBUG) Log.d(TAG, "-- passThroughInput: "+passThroughInput);
        if(passThroughInput!=null){
            String id = passThroughInput.getId();

            // save to SharedPreference
            SharedPreferences sp = ExampleTvInputService.getSP(this);
            SharedPreferences.Editor editor = sp.edit();
            editor.putString(ExampleTvInputService.KEY_HW_ID, id);
            editor.commit();

            Log.d(TAG, "getHDMITvInputId id:"+id);
            return id;
        }else{
            Log.d(TAG, "getHDMITvInputId id: N/A");
            return "N/A";
        }
    }

    private class TvInputSearchTask extends AsyncTask<Void, Void, String>{

        protected String doInBackground (Void... param){
            return getHDMITvInputId();
        }

        protected void onPostExecute(String result) {
            mLabel.setText(result);
        }
    }

    private void registerTvProvider(){
        Uri uri = TvContract.buildChannelsUriForInput(mInputId);
        if(DEBUG) Log.d(TAG, "try registerTvProvider with uri:"+uri);
        Cursor cursor = null;

        try{
           cursor = getContentResolver().query(uri, null, null, null, null);
            if (cursor != null && cursor.getCount() > 0) {
                Log.d(TAG, "uri "+uri+" already in database, return");
                return;
            }
        }catch(UnsupportedOperationException e) {
            Log.e(TAG, "catch UnsupportedOperationException");
        } finally {
            if (cursor != null) {
                if(DEBUG) Log.d(TAG,"close cursor");
                cursor.close();
            }
        }

        if(DEBUG) Log.d(TAG, "do register channel");
        // register channel info
        ContentValues values = new ContentValues();
        values.put(TvContract.Channels.COLUMN_INPUT_ID, mInputId);
        values.put(TvContract.Channels.COLUMN_DISPLAY_NUMBER, "1-1");
        values.put(TvContract.Channels.COLUMN_DISPLAY_NAME, "HDMI passthrough");
        getContentResolver().insert(/*TvContract.Channels.CONTENT_URI*/ uri, values);
    }

    public void onClick (View v){
        if(v.equals(mOkButton)){
            registerTvProvider();
            finish();
        }
    }
}
