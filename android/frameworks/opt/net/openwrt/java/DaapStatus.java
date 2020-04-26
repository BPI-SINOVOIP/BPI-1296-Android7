package com.rtk.net.openwrt;

public class DaapStatus extends ServiceStatus {
	public String servername;
	public int port;

	@Override
	void processCustomTag(String text) {
        /*
		if(tagName.equals("servername")){
			servername = text;
		} else if(tagName.equals("port")) {
			port = Integer.parseInt(text);
		}*/

	}
	@Override
	public String toString() {
		return "DaapStatus [servername=" + servername + ", port=" + port
				+ ", isRunning=" + isRunning + ", isEnabled=" + isEnabled + "]";
	}
}
