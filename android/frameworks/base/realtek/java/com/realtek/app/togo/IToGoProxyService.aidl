package com.realtek.app.togo;

import android.os.IBinder;

interface IToGoProxyService {
	int bindToGoAppService();
	IBinder getToGoAppService();
	IBinder getGraphicBufferProducer(int handle, int type, int width, int height);
	void clearGraphicBufferProducer(int handle);
}
