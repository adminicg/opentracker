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
/** header file for SpeechModule module.
  *
  * @author Reinhard Steiner
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/SpeechModule.h,v 1.1 2002/12/10 17:23:44 kaufmann Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section speechmodule SpeechModule
 * The SpeechModule provides and drives @ref speechsource nodes that generate
 * sr events.
 */

#ifndef _SPEECHMODULE_H
#define _SPEECHMODULE_H

#include "../../config.h"
#ifdef USE_SAPISPEECH


#include "../dllinclude.h"

#include "../Input/SpeechDef.h"
#include "../Input/SpeechVoiceModule.h"

#include "../Core/Module.h"
#include "../Core/NodeFactory.h"

typedef std::vector<Node*> NodeVector;

/**
 * The module and factory to drive the speech source nodes. It constructs
 * SpeechSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Reinhard Steiner
 * @ingroup input
 */
class OPENTRACKER_API SpeechModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of SpeechSource nodes in the tree
    NodeVector m_Nodes;

    /// next node id/rule name
    DWORD m_NextNodeId;
    
    /// Speech Core (SR) component
    CSpeechCore *m_SpeechCore;

    /// Speech Voice (TTS) component
    SpeechVoiceModule *m_Voice;

    /// Configuration Tree for following SpeechSources
    ConfigNode *m_ConfigTree;

    /// vector of all nodes that should push a speech event a second time
    NodeVector m_Push2Nodes;


// Methods
public:
    /** constructor method. */
    SpeechModule() : Module(), NodeFactory()
    {
      m_NextNodeId = 1;
      m_SpeechCore = 0;
      m_Voice = 0;
    };
    
    /** Destructor method, clears nodes member. */
    virtual ~SpeechModule();


    /** This method is called to construct a new Node. It compares
     * name to the SpeechSource element name, and if it matches
     * creates a new SpeechSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node* createNode(const std::string& name, StringTable& attributes);


    /**
     * initializes the tracker module. This class provides an implementation
     * that sets the initialization flag to true. Subclasses should call this
     * method if they override this method. It takes the attributes of the
     * element configuring this module and a local tree consisting of the
     * children of the element. This tree must be build of Nodes.
     * @param attributes StringTable of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes, ConfigNode *localTree);

    /**
     * closes the module. A place for cleanup code etc.
     * This class provides an empty implementation for subclasses not doing
     * anything here. */
    virtual void close();

    /**
     * pushes state information into the tracker tree. It enables the module
     * to push new data into the tree by updating EventGenerator nodes and
     * thereby triggering an event.
     */
    virtual void pushState();


    /**
     * Speak a sentence with the default voice
     */
    void Speak(const char *p_Sentence, bool p_Async = true)
    {
      assert(m_Voice);
      m_Voice->Speak(p_Sentence, p_Async);
    }

    /**
     * Get a command string from a command id
     * @return true if successfull, false in case of a failure
     */
    bool GetCommand(DWORD p_CommandId, DWORD p_SpeechSetId, std::string &p_Command);
};


#endif //ifdef USE_SAPISPEECH

#endif
