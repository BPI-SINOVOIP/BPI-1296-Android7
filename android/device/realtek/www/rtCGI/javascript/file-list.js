var DEBUG_FLAG = false;

//====================================================================================
//=     Global Variables
//====================================================================================
var g_isAndroidAirFun = isAndroidAirFun();
var g_isIOSAirFun = isiOSAirFun();
var g_itemBeTapped = false;

// for detect click event
var g_touchEvtStart = "touchstart";
var g_touchEvtEnd = "touchend";
var g_touchEvtMove = "touchmove";

//==== Layout ====
var TITLE_BAR_HEIGHT_DEFAULT = 44; // px

//==== AJAX Setup ====
// ajax request
var AJAXREQ_CGIURL = "../rtCGI.fcgi";
var AJAXREQ_TIMEOUT = 15000;

// enum for AJAX response type
var AJAXREP_FILELIST = "filelist";
var AJAXREP_MUSICLIST = "musiclist";
var AJAXREP_ALBUMLIST = "albumlist";
var AJAXREP_ARTISTLIST = "artistlist";
var AJAXREP_GENRELIST = "genrelist";
var AJAXREP_PHOTOLIST = "photolist";
var AJAXREP_VIDEOLIST = "videolist";
var AJAXREP_VIEWCONTENT = "viewcontent";

var AJAXREP_QUERYSTORAGE = "storageStatus";

var AJAXREP_RET_ERR = "error";

// ajax command response
var AJAXREP_VIEW_RET_OK = "PLAYED";
var AJAXREP_VIEW_RET_FAILED = "FAIL";

//==== File List ====
//---- layout ----
var HTML_DIV_TITLE = "div#titleBar h1:first";
var HTML_BTN_UPLEVEL = "div#titleBar a#levelUpBtn";
var HTML_ELEMENT_LIST = "ul#listSelector";
var CSSCLS_LISTITEM = "dspLabel";
var CSSCLS_LISTITEM_CONTAINER = "arrow";
var CSSCLS_LISTITEM_EMPTY = "itemTextEmpty";
var CSSCLS_ITEMTEXT = "listItemText";
var CSSCLS_ITEMTEXT_MORE = "itemTextMore";
var g_layoutElemList;
var g_layoutBtnUpLevel;
var g_layoutDivTitle;

var g_FileListListArea;

//---- resources: image ----
var FILELIST_ICON_DIR = "./images/filelist/";
var FILELIST_ICON_THUMB_USB = "icon-usb.png";
var FILELIST_ICON_THUMB_CARD = "icon-card.png";
var FILELIST_ICON_THUMB_HDD = "icon-hdd.png";
var FILELIST_ICON_THUMB_DIR = "icon-folder.png";
var FILELIST_ICON_THUMB_PIC = "icon-photo.png";
var FILELIST_ICON_THUMB_AUD = "icon-audio.png";
var FILELIST_ICON_THUMB_VID = "icon-video.png";

var FILELIST_ICON_DEFAULT = FILELIST_ICON_THUMB_DIR;

//---- resources: message string ----
var STORAGE_CORRUPT = "folder corrupt. please check your device.";
var FOLDER_EMPTY = "folder is empty";
var LIST_EMPTY = "Empty";
var TITLE_HOME = "Home";
var TITLE_PHOTOLIST = "Photo";
var TITLE_MUSICLIST = "Music";
var TITLE_VIDEOLIST = "Video";
//---- constant ----
// to setup a limit of media contents for querying result
var FILELIST_QUERYCOUNTLIMIT = 20;

//---- enum type ----
// item type
var FILELIST_ID_ROOT = "root";
var PHOTOLIST_ID = "photo";
var MUSICLIST_ID = "music";
var VIDEOLIST_ID = "video";

var FILELIST_TYPE_SOURCE = "source"; // for File entry in Home screen
var FILELIST_TYPE_MEDIA = "media";	// for Photo/Music/Video entry in Home screen
var FILELIST_TYPE_FOLDER = "folder";
var FILELIST_TYPE_VIDEO = "video";
var FILELIST_TYPE_MUSIC = "music";
var FILELIST_TYPE_PHOTO = "photo";
var FILELIST_TYPE_HDD = "hdd";
var FILELIST_TYPE_USB = "usb";
var FILELIST_TYPE_CARD = "card";
var FILELIST_TYPE_MORE = "more"

