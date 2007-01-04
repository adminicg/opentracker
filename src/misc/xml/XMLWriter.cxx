/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** source file for XMLWriter class.
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>

#ifndef OT_NO_XMLWRITER_SUPPORT


// selects between usage of XERCES and TinyXML
#include <OpenTracker/tool/XMLSelection.h>

#ifdef USE_XERCES
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>


#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif
#include <xercesc/util/OutOfMemoryException.hpp>

#endif //USE_XERCES

#include <memory>

#include <OpenTracker/misc/xml/XMLWriter.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

//using namespace std;

#ifdef USE_XERCES
XERCES_CPP_NAMESPACE_USE
#endif //USE_XERCES

namespace ot {

#ifdef OT_LOCAL_GRAPH
    XMLWriter::XMLWriter( Context & context_ , unsigned int indent_ ) :
        context( context_ ), indent( indent_ )
    {};

    XMLWriter::~XMLWriter()
    {
  
    };


#  ifdef USE_XERCES
// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of char* data to XMLCh data.
// ---------------------------------------------------------------------------
class XStr
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XStr(const char* const toTranscode)
    {
        // Call the private transcoding method
        fUnicodeForm = XMLString::transcode(toTranscode);
    }

    ~XStr()
    {
        XMLString::release(&fUnicodeForm);
    }


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* unicodeForm() const
    {
        return fUnicodeForm;
    }

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fUnicodeForm
    //      This is the Unicode XMLCh format of the string.
    // -----------------------------------------------------------------------
    XMLCh*   fUnicodeForm;
};

