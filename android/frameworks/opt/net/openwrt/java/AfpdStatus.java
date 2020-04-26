package com.rtk.net.openwrt;

public class AfpdStatus extends ServiceStatus {

	@Override
	void processCustomTag(String text) {
		// no custome tags
	}

	@Override
	public String toString() {
		return "AfpdStatus [isRunning=" + isRunning + ", isEnabled="
				+ isEnabled + "]";
	}

}