// ---- global values ----
var g_ParentFolderId = "";
var g_CurrentFolderId = "";
var g_itemListAjaxCallLock = false;


//==== Remote Storage ====
//---- constant ----
//Time interval for checking the storage's plug-status of remote device
var REMOTESTORAGE_CHECKINTERVAL = 2000;
var REMOTESTORAGE_CHECKERRWAITING = 3000;

//---- enum type ----
// storage type enum
var STORAGE_ID_UNSUPPORTED = -1;
var STORAGE_ID_USB = 0;
var STORAGE_ID_CARD = 1;
var STORAGE_ID_HDD = 2;
var STORAGE_ID_SMB = 3;
var STORAGE_ID_MAX = 4;
// storage type name
var STORAGE_NAME_USB = "usb";
var STORAGE_NAME_CARD = "card";
var STORAGE_NAME_SMB = "smb";
var STORAGE_NAME_HDD = "hdd";
// storage status enum
var STORAGE_STATUS_READY = "ready";
var STORAGE_STATUS_ABSENT = "absent";
var STORAGE_STATUS_UNSUPPORTED = "unsupported";
var STORAGE_STATUS_BUSY = "busy";

//---- global values ----
var g_checkStorageTimer = null; // for remote device's storage status
var g_storagePlugFlag = 0; // bitwise usage, refer to above enum. (0 for storage absent and 1 means plugged)
var g_gotInitStorageFlag = false;
var g_queryStorageLock = false; // lock the query storage operation

//---- debugging ----
var g_testStorageRespParser = false;
var g_keepCheckingStorage = true;
var CHECKSTORAGE_MAX_TIMES = 50; // used when keep_checking if false
var g_checkingStorageCount = CHECKSTORAGE_MAX_TIMES; 


//====================================================================================
//=     Helper functions
//====================================================================================
function isValidString(str) {
	return (str === null ||
		(typeof str === "undefined") ||
		(typeof str.toString === "undefined")) ? false : true;
}

// locks for AJAX command
function setQueryFileListLock(lockFlag) {
	g_itemListAjaxCallLock = (lockFlag === false) ? false : true;
}
function lockQueryFileList() {
	setQueryFileListLock(true);
}
function unlockQueryFileList() {
	setQueryFileListLock(false);
}
function isQueryingFileList() {
	return g_itemListAjaxCallLock ? true : false;
}
function setQueryStorageLock(lockFlag) {
	g_queryStorageLock = (lockFlag === false) ? false : true;
}
function lockQueryStorage() {
	setQueryStorageLock(true);
}
function unlockQueryStorage() {
	setQueryStorageLock(false);
}
function isCheckingStorage() {
	return g_queryStorageLock ? true : false;
}

