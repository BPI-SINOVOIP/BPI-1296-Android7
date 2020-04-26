/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.widget;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.media.AudioManager;
import android.media.MediaFormat;
import android.media.MediaPlayer;
import android.media.MediaPlayer.NaviInfo;
import android.media.MediaPlayer.BDInfo;
import android.media.MediaPlayer.NaviType;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnSeekCompleteListener;
import android.media.MediaPlayer.OnSeekResultListener;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnInfoListener;
import android.media.MediaPlayer.OnSpeedChangedListener;
import android.media.MediaPlayer.TrackInfo;
import android.media.Metadata;
import android.media.SubtitleController;
import android.media.SubtitleTrack.RenderingWidget;
import android.media.WebVttRenderer;
import android.net.Uri;
import android.os.Looper;
import android.os.Parcel;
import android.util.AttributeSet;
import android.util.Log;
import android.util.Pair;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.accessibility.AccessibilityEvent;
import android.view.accessibility.AccessibilityNodeInfo;
import android.widget.RtkMediaController.MediaPlayerControl;
import android.widget.RtkMediaController.retSubtitle;
import android.widget.RtkMediaController.retAudioTrack;

import java.io.IOException;
import java.io.InputStream;
import java.util.Map;
import java.util.Vector;

/**
 * Displays a video file.  The RtkVideoView class
 * can load images from various sources (such as resources or content
 * providers), takes care of computing its measurement from the video so that
 * it can be used in any layout manager, and provides various display options
 * such as scaling and tinting.
 */
public class RtkVideoView extends SurfaceView
        implements MediaPlayerControl, SubtitleController.Anchor {
    private String TAG = "RtkVideoView";
    // settable by the client
    private Uri         mUri;
    private Map<String, String> mHeaders;

    // all possible internal states
    private static final int STATE_ERROR              = -1;
    private static final int STATE_IDLE               = 0;
    private static final int STATE_PREPARING          = 1;
    private static final int STATE_PREPARED           = 2;
    private static final int STATE_PLAYING            = 3;
    private static final int STATE_PAUSED             = 4;
    private static final int STATE_PLAYBACK_COMPLETED = 5;

    // mCurrentState is a RtkVideoView object's current state.
    // mTargetState is the state that a method caller intends to reach.
    // For instance, regardless the RtkVideoView object's current state,
    // calling pause() intends to bring the object to a target state
    // of STATE_PAUSED.
    private int mCurrentState = STATE_IDLE;
    private int mTargetState  = STATE_IDLE;

    // All the stuff we need for playing and showing a video
    private SurfaceHolder mSurfaceHolder = null;
    private MediaPlayer mMediaPlayer = null;
    private int         mAudioSession;
    protected int       mVideoWidth;
    protected int       mVideoHeight;
    private int         mSurfaceWidth;
    private int         mSurfaceHeight;
    private RtkMediaController mMediaController;
    private OnCompletionListener mOnCompletionListener;
    private OnSeekCompleteListener mOnSeekCompleteListener;
    private OnSeekResultListener mOnSeekResultListener;
    private OnSpeedChangedListener mOnSpeedChangedListener;
    private MediaPlayer.OnPreparedListener mOnPreparedListener;
    private int         mCurrentBufferPercentage;
    private OnErrorListener mOnErrorListener;
    private OnInfoListener  mOnInfoListener;
    private int         mSeekWhenPrepared;  // recording the seek position while preparing
    private boolean     mCanPause;
    private boolean     mCanSeekBack;
    private boolean     mCanSeekForward;
    private boolean     mCanSeek;
    private int mAutoScanExtSubtitle=1;
    private int mForceUseRTMediaPlayer = MediaPlayer.FORCE_NONE;
    private int mVideoBufferSize = -1;
    private boolean mVideoScaledSize = false;
    private boolean     mBroadcastStatus = false;
    private boolean     mContinuousPlay = false;  // TRUE => PlayMode + Stop will not be broadcasted
    private String      mSourceFrom = null;       // Local / NetWork / DLNA / HTTP / DirectConnect / ..... etc

    private static final int CAN_PAUSE_FLAG = 0x01;
    private static final int CAN_SEEK_FWD_FLAG = 0x02;
    private static final int CAN_SEEK_BWD_FLAG = 0x04;
    private static final int CAN_SEEK = 0x08;

    private static final String RTK_PLAYBACK_STATUS = "rtk.provider.playback.STATUS";

    /** Subtitle rendering widget overlaid on top of the video. */
    private RenderingWidget mSubtitleWidget;

    /** Listener for changes to subtitle data, used to redraw when needed. */
    private RenderingWidget.OnChangedListener mSubtitlesChangedListener;

    public RtkVideoView(Context context) {
        super(context);
        initRtkVideoView();
    }

    public RtkVideoView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
        initRtkVideoView();
    }

    public RtkVideoView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        initRtkVideoView();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        //Log.i("@@@@", "onMeasure(" + MeasureSpec.toString(widthMeasureSpec) + ", "
        //        + MeasureSpec.toString(heightMeasureSpec) + ")");

        int width = getDefaultSize(mVideoWidth, widthMeasureSpec);
        int height = getDefaultSize(mVideoHeight, heightMeasureSpec);
        if (mVideoWidth > 0 && mVideoHeight > 0) {

            int widthSpecMode = MeasureSpec.getMode(widthMeasureSpec);
            int widthSpecSize = MeasureSpec.getSize(widthMeasureSpec);
            int heightSpecMode = MeasureSpec.getMode(heightMeasureSpec);
            int heightSpecSize = MeasureSpec.getSize(heightMeasureSpec);

            if (widthSpecMode == MeasureSpec.EXACTLY && heightSpecMode == MeasureSpec.EXACTLY) {
                // the size is fixed
                width = widthSpecSize;
                height = heightSpecSize;

                // for compatibility, we adjust size based on aspect ratio
                if ( mVideoWidth * height  < width * mVideoHeight ) {
                    //Log.i("@@@", "image too wide, correcting");
                    width = height * mVideoWidth / mVideoHeight;
                } else if ( mVideoWidth * height  > width * mVideoHeight ) {
                    //Log.i("@@@", "image too tall, correcting");
                    height = width * mVideoHeight / mVideoWidth;
                }
            } else if (widthSpecMode == MeasureSpec.EXACTLY) {
                // only the width is fixed, adjust the height to match aspect ratio if possible
                width = widthSpecSize;
                height = width * mVideoHeight / mVideoWidth;
                if (heightSpecMode == MeasureSpec.AT_MOST && height > heightSpecSize) {
                    // couldn't match aspect ratio within the constraints
                    height = heightSpecSize;
                }
            } else if (heightSpecMode == MeasureSpec.EXACTLY) {
                // only the height is fixed, adjust the width to match aspect ratio if possible
                height = heightSpecSize;
                width = height * mVideoWidth / mVideoHeight;
                if (widthSpecMode == MeasureSpec.AT_MOST && width > widthSpecSize) {
                    // couldn't match aspect ratio within the constraints
                    width = widthSpecSize;
                }
            } else {
                // neither the width nor the height are fixed, try to use actual video size
                width = mVideoWidth;
                height = mVideoHeight;
                if (heightSpecMode == MeasureSpec.AT_MOST && height > heightSpecSize) {
                    // too tall, decrease both width and height
                    height = heightSpecSize;
                    width = height * mVideoWidth / mVideoHeight;
                }
                if (widthSpecMode == MeasureSpec.AT_MOST && width > widthSpecSize) {
                    // too wide, decrease both width and height
                    width = widthSpecSize;
                    height = width * mVideoHeight / mVideoWidth;
                }
            }
        } else {
            // no size yet, just adopt the given spec sizes
        }
        setMeasuredDimension(width, height);
    }

    @Override
    public void onInitializeAccessibilityEvent(AccessibilityEvent event) {
        super.onInitializeAccessibilityEvent(event);
        event.setClassName(RtkVideoView.class.getName());
    }

    @Override
    public void onInitializeAccessibilityNodeInfo(AccessibilityNodeInfo info) {
        super.onInitializeAccessibilityNodeInfo(info);
        info.setClassName(RtkVideoView.class.getName());
    }

    public int resolveAdjustedSize(int desiredSize, int measureSpec) {
        return getDefaultSize(desiredSize, measureSpec);
    }

    private void initRtkVideoView() {
        mVideoWidth = 0;
        mVideoHeight = 0;
        getHolder().addCallback(mSHCallback);
        getHolder().setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
        setFocusable(true);
        setFocusableInTouchMode(true);
        requestFocus();
        mPendingSubtitleTracks = new Vector<Pair<InputStream, MediaFormat>>();
        mCurrentState = STATE_IDLE;
        mTargetState  = STATE_IDLE;
    }
