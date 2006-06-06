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
/** header file for GPSGarminCompass Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section gpsgarmincompass GPSGarminCompass
 * The GPSGarminCompass node is a simple EventGenerator that outputs GPS 
 * direction data from a magnetic compass of a Garmin receiver like the eTrex. 
 * It parses the Garmin NMEA string $HCHDG. The direction is encoded in the orientation 
 * field as a mathematically positive rotation around the Y axis starting from 
 * the X axis. It is managed by the @ref gpsmodule, see there for
 * more information on how to configure GPS support.
 *
 * An example element looks like this :
 * @verbatim
<GPSGarminCompass/>@endverbatim
 */

#ifndef _GPSGARMINCOMPASS_H
#define _GPSGARMINCOMPASS_H

#include "../OpenTracker.h"
#include "GPSDriver.h"


namespace ot {


/**
 * A very simple EventGenerator node for outputing GPS direction and velocity data.
 * @author Gerhard Reitmayr
 * @ingroup input
 */
class OPENTRACKER_API GPSGarminCompass : public Node, public GPSListener
{
public:

	 /// the event that is posted to the EventObservers
    Event event;
    /// the buffer event for data from the GPS receiver
    Event buffer;

	/** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    virtual void newData( const GPResult * point, const char * line, void * userData );

protected:
	/// protected constructor so it is only accessible by the module
	GPSGarminCompass() {};

	friend class GPSModule;
};

inline void GPSGarminCompass::newData( const GPResult * res, const char * line, void * userData )
{
    assert( userData != NULL );
    if( res->type == GPResult::HCHDG){
        HCHDG * point = (HCHDG *) res;
        GPSModule * module = (GPSModule *)userData;
        if( !module->driver->hasFix() )
            return;
        module->lock();
        buffer.timeStamp();
        float temp[4];
        temp[0] = 0;
        temp[1] = 1;
        temp[2] = 0;
        temp[3] = (float)(point->heading * MathUtils::GradToRad);
        MathUtils::axisAngleToQuaternion( copyA2V(temp, 4), buffer.getOrientation() );
        buffer.getConfidence() = (float)(1 / module->driver->getHdop());
        module->unlock();
    }
}


}  // namespace ot


#endif // !defined(_GPSGARMINCOMPASS_H)
