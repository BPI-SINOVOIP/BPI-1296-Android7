package com.realtek.addon.dialog;

import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnKeyListener;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.view.WindowManager;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.PopupWindow;

import com.android.launcher2.ApplicationInfo;
import com.android.launcher2.AppsCustomizePagedView;
import com.android.launcher2.AppsCustomizePagedView.ContentType;
import com.android.launcher2.AppsCustomizeTabHost;
import com.android.launcher2.Launcher;
import com.android.launcher2.PendingAddShortcutInfo;
import com.android.launcher2.PendingAddWidgetInfo;
import com.realtek.addon.dialog.RTKPopupWindowBaseAdapter.ViewHolder;
import com.realtek.addon.exceptions.NoFocusViewException;
import com.realtek.addon.exceptions.ViewTagErrorException;
import com.realtek.addon.helper.ConstantsDef;
import com.realtek.addon.helper.DebugHelper;
import com.realtek.addon.helper.IRTKActionHandler;
import com.realtek.launcher.R;

/**
 * Helper class to manage option menu shows in workspace
 * 
 * @author bruce_huang
 * 
 */

enum OPTION_TAG{
	move,
	remove,
	addFavorite,
	removeFavorite,
	addVideo,
	removeVideo,
	addWorkspace,
	uninstall
}

public class RTKPopupWindowHelper implements OnKeyListener, OnClickListener{
	
	public static final int POPUP_MODE_WORKSPACE=1;
	public static final int POPUP_MODE_DRAWER=2;
	
	// workspace option menu item tags
	//public static final String WTAG_MOVE="move";
	//public static final String WTAG_REMOVE="remove";
	
	// app drawer option menu item tags
	//public static final String DTAG_ADD_FAVORITE="addFavorite";
	//public static final String DTAG_ADD_VIDEO="addVideo";
	//public static final String DTAG_ADD_WORKSPACE="addWorkspace";
	//public static final String DTAG_UNINSTALL="uninstall";
	
	private Launcher mLauncher;
	public PopupWindow mPopupWindow;
	public int mMode;
	public int mWidth;
	public int mHeight;
	//public int mItemLeftPadding;
	public ListView mListView;
	public View mFocusedView;
	
	public RTKPopupWindowItemModel[] mContent=null;
	RTKPopupWindowBaseAdapter mAdapter=null;
	public int mFocusIndex;
	
	/**
	 * Constructor RTKWorkspacePopupWindowHelper
	 * 
	 * @param l instance of {@link com.android.launcher2.Launcher}
	 * @param m mode
	 * @param focusView focused view to be processed
	 */
	public RTKPopupWindowHelper(Launcher l, int m, View focusView) {
		mLauncher = l;
		mMode=m;
		mFocusedView=focusView;
		mFocusIndex=0;
		
		//BitmapDrawable lbgDrawable = (BitmapDrawable) mLauncher.getResources().getDrawable(R.drawable.bg_option_loption);
		//BitmapDrawable rbgDrawable = (BitmapDrawable) mLauncher.getResources().getDrawable(R.drawable.bg_option_roption);
		
		int widthL = mLauncher.getResources().getDimensionPixelSize(R.dimen.option_menu_l_width);
		int widthR = mLauncher.getResources().getDimensionPixelSize(R.dimen.option_menu_r_width);
		
		mWidth = widthL+widthR;
		mHeight = mLauncher.getResources().getDimensionPixelSize(R.dimen.option_menu_height);
		//mItemLeftPadding=widthL;
		createPopupWindow();
	}
	
	/**
	 * create adapter model of current popup window.
	 * @return
	 */
	public BaseAdapter createAdapter() {
		mAdapter = new RTKPopupWindowBaseAdapter(
				mLauncher,
				mMode,
				mContent,
				this);
		
		return mAdapter;
	}
	
