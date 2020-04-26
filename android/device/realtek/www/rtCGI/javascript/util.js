var command_xmlhttp;
var DSP_SIZE_TAG_WIDTH = 'w';
var DSP_SIZE_TAG_HEIGHT = 'h';
var DSP_SIZE_COOKIE_WIDTH = 'dspWidth';
var DSP_SIZE_COOKIE_HEIGHT = 'dspHeight';

var HOME_ENTRY_MODE = "entrymode";
var WIFISET_ENTRY_MODE = "wifientry";
var SETUP_ENTRY_MODE = "setupentry";
var VAL_ENTRY_REMOTE = "remote"; // from remote control
var VAL_ENTRY_DIRECT = "directly";
var HOME_COOKIE_ENTRY_MODE = "entryMode";
var WIFISET_COOKIE_ENTRY_MODE = "wifiEntry";
var SETUP_COOKIE_ENTRY_MODE = "setupEntry";

var LOAD_PAGE_MODE = "loadpagemode";
var VAL_LOADPAGE_ONCE = "once";

var CACHE_NONE = 0;
var CACHE_ANY = 1; // write if any
var CACHE_SAVE = 2;// force save

var RESIZE_MODE_NONE = 0;
var RESIZE_MODE_SCALEUP = 1;

function d2h(d) {
	return d.toString(16);
}
function h2d (h) {
	return parseInt(h, 16);
}
function stringToHex (tmp) {
	var str = '',
	i = 0,
	tmp_len = tmp.length,
	c;

	for (; i < tmp_len; i += 1) {
		c = tmp.charCodeAt(i);
		str += d2h(c) + ' ';
	}
	return str;
}
function hexToString (tmp) {
	var arr = tmp.split(' '),
	str = '',
	i = 0,
	arr_len = arr.length,
	c;

	for (; i < arr_len; i += 1) {
		c = String.fromCharCode( h2d( arr[i] ) );
		str += c;
	}
	return str;
}

function getTextRaw(textStr) {
	var nameInHex = stringToHex(textStr);
	var str = "str_len=" + textStr.length + ", hex=0x" + nameInHex + " (str=" + textStr + "!!";
	return str;
}

function hexStrToByteArray(hexStr) {
	var i = 0, j = 0;
	var intArray = new Array();
	intArray[0] = 0;
	var oneC;

	while (i < hexStr.length) {
		oneC = hexStr[i] + hexStr[i+1];
		intArray[j] = h2d(oneC);
		j++;
		i += 2;
	}
	return intArray;
}

function ssidStrToUtf8(hexStr) {
	// covert hex-representation characters to byte array
	var byteArray = hexStrToByteArray(hexStr);
	return String.fromCharCode.apply(String, byteArray);
}

function getSSIDDspStr(hexStr) {
	var ssidStr = ssidStrToUtf8(hexStr);
	try {
		var ssidEsc = escape(ssidStr);
		ssidStr = decodeURI(ssidEsc);
	} catch (err) {
		console.log("unable to decode uri, err=" + err + " for hexStr=" + hexStr);
	}
	return ssidStr;
}

function fixConsole(alertFallback)
{
	if (typeof console === "undefined")
	{
		console = {}; // define it if it doesn't exist already
	}
	if (typeof console.log === "undefined")
	{
		if (alertFallback) { console.log = function(msg) { alert(msg); }; }
		else { console.log = function() {}; }
	}
}
var alertFallback = false;
fixConsole(alertFallback);

function printStrCode(tag, ssid) {
	console.log("[" + tag + "]" + getTextRaw(ssid));
	var ssidEncode = encodeURIComponent(ssid);
	console.log("[encode_" + tag + "]" + getTextRaw(ssidEncode));
}

