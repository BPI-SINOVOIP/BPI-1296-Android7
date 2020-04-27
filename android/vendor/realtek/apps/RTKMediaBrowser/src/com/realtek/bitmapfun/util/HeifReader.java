package com.realtek.bitmapfun.util;

import java.io.IOException;
import java.io.ByteArrayOutputStream;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaColorConverter;
import android.media.MediaExtractor;
import android.media.MediaFormat;
import android.media.MediaCodec.BufferInfo;
import android.util.Log;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
import android.graphics.Matrix;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.HandlerThread;
import android.widget.ImageView;

import android.renderscript.RenderScript;
import android.renderscript.ScriptIntrinsicYuvToRGB;
import android.renderscript.Type;
import android.renderscript.Allocation;
import android.renderscript.Element;
import android.content.Context;

public class HeifReader extends AsyncTask<Void, Void, Void>
{
    public static final String TAG = "HeifReader";
    public static final String TAG2 = "[RTKRRS]";
    private static final int MAX_BITMAP_SIZE = 4096;
    private static final boolean DEBUG_CROP = true;

    private Context mContext;
    private MediaExtractor extractor;
    private MediaCodec decoder;
    private MediaFormat mFormat;
    private String mPath;
    private boolean eosThread = false;
    private boolean formatInited = false;
    private int flag = 0;
    private int terminateTimeout = 3;
    private boolean isEOS = false;
    private BufferInfo decoderInfo;
    private HandlerThread decoderCallbackThread;
    private Handler decoderHandler;
    private boolean isCodecErr = false;
    private boolean mStartRealease = false;
    private Bitmap mBitmap = null;
    private int mTileIndex=0;
    private int mCount=0;
    private int mAllocCount=0;
    private ImageView mImageView;
    private int mRows;
    private int mColumns;
    private int mOutputWidth;
    private int mOutputHeight;
    private int mWidth;
    private int mHeight;
    private int mRotate;
    private long decodeTime = 0;

    private RenderScript rs;
    private ScriptIntrinsicYuvToRGB yuvToRgbIntrinsic;
    private Type.Builder yuvType, rgbaType;
    private Allocation in, out;

    public HeifReader(Context context, String path, ImageView imageView)
    {
        this.mPath = path;
        this.mImageView = imageView;
        this.mContext = context;
    }

    @Override
    protected void onPreExecute()
    {
        super.onPreExecute();
    }

    @Override
    protected Void doInBackground(Void... para)
    {
        decodeHeif();
        return null;
    }

    @Override
    protected void onPostExecute (Void rlt)
    {
        Log.d(TAG, "HeifDecodeTask onPostExecute ");
        if(mImageView!=null)
        {
            mImageView.setImageBitmap(mBitmap);
        }
    }

    @Override
    protected void onCancelled()
    {
        super.onCancelled();
        Log.d(TAG, "HeifDecodeTask onCancelled ");
    }
    @Override
    protected void finalize ()throws Throwable
    {
        super.finalize();
        releaseResource();
        Log.d(TAG, "HeifDecodeTask finalize()");
    }

    public void releaseResource()
    {
        if(eosThread == false)
            eosThread = true;
        if(mStartRealease == false)
        {
            Log.d(TAG, "Video Thread start releasing !! " + mPath);
            mStartRealease = true;
            if(decoder != null)
            {
                if(isCodecErr == false)
                {
                    decoder.flush();
                    decoder.stop();
                }
                decoder.release();
            }

            if(extractor != null)
                extractor.release();

            if(rs!=null)
            {
                rs.destroy();
            }

            if(decoderCallbackThread != null)
            {
                decoderCallbackThread.quitSafely();
                try
                {
                    decoderCallbackThread.join();
                }
                catch(InterruptedException  ex)
                {
                }
            }
            flag = -1;
        }
    }

    public void stopThread()
    {
        Log.e(TAG2, "ErrCode 6 received VideoDecodeThread terminate command ");
        eosThread = true;
    }

    public void cropYUV420SP(byte[] srcBuffer, int srcWidth, int srcHeight, int srcActualWidth,
                             int srcActualHeight, byte[] dstBuffer, int offset_x, int offset_y, int dstWidth, int dstHeight)
    {
        if(offset_x + srcActualWidth > dstWidth || offset_y + srcActualHeight > dstHeight)
        {
            return;
        }
        for(int i=0; i<srcActualHeight; i++)
        {
            System.arraycopy(srcBuffer, i*srcWidth, dstBuffer, offset_x + offset_y*dstWidth + i*dstWidth, srcActualWidth);
            if(i%2==0)
            {
                System.arraycopy(srcBuffer, srcWidth*srcHeight-1 + i/2*srcWidth, dstBuffer, dstWidth*dstHeight-1 + offset_x + offset_y/2*dstWidth + i/2*dstWidth, srcActualWidth);
            }
        }
    }

