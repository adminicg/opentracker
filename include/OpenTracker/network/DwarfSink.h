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
/** header file for DwarfSink Node.
 *
 * @author Gerhard Reitmayr, Christian Sandor, Martin Bauer
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section dwarfsink DwarfSink

 * An example element looks like this :
 * @verbatim
 <DwarfSink frequency="10" offset="5" />@endverbatim
*/

#ifndef _DWARFSINK_H
#define _DWARFSINK_H

#include "../OpenTracker.h"

#ifdef USE_DWARF

namespace ot {

    /**
     * This class implements a simple source that is fired by its module in
     * regular intervals and updates any EventObservers.
     * @ingroup network
     * @author Gerhard Reitmayr, Christian Sandor, Martin Bauer
     */
    class OPENTRACKER_API DwarfSink : public Node
    {
        // Members
    public:
        /// new event arrived ?
        bool changed;
        /// name of associated dwarf ability
        std::string name;

        // Methods
    protected:
        /** simple constructor, sets members to initial values
         * @param frequency_ initial value for member frequency
         * @param offset_ initial value for member offset */
        DwarfSink( const std::string & name_) :
            Node(),
            changed( false ),
            name( name_ )
            {}

    public:
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        int isEventGenerator()
        {
            return 1;
        }

        /**
         * this method notifies the object that a new event was generated.
         * @param event reference to the new event.
         * @param generator reference to the EventGenerator object that
         *        notified the EventObserver.
         */
        virtual void onEventGenerated( Event& event, Node& generator)
        {
            changed = true;
            updateObservers( event );
        }

        friend class DwarfModule;
    };

} // namespace ot

#endif // USE_DWARF

#endif

/* 
 * ------------------------------------------------------------
 *   End of DwarfSink.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