function parseParam(queryString, parameterName, bOnlyValue)
{
	// Add "=" to the parameter name (i.e. parameterName=value)
	var parameter = parameterName + "=";
	var startPos = 0, endPos = -1, lookupNameLen = parameter.length;
	if ((lookupNameLen > 1) && ((endPos = queryString.length) > 0))
	{
		var begin = -1, end = -1;
		while (endPos > startPos)
		{
			// Find the beginning of the string
			begin = queryString.indexOf(parameter, startPos);
			// If the parameter name is not found, skip it, otherwise return the value
			if ((begin == 0) || ((begin > 0) && ((queryString.charAt(begin - 1) == '&') || (queryString.charAt(begin - 1) == '?'))))
			{
				if (bOnlyValue)
				{
					// Add the length (integer) to the beginning
					begin += lookupNameLen;
				}
				// Multiple parameters are separated by the "&" sign
				end = queryString.indexOf ("&", begin);
				if (end == -1)
				{
					end = queryString.length;
				}
				// Return the string
				return decodeURI(queryString.substring(begin, end));
			}
			else if (begin == -1)
			{
				// not fount
				break;
			}
			else
			{
				// keep searching
				startPos = begin + lookupNameLen;
			}
		}
	}
	return "null";
}

/*
	get value of parameter from the given string,
	for e.g., query string:"a=1&b=3&c=5", getParameter(string, "b") = "3"
*/
function getParameter(queryString, parameterName)
{
	return parseParam(queryString, parameterName, true);
}

/*
	get the key-value pair of this parameter from the given string,
	for e.g., given string:"a=1&b=3&c=5", getParameterPairStr(string, "b") = "b=3"
*/
function getParameterPairStr(queryString, parameterName)
{
	return parseParam(queryString, parameterName, false);
}

function getXmlHttpRequestObject()
{
	var xmlhttp;
	if (window.XMLHttpRequest)
	{
	  // code for IE7+, Firefox, Chrome, Opera, Safari
	  xmlhttp=new XMLHttpRequest();
	}
	else
	{
	  // code for IE6, IE5
	  xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
	}
	return xmlhttp;
}

function handleCommand()
{
	if(command_xmlhttp.readyState == 4)
	{
		if(command_xmlhttp.status == 200)
		{
			showDiscription(command_xmlhttp.responseText);
		}
	}
}

function sendCommand(command, commandHandler)
{
	command_xmlhttp = getXmlHttpRequestObject();
	command_xmlhttp.onreadystatechange = commandHandler;
	var url="/cgi-bin/rtCGI.fcgi?id=0&command=" + command;
	command_xmlhttp.open("GET", url ,true);
	command_xmlhttp.setRequestHeader("If-Modified-Since","0");
	command_xmlhttp.setRequestHeader("Cache-Control","no-cache");
	//command_xmlhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded;charset=UTF-8");
	command_xmlhttp.send(null);
	sendCommand_print(command);
}

function sendCommandDef(command)
{
	sendCommand(command, null);
}

function sendRemoteCommand(command)
{
	command_xmlhttp = getXmlHttpRequestObject();
	command_xmlhttp.onreadystatechange = null;
	var url="/remoteCmd?id=0&command=" + command;
	command_xmlhttp.open("GET", url ,true);
	command_xmlhttp.setRequestHeader("If-Modified-Since","0");
	command_xmlhttp.setRequestHeader("Cache-Control","no-cache");
	command_xmlhttp.send(null);
	sendCommand_print(command);	
}

function sendInputString(input)
{
	command_xmlhttp = getXmlHttpRequestObject();
	command_xmlhttp.onreadystatechange = null;
	var url="/rtCGI.fcgi?id=4&string=" + input;
	command_xmlhttp.open("GET", url ,true);
	command_xmlhttp.setRequestHeader("If-Modified-Since","0");
	command_xmlhttp.setRequestHeader("Cache-Control","no-cache");
	command_xmlhttp.send(null);
	sendCommand_print(command);	
}

function sendCommandPrintRsp(command)
{
	sendCommand(command, handleCommand);
}

function sendCommand_print(command)
{
	var ele = document.getElementById("command_desc");
	if (ele != null)
	{
		ele.innerHTML="Send: " + command;
	}
}

function showDiscription(text)
{
	var ele;
	if ((ele = document.getElementById("desc")) != null)
		ele.innerHTML=text;
}

