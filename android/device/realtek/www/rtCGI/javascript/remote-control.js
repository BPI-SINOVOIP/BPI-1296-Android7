var initialSetup = true;
var REMOTE_CONTROL_URL = "/remote/remote_control.html?";
var REMOTE_GESTURE_URL = "/remote/remote_gesture.html?";

/**************************************************************
 *	Touch key control
 **************************************************************/
// for detect click event
var g_TOUCH_START = "touchstart";
var g_TOUCH_END = "touchend";
var g_elementTapped = null;

function setNaviKey(EleId)
{
	if ((typeof EleId !== "string") || (EleId.length < 1))
	{
		return null;
	}

	var retKey = EleId;
	var ele = document.getElementById(EleId);
	if (ele != null)
	{
		if (EleId === "navikey_up") {
			ele.navi_val = "UP";
		} else if (EleId === "navikey_down") {
			ele.navi_val = "DOWN";
		} else if (EleId === "navikey_left") {
			ele.navi_val = "LEFT";
		} else if (EleId === "navikey_right") {
			ele.navi_val = "RIGHT";
		} else if (EleId === "navikey_back") {
			ele.navi_val = "BACK";
		} else if (EleId === "navikey_ok") {
			ele.navi_val = "OK";
		} else if (EleId === "key_volup") {
			ele.navi_val = "VOL_UP";
		} else if (EleId === "key_voldown") {
			ele.navi_val = "VOL_DOWN";
		}else {
			retKey = null;
		}
	}
	else
	{
		retKey = null;
	}
	return retKey;
}

function touchKeyStart(evt)
{
	//console.log("got touch START for:" + evt.target.id);
	g_elementTapped = setNaviKey(evt.target.id);
}

function touchKeyEnd(evt)
{
	//console.log("got touch END for:" + evt.target.id + ", val=" + evt.target.navi_val);
	if (g_elementTapped === evt.target.id)
	{
		// click
		sendRemoteCommand(evt.target.navi_val);
	}
	g_elementTapped = null;
}

function initTouchKeyListener()
{
	if (!isTouchDevice())
	{
		g_TOUCH_START = "mousedown";
		g_TOUCH_END = "mouseup";
	}
	document.addEventListener(g_TOUCH_START, touchKeyStart, false);
	document.addEventListener(g_TOUCH_END, touchKeyEnd, false);
}

/**************************************************************
 *	Gesture Control
 **************************************************************/
function initGesture(imageId)
{
	var dspSize = {width:10, height:10};
	var downRatio = resizeImage(imageId, dspSize, CACHE_SAVE, RESIZE_MODE_SCALEUP);
	if (downRatio == 0 && isAndroidAirFun())
	{
		reloadOnce();
	}
	else if (downRatio > 0)
	{
		addGestureListeners(imageId);
	}
}

var startX = -1;
var startY = -1;
var endX = -1;
var endY = -1;

var CHECK_POINT_NUMBER = 1; // single tap
var check_point_counter = 0;
var direction = -1; 	
		// 0 --> X+
		// 1 --> Y+
		// 2 --> X-
		// 3 --> Y-
var bBadGesture = false;
var func_touch_start_calling_counter = 0;

function addGestureListeners(imageId)
{
	var ele_gestureArea = document.getElementById(imageId);
	if (ele_gestureArea == null)
		return;

	ele_gestureArea.addEventListener('touchstart', handleTouchStart, false);
	ele_gestureArea.addEventListener('touchmove', handleTouchMove, false);
	ele_gestureArea.addEventListener('touchend', handleTouchEnd, false);
	ele_gestureArea.addEventListener('touchcancel', handleTouchLeave, false);

	// for mouse event
	//var ele_gestureArea = document.getElementById(imageId);
	//ele_gestureArea.addEventListener('mousedown', handleTouchStart, false);
	//ele_gestureArea.addEventListener('mousemove', handleTouchMove, false);
	//ele_gestureArea.addEventListener('mouseup', handleTouchEnd, false);
	//ele_gestureArea.addEventListener('mouseout', handleTouchLeave, false);
}

