package com.rtk.net.openwrt;

public class SambaStatus extends ServiceStatus {
	//String directory;
	//String workgroup;

	@Override
	void processCustomTag(String jsonString) {
    }
        /*
	void processCustomTag(String tagName, String text) {
		if(tagName.equals("directory")){
			directory = text;
		} else if(tagName.equals("workgroup")){
			workgroup = text;
		}
	}*/

	@Override
	public String toString() {
        /*
		return "SambaStatus [directory=" + directory + ", workgroup="
				+ workgroup + ", isRunning=" + isRunning + ", isEnabled="
				+ isEnabled + "]";
                */
		return "SambaStatus [isRunning= " + isRunning + "]";
	}
}