// timer
function killCheckStorageTimer() {
	if (g_checkStorageTimer != null) {
		clearTimeout(g_checkStorageTimer);
		g_checkStorageTimer = null;
	}
}
function setCheckStorageTimer(extraWaitingTime) {
	if (isCheckingStorage() == false) {
		killCheckStorageTimer();
		var plusTime = 0;
		if (typeof extraWaitingTime === "number") {
			plusTime = parseInt(extraWaitingTime);
		}
		g_checkStorageTimer = setTimeout(doCheckStorage, (REMOTESTORAGE_CHECKINTERVAL + plusTime));

		if (DEBUG_FLAG && g_keepCheckingStorage) {
			g_checkingStorageCount++;
		}
	}
}
function setupNextRoundQueryingStorage() {
	if (g_keepCheckingStorage) {
		setCheckStorageTimer();
	} else if (g_checkingStorageCount > 0) {
		g_checkingStorageCount--;
		if (g_checkingStorageCount > 0) {
			setCheckStorageTimer();
		}
	}
}
//====================================================================================
//=     HTML document
//====================================================================================
// document ready
$(document).ready(function() {
	var g_isTouchDevice = isTouchDevice();
	if (!g_isTouchDevice) {
		g_touchEvtStart = "mousedown";
		g_touchEvtEnd = "mouseup";
	} else {
		// sending all touchmove to iScroll lib
		document.addEventListener(g_touchEvtMove, function(e) { e.preventDefault(); });
	}

	// layout initialation
	g_layoutElemList = $(HTML_ELEMENT_LIST);
	g_layoutBtnUpLevel = $(HTML_BTN_UPLEVEL);
	g_layoutDivTitle = $(HTML_DIV_TITLE);

	g_layoutBtnUpLevel.hide();
	g_FileListListArea = new iScroll('fileListArea', { desktopCompatibility: true });
	// adjust list hight
	var headerHeight = parseInt($(".titleSize").css("height"));
	if (headerHeight < TITLE_BAR_HEIGHT_DEFAULT) {
		headerHeight = TITLE_BAR_HEIGHT_DEFAULT;
	}
	var newHeight = $(window).height() - headerHeight;
	$(".listSize").css("height", newHeight);

	// setup AJAX env
	$.ajaxSetup({
		url: AJAXREQ_CGIURL,
		type: "GET",
		contentType: "application/x-www-form-urlencoded; charset=UTF-8",
		dataType: "json",	// data type back from server
		timeout: AJAXREQ_TIMEOUT,
		success: function(data) {
			handleAjaxSuccess(data);
		},
	});

	// init variables
	getFileListById(FILELIST_ID_ROOT, 0);
});

//====================================================================================
//=     Functions
//====================================================================================
//-------- touch handler --------
function touchStart(evt) {
	if (DEBUG_FLAG) {
		console.log("got touch event for:" + evt.target.id);
	}
	g_itemBeTapped = true;
}

// handler for tapping on items in File List
function itemListClick(folderId, startIndex, caller_name, queryAct) {
	if (DEBUG_FLAG) {
		console.log("got item tap for " + unescape(folderId) + "(caller:" + unescape(caller_name) + ") is Valid=" + g_itemBeTapped);
	}

	if (g_itemBeTapped) {
		getFileListById(unescape(folderId), startIndex);

		if (!queryAct) {
			unlockQueryFileList();
		}
		g_itemBeTapped = false;
	}
}

//-------- File List --------
// folder traversing
function isValidFolder(folderId) {
	return ((folderId == null) || (folderId === "")) ? false : true;
}
function isFocusOnRoot() {
	// if focus level or parent level are invalid
	// or, current folder id is root
	if (!isValidFolder(g_ParentFolderId) || (g_CurrentFolderId === FILELIST_ID_ROOT)) {
		return true;
	}
	return false;
}
// traverse level up
function traverseUp() {
	if (!isFocusOnRoot()) {
		getFileListById(g_ParentFolderId, 0);
	}
}

// sending an AJAX with querying folder id in JSON format
function createFileListReqCmd(folderId, startIdx) {
	var cmdStr = '{"request":{"requestType":"itemList"';
	var idStr = (folderId != null) ? folderId : '';
	cmdStr += ',"id":"' + idStr + '"';
	if (startIdx != null && startIdx >= 0) {
		cmdStr += ',"startIndex":"' + startIdx + '"';
		cmdStr += ',"numberOfItems":"' + FILELIST_QUERYCOUNTLIMIT;
	}
	cmdStr += ',"ts":"' + new Date().getTime() + '"}}';
	return cmdStr;
}

function getFileListById(folderId, startIdx) {
	try {
		// to stop the check-stoage timer
		killCheckStorageTimer();

		// sending AJAX
		if (!isQueryingFileList()) {
			lockQueryFileList();
			$.ajax({
				complete: function(jqXHR, textStatus) {
						handleFileListAjaxComplete(jqXHR, textStatus);
				},
				data: {id:3, url:folderId}, //createFileListReqCmd(folderId, startIdx),
				error: handleFileListAjaxError
			});
		}
	} catch (err) {
		console.log("error in getFileListById, err=" + err + " for id=" + folderId);
		return;
	}
}