    public void decodeYUV420SP(int[] rgba, byte[] yuv420sp, int width, int height, int actual_width, int actual_height)
    {
        if(rs==null)
        {
            rs = RenderScript.create(mContext);
            yuvToRgbIntrinsic = ScriptIntrinsicYuvToRGB.create(rs, Element.U8_4(rs));
        }
        if (yuvType == null)
        {
            yuvType = new Type.Builder(rs, Element.YUV(rs)).setX(yuv420sp.length);
            in = Allocation.createTyped(rs, yuvType.create(), Allocation.USAGE_SCRIPT);

            rgbaType = new Type.Builder(rs, Element.RGBA_8888(rs)).setX(width).setY(height);
            out = Allocation.createTyped(rs, rgbaType.create(), Allocation.USAGE_SCRIPT);
        }

        swapUV(yuv420sp, width, height);
        in.copyFrom(yuv420sp);

        yuvToRgbIntrinsic.setInput(in);
        yuvToRgbIntrinsic.forEach(out);

        Bitmap bmpout = null;;
        try
        {
            bmpout = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            out.copyTo(bmpout);
            bmpout.getPixels(rgba, 0, actual_width, 0, 0, actual_width, actual_height);
            bmpout.recycle();
            bmpout = null;
            System.gc();
        }
        catch (OutOfMemoryError e)
        {
            Runtime.getRuntime().gc();
            Log.d(TAG, mPath + ": " + e);
        }
    }



    public void swapUV(byte[] data, int width, int height)
    {
        final int frameSize = width * height;
        byte temp =0;
        for(int i=0; i<frameSize/4 && i<data.length-1; i++)
        {
            temp = data[frameSize+2*i];
            data[frameSize+2*i] = data[frameSize+2*i+1];
            data[frameSize+2*i+1] = temp;
        }
    }

    public Bitmap rotateBitmap(Bitmap source, float angle, int width, int height, boolean recycle)
    {
        Matrix matrix = new Matrix();
        matrix.postRotate(angle);
        Bitmap bmpout = null;
        try
        {
            bmpout =  Bitmap.createBitmap(source, 0, 0, width, height, matrix, true);
        }
        catch (OutOfMemoryError e)
        {
            System.gc();
            Runtime.getRuntime().gc();
            Log.d(TAG, mPath + ": " + e);
        }
        if (recycle)
        {
            source.recycle();
            source=null;
            System.gc();
        }
        return bmpout;
    }

    public Bitmap resizeBitmapByScale(Bitmap bitmap, float scale, boolean recycle)
    {
        if(mBitmap == null)
        {
            Log.d(TAG, "resizeBitmapByScale: bitmap is null. ("+mPath+")");
        }

        int width = Math.round(bitmap.getWidth() * scale);
        int height = Math.round(bitmap.getHeight() * scale);
        if (width == bitmap.getWidth() && height == bitmap.getHeight()) return bitmap;
        Bitmap target = null;
        try
        {
            target = Bitmap.createScaledBitmap(bitmap, width, height, true);
        }
        catch (OutOfMemoryError e)
        {
            System.gc();
            Runtime.getRuntime().gc();
            Log.d(TAG, mPath + ": " + e);
        }

        if (recycle)
        {
            bitmap.recycle();
            bitmap=null;
            System.gc();
        }
        return target;
    }

