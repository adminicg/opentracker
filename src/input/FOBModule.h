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
/** header file for FOBModule module.
  *
  * @author
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/FOBModule.h,v 1.1 2001/07/13 23:06:50 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section fobmodule FOBModule (Flock of Birds)
 * 
 * User guide description of what it does here
 * list of configuration attributes :
  *
 * @li @c id a unique id to identify the tracker, this the same as the id 
 *           attribute of the @ref intersensesource nodes.
 * @li @c comport the serial port the tracker is connected to. If 0 then
 *        the first one is used or an InterTrax2 connected to the USB port.
 *
 * An example configuration element looks like this :
 * @verbatim
<InterSenseConfig>
    <ISTracker comport="0" id="InterTrax"/>
</InterSenseConfig>@endverbatim
 *
 * Note that this driver has not been fully tested, because we have only
 * InterTrax2 devices in our lab. The ISTracker element is a placeholder for
 * more advanced configuration options in the future.
 */

#ifndef _FOBMODULE_H
#define _FOBMODULE_H

#include "../OpenTracker.h"

/**
 * developer level information and implementation specifics here
 *
 * @author 
 * @ingroup input
 */
class OPENTRACKER_API FOBModule : public ThreadModule, public NodeFactory
{
// Members
protected:

// Methods
protected:
    /** this method is the code executed in its own thread, see
     * ARToolKitModule for an example of such a module. */
    virtual void run();

public:
    /** constructor method. */
    FOBModule() : 
        ThreadModule(), 
        NodeFactory()        
    { 
    };
    /** Destructor method, clears nodes member. */
    virtual ~FOBModule();
    /**
     * initializes the tracker module. For each configured tracker it
     * allocates an ISTracker structure and initializes the tracker.
     * @param attributes StringTable of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes,  ConfigNode * localTree);
    /** This method is called to construct a new Node. It compares
     * name to the InterTraxSource element name, and if it matches
     * creates a new InterTraxSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringTable& attributes);    
    
    virtual void start();
	/**
     * closes InterSense library */
    virtual void close();
    /**
     * pushes events into the tracker tree. Checks all trackers for new data
     * and fires the InterSenseSources, if new data is present.
     */
    virtual void pushState();
};

#endif
