package com.realtek.dtv;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

//import com.example.android.threadsample.R;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;

public class TaskManager {
    /*
     * Status indicators
     */
    static final int SCAN_FAILED = -1;
    static final int MANUAL_SCAN_STARTED = 1;
    static final int DOWNLOAD_COMPLETE = 2;
    static final int DECODE_STARTED = 3;
    static final int TASK_COMPLETE = 4;
    
    private static TaskManager sInstance = null;
    private final ThreadPoolExecutor DTVThreadPool;
    private final BlockingQueue<Runnable> mDTVWorkQueue;
    private Handler mHandler;

    // Sets the amount of time an idle thread will wait for a task before terminating
    private static final int KEEP_ALIVE_TIME = 1;

    // Sets the Time Unit to seconds
    private static final TimeUnit KEEP_ALIVE_TIME_UNIT;

    // Sets the initial threadpool size to 8
    private static final int CORE_POOL_SIZE = 8;

    // Sets the maximum threadpool size to 8
    private static final int MAXIMUM_POOL_SIZE = 8;

    // A static block that sets class fields
    static {

        // The time unit for "keep alive" is in seconds
        KEEP_ALIVE_TIME_UNIT = TimeUnit.SECONDS;

        // Creates a single static instance of PhotoManager
        sInstance = new TaskManager();
    }

    public TaskManager() {
        mDTVWorkQueue = new LinkedBlockingQueue<Runnable>();
        DTVThreadPool = new ThreadPoolExecutor(CORE_POOL_SIZE, MAXIMUM_POOL_SIZE,
                KEEP_ALIVE_TIME, KEEP_ALIVE_TIME_UNIT, mDTVWorkQueue);
        
        mHandler = new Handler(Looper.getMainLooper()) {

            /*
             * handleMessage() defines the operations to perform when the
             * Handler receives a new Message to process.
             */
            @Override
            public void handleMessage(Message inputMessage) {
                switch (inputMessage.what) {

                // If the download has started, sets background color to dark green
                case MANUAL_SCAN_STARTED:
                    //localView.setStatusResource(R.drawable.imagedownloading);
                    break;

                /*
                 * If the download is complete, but the decode is waiting, sets the
                 * background color to golden yellow
                 */
                case DOWNLOAD_COMPLETE:
                    // Sets background color to golden yellow
                    //localView.setStatusResource(R.drawable.decodequeued);
                    break;
                // If the decode has started, sets background color to orange
                case DECODE_STARTED:
                    //localView.setStatusResource(R.drawable.decodedecoding);
                    break;
                /*
                 * The decoding is done, so this sets the
                 * ImageView's bitmap to the bitmap in the
                 * incoming message
                 */
                case TASK_COMPLETE:
                    //localView.setImageBitmap(photoTask.getImage());
                    //recycleTask(photoTask);
                    break;
                // The download failed, sets the background color to dark red
                /*
                case DOWNLOAD_FAILED:
                    localView.setStatusResource(R.drawable.imagedownloadfailed);
                    
                    // Attempts to re-use the Task object
                    recycleTask(photoTask);
                    break;
                */
                default:
                    // Otherwise, calls the super method
                    super.handleMessage(inputMessage);
                }
            }
        };
    }

}
