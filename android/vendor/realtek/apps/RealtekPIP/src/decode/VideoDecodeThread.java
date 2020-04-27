package decode;

import java.io.IOException;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;

import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.media.MediaExtractor;
import android.media.MediaFormat;
import android.media.MediaCodec.BufferInfo;
import android.util.Log;
import android.view.Surface;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Bundle;

public class VideoDecodeThread extends Thread {
    public static final String TAG = "VideoDecodeThread";
    public static final String TAG2 = "[RTKRRS]";
    private MediaExtractor extractor;
    private MediaCodec decoder;
    private Surface surface;
    private String videoPath;
    private boolean eosThread = false;
    private boolean formatInited = false;
    private int freeRun = 0;
    private int performance = 0;
    private int render = 0;
    private int outOfTimeThreshold = 30;//ms
    public int flag = 0;
    public long startMs = 0;//ms
    public long curTimeStamp = 0;
    public long curExtractorTimeStamp = 0;
    private int terminateTimeout = 3;
    public int audioSessionID;
    private boolean isEOS = false;
    private BufferInfo decoderInfo;
    private HandlerThread decoderCallbackThread;
    private Handler decoderHandler;
    private boolean isCodecErr = false;
    private boolean mStartRealease = false;
    private double mDecodeframeCount = 0;
    public double mDecodeFPS = 0;
    
    public VideoDecodeThread(Surface surface, String path, int freeRun, int audioSessionId
            , int performance, int render) {
        this.surface = surface;
        this.videoPath = path;
        this.freeRun = freeRun;
        this.audioSessionID = audioSessionId;
        this.performance = performance;
        this.render = render;
        Log.d(TAG, "audioSessionID " + this.audioSessionID);
    }

