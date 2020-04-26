package com.realtek.server;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.MediaScannerConnection;
import android.media.AudioManager;
import android.net.Uri;
import android.provider.MediaStore;
import android.os.Bundle;
import android.os.RemoteException;
import android.os.Handler;
import android.os.Message;
import android.os.UserHandle;
import android.util.Log;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningTaskInfo;
import com.realtek.app.IAirFunService;
import java.util.List;

public class AirFunService extends IAirFunService.Stub {
	static {
                System.loadLibrary("realtek_runtime");
	}

	interface OnEventListener {
		int onEvent(String msg);
	}

	private static final String TAG = "AirFunService";

	// Define the play back status
	public static final int PLAYBACKSTATUS_CURRENTPLAYMODE = 0;
	public static final int PLAYBACKSTATUS_CONTRAST = 1;
	public static final int PLAYBACKSTATUS_BRIGHTNESS = 2;
	public static final int PLAYBACKSTATUS_VOLUME = 3;
	public static final int PLAYBACKSTATUS_MUTE = 4;
	public static final int PLAYBACKSTATUS_TRANSPORTSTATE = 5;
	public static final int PLAYBACKSTATUS_DURATION = 6;
	public static final int PLAYBACKSTATUS_TIMEPOSITION = 7;
	public static final int PLAYBACKSTATUS_SEEK = 8;
	public static final int PLAYBACKSTATUS_TYPE = 9;

	// Define value for PLAYBACKSTATUS_TRANSPORTSTATE
	public static final int TRANSPORTSTATE_NoMedia = 0;
	public static final int TRANSPORTSTATE_Stopped = 1;
	public static final int TRANSPORTSTATE_Paused = 2;
	public static final int TRANSPORTSTATE_Playing = 3;
	public static final int TRANSPORTSTATE_Transitioning = 4;

	public static final int DOWNLOAD_OK = 0;	// download complete
	public static final int DOWNLOAD_NG = 1;	// download fail
	public static final int DOWNLOAD_GO = 2;	// downloading

    public static final int PLAY_TYPE_NONE = 0;
	public static final int PLAY_TYPE_VIDEO = 1;	// Play video
	public static final int PLAY_TYPE_AUDIO = 2;	// Play audio
    public static final int PLAY_TYPE_IMAGE = 4;	// Play image

    public static final int MIME_ID_VIDEO = 1;
    public static final int MIME_ID_AUDIO = 2;
    public static final int MIME_ID_IMAGE = 3;

    private static final String JNI_MESSAGE_KEY = "JniMsgKey";
    private static final int JNI_MESSAGE = 1;
    private static final int UPDATE_VOLUME_STATUS_MESSAGE = 2;

	private static float MAX_SYSTEM_VOLUME;
	private static int MAX_VOLUME = 32;

