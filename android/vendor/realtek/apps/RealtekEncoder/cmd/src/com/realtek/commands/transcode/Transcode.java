/*
 * Copyright (C) 2007 The Android Open Source Project
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

package com.realtek.commands.transcode;

import java.io.IOException;

import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.content.Context;
import android.content.Intent;
import android.content.ComponentName;
import android.content.ServiceConnection;

import com.realtek.transcode.TranscodeManager;

public final class Transcode {
    private static final String TAG = "Transcode";

    private static final int VIDEO_ONLY = 0;
    private static final int AUDIO_ONLY = 1;
    private static final int AUDIO_AND_VIDEO = 2;

    private String[] mArgs;
    private int mNextArg;
    private String mCurArgData;

    private String mInputFile = null;
    private String mOutputFile = "/dev/null";
    private String mOutputFormat = "MPEG2TS";
    private int mOutputTracks = AUDIO_AND_VIDEO;
    private int mTargetWidth = 0;
    private int mTargetHeight = 0;
    private String mVideoFrameRate = "Original";
    private int mVideoBitrate = -1;
    private int mVideoFrameInterval = 10;
    private int mDeinterlace = 0;
    private int mBitrateMode = 0;
    private int mProfile = 8;
    private int mLevel = 8192;
    private String mAudioMode = "AAC";
    private int mAudioSampleRate = 48000;
    private int mAudioChannel = 2;
    private int mKillTaskWithIdx = -1;
    private int mRepeatTimes = 0;
    private String mUdpIp = "239.0.0.1";
    private int mUdpPort = 7890;

    private TranscodeManager mManager;

    public static void main(String[] args) {
        int exitCode = 1;
        try {
            exitCode = new Transcode().run(args);
        } catch (Exception e) {
            Log.e(TAG, "Error", e);
            System.err.println("Error: " + e);
            if (e instanceof RemoteException) {
                System.err.println("run transcode error");
            }
        }
        System.exit(exitCode);
    }

    private int run(String[] args) throws IOException, RemoteException {
        if (args.length < 1) {
            return showUsage();
        }

        mArgs = args;
        mNextArg = 0;

        try {
            String opt;
            while ((opt=nextOption()) != null) {
                if (opt.equals("-o")) {
                    mInputFile = nextArg();
                } else if (opt.equals("-d")) {
                    mOutputFile = nextArg();
                } else if (opt.equals("-m")) {
                    mOutputFormat = nextArg();
                } else if (opt.equals("-t")) {
                    mOutputTracks = Integer.parseInt(nextArg());
                } else if (opt.equals("-w")) {
                    mTargetWidth = Integer.parseInt(nextArg());
                } else if (opt.equals("-h")) {
                    mTargetHeight = Integer.parseInt(nextArg());
                } else if (opt.equals("-f")) {
                    mVideoFrameRate = nextArg();
                } else if (opt.equals("-b")) {
                    mVideoBitrate = Integer.parseInt(nextArg());
                } else if (opt.equals("-i")) {
                    mVideoFrameInterval = Integer.parseInt(nextArg());
                } else if (opt.equals("-a")) {
                    mDeinterlace = Integer.parseInt(nextArg());
                } else if (opt.equals("-e")) {
                    mBitrateMode = Integer.parseInt(nextArg());
                } else if (opt.equals("-p")) {
                    mProfile = Integer.parseInt(nextArg());
                } else if (opt.equals("-l")) {
                    mLevel = Integer.parseInt(nextArg());
                } else if (opt.equals("-s")) {
                    mAudioSampleRate = Integer.parseInt(nextArg());
                } else if (opt.equals("-c")) {
                    mAudioChannel = Integer.parseInt(nextArg());
                } else if (opt.equals("-j")) {
                    mUdpIp = nextArg();
                } else if (opt.equals("-n")) {
                    mUdpPort = Integer.parseInt(nextArg());
                } else if (opt.equals("-g")) {
                    mRepeatTimes = Integer.parseInt(nextArg());
                } else if (opt.equals("-k")) {
                    mKillTaskWithIdx = Integer.parseInt(nextArg());
                } else {
                    System.err.println("Error: Unknown option: " + opt);
                    return 1;
                }

            }
        } catch (RuntimeException ex) {
            System.err.println("Error: " + ex.toString());
            return 1;
        }

        if(mKillTaskWithIdx != -1) {
            mManager = new TranscodeManager();
            mManager.killSpecificTranscodeIdx(mKillTaskWithIdx);
            return 1;
        }

        if (mInputFile == null) {
            System.err.println("Error: no input file specified");
            showUsage();
            return 1;
        }

        mManager = new TranscodeManager();
        mManager.initTranscode();
        mManager.setDataSource(mInputFile, mOutputFile, mOutputFormat, mOutputTracks, mRepeatTimes);
        mManager.setVideoParameter(mTargetWidth, mTargetHeight, mVideoFrameRate, mVideoBitrate, mVideoFrameInterval, mDeinterlace, mBitrateMode, mProfile, mLevel);
        mManager.setAudioParameter(mAudioMode, mAudioSampleRate, mAudioChannel);

        if(mUdpIp.compareTo("239.0.0.1") != 0 || mUdpPort != 7890)
            mManager.setNetworkParameter(mUdpIp, mUdpPort);

        mManager.startTranscode();

        do{
            long curPts = mManager.getCurrentPTS();
            String decAvgFps = String.format("%.2f", mManager.getDecodeAverageFPS());
            String encAvgFps = String.format("%.2f", mManager.getEncodeAverageFPS());
            String decCurFps = String.format("%.2f", mManager.getVideoDecodeCurrentFPS());
            String encCurFps = String.format("%.2f", mManager.getVideoEncodeCurrentFPS());
            Log.i(TAG, "pts:" + curPts + " average:" + decAvgFps + "/" + encAvgFps + " current:" + decCurFps + "/" + encCurFps);
            try {
                Thread.sleep(1000);
            } catch(InterruptedException  ex) {
            }
            //Log.i(TAG, "alive A:" + mManager.isAudioTranscodeAlive() + " V:" + mManager.isVideoTranscodeAlive());
        }while(mManager.getTranscodeState() == 0);
        if(mRepeatTimes == 0)
            mManager.stopTranscode();
        return 0;
    }

    private int startTranscode() {
        System.err.println("Start transcoding...");
        return 0;
    }
    private String nextOption() {
        if (mNextArg >= mArgs.length) {
            return null;
        }
        String arg = mArgs[mNextArg];
        if (!arg.startsWith("-")) {
            return null;
        }
        mNextArg++;
        if (arg.equals("--")) {
            return null;
        }
        if (arg.length() > 1 && arg.charAt(1) != '-') {
            if (arg.length() > 2) {
                mCurArgData = arg.substring(2);
                return arg.substring(0, 2);
            } else {
                mCurArgData = null;
                return arg;
            }
        }
        mCurArgData = null;
        return arg;
    }

    private String nextOptionData() {
        if (mCurArgData != null) {
            return mCurArgData;
        }
        if (mNextArg >= mArgs.length) {
            return null;
        }
        String data = mArgs[mNextArg];
        mNextArg++;
        return data;
    }

    private String nextArg() {
        if (mNextArg >= mArgs.length) {
            return null;
        }
        String arg = mArgs[mNextArg];
        mNextArg++;
        return arg;
    }

    private static int showUsage() {
        System.err.println("usage: transcode [-o src] [-d dst] [-m muxer] [-t track] [-w width] [-h height] [-f fps] [-b bitrate] [-i interval] [-a deinterlace] ");
        System.err.println("[-e bitrate mode] [-p profile] [-l level] [-s samplerate] [-c channel] [-j IP] [-n port] [-g repeat] [-k kill] ");
        System.err.println("    -o src: input file");
        System.err.println("    -d dst: output file path / UDP");
        System.err.println("    -m muxer: MPEG2TS / MP4");
        System.err.println("    -t tracks: 0:VideoOnly 1:AudioOnly 2:Audio+Video");
        System.err.println("    -w width: target width");
        System.err.println("    -h height: target height");
        System.err.println("    -f fps: Original / 60 / 50 / 30 / 24 ...");
        System.err.println("    -b bitrate: default width x height x 15");
        System.err.println("    -i interval: I frame interval");
        System.err.println("    -a deinterlace: 0 (AUTO) / 1 (DEINT) / 2 (PROGRESS)");
        System.err.println("    -e bit rate mode: 0 (disable)/ 1 (variable)/ 2 (Constant)/ 3 (VariableSkipFrames)/ 4 (ConstantSkipFrame)");
        System.err.println("    -p AVC profile: 1 / 2 / 8");
        System.err.println("    -l AVC level: 1/ 2/ 4/ 8/ 16/ 32/ 64/ 128/ 256/ 512/ 1024/ 2048/ 4096/ 8192");
        System.err.println("    -s samplerate: 48000 / 44100 / ...");
        System.err.println("    -c channel: 1 ~ 6");
        System.err.println("    -j IP address: ex. 239.0.0.1");
        System.err.println("    -n UDP port: ex. 7890");
        System.err.println("    -g test: repeat times");
        System.err.println("    -k kill transcode idx: 0 ~ 3 (4 for all)");
        return 1;
    }
}

