package com.realtek.Utils;



import java.lang.ref.SoftReference;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.Semaphore;
import android.media.MediaPlayer;
import android.os.Handler;
import android.os.Message;

public class AsyncInfoLoader {
	private static final int MAX_AVAILABLE = 1;
	private final Semaphore available = new Semaphore(MAX_AVAILABLE, true);
	private HashMap<String, SoftReference<Integer>> timeCache;
	//MediaPlayer mp = new MediaPlayer();
	public AsyncInfoLoader() {
		timeCache = new HashMap<String, SoftReference<Integer>>();
	}

	public Integer loadTime(final String url,
			final TimeInfoCallback infoCallback) {
		if (timeCache.containsKey(url)) {
			SoftReference<Integer> softReference = timeCache.get(url);
			Integer time = softReference.get();
			if (time != null) {
				return time;
			}
		}
		final Handler handler = new Handler() {
			public void handleMessage(Message message) {
				infoCallback.infoLoaded((Integer) message.obj, url);
			}
		};
		new Thread() {
			@Override
			public void run() {
				try {
					available.acquire();
					Integer time = null;
					if (timeCache.containsKey(url)) {
						SoftReference<Integer> softReference = timeCache
								.get(url);
						time = softReference.get();
					}
					if (time == null) {
						time = loadTimeFromUrl(url);
						timeCache.put(url, new SoftReference<Integer>(
								time));
					}
					Message message = handler.obtainMessage(0, time);
					handler.sendMessage(message);
					available.release();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}.start();
		return null;
	}

	public Integer loadTimeFromUrl(String url) {
		//url = "/tmp/usbmounts/sda1/test.mp4";
		Integer time =null;
		/*if(mp!=null)
		{
			mp.stop();
			mp.release();
			mp = null;
		}*/
		MediaPlayer mp = new MediaPlayer();
		Map<String, String> config;
		config = new HashMap<String, String>();
		config.put("FLOWTYPE", "PLAYBACK_TYPE_AUDIO_ONLY");
		try {
			Method method = mp.getClass().getMethod("setDataSource",
					String.class, Map.class);
			method.invoke(mp, url, config);
			//mp.setDataSource(url);
			mp.prepare();
			time = mp.getDuration();
		} catch (Exception e) {
			e.printStackTrace();
		}
		mp.stop();
		mp.release();
		mp = null;
		return time;
	}

	public interface TimeInfoCallback {
		public void infoLoaded(Integer time, String url);
	}

}