function handleResponseText(responseText)
{
	var str = new String();
	str = responseText.substring(responseText.search("Response:: ")+11, responseText.search("\r\n</Context>\r\n"));
	return str;
}

function addHyperlinkToText(text, hyperLink)
{
	var newText = new String();

	newText = "<a href=" + hyperLink + ">" + text + "</a>";
	return newText;
}

function setElementVisible(element, visibleMode)
{
	var obj = document.getElementById(element);
	if (obj == null)
		return;

	if ("true" == visibleMode)
	{
		obj.style.display = "inline";
		obj.style.visibility = "visible";
	}
	else if ("hide" == visibleMode)
	{
		obj.style.visibility = "hidden";
	}
	else
	{
		obj.style.display = "none";
	}
}

function setTitleFontSize(mainAreaName, titleAreaName)
{
	var mainArea = document.getElementById(mainAreaName);
	var titleArea = document.getElementById(titleAreaName);
	if (titleArea && mainArea)
	{
		var screenLarge = window.screen.height, screenSmall = window.screen.width;
		if (window.screen.height < window.screen.width)
		{
			screenSmall = window.screen.height;
			screenLarge = window.screen.width;
		}

		var mainAreaFont = '16pt', titleAreaFont = '20pt';
		if (screenLarge <= 640 || screenSmall <= 480)
		{
			mainAreaFont = '10pt';
			titleAreaFont = '14pt';
		}
		else if (screenLarge <= 1280 || screenSmall <= 720)
		{
			mainAreaFont = '12pt';
			titleAreaFont = '16pt';
		}

		mainArea.style.fontSize = mainAreaFont;
		titleArea.style.fontSize = titleAreaFont;
	}
}

function getDspSize(dspSize)
{
	var dspWidth, dspHeight;
	var url = window.location.href, paramIdx = url.indexOf("?");
	var validSize = false;
	if (paramIdx != -1)
	{
		var paraString = url.substr(paramIdx+1);
		var paramWidth = getParameter(paraString, DSP_SIZE_TAG_WIDTH), paramHeight = getParameter(paraString, DSP_SIZE_TAG_HEIGHT);
		if (paramWidth != "null" && paramHeight != "null")
		{
			dspWidth = parseInt(paramWidth);
			dspHeight = parseInt(paramHeight);
			validSize = true;
		}
	}

	if (!validSize)
	{
		printImgArea();
		dspWidth = document.documentElement.clientWidth;
		if (dspWidth > 0)
		{
			dspHeight = document.documentElement.clientHeight;
		}
		else
		{
			dspWidth = document.body.clientWidth;
			dspHeight = document.body.clientHeight;
		}
	}
	dspSize.width = dspWidth;
	dspSize.height = dspHeight;
	return validSize;
}

function getDspSizeStrInUrl()
{
	var url = window.location.href, paramIdx = url.indexOf("?");
	if (paramIdx != -1)
	{
		var paraString = url.substr(paramIdx+1);
		var paramWidth = getParameterPairStr(paraString, DSP_SIZE_TAG_WIDTH), paramHeight = getParameterPairStr(paraString, DSP_SIZE_TAG_HEIGHT);
		if (paramWidth && paramHeight && paramWidth != "null" && paramHeight != "null")
		{
			var sizeStr = paramWidth + "&" + paramHeight;
			console.log("dsp_size:" + sizeStr);
			return sizeStr;
		}
	}
	return null;
}

function getHrefUrl(htmlLink, param)
{
	var paramStr = htmlLink;
	if (param)
	{
		var conStr = '?';
		if (paramStr.indexOf('?') != -1)
			conStr = '&';

		paramStr += conStr + param;
	}
	return paramStr;
}

function getSelfRedirectUrl(htmlLink, param)
{
	var paramStr = getHrefUrl(htmlLink, param);
	return "self.location.href='" + paramStr + "'";
}

function getSelfRedirectUrlWithSizeArgs(htmlFile)
{
	var sizeStr = getDspSizeStrInUrl();
	return getSelfRedirectUrl(htmlFile, sizeStr);
}

