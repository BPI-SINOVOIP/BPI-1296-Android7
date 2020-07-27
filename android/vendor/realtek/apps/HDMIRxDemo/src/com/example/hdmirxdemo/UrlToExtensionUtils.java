package com.example.hdmirxdemo;
	
import android.text.TextUtils;
import java.util.regex.Pattern;

public class UrlToExtensionUtils {	

	public  static String getFileExtensionFromUrl(String url) {
        	if (!TextUtils.isEmpty(url)) {
            		int fragment = url.lastIndexOf('#');
            		if (fragment > 0) {
                		url = url.substring(0, fragment);
            		}

            		int query = url.lastIndexOf('?');
            		if (query > 0) {
                		url = url.substring(0, query);
            		}

            		int filenamePos = url.lastIndexOf('/');
            		String filename = 0 <= filenamePos ? url.substring(filenamePos + 1) : url;

            		// if the filename contains special characters, we don't
            		// consider it valid for our matching purposes:
            		if (!filename.isEmpty() && Pattern.matches("[a-zA-Z_0-9 \\.\\-\\(\\)\\%]+", filename)) {
                		int dotPos = filename.lastIndexOf('.');
                		if (0 <= dotPos) {
					String fileExtension = filename.substring(dotPos + 1);
					if (!fileExtension.equals("VOB"))
						fileExtension = toLowerCase(fileExtension);
					return fileExtension;		
                		}
            		}
        	}

        	return "";
    	}
	
	
	public static String toLowerCase(String str) {
		StringBuffer sb = new StringBuffer();
		for(int i = 0; i <= str.length()-1;i++) {
			char ch;
			if(str.charAt(i) >= 'A' && str.charAt(i) <= 'Z')
				ch = (char) (str.charAt(i)+32);
			else
				ch = str.charAt(i);
			sb.append(ch);
		}
		return sb.toString();
	}
}
