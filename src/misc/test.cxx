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
/** source file containing the main function and test code 
  * for the test program use.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/misc/test.cxx,v 1.1 2001/04/29 16:33:46 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include <OpenTracker.h>

#ifdef WIN32
#include <iostream>    // new IOStream for VisualC++
#else
#include <iostream.h>
#endif

#include <common/CallbackModule.h>

CallbackFunction testCB;

/**
 * The main function for the test program. It expects a
 * filename as argument, tries to parse the configuration file
 * and starts the main loop, if successful
 */
int main(int argc, char **argv)
{
    if( argc != 2 )
    {
        cout << "Usage : " << argv[0] << " configfile" << endl;
        return 1;
    }

    cout << "OpenTracker Test program ...\n";

    // important parts of the system
    // get a context, the default modules and factories are
    // added allready.
    Context context;
    initializeContext( context );
    CallbackModule  cbModule;
    context.addFactory( cbModule );
    context.addModule( "CallbackConfig", cbModule );
    
    cout << "Context established." << endl;

    // parse the configuration file, builds the tracker tree
    context.parseConfiguration( argv[1] );
    cbModule.setCallback( "test1", &testCB );
    
    cout << "Parsing complete." << endl << endl << "Press any return to start mainloop !" << endl;    
     
    cin.get();
    
    // initializes the modules and starts the tracker main loop
    context.run();
    return 0;
}

void testCB( const State & event ){
    cout << "Got event !\n";
}