String.prototype.Trim = function() {
	return this.replace(/^\s+|\s+$/g, "");
};

function getCookie(cookieName)
{
	var retCookie = null;
	if (document.cookie.length > 0)
	{
		var nameList = document.cookie.split("\;");
		for (idx in nameList)
		{
			var pairs = nameList[idx].split('=');
			if (pairs[0].Trim() == cookieName)
			{
				retCookie = unescape(pairs[1]);
			}
		}
	}
	console.log("read cookie for " + cookieName + ", val=" + retCookie + ". Cnum:" + document.cookie.length);
	return retCookie;
}

function setCookie(cookieName, val)
{
	if (cookieName != null)
	{
		//console.log("set cookie " + cookieName + " as " + val);
		document.cookie = cookieName + '=' + escape(val);
	}
}

function deleteCookie(cookieName)
{
	if (cookieName != null)
	{
		var expires = new Date();
		expires.setTime (expires.getTime() - 1);
		document.cookie = "cookieName=; expires=" + expires.toGMTString()
	}
}

var initFlag = true;
function resetLineHeight(ratio)
{
	if(document.getElementsByTagName('body')[0])
		document.getElementsByTagName('body')[0].style.lineHeight = ratio;
}

function setImageWidthHeight(ratio)
{
	var image = document.getElementsByTagName('img')[0];
	if(image)
	{
		image.width *= ratio;
		image.height *= ratio;
	}
}

function resetImageArea(ratio)
{
	var i, j;
	var area_array = document.getElementsByTagName('area');
	var area_count = area_array.length;
	var area_i;
	for(i = 0; i < area_count; i++)
	{
		if (area_i = area_array[i])
		{
			var coords = area_i.getAttribute('coords');
			var xy = coords.split(',');
			for(j = 0; j < xy.length; ++j)
			{
				xy[j] = xy[j]*ratio;
			}
			area_i.setAttribute('coords', xy.join(','));
		}
	}
}