#    define X(str) XStr(str).unicodeForm()


    void writeNode(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode * toWrite, XERCES_CPP_NAMESPACE_QUALIFIER XMLFormatTarget * target )
    {
        XMLCh * lsCh =  XMLString::transcode("LS");
        DOMImplementation * impl = DOMImplementationRegistry::getDOMImplementation(lsCh);
        DOMWriter * writer = impl->createDOMWriter();
        if (writer->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
            writer->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);
        if (writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
            writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
        try {
            // do the serialization through DOMWriter::writeNode();
            writer->writeNode(target, *toWrite);
        }
        catch (const XMLException& toCatch) {
            char * message = XMLString::transcode(toCatch.getMessage());
            logPrintE("XMLWriter Exception message is: %s\n", message);
            XMLString::release( &message );
        }
        catch (const DOMException& toCatch) {
            char * message = XMLString::transcode(toCatch.msg);
            logPrintE("XMLWriter Exception message is: %s\n", message);
            XMLString::release( &message );
        }
        catch (...) {
            logPrintE("XMLWriter Unexpected Exception \n");
        }
        writer->release();
        XMLString::release( &lsCh );
    }


    
    DOMElement * createNode(DOMDocument * doc, Node * someNode){
        if ( doc == NULL || someNode ==NULL)
            return NULL;

        DOMElement* theElement = NULL;
        try {
            theElement = doc->createElement(X((someNode->getType()) .c_str()));
            // add all attributes
            StringTable & attr = someNode->getAttributes();
            KeyIterator keys(attr);
            while( keys.hasMoreKeys())
            {
                
                const std::string & key = keys.nextElement();
                XMLCh * tempKey = XMLString::transcode( key.c_str() );
                XMLCh * tempValue = XMLString::transcode( attr.get( key ) .c_str());
                XMLCh * xmlspace = XMLString::transcode("");
                theElement->setAttributeNS( xmlspace, tempKey, tempValue );
                XMLString::release( & tempKey );
                XMLString::release( & tempValue );
                XMLString::release( & xmlspace );
                
            }
            
            // add all children
            for (unsigned int i = 0; i < someNode->countAllChildren() ; i++){
                DOMElement * el = createNode(doc, someNode->getAnyChild(i));
                
                if (el != NULL)
                    theElement->appendChild(el);
            }
        }catch (const OutOfMemoryException&)
        {
            logPrintE("XMLWriter error, out of memory exception caught while trying to create node %s!!\n", (someNode->getType() .c_str()));
        }
        catch (const DOMException&)
        {
            logPrintE("XMLWriter error, DOM exception caught while trying to create node %s!!\n", (someNode->getType() .c_str()));
        }
        catch (...)
        {
            logPrintE("XMLWriter error, unknown exception caught while trying to create node %s!!\n", (someNode->getType() .c_str()));
        }
        return theElement;
    }

    DOMDocument * createDocument(Node * rootNode){
        if (rootNode == NULL)
            return NULL;
       DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(X("Core"));
       DOMDocument * doc = NULL;
       DOMElement  * docElem = NULL;
       if (impl != NULL)
       {
           try
           {
               doc = impl->createDocument(
                           0,                    // root element namespace URI.
                           X("OpenTracker"),         // root element name
                           0);                   // document type object (DTD).
               docElem = doc->getDocumentElement();
           }catch(...){
               logPrintE("XMLWriter failed to create DOMDocument instance\n");
               
           }
       }

       if (docElem != NULL){
           
           // now create all the nodes starting from the rootNode
           for (unsigned int i=0 ; i < (rootNode->countAllChildren()); i++){
               DOMElement * el = createNode(doc, rootNode->getAnyChild(i));
               if (el != NULL)
                   docElem->appendChild(el);
           }
       }
       return doc;
    }


    void XMLWriter::write(const char * file){
        // create the DOMDocument from the graph
        DOMDocument * doc = createDocument(context.getRootNode());
        
	std::auto_ptr<XMLFormatTarget> myFormatTarget ( new LocalFileFormatTarget( file ));
        writeNode( (DOMNode *)(doc) , myFormatTarget.get());
        

        doc->release();
    };
#  endif //USE_XERCES

#  ifdef USE_TINYXML

    char * header = "<?xml version=\"1.0\" standalone=\"no\" >\n<!-- OpenTracker data version 2.0 TinyXMLWriter -->"/*
                                                                                 "<OpenTracker>"*/;
 
    void insertNode(Node * node, TiXmlElement & parent){
        // create the node with the right name
        TiXmlElement theNode((node->getType()) .c_str());
        // add the attributes to the node
        StringTable & attr = node->getAttributes();
        KeyIterator keys(attr);
        while( keys.hasMoreKeys())
        {
            const std::string & key = keys.nextElement();
            theNode.SetAttribute(key.c_str(), attr.get(key) .c_str());
        }
        // add children to the node
        for (unsigned int i = 0; i < node->countAllChildren() ; i++){
            insertNode(node->getAnyChild(i), theNode);
        }

        // insert the node into the tree
        parent.InsertEndChild(theNode);

    }

    void XMLWriter::write( const char * file )
    {
	TiXmlDocument doc;
        doc.Parse(header);
        if (doc.Error()){
            std::string errorstring = "XMLWriter: ERROR parsing header: ";
            errorstring +=  doc.Value();
            errorstring += doc.ErrorDesc();
            // throw OtException(errorstring);
        }

        TiXmlElement opentracker("OpenTracker");

        Node * someNode = context.getRootNode();
        if (someNode != NULL){

            for (unsigned int i = 0; i < someNode->countAllChildren() ; i++){
                 insertNode(someNode->getAnyChild(i), opentracker);
            }
        }
        
        doc.InsertEndChild(opentracker);


	doc.SaveFile(file);
    }


#  endif //USE_TINYXML
       


#else  // OT_LOCAL_GRAPH

#  ifdef USE_XERCES

    /** internal method that writes out the graph recursively. This may change 
     * and therefore is not part of the interface.
     * @param toWrite the current XML element to write out
     * @param target the output stream to write to
     */
    void writeNode(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode * toWrite, XERCES_CPP_NAMESPACE_QUALIFIER XMLFormatTarget * target );

    XMLWriter::XMLWriter( Context & context_ , unsigned int indent_ ) :
        context( context_ ), indent( indent_ )
    {}

    XMLWriter::~XMLWriter()
    {
  
    }
         
    void XMLWriter::write( const char * file )
    {
	std::auto_ptr<XMLFormatTarget> myFormatTarget ( new LocalFileFormatTarget( file ));
        writeNode( ((DOMNode *)(context.getRootNode()->parent))->getOwnerDocument(), 
                   myFormatTarget.get());
    }
    /*
      void XMLWriter::write( ostream & stream )
      {
      // empty
      }
    */
    void writeNode(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode * toWrite, XERCES_CPP_NAMESPACE_QUALIFIER XMLFormatTarget * target )
    {
        XMLCh * lsCh =  XMLString::transcode("LS");
        DOMImplementation * impl = DOMImplementationRegistry::getDOMImplementation(lsCh);
        DOMWriter * writer = impl->createDOMWriter();
        if (writer->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
            writer->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);
        if (writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
            writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
        try {
            // do the serialization through DOMWriter::writeNode();
            writer->writeNode(target, *toWrite);
        }
        catch (const XMLException& toCatch) {
            char * message = XMLString::transcode(toCatch.getMessage());
            logPrintE("XMLWriter Exception message is: %s\n", message);
            XMLString::release( &message );
        }
        catch (const DOMException& toCatch) {
            char * message = XMLString::transcode(toCatch.msg);
            logPrintE("XMLWriter Exception message is: %s\n", message);
            XMLString::release( &message );
        }
        catch (...) {
            logPrintE("XMLWriter Unexpected Exception \n");
        }
        writer->release();
        XMLString::release( &lsCh );
    }

#  endif //USE_XERCES


#  ifdef USE_TINYXML

    XMLWriter::XMLWriter( Context & context_ , unsigned int indent_ ) :
        context( context_ ), indent( indent_ )
    {}

    XMLWriter::~XMLWriter()
    {
  
    }
         
    void XMLWriter::write( const char * file )
    {
	TiXmlNode* node = (TiXmlNode*)context.getRootNode()->parent;
	TiXmlDocument* doc = node->GetDocument();

	doc->SaveFile(file);
    }

#  endif //USE_TINYXML

#endif //OT_LOCAL_GRAPH

}  // namespace ot



#else
#pragma message(">>> OT_NO_XMLWRITER_SUPPORT")
#endif //OT_NO_XMLWRITER_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of XMLWriter.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