//-------- Storage Status --------
// is the client webview is suitable for querying storage status at the moment
function isReadyToSendQueryStorage() {
	// set default to false to avoid noise
	var bAllowed = false;
	if (g_isAndroidAirFun) {
		if ((typeof window.myAndroidFunc === "object") &&
			(typeof window.myAndroidFunc.isInStorageFunction === "function")) {
			// asking Android AirFun if it is ready for querying box's storage
			var ret = window.myAndroidFunc.isInStorageFunction();
			if (DEBUG_FLAG) {
				console.log("AirFun.inStorage()? " + ret);
			}
			if (typeof ret === "string") {
				bAllowed = ret === "true" ? true : false;
			} else {
				bAllowed = false;
			}
		} else {
			console.log("Android AirFun didn't contains JS_inf");
		}
	}
	else if (g_isIOSAirFun) {
		window.location.href = "nativecmd://ios/request=storage";
		if (typeof window.isInStorageFunction === "function") {
			var ret = window.isInStorageFunction();
			if (typeof ret === "string") {
				bAllowed = ret === "true" ? true : false;
			} else {
				bAllowed = false;
			}
		}
	}
	return bAllowed;
}

// sending an AJAX to get box's storage status in JSON format
function createQueryStorageReqCmd() {
	var request = '{"request":{"requestType":"queryStorage"';
	// if (queryStorage == "STORAGE_ALL") {
	//  request += ',"storage":"all"';
	// } else if (queryStorage == "STORAGE_USB") {
	//  request += ',"storage":"usb"';
	// } else if (queryStorage == "STORAGE_CARD") {}
	request += '}}';
	return request;
}

function doCheckStorage() {
	// setTimeout
	g_checkStorageTimer = null;
	var doChecking = isReadyToSendQueryStorage();
	if (DEBUG_FLAG) {
		console.log("do check storage timer, queryRemote=" + doChecking 
			+ ", counter=" + g_checkingStorageCount);
	}

	if (doChecking === false) {
		// keep checking latter
		setupNextRoundQueryingStorage();
		return;
	}

	// debug
	if (g_testStorageRespParser === true) {
		var testData = '{"responseType":"storageStatus","storageList":[{"sName":"usb1","sStatus":"ready"},{"sName":"card","sStatus":"absent"}]}';
		if (typeof testData === "string") {
			if (testData.charAt(0) !== '(') {
				testData = '(' + testData + ')';
			}
		}
		var respObj = eval(testData);
		handleQueryStorageResponse(respObj);
	} else if (!isQueryingFileList() && !isCheckingStorage()) {
		lockQueryStorage();
		try {
			// request a checking-storage command
			$.ajax({
				data: createQueryStorageReqCmd(),
				error: handleQueryStorageAjaxError,
			});
		} catch (err) {
			console.log("Error on requestAjaxQueryStorage, err=" + err);
		}
	}
}


//====================================================================================
//=     AJAX Response Handler
//====================================================================================
// ajax response in json format
function handleAjaxSuccess(data) {
	var respObj;
	try {
		respObj = eval(data); // to use jQuery.parseJSON() ??
	} catch (err) {
		unlockQueryFileList();
		unlockQueryStorage();
		console.log("Error: Response could not be parsed. Response=" + data + "--Exception=" + err);
		return false;
	}
	if (respObj.d) {
		respObj = respObj.d;
	}

	try {
		// dispatch evt according to the responseType
		if ((respObj.response_type == AJAXREP_FILELIST) ||
			(respObj.response_type == AJAXREP_PHOTOLIST) ||
			(respObj.response_type == AJAXREP_MUSICLIST) ||
			(respObj.response_type == AJAXREP_ALBUMLIST) ||
			(respObj.response_type == AJAXREP_ARTISTLIST) ||
			(respObj.response_type == AJAXREP_GENRELIST) ||
			(respObj.response_type == AJAXREP_VIDEOLIST)) {
			handleFileListResponse(respObj);		
		} else if (respObj.response_type == AJAXREP_VIEWCONTENT){
			handleViewContentResponse(respObj);
		}				
		/*if (respObj.responseType == AJAXREP_FILELIST){
			handleFileListResponse(respObj);
		} else if (respObj.responseType == AJAXREP_QUERYSTORAGE){
			handleQueryStorageResponse(respObj);
		} else if (respObj.responseType == AJAXREP_RET_ERR){
			handleErrorResponse(respObj);
		} */
		else {
			//Reset the lock in case of error, just in case
			unlockQueryFileList();
			unlockQueryStorage();
			console.log("err: unsupported AJAX.rspType=" + data);
		}		
	} catch (err) {
		console.log("AJAX response handler got err=" + err);
		unlockQueryFileList();
		unlockQueryStorage();
		return false;
	}
}

