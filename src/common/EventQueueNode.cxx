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
/** source file for class EventQueueNode.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/EventQueueNode.cxx,v 1.5 2002/09/26 13:56:25 bornik Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "EventQueueNode.h"

#if defined (WIN32) || defined (GCC3)
#include <iostream>    // new IOStream for VisualC++
#else
#include <iostream.h>
#endif

// this method is called by the EventGenerator to update it's observers

void EventQueueNode::onEventGenerated( State& event, Node& generator)
{
    if( queue.size() == length )
    {
        queue.pop_front();
    }
    insertAtTime( event );
    updateObservers( event );
}
