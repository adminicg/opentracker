 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for InterTraxSource Node.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/Attic/InterTraxSource.h,v 1.5 2001/03/05 17:53:05 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section intertraxsource InterTraxSource
 * The InterTraxSource node is a simple EventGenerator that fires in fixed intervalls
 * standard events. It has the following element :
 * @li @c state, stores information of current orientation
 *
 * An example element looks like this :
 * @verbatim
<InterTraxSource/>@endverbatim
 */

#ifndef _INTERTRAXSOURCE_H
#define _INTERTRAXSOURCE_H

#include "../OpenTracker.h"

#ifdef USE_INTERTRAX

/**
 * This class implements a simple source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Ivan Viola, Matej Mlejnek
 */
class InterTraxSource : public Node, public EventGenerator
{
// Members
public: 
    /// the state that is posted to the EventObservers
    State state;

// Methods
public:
    /** simple constructor, sets members to initial values */
    InterTraxSource() : 
        Node(),
	    EventGenerator()
    {};
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return this always.
     * @return always this */
    EventGenerator * isEventGenerator()
    {
        return this;
    }

    /** pushes event down the line. Needed to access protected
     * updateObservers method in EventGenerator */
    void push()
    {
        updateObservers( state );
    }
};

#endif

#endif