//-------- File List --------
function handleFileListResponse(respObject) {
	// release lock
	unlockQueryFileList();

	// create the list items from the ajax response
	if (respObject.response_type == AJAXREP_FILELIST) {
		//Change title on toolbar
		g_layoutDivTitle.text(respObject.current.disp_name);
	
		if (respObject.parent == null) {
			// parent object is missing
			g_ParentFolderId = "";
			g_layoutBtnUpLevel.hide();
		} else {
			// set parent folder & show backbtn if not staying at root
			g_ParentFolderId = respObject.parent.path;
			g_CurrentFolderId = respObject.current.path;
	
			if (isFocusOnRoot()) {
				g_layoutBtnUpLevel.hide();
			} else {
				g_layoutBtnUpLevel.text(respObject.parent.disp_name);
				g_layoutBtnUpLevel.show();
			}
		}
		showFileList(respObject);
	} else if (respObject.response_type == AJAXREP_PHOTOLIST) {
		g_layoutDivTitle.text(TITLE_PHOTOLIST);
		
		g_ParentFolderId = FILELIST_ID_ROOT;
		g_CurrentFolderId = PHOTOLIST_ID;
		g_layoutBtnUpLevel.text(TITLE_HOME);
		g_layoutBtnUpLevel.show();
		showFileList(respObject);
	} else if (respObject.response_type == AJAXREP_MUSICLIST) {
		g_layoutDivTitle.text(TITLE_MUSICLIST);
		
		g_ParentFolderId = FILELIST_ID_ROOT;
		g_CurrentFolderId = MUSICLIST_ID;
		g_layoutBtnUpLevel.text(TITLE_HOME);
		g_layoutBtnUpLevel.show();
		showFileList(respObject);	
	} else if (respObject.response_type == AJAXREP_ALBUMLIST) {
	} else if (respObject.response_type == AJAXREP_ARTISTLIST) {
	} else if (respObject.response_type == AJAXREP_GENRELIST) {
	} else if (respObject.response_type == AJAXREP_VIDEOLIST) {
		g_layoutDivTitle.text(TITLE_VIDEOLIST);
		
		g_ParentFolderId = FILELIST_ID_ROOT;
		g_CurrentFolderId = VIDEOLIST_ID;
		g_layoutBtnUpLevel.text(TITLE_HOME);
		g_layoutBtnUpLevel.show();
		showFileList(respObject);
	}	

	// then keep monitoring on remote's storage status
	setupNextRoundQueryingStorage();
	if (DEBUG_FLAG && g_keepCheckingStorage) {
		g_checkingStorageCount = 0; // reset counter
	}
}

function handleFileListAjaxComplete(jqXHR, textStatus) {
	// to make sure the webviewer will redraw the list
	$("#fileList").height($("#fileList").height()+1);
	$("#fileList").height($("#fileList").height()-1);

	setTimeout(function () {
		g_FileListListArea.refresh();
	}, 0);
}

// error occurs in ajax command -- FileList
function handleFileListAjaxError(result, textStatus, errorThrown) {
	unlockQueryFileList();

	// go to up level if timeout??? think about it!
	//if (textStatus == "timeout") {
	//	traverseUp();
	//}
}

function createNewListItem(dspTxt, dspTxtCls, itemCls, imgSrc) {
	if (!isValidString(dspTxt)) {
		// nothing to be displayed
		return null;
	}

	// to create a new list item
	var newItem = $(document.createElement("li"));
	if (isValidString(itemCls)) {
		newItem.addClass(itemCls);
	}

	// icon image
	if (isValidString(imgSrc)) {
		var iconImg = $(document.createElement("img"));
		iconImg.get(0).src = FILELIST_ICON_DIR + imgSrc;
		iconImg.appendTo(newItem);
	}

	// display text
	var dspTxtArea = $(document.createElement("div"));
	if (isValidString(dspTxtCls)) {
		dspTxtArea.addClass(dspTxtCls);
	}
	dspTxtArea.append(dspTxt);
	newItem.append(dspTxtArea);

	return newItem;
}

