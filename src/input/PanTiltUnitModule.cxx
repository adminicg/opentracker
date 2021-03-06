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
/** header file for PanTiltUnitSinkSource Node.
 *
 * @author Markus Sareika
 * 
 * $Id: PanTiltUnitModule.h
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <OpenTracker/tool/disable4786.h>
#include <ace/OS.h>
#include <OpenTracker/input/PanTiltUnitSinkSource.h>
#include <OpenTracker/input/PanTiltUnitModule.h>

#ifdef USE_PANTILTUNIT

#include <cstdio>
#if defined (WIN32) || defined (GCC3)
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream>
#endif

#include <Windows.h>
#include <math.h>

#include <opentracker\tool\OT_ACE_Log.h>


namespace ot {
    OT_MODULE_REGISTER_FUNC(PanTiltUnitModule){
        OT_MODULE_REGISTRATION_DEFAULT(PanTiltUnitModule, "PanTiltUnitConfig");
    }


    // Destructor method
    PanTiltUnitModule::~PanTiltUnitModule()
    {
        nodes.clear();
    }

    // This method is called to construct a new Node.
    Node * PanTiltUnitModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if( name.compare("PanTiltUnitSinkSource") == 0 )
        {       
            PanTiltUnitSinkSource * source = new PanTiltUnitSinkSource;
            source->event.addAttribute("float", "fieldOfView", "0");
            source->event.addAttribute("vector<float>", "ptuOri", "0");
            source->event.addAttribute("float", "ptuPan", "0");
            source->event.addAttribute("float", "ptuTilt", "0");
            // read values from xml config file and initialize PTU
            if ( !attributes.get("comPort").empty() ) {
                int comPort = atoi(attributes.get("comPort").c_str());
                if (!source->initComPort(comPort)) 
                    std::cerr << "PTU Serial Port: "<<comPort<<" setup error.\n" << std::endl;
                else std::cerr << "PTU Serial Port: "<<comPort<<" initialized" << std::endl;
            }
            if ( !attributes.get("delayEvent").empty() ) 
                source->delayEvent = atoi(attributes.get("delayEvent").c_str());
			
            source->event.setConfidence( 1.0f );
            nodes.push_back( source );
            logPrintS("Built PanTiltUnitSinkSource node\n");
            return source;
        }
        if( (name.compare("PtuLocation") == 0) ||(name.compare("SetPtuOrientation") == 0) 
			||(name.compare("RelativeInput") == 0) || (name.compare("TopOffset") == 0)) 
        {
            // create just a pass-through node
            NodePort *np = new NodePort();
            np->name = name;
            return np;
        }

        // no node configured 
        return NULL;
    }


    void PanTiltUnitModule::pushEvent()
    {
		
    }

} // namespace ot

#endif //USE_PANTILTUNIT

/* 
 * ------------------------------------------------------------
 *   End of PanTiltUnitModule.cxx
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
