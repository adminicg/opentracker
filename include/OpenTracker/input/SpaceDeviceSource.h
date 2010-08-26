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
/** header file for SpaceDeviceSource Node.
  *
  * @author Michael W�gerbauer, Mathis Csisinko
  *
  * $Id: SpaceDeviceSource.h 1260 2006-07-13 09:46:25Z sareika $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section SpaceDeviceSource SpaceDeviceSource
 * The SpaceDeviceSource node is a EventGenerator 
 * 
 * see also the @ref SpaceDeviceModule. 
 *
 * An example element looks like this :
 * @verbatim
 <ConsoleSink comment="spaceDevice">
 <EventSink tracking="spaceDevice">
		<SpaceDeviceSource/>
 </EventSink> 
 </ConsoleSink>
	
@endverbatim
 */

#ifndef _SPACEDEVICESOURCE_H
#define _SPACEDEVICESOURCE_H

#include "../OpenTracker.h"
#ifdef USE_SPACEDEVICE
#ifdef WIN32

#ifndef SPACEDEVICE_DEPRECATED
#include <windows.h>
#else
#include "../common/ConsoleSource.h"

#define OTCOM_NONE				0
#define OTCOM_RESETPOSITION		1
#define OTCOM_RESETROTATION		2
#define OTCOM_TOGGLE_ROTATECAMERAAXIS  3
#endif




namespace ot {

/**
 * This class implements a simple source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @ingroup input
 */
class OPENTRACKER_API SpaceDeviceSource : public Node
{
#ifndef SPACEDEVICE_DEPRECATED
	friend class SpaceDeviceModule;
#else
public: 
    /// the state that is posted to the EventObservers
    Event event;

// Methods
public:
    /** simple constructor, sets members to initial values */
    SpaceDeviceSource() : Node()
    {};
        
    /** pushes event down the line. */
    void push()
    {
        event.timeStamp();
        updateObservers( event );
    }
#endif

protected:
#ifndef SPACEDEVICE_DEPRECATED
    /** constructor method
     * @param hDevice handle to the input device
     */
    SpaceDeviceSource(HANDLE hDevice);

    const HANDLE getDevice() const { return hDevice; }

    void setButtonEvent(unsigned short);

    void setPosition(float,float,float);

	void setOrientation(float,float,float);

    bool calcEvent();

    Event & getEvent() { return event; }
#endif

	/** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

#ifndef SPACEDEVICE_DEPRECATED
private:
    bool modified;
    Event event;
    HANDLE hDevice;
#else
	// This Event holds the temporary data
    Event tmpEvent;

    // A flag to indicate whether tmpState was changed during processing
    int changed;

	friend class SpaceDeviceModule;
#endif
};

}  // namespace ot

#endif
#endif //USE_SPACEDEVICE

#endif //_SPACEDEVICESOURCE_H