//========= added by RTK ====================
    /**
     * This function must be called before calling setVideoPath
     * or setVideoURI, otherwise, it can't take effect.
     */
    public void useRTMediaPlayer(int forceType) {
        mForceUseRTMediaPlayer = forceType;
    }
//============================================

    //========= added by RTK ====================
    /**
     * This function must be called before calling prepareAsync,
     * otherwise, it can't take effect.
     */
    public void enableVideoScaledSize(boolean b) {
        mVideoScaledSize = b;
    }
//============================================
    public void setVideoPath(String path) {
        setVideoURI(Uri.parse(path));
    }

    public void setVideoURI(Uri uri) {
        setVideoURI(uri, null);
    }

    /**
     * @hide
     */
    public void setVideoURI(Uri uri, Map<String, String> headers) {
        mUri = uri;
        mHeaders = headers;
        mSeekWhenPrepared = 0;
        openVideo();
        requestLayout();
        invalidate();
    }

    /**
     * Adds an external subtitle source file (from the provided input stream.)
     *
     * Note that a single external subtitle source may contain multiple or no
     * supported tracks in it. If the source contained at least one track in
     * it, one will receive an {@link MediaPlayer#MEDIA_INFO_METADATA_UPDATE}
     * info message. Otherwise, if reading the source takes excessive time,
     * one will receive a {@link MediaPlayer#MEDIA_INFO_SUBTITLE_TIMED_OUT}
     * message. If the source contained no supported track (including an empty
     * source file or null input stream), one will receive a {@link
     * MediaPlayer#MEDIA_INFO_UNSUPPORTED_SUBTITLE} message. One can find the
     * total number of available tracks using {@link MediaPlayer#getTrackInfo()}
     * to see what additional tracks become available after this method call.
     *
     * @param is     input stream containing the subtitle data.  It will be
     *               closed by the media framework.
     * @param format the format of the subtitle track(s).  Must contain at least
     *               the mime type ({@link MediaFormat#KEY_MIME}) and the
     *               language ({@link MediaFormat#KEY_LANGUAGE}) of the file.
     *               If the file itself contains the language information,
     *               specify "und" for the language.
     */
    public void addSubtitleSource(InputStream is, MediaFormat format) {
        if (mMediaPlayer == null) {
            mPendingSubtitleTracks.add(Pair.create(is, format));
        } else {
            try {
                mMediaPlayer.addSubtitleSource(is, format);
            } catch (IllegalStateException e) {
                mInfoListener.onInfo(
                        mMediaPlayer, MediaPlayer.MEDIA_INFO_UNSUPPORTED_SUBTITLE, 0);
            }
        }
    }

    private Vector<Pair<InputStream, MediaFormat>> mPendingSubtitleTracks;

    // add by RTK ///
    
    public void setBroadcastStatus(boolean broadcastStatus)
    {
        mBroadcastStatus = broadcastStatus;
    }
    
    
    public void stopPlayback() {
        if (mMediaPlayer != null) {
            mMediaPlayer.stop();
            mMediaPlayer.release();
            mMediaPlayer = null;
            mCurrentState = STATE_IDLE;
            mTargetState  = STATE_IDLE;
            mContinuousPlay = false;
            if (mBroadcastStatus)
            {
                //// Broadcast playback status for DMR usage////////////
                Log.d(TAG, "Broadcast PlayMode: Stop");
                Intent i = new Intent(RTK_PLAYBACK_STATUS)
                                .putExtra("PlayMode", "Stop");
                i.putExtra("PlayType", "Video");
                if (null != mSourceFrom) {
                    i.putExtra("SourceFrom", mSourceFrom);
                }
                mContext.sendBroadcast(i);
                ////////////////////////////////////////////////
            }
        }
    }

    private void openVideo() {
        if (mUri == null || mSurfaceHolder == null) {
            // not ready for playback just yet, will try again later
            return;
        }
        // Tell the music playback service to pause
        // TODO: these constants need to be published somewhere in the framework.
        Intent i = new Intent("com.android.music.musicservicecommand");
        i.putExtra("command", "pause");
        mContext.sendBroadcast(i);

        // we shouldn't clear the target state, because somebody might have
        // called start() previously
        release(false);
        try {
            mMediaPlayer = new MediaPlayer();
            // TODO: create SubtitleController in MediaPlayer, but we need
            // a context for the subtitle renderers
            final Context context = getContext();
            final SubtitleController controller = new SubtitleController(
                    context, mMediaPlayer.getMediaTimeProvider(), mMediaPlayer);
            controller.registerRenderer(new WebVttRenderer(context));
            mMediaPlayer.setSubtitleAnchor(controller, this);

            if (mAudioSession != 0) {
                mMediaPlayer.setAudioSessionId(mAudioSession);
            } else {
                mAudioSession = mMediaPlayer.getAudioSessionId();
            }
            mMediaPlayer.setOnPreparedListener(mPreparedListener);
            mMediaPlayer.setOnVideoSizeChangedListener(mSizeChangedListener);
            mMediaPlayer.setOnCompletionListener(mCompletionListener);
            mMediaPlayer.setOnSeekCompleteListener(mSeekCompleteListener);
            mMediaPlayer.setOnSeekResultListener(mSeekResultListener);
            mMediaPlayer.setOnErrorListener(mErrorListener);
            mMediaPlayer.setOnInfoListener(mInfoListener);
            mMediaPlayer.setOnBufferingUpdateListener(mBufferingUpdateListener);
            mMediaPlayer.setOnSpeedChangedListener(mSpeedChangedListener);
            mCurrentBufferPercentage = 0;
            mMediaPlayer.useRTMediaPlayer(mForceUseRTMediaPlayer);
            //ATM-801
            if(mForceUseRTMediaPlayer==MediaPlayer.FORCE_RT_MEDIAPLAYER){
                if(mUri.getScheme()!=null && mUri.getScheme().equals("content")){
                    String uriPath = mUri.getEncodedPath();
                    final String keyword = "rtk_media";
                    String fallbackPath = uriPath.substring(uriPath.indexOf(keyword)+keyword.length());
                    mUri = Uri.parse("file://"+fallbackPath);
                    Log.d(TAG, "uri for DvdPlayer: "+mUri);
                }
            }
            mMediaPlayer.setDataSource(mContext, mUri, mHeaders);
            Parcel value = Parcel.obtain();
            value.writeInt(mVideoBufferSize);
            mMediaPlayer.setParameter(MediaPlayer.KEY_PARAMETER_VIDEO_BUFFER_SIZE, value);
            //if (mUri.toString().startsWith("http") == false)
                //mMediaPlayer.setAutoScanExtSubtitle(mAutoScanExtSubtitle);

            if(mForceUseRTMediaPlayer==MediaPlayer.FORCE_RT_MEDIAPLAYER && mVideoScaledSize){
                Log.i(TAG, String.format("getMeasuredWxH=%dx%d", getMeasuredWidth(), getMeasuredHeight()));
                mMediaPlayer.setVideoScaledSize(getMeasuredWidth(), getMeasuredHeight());
            }
            
            mMediaPlayer.setDisplay(mSurfaceHolder);
            mMediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
            mMediaPlayer.setScreenOnWhilePlaying(true);
            mMediaPlayer.prepareAsync();

            for (Pair<InputStream, MediaFormat> pending: mPendingSubtitleTracks) {
                try {
                    mMediaPlayer.addSubtitleSource(pending.first, pending.second);
                } catch (IllegalStateException e) {
                    mInfoListener.onInfo(
                            mMediaPlayer, MediaPlayer.MEDIA_INFO_UNSUPPORTED_SUBTITLE, 0);
                }
            }

            // we don't set the target state here either, but preserve the
            // target state that was there before.
            mCurrentState = STATE_PREPARING;
            Log.d(TAG, "232 : attachMediaController");
            attachMediaController();
        } catch (IOException ex) {
            Log.w(TAG, "Unable to open content: " + mUri, ex);
            mCurrentState = STATE_ERROR;
            mTargetState = STATE_ERROR;
            mErrorListener.onError(mMediaPlayer, MediaPlayer.MEDIA_ERROR_UNKNOWN, 0);
            return;
        } catch (IllegalArgumentException ex) {
            Log.w(TAG, "Unable to open content: " + mUri, ex);
            mCurrentState = STATE_ERROR;
            mTargetState = STATE_ERROR;
            mErrorListener.onError(mMediaPlayer, MediaPlayer.MEDIA_ERROR_UNKNOWN, 0);
            return;
        } finally {
            mPendingSubtitleTracks.clear();
        }
    }

    public void setMediaController(RtkMediaController controller) {
        if (mMediaController != null) {
            mMediaController.hide();
        }
        mMediaController = controller;
        Log.d(TAG, "254 : attachMediaController");
        attachMediaController();
    }

    private void attachMediaController() {
        if (mMediaPlayer != null && mMediaController != null) {
            mMediaController.setMediaPlayer(this);
            View anchorView = this.getParent() instanceof View ?
                    (View)this.getParent() : this;
            mMediaController.setAnchorView(anchorView);
            mMediaController.setEnabled(isInPlaybackState());
        }
        else if (mMediaPlayer == null)
            Log.d(TAG, "mMediaPlayer is null !!");
        else if (mMediaController == null)
            Log.d(TAG, "mMediaController is null !!");
        
    }

    MediaPlayer.OnVideoSizeChangedListener mSizeChangedListener =
        new MediaPlayer.OnVideoSizeChangedListener() {
            public void onVideoSizeChanged(MediaPlayer mp, int width, int height) {
                mVideoWidth = mp.getVideoWidth();
                mVideoHeight = mp.getVideoHeight();
                if (mVideoWidth != 0 && mVideoHeight != 0) {
                    getHolder().setFixedSize(mVideoWidth, mVideoHeight);
                    requestLayout();
                }
            }
    };

    MediaPlayer.OnPreparedListener mPreparedListener = new MediaPlayer.OnPreparedListener() {
        public void onPrepared(MediaPlayer mp) {
            mCurrentState = STATE_PREPARED;

            // Get the capabilities of the player for this stream
            Metadata data = mp.getMetadata(MediaPlayer.METADATA_ALL,
                                      MediaPlayer.BYPASS_METADATA_FILTER);

            
            if (data != null) {
                mCanPause = !data.has(Metadata.PAUSE_AVAILABLE)
                        || data.getBoolean(Metadata.PAUSE_AVAILABLE);
                mCanSeekBack = !data.has(Metadata.SEEK_BACKWARD_AVAILABLE)
                        || data.getBoolean(Metadata.SEEK_BACKWARD_AVAILABLE);
                mCanSeekForward = !data.has(Metadata.SEEK_FORWARD_AVAILABLE)
                        || data.getBoolean(Metadata.SEEK_FORWARD_AVAILABLE);
                mCanSeek = !data.has(Metadata.SEEK_AVAILABLE)
                        || data.getBoolean(Metadata.SEEK_AVAILABLE);


                Log.d (TAG, "Metadata : mCanPause = " + mCanPause);
                Log.d (TAG, "Metadata : mCanSeekBack = " + mCanSeekBack);
                Log.d (TAG, "Metadata : mCanSeekForward = " + mCanSeekForward);
                Log.d (TAG, "Metadata : mCanSeek = " + mCanSeek);
            } else {
                mCanPause = mCanSeekBack = mCanSeekForward = mCanSeek = true;
                Log.d (TAG, "Metadata is null !");
            }

            if (mBroadcastStatus)
            {
                int SeekSupportFlag = 0;
                SeekSupportFlag = (mCanPause)? SeekSupportFlag | CAN_PAUSE_FLAG : SeekSupportFlag;
                SeekSupportFlag = (mCanSeekForward)? SeekSupportFlag | CAN_SEEK_FWD_FLAG : SeekSupportFlag;
                SeekSupportFlag = (mCanSeekBack)? SeekSupportFlag | CAN_SEEK_BWD_FLAG : SeekSupportFlag;
                SeekSupportFlag = (mCanSeek)? SeekSupportFlag | CAN_SEEK : SeekSupportFlag;
                //// Broadcast playback status for DMR usage////////////
                Log.d(TAG, "Broadcast SeekSupported = " + SeekSupportFlag);
                Intent i = new Intent(RTK_PLAYBACK_STATUS)
                        .putExtra("SeekSupported", SeekSupportFlag);
                mContext.sendBroadcast(i);
                ////////////////////////////////////////////////
            }

            if (mOnPreparedListener != null) {
                mOnPreparedListener.onPrepared(mMediaPlayer);
            }
            if (mMediaController != null) {
                mMediaController.setEnabled(true);
            }
            mVideoWidth = mp.getVideoWidth();
            mVideoHeight = mp.getVideoHeight();
            int seekToPosition = mSeekWhenPrepared;  // mSeekWhenPrepared may be changed after seekTo() call
            if (seekToPosition != 0) {
                seekTo(seekToPosition);
            }
            if (mVideoWidth != 0 && mVideoHeight != 0) {
                //Log.i("@@@@", "video size: " + mVideoWidth +"/"+ mVideoHeight);
                getHolder().setFixedSize(mVideoWidth, mVideoHeight);
                if (mSurfaceWidth == mVideoWidth && mSurfaceHeight == mVideoHeight) {
                    // We didn't actually change the size (it was already at the size
                    // we need), so we won't get a "surface changed" callback, so
                    // start the video here instead of in the callback.
                    if (mTargetState == STATE_PLAYING) {
                        start();
                        if (mMediaController != null) {
                            mMediaController.show();
                        }
                    } else if (!isPlaying() &&
                               (seekToPosition != 0 || getCurrentPosition() > 0)) {
                       if (mMediaController != null) {
                           // Show the media controls when we're paused into a video and make 'em stick.
                           mMediaController.show(0);
                       }
                   }
                }
            } else {
                // We don't know the video size yet, but should start anyway.
                // The video size might be reported to us later.
                if (mTargetState == STATE_PLAYING) {
                    start();
                }
            }
        }
    };

    private MediaPlayer.OnCompletionListener mCompletionListener =
        new MediaPlayer.OnCompletionListener() {
        public void onCompletion(MediaPlayer mp) {
            mCurrentState = STATE_PLAYBACK_COMPLETED;
            mTargetState = STATE_PLAYBACK_COMPLETED;
            if (mMediaController != null) {
                mMediaController.hide();
            }
            if (mOnCompletionListener != null) {
                if (mBroadcastStatus && mContinuousPlay == false)
                {
                    //// Broadcast playback status for DMR usage////////////
                    Log.d(TAG, "Broadcast PlayMode: Stop");
                    Intent i = new Intent(RTK_PLAYBACK_STATUS)
                            .putExtra("PlayMode", "Stop");
                    i.putExtra("PlayType", "Video");
                    if (null != mSourceFrom) {
                        i.putExtra("SourceFrom", mSourceFrom);
                    }
                    mContext.sendBroadcast(i);
                    ////////////////////////////////////////////////
                }
                mOnCompletionListener.onCompletion(mMediaPlayer);
            }
        }
    };

    private MediaPlayer.OnSeekCompleteListener mSeekCompleteListener =
        new MediaPlayer.OnSeekCompleteListener() {
        public void onSeekComplete(MediaPlayer mp) {
            if (mOnSeekCompleteListener != null) {
                mOnSeekCompleteListener.onSeekComplete(mMediaPlayer);
            }
        }
    };

    private MediaPlayer.OnSeekResultListener mSeekResultListener =
        new MediaPlayer.OnSeekResultListener() {
        public void onSeekResult(MediaPlayer mp, int result) {
            if (mOnSeekResultListener != null) {
                mOnSeekResultListener.onSeekResult(mMediaPlayer, result);
            }
        }
    };

    private MediaPlayer.OnSpeedChangedListener mSpeedChangedListener = 
        new MediaPlayer.OnSpeedChangedListener() {
        public void onSpeedChanged(MediaPlayer mp, int speed) {
            if (mOnSpeedChangedListener != null) {
                mOnSpeedChangedListener.onSpeedChanged(mp, speed);
            }
        }
    };

    private MediaPlayer.OnInfoListener mInfoListener =
        new MediaPlayer.OnInfoListener() {
        public  boolean onInfo(MediaPlayer mp, int arg1, int arg2) {
            if (mOnInfoListener != null) {
                mOnInfoListener.onInfo(mp, arg1, arg2);
            }
            return true;
        }
    };

    private MediaPlayer.OnErrorListener mErrorListener =
        new MediaPlayer.OnErrorListener() {
        public boolean onError(MediaPlayer mp, int framework_err, int impl_err) {
            Log.d(TAG, "Error: " + framework_err + "," + impl_err);
            mCurrentState = STATE_ERROR;
            mTargetState = STATE_ERROR;
            if (mMediaController != null) {
                mMediaController.hide();
            }

            /* If an error handler has been supplied, use it and finish. */
            if (mOnErrorListener != null) {
                if (mOnErrorListener.onError(mMediaPlayer, framework_err, impl_err)) {
                    return true;
                }
            }

            /* Otherwise, pop up an error dialog so the user knows that
             * something bad has happened. Only try and pop up the dialog
             * if we're attached to a window. When we're going away and no
             * longer have a window, don't bother showing the user an error.
             */
            if (getWindowToken() != null) {
                Resources r = mContext.getResources();
                int messageId;

                if (framework_err == MediaPlayer.MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK) {
                    messageId = com.android.internal.R.string.VideoView_error_text_invalid_progressive_playback;
                } else {
                    messageId = com.android.internal.R.string.VideoView_error_text_unknown;
                }

                new AlertDialog.Builder(mContext)
                        .setMessage(messageId)
                        .setPositiveButton(com.android.internal.R.string.VideoView_error_button,
                                new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int whichButton) {
                                        /* If we get here, there is no onError listener, so
                                         * at least inform them that the video is over.
                                         */
                                        if (mOnCompletionListener != null) {
                                            if (mBroadcastStatus)
                                            {
                                                //// Broadcast playback status for DMR usage////////////
                                                Log.d(TAG, "Broadcast PlayMode: Stop");
                                                Intent i = new Intent(RTK_PLAYBACK_STATUS)
                                                    .putExtra("PlayMode", "Stop");
                                                i.putExtra("PlayType", "Video");
                                                if (null != mSourceFrom) {
                                                    i.putExtra("SourceFrom", mSourceFrom);
                                                }
                                                mContext.sendBroadcast(i);
                                                ////////////////////////////////////////////////
                                            }
                                            mOnCompletionListener.onCompletion(mMediaPlayer);
                                        }
                                    }
                                })
                        .setCancelable(false)
                        .show();
            }
            return true;
        }
    };

    private MediaPlayer.OnBufferingUpdateListener mBufferingUpdateListener =
        new MediaPlayer.OnBufferingUpdateListener() {
        public void onBufferingUpdate(MediaPlayer mp, int percent) {
            mCurrentBufferPercentage = percent;
        }
    };

    /**
     * Register a callback to be invoked when the media file
     * is loaded and ready to go.
     *
     * @param l The callback that will be run
     */
    public void setOnPreparedListener(MediaPlayer.OnPreparedListener l)
    {
        mOnPreparedListener = l;
    }

    /**
     * Register a callback to be invoked when the end of a media file
     * has been reached during playback.
     *
     * @param l The callback that will be run
     */
    public void setOnCompletionListener(OnCompletionListener l)
    {
        mOnCompletionListener = l;
    }

    /**
     * Register a callback to be invoked when seeking completely
     * has been reached during playback.
     *
     * @param l The callback that will be run
     */
    public void setOnSeekCompleteListener(OnSeekCompleteListener l)
    {
        mOnSeekCompleteListener = l;
    }

    /**
     * Register a callback to be invoked when seeking completely
     * has been reached during playback.
     *
     * @param l The callback that will be run
     */
    public void setOnSeekResultListener(OnSeekResultListener l)
    {
        mOnSeekResultListener = l;
    }

    /**
     * Register a callback to be invoked when speed
     * has been changed during playback.
     *
     * @param l The callback that will be run
     */
    public void setOnSpeedChangedListener(OnSpeedChangedListener l)
    {
        mOnSpeedChangedListener = l;
    }
    
    /**
     * Register a callback to be invoked when an error occurs
     * during playback or setup.  If no listener is specified,
     * or if the listener returned false, RtkVideoView will inform
     * the user of any errors.
     *
     * @param l The callback that will be run
     */
    public void setOnErrorListener(OnErrorListener l)
    {
        mOnErrorListener = l;
    }

    /**
     * Register a callback to be invoked when an informational event
     * occurs during playback or setup.
     *
     * @param l The callback that will be run
     */
    public void setOnInfoListener(OnInfoListener l) {
        mOnInfoListener = l;
    }

    /**
     * Sets the player to be looping or non-looping.
     *
     * @param looping whether to loop or not
     */
    public void setLooping(boolean looping) {
        if (mMediaPlayer != null)
            mMediaPlayer.setLooping(looping);
    }

    SurfaceHolder.Callback mSHCallback = new SurfaceHolder.Callback()
    {
        public void surfaceChanged(SurfaceHolder holder, int format,
                                    int w, int h)
        {
            mSurfaceWidth = w;
            mSurfaceHeight = h;
            boolean isValidState =  (mTargetState == STATE_PLAYING);
            boolean hasValidSize = (mVideoWidth == w && mVideoHeight == h);
            if (mMediaPlayer != null && isValidState && hasValidSize) {
                if (mSeekWhenPrepared != 0) {
                    seekTo(mSeekWhenPrepared);
                }
                start();
            }
        }

        public void surfaceCreated(SurfaceHolder holder)
        {
            mSurfaceHolder = holder;
            openVideo();
        }

        public void surfaceDestroyed(SurfaceHolder holder)
        {
            // after we return from this we can't use the surface any more
            mSurfaceHolder = null;
            if (mMediaController != null) mMediaController.hide();
            release(true);
        }
    };

    /*
     * release the media player in any state
     */
    private void release(boolean cleartargetstate) {
        if (mMediaPlayer != null) {
            mMediaPlayer.reset();
            mMediaPlayer.release();
            mMediaPlayer = null;
            mPendingSubtitleTracks.clear();
            mCurrentState = STATE_IDLE;
            if (cleartargetstate) {
                mTargetState  = STATE_IDLE;
            }
        }

        if (cleartargetstate && mBroadcastStatus) {
            //// Broadcast playback status for DMR usage////////////
            Log.d(TAG, "Broadcast PlayMode: Stop");
            Intent i = new Intent(RTK_PLAYBACK_STATUS)
                .putExtra("PlayMode", "Stop");
            i.putExtra("PlayType", "Video");
            if (null != mSourceFrom) {
                i.putExtra("SourceFrom", mSourceFrom);
            }
            mContext.sendBroadcast(i);
            ////////////////////////////////////////////////
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent ev) {
        if (isInPlaybackState() && mMediaController != null) {
            toggleMediaControlsVisiblity();
        }
        return false;
    }

    @Override
    public boolean onTrackballEvent(MotionEvent ev) {
        if (isInPlaybackState() && mMediaController != null) {
            toggleMediaControlsVisiblity();
        }
        return false;
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        boolean isKeyCodeSupported = keyCode != KeyEvent.KEYCODE_BACK &&
                                     keyCode != KeyEvent.KEYCODE_VOLUME_UP &&
                                     keyCode != KeyEvent.KEYCODE_VOLUME_DOWN &&
                                     keyCode != KeyEvent.KEYCODE_VOLUME_MUTE &&
                                     keyCode != KeyEvent.KEYCODE_MENU &&
                                     keyCode != KeyEvent.KEYCODE_CALL &&
                                     keyCode != KeyEvent.KEYCODE_ENDCALL;
        if (isInPlaybackState() && isKeyCodeSupported && mMediaController != null) {
            if (keyCode == KeyEvent.KEYCODE_HEADSETHOOK ||
                    keyCode == KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE) {
                if (mMediaPlayer.isPlaying()) {
                    pause();
                    mMediaController.show();
                } else {
                    start();
                    mMediaController.hide();
                }
                return true;
            } else if (keyCode == KeyEvent.KEYCODE_MEDIA_PLAY) {
                if (!mMediaPlayer.isPlaying()) {
                    start();
                    mMediaController.hide();
                }
                return true;
            } else if (keyCode == KeyEvent.KEYCODE_MEDIA_STOP
                    || keyCode == KeyEvent.KEYCODE_MEDIA_PAUSE) {
                if (mMediaPlayer.isPlaying()) {
                    pause();
                    mMediaController.show();
                }
                return true;
            } else {
                //Log.d(TAG, "toggleMediaControlsVisiblity !! ");
                //toggleMediaControlsVisiblity();
            }
        }

        return super.onKeyDown(keyCode, event);
    }

    private void toggleMediaControlsVisiblity() {
        if (mMediaController.isShowing()) {
            mMediaController.hide();
        } else {
            mMediaController.show();
        }
    }

    @Override
    public void start() {
        if (isInPlaybackState()) {
            mMediaPlayer.start();
            mCurrentState = STATE_PLAYING;
        }
        if (mBroadcastStatus && mCurrentState == STATE_PLAYING)
        {
            //// Broadcast playback status for DMR usage /////////
            Log.d(TAG, "Broadcast PlayMode: Play");
            Intent i = new Intent(RTK_PLAYBACK_STATUS)
                            .putExtra("PlayMode", "Play");
            i.putExtra("PlayType", "Video");
            if (null != mSourceFrom) {
                i.putExtra("SourceFrom", mSourceFrom);
            }
            mContext.sendBroadcast(i);
            ////////////////////////////////////////////////
        }
        mTargetState = STATE_PLAYING;
    }

    @Override
    public void pause() {
        if (isInPlaybackState()) {
            if (mMediaPlayer.isPlaying()) {
                mMediaPlayer.pause();
                mCurrentState = STATE_PAUSED;
            }
        }
        if (mBroadcastStatus)
        {
            //// Broadcast playback status for DMR usage////////////
            Log.d(TAG, "Broadcast PlayMode: Pause");
            Intent i = new Intent(RTK_PLAYBACK_STATUS)
                            .putExtra("PlayMode", "Pause");
            i.putExtra("PlayType", "Video");
            if (null != mSourceFrom) {
                i.putExtra("SourceFrom", mSourceFrom);
            }
            mContext.sendBroadcast(i);
            ////////////////////////////////////////////////
        }
        mTargetState = STATE_PAUSED;
    }

    public void suspend() {
        release(false);
    }

    public void resume() {
        openVideo();
    }

    @Override
    public int getDuration() {
        if (isInPlaybackState()) {
            int duration = mMediaPlayer.getDuration();
            if (mBroadcastStatus)
            {
                //// Broadcast playback status for DMR usage////////////
                Log.d(TAG, "Broadcast Duration = " + duration / 1000);
                Intent i = new Intent(RTK_PLAYBACK_STATUS)
                                .putExtra("Duration", duration / 1000);
                mContext.sendBroadcast(i);
                ////////////////////////////////////////////////
            }
            return duration;
        }

        return -1;
    }

    @Override
    public int getCurrentPosition() {
        if (isInPlaybackState()) {
            int position = mMediaPlayer.getCurrentPosition();
            if (mBroadcastStatus)
            {
                //// Broadcast playback status for DMR usage////////////
                //Log.v(TAG, "Broadcasr TimePosition = " + position / 1000);
                Intent i = new Intent(RTK_PLAYBACK_STATUS)
                                .putExtra("TimePosition", position / 1000);
                mContext.sendBroadcast(i);
                ////////////////////////////////////////////////
            }
            return position;
        }
        return 0;
    }

    @Override
    public void seekTo(int msec) {
        if (isInPlaybackState()) {
            mMediaPlayer.seekTo(msec);
            mSeekWhenPrepared = 0;
        } else {
            mSeekWhenPrepared = msec;
        }
    }

    public boolean fastforward(int speed)
    {
        boolean ret = false;
        if (isInPlaybackState()) {
            ret = mMediaPlayer.fastforward(speed);
        } else {
            Log.i(TAG, "fastforward fail , not playback state , speed "+speed);
        }
        return ret;
    }
    public boolean fastrewind(int speed)
    {
        boolean ret = false;
        if (isInPlaybackState()) {
            ret = mMediaPlayer.fastrewind(speed);
        } else {
            Log.i(TAG, "fastrewind fail , not playback state , speed "+speed);
        }
        return ret; 
    }
    public retSubtitle getSubtitleInfo()
    {
        return getSubtitleInfo(-1);
    }    
    public retSubtitle getSubtitleInfo(int streamNum)
    {
        retSubtitle ret=new retSubtitle();
        ret.iTotalStreamsNum=0;
        if (isInPlaybackState()) 
        {
            int[] tmp=mMediaPlayer.getSubtitleInfo(streamNum);
            if(tmp == null)
            {
                ret.retInfo="getSubtitleInfo fail , can't get any subtitle info.";	     
            }
	     else
	     {
	         ret.iTotalStreamsNum=tmp[1];
                ret.iCurrentStream=tmp[2];
                ret.iLangCode=tmp[3];
                ret.retInfo="getSubtitleInfo ok , "+tmp[0]+","+tmp[1]+","+tmp[2]+","+tmp[3]+"....";
                if(tmp.length >= 4)
                    ret.retInfo = ret.retInfo+" length = "+tmp.length+ ", "+tmp[3]+"....";
	     }
        } else {
            ret.retInfo="getSubtitleInfo fail , it's not in playback state"; 
        }
        Log.i(TAG, ret.retInfo);
        return  ret;
    }

    /*
    * getSubtitleTrack: get all the subtitle tracks
    */
    public TrackInfo[] getSubtitleTrackInfo(boolean bForceUpdate)
    {
        String ret;
        if (mMediaPlayer!=null) {
            return mMediaPlayer.getSubtitleTrackInfo(bForceUpdate);
        }
        else {
            TrackInfo trackInfo[] = new TrackInfo[0];
            return trackInfo;
        }
    }

    /*
    * getSubtitleCurrentIndex: get the current subtitle track index (1-based)
    *
    * @parameter: None
    * @return: integer
    */

    public int getSubtitleCurrentIndex() {
        String ret;
        if (mMediaPlayer!=null) {
            return mMediaPlayer.getSubtitleCurrentIndex();
        }
        else {
            return 0;
        }
    }

    public TrackInfo[] getTrackInfo() throws IllegalStateException {
        String ret;
        TrackInfo[] trackInfo = null;
        if (isInPlaybackState())
        {
            ret="getTrackInfo ok";
            trackInfo =  mMediaPlayer.getTrackInfo();
        } else {
            ret="getTrackInfo fail , it's not in playback state";
        }
        Log.i(TAG, ret);
        return trackInfo;
    }
    public void setSubtitleInfo(int streamNum,int enable,int textEncoding,int testColor,int fontSize,int syncTime,int offset)
    {
        String ret;
        if (isInPlaybackState()) {
            mMediaPlayer.setSubtitleInfo(streamNum,enable,textEncoding,testColor,fontSize, syncTime, offset);
            ret="setSubtitleInfo ok , streamNum="+streamNum+",enable="+enable+",textEncoding="+textEncoding+",testColor="+testColor+",fontSize="+fontSize+"....";
        } else {
            ret="setSubtitleInfo fail ,  it's not in playback state.";
        }
        Log.i(TAG, ret);
        return;
    }
    public void setAutoScanExtSubtitle(int autoScan)
    {
        String ret;
        if(mCurrentState == STATE_IDLE)
        {
            mAutoScanExtSubtitle=autoScan;
            ret="setAutoScanExtSubtitle ok , autoScan="+autoScan +"....";
        } else {
            ret="setAutoScanExtSubtitle fail ,  it's not in idle state.";
        }
        Log.i(TAG, ret);
        return;
    }
    public void setExtSubtitlePath(String path)
    {
        String ret;
        if (isInPlaybackState()) {
            mMediaPlayer.setExtSubtitlePath(path);
            ret="setExtSubtitlePath ok , path="+path+"....";
        } else {
            ret="setExtSubtitlePath fail ,  it's not in playback state.";
        }
        Log.i(TAG, ret);
        return;
    }
    public retAudioTrack getAudioTrackInfo(int streamNum)
    {
        retAudioTrack ret=new retAudioTrack();
        if (isInPlaybackState()) {
            int[] tmp=mMediaPlayer.getAudioTrackInfo(streamNum);
            if(tmp == null)
            {
                ret.retInfo="getAudioTrackInfo fail , can't get any stream = "+streamNum+" info.";
            }
	     else
	     {
                ret.iTotalStreamsNum=tmp[1];
                ret.iCurrentStream=tmp[2];
                ret.iAudioType=tmp[3];
                ret.iChannelNum=tmp[4];
                ret.iBitsPerSample=tmp[5];
                ret.iSampleRate=tmp[6];
                ret.iPid=tmp[7];
                ret.retInfo="getAudioTrackInfo ok , "+tmp[0]+","+tmp[1]+","+tmp[2]+","+tmp[3]+","+tmp[4]+","+tmp[5]+","+tmp[6]+","+tmp[7]+"....";
	     }
			
        } else {
            ret.retInfo="getAudioTrackInfo fail ,it's not in playback state"; 
        }
        Log.i(TAG, ret.retInfo);
        return  ret;
    }

    /*
    * getAudioTrack: get all the subtitle tracks
    */
    public TrackInfo[] getAudioTrackInfo(boolean bForceUpdate)
    {
        String ret;
        if (mMediaPlayer!=null) {
            return mMediaPlayer.getAudioTrackInfo(bForceUpdate);
        }
        else {
            TrackInfo trackInfo[] = new TrackInfo[0];
            return trackInfo;
        }
    }

    /*
    * getAudioCurrentIndex: get the current audio track index (1-based)
    *
    * @parameter: None
    * @return: integer
    */
    public int getAudioCurrentIndex() {
        String ret;
        if (mMediaPlayer!=null) {
            return mMediaPlayer.getAudioCurrentIndex();
        }
        else {
            return 0;
        }
    }

    public void setAudioTrackInfo(int streamNum)
    {
        String ret;
        if (isInPlaybackState()) {
            mMediaPlayer.setAudioTrackInfo(streamNum);
            ret="setAudioTrackInfo ok , streamNum="+streamNum+"....";
        } else {
            ret="setSubtitleInfo fail ,  it's not in playback state.";
        }
        Log.i(TAG, ret);
        return;
    }

    public void selectTrack(int streamNum) {
        String ret;
        if (isInPlaybackState()) {
            mMediaPlayer.selectTrack(streamNum);
            ret="selectTrack ok , streamNum="+streamNum+"....";
        } else {
            ret="selectTrack fail ,  it's not in playback state.";
        }
        Log.i(TAG, ret);
        return;
    }

    public  byte[] execSetGetNavProperty(int propertyID,byte[] inputArray)
    {
        byte[] out=null;
        String ret;
        if (isInPlaybackState()) {
            out=mMediaPlayer.execSetGetNavProperty(propertyID,inputArray);
            ret="execSetGetNavProperty ok";
        } else {
            ret="execSetGetNavProperty fail ,  it's not in playback state.";
        }
        Log.i(TAG, ret);
        return out;
    }
    public boolean deliverNaviControlCmd(int action, int target)
    {
        boolean ret = false;
        if (isInPlaybackState()) {
            ret = mMediaPlayer.deliverNaviControlCmd(action, target);
            if (ret == true) {
                Log.i(TAG, "deliverNaviControlCmd ok , action: "+ action + ", target: " + target);
            }
            else
                Log.i(TAG, "deliverNaviControlCmd fail , action: "+ action + ", target: " + target);
        } else {
            Log.i(TAG, "deliverNaviControlCmd fail , not playback state , action: " + action + ", target: " + target);
        }
        return ret;
    }

    public NaviInfo getNavigationInfo() {
        if (isInPlaybackState()) {
            try {
                return mMediaPlayer.getNavigationInfo();
            } catch (IllegalStateException ise) {
                Log.w(TAG, "Unable to get navigation information, exception:" + ise);
            }
        } else {
            Log.w(TAG, "Not in playback state, can't get navigation information!");
        }
        return null;
    }

    public BDInfo getBDInfo() {
        if (isInPlaybackState()) {
            try {
                return mMediaPlayer.getBDInfo();
            } catch (IllegalStateException ise) {
                Log.w(TAG, "Unable to get BD information, exception:" + ise);
            }
        } else {
            Log.w(TAG, "Not in playback state, can't get BD information!");
        }
        return null;
    }

    public BDInfo getBDChapterInfo() {
        if (isInPlaybackState()) {
            try {
                return mMediaPlayer.getBDChapterInfo();
            } catch (IllegalStateException ise) {
                Log.w(TAG, "Unable to get BD chapter information, exception:" + ise);
            }
        } else {
            Log.w(TAG, "Not in playback state, can't get BD chapter information!");
        }
        return null;
    }

    /**
     * setMVC - It is used to enable or disable MVC (Multiview Video Coding) (3D video).
     *
     * @param bEnabled - true means enable MVC; otherwise disable MVC mode.
     * @return void
     */
    public void setMVC(boolean bEnabled) {
        if (isInPlaybackState()) {
            mMediaPlayer.setMVC(bEnabled);
        } else {
            Log.w(TAG, "Not in playback state, can't setMVC!");
        }
    }

    /**
     * setVideoBufferSize - It is used to set video buffer size
     *
     * @param bufferSize - video buffer size (MBytes)
     * @return void
     */
    public void setVideoBufferSize(int bufferSize) {
        mVideoBufferSize = bufferSize;
    }

    @Override
    public boolean isPlaying() {
        return isInPlaybackState() && mMediaPlayer.isPlaying();
    }

    @Override
    public int getBufferPercentage() {
        if (mMediaPlayer != null) {
            return mCurrentBufferPercentage;
        }
        return 0;
    }

    private boolean isInPlaybackState() {
        return (mMediaPlayer != null &&
                mCurrentState != STATE_ERROR &&
                mCurrentState != STATE_IDLE &&
                mCurrentState != STATE_PREPARING);
    }

    @Override
    public boolean canPause() {
        return mCanPause;
    }

    @Override
    public boolean canSeekBackward() {
        return mCanSeekBack;
    }

    @Override
    public boolean canSeekForward() {
        return mCanSeekForward;
    }

    @Override
    public int getAudioSessionId() {
        if (mAudioSession == 0) {
            MediaPlayer foo = new MediaPlayer();
            mAudioSession = foo.getAudioSessionId();
            foo.release();
        }
        return mAudioSession;
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();

        if (mSubtitleWidget != null) {
            mSubtitleWidget.onAttachedToWindow();
        }
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();

        if (mSubtitleWidget != null) {
            mSubtitleWidget.onDetachedFromWindow();
        }
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);

        if (mSubtitleWidget != null) {
            measureAndLayoutSubtitleWidget();
        }
    }

    @Override
    public void draw(Canvas canvas) {
        super.draw(canvas);

        if (mSubtitleWidget != null) {
            final int saveCount = canvas.save();
            canvas.translate(getPaddingLeft(), getPaddingTop());
            mSubtitleWidget.draw(canvas);
            canvas.restoreToCount(saveCount);
        }
    }

    /**
     * Forces a measurement and layout pass for all overlaid views.
     *
     * @see #setSubtitleWidget(RenderingWidget)
     */
    private void measureAndLayoutSubtitleWidget() {
        final int width = getWidth() - getPaddingLeft() - getPaddingRight();
        final int height = getHeight() - getPaddingTop() - getPaddingBottom();

        mSubtitleWidget.setSize(width, height);
    }

    /** @hide */
    @Override
    public void setSubtitleWidget(RenderingWidget subtitleWidget) {
        if (mSubtitleWidget == subtitleWidget) {
            return;
        }

        final boolean attachedToWindow = isAttachedToWindow();
        if (mSubtitleWidget != null) {
            if (attachedToWindow) {
                mSubtitleWidget.onDetachedFromWindow();
            }

            mSubtitleWidget.setOnChangedListener(null);
        }

        mSubtitleWidget = subtitleWidget;

        if (subtitleWidget != null) {
            if (mSubtitlesChangedListener == null) {
                mSubtitlesChangedListener = new RenderingWidget.OnChangedListener() {
                    @Override
                    public void onChanged(RenderingWidget renderingWidget) {
                        invalidate();
                    }
                };
            }

            setWillNotDraw(false);
            subtitleWidget.setOnChangedListener(mSubtitlesChangedListener);

            if (attachedToWindow) {
                subtitleWidget.onAttachedToWindow();
                requestLayout();
            }
        } else {
            setWillNotDraw(true);
        }

        invalidate();
    }

    /** @hide */
    @Override
    public Looper getSubtitleLooper() {
        return Looper.getMainLooper();
    }

    public void setContinuousPlay(boolean bContinuous) {
        mContinuousPlay = bContinuous;
    }
    
    public void setSourceFrom(String sourceFrom) {
        mSourceFrom = sourceFrom;
    }

    public NaviType getNavigationType() {
        if (isInPlaybackState()) {
            try {
                return mMediaPlayer.getNavigationType();
            } catch (IllegalStateException ise) {
                Log.w(TAG, "Unable to get media type, exception:" + ise);
            }
        } else {
            Log.w(TAG, "Not in playback state, can't get media type!");
        }
        return null;
    }

    public void setStreamingBufferThreshold(int highThreshold, int lowThreshold) {
        if (isInPlaybackState()) {
            mMediaPlayer.setStreamingBufferThreshold(highThreshold, lowThreshold);
        }
    }

    public boolean setBookmark(Uri Uri) {
        boolean ret = false;
        if (null != mMediaPlayer && isInPlaybackState()) {
            ret = mMediaPlayer.setBookmark(Uri);
        }
        return ret;
    }

    public boolean applyBookmark(Uri Uri) {
        boolean ret = false;
        if (null != mMediaPlayer && isInPlaybackState()) {
            ret = mMediaPlayer.applyBookmark(Uri);
        }
        return ret;
    }

    public boolean removeBookmark(Uri Uri) {
        boolean ret = false;
        if (null != mMediaPlayer && isInPlaybackState()) {
            ret = mMediaPlayer.removeBookmark(Uri);
        }
        return ret;
    }

    public boolean checkBookmark(Uri Uri) {
        boolean ret = false;
        if (null != mMediaPlayer && isInPlaybackState()) {
            ret = mMediaPlayer.checkBookmark(Uri);
        }
        return ret;
    }

    public boolean setParameter(int key, Parcel value) {
        boolean ret = false;
        if (null != mMediaPlayer) {
            ret = mMediaPlayer.setParameter(key, value);
        }
        return ret;
    }
}
