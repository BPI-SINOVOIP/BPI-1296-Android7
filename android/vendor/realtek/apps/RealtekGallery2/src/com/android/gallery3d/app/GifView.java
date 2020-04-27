package com.android.gallery3d.app;

import android.app.Activity;
import android.content.Context;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Movie;
import android.graphics.Paint;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Handler;
import android.view.View;
import java.io.InputStream;

import com.realtek.bitmapfun.util.ImageResizer;


public class GifView extends View {
    private static final String TAG = "GifView";
    private static final int MAX_SCALE_RATIO = 4;
    private final Handler mHandler = new Handler();
    private Movie mMovie;
    private String filePath;
    private Boolean mShow = false;
    private long mMovieStart;
    private MovieWorkerTask mMovieTask;

    private final Runnable showGif = new Runnable() {
        @Override
        public void run() {
            show();
        }
    };

    public GifView(Context context) {
        super(context);
        hide();
    }

    public GifView(Context context, android.util.AttributeSet attrs) {
        this(context, attrs, 0);
        hide();
    }

    public GifView(Context context, android.util.AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        hide();
    }

    public void show() {
//        Log.d(TAG, "show()");
        mShow=true;
        invalidate();
    }

    public void hide() {
//        Log.d(TAG, "hide()");
        mHandler.removeCallbacks(showGif);
        mShow=false;
    }

    protected void setFilePath(String path) {
        Log.d(TAG, "setFilePath(" + path + ")");
        hide();
        if(!path.endsWith("gif")) {
            return;
        }
        if( path!=null && path.indexOf("file://")>=0 ) {
            path = path.substring(path.indexOf("file://")+7);
        }
        mHandler.removeCallbacks(showGif);
        mHandler.postDelayed(showGif, 500);

        if(filePath!=null && filePath.equals(path)) {
            return;
        }
              
        synchronized(this) {
            mMovie = null;
            filePath = path;
            if(mMovieTask!=null) {
                mMovieTask.cancel(true);
            }
        
            BitmapFactory.Options options = new BitmapFactory.Options();
            ImageResizer.calculateImageSize(filePath, options);
            if(options.outWidth > 1920 || options.outHeight > 1920) {
                Log.d(TAG,"Image size(width="+options.outWidth+", height="+options.outHeight+") is too large, No animation effects");
                return;
            }
                   
            mMovieTask = new MovieWorkerTask();
            mMovieTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR, filePath);
            mMovieStart=0;
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {   
        synchronized(this) {
            if (mMovie != null && mShow) {
                Paint p = new Paint();
                p.setAntiAlias(true);
                setLayerType(LAYER_TYPE_SOFTWARE, p);
                
                float ratio = 1;
                if((float)this.getWidth() / (float)mMovie.width() > (float)this.getHeight() / (float)mMovie.height())
                    ratio = (float)this.getHeight() / (float)mMovie.height();
                else 
                    ratio = (float)this.getWidth() / (float)mMovie.width();
                if(ratio>MAX_SCALE_RATIO)
                    ratio = MAX_SCALE_RATIO;
                canvas.scale(ratio,ratio);

                int width = (int)(mMovie.width() * ratio);
                int height = (int)(mMovie.height() * ratio);
                int x = (int)((this.getWidth()-width)/2/ratio);
                int y = (int)((this.getHeight()-height)/2/ratio);
                canvas.drawRoundRect(x, y, x+mMovie.width(), y+mMovie.height(), 0, 0, p);

                int duration = mMovie.duration();
                //Log.d(TAG, "duration:" + duration);
                if(duration<=0) {
                    duration = 1000;
                }
                long now = android.os.SystemClock.uptimeMillis();
                if (mMovieStart == 0) {   // first time
                    mMovieStart = now;
                }
                int relTime = (int)((now - mMovieStart) % duration);
                mMovie.setTime(relTime);
                mMovie.draw(canvas, x, y, p);
                invalidate();
            }
        }
    }

    private class MovieWorkerTask extends AsyncTask<Object, Void, Movie> {
        String dataString;
        public MovieWorkerTask()
        {
        }

        @Override
        protected void onPreExecute()
        {
            super.onPreExecute();
        }

        @Override
        protected void onCancelled()
        {
            super.onCancelled();
        }

        @Override
        protected Movie doInBackground(Object... params) {
            dataString = String.valueOf(params[0]);
            Log.d(TAG, "Movie.decodeFile:" + dataString + " [start]");

            Movie movie = null;
            try {
                if(android.os.Build.VERSION.SDK_INT>=android.os.Build.VERSION_CODES.N && dataString.contains("rtk_media")) {
                    InputStream is = ((Activity)mContext).getApplication().getContentResolver().openInputStream(Uri.parse(dataString));
                    movie = Movie.decodeStream(is);
                    if(is!=null)
                        is.close();
                }
                else {
                    movie = Movie.decodeFile(dataString);
                }
            }
            catch(Exception e) {
                Log.d(TAG, "Movie.decodeFile Error:" + e);
            }
            Log.d(TAG, "Movie.decodeFile:" + dataString + " ("+movie+")[end]");
            return movie;
        }

        @Override
        protected void onPostExecute(Movie movie) {
            mMovie = movie;
            invalidate();
            Log.d(TAG, "onPostExecute:" + dataString);
        }
    }

}

