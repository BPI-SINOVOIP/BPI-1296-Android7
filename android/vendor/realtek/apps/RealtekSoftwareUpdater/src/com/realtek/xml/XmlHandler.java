package com.realtek.xml;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.jar.Attributes;

import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class XmlHandler extends DefaultHandler {

    private StringBuffer buffer = new StringBuffer();
    private ArrayList<RSSItemField> itemList;
    private RSSItemField item;
    private String template;
    
    @Override
    public void startElement(String uri, String localName, String qName,
            org.xml.sax.Attributes attributes) throws SAXException {
        // TODO Auto-generated method stub

        buffer.setLength(0);
        
        if(localName.equals("array")) {
            itemList = new ArrayList<RSSItemField>();
        }
        
        if(localName.equals("dict")) {
            item = new RSSItemField();
        }
        
    }
    
    @Override
    public void endElement(String uri, String localName, String qName) throws SAXException {
        
        if(localName.equals("dict")) {
            itemList.add(item);
            item = null;
        } 

        if(localName.equals("key")) {
            template = removeEnter(buffer.toString());
        }
        
        if(localName.equals("string")) {  // first
            if(template.compareTo("product") == 0) {
                template = removeEnter(buffer.toString());
            } else if(template.compareTo("type") == 0) {
                item.type = removeEnter(buffer.toString());
            } else if(template.compareTo("old-Version") == 0) {
                item.oldVersion = removeEnter(buffer.toString());
            } else if(template.compareTo("new-Version") == 0) {
                item.newVersion = removeEnter(buffer.toString());
            } else if(template.compareTo("Region") == 0) {
                item.region = removeEnter(buffer.toString());
            } else if(template.compareTo("SWURL") == 0) {
                item.swURL = removeEnter(buffer.toString());
            } else if(template.compareTo("SWReleaseNotes") == 0) {
                item.swReleaseNotes = removeEnter(buffer.toString());
            } else  {

            }
            template = null;
        }        
        
    }
    
    public void characters(char[] ch, int start, int length) {
        buffer.append(ch, start, length);
    }  
    
    public ArrayList<RSSItemField> retrieveXmlList() {
        return itemList;
    }
    
    // the data contains "\n" we have to remove them.
    private String removeEnter(String in) {
        return in;
        //return in.replaceAll("[\n]", "");        
    }
}