// type helper
function isContentType(typeName) {
	return ((typeName === FILELIST_TYPE_VIDEO) ||
		(typeName === FILELIST_TYPE_MUSIC) ||
		(typeName === FILELIST_TYPE_PHOTO)) ? true : false;
}
function isContainerType(typeName) {
	return ((typeName == FILELIST_TYPE_FOLDER) ||
		(typeName == FILELIST_TYPE_MEDIA) ||
		(typeName == FILELIST_TYPE_SOURCE) ||
		(typeName == FILELIST_TYPE_HDD) ||
		(typeName == FILELIST_TYPE_USB) ||
		(typeName == FILELIST_TYPE_CARD)) ? true : false;
}

function composeClkFuncStr(folderId, pos, tag, bNewQuery) {
	var retStr = "";
	if (isValidString(folderId)) {
		retStr = "itemListClick('" + escape(folderId) + "', " 
			+ pos.toString() + ", '" 
			+ tag + "=" + escape(folderId) + "', " 
			+ bNewQuery.toString() + ")";
	}
	return retStr;
}

function mapThumbTypeToIconSrc(mediaType) {
    var retSrc = FILELIST_ICON_DEFAULT;
    if (isValidString(mediaType)) {
        if (mediaType === FILELIST_TYPE_SOURCE) {
        	retSrc = FILELIST_ICON_THUMB_DIR;
        } else if (mediaType === FILELIST_TYPE_MEDIA) {
        	retSrc = FILELIST_ICON_THUMB_DIR;
        } else if (mediaType === FILELIST_TYPE_FOLDER) {
            retSrc = FILELIST_ICON_THUMB_DIR;
        } else if (mediaType === FILELIST_TYPE_PHOTO) {
            retSrc = FILELIST_ICON_THUMB_PIC;
        } else if (mediaType === FILELIST_TYPE_MUSIC) {
            retSrc = FILELIST_ICON_THUMB_AUD;
        } else if (mediaType === FILELIST_TYPE_VIDEO) {
            retSrc = FILELIST_ICON_THUMB_VID;
        } else if (mediaType === FILELIST_TYPE_HDD) {
            retSrc = FILELIST_ICON_THUMB_HDD;
        } else if (mediaType === FILELIST_TYPE_USB) {
            retSrc = FILELIST_ICON_THUMB_USB;
        } else if (mediaType === FILELIST_TYPE_CARD) {
            retSrc = FILELIST_ICON_THUMB_CARD;
        }
    }
    return retSrc;
}
        