function resizeImage(imgId, dspSize, checkCached, rsMode)
{
	console.log("reset image:" + imgId);
	var downRatio = 1;
	if (initFlag == true)
	{
		//printImgArea();
		// get image obj
		var imgEle = document.getElementById(imgId);
		if (imgEle == null)
		{
			return -1;
		}

		// get display size
		if (getDspSize(dspSize) != true)
		{
			console.log("no dsp_size found. using dsp.width=" + dspSize.width + ", dsp.height=" + dspSize.height);
			if (CACHE_NONE != checkCached)
			{
				// get display size from cookies
				var cooWidth = getCookie(DSP_SIZE_COOKIE_WIDTH);
				var cooHeight = getCookie(DSP_SIZE_COOKIE_HEIGHT);
				var iCookieW = 0, iCookieH = 0;
				if ((cooWidth != null) && (cooHeight != null) &&
					((iCookieW = parseInt(cooWidth)) !== 0) && ((iCookieH = parseInt(cooHeight)) !== 0))
				{
					dspSize.width = iCookieW;
					dspSize.height = iCookieH;
					console.log("Got dsp_size in cookie! cookie.w=" + dspSize.width + ", cookie.h=" + dspSize.height);
				}
				else if (CACHE_SAVE === checkCached)
				{
					if ((dspSize.width == 0) || (dspSize.height == 0))
					{
						console.log("dsp_size invalid! do not save.");
					}
					else
					{
						setCookie(DSP_SIZE_COOKIE_WIDTH, dspSize.width);
						setCookie(DSP_SIZE_COOKIE_HEIGHT, dspSize.height);
						console.log("Save dsp_size in cookie! cookie.w=" + dspSize.width + ", cookie.h=" + dspSize.height);
					}
				}
			}
		}
		else if (CACHE_NONE != checkCached)
		{
			console.log("write dsp_size to cache. w=" + dspSize.width + ", h=" + dspSize.height);
			setCookie(DSP_SIZE_COOKIE_WIDTH, dspSize.width);
			setCookie(DSP_SIZE_COOKIE_HEIGHT, dspSize.height);
		}

		if ((dspSize.width === 0) || (dspSize.height === 0))
		{
			console.log("dsp_size invalid! do not resizeImg.");
			return 0; // to alert invalid display size!
		}

		var dspAreaWidth = dspSize.width, dspAreaHeight = dspSize.height;
		//DisplaySizeStr = DSP_SIZE_TAG_WIDTH + '=' + dspAreaWidth + '&' + DSP_SIZE_TAG_HEIGHT + '=' + dspAreaHeight;

		// pickup which radio will be apply to scale down the image
		var initWidth = imgEle.width * 1.025, initHeight = imgEle.height * 1.025;
		var widthRatio = dspAreaWidth/initWidth, heightRatio = dspAreaHeight/initHeight;
		var fitableRatio = (widthRatio < heightRatio) ? widthRatio : heightRatio;

		if (initWidth > dspAreaWidth)
		{
			downRatio = (initHeight > dspAreaHeight) ? fitableRatio : widthRatio;
		}
		else if (initHeight > dspAreaHeight)
		{
			downRatio = heightRatio;
		}
		else if (rsMode == RESIZE_MODE_SCALEUP)
		{
			downRatio = fitableRatio;
		}

		if ((downRatio < 1) || (rsMode == RESIZE_MODE_SCALEUP))
		{
			setImageWidthHeight(downRatio);
			resetImageArea(downRatio);
			resetLineHeight(downRatio);
		}
		initFlag = false;
		//printImgArea();
	}
	console.log("finish reset_image:" + imgId);

	/*var remoteParentEle = window.parent.document.getElementById("remote");
	if (remoteParentEle)
	{
		var remoteSizeStr = "[ParentRemote] w:" + remoteParentEle.width + ",h:" + remoteParentEle.height + " cssW:" + remoteParentEle.style.width + ",cssH:" + remoteParentEle.style.height;
		console.log(remoteSizeStr);
	}
	else
	{
		console.log("[ParentRemote] unable to get parent frame");
	}*/

	return downRatio;
}

function printImgArea()
{
	var bodyEle = document.body, htmlEle = document.documentElement;

	var outStr = "[Screen]w:" + window.screen.width + ",h:" + window.screen.height + "<next>";
	var bodyOutStr = "[body]clientW:" + bodyEle.clientWidth + ",clientH:" + bodyEle.clientHeight + "(scroll_w:" + bodyEle.scrollWidth + ",h:" + bodyEle.scrollHeight + ") (offsetW:" + bodyEle.offsetWidth + ",h:" + bodyEle.offsetHeight + ") <next>";
	var htmlOutStr = "[html]clientW:" + htmlEle.clientWidth + ",clientH:" + htmlEle.clientHeight + "(scroll_w:" + htmlEle.scrollWidth + ",h:" + htmlEle.scrollHeight + ") (offsetW:" + htmlEle.offsetWidth + ",h:" + htmlEle.offsetHeight + ") <next>";
/*
	var imgOutStr = "";
	var imgEle = document.getElementsByTagName('img')[0];
	if(imgEle)
	{
		imgOutStr = "<next>[Img]w:" + imgEle.width + ",h:" + imgEle.height + " (cssW:" + imgEle.style.width + ",cssH:" + imgEle.style.height + ")";
	}

	outStr = outStr + bodyOutStr + htmlOutStr + imgOutStr;
*/
	outStr = outStr + bodyOutStr + htmlOutStr;
	console.log(outStr);
}


function isParamMatched(paramName, expectVal, cookieName)
{
	var matched = false;
	var paramVal = null;
	var currentUrl = window.location.href;
	var paramIdx = currentUrl.indexOf('?');
	if (paramIdx != -1)
	{
		paramVal = getParameter(currentUrl.substr(paramIdx+1), paramName);
		if (paramVal != "null")
		{
			// parameter assigned in url
			if (paramVal === expectVal)
			{
				matched = true;
			}
			if (cookieName != null)
			{
				setCookie(cookieName, paramVal);
			}
		}
	}

	if ((cookieName != null) && ((paramVal == null) || (paramVal == "null")))
	{
		// read from cookie
		if (getCookie(cookieName) === expectVal)
		{
			matched = true;
		}
	}

	return matched;
}