	/**
	 * API to convert {@link OPTION_TAG} into multi-language string<br>
	 * @param opt
	 * @return multi-language string
	 */
	public String getOptTagString(OPTION_TAG opt){
		switch(opt){
		case move:
			return mLauncher.getResources().getString(R.string.option_move_workspace_item);
		case remove:
			return mLauncher.getResources().getString(R.string.option_remove_workspace_item);
		case addFavorite:
			return mLauncher.getResources().getString(R.string.option_add_to_favorite_item);
		case removeFavorite:
			return mLauncher.getResources().getString(R.string.option_remove_from_favorite_item);
		case addVideo:
			return mLauncher.getResources().getString(R.string.option_add_to_video_item);
		case removeVideo:
			return mLauncher.getResources().getString(R.string.option_remove_from_video_item);
		case addWorkspace:
			return mLauncher.getResources().getString(R.string.option_add_to_wallpaper_item);
		case uninstall:
			return mLauncher.getResources().getString(R.string.option_uninstall_item);
		default:
			return null;
		}
	}
	
	/**
	 * Sub routine for {@link #createContent()}<br>
	 * used to create Content in AppCustomPage mode.<br>
	 * - 
	 */
	public void createDrawerContent(){
		String tab=mLauncher.getCurrentDrawerTabTag();
		if(tab==AppsCustomizeTabHost.APPS_TAB_TAG){
			// 1. add to favorite
			// 2. add to video
			// 3. add to wallpaper
			// 4. uninstall
			mContent=new RTKPopupWindowItemModel[4];
			mContent[0]=new RTKPopupWindowItemModel(
					getOptTagString(OPTION_TAG.addFavorite),
					OPTION_TAG.addFavorite,
					drawerItemEnable(OPTION_TAG.addFavorite));
			mContent[1]=new RTKPopupWindowItemModel(
					getOptTagString(OPTION_TAG.addVideo),
					OPTION_TAG.addVideo,
					drawerItemEnable(OPTION_TAG.addVideo));
			mContent[2]=new RTKPopupWindowItemModel(
					getOptTagString(OPTION_TAG.addWorkspace),
					OPTION_TAG.addWorkspace,
					drawerItemEnable(OPTION_TAG.addWorkspace));
			mContent[3]=new RTKPopupWindowItemModel(
					getOptTagString(OPTION_TAG.uninstall),
					OPTION_TAG.uninstall,
					drawerItemEnable(OPTION_TAG.uninstall));
			
		}else if(tab==AppsCustomizeTabHost.FAVORITE_TAB_TAG){
			// 1. add to wallpaper
			// 2. remove from favorite
			// 2. uninstall
			mContent=new RTKPopupWindowItemModel[3];
			mContent[0]=new RTKPopupWindowItemModel(
					getOptTagString(OPTION_TAG.addWorkspace),
					OPTION_TAG.addWorkspace,
					drawerItemEnable(OPTION_TAG.addWorkspace));
			mContent[1]=new RTKPopupWindowItemModel(
					getOptTagString(OPTION_TAG.removeFavorite),
					OPTION_TAG.removeFavorite,
					drawerItemEnable(OPTION_TAG.removeFavorite));
			mContent[2]=new RTKPopupWindowItemModel(
					getOptTagString(OPTION_TAG.uninstall),
					OPTION_TAG.uninstall,
					drawerItemEnable(OPTION_TAG.uninstall));
		}else if(tab==AppsCustomizeTabHost.VIDEO_TAB_TAG){
			// 1. add to wallpaper
			// 2. remove from video
			// 2. uninstall
			mContent=new RTKPopupWindowItemModel[3];
			mContent[0]=new RTKPopupWindowItemModel(
					getOptTagString(OPTION_TAG.addWorkspace),
					OPTION_TAG.addWorkspace,
					drawerItemEnable(OPTION_TAG.addWorkspace));
			mContent[1]=new RTKPopupWindowItemModel(
					getOptTagString(OPTION_TAG.removeVideo),
					OPTION_TAG.removeVideo,
					drawerItemEnable(OPTION_TAG.removeVideo));
			mContent[2]=new RTKPopupWindowItemModel(
					getOptTagString(OPTION_TAG.uninstall),
					OPTION_TAG.uninstall,
					drawerItemEnable(OPTION_TAG.uninstall));
		}else{
			// should be app widget page
			// 1. add to wallpaper
			mContent=new RTKPopupWindowItemModel[1];
			mContent[0]=new RTKPopupWindowItemModel(
					getOptTagString(OPTION_TAG.addWorkspace),
					OPTION_TAG.addWorkspace,
					drawerItemEnable(OPTION_TAG.addWorkspace));
		}
	}
	
