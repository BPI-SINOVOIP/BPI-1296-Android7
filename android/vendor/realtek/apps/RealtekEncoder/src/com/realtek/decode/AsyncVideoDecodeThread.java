package com.realtek.decode;

import java.io.IOException;
import java.io.FileOutputStream;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.util.HashMap;

import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.media.MediaExtractor;
import android.media.MediaFormat;
import android.media.MediaMuxer;
import android.media.MediaCodec.BufferInfo;
import android.util.Log;
import android.view.Surface;
import android.os.Handler;
import android.os.HandlerThread;

public class AsyncVideoDecodeThread extends Thread {
    public static final String TAG = "AsyncVideoDecodeThread";
    private MediaExtractor extractor = null;
    private MediaCodec decoder = null;
    private MediaCodec encoder = null;
    private MediaMuxer muxer;
    private Surface mSurface;
    private String videoPath;
    private boolean eosThread = false;
    private boolean formatInited = false;
    private int outOfTimeThreshold = 10;//ms
    public int flag = 0;
    public long startMs = 0;//ms
    public long curTimeStamp = 0;
    private int terminateTimeout = 3;
    private int videoTrackIdx;
    public boolean muxFlg = false;
    public boolean trackReady = false;
    private String frameRate;
    private double mDecodeframeCount = 0;
    private double mEncodeframeCount = 0;
    public double mInstantDecodeframeCount = 0;
    public double mInstantEncodeframeCount = 0;
    public double mDecodeFPS = 0;
    public double mEncodeFPS = 0;
    private int bitRate = 0;
    private int frameInterval = 0;
    private int deinterlace = 1;
    private int bitrateMode = 0;
    private int profile = 1;
    private int level = 1;
    private boolean isEOS = false;
    private BufferInfo decoderInfo;
    private HandlerThread decoderCallbackThread = null;
    private HandlerThread encoderCallbackThread = null;
    private Handler decoderHandler;
    private Handler encoderHandler;
    private boolean mStartRealease = false;
    private boolean mDecodeErr = false;
    private boolean mEncodeErr = false;

    private boolean streaming = false;
    private int mRotationAngle = 0;
    private int mWidth;
    private int mHeight;

    public AsyncVideoDecodeThread(String path, MediaMuxer muxer, String frameRate, int bitRate, int frameInterval, 
        int width, int height, int enableDeinterlace, int bitrateMode, int profile, int level, boolean streaming) {
        this.videoPath = path;
        this.muxer = muxer;
        this.frameRate = frameRate;
        this.bitRate = bitRate;
        this.frameInterval = frameInterval;
        this.mWidth = width;
        this.mHeight = height;
        this.deinterlace = enableDeinterlace;
        this.bitrateMode = bitrateMode;
        this.profile = profile;
        this.level = level;
        this.streaming = streaming;
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
                    Log.e(TAG, "selectTrack " + i + " occur " + e.toString() + " !!");
                    releaseResource();
                    return;
                }

                try {
                      decoder = MediaCodec.createDecoderByType(mime);
                      Log.d(TAG, "create decoder with mime " + mime + " " + videoPath);
                } catch (java.io.IOException e) {
                    Log.e(TAG, "Create decoder codec from type ERROR !!!");
                }

                if(eosThread == true)
                {
                    releaseResource();
                    return;
                }

                String encode_mime = MediaFormat.MIMETYPE_VIDEO_AVC;
                //We need to signal component this is transcode task
                if(mWidth <= 0 || mHeight <= 0)
                {
                    mWidth = format.getInteger(MediaFormat.KEY_WIDTH);
                    mHeight = format.getInteger(MediaFormat.KEY_HEIGHT);
                }
                format.setInteger("eWid", mWidth);
                format.setInteger("eHei", mHeight);

                if(deinterlace >= 0 && deinterlace < 3)
                    format.setInteger("video-de-interlace", deinterlace);