function hideEleIf(eleId, paramName, expectVal, cookieName, isMatched)
{
	if (eleId != null)
	{
		var ele = document.getElementById(eleId);
		if ((ele != null) && (isParamMatched(paramName, expectVal, cookieName) == isMatched))
		{
			ele.style.display = 'none';
			return true;
		}
	}
	return false;
}

function hideEleIfNotMatched(eleId, paramName, expectVal, cookieName)
{
	return hideEleIf(eleId, paramName, expectVal, cookieName, false);
}

function hideEleIfMatched(eleId, paramName, expectVal, cookieName)
{
	return hideEleIf(eleId, paramName, expectVal, cookieName, true);
}

function getOffset( el ) {
	var _x = 0;
	var _y = 0;
	while( el && !isNaN( el.offsetLeft ) && !isNaN( el.offsetTop ) ) {
		_x += el.offsetLeft - el.scrollLeft;
		_y += el.offsetTop - el.scrollTop;
		el = el.offsetParent;
	}
	return { top: _y, left: _x };
}

/*
Return the value of the radio button that is checked
Return an empty string if none are checked, or there are no radio buttons
*/
function getRadioCheckedValue(radioObj)
{
	var retVal = "";
	var radioLength = 0;
	var i = 0;

	do
	{
		if (!radioObj)
			break;

		radioLength = radioObj.length;
		if (undefined == radioLength)
			break;

		for (i = 0; i < radioLength; i++)
		{
			if (true ==  radioObj[i].checked)
			{
				retVal = radioObj[i].value;
				break;
			}
		}
	} while (false);

	return retVal;
}

function isWindows()
{
	return navigator.userAgent.match(/windows/i) === null ? false : true;
}
function isAndroid()
{
	return navigator.userAgent.match(/android/i) === null ? false : true;
}
function isiOSDevice()
{
	return navigator.userAgent.match(/iPhone|iPad|iPod/i) === null ? false : true;
}
function isiPhone()
{
	return navigator.userAgent.match(/iPhone/i) === null ? false : true;
}
function isiPad()
{
	return navigator.userAgent.match(/iPad/i) === null ? false : true;
}
function isiPod()
{
	return navigator.userAgent.match(/iPod/i) === null ? false : true;
}
function isAndroidAirFun()
{
	return (isAndroid() && isParamMatched(HOME_ENTRY_MODE, VAL_ENTRY_REMOTE, HOME_COOKIE_ENTRY_MODE)) ? true : false;
}
function isiOSAirFun()
{
	return (isiOSDevice() && isParamMatched(HOME_ENTRY_MODE, VAL_ENTRY_REMOTE, HOME_COOKIE_ENTRY_MODE)) ? true : false;
}

function isTouchDevice()
{
	var uaStr = navigator.userAgent;
	return ("ontouchend" in document) ||
			(uaStr.match(/(iPhone|iPod|iPad)/) !== null) ||
			(uaStr.match(/BlackBerry/) !== null) ||
			(uaStr.match(/Android/) !== null);
}

function getUserDeviceType()
{
	var deviceType = "device";

	if (isAndroid())
	{
		deviceType = "Android device";
	}
	else if (isiPhone())
	{
		deviceType = "iPhone";
	}
	else if (isiPad())
	{
		deviceType = "iPad";
	}
	else if (isiPod())
	{
		deviceType = "iPod";
	}

	return deviceType;
}