    private void setupDecoderCallback(Handler handle)
    {
        decoder.setCallback(new MediaCodec.Callback()
        {
            @Override
            public void onInputBufferAvailable(MediaCodec mc, int inputBufferId)
            {
                if(mStartRealease == true)
                    return;
                if (!isEOS)
                {
                    ByteBuffer buffer = mc.getInputBuffer(inputBufferId);
                    int sampleSize = extractor.readSampleData(buffer, 0);
                    long timeStamp = extractor.getSampleTime();
                    flag = 2;
                    if (sampleSize < 0)
                    {
                        Log.d(TAG, "InputBuffer BUFFER_FLAG_END_OF_STREAM " + mPath);
                        mc.queueInputBuffer(inputBufferId, 0, 0, 0, MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                        isEOS = true;
                    }
                    else
                    {
                        if(eosThread && formatInited)
                        {
                            Log.d(TAG, "InputBuffer force BUFFER_FLAG_END_OF_STREAM " + mPath);
                            mc.queueInputBuffer(inputBufferId, 0, sampleSize, timeStamp, MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                            isEOS = true;
                        }
                        else
                        {
                            mc.queueInputBuffer(inputBufferId, 0, sampleSize, timeStamp, 0);
                            flag = 3;
                        }
                        extractor.advance();
                    }
                }
            }

            @Override
            public void onOutputBufferAvailable(MediaCodec mc, int outputBufferId, MediaCodec.BufferInfo info)
            {
                if(decodeTime==0)
                    decodeTime = System.currentTimeMillis();

                decoderInfo = info;
                if(mStartRealease == true)
                    return;

                try
                {
                    if(mTileIndex < mColumns*mRows)
                    {
                        ByteBuffer buffer = mc.getOutputBuffer(outputBufferId);
                        byte[] ba = new byte[buffer.remaining()];
                        buffer.get(ba);

                        int actual_x = (mTileIndex%mColumns)*mWidth;
                        int actual_y = (mTileIndex/mColumns)*mHeight;
                        int actual_width = (mOutputWidth<actual_x+mWidth)?(mOutputWidth-actual_x):mWidth;
                        int actual_height = (mOutputHeight<actual_y+mHeight)?(mOutputHeight-actual_y):mHeight;
                        int[] pixels = new int[actual_width*actual_height];

//                        decodeYUV420SP(pixels, ba, mWidth, mHeight, actual_width, actual_height);
                        MediaColorConverter mediaColorConverter = new MediaColorConverter();
                        mediaColorConverter.NV12ToARGB(ba, mWidth, mHeight, actual_width, actual_height, pixels);
                        if(mBitmap!=null)
                        {
                            mBitmap.setPixels(pixels, 0, actual_width, actual_x, actual_y, actual_width, actual_height);
                        }
                    }

                    mTileIndex++;
                    if(mTileIndex >= mColumns*mRows)
                    {
                        Log.d(TAG, mPath+ " decodeTime: " + (System.currentTimeMillis() - decodeTime));
                    }

                    mc.releaseOutputBuffer(outputBufferId, false);
                }
                catch (IllegalStateException e1)
                {
                    Log.d(TAG2, "Exception in onOutputBufferAvailable: " + e1);
                }
                flag = 4;
            }

            @Override
            public void onOutputFormatChanged(MediaCodec mc, MediaFormat format)
            {
                Log.d(TAG, "New format " + decoder.getOutputFormat() + mPath);
                formatInited = true;
            }

            @Override
            public void onError(MediaCodec codec, MediaCodec.CodecException e)
            {
                isCodecErr = true;
                e.printStackTrace();
                if(isCodecErr)
                    Log.e(TAG2, "ErrCode 5 Callback onError " + mPath);
            }
        }, handle);
    }

    public void openHeif()
    {
        extractor = new MediaExtractor();
        Log.d(TAG, "Start setDataSource ... " + mPath);
        try
        {
            extractor.setDataSource(mPath);
        }
        catch (IOException e1)
        {
            // TODO Auto-generated catch block
            e1.printStackTrace();
            releaseResource();
            Log.d(TAG2, "ErrCode 1 setDataSource IO error " + mPath);
            return;
        }
        Log.d(TAG, "End setDataSource ... " + mPath);
        for (int i = 0; i < extractor.getTrackCount(); i++)
        {
            mFormat = extractor.getTrackFormat(i);
            String mime = mFormat.getString(MediaFormat.KEY_MIME);
            Log.d(TAG, "mime type " + mime);
            if (mime.startsWith("video/"))
            {
                mRows = mFormat.containsKey("rows")?mFormat.getInteger("rows"):1;
                mColumns = mFormat.containsKey("columns")?mFormat.getInteger("columns"):1;
                mWidth = mFormat.getInteger(MediaFormat.KEY_WIDTH);
                mHeight = mFormat.getInteger(MediaFormat.KEY_HEIGHT);
                mRotate = mFormat.containsKey(MediaFormat.KEY_ROTATION)?mFormat.getInteger(MediaFormat.KEY_ROTATION):0;
                mOutputWidth = mFormat.containsKey("outputWidth")?mFormat.getInteger("outputWidth"):mWidth;
                mOutputHeight = mFormat.containsKey("outputHeight")?mFormat.getInteger("outputHeight"):mHeight;
                Log.d(TAG, "rows:"+mRows+", columns:"+mColumns+", outputWidth:"+mOutputWidth+", outputHeight:"+mOutputHeight+", rotate:"+mRotate);

                try
                {
                    extractor.selectTrack(i);
                }
                catch(Exception e)
                {
                    Log.e(TAG2, "ErrCode 2 selectTrack " + i + " occur " + e.toString() + " !!");
                    releaseResource();
                    return;
                }
                break;
            }
        }
    }

    public synchronized Bitmap decodeHeif()
    {
        openHeif();
        Log.d(TAG, "decodeHeif [Start]");

        if(!DEBUG_CROP)
        {
            mOutputWidth = mWidth*mColumns;
            mOutputHeight = mHeight*mRows;
        }

        System.gc();
        Runtime.getRuntime().gc();

        while(mBitmap==null && mAllocCount++<10)
        {
            try
            {
                mBitmap = Bitmap.createBitmap(mOutputWidth, mOutputHeight, Bitmap.Config.ARGB_8888);
            }
            catch (OutOfMemoryError e)
            {
                System.gc();
                Runtime.getRuntime().gc();
                Log.d(TAG, mPath + ": " + e);
            }
        }
        if(mBitmap == null)
        {
            Log.d(TAG, "mBitmap create failed. ("+mPath+").");
        }

        if (!isCancelled())
        {
            try
            {
                String mime = mFormat.getString(MediaFormat.KEY_MIME);
                mFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar);
                mFormat.setInteger(MediaFormat.KEY_PUSH_BLANK_BUFFERS_ON_STOP,1);
                decoder = MediaCodec.createDecoderByType(mime);
                Log.d(TAG, "create decoder with mime " + mime);
            }
            catch (java.io.IOException e)
            {
                Log.e(TAG2, "ErrCode 3 Create decoder codec from type ERROR !!!");
                releaseResource();
                return null;
            }

            decoderCallbackThread = new HandlerThread("DecoderHanlderThread");
            decoderCallbackThread.start();
            decoderHandler = new Handler(decoderCallbackThread.getLooper());
            setupDecoderCallback(decoderHandler);
            decoder.configure(mFormat, null, null, 0);

            if (decoder == null)
            {
                Log.e(TAG2, "ErrCode 4 Create decoder FAIL !! " + mPath);
                releaseResource();
                return null;
            }
            decoderInfo = new BufferInfo();
            decoder.start();
            flag = 1;
            while (true)
            {
                if(mTileIndex < mColumns*mRows)
                {
                    if(mCount++ > mColumns*mRows + 100)
                    {
                        Log.d(TAG, "Timeout when mTileIndex("+mTileIndex+") is always less than mColumns("+mColumns+")*mRows("+mRows+")");
                        break;
                    }
                }
                else if((decoderInfo.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0)
                {
                    Log.d(TAG, "OutputBuffer BUFFER_FLAG_END_OF_STREAM " + mPath);
                    break;
                }
                else if(eosThread == true)
                {
                    Log.d(TAG, "Waiting for OutputBuffer BUFFER_FLAG_END_OF_STREAM " + mPath);
                    terminateTimeout--;
                    if(terminateTimeout < 0)
                    {
                        Log.d(TAG, "Terminate thread occur TIMEOUT !! ");
                        break;
                    }
                }

                try
                {
                    Thread.sleep(20);
                }
                catch(InterruptedException  ex)
                {
                }
            }
            releaseResource();
        }

        float scale = Math.min( (float) MAX_BITMAP_SIZE/mOutputWidth, (float) MAX_BITMAP_SIZE/mOutputHeight);
        Bitmap ret = mBitmap;

        if (scale < 1 && mBitmap!=null)
        {
            ret =  resizeBitmapByScale(mBitmap, scale, true);
        }
        Log.d(TAG, "decodeHeif [End]");
        Log.d(TAG, mPath+ " decodeTime 2: " + (System.currentTimeMillis() - decodeTime));

        System.gc();
        Runtime.getRuntime().gc();
        if(ret == null)
        {
            Log.d(TAG, "ret is null. ("+mPath+")");
        }
        return ret;
    }

    public Bitmap getThumbnail()
    {
        int thumb_width = 604;
        int thumb_height = 420;
        Bitmap bmpout = decodeHeif();
        Bitmap thumb = bmpout;
        if(bmpout!=null)
        {
            float scale = Math.min( (float) thumb_width /bmpout.getWidth(), (float) thumb_height /bmpout.getHeight());
            if (scale <= 0.5)
            {
                thumb =  resizeBitmapByScale(bmpout, scale, true);
            }
            thumb = rotateBitmap(thumb, mRotate, thumb.getWidth(), thumb.getHeight(), true);
        }
        return thumb;
    }

    public Bitmap getBitmap()
    {
        return mBitmap;
    }

    public int getRotation()
    {
        openHeif();
        releaseResource();
        return mRotate;
    }

    public int getWidth()
    {
        openHeif();
        releaseResource();
        return mOutputWidth;
    }

    public int getHeight()
    {
        openHeif();
        releaseResource();
        return mOutputHeight;
    }
}
