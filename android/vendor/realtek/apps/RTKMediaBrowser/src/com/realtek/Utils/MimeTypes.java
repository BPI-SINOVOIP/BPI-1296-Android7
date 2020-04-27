package com.realtek.Utils;


import java.util.HashMap;
import java.util.Map;

import android.util.Log;
import android.webkit.MimeTypeMap;

public class MimeTypes {

	private Map<String, String> mMimeTypes;

	public MimeTypes() {
		mMimeTypes = new HashMap<String,String>();
	}
	
	public void put(String type, String extension) {
		// Convert extensions to lower case letters for easier comparison
		extension = extension.toLowerCase();
		
		mMimeTypes.put(type, extension);
	}
    public boolean isSupportThisMimeType(String mimetype)
    {
        return mMimeTypes.containsValue(mimetype);
    }	
	public String getMimeType(String filename) {
		
		//Log.d("FileFilter","getMimeType filename:"+filename);
		String extension = FileUtils.getExtension(filename);
        /*
		extension =extension.toLowerCase();
		
		//Log.d("FileFilter","getMimeType extension:"+extension);
		
		// Let's check the official map first. Webkit has a nice extension-to-MIME map.
		// Be sure to remove the first character from the extension, which is the "." character.
		if (extension.length() > 0)
		{
			//Log.d("FileFilter","getMimeType extension:1");
			String webkitMimeType = MimeTypeMap.getSingleton().getMimeTypeFromExtension(extension.substring(1));
		
			///Log.d("FileFilter","getMimeType extension:2"+webkitMimeType);
			if (webkitMimeType != null) {
				// Found one. Let's take it!
				
				//Log.d("FileFilter","getMimeType webkitMimeType:"+webkitMimeType);
				return webkitMimeType;
			}
		}
		*/
		//Log.d("FileFilter","getMimeType extension:3");
		
		// Convert extensions to lower case letters for easier comparison
		extension = extension.toLowerCase();
		
		//Log.d("FileFilter","getMimeType extension:"+ extension);
		
		String mimetype = mMimeTypes.get(extension);
		
		if(mimetype==null) return null;
		
		
		//Log.d("FileFilter","getMimeType mimetype:"+mimetype);
		return mimetype;
	}
	
	public boolean isImageFile(String filename)
	{
		if(filename == null) 
			return false;
		
		String mimeType = this.getMimeType(filename);
		
		if(mimeType == null)
			return false;
		
		if((mimeType.substring(0, 6)).equals("image/"))
		{
			return true;
		}
			
		return false;	
	}
	
	public boolean isVideoFile(String filename)
	{
		if(filename == null) 
			return false;
		
		String mimeType = this.getMimeType(filename);
		
		if(mimeType == null)
			return false;
				
		
		if((mimeType.substring(0, 6)).equals("video/"))
		{
			return true;
		}
			
		return false;	
	}
	
	public boolean isAudioFile(String filename)
	{
		if(filename == null) 
			return false;
		
		String mimeType = this.getMimeType(filename);
		
		if(mimeType == null)
			return false;
		
		
		if((mimeType.substring(0, 6)).equals("audio/"))
		{
			return true;
		}
			
		return false;	
	}
	public boolean isPlaylistFile(String filename)
	{
		if(filename == null) 
			return false;
		
		String mimeType = this.getMimeType(filename);
		
		if(mimeType == null)
			return false;
		
		
		if((mimeType.substring(0, 5)).equals("list/"))
		{
			return true;
		}
			
		return false;	
	}
	public boolean isAPKFile(String filename)
	{
		if (filename == null)
			return false;
		
		String mimeType = this.getMimeType(filename);
		Log.d("FileFilter","isAPKFile"+mimeType);
		if(mimeType == null)
			return false;
		if(39==mimeType.length())
		{
			if((mimeType.substring(0, 39)).equals("application/vnd.android.package-archive"))
			{
				return true;
			}
		}	
		return false;	
	}
	

}
