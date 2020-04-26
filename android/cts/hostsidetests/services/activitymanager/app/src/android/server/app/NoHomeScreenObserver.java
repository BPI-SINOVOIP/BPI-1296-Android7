package android.server.app;

import android.app.Activity;
import android.content.res.Resources;
import android.os.Bundle;
import android.util.Log;

public class NoHomeScreenObserver extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        boolean support = false;
        try {
            int id = Resources.getSystem().getIdentifier("config_noHomeScreen", "bool", "android");
            support = Resources.getSystem().getBoolean(id);
        } catch (android.content.res.Resources.NotFoundException e) {
            // Ignore the exception.
        }
        Log.i(getClass().getSimpleName(), "HEAD=OK");
        Log.i(getClass().getSimpleName(), "config_noHomeScreen=" + support);
    }
}

