/*
 * Copyright (C) 2015 The Realtek Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.android.BluetoothRTKAutoPairService;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
import android.util.Log;

import java.util.HashMap;
import java.util.ArrayList;


public class BluetoothRTKAutoPairLoadConfig {
    private static final String TAG = "BluetoothRTKAutoPairLoadconfig";
    private static final boolean RTK_DBG = true;
    private static final boolean RTK_INFO = true;
/*************************************************************************
    value    type
    1    :  address_pair
    2    :  name_pair
    4    :  key_pair
    8    :  special_pair
    128  :  vendor_event to scan
***************************************************************************/
    public static final int  address_pair                   = (0x01);
    public static final int  name_pair                      = (0x02);
    public static final int  key_pair                       = (0x04);
    public static final int  manufactSpeciData_pair         = (0x08);
    public static final int  name_flag_pair                 = (0x20); //set: full name, clear: shorten name
    public static final int  vendor_event_pair             = (0x80); //set: vendor event trigger pair

/**************************************************************************
        config global static variables
***************************************************************************/
    public static byte              autopair_type       = 0;
    public static int               total_type_num      = 0;
    public static int               autopair_rssi       = 0;
    public static int               data_length         = 0;
    public static int               vendor_type         = 0;
    public static int               service_timeout     = 0;
    public static String            autopair_mfsd_str   = "";
    public static String            autopair_mask_str   = "";

    public volatile static boolean  service_start       = false;
    public static HashMap<String, String>           autoPairInfoMap = new HashMap<String, String>();

    private static BluetoothRTKAutoPairLoadConfig   instance;

	final static synchronized BluetoothRTKAutoPairLoadConfig getInstance() {
		if(instance == null) {
			instance = new BluetoothRTKAutoPairLoadConfig();
		}
		return instance;
	}

	private InputStream open_config(String name) {
		FileInputStream f_config = null;
		try {
			f_config = new FileInputStream(name);
		} catch (FileNotFoundException e) {
			Log.d(TAG, name + "can not found");
			e.printStackTrace();
		}
		return f_config;
	}

	private void close_config(InputStream is) {
		try {
			is.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

    private void get_info_to_hashmap(Element rootElement,String type_name, String info_name) {

            NodeList type_pair_list = rootElement.getElementsByTagName(type_name);
            Element type_pair_elem = (Element) type_pair_list.item(0);
            String num_str = (type_pair_elem.getAttribute(info_name+"num"));
            String info_str = info_name;
            int num = Integer.parseInt(num_str);
            info(type_name+" : num is : " + num_str);
            autoPairInfoMap.put(info_name+"num",num_str);
            NodeList type_property = type_pair_elem.getChildNodes();
            info(type_name+" : length is : " + type_property.getLength());
            for (int i = 0; i < type_property.getLength(); i++) {
                Node property = type_property.item(i);
                String nodeName = property.getNodeName();
                if(nodeName.indexOf(info_name) >= 0) {
                    String value = property.getFirstChild().getNodeValue();
                    info("get_info_to_hashmap : " + nodeName + " : " + value);
                    autoPairInfoMap.put(nodeName, value);

                }
            }
            print_hashmap();
    }
	private void parse_config(InputStream is) {
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory
					.newInstance();
			DocumentBuilder builder;
			builder = factory.newDocumentBuilder();
			Document doc = builder.parse(is);
            Element rootElement = doc.getDocumentElement();

            //acquiry service timeout
            NodeList autopair_timeout_list = rootElement.getElementsByTagName("Autopair_service_timeout");
            Element autopair_timeout_elem = (Element) autopair_timeout_list.item(0);
            NodeList timeout_property = autopair_timeout_elem.getChildNodes();
            for (int i = 0; i < timeout_property.getLength(); i++) {
                Node property = timeout_property.item(i);
                String nodeName = property.getNodeName();
                if(nodeName.equals("timeout")) {
                    String autopair_timeout_str = property.getFirstChild().getNodeValue();
                    info("timeout value is : " + autopair_timeout_str);
                    service_timeout = Integer.parseInt(autopair_timeout_str);
                }
            }

            //acquiry rssi
			NodeList autopair_rssi_list = rootElement.getElementsByTagName("Autopair_rssi");
			Element autopair_rssi_elem = (Element) autopair_rssi_list.item(0);
            NodeList rssi_property = autopair_rssi_elem.getChildNodes();
            for (int i = 0; i < rssi_property.getLength(); i++) {
                Node property = rssi_property.item(i);
                String nodeName = property.getNodeName();
                if(nodeName.equals("rssi")) {
                    String autopair_rssi_str = property.getFirstChild().getNodeValue();
                    info("rssi value is : " + autopair_rssi_str);
                    autopair_rssi = Integer.parseInt(autopair_rssi_str);
                }
            }

            //acquiry type total_num and type value
			NodeList autopair_type_list = rootElement.getElementsByTagName("Autopair_type");
			Element autopair_type_elem = (Element) autopair_type_list.item(0);
            total_type_num = Integer.parseInt(autopair_type_elem.getAttribute("total_num"));
            info("total_num is :" + total_type_num);
            NodeList type_property = autopair_type_elem.getChildNodes();
            for (int i = 0; i < type_property.getLength(); i++) {
                Node property = type_property.item(i);
                String nodeName = property.getNodeName();
                if(nodeName.equals("type")) {
                    String autopair_type_str = property.getFirstChild().getNodeValue();
                    info("type is : " +autopair_type_str);
                    autopair_type = (byte)Integer.parseInt(autopair_type_str);
                }
            }

            /*  value    type                           explanation
                    0x01    :  address_pair                    the value in the config must be upper-alpha
                    0x02    :  name_pair                       the value must be the full name
                    0x04    :  key_pair                        the value is decimal digit
                    0x08    :  manucfactory_special_pair
                    0x20    :  name match flag
                    0x80    :  vendor_event to scan            if we need vendor event to scan set 0x80 | type_pair
                */
            for(int i = 0; i < total_type_num; i++){
                int autopair_type_temp = autopair_type & (0x01 << i);
                switch(autopair_type_temp) {
                    case address_pair :
                    {
                        info("address pair");
                        get_info_to_hashmap(rootElement, "Address_pair", "address_");
                    }
                    break;

                    case name_pair :
                    {
                        info("name pair");
                        get_info_to_hashmap(rootElement, "Name_pair", "name_");
                    }
                    break;

                    case key_pair :
                    {
                        info("key pair");
                        get_info_to_hashmap(rootElement, "Key_pair", "key_");
                    }
                    //break;
                    //fall throutgh

                    case manufactSpeciData_pair:
                    {
                        info("ManufacturerSpecificData pair");
                        NodeList autopair_manucfact_list = rootElement.getElementsByTagName("ManufactSpeciData");
                        Element autopair_manucfact_elem = (Element) autopair_manucfact_list.item(0);
                        data_length = Integer.parseInt(autopair_manucfact_elem.getAttribute("data_length"));
                        info("data_length is :" + data_length);
                        NodeList manucfact_property = autopair_manucfact_elem.getChildNodes();
                        for (int k = 0; k < manucfact_property.getLength(); k++) {
                            Node property = manucfact_property.item(k);
                            String nodeName = property.getNodeName();
                            if(nodeName.equals("specidata_value")) {
                                autopair_mfsd_str = property.getFirstChild().getNodeValue();
                                info("manufact_data is : " +autopair_mfsd_str);

                            }
                            if(nodeName.equals("mask_value")) {
                                autopair_mask_str = property.getFirstChild().getNodeValue();
                                info("mask_value is : " +autopair_mask_str);
                            }
                        }
                    }
                    break;

                    default :
                        info("unknow pair type!");
                    break;
                }
            }


            //acquiry vendor type for vendor event
            if((byte)(autopair_type & (byte)vendor_event_pair) == (byte)vendor_event_pair) {
			NodeList autopair_vendor_list = rootElement.getElementsByTagName("Autopair_vendor_event");
			Element autopair_vendor_elem = (Element) autopair_vendor_list.item(0);
            vendor_type = Integer.parseInt(autopair_vendor_elem.getAttribute("vendor_type"));
            info("vendor_type is :" + vendor_type);
            /*
            NodeList vendor_property = autopair_vendor_elem.getChildNodes();
            for (int i = 0; i < vendor_property.getLength(); i++) {
                Node property = vendor_property.item(i);
                String nodeName = property.getNodeName();
                if(nodeName.equals("vendor_data")) {
                    autopair_vendor_str = property.getFirstChild().getNodeValue();
                    info("vendor_data is : " +autopair_vendor_str);

                }
                if(nodeName.equals("mask_value")) {
                    autopair_mask_str = property.getFirstChild().getNodeValue();
                    info("mask_value is : " +autopair_mask_str);
                }
            }
            */
            }
		} catch (ParserConfigurationException e) {
			e.printStackTrace();
		} catch (SAXException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void do_config() {
        info("do_config");
		String path = "/system/etc/bluetooth/rtkbt_autopair_config.xml";
        autoPairInfoMap.clear();
		InputStream is = open_config(path);
		parse_config(is);
		close_config(is);
	}

    private static void log(String msg) {
        if(RTK_DBG)
            Log.d(TAG, msg);
    }
    private static void info(String msg) {
        if(RTK_INFO)
            Log.i(TAG, msg);
    }

    private static void print_hashmap() {
        if(RTK_DBG) {
            info("print_hashmap");
            if(autoPairInfoMap.containsKey("num")) {
                int num = Integer.parseInt(autoPairInfoMap.get("num"));
                info("num is : " + num);
                if((byte)(autopair_type & (byte)vendor_event_pair) == (byte)vendor_event_pair) {
                    info("vendor_type is :" + vendor_type);
                    info("manufact_data is : " +autopair_mfsd_str);
                    info("mask_value is : " +autopair_mask_str);
                }
                for(int i = 0; i < total_type_num; i++){
                int autopair_type_temp = autopair_type & (0x01 << i);
                info("type is : " +autopair_type);
                switch(autopair_type_temp) {
                    case address_pair :
                    {
                        info("address pair");
                        for(int j = 0; j < num; j++) {
                            String address = "address_" + String.valueOf(j+1);
                            String value = autoPairInfoMap.get(address);
                            info(address + " : " + value);
                        }
                    }
                    break;

                    case name_pair :
                    {
                        info("name pair");
                        for(int j = 0; j < num; j++) {
                            String name = "name_" + String.valueOf(j+1);
                            String value = autoPairInfoMap.get(name);
                            info(name + " : " + value);
                        }
                    }
                    break;

                    case key_pair :
                    {
                        info("key pair");
                        for(int j = 0; j < num; j++) {
                            String key = "key_" + String.valueOf(j+1);
                            String value = autoPairInfoMap.get(key);
                            info(key + " : " + value);
                        }

                    }
                    break;

                    default :
                        info("unknow pair type!");
                    break;
                }
            }

            }
        }
    }

}



