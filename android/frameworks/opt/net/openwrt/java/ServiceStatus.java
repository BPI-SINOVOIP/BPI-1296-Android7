package com.rtk.net.openwrt;

//import java.io.IOException;
import android.util.Log;
//import com.jayway.jsonpath.Configuration;
import com.jayway.jsonpath.JsonPath;
import com.jayway.jsonpath.PathNotFoundException;
//import com.jayway.jsonpath.spi.json.JsonProvider;
import com.rtk.net.openwrt.UbusRpc;

public abstract class ServiceStatus {
	public boolean isRunning = false;
	public boolean isEnabled;
	public boolean isError = false;
	public String reason = null;
    private static final String TAG = "ServiceStatus";
    //static JsonProvider sJsonPath = Configuration.defaultConfiguration().jsonProvider();

	void parse(String inputString){
        if (UbusRpc.hasUbusError(inputString)) {
            isError = true;
            return;
        }

		try {
            //Object doc = sJsonPath.parse(inputString);
            String ret = JsonPath.read(inputString, "$.value");

            if ("1".equals(ret)) {
                isRunning = true;
            } else {
                isRunning = false;
            }

			processCustomTag(inputString);

        } catch (PathNotFoundException e) {
            isError = true;
            e.printStackTrace();
        }
	}

	abstract void processCustomTag(String jsonString);
}
