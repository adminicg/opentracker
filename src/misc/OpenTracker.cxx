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
/** Source file for OpenTracker. It contains global static definitions and
  * global functions etc. 
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/misc/OpenTracker.cxx,v 1.14 2001/08/04 18:07:31 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <string>

#include <ace/ACE.h>

#include "../dllinclude.h"

#include "../core/State.h"
#include "../core/Context.h"
#include "../common/CommonNodeFactory.h"
#include "../core/TestModule.h"
#include "../common/ConsoleModule.h"
#include "../network/NetworkSinkModule.h"
#include "../network/NetworkSourceModule.h"
#include "../input/InterSenseModule.h"
#include "../common/TimeModule.h"
#include "../common/FileModule.h"
#include "../input/ParButtonModule.h"

// these modules depend on compile options
#include "../input/InterTraxModule.h"
#include "../input/ARToolKitModule.h"
#include "../input/CyberMouseModule.h"
#include "../input/WacomGraphireModule.h"

// DLL main function

#ifdef WIN32

// local object manager
ACE_Object_Manager * objectManager;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch( ul_reason_for_call )
    {
        case DLL_PROCESS_ATTACH :
            objectManager = new ACE_Object_Manager;
            break;
        case DLL_PROCESS_DETACH :
            delete objectManager;
    }
    return TRUE;
}
#endif

// initializes context

void OPENTRACKER_API initializeContext( Context & context )
{
    // Instance the default modules and add to factory and parser
    CommonNodeFactory * common = new CommonNodeFactory;
    context.addFactory( *common );

    TestModule * test = new TestModule;
    context.addFactory( *test );
    // actually it doesn't have a configuration element
    context.addModule( "TestConfig", *test );

    NetworkSinkModule * networksink = new NetworkSinkModule;
    context.addFactory( *networksink );
    context.addModule( "NetworkSinkConfig", *networksink );
	
    ConsoleModule * console = new ConsoleModule ;
    context.addFactory( *console );
    context.addModule( "ConsoleConfig", *console );

    NetworkSourceModule * network = new NetworkSourceModule;
    context.addFactory( * network );
    context.addModule( "NetworkSourceConfig", *network );    

#ifdef USE_ARTOOLKIT
    ARToolKitModule * artool = new ARToolKitModule;
    context.addFactory( * artool );
    context.addModule( "ARToolKitConfig", *artool );
#endif

#ifdef USE_INTERTRAX
    InterTraxModule * intertrax = new InterTraxModule;
    context.addFactory( * intertrax );
    context.addModule( "InterTraxConfig", * intertrax );
#endif

#ifdef USE_WACOMGRAPHIRE
    WacomGraphireModule * wacom = new WacomGraphireModule;
    context.addFactory( * wacom );
    context.addModule( "WacomGraphireConfig", * wacom );
#endif

#ifdef USE_CYBERMOUSE
    CyberMouseModule * cmouse = new CyberMouseModule;
    context.addFactory( * cmouse );
    context.addModule( "CyberMouseConfig", * cmouse );
#endif	

    InterSenseModule * intersense = new InterSenseModule;
    context.addFactory( * intersense );
    context.addModule( "InterSenseConfig", * intersense );
    
    TimeModule * time = new TimeModule();
    context.addModule( "TimeConfig", * time );
    
    FileModule * file = new FileModule();
    context.addFactory( * file );
    context.addModule( "FileConfig", * file );
    
    ParButtonModule * parbutton = new ParButtonModule();
    context.addFactory( *parbutton );
    context.addModule( "ParButtonConfig", *parbutton );
}