function mapListTypeToIconSrc(listType) {
	var retSrc = FILELIST_ICON_DEFAULT;
	
	if (isValidString(listType)) {
		if (listType === AJAXREP_MUSICLIST) {
			retSrc = FILELIST_ICON_THUMB_AUD;
		} else if (listType == AJAXREP_PHOTOLIST) {
			retSrc = FILELIST_ICON_THUMB_PIC;
		} else if (listType == AJAXREP_VIDEOLIST) {
			retSrc = FILELIST_ICON_THUMB_VID;
		}		
	}
	return retSrc;
}
function showFileList(respObj) {
	// handle the response list
	// A. a newly querying folder: to clear the old list first
	// B. asking for more items for the same folder: to remove the latest item 
	//    (it should be "more") then append the result list next to the older itmes
	var childNodes = respObj.items;

	//==== 1. to handle the old list ====
	if (respObj.offset == null || respObj.offset == 0) {
		// the returning list starts from 0 means the result list represents
		// for a newly querying folder, it has to clear the old list
		g_layoutElemList.empty();
	} else {
		if (g_CurrentFolderId != respObj.current.path) {
			// a new scenario but getting a response list which is not starts from 0???
			// just leave
			return;
		}

		// it means the last query command is asking for more items on the same folder.
		// to remove the item "more" (the last item of the old list) first
		var bottomItemPosition = respObj.offset-1;
		var htmlBottomItem = HTML_ELEMENT_LIST + " li:gt(" + bottomItemPosition + ")";
		$(htmlBottomItem).remove();

		// break if nothing returned
		if (childNodes.length <= 0 ) {
			return;
		}
	}

	//==== 2. prepare the new list ====
	// store the current directory
	var htmlNewListItem = null;
	var bFocusOnRoot = isFocusOnRoot();
	var storageFlag = 0, storageId = STORAGE_ID_UNSUPPORTED;

	var dspStr;
	if (childNodes.length <= 0) {
		// to create an empty list if there's no child node
		if (respObj.parent == null) {
			dspStr = STORAGE_CORRUPT;
		} else {
			dspStr = FOLDER_EMPTY;
		}

		htmlNewListItem = createNewListItem(dspStr, CSSCLS_ITEMTEXT, CSSCLS_LISTITEM_EMPTY, null);
		if (htmlNewListItem != null) {
			htmlNewListItem.appendTo(g_layoutElemList);
		}
	} else {
		var imgSrc;
		var itemClass, dspTxtClass; // css class
		var clkFuncStr; // item click function string
		var itemStartPos; // item start postion is using for next run query
		var newQueryWhenClk; // for a "folder" (container) & "more"

		var debugTag;   // for showing the debug console log

		for (var idx in childNodes) {
			dspStr = childNodes[idx].disp_name;
			dspTxtClass = CSSCLS_ITEMTEXT;
			itemClass = CSSCLS_LISTITEM;
			imgSrc = null;

			itemStartPos = 0; // default it query files from 0-position
			newQueryWhenClk = true;

			if (respObj.response_type == AJAXREP_FILELIST) { // a file or a folder
				// asign one icon image for every existing content or folder
				imgSrc = mapThumbTypeToIconSrc(childNodes[idx].type);

				if (isContentType(childNodes[idx].type)) {
					debugTag = "file";
					itemStartPos = -1; // start position is unmeaning for a file
					newQueryWhenClk = false; // click on a file only trigger the viewer
				} else if (isContainerType(childNodes[idx].type)) {
					debugTag = "folder";
					itemClass += " " + CSSCLS_LISTITEM_CONTAINER;

					// to parse the Storage Status value
					if (!g_gotInitStorageFlag && bFocusOnRoot) {
						storageId = mapStorageNameToId(childNodes[idx].disp_name);
						storageFlag = setStorageFlag(storageFlag, storageId, STORAGE_STATUS_READY);
					}
				}
			} else if (	(respObj.response_type == AJAXREP_PHOTOLIST) ||
						(respObj.response_type == AJAXREP_MUSICLIST) ||
						(respObj.response_type == AJAXREP_VIDEOLIST))
			{
				debugTag = "file";
				imgSrc = mapListTypeToIconSrc(respObj.response_type);
				itemStartPos = -1; // start position is unmeaning for a file
				newQueryWhenClk = false; // click on a file only trigger the viewer				
			}

			// to create a new list item (li)
			htmlNewListItem = createNewListItem(dspStr, dspTxtClass, itemClass, imgSrc);
			if (htmlNewListItem != null) {
				// add touch listener
				if (respObj.response_type == AJAXREP_FILELIST) {
					clkFuncStr = composeClkFuncStr(childNodes[idx].path, itemStartPos, debugTag, newQueryWhenClk);
				}
				else if (respObj.response_type == AJAXREP_PHOTOLIST) {
					clkFuncStr = composeClkFuncStr("photo:" + childNodes[idx].id, itemStartPos, debugTag, newQueryWhenClk);
				}
				else if (respObj.response_type == AJAXREP_MUSICLIST) {
					clkFuncStr = composeClkFuncStr("music:" + childNodes[idx].id, itemStartPos, debugTag, newQueryWhenClk);
				}
				else if (respObj.response_type == AJAXREP_VIDEOLIST) {
					clkFuncStr = composeClkFuncStr("video:" + childNodes[idx].id, itemStartPos, debugTag, newQueryWhenClk);					
				}
				else {
					clkFuncStr = composeClkFuncStr(childNodes[idx].path, itemStartPos, debugTag, newQueryWhenClk);
				}
				
				if (DEBUG_FLAG) {
					console.log("clkFunc=" + clkFuncStr);
				}
				htmlNewListItem.get(0).onclick = Function(clkFuncStr);
				htmlNewListItem.get(0).addEventListener(g_touchEvtStart, touchStart, false);

				// append this item to the file list
				htmlNewListItem.appendTo(g_layoutElemList);
			}
		}	
	}

	// got Storage Status
	if (!g_gotInitStorageFlag && bFocusOnRoot) {
		g_gotInitStorageFlag = true;
		g_storagePlugFlag = storageFlag;
	}
}