	/**
	 * Based on current status of Launcher to create popup window content
	 * @return content
	 */
	public void createContent(){
		if(mMode==POPUP_MODE_WORKSPACE){
			// workspace
			mContent=new RTKPopupWindowItemModel[2];
			mContent[0]=new RTKPopupWindowItemModel(
					//mLauncher.getResources().getString(R.string.option_move_workspace_item),
					getOptTagString(OPTION_TAG.move),
					OPTION_TAG.move,
					workspaceItemEnable(OPTION_TAG.move));
			mContent[1]=new RTKPopupWindowItemModel(
					//mLauncher.getResources().getString(R.string.option_remove_workspace_item),
					getOptTagString(OPTION_TAG.remove),
					OPTION_TAG.remove,
					workspaceItemEnable(OPTION_TAG.remove));
		}else{
			// API to create option menu content under app drawer
			createDrawerContent();
		}
	}
	
	/**
	 * Find first enabled item index 
	 * @return 
	 */
	private int findFirstEnabledItemIndex(){
		if(mContent!=null){
			for(int i=0;i<mContent.length;i++){
				if(mContent[i].bIsEnable)
					return i;
			}
		}
		return 0;
	}
	
	public void showsPopup(ViewGroup parent){
		createPopupWindow();
		// save focused view, could be
		// 1. workspace shortcut or widget host view
		// 2. app drawer icon
		// 3. widget drawer icon
		
		mFocusIndex=findFirstEnabledItemIndex();
		mListView.setSelection(mFocusIndex);
		mPopupWindow.showAtLocation(parent, Gravity.RIGHT|Gravity.TOP, 0, 0);
	}
	
	public void createPopupWindow() {
		
		// create mContent,
		// one should call this API here!!
		createContent();
		
		ColorDrawable cd = new ColorDrawable(Color.TRANSPARENT);
		// initialize a pop up window type
		mPopupWindow = new PopupWindow(mLauncher);
		// some other visual settings
		mPopupWindow.setFocusable(true);
		mPopupWindow.setWidth(mWidth);
		mPopupWindow.setHeight(WindowManager.LayoutParams.MATCH_PARENT);
		mPopupWindow.setBackgroundDrawable(cd);
		
		LayoutInflater inflater = (LayoutInflater) mLauncher.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		View popupView=inflater.inflate(R.layout.rtk_popup_type_1,null);
		mListView = (ListView) popupView.findViewById(R.id.option_list);
		
		// set our adapter and pass our pop up window contents
		mListView.setAdapter(createAdapter());

		// set the item click listener
		// RTKTODO: set click listener and
		// listViewDogs.setOnItemClickListener(new
		// DogsDropdownOnItemClickListener());
		
		// RTKCOMMENT: register key-event listener
		mListView.setOnKeyListener(this);
		// set the list view as pop up window content
		mPopupWindow.setContentView(popupView);
		mPopupWindow.setAnimationStyle(R.style.PopupAnimation);

		//return mPopupWindow;
	}
	
	public int findNextEnabledItem(int keyCode){
		int newFocusIndex=mFocusIndex;
		boolean bItemFound=false;
		int step=0;
		int length = mContent.length;
		
		if(keyCode==KeyEvent.KEYCODE_DPAD_UP){
			step=-1;
		}else{
			step=1;
		}
		// start calculate
		do{
			newFocusIndex=newFocusIndex+step;
			if(newFocusIndex>=0 && newFocusIndex<length){
				bItemFound=(mContent[newFocusIndex].bIsEnable)?true:false;
			}
		}while(newFocusIndex>=0 && newFocusIndex<length && !bItemFound);
		
		if(bItemFound && newFocusIndex>=0 && newFocusIndex<length){
			return newFocusIndex;
		}else{
			// did not find new focus item, return original one.
			return mFocusIndex;
		}
	}
	