    @Override
    public void run() {
        extractor = new MediaExtractor();
        Log.d(TAG, "Start setDataSource ... " + videoPath);
        try {
            extractor.setDataSource(videoPath);
        } catch (IOException e1) {
            // TODO Auto-generated catch block
            e1.printStackTrace();
            releaseResource();
            Log.d(TAG2, "ErrCode 1 setDataSource IO error " + videoPath);
            return;
        }
        Log.d(TAG, "End setDataSource ... " + videoPath);
        
        for (int i = 0; i < extractor.getTrackCount(); i++) {
            MediaFormat format = extractor.getTrackFormat(i);
            String mime = format.getString(MediaFormat.KEY_MIME);
            Log.d(TAG, "mime type " + mime);
            if (mime.startsWith("video/")) {
                try {
                    extractor.selectTrack(i);
                } catch(Exception e)
                {
                    Log.e(TAG2, "ErrCode 2 selectTrack " + i + " occur " + e.toString() + " !!");
                    releaseResource();
                    return;
                }
                try {
                      if(audioSessionID != -1)
                      {
                         format.setFeatureEnabled(MediaCodecInfo.CodecCapabilities.FEATURE_TunneledPlayback, true);
                         format.setInteger(MediaFormat.KEY_AUDIO_SESSION_ID, audioSessionID);
                      }
                      format.setInteger(MediaFormat.KEY_PUSH_BLANK_BUFFERS_ON_STOP,1);
                      decoder = MediaCodec.createDecoderByType(mime);
                      Log.d(TAG, "create decoder with mime " + mime);
                } catch (java.io.IOException e) {
                    Log.e(TAG2, "ErrCode 3 Create decoder codec from type ERROR !!!");
                    releaseResource();
                    return;
                }

                decoderCallbackThread = new HandlerThread("DecoderHanlderThread");
                decoderCallbackThread.start();
                decoderHandler = new Handler(decoderCallbackThread.getLooper());
                setupDecoderCallback(decoderHandler);
                decoder.configure(format, surface, null, 0);
                break;
            }
        }

        if (decoder == null) {
            Log.e(TAG2, "ErrCode 4 Create decoder FAIL !! " + videoPath);
            releaseResource();
            return;
        }
        decoderInfo = new BufferInfo();
        decoder.start();
        flag = 1;
        while (true) {
            if ((decoderInfo.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                Log.d(TAG, "OutputBuffer BUFFER_FLAG_END_OF_STREAM " + videoPath);
                break;
            }
            else if(eosThread == true)
            {
                if(audioSessionID != -1)
                    break;
                Log.d(TAG, "Waiting for OutputBuffer BUFFER_FLAG_END_OF_STREAM " + videoPath);
                terminateTimeout--;
                if(terminateTimeout < 0)
                {
                    Log.d(TAG, "Terminate thread occur TIMEOUT !! ");
                    break;
                }
            }

            try {
                    Thread.sleep(1000);
                } catch(InterruptedException  ex) {
                }
        }
        releaseResource();
    }

    public void releaseResource()
    {
        if(eosThread == false)
            eosThread = true;
        if(mStartRealease == false) {
            Log.d(TAG, "Video Thread start releasing !! " + videoPath);
            mStartRealease = true;
            if(decoder != null)
            {
                if(isCodecErr == false) {
                    decoder.flush();
                    decoder.stop();
                }
                decoder.release();
            }

            if(extractor != null)
                extractor.release();

            if(decoderCallbackThread != null)
            {
                decoderCallbackThread.quitSafely();
                try {
                    decoderCallbackThread.join();
                    } catch(InterruptedException  ex) {
                    }
            }
            flag = -1;
        }
        Log.d(TAG2, "ErrCode 0 FPS " + mDecodeFPS + " path " + videoPath);
    }

    public void skipRefFrameControl(int ctrl)
    {
        Log.d(TAG, "Skip reference frame control " + ctrl);
        try {
            Bundle skipRefFrame = new Bundle();
            skipRefFrame.putInt("skip-non-ref-frame", ctrl);
            decoder.setParameters(skipRefFrame);
        } catch (IllegalStateException e) {
            e.printStackTrace();
        }
    }

    private void setupDecoderCallback(Handler handle)
    {
        decoder.setCallback(new MediaCodec.Callback() {
        @Override
        public void onInputBufferAvailable(MediaCodec mc, int inputBufferId) 
        {
            if(mStartRealease == true)
                return;
            if (!isEOS) {
                ByteBuffer buffer = mc.getInputBuffer(inputBufferId);
                int sampleSize = extractor.readSampleData(buffer, 0);
                long timeStamp = extractor.getSampleTime();
                if(performance == 1 && audioSessionID != -1)
                    timeStamp = 0;
                flag = 2;
                if (sampleSize < 0) {
                    Log.d(TAG, "InputBuffer BUFFER_FLAG_END_OF_STREAM " + videoPath);
                    mc.queueInputBuffer(inputBufferId, 0, 0, 0, MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                    isEOS = true;
                } else {
                    if(eosThread && formatInited)
                    {
                        Log.d(TAG, "InputBuffer force BUFFER_FLAG_END_OF_STREAM " + videoPath);
                        mc.queueInputBuffer(inputBufferId, 0, sampleSize, timeStamp, MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                        isEOS = true;
                    }
                    else
                    {
                        if(audioSessionID != -1)
                        {
                            if(startMs == 0)
                                startMs = System.currentTimeMillis();
                            mDecodeframeCount++;
                            mDecodeFPS = mDecodeframeCount / (double)((System.currentTimeMillis() - startMs)/1000);
                        }
                        mc.queueInputBuffer(inputBufferId, 0, sampleSize, timeStamp, 0);
                        flag = 3;
                    }
                    curExtractorTimeStamp = extractor.getSampleTime();
                    if(audioSessionID != -1)
                        curTimeStamp = curExtractorTimeStamp;
                    extractor.advance();
                }
            }
        }

        @Override
        public void onOutputBufferAvailable(MediaCodec mc, int outputBufferId, MediaCodec.BufferInfo info) 
        {
            decoderInfo = info;
            if(mStartRealease == true)
                return;
            if(startMs == 0)
                startMs = System.currentTimeMillis();
            ByteBuffer buffer = mc.getOutputBuffer(outputBufferId);
            curTimeStamp = info.presentationTimeUs;
            if(freeRun == 0 && ((info.presentationTimeUs / 1000) + outOfTimeThreshold < System.currentTimeMillis() - startMs))
            {
                Log.v(TAG, "Video packet too late drop it ... " + videoPath);
                mc.releaseOutputBuffer(outputBufferId, false);
                flag = 4;
                return;
            }

            if(performance == 0)
            {
                while ((info.presentationTimeUs / 1000 > System.currentTimeMillis() - startMs) && !eosThread)
                {
                    try {
                        sleep(1);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                        return;
                    }
                }
            }
            if(mStartRealease == false)
            {
                if(render == 1)
                    mc.releaseOutputBuffer(outputBufferId, true);
                else
                    mc.releaseOutputBuffer(outputBufferId, false);
            }
            flag = 4;
            mDecodeframeCount++;
            mDecodeFPS = mDecodeframeCount / (double)((System.currentTimeMillis() - startMs)/1000);
        }

        @Override
        public void onOutputFormatChanged(MediaCodec mc, MediaFormat format) 
        {
            Log.d(TAG, "New format " + decoder.getOutputFormat() + videoPath);
            formatInited = true;
        }

        @Override
        public void onError(MediaCodec codec, MediaCodec.CodecException e)
        {
            if(audioSessionID != -1 && e.getErrorCode() == -11 /* WOULD_BLOCK */)
                eosThread = true;
            else
                isCodecErr = true;
            e.printStackTrace();
            if(isCodecErr)
                Log.e(TAG2, "ErrCode 5 Callback onError " + videoPath);
        }
        }, handle);
    }

    public void stopThread() {
        Log.e(TAG2, "ErrCode 6 received VideoDecodeThread terminate command ");
        eosThread = true;
    }
}
