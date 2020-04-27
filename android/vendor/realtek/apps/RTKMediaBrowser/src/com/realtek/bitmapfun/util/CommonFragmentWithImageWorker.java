package com.realtek.bitmapfun.util;

import android.app.Fragment;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

/**
 * @author Eugene Popovich
 */
public abstract class CommonFragmentWithImageWorker extends Fragment {
    protected ImageWorker mImageWorker;
    private static final String TAG = "CmnFragWithImageWorker";

    protected abstract void initImageWorker();

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        initImageWorker();
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
    }


    @Override
    public void onDestroyView() {
        super.onDestroyView();
        Log.d(TAG, "clear ImageWorker memory");
        ImageFetcher.getInstance().clearMemoryCache();
    }

    @Override
    public void onResume() {
        if (mImageWorker != null) {
            mImageWorker.setExitTasksEarly(false);
        } else {
            initImageWorker();
        }
        super.onResume();

    }

    @Override
    public void onPause() {
        super.onPause();
        ImageFetcher fetcher = ImageFetcher.getInstance();
        if (fetcher != null) {
            fetcher.setExitTasksEarly(true);
            fetcher.clearMemoryCache();
        }
    }
}