	/**
     * RTK Launcher helper class to help pop-up window to handle key-event
     * @param v
     * @param keyCode
     * @param e
     * @param win
     * @return
     */
    public boolean handlePopupWindowKeyEvent(View v, int keyCode, KeyEvent e){
    	
    	final int action = e.getAction();
    	final boolean handleKeyEvent = (action != KeyEvent.ACTION_UP);
    	boolean wasHandled = false;
    	
    	if(handleKeyEvent){
    		
    		DebugHelper.dump("handlePopupWindowKeyEvent...");
    		
    		// When Option menu shows up with no selected item, but user press ENTER.
    		if(keyCode==KeyEvent.KEYCODE_ENTER||keyCode==KeyEvent.KEYCODE_DPAD_CENTER){
    		    ListView lv = (ListView)v;
    		    if(lv.getSelectedView()==null)
    		        keyCode=KeyEvent.KEYCODE_DPAD_DOWN;
    		}
    		
    		switch(keyCode){
    		case KeyEvent.KEYCODE_DPAD_UP:
    		case KeyEvent.KEYCODE_DPAD_DOWN:
    			mFocusIndex = findNextEnabledItem(keyCode);
    			mListView.setSelection(mFocusIndex);
    			wasHandled=true;
    			break;
    		case KeyEvent.KEYCODE_DPAD_LEFT:
    		case KeyEvent.KEYCODE_DPAD_RIGHT:
    		    // consume Left,Right key
    		    wasHandled=true;
                break;
    		case ConstantsDef.LAUNCHER_KEY_OPTION_MENU: //KeyEvent.KEYCODE_M:
    			if(mPopupWindow!=null){
    				mPopupWindow.dismiss();
    			}
    			wasHandled=true;
    			break;
    		case KeyEvent.KEYCODE_ENTER:
    		case KeyEvent.KEYCODE_DPAD_CENTER:
    			ListView lv = (ListView)v;
    			View sv = lv.getSelectedView();
    			if(sv==null){
    			    // error handling, but should not reach here.
    			    wasHandled=true;
    			    break;
    			}
    			Object tag = lv.getSelectedView().getTag();
    			ViewHolder vh = (ViewHolder)tag;
    			//DebugHelper.dump("handlePopupWindowKeyEvent getTag:"+lv.getSelectedView().getTag());
    			if(mPopupWindow!=null){
    				mPopupWindow.dismiss();
    			}
    			
    			// prevent system from executing disabled option item
    			if(vh.isEnabled)
    				processCommand(vh.opt);
    			else
    				DebugHelper.dump("unable to execute command");
    			
    			wasHandled=true;
    			break;
    		}
    		
    	}
    	
    	return wasHandled;
    }
	
    public int optTagToRTKHandlerActionID(OPTION_TAG opt_tag){
    	switch(opt_tag){
    	case addFavorite:
    		return IRTKActionHandler.RTK_ADD_FAVORITE;
    	case addVideo:
    		return IRTKActionHandler.RTK_ADD_VIDEO;
    	case removeFavorite:
    		return IRTKActionHandler.RTK_REMOVE_FAVORITE;
    	case removeVideo:
    		return IRTKActionHandler.RTK_REMOVE_VIDEO;
    	default:
    		return IRTKActionHandler.RTK_INVALID_ACTION_ID;
    	}
    }
    