/* common function for closing current browser window */
function closeWindow()
{
	var bIsAirFunAndroid = isAndroidAirFun();
	var bIsAirFunIOS = isiOSAirFun();
	if (bIsAirFunAndroid)
	{
		window.location.href = "nativecmd://android/closeapp=now";
	}
	else if (bIsAirFunIOS)
	{
		window.location.href = "nativecmd://ios/closeapp=now";
	}
	else
	{
		window.open('', '_self', '');
		window.close();
	}
}
function reloadOnce()
{
	var queryStr = window.location.href, paramIdx = queryStr.indexOf("?");
	var newUrl = window.location.href;
	if (paramIdx === -1)
	{
		newUrl += "?";
	}
	else
	{
		var loadMode = getParameter(queryStr.substr(paramIdx+1), LOAD_PAGE_MODE);
		if (loadMode != "null")
		{
			if (loadMode === VAL_LOADPAGE_ONCE)
			{
				// has reloaded
				return;
			}
			var loadModePair = getParameterPairStr(queryStr.substr(paramIdx+1), LOAD_PAGE_MODE);
			newUrl = newUrl.replace("&" + loadModePair, "");  // remove parameter (is it needed?? TBD)
			return;
		}
		newUrl += "&";
	}
	newUrl += LOAD_PAGE_MODE + "=" + VAL_LOADPAGE_ONCE;
	window.location.href = newUrl;
}

/* common function for closing current browser window */
function backHistory()
{
	var bIsAirFunAndroid = isAndroidAirFun();
	var bIsAirFunIOS = isiOSAirFun();
	if (bIsAirFunAndroid)
	{
		window.location.href = "nativecmd://android/gohistory=back";
	}
	else if (bIsAirFunIOS)
	{
		window.location.href = "nativecmd://ios/gohistory=back";
	}
	else
	{
		window.history.back();
	}
}

/* common function for count string length in bytes */
function isValidStr(str) {
	return !(str === null ||
		(typeof str === "undefined") ||
		(typeof str.toString === "undefined"));
}

function countStringUtf8Length(str)
{
	var len = 0;
	if (!isValidStr(str)) {
		return 0;
	}
	
	for (var i = 0; i < str.length; i++)
	{
		var code = str.charCodeAt(i);
		if (code <= 0x7f)
		{
		  len += 1;
		}
		else if (code <= 0x7ff)
		{
		  len += 2;
		}
		else if (code >= 0xd800 && code <= 0xdfff)
		{
		  // Surrogate pair: These take 4 bytes in UTF-8 and 2 chars in UCS-2
		  // (Assume next char is the other [valid] half and just skip it)
		  len += 4; i++;
		}
		else if (code < 0xffff)
		{
		  len += 3;
		}
		else
		{
		  len += 4;
		}
	}
	return len;
}

/* Check the input text for alpha or numeric format */
function checkAlphaNumeric(inputTxt)
{
	if (!isValidStr(inputTxt)) {
		return false;
	}
	
	var letters = /^[0-9a-zA-Z]+$/;

	if (inputTxt.match(letters))
	{
		return true;
	}
	else
	{
		return false;
	}
}

function isAsciiStr(inputTxt) {
	if (!isValidStr(inputTxt)) {
		return false;
	}
	return /^[\x20-\x7E]*$/.test(inputTxt)
}

/* HashSet for javascript */
/* notice: the input item is hashed by toString() */
var HashSet = function ()
{
	this.items = {};
	this.itemCount = 0;
};
HashSet.prototype.isValid = function (item)
{
	return (item === null ||
		(typeof item === "undefined") ||
		(typeof item.toString === "undefined")) ? false : true;
}
HashSet.prototype.has = function (item)
{
	return this.isValid(item) ? this.items.hasOwnProperty(item.toString()) : false;
};
HashSet.prototype.add = function (item)
{
	if (this.isValid(item) && !this.has(item))
	{
		this.items[item.toString()] = item;
		this.itemCount++;
	}
	return this;
};
HashSet.prototype.remove = function (item)
{
	if (this.has(item))
	{
		delete this.items[item.toString()];
		this.itemCount--;
	}
	return this;
};
HashSet.prototype.reset = function ()
{
	this.items = {};
	this.itemCount = 0;
	return this;
};
HashSet.prototype.isEmpty = function ()
{
	return this.itemCount === 0;
};
HashSet.prototype.size = function ()
{
	return this.itemCount;
};
