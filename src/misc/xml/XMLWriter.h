 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for XMLWriter class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/misc/xml/XMLWriter.h,v 1.3 2001/08/20 09:54:52 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _XMLWRITER_H
#define _XMLWRITER_H

#include "../../OpenTracker.h"
#ifdef WIN32
#include <iostream>
#else
#include <iostream.h>
#endif

class DOM_Node;
class XMLFormatter;

/**
 * This class writes out the contents of the Context as a valid XML file. 
 * It can be used to store a changed tracker graph in a file or some other
 * device accessible by an output stream. It can be reused on a context as
 * often as desired and will allways record the current status of the context,
 * as stored in the XML elements.
 * @author Gerhard Reitmayr
 * @ingroup xml
 */ 
class OPENTRACKER_API XMLWriter {

protected:
    /// the context to write out
    Context & context;
    /// amount of indentation per level
    unsigned int indent;
    
    /** internal method that writes out the graph recursively. This may change 
     * and therefore is not part of the interface.
     * @param toWrite the current XML element to write out
     * @param target the output stream to write to
     * @param gFormatter XML formatting object 
     * @param level indentation level to use, this is multiplied with indent to 
     *        get the number of spaces.
     */
    void writeNode(DOM_Node& toWrite, std::ostream & target, XMLFormatter & gFormatter, int level);

public:
    /** constructor, takes a context to work with.
     * @param context_ the context to write out
     * @param indent_ the indentation step to use, default value is 2
     */
    XMLWriter( Context & context_ , unsigned int indent_ = 2);      
    
    virtual ~XMLWriter();
    
    /** writes the tracker graph and configuration to a file given as
     * the parameter. It creates the file if necessary. If the file
     * exists, it will be overwritten by the new configuration data.
     * @param file char array containing the file name. */
    void write( const char * file );
    
    /** writes the tracker graph and configuration to an output stream.
     * @param stream the stream to write to. */
    void write( std::ostream & stream );            

    /** returns the current indentation step
     * @return indentation step */
    unsigned int getIndent()
    {
        return indent;
    }

    /** sets the indentation step
     * @param ind the new value */
    void setIndent( unsigned int ind )
    {
        indent = ind;
    }
};

#endif