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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for CommonNodeFactory class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/CommonNodeFactory.h,v 1.6 2001/04/01 13:22:40 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _COMMONNODEFACTORY_H
#define _COMMONNODEFACTORY_H

#include "../OpenTracker.h"
#include "Transformation.h"
#include "VirtualTransformation.h"
#include "EventQueueNode.h"

/**
 * maps a string to another string. Mostly used to map element attributes
 * to values.
 */
typedef map<string, string> StringMap;

/**
 * This class provides the general node types found in the configuration files.
 * The following nodes are build by this factory :
 * \li Transformation, for all three types of transformations
 * \li VirtualTransformation, again all three types of transformations
 * \li EventQueueNode
 * \li MergeNode
 * @author Gerhard Reitmayr
 */
class CommonNodeFactory : public NodeFactory
{

// members 
protected:
    /// stores names of known WrapperNodes, to make instantiation more generic
    vector<string> wrapperNodes;

protected:
    
    /** parses a string for a three component vector, returns int value
     * indicating an error. If an error occurs, the val values are not changed.
     * @param line string to parse
     * @param val pointer to three floats
     * @return 0 if ok, other value if error occured
     */
    static int parseVector( const string & line, float * val );
    /** parses a string for a rotation, where the type is defined by
     * a second string, returns int value indicating an error. If an error occurs, 
     * the val values are not changed.
     * @param line string to parse
     * @param type string defining type of rotation (quaternion|matrix|axisangle|euler)
     * @param val pointer to four floats, rotation will be stored in quaternion format
     * @return 0 if ok, other value if error occured
     */
    static int parseRotation( const string & line, const string & type, float * val );

    /** builds a EventQueueNode node.
     * @param attributes reference to StringMap of elements attribute values
     * @return pointer to EventQueueNode created on the heap */
    EventQueueNode * buildEventQueue( StringMap& attributes);

public:
    /** constructor method. Initalizes the wrapperNodes array.*/
    CommonNodeFactory();
    /** creates the described node types.
     * @param name reference to string containing element name
     * @param attributes reference to StringMap of elements attribute values
     * @return new Node or NULL, if element name was not recognized */
    virtual Node * createNode( string& name,  StringMap& attributes);
};

#endif