    public void processCommand(OPTION_TAG tag){
    	// check if focus view is null or not first
    	if(mFocusedView==null){
    		try {
				throw new NoFocusViewException();
			} catch (NoFocusViewException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return;
			}
    	}
    	
    	Object vtag = mFocusedView.getTag();
    	switch(tag){
    	// workspace type command:
    	case move:
    		mLauncher.handleWorkspaceMoveCommand();
    		break;
    	case remove:
    		mLauncher.handleWorkspaceRemoveCommand(mFocusedView);
    		break;
    	// app drawer type command:
    	case addFavorite:
    	case addVideo:
    	case removeVideo:
    	case removeFavorite:
    		if(vtag instanceof ApplicationInfo){
    			ViewParent vp=mFocusedView.getParent().getParent().getParent();
    			
    			if(vp instanceof IRTKActionHandler){
    				int actionID=optTagToRTKHandlerActionID(tag);
    				IRTKActionHandler handler = (IRTKActionHandler)vp;
    				handler.handleRTKLauncherAction(actionID, vtag);
    			}
    		}
    		break;
    	/*
    	case addVideo:
    		if(vtag instanceof ApplicationInfo){
    			ViewParent vp=mFocusedView.getParent().getParent().getParent();
    			if(vp instanceof IRTKActionHandler){
    				IRTKActionHandler handler = (IRTKActionHandler)vp;
    				handler.handleRTKLauncherAction(IRTKActionHandler.RTK_ADD_VIDEO, vtag);
    			}
    		}
    		break;
    	*/
    	case addWorkspace:
    		// RTKTODO: system could also add a widget into workspace.
    		//DebugHelper.dump("RTKPopupWindowHelper addWorkspace vtag:"+vtag);
    		//DebugHelper.dump("break");
    		//boolean bAddWorkspaceResult=false;
    		
    		if(vtag instanceof ApplicationInfo){
    			
    			ApplicationInfo appInfo = (ApplicationInfo)vtag;
    			mLauncher.completeAddApplicationToWorkspaceFromOptionMenu(appInfo.intent);
    			
    		}else if(vtag instanceof PendingAddWidgetInfo){
    			
    			PendingAddWidgetInfo wInfo=(PendingAddWidgetInfo)vtag;
    			wInfo.bSwitchToMoveModeAfterAdd=true;
    			int[] span=new int[2];
    			wInfo.fatchSpanInfo(span);
    			mLauncher.addAppWidgetFromOptionMenu(wInfo, span);
    			
    		}else if(vtag instanceof PendingAddShortcutInfo){
    			
    			PendingAddShortcutInfo sInfo = (PendingAddShortcutInfo)vtag;
    			int[] span=new int[2];
    			sInfo.fatchSpanInfo(span);
    			mLauncher.processShortcutFromOptionMenu(sInfo.getComponentName(),span);
    			
    		}
    		
    		/*
    		if(bAddWorkspaceResult){
    			//DebugHelper.dump("New added view info:"+mLauncher.mLatestAddedViewByOptionMenu);
    			// set a flag that would be checked after switch back to workspace mode
    			mLauncher.bAddNewChildViaOptionMenu=true;
    		}else{
    			DebugHelper.dump("[RTKPopupWindowHelper] add view to workspace failed.");
    		}
    		*/
    		
    		break;
    	case uninstall:
    		if(vtag instanceof ApplicationInfo){
    			ApplicationInfo appInfo = (ApplicationInfo)vtag;
    			mLauncher.completeUninstallApplication(appInfo);
    		}
    		break;
    	default:
    		DebugHelper.dump("Process command, un-rec TAG, do nothing");
    		break;
    	}
    }
	
	@Override
	public boolean onKey(View v, int keyCode, KeyEvent event) {
		return handlePopupWindowKeyEvent(v, keyCode, event);
	}
	
	// callback to check if item enabled in workspace option.
	public boolean workspaceItemEnable(OPTION_TAG opt){
		if(opt==OPTION_TAG.remove){
			// check if focused item is app widget
			// View v=mLauncher.getWorkspaceFocusView();
			if(mFocusedView!=null){
				return mLauncher.isRemovableView(mFocusedView);
			}
		}
		return true;
	}
	
	private String getTagInfoPackageName(Object tag){
		if(tag instanceof ApplicationInfo){
			return ((ApplicationInfo)tag).packageName;
		}else{
			return null;
		}
	}
	