//-------- view a content --------
function handleViewContentResponse(respObj) {

	if (respObj.status == 0) {
		// launching viewer successfully, go to REMOTE CONTROL
		if (g_isAndroidAirFun) {
			window.location.href = "nativecmd://android/switchto=remote";
		}
		else if (g_isIOSAirFun) {
			window.location.href = "nativecmd://ios/switchto=remote";
		}
	}
}

//-------- Storage Status --------
// error occurs in ajax command -- query storage
function handleQueryStorageAjaxError(result, textStatus, errorThrown) {
	unlockQueryStorage();
	console.log("storage status err=" + textStatus + "!");

	// retry again ?
	//setCheckStorageTimer(REMOTESTORAGE_CHECKERRWAITING);
}

function handleQueryStorageResponse(respObj) {
	// {"responseType":"storageStatus","storageList":[{"sName":"usb1","sStatus":"ready"},{"sName":"card","sStatus":"absent"}]}
	unlockQueryStorage();

	var bReloadPage = false;
	var storages = respObj.storageList;
	if (storages !== null) {
		if (storages.length > 0) {
			var storageFlag = 0, storageId = STORAGE_ID_UNSUPPORTED;
			for (var sItem in storages) {
				storageId = mapStorageNameToId(storages[sItem].sName);
				storageFlag = setStorageFlag(storageFlag, storageId, storages[sItem].sStatus);
			}
			if (!g_gotInitStorageFlag) {
				g_gotInitStorageFlag = true;
				g_storagePlugFlag = storageFlag;
			} else if (storageFlag !== g_storagePlugFlag) {
				// storage changed, reload the page by itself
				bReloadPage = true;
			}
		}
	}

	if (bReloadPage) {
		setTimeout(function () {
			window.location.reload(true);
		}, 20);
	} else {
		// keep polling 
		setupNextRoundQueryingStorage();
	}
}

//-------- remote reports an error --------
//error occurs in remote's command execution
function handleErrorResponse(respObj) {
	unlockQueryFileList();
	// so far it just prints an warning message
	console.log("reports error from remote. type=" + respObj.type + 
		" [msg=" + respObj.message + ", conseq=" + respObj.consequence + "]");
}


function mapStorageNameToId(storageName) {
	if (storageName !== null) {
		var sName = storageName.toLowerCase();
		if (sName.substr(0, STORAGE_NAME_USB.length) === STORAGE_NAME_USB) {
			return STORAGE_ID_USB;
		} else if (sName.substr(0, STORAGE_NAME_CARD.length) === STORAGE_NAME_CARD) {
			return STORAGE_ID_CARD;
		} else if (sName.substr(0, STORAGE_NAME_HDD.length) === STORAGE_NAME_HDD) {
			return STORAGE_ID_HDD;
		} else if (sName.substr(0, STORAGE_NAME_SMB.length) === STORAGE_NAME_SMB) {
			return STORAGE_ID_SMB;
		}
	}
	return STORAGE_ID_UNSUPPORTED;
}

function setStorageFlag(storFlag, storageIdx, storageStatus) {
	var val = 0;
	if (storageStatus === STORAGE_STATUS_READY) {
		// only READY is meaning for storage checking
		val = 1;
	}
	if ((storageIdx !== STORAGE_ID_UNSUPPORTED) && (storageIdx < STORAGE_ID_MAX)) {
		if (val == 1) {
			storFlag |= (0x1 << storageIdx);
		} else {
			storFlag &= (~(0x1 << storageIdx));
		}
	}
	return storFlag;
}