	private final Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case JNI_MESSAGE: {
                    Bundle bundle = msg.getData();
                    String string = bundle.getString(JNI_MESSAGE_KEY);
                    processJniStr(string);
                }
                break;
            case UPDATE_VOLUME_STATUS_MESSAGE: {
                    exeVolumeUpdater();
                    Message message = handler.obtainMessage(UPDATE_VOLUME_STATUS_MESSAGE);
                    handler.sendMessageDelayed(message, 1000);
                }
                break;
            default:
                break;
            }
        }
    };
	private AudioManager audioManager = null;

	private IntentFilter pbFilter = null;
	private boolean pbReceiverRegistered = false;
	private Context mContext;
	private String mDownloadUrl;
	private int mDownloadStatus;

    private int mPlayType = PLAY_TYPE_NONE;

	private int currentVolume = 0;
	private boolean stopVolumeMonitor = false;
    private boolean imagePlaybackLaunched = false;
    private boolean audioPlaybackLaunched = false;
	private static OnEventListener mOnEventListener = null;

	public AirFunService(Context context) {
		mContext = context;
		pbFilter = new IntentFilter("rtk.provider.playback.STATUS");

		startServer();
		mOnEventListener = AirFunEventListener;

		mContext.registerReceiver(mediaScanReceiver, new IntentFilter("rtk.provider.media.scan"));

		Log.d(TAG, "StartServer");
	}

	protected void finalize() throws Throwable {
		try {
		} finally {
			mContext.unregisterReceiver(mediaScanReceiver);
			hookPlaybackStatus(false);

			mOnEventListener = null;
			stopServer();

			Log.d(TAG, "StopServer");

			super.finalize();
		}
	}

	@Override
	public String openAndDownloadImage(String url)
			throws RemoteException {
		// TODO Auto-generated method stub
		mDownloadUrl = url;
		mDownloadStatus = DOWNLOAD_GO;
		return openAndStartDownload(url);
	}

	@Override
	public int getDownloadStatus() {
		return mDownloadStatus;
	}

	private BroadcastReceiver mediaScanReceiver = new BroadcastReceiver() {

		@Override
		public void onReceive(Context context, Intent intent) {
			String scanPath = intent.getStringExtra("path");
			//Log.d(TAG, "mediaScanReceiver(), scanPath(" + scanPath + ")");

			if( scanPath != null ) {
				MediaScannerConnection.scanFile(mContext,
							new String[] { scanPath	},
							null,
							new MediaScannerConnection.OnScanCompletedListener() {
								@Override
								public void onScanCompleted(String path, Uri uri) {
									Log.i(TAG, "Scanned " + path + ":");
									Log.i(TAG, "-> uri=" + uri);
								}
							});
			}
		}
	};

	private BroadcastReceiver playbackReceiver = new BroadcastReceiver() {

		@Override
		public void onReceive(Context context, Intent intent) {
			String playMode = intent.getStringExtra("PlayMode");
			String playType = intent.getStringExtra("PlayType");
            String sourceFrom = intent.getStringExtra("SourceFrom");

			if (playMode != null) {
                if (null == sourceFrom || false == sourceFrom.equals("DirectConnect")) {
                    return;
                }            
				Log.v(TAG, "PlayMode is received. - " + playMode);
				if (playMode.equalsIgnoreCase("Play")) {
					updatePlaybackStatus(
							PLAYBACKSTATUS_TRANSPORTSTATE,
							TRANSPORTSTATE_Playing);
					currentVolume = Math.round(audioManager.getStreamVolume(AudioManager.STREAM_MUSIC) /
										MAX_SYSTEM_VOLUME * MAX_VOLUME);
					updatePlaybackStatus(
							PLAYBACKSTATUS_VOLUME,
							currentVolume);
                    if (playType != null) {
                        if (playType.equals("Video")) {
                            mPlayType |= PLAY_TYPE_VIDEO;
                            updatePlaybackStatusByMime(
                                    MIME_ID_VIDEO,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Playing);
                        } else if (playType.equals("Audio")) {
                            mPlayType |= PLAY_TYPE_AUDIO;
                            updatePlaybackStatusByMime(
                                    MIME_ID_AUDIO,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Playing);
                        } else if (playType.equals("Image")) {
                            mPlayType |= PLAY_TYPE_IMAGE;
                            updatePlaybackStatusByMime(
                                    MIME_ID_IMAGE,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Playing);
                        } else if (playType.equals("Image/Audio")) {
                            mPlayType |= (PLAY_TYPE_IMAGE | PLAY_TYPE_AUDIO);
                            updatePlaybackStatusByMime(
                                    MIME_ID_AUDIO,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Playing);
                            updatePlaybackStatusByMime(
                                    MIME_ID_IMAGE,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Playing);
                        }
                        Log.d(TAG, "Current playType:" + mPlayType);
                        updatePlaybackStatus(
                                PLAYBACKSTATUS_TYPE,
                                mPlayType);
					}
				} else if (playMode.equalsIgnoreCase("Stop")) {
                    if (playType != null) {
                        if (playType.equals("Video")) {
                            mPlayType &= ~PLAY_TYPE_VIDEO;
                            updatePlaybackStatusByMime(
                                    MIME_ID_VIDEO,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Stopped);
                            // Reset time-position & duration since video playback is stopped
                            updatePlaybackStatus(PLAYBACKSTATUS_DURATION, 0);
                            updatePlaybackStatus(PLAYBACKSTATUS_TIMEPOSITION, 0);
                        } else if (playType.equals("Audio")) {
                            mPlayType &= ~PLAY_TYPE_AUDIO;
                            updatePlaybackStatusByMime(
                                    MIME_ID_AUDIO,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Stopped);
                            // Reset time-position & duration since audio playback is stopped
                            updatePlaybackStatus(PLAYBACKSTATUS_DURATION, 0);
                            updatePlaybackStatus(PLAYBACKSTATUS_TIMEPOSITION, 0);
                            audioPlaybackLaunched = false;
                        } else if (playType.equals("Image")) {
                            mPlayType &= ~PLAY_TYPE_IMAGE;
                            updatePlaybackStatusByMime(
                                    MIME_ID_IMAGE,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Stopped);
                            imagePlaybackLaunched = false;
                        } else if (playType.equals("Image/Audio")) {
                            mPlayType &= ~(PLAY_TYPE_IMAGE | PLAY_TYPE_AUDIO);
                            updatePlaybackStatusByMime(
                                    MIME_ID_AUDIO,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Stopped);
                            // Reset time-position & duration since audio playback is stopped
                            updatePlaybackStatus(PLAYBACKSTATUS_DURATION, 0);
                            updatePlaybackStatus(PLAYBACKSTATUS_TIMEPOSITION, 0);

                            updatePlaybackStatusByMime(
                                    MIME_ID_IMAGE,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Stopped);
                            audioPlaybackLaunched = false;
                            imagePlaybackLaunched = false;
                        }
                        else {
                            mPlayType = PLAY_TYPE_NONE;
                        }
                    }
                    Log.d(TAG, "Current playType:" + mPlayType);
                    updatePlaybackStatus(
                            PLAYBACKSTATUS_TYPE,
                            mPlayType);
                    if (mPlayType == PLAY_TYPE_NONE) {
                        Log.d(TAG, "Update transport state to stopped");
                        updatePlaybackStatus(
                                PLAYBACKSTATUS_TRANSPORTSTATE,
                                TRANSPORTSTATE_Stopped);
                        hookPlaybackStatus(false);
                    }
				} else if (playMode.equalsIgnoreCase("Pause")) {
                    if (playType != null) {
                        if (playType.equals("Video")) {
                            updatePlaybackStatusByMime(
                                    MIME_ID_VIDEO,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Paused);
                        } else if (playType.equals("Audio")) {
                            updatePlaybackStatusByMime(
                                    MIME_ID_AUDIO,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Paused);
                        } else if (playType.equals("Image")) {
                            updatePlaybackStatusByMime(
                                    MIME_ID_IMAGE,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Paused);
                        } else if (playType.equals("Image/Audio")) {
                            updatePlaybackStatusByMime(
                                    MIME_ID_AUDIO,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Paused);
                            updatePlaybackStatusByMime(
                                    MIME_ID_IMAGE,
                                    PLAYBACKSTATUS_TRANSPORTSTATE,
                                    TRANSPORTSTATE_Paused);
                        }
                    }
					updatePlaybackStatus(
							PLAYBACKSTATUS_TRANSPORTSTATE,
							TRANSPORTSTATE_Paused);
				}
			}

			int duration = intent.getIntExtra("Duration", -1);
			if (duration != -1) {
				updatePlaybackStatus(PLAYBACKSTATUS_DURATION,
						duration);
			}

			int timePosition = intent.getIntExtra("TimePosition", -1);
			if (timePosition != -1) {
				updatePlaybackStatus(
						PLAYBACKSTATUS_TIMEPOSITION,
						timePosition);
			}

			int seekSupported = intent.getIntExtra("SeekSupported", -1);
			if (seekSupported != -1) {
				updatePlaybackStatus(PLAYBACKSTATUS_SEEK,
						seekSupported != 0 ? 1 : 0);
			}
		}
	};

	private OnEventListener AirFunEventListener = new OnEventListener() {
		@Override
		public int onEvent(String evtStr) {
			try {
				Log.v(TAG, "AirFunService event string[" + evtStr + "]");
                Bundle bundle = new Bundle();
                Message message = handler.obtainMessage(JNI_MESSAGE);
                bundle.putString(JNI_MESSAGE_KEY, evtStr);
                message.setData(bundle);
                handler.sendMessageDelayed(message, 500);
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return 0;
		}
	};

	private void hookPlaybackStatus(boolean bEnable) {
		//Log.v(TAG, "hookPlaybackStatus " + bEnable);
		if( bEnable ) {
			if( ! pbReceiverRegistered ) {
				mContext.registerReceiver(playbackReceiver, pbFilter);
				pbReceiverRegistered = true;
				Log.v(TAG, "playbackReceiver register");
				if (null == audioManager)
				{
					audioManager = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
					MAX_SYSTEM_VOLUME = audioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
				}
				stopVolumeMonitor = false;
                startVolumeUpdater();
				//exeVolumeUpdater();
			}

		} else {
			if( pbReceiverRegistered ) {
				mContext.unregisterReceiver(playbackReceiver);
				pbReceiverRegistered = false;
				Log.v(TAG, "playbackReceiver unregister");
				stopVolumeMonitor = true;
                stopVolumeUpdater();
			}
		}
	}

	private String getMimeType(String urlStr) {
		String mimeType = null;
		int subIdx = urlStr.lastIndexOf("sub=");

		if( subIdx != -1 ) {
			subIdx = Integer.valueOf( urlStr.substring(subIdx + "sub=".length()) );

			switch(subIdx) {
			case 65:  // 'A'
				mimeType = "audio/*";
				break;
			case 70:  // 'F'
				mimeType = "video/*";
				break;
			case 73:  // 'I'
				mimeType = "image/*";
				break;
			default:
				Log.d(TAG, "non-supported sub type");
				break;
			}
		}
		return mimeType;
	}

	// expand the list command, directconnect://L?seq=3&size=2&kernelID=1&idx=0&sub=70
	// to an array of playlist:
	// directconnect://F?seq=3&size=2&kernelID=1&idx=0&sub=70
	// directconnect://F?seq=3&size=2&kernelID=1&idx=1&sub=70
	private String[] processListCommand(String url) {
		String temp = url.replaceFirst("L\\?", "F\\?");
		int size = temp.lastIndexOf("size=");
		int idx = temp.lastIndexOf("idx=");
		if( (size == -1) || (idx == -1) ) {
			return null;
		}

		idx += "idx=".length();
		String prefix = temp.substring(0, idx);
		String suffix = temp.substring(idx+1);
		temp = temp.substring(size + "size=".length());
		size = Integer.valueOf( temp.substring(0, temp.indexOf("&")) );
		if( size <= 0 ) {
			return null;
		}

		String[] playList = new String[size];
		for( int i=0; i<size; i++ ) {
			playList[i] = prefix + i + suffix;
		}
		/*
		for( int i=0; i<playList.length; i++) {
			Log.d(TAG, playList[i]);
		}
		 */
		return playList;
	}

	private void processPlayCommand(String[] urlList) {
		String mimeType = null;
		ComponentName comp = null;
        boolean startActivity = true;

		try {
			if (urlList == null) {
				throw new Exception("NULL URL");
			}

			mimeType = getMimeType(urlList[0]);
			if (mimeType == null) {
				throw new Exception("NULL mimeType");
			}

            ActivityManager am = (ActivityManager)mContext.getSystemService(Context.ACTIVITY_SERVICE);
            String topActivityClassName;
            List<RunningTaskInfo> taskInfo = am.getRunningTasks(1);
            topActivityClassName =  taskInfo.get(0).topActivity.getClassName();
            Log.d(TAG, "topActivity class name:" + topActivityClassName + " playType:" + mPlayType);
            if (topActivityClassName.equals("com.android.gallery3d.app.GalleryActivity") ||
                imagePlaybackLaunched == true) {
                if (mimeType.equals("audio/*")) {
                    Intent i = new Intent("rtk.gallery3d.background.MUSIC")
                                .putExtra("playlist-url", urlList);
                    i.putExtra("position", 0);
                    i.putExtra("android.intent.playback.broadcast.status", true);
                    i.putExtra("SourceFrom", "DirectConnect");
                    Log.d(TAG, "AirFun service broadcast intent to GalleryActivity");
                    mContext.sendStickyBroadcast(i);
                    startActivity = false;
                }
            } else if (topActivityClassName.equals("com.android.music.MediaPlaybackActivity") ||
                       audioPlaybackLaunched == true) {
                if (mimeType.equals("image/*")) {
                    Intent i = new Intent("com.android.music.setProperty")
                                .putExtra("backgroundPlay", true);
                    Log.d(TAG, "AirFun service broadcast intent to MediaPlaybackActivity");
                    mContext.sendBroadcastAsUser(i, new UserHandle(UserHandle.USER_ALL));
                }
            }

            if (startActivity == true) {
                Intent intent = new Intent();
                if (mimeType.equals("audio/*")) {
                    comp = new ComponentName(
                                            "com.android.music",
                                            "com.android.music.MediaPlaybackActivity");
                    intent.setAction(Intent.ACTION_VIEW);
                    intent.setComponent(comp);
                    intent.putExtra("playlist-url", urlList);
                    intent.putExtra("position", 0);

                    intent.putExtra( MediaStore.EXTRA_FINISH_ON_COMPLETION,	true);
                    intent.putExtra( "android.intent.playback.broadcast.status", true);
                    intent.putExtra("SourceFrom", "DirectConnect");
                    audioPlaybackLaunched = true;
                }
                else if (mimeType.equals("video/*")) {
                    comp = new ComponentName(
                            "com.android.gallery3d",
                            "com.android.gallery3d.app.MovieActivity");

                    intent.setAction(Intent.ACTION_VIEW);
                    intent.setDataAndType(Uri.parse(urlList[0]), mimeType);
                    intent.setComponent(comp);
                    intent.putExtra("playlist-url", urlList);
                    intent.putExtra( "android.intent.playback.broadcast.status", true);
                    intent.putExtra("com.realtek.player.USE_RT_MEDIAPLAYER", true);
                    intent.putExtra("SourceFrom", "DirectConnect");
                }
                else if (mimeType.equals("image/*")) {
                    comp = new ComponentName(
                            "com.android.gallery3d",
                            "com.android.gallery3d.app.GalleryActivity");

                    intent.setAction(Intent.ACTION_VIEW);
                    intent.setDataAndType(Uri.parse(urlList[0]), mimeType);
                    intent.setComponent(comp);
                    intent.putExtra("playlist", urlList);
                    intent.putExtra("slideshow", true);
                    intent.putExtra("repeat", false);
                    intent.putExtra("android.intent.playback.broadcast.status", true);
                    intent.putExtra("SourceFrom", "DirectConnect");
                    imagePlaybackLaunched = true;
                }
                else {
                    throw new Exception("Invalid mimeType");
                }

                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK
                        | Intent.FLAG_ACTIVITY_CLEAR_TASK
                        | Intent.FLAG_ACTIVITY_TASK_ON_HOME); // BUG_FIX: 43406 43830
                Log.d(TAG, "AirFun service start activity");
                mContext.startActivity(intent);
            }
		} catch (Exception e) {
			// TODO Auto-generated catch block
			System.out.println(e.getMessage());
		}
	}

	private void processDisconnectCommand(String urlStr) {
		Intent i = createCommand("stop", urlStr);
		if (i != null) {
			mContext.sendBroadcastAsUser(i, new UserHandle(UserHandle.USER_ALL));
		}
	}

	private void processPauseResumeCommand(Character cmdChar, String urlStr) {
		Intent i = null;
        String mimeType = getMimeType(urlStr);
		switch (cmdChar) {
		case 'p':
			i = createCommand("pause", urlStr);
			break;
		case 'P':
			i = createCommand("play", urlStr);
			break;
		case 'N':
			i = createCommand("next", urlStr);
            if (mimeType != null && (mimeType.equals("video/*") || mimeType.equals("audio/*"))) {
                // Reset time-position & duration since we want to play next audio/video
                updatePlaybackStatus(PLAYBACKSTATUS_DURATION, 0);
                updatePlaybackStatus(PLAYBACKSTATUS_TIMEPOSITION, 0);
            }
			break;
		case 'V':
			i = createCommand("previous", urlStr);
            if (mimeType != null && (mimeType.equals("video/*") || mimeType.equals("audio/*"))) {
                // Reset time-position & duration since we want to play previous audio/video
                updatePlaybackStatus(PLAYBACKSTATUS_DURATION, 0);
                updatePlaybackStatus(PLAYBACKSTATUS_TIMEPOSITION, 0);
            }
			break;
        case 'r':
            i = createCommand("repeat", urlStr);
            break;
		default:
			break;
		}

		if (i != null) {
			mContext.sendBroadcastAsUser(i, new UserHandle(UserHandle.USER_ALL));
		}
	}

	private void processSeekCommand(String urlStr) {
		String timeStr = null;

		int startIdx = urlStr.lastIndexOf("size=") + 5;
		int endIdx = urlStr.lastIndexOf("&kernelID");

		if( (startIdx!=-1) && (endIdx!=-1) ) {
			timeStr = urlStr.substring(startIdx, endIdx);
			if (timeStr != null) {
				Intent i = createCommand("seek", urlStr);
				if (i != null) {
					i.putExtra("seektime",
							Integer.parseInt(timeStr) * 1000);
					mContext.sendBroadcastAsUser(i, new UserHandle(UserHandle.USER_ALL));
				}
			}
		}
	}

	private void processVolCommand(String urlStr) {
		String volStr = null;
        boolean muteNow = false;
        int newVolume = 0;

		int startIdx = urlStr.lastIndexOf("size=");
		int endIdx = urlStr.lastIndexOf("&kernelID");

		if( (startIdx != -1) && (endIdx != -1) ) {
            startIdx += 5;
			volStr = urlStr.substring(startIdx, endIdx);
            if (volStr != null) {
                newVolume = Integer.parseInt(volStr);
                muteNow = this.audioManager.isStreamMute(AudioManager.STREAM_MUSIC);
                Log.d(TAG, "processVolCommand - muteNow:" + muteNow);

                if (muteNow) {
                    Log.d(TAG, "processVolCommand - Un-mute first");
                    processMuteCommand();
                }
                //Log.d(TAG, "processVolCommand - vol:" + volStr);
                audioManager.setStreamVolume(AudioManager.STREAM_MUSIC,
                                    (int) (Math.ceil(MAX_SYSTEM_VOLUME * newVolume / MAX_VOLUME)),
                                    AudioManager.FLAG_SHOW_UI);
                updatePlaybackStatus(PLAYBACKSTATUS_VOLUME, newVolume);
            }
		}
	}

	private void processMuteCommand() {
        boolean mute = false;
        boolean muteNow = this.audioManager.isStreamMute(AudioManager.STREAM_MUSIC);

        mute = !muteNow;
        Log.d(TAG, "Set mute to:" + mute);
		audioManager.setStreamMute(AudioManager.STREAM_MUSIC, mute);
        updatePlaybackStatus(PLAYBACKSTATUS_MUTE, (mute == true ? 1 : 0));
	}

    private void processRestartCommand(String urlStr) {
        String[] playList = processListCommand(urlStr);
        Intent i = createCommand("restart", urlStr);
        if (i != null && playList != null) {
            i.putExtra("playlist-url", playList);
			mContext.sendBroadcastAsUser(i, new UserHandle(UserHandle.USER_ALL));
		}
    }

    private void processUpdatePlaylistCommand(String urlStr) {
        String[] playList = processListCommand(urlStr);
        Intent i = createCommand("updatePlaylist", urlStr);
        if (i != null && playList != null) {
            i.putExtra("playlist-url", playList);
			mContext.sendBroadcastAsUser(i, new UserHandle(UserHandle.USER_ALL));
        }
    }

	private Intent createCommand(String cmdStr, String urlStr) {
		String mimeType = getMimeType(urlStr);
		Intent i = null;

		if (mimeType != null) {
			if (mimeType.equals("video/*")) {
				i = new Intent("com.android.gallery3d.movieactivitycommand");
			} else if (mimeType.equals("audio/*")) {
				i = new Intent("com.android.music.musicservicecommand");
			} else if (mimeType.equals("image/*")) {
				i = new Intent("com.android.gallery3d.photoviewcommand");
			}
			if (i != null) {
				Log.d(TAG, "command:" + cmdStr);
				i.putExtra("command", cmdStr);
			}
		}

		return i;
	}

    private void startVolumeUpdater() {
        Message message = handler.obtainMessage(UPDATE_VOLUME_STATUS_MESSAGE);
        handler.sendMessageDelayed(message, 1000);
    }

    private void stopVolumeUpdater() {
        handler.removeMessages(UPDATE_VOLUME_STATUS_MESSAGE);
    }

	private void exeVolumeUpdater() {
		if (true == stopVolumeMonitor) {
			return;
		}

        boolean muteNow = this.audioManager.isStreamMute(AudioManager.STREAM_MUSIC);
		int volumeNow = Math.round(this.audioManager.getStreamVolume(AudioManager.STREAM_MUSIC) / MAX_SYSTEM_VOLUME * MAX_VOLUME);
        //Log.d(TAG, "exeVolumeUpdater - muteNow:" + muteNow + " volumeNow:" + volumeNow);
        updatePlaybackStatus(PLAYBACKSTATUS_MUTE, (muteNow == true || volumeNow == 0 ? 1 : 0));

        if (currentVolume != volumeNow) {
            if (false == muteNow) {
                currentVolume = volumeNow;
            }

            //Log.d(TAG, "exeVolumeUpdater, currentVolume:" + currentVolume);
            updatePlaybackStatus(PLAYBACKSTATUS_VOLUME, currentVolume);
        }
	}

    private void processJniStr(String jniStr) {
        int cmdSeparator = jniStr.indexOf("|");
        if( cmdSeparator != -1) {
            Character cmdChar = jniStr.charAt(cmdSeparator - 1);
            String url = jniStr.substring(cmdSeparator + 1);

            if(null != cmdChar) {
                switch (cmdChar) {
                case 'L':	// 'L' List
                    String[] playList = processListCommand(url);
                    hookPlaybackStatus(true);
                    processPlayCommand(playList);
                    break;
                case 'p':	// 'P' Resume play
                case 'P':	// 'p' Pause
                case 'N':	// 'N' Next
                case 'V':	// 'V' Previous
                case 'r':   // 'r' Repeat
                    processPauseResumeCommand(cmdChar, url);
                    break;
                case 'J':	// 'J' Seek
                    processSeekCommand(url);
                    break;
                case 'D':	// 'D' Disconnect
                    processDisconnectCommand(url);
                    //hookPlaybackStatus(false);
                    break;
                case 'S':	// Download OK
                    Log.d(TAG, "Image download ok");
                    mDownloadStatus = DOWNLOAD_OK;
                    /*
                    if( url.equals(mDownloadUrl) ) {
                        mDownloadStatus = DOWNLOAD_OK;
                    }
                    */
                    break;
                case 'Z':	// Download Fail
                    Log.d(TAG, "Image download fail");
                    mDownloadStatus = DOWNLOAD_NG;
                    /*
                    if( url.equals(mDownloadUrl) ) {
                        mDownloadStatus = DOWNLOAD_NG;
                    }
                    */
                    break;
                case 'v':
                    processVolCommand(url);
                    break;
                case 'm':
                    processMuteCommand();
                    break;
                case 's':   // 's' Restart
                    processRestartCommand(url);
                    break;
                case 'l':   // 'l' Update playlist
                    processUpdatePlaylistCommand(url);
                    break;
                default:
                    break;
                }
            }
        }
    }

	// Native methods
	private native final boolean startServer();
	private native final void stopServer();
	private native final void updatePlaybackStatus(int type, int value);
    private native final void updatePlaybackStatusByMime(int mine, int type, int value);
	private native final String openAndStartDownload(String url);
	public native final String[] extractMetaData(String url);

	// Native callback method
	// DirectConnect commands from DirectConnectAgent.
	public final static boolean notify(String msg) {
		if (mOnEventListener != null) {
			mOnEventListener.onEvent(msg);
		}

		return false;
	}
}
