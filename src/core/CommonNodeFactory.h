 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for CommonNodeFactory class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/CommonNodeFactory.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _COMMONNODEFACTORY_H
#define _COMMONNODEFACTORY_H

#include <string>
#include <map>

#include "Node.h"
#include "NodeFactory.h"
//#include "MaxConfidenceFilter.h"
#include "Transformation.h"
#include "VirtualTransformation.h"
#include "EventQueueNode.h"

/**
 * maps a string to another string. Mostly used to map element attributes
 * to values.
 */
typedef std::map<string, string> StringMap;

/**
 * This class provides the general node types found in the configuration files.
 * The following nodes are build by this factory :
 * \li Transformation, for all three types of transformations
 * \li VirtualTransformation, again all three types of transformations
 * \li EventQueueNode
 * \li MaxConfidenceFilter ( this is not implemented )
 * @author Gerhard Reitmayr
 */
class CommonNodeFactory : public NodeFactory
{

protected:
    /** builds a MaxConfidenceFilter node.
     * @param attributes reference to StringMap of elements attribute values
     * @return pointer to MaxConfidenceFilter created on the heap */
//    MaxConfidenceFilter* buildMaxConfidenceFilter( StringMap& attributes);
    /** builds a Transformation node.
     * @param attributes reference to StringMap of elements attribute values
     * @return pointer to Transformation created on the heap */
     Transformation * buildTransformation( StringMap& attributes);
    /** builds a VirtualTransformation node.
     * @param attributes reference to StringMap of elements attribute values
     * @return pointer to VirtualTransformation created on the heap */
    VirtualTransformation * buildVirtualTransformation( StringMap& attributes);
    /** builds a EventQueueNode node.
     * @param attributes reference to StringMap of elements attribute values
     * @return pointer to EventQueueNode created on the heap */
    EventQueueNode * buildEventQueue( StringMap& attributes);

public:
    /** constructor method */
    CommonNodeFactory()
    {};
    /** creates the described node types.
     * @param name reference to string containing element name
     * @param attributes reference to StringMap of elements attribute values
     * @return new Node or NULL, if element name was not recognized */
    virtual Node * createNode( string& name,  StringMap& attributes);
};

#endif
