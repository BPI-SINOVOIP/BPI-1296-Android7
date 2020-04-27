package com.realtek.xml;

import java.io.StringReader;
import java.util.ArrayList;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;

public class XmlParser {
    
    private XMLReader initializeReader() throws ParserConfigurationException, SAXException {
        SAXParserFactory factory = SAXParserFactory.newInstance();
        // create a parser
        SAXParser parser = factory.newSAXParser();
        // create the reader (scanner)
        XMLReader xmlreader = parser.getXMLReader();
        return xmlreader;
    }
    
    public ArrayList<RSSItemField> parseXmlResponse(String xml) {
        
        try {
            
            XMLReader xmlreader = initializeReader();
            
            XmlHandler handler = new XmlHandler();  // TODO: implement later???

            // assign our handler
            xmlreader.setContentHandler(handler);
            // perform the synchronous parse
            xmlreader.parse(new InputSource(new StringReader(xml)));
            
            return handler.retrieveXmlList();
            
        } 
        catch (Exception e) {
            e.printStackTrace();
            return null;
        }
        
    }    
    
}