function handleTouchStart(event)
{
	console.log("touch start...");
	event.preventDefault();
	func_touch_start_calling_counter++;
	//startX = event.pageX; // for mouse event
	//startY = event.pageY;	// for mouse event
	startX = event.touches[0].pageX;
	startY = event.touches[0].pageY;
	endX = startX;
	endY = startY;
	console.log("touch start : [" + startX + ", " + startY + "]");
}

function handleTouchMove(event)
{
	if (func_touch_start_calling_counter < 1)
		return;

	if(func_touch_start_calling_counter > 1 || bBadGesture == true)
	{
		bBadGesture = true;
		return;
	}

	//endX = event.pageX; // for mouse event
	//endY = event.pageY; // for mouse event
	endX = event.touches[0].pageX;
	endY = event.touches[0].pageY;
	//console.log("touch move : [" + endX + ", " + endY + "]");
	var deltaX = endX - startX;
	var deltaY = endY - startY;
	//console.log("touch move delta : [" + deltaX + ", " + deltaY + "]");
	
	check_point_counter++;
	if(CHECK_POINT_NUMBER == check_point_counter)
	{
		// reach check point, decide moving direction
		if(Math.abs(deltaY) >= Math.abs(deltaX))
		{
			// asume moving along Y direction 
			if(deltaY > 0)
			{
				// Y+ direction
				direction = 1;
			}
			else if(deltaY < 0)
			{
				// Y- direction
				direction = 3;
			}
			else
			{
				// deltaY == deltaX == 0, doesn't move
			}
		}
		else
		{
			if(deltaX > 0)
			{
				// X+ direction, do favorite+ opration
				direction = 0;
			}
			else
			{
				// X- direction, do favorite- opration
				direction = 2;
			}
		}
	}
	else if(CHECK_POINT_NUMBER < check_point_counter && bBadGesture == false)
	{
		switch(direction)
		{
		case 0:	// X+
			if(Math.abs(deltaY) >= Math.abs(deltaX) || deltaX < 0)
			{
				bBadGesture = true;		
				console.log("move direction X+ failed...");
			}
			break;
		case 1:	// Y+
			if(Math.abs(deltaY) < Math.abs(deltaX) || deltaY < 0)
			{
				bBadGesture = true;		
				console.log("move direction Y- failed...");
			}
			break;
		case 2:	// X-
			if(Math.abs(deltaY) >= Math.abs(deltaX) || deltaX > 0)
			{
				bBadGesture = true;		
				console.log("move direction X- failed...");
			}
			break;
		case 3:	// Y-
			if(Math.abs(deltaY) < Math.abs(deltaX) || deltaY > 0)
			{
				bBadGesture = true;		
				console.log("move direction Y+ failed...");
			}
			break;
		default:
			break;
		}
	}
}

function handleTouchRelease(event, bAway)
{
	console.log("touch end...");
	if(func_touch_start_calling_counter == 1)
	{
		if (bBadGesture == true)
		{
			// bad gesture, do nothing or send hint...	
			console.log("bad gesture...");
		}
		else if(CHECK_POINT_NUMBER > check_point_counter)
		{
		 	//
		 	console.log("tap command, and away is:" + bAway);
		 	if (bAway == false)
		 	{
		 		sendRemoteCommand('OK');
		 	}
		}
		else
		{
			// do opration accordingly
			switch(direction)
			{
				case 0:	// X+ direction
					console.log("move direction X+ ...");
					sendRemoteCommand('RIGHT');
					break;
				case 1:	// Y- direction
					console.log("move direction Y+ ...");
					sendRemoteCommand('DOWN');
					break;
				case 2:	// X- direction
					console.log("move direction X- ...");
					sendRemoteCommand('LEFT');
					break;
				case 3:	// Y+ direction
					console.log("move direction Y- ...");
					sendRemoteCommand('UP');
					break;
				default:
					break;
			}
		}
	}
	else
	{
		console.log("bad gesture with touch_times:" + func_touch_start_calling_counter);
	}

	// reset some variables
	check_point_counter = 0;
	direction = -1;
	bBadGesture = false;
	startX = startY = endX = endY = -1;
	func_touch_start_calling_counter = 0;
}

function handleTouchEnd(event)
{
	handleTouchRelease(event, false);
}
function handleTouchLeave(event)
{
	handleTouchRelease(event, true);
}