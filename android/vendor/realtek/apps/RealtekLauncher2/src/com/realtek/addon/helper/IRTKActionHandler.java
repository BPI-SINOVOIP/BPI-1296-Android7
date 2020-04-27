package com.realtek.addon.helper;



public interface IRTKActionHandler {
	
	public static final int RTK_INVALID_ACTION_ID=-1;
	public static final int RTK_ADD_FAVORITE=1;
	public static final int RTK_ADD_VIDEO=2;
	public static final int RTK_REMOVE_FAVORITE=3;
	public static final int RTK_REMOVE_VIDEO=4;
	
	/**
	 * handle action defined in realtek launcher<p>
	 * actionID list:<br>
	 * - RTK_ADD_FAVORITE: add to favorite<br>
	 * - RTK_ADD_VIDEO: add to video<br>
	 * - RTK_REMOVE_FAVORITE: delete favorite<br>
	 * - RTK_REMOVE_VIDEO: delete video<br>
	 * 
	 * @param payload : payload of each actionID
	 */
	public void handleRTKLauncherAction(int actionID, Object paylod);

}