	// callback to check if item enabled in app drawer option.
	public boolean drawerItemEnable(OPTION_TAG opt){
		
		// if no item is focused, always return false.
		if(mFocusedView==null)
			return false;
		
		Object focuseViewTag=mFocusedView.getTag();
		DebugHelper.dump("[RTKPopupWindowHelper] drawerItemEnable, focusViewTag:"+focuseViewTag+" class:"+focuseViewTag.getClass().getName());
		
		if( focuseViewTag instanceof ApplicationInfo || 
			focuseViewTag instanceof PendingAddWidgetInfo ||
			focuseViewTag instanceof PendingAddShortcutInfo) {
			
			String packageName = getTagInfoPackageName(focuseViewTag);
			ViewParent vp=mFocusedView.getParent().getParent().getParent();
			AppsCustomizePagedView pv=null;
			
			if(vp instanceof AppsCustomizePagedView){
				pv = (AppsCustomizePagedView)vp;
			}
			
			switch(opt){
			case addFavorite:
				if( focuseViewTag instanceof ApplicationInfo && 
					packageName!=null && 
					pv.checkAppExistance(ContentType.Favorite, packageName)!=true){
					return true;
				}else{
				    return false;
				}
				//break;
			case addVideo:
				if( focuseViewTag instanceof ApplicationInfo && 
					packageName!=null && 
					pv.checkAppExistance(ContentType.Video, packageName)!=true){
					return true;
				}else{
				    return false;
				}
				//break;
			case uninstall:
				// Disable if focus is system app.
				if(focuseViewTag instanceof ApplicationInfo){
					ApplicationInfo appInfo=(ApplicationInfo)focuseViewTag;
					return mLauncher.isRemovableApp(appInfo);
				}
				
				// focusView should not be Widget, if does, throws Exception
				if(focuseViewTag instanceof PendingAddWidgetInfo){
					try {
						throw new ViewTagErrorException();
					} catch (ViewTagErrorException e) {
						e.printStackTrace();
						return false;
					}
				}
				break;
			case removeFavorite:
				// Disable if there is no item exists in FAVORITE page
				if(pv==null || pv.getFavoriteAppCount()<=0)
					return false;
				break;
			case removeVideo:
				// Disable if there is no item exists in VIDEO page
				if(pv==null || pv.getVideoAppCount()<=0)
					return false;
				break;
			case addWorkspace:
				// should always be enabled
			default:
				return true;
			}
			
			/*
			if(opt==OPTION_TAG.addFavorite){
				if(focuseViewTag instanceof PendingAddWidgetInfo)
					return false;
			}else if(opt==OPTION_TAG.addVideo){
				if(focuseViewTag instanceof PendingAddWidgetInfo)
					return false;
			}else if(opt==OPTION_TAG.addWorkspace){
				return true;
			}else if(opt==OPTION_TAG.uninstall){
				if(focuseViewTag instanceof PendingAddWidgetInfo)
					return false;
				if(focuseViewTag instanceof ApplicationInfo){
					ApplicationInfo appInfo=(ApplicationInfo)focuseViewTag;
					return mLauncher.isRemovableApp(appInfo);
				}
			}
			return true;
			*/
		}else{
			try {
				throw new ViewTagErrorException();
			} catch (ViewTagErrorException e) {
				e.printStackTrace();
			}
		}
		
		// system always tends to enable option item as default
		return true;
	}

    @Override
    public void onClick(View v) {
        ViewHolder holder = (ViewHolder)v.getTag();
        
        if(!holder.isEnabled)
            return;
        
        DebugHelper.dumpSQA("clicked view:"+holder.textView.getText());
        processCommand(holder.opt);
        if(mPopupWindow!=null){
            mPopupWindow.dismiss();
        }
    }

    public void forceDismissPopup(){
        Log.i("RTKPopupWindowHelper","forceDismissPopup");
        if(mPopupWindow!=null){
            mPopupWindow.dismiss();
        }
    }
}
