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
/** header file for SpeechSource Node.
  *
  * @author Reinhard Steiner
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/SpeechSource.h,v 1.2 2002/12/12 17:31:02 kaufmann Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section speechsource SpeechSource
 * The SpeechSource node pushes speech events. A speech event 
 * is coded into the translation field. x holds the speech command id
 * as configured in the opentracker xml config file. y holds the 
 * speech set id. z is only used for help purposes.
 * This node always pushes 2 events, one speech "start" event,
 * and one speech "end" event. This is used for button press/release
 * actions. The start is coded as button 0 pressed, end as button 0
 * released.
 * The source only fires events when a command is successfully
 * recognised.
 */

#ifndef _SPEECHSOURCE_H
#define _SPEECHSOURCE_H


#include "../../config.h"
#ifdef USE_SAPISPEECH


#include "../dllinclude.h"

#include "../Input/SpeechDef.h"
#include "../Input/SpeechCore.h"
#include "../Input/SpeechSet.h"
#include "../Input/SpeechVoice.h"

#include "../Core/Node.h"

/**
 * This class implements a speech source that is fired by its module
 * when any speech recognition occured.
 * @ingroup input
 * @author Reinhard Steiner
 */
class OPENTRACKER_API SpeechSource : public Node
{
// Members
public:
    /// speech module parent object
    SpeechModule *m_SpeechModule;

    /// speech set of this source node
    CSpeechSet *m_SpeechSet;

    /// the state that is posted to the EventObservers
    State state;

    /// the last command string
    std::string command;

// Methods
protected:
    /** simple constructor, sets members to initial values
     * @param p_SpeechModule the parent module (node factory)
     * @param p_SpeechSet the speech set of this node
     */
    SpeechSource(SpeechModule *p_SpeechModule, CSpeechSet *p_SpeechSet) : 
      Node(),
      m_SpeechModule(p_SpeechModule),
	    m_SpeechSet(p_SpeechSet)
    {
      assert(m_SpeechModule); // must be a valid pointer
      assert(m_SpeechSet);    // must be a valid pointer
    }
public:            
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1
     */
    int isEventGenerator()
    {
      return 1;
    }

    /** pushes event down the line. Needed to access protected
     * updateObservers method in EventGenerator
     */
    bool push()
    {
      if(m_SpeechSet->GetReco(command))
      {
        //m_SpeechModule->Speak(command.c_str(), true); // TEST only

        state.button = 0x0001;

        // store command id at position[0], x coordinate
        state.position[0] = m_SpeechSet->GetCommandId(command.c_str());
        // store speech set id at position[1], y coordinate
        state.position[1] = m_SpeechSet->GetId();
        // change the z position, so the event will continue
        state.position[2] = state.position[2] + 1;
        
        printf("SR: SpeechSource::push, Command = '%s', CommandId = %i\n", command.c_str(), (int)state.position[0]);
        state.timeStamp();
        updateObservers(state);

        return(true);
      }
      return(false);
    }

    void push2()
    { // Fire a second time with no button pressed
      //printf("SpeechSource::push2\n");

      state.button = 0x0000;
      
      // store command id at position[0], x coordinate
      state.position[0] = m_SpeechSet->GetCommandId(command.c_str());
      // store speech set id at position[1], y coordinate
      state.position[1] = m_SpeechSet->GetId();
      // change the z position, so the event will continue
      state.position[2] = state.position[2] + 1;
      
      //printf("SR: SpeechSource::push2, Command = '%s', CommandId = %i\n", command.c_str(), (int)state.position[0]);
      state.timeStamp();
      updateObservers(state);
    }

    friend class SpeechModule;
};

#endif //ifdef USE_SAPISPEECH

#endif
