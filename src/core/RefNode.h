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
/** The header file for the RefNode class.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section refnode Ref
 * A Ref element allows to use the output of a node in another place in the
 * data flow graph. Every element can have a DEF attribute specified that
 * contains a unique id. This id can then be referenced in a Ref element
 * specifing the node the Ref element should mimik. The Ref node will then
 * resemble the referenced node in any way. 
 *
 * However it cannot have any children, as the referenced node would have no
 * information about these children. The Ref node is simply a way to reuse
 * the output of a node somewhere else. It has the following attribute :
 * @li @c USE unique id of the referenced node, this id must exist !
 *
 * An example element looks like this :
 * @verbatim
<Ref USE="id"/>@endverbatim
 */

#ifndef _REFNODE_H
#define _REFNODE_H

#include "../dllinclude.h"

#include "Node.h"

/**
 * A Reference Node contains the reference to another node and its subtree.
 * It can only occur as a leaf node. However it reflects the interface of
 * the referenced node and also updates it's parents, whenever the referenced
 * node generates an event.
 * @author Gerhard Reitmayr
 * @ingroup core
 */

namespace ot {

class OPENTRACKER_API RefNode : public Node
{

protected:
	/** pointer to the referenced node */
	Node * reference;
 
   /**
    * basic constructor.
    */
    RefNode( Node * ref ) :
	  Node(),
      reference( ref )		
    {
		reference->addReference( this );  
	}

public:   
   /**
    * basic destructor.
    */
    virtual ~RefNode()
    {
		reference->removeReference( this );
	}
	
    /** returns the referenced node
     * @returns pointer to the referenced node.
     */
    Node * getReferenced()
    {
        return reference;
    }
    
	// begin EventGenerator & EventObserver interface

	/**
     * this method notifies the object that a new event was generated.
     * It is called by an EventGenerator.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( State& event,
                                   Node& generator)
	{
		updateObservers( event );
	}

    /**
     * tests for EventGenerator interface being implemented. This has to
     * be overriden in classes that subclass EventGenerator. Due to
     * inheritance constraints it cannot be done automatically.
     * @returns a pointer to the EventGenerator interface, or NULL if it
     *   is not implemented
     */
    virtual int isEventGenerator()
    {
        return reference->isEventGenerator();
    }

	// end EventGenerator interface

	// begin EventQueue interface

    /**
     * tests for EventQueue interface being present. This has to be overriden in
     * classes that subclass EventQueue. See isEventGenerator for details.
     * @returns a pointer to the EventQueue interface, or NULL if it is not
     *       implemented.
     */
    virtual int isEventQueue()
    {
        return reference->isEventQueue();
    }
	/**
     * returns a stored event by index. The index starts with 0 for the
     * latest event and goes back in time with increasing index.
     * @param index index of the desired event, if no index is given the
     *        latest is returned.
     * @return reference to the value of the event. */
    virtual State& getEvent(unsigned int index = 0)
	{
		return reference->getEvent( index );
	}
    /**
     * returns the event closest to a given point in time.
     * @param time the point in time in milliseconds since 1.1.1970
     * @return reference to the value of the event. */
    virtual State& getEventNearTime(double time)
	{
		return reference->getEventNearTime( time );
	}
    /**
     * returns the number of stored events.
     * @return number of stored events. */
    virtual unsigned int getSize()
	{
		return reference->getSize();
	}

	// end EventQueue interface

	// begin TimeDependend interface

    /**
     * tests for TimeDependend interface being present. This has to be overriden
     * in classes that subclass TimeDependend. See isEventGenerator for details.
     * @returns a pointer to the TimeDependend interface, or NULL if it is not
     *      implemented.
     */
    virtual int isTimeDependend()
    {
        return reference->isTimeDependend();
    }

	/**
     * returns the state of the time dependend function at a given
     * point in time.
     * @param time point in time in milliseconds since 1.1.1970
     * @return reference to the state value */
    virtual State& getStateAtTime(double time)
	{
		return reference->getStateAtTime( time );
	}

	// end TimeDependend interface
    friend class ConfigurationParser;
};

} // namespace ot {

#endif
