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
/** header file for the WrapperNode class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/WrapperNode.h,v 1.5 2001/04/08 19:31:09 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _WRAPPERNODE_H
#define _WRAPPERNODE_H

#include "../dllinclude.h"

#include "Node.h"

/**
 * This node implements a wrapper node. Its just there to signal the existence
 * of a wrapper tag. It is used in searching for wrapped children.
 * @note write explanation of wrapper tags here
 * @author Gerhard Reitmayr
 * @ingroup core
*/
class OPENTRACKER_API WrapperNode : public Node
{
protected:
    /// the name of the wrapper nodes element element
	string tagname;

    /** sets the DOM element that corresponds to this
     * node. 
     * @param parElement the DOM element parent
     */
	virtual void setParent( DOM_Element & parElement );
// Methods
public:
    /**
     * constructor method */
    WrapperNode()
        : Node()
    {};
    /// destructor method
    virtual ~WrapperNode()
    {};
    /** tests whether the given node is a wrapper node.
     * @return always returns 1 */
    virtual int isWrapperNode()
    {
        return 1;
    };

	/**
     * this method notifies the object that a new event was generated.
     * It is called by an EventGenerator.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( State& event, Node& generator)
	{
		updateObservers( event );
	}

    /** returns the elements name, the so called tag name
     * @return reference to string
     */     
	string & getTagName()
	{
		return tagname;
	}
};

#endif
