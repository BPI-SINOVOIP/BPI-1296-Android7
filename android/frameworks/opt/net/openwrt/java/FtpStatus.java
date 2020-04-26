package com.rtk.net.openwrt;

public class FtpStatus extends ServiceStatus{
	public int maxPerIP;
	public int maxClients;
	public boolean enableAnonymous;
	public String directory;
	public int port;
	@Override
	void processCustomTag(String text) {
        /*
		if(tagName.equals("maxperip")){
			maxPerIP = Integer.parseInt(text);
		} else if(tagName.equals("maxclients")) {
			maxClients = Integer.parseInt(text);
		} else if(tagName.equals("anonymous_enable")) {
			enableAnonymous = "yes".equals(text);
		} else if(tagName.equals("directory")) {
			directory = text;
		} else if(tagName.equals("port")) {
			port = Integer.parseInt(text);
		}*/

	}
	@Override
	public String toString() {
		return "FtpStatus [maxPerIP=" + maxPerIP + ", maxClients=" + maxClients
				+ ", enableAnonymous=" + enableAnonymous + ", directory="
				+ directory + ", port=" + port + ", isRunning=" + isRunning
				+ ", isEnabled=" + isEnabled + "]";
	}

}
