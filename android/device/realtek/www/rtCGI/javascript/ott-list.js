var debug_type = "console"; // "alert" or "console"

//-----------------------------------GLOBAL VARIABLES AND CONSTANTS---------------------------------
//Object to handle the scrolling functionality
var myScroll;

var gOttListNames; // Array to store profile name

// using the icon perfix filename to indicate it's an app shortcut
var APPICONFILENAMEPREFIX = "-aPpIcon-";
var APPICONPREFIXSIG = APPICONFILENAMEPREFIX.charAt(0);
var APPICONDIR = "images/";

// layout
var TITLE_BAR_HEIGHT_DEFAULT = 44; // px
var OTT_ICON_WIDTH = 180;   // px
var OTTICONDIR = "images/ott/";
var OTT_COL_ICON_MIN = 2;
var ottColIconCount = OTT_COL_ICON_MIN;
var columnStyle = "";
var bIsAndroidAirFun = isAndroidAirFun();
var bIsIOSAirFun = isiOSAirFun();
//---------------------ATTACH FUNCTIONALITY TO DOCUMENT EVENTS-----------------------------------

//Prevent default behaviour of the touch event. 
//This is required for the iScroll framework to manage all the touch events and handle the scrolling on the list.
//Also to dont allow the user to move the remote control screen, this way it will always remain static
document.addEventListener('touchmove', function(e) { e.preventDefault(); });

//This will be executed the first time when the page is loaded, after the elements of the page are ready
$(document).ready(function() {
	//Initialize the iScroll object which will manage the scrolling on the itemlistcontent html div element
	myScroll = new iScroll('ottListArea', { desktopCompatibility: true });

	var headerHeight = parseInt($(".titleSize").css("height"));
	if (headerHeight < TITLE_BAR_HEIGHT_DEFAULT) {
		headerHeight = TITLE_BAR_HEIGHT_DEFAULT;
	}
	var newListHeight = $(window).height() - headerHeight;
	$(".listSize").css("height", newListHeight);

	ottColIconCount = Math.floor($(window).width() / OTT_ICON_WIDTH);
	if (ottColIconCount < OTT_COL_ICON_MIN) {
		ottColIconCount = OTT_COL_ICON_MIN;
		var newColWidthProp = Math.floor(100 * $(window).width() / (OTT_ICON_WIDTH * OTT_COL_ICON_MIN) / OTT_COL_ICON_MIN) + "%";
		//debug_sprc("win_width=" + $(window).width() + ", new_width=" + newColWidthProp);
		columnStyle = "style='width: " + newColWidthProp + "'";
	}
	sendCommand("getOttList", handleOttList);
});

function handleOttList()
{
	var ottList = new String();
	var col, col_max;
	var row, row_max;
	var listIdx, list_num;
	var gOttHTML = new Array();
		
	if(command_xmlhttp.readyState==4)
	{
		if(command_xmlhttp.status == 200)
		{
			// Retrieve device name from the response text
			ottList = handleResponseText(command_xmlhttp.responseText);
			console.log(ottList);

			// Response format:
			// OttNum\n[OttName-1]\n[OttName-2]\n[OttName-3]........\n[OttName-X]

			// Split profile name information into array
			gOttListNames = ottList.split("\n");
			listIdx = 1;
			
			col_max = ottColIconCount;
			list_num = gOttListNames[0];
			row_max = Math.ceil(list_num / col_max);								
			debug_sprc("column_num:" + col_max + ", row_num:" + row_max);

			// Display Image
			var appIconPrefixSig = APPICONFILENAMEPREFIX.charAt(0); // the significant char
			var appIconPrefixLen = APPICONFILENAMEPREFIX.length;
			var ottImgName = "", ottImgPath = "";
			var bGotAppShortcut = false;
			debug_sprc("column style is: " + columnStyle);
			for (row = 0; row < row_max; row++)
			{
				gOttHTML[row] = "<li><div>";
				for (col = 0; col < col_max; col++)
				{
					if ((listIdx/2) <= list_num)
					{
						ottImgName = gOttListNames[listIdx+1];
						if (ottImgName.charAt(0) === appIconPrefixSig &&
							ottImgName.length > appIconPrefixLen &&
							ottImgName.substring(0, appIconPrefixLen) === APPICONFILENAMEPREFIX)
						{
							// it's an app shortcut
							ottImgPath = APPICONDIR + ottImgName.substr(appIconPrefixLen);
							bGotAppShortcut = true;
						}
						else
						{
							ottImgPath = OTTICONDIR + ottImgName;
						}
						gOttHTML[row] = gOttHTML[row] + " <input " + columnStyle + " type=\"image\" src=\"" + ottImgPath + "\" onClick=\"launchAp(\'" + gOttListNames[listIdx] + "\');\" />";
						listIdx = listIdx + 2;
					}
				}
				gOttHTML[row] = gOttHTML[row] + "</div></li>";
				//console.log(gOttHTML[row]);
				document.getElementById("ottListSel").innerHTML = document.getElementById("ottListSel").innerHTML + gOttHTML[row];
			}
			if (bGotAppShortcut)
			{
				document.getElementById("listtitle").innerHTML = "APP LIST";
			}
		}
	}
}

function launchAp(apname){
	var cmd;
	cmd = "launchOttAp:" + apname;
	sendCommand(cmd, handleLaunchOttAp);
}

function handleLaunchOttAp(){
	var rsp;
	if(command_xmlhttp.readyState==4)
	{
		if(command_xmlhttp.status == 200)
		{
			// Retrieve device name from the response text
			rsp = handleResponseText(command_xmlhttp.responseText);
			console.log("handleLaunchOttAp rsp=",rsp);
			if (rsp === "BUSY")
			{
				alert("System busy. Please quit the current service and try again");
			}
			else if (rsp === "CLOSE_NET")
			{
				alert("This app will close connection of AirFun");
			}
			else if (bIsAndroidAirFun)
			{
				window.location.href = "nativecmd://android/switchto=remote";
			}
			else if (bIsIOSAirFun)
			{
				window.location.href = "nativecmd://ios/switchto=remote";
			}
		}
	}
}

function debug_sprc(message) {
	if (debug_type == "console") {
		console.log(message);
	} else if (debug_type == "alert") {
		alert(message);	
	}
} 