                MediaFormat encode_format = MediaFormat.createVideoFormat(encode_mime, mWidth, mHeight);
                encode_format.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatSurface);
                if(bitRate == -1)
                    encode_format.setInteger(MediaFormat.KEY_BIT_RATE, mWidth*mHeight*15);
                else
                    encode_format.setInteger(MediaFormat.KEY_BIT_RATE, bitRate);

                if(frameRate.compareTo("Original") != 0)
                {
                    format.setInteger("eFps", Integer.parseInt(frameRate));
                    encode_format.setInteger(MediaFormat.KEY_FRAME_RATE, Integer.parseInt(frameRate));
                }
                else
                    encode_format.setInteger(MediaFormat.KEY_FRAME_RATE, 30);//That's for encoder reference only
                encode_format.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, frameInterval);

                encode_format.setInteger(MediaFormat.KEY_BITRATE_MODE, bitrateMode);
                encode_format.setInteger(MediaFormat.KEY_PROFILE, profile);
                encode_format.setInteger(MediaFormat.KEY_LEVEL, level);

                //Check rotation angle
                if(format.containsKey(MediaFormat.KEY_ROTATION)) {
                    mRotationAngle = format.getInteger(MediaFormat.KEY_ROTATION);
                    if(mRotationAngle != 0) {
                        Log.d(TAG, "Get rotation angle " + mRotationAngle);
                        encode_format.setInteger(MediaFormat.KEY_ROTATION, mRotationAngle);
                    }
                }

                try {
                    encoder = MediaCodec.createEncoderByType(encode_mime);
                    Log.d(TAG, "create encoder with mime " + mime + " "  + videoPath);
                } catch (java.io.IOException e) {
                    Log.e(TAG, "Create encoder codec from type ERROR !!!");
                }

                if(eosThread == true)
                {
                    releaseResource();
                    return;
                }

                decoderCallbackThread = new HandlerThread("DecoderHanlderThread");
                decoderCallbackThread.start();
                decoderHandler = new Handler(decoderCallbackThread.getLooper());
                Log.d(TAG, "register decoder callback " + videoPath);

                encoderCallbackThread = new HandlerThread("EncoderHanlderThread");
                encoderCallbackThread.start();
                encoderHandler = new Handler(encoderCallbackThread.getLooper());
                Log.d(TAG, "register encoder callback " + videoPath);

                setupDecoderCallback(decoderHandler);
                setupEncoderCallback(encoderHandler);

                if(eosThread == true)
                {
                    releaseResource();
                    return;
                }
                Log.d(TAG, "Encoder format " + encode_format);
                encoder.configure(encode_format, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
                mSurface = encoder.createInputSurface();
                Log.d(TAG, "Decoder format " + format);
                decoder.configure(format, mSurface, null, 0);
                break;
            }
        }

        if (decoder == null || encoder == null) {
            Log.e(TAG, "Create decoder or encoder FAIL !! " + videoPath);
            trackReady = true;
            return;
        }
        flag = 0;
        decoderInfo = new BufferInfo();
        decoder.start();
        encoder.start();
        flag = 1;
        startMs = System.currentTimeMillis();
        while (true) {
            if ((decoderInfo.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                Log.d(TAG, "OutputBuffer BUFFER_FLAG_END_OF_STREAM " + videoPath);
                isEOS = true;
                break;
            }
            else if(isEOS == true || eosThread == true)
            {
                try {
                    Thread.sleep(10);
                    Log.d(TAG, "Waiting for OutputBuffer BUFFER_FLAG_END_OF_STREAM " + videoPath);
                } catch(InterruptedException  ex) {
                }
                if(terminateTimeout > 0)
                    terminateTimeout--;
                else
                {
                    Log.d(TAG, "Terminate thread occur TIMEOUT !! ");
                    break;
                }
                flag = 25;
            }

            try {
                    Thread.sleep(1000);
                } catch(InterruptedException  ex) {
                }
        }
        flag = 26;
        releaseResource();
    }

    public void releaseResource()
    {
        eosThread = true;
        if(mStartRealease == false)
        {
            mStartRealease = true;

            Log.d(TAG, "Video Thread start releasing decoder !! " + videoPath);
            if(decoder != null)
            {
                if(mDecodeErr == false)
                {
                    decoder.flush();
                    decoder.stop();
                }
                decoder.release();
            }

            Log.d(TAG, "Video Thread start releasing encoder !! " + videoPath);
            if(encoder != null)
            {
                if(mEncodeErr == false)
                {
                    encoder.flush();
                    encoder.stop();
                }
                encoder.release();
            }

            Log.d(TAG, "Video Thread start releasing extractor !! " + videoPath);
            if(extractor != null)
                extractor.release();

            Log.d(TAG, "Video Thread start releasing callback !! " + videoPath);
            if(decoderCallbackThread != null && encoderCallbackThread != null)
            {
                decoderCallbackThread.quitSafely();
                encoderCallbackThread.quitSafely();
                try {
                    decoderCallbackThread.join();
                    encoderCallbackThread.join();
                    } catch(InterruptedException  ex) {
                    }
            }
            Log.d(TAG, "Video Thread normally Stop !! " + videoPath);
            flag = 27;
        }
    }
    private void setupDecoderCallback(Handler handle)
    {
        decoder.setCallback(new MediaCodec.Callback() {
        @Override
        public void onInputBufferAvailable(MediaCodec mc, int inputBufferId) 
        {
            if(mDecodeErr == true || mStartRealease == true)
                return;
            if (!isEOS) {
                ByteBuffer buffer = mc.getInputBuffer(inputBufferId);
                flag = 4;
                int sampleSize = extractor.readSampleData(buffer, 0);
                flag = 5;
                if (sampleSize < 0) {
                    Log.d(TAG, "InputBuffer BUFFER_FLAG_END_OF_STREAM " + videoPath);
                    flag = 6;
                    mc.queueInputBuffer(inputBufferId, 0, 0, 0, MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                    flag = 7;
                } else {
                    if(eosThread && formatInited)
                    {
                        Log.d(TAG, "InputBuffer force BUFFER_FLAG_END_OF_STREAM " + videoPath);
                        flag = 8;
                        mc.queueInputBuffer(inputBufferId, 0, sampleSize, extractor.getSampleTime(), MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                        flag = 9;
                        isEOS = true;
                    }
                    else
                    {
                        flag = 10;
                        mc.queueInputBuffer(inputBufferId, 0, sampleSize, extractor.getSampleTime(), 0);
                        mDecodeframeCount++;
                        mInstantDecodeframeCount++;
                        mDecodeFPS = mDecodeframeCount / (double)((System.currentTimeMillis() - startMs)/1000);
                        flag = 11;
                    }
                    flag = 12;
                    extractor.advance();
                    flag = 13;
                }
            }
        }

        @Override
        public void onOutputBufferAvailable(MediaCodec mc, int outputBufferId, MediaCodec.BufferInfo info) 
        {
            if(mDecodeErr == true || mStartRealease == true)
                return;
            decoderInfo = info;
            ByteBuffer buffer = mc.getOutputBuffer(outputBufferId);
            mc.releaseOutputBuffer(outputBufferId, true);
        }

        @Override
        public void onOutputFormatChanged(MediaCodec mc, MediaFormat format) 
        {
            Log.d(TAG, "New format " + decoder.getOutputFormat() + videoPath);
            formatInited = true;
            flag = 18;
        }

        @Override
        public void onError(MediaCodec codec, MediaCodec.CodecException e)
        {
            mDecodeErr = true;
            e.printStackTrace();
        }
        }, handle);
    }

    private void setupEncoderCallback(Handler handle)
    {
        encoder.setCallback(new MediaCodec.Callback() {
        @Override
        public void onInputBufferAvailable(MediaCodec mc, int inputBufferId) 
        {
            //We use encoder input surface, nothing to do
        }

        @Override
        public void onOutputBufferAvailable(MediaCodec mc, int outputBufferId, MediaCodec.BufferInfo info) 
        {
            if(mEncodeErr == true || mStartRealease == true)
                return;
            ByteBuffer buffer = mc.getOutputBuffer(outputBufferId);
            buffer.position(info.offset);
            buffer.limit(info.offset + info.size);
            if(muxFlg == true && muxer != null && eosThread == false)
                muxer.writeSampleData(videoTrackIdx, buffer, info);
            curTimeStamp = info.presentationTimeUs;
            //Log.d(TAG, "Muxer size " + info.size + " PTS " + info.presentationTimeUs);
            mc.releaseOutputBuffer(outputBufferId, false);
            mEncodeframeCount++;
            mInstantEncodeframeCount++;
            mEncodeFPS = mEncodeframeCount / (double)((System.currentTimeMillis() - startMs)/1000);

            try {
                if(streaming) {
                    while((curTimeStamp/1000 > (System.currentTimeMillis() - startMs) + 10000) && eosThread == false)
                        Thread.sleep(10);
                }
            } catch(InterruptedException  ex) {
            }
            flag = 24;
        }

        @Override
        public void onOutputFormatChanged(MediaCodec mc, MediaFormat format) 
        {
            Log.d(TAG, "New format " + encoder.getOutputFormat() + videoPath);
            if(trackReady == false)
            {
                videoTrackIdx = muxer.addTrack(encoder.getOutputFormat());
                trackReady = true;
                int errTimeout = 20;
                while(muxFlg == false && eosThread != true)
                {
                    try {
                        Thread.sleep(1000);
                        errTimeout--;
                        if(errTimeout < 0)
                            releaseResource();
                        Log.d(TAG, "Waiting audio track ready " + videoPath);
                    } catch(InterruptedException  ex) {
                    }
                }
            }
            flag = 18;
        }

        @Override
        public void onError(MediaCodec codec, MediaCodec.CodecException e)
        {
            mEncodeErr = true;
            e.printStackTrace();
        }
        }, handle);
    }
    private static void dumpFile(String fileName, byte[] data) {

        FileOutputStream outStream;
        try {
            Log.v(TAG, "output will be saved as " + fileName);
            outStream = new FileOutputStream(fileName, true);
        } catch (IOException ioe) {
            throw new RuntimeException("Unable to create debug output file " + fileName, ioe);
        }

        try {
            outStream.write(data);
            outStream.close();
        } catch (IOException ioe) {
            throw new RuntimeException("failed writing data to file " + fileName, ioe);
        }
    }

    public void resetInstantFrameCount() {
        mInstantDecodeframeCount = 0;
        mInstantEncodeframeCount = 0;
    }

    public void stopThread() {
        eosThread = true;
        Log.d(TAG, "received stop CMD " + videoPath);
    }
}
