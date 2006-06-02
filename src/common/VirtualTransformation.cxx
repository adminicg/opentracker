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
/** source file for VirtualTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "VirtualTransformation.h"

namespace ot {

  // transforms a event.

  Event* VirtualTransformation::transformEvent( Event* event)
  {
    // transform the position of the event
    if( usePos )
      {
        MathUtils::rotateVector( event->getOrientation(), copyA2V(translation, 4),
                                 localEvent.getPosition() );
        localEvent.getPosition()[0] += event->getPosition()[0];
        localEvent.getPosition()[1] += event->getPosition()[1];
        localEvent.getPosition()[2] += event->getPosition()[2];
      }
    else {
      localEvent.getPosition()[0] = event->getPosition()[0];
      localEvent.getPosition()[1] = event->getPosition()[1];
      localEvent.getPosition()[2] = event->getPosition()[2];
    }
    // transform the orientation of the event
    if( useOrient )
      {
        MathUtils::multiplyQuaternion( event->getOrientation(), copyA2V(rotation, 4), localEvent.getOrientation() );
      }
    else {
      localEvent.getOrientation() = event->getOrientation();
    }
    // copy other event fields
    localEvent.copyAllButStdAttr(*event);

    localEvent.getButton() = event->getButton();
    localEvent.getConfidence() = event->getConfidence();
    localEvent.time = event->time;
    return &localEvent;
  }

} // namespace ot
