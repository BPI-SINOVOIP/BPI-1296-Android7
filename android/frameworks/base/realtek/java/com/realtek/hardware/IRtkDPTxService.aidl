package com.realtek.hardware;

interface IRtkDPTxService
{
	/* Get DPTx status */
	boolean checkifDPTxPlugged();
	boolean checkIfDPTxEDIDReady();

	int setTVSystem(int inputTvSystem);
	int getTVSystem();
}
