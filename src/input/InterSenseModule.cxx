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
/** source file for InterSenseModule module.
  *
  * @author Ivan Viola, Matej Mlejnek, Gerhard Reitmayr, Jan Prikryl
  *
  * $Id$
  *
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"
#include "../tool/StringAligner.h"

#include <ace/Log_Msg.h>

#include "InterSenseSource.h"
#include "InterSenseModule.h"

#include <stdio.h>
#include <iostream>
#include <isense.h>

//using namespace std;

namespace ot {

struct ISTracker {
    ISD_TRACKER_HANDLE handle;
    ISD_TRACKER_INFO_TYPE info;
    ISD_TRACKER_DATA_TYPE data;
    std::string id;
    int comport;
    NodeVector sources;
};

// Destructor method
InterSenseModule::~InterSenseModule()
{
}

// initializes trackers
void InterSenseModule::init(StringTable& attributes, ConfigNode * localTree)
{
    Module::init( attributes, localTree );
    bool verbose = TRUE;
    for( unsigned i = 0; i < localTree->countChildren(); i++ )
    {
        ConfigNode * trConfig = (ConfigNode *)localTree->getChild(i);
        StringTable & trackerAttrib = trConfig->getAttributes();
        const std::string & id = trackerAttrib.get("id");
        int comport = 0;
        int num = sscanf(trackerAttrib.get("comport").c_str(), " %i", &comport );
        if( num == 0 ){            
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting serial port number !\n")));
            comport = 0;
        }
		ISTrackerVector::iterator it;
        for(it = trackers.begin(); it != trackers.end(); it ++ )
        {
            if( (*it)->id.compare( id ) == 0 && (*it)->comport == comport )
                break;
        }
        if(it == trackers.end())  // we got a truly new tracker
        {
            ISD_TRACKER_HANDLE handle = ISD_OpenTracker( 0, comport, FALSE, verbose );
            if( handle <= 0 )
            {                
				ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Failed to open tracker %d\n"), id));
            } 
            else {
                ISTracker * tracker = new ISTracker;
                tracker->id = id;
                tracker->comport = comport;
                tracker->handle = handle;
                Bool res;
                res = ISD_GetTrackerConfig( tracker->handle, &tracker->info , FALSE);
                if( res == FALSE )
                {                    
					ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Failed to get tracker config for %d\n"), id));
                }
				/* InterTrax does not support quaternions */
                if (tracker->info.TrackerType != ISD_INTERTRAX_SERIES ) 
                {
                    /* Set up each station for Precision class of trackers.
                       Ensure the orientation measure of the tracker stations is
                       quaternions.  Ensure that button values are returned. */
                    ISD_STATION_INFO_TYPE station;
                    for( int j = 1; j <= ISD_MAX_STATIONS; j++ )
                    {   
                        int error = 0;
                        if( !ISD_GetStationConfig( tracker->handle, &station, j, FALSE ) )
                        {
                            error = 1;                
                        }
                        else {
                            if( station.State == TRUE )
                            {
                                station.AngleFormat = ISD_QUATERNION;
                                station.GetInputs = TRUE;
                                if( !ISD_SetStationConfig( tracker->handle, 
                                    &station, j, FALSE ) )
                                    error = 2;
                            }
                        }               
                        if( error )
                        {
							ACE_DEBUG((LM_WARNING, ACE_TEXT("ot:WARNING: InterSenseModule cannot %s state for tracker %d station %d \n"), ((error == 1 ) ? "get " : "set "), id, j));
							ACE_DEBUG((LM_WARNING, ACE_TEXT("ot:- orientation measure may not be quaternion.\n")));
                        }                       
                    }
                }   // setup not intertrax
                trackers.push_back( tracker );                
				ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Configured tracker %d of %d\n"), id, tracker->info.TrackerType));
            }       // open tracker ok
        }           // got a new tracker
        else {      // some conflict with another tracker            
			ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tracker %d at port %d conflicts with with %d\n"), id, comport, (*it)->id));
        }
    }               // all ConfigNodes
}

// This method is called to construct a new Node.
Node * InterSenseModule::createNode( const std::string& name, StringTable& attributes)
{
    if( name.compare("InterSenseSource") == 0 )
    {       
        const std::string & id = attributes.get( "id" );
	ISTrackerVector::iterator it;
        for(it = trackers.begin(); it != trackers.end(); it ++ )
        {
            if( (*it)->id.compare( id ) == 0 )
                break;
        }
        if( it != trackers.end())
        {
            int station;
            int num = sscanf(attributes.get("station").c_str(), " %i", &station );
            if( num == 0 ){                
				ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Error in converting station number !\n")));
                return NULL;
            }
            if( station < 0 || station >= ISD_MAX_STATIONS )
            {
				ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Station number out of range [0,%d] !\n"), ISD_MAX_STATIONS-1));
                return NULL;
            }
            InterSenseSource * source = new InterSenseSource( station );
            (*it)->sources.push_back( source );            
			ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Build InterSenseSource node\n")));
            return source;
        }
        else {            
			ACE_DEBUG((LM_INFO, ACE_TEXT("ot:No tracker %d configured !\n"), id));
        }
    }
    return NULL;
}

void InterSenseModule::start()
{
    ISTrackerVector::iterator it;
    for(it = trackers.begin(); it != trackers.end(); it ++ )
    {
        if( (*it)->info.TrackerType == ISD_INTERTRAX_SERIES || 
            (*it)->info.TrackerModel == ISD_IS300 ||
            (*it)->info.TrackerModel == ISD_ICUBE2 ||
            (*it)->info.TrackerModel == ISD_ICUBE2_PRO )
        {
            for( int j = 1; j <= ISD_MAX_STATIONS; j++ )                
                ISD_ResetHeading((*it)->handle, j);
        }
    }
}

// closes InterSense library
void InterSenseModule::close()
{
    for(ISTrackerVector::iterator it = trackers.begin(); it != trackers.end(); it ++ )
    {
        ISD_CloseTracker( (*it)->handle );
    }    
}

// pushes events into the tracker tree.
void InterSenseModule::pushState()
{
    for(ISTrackerVector::iterator it = trackers.begin(); it != trackers.end(); it ++ )
    {
        if((*it)->sources.size() > 0 )
        {
            ISTracker * tracker = *it;
            ISD_GetData( tracker->handle, &tracker->data );
            for( NodeVector::iterator si = tracker->sources.begin(); si != tracker->sources.end(); si++ )
            {
                InterSenseSource * source = ( InterSenseSource * )*si;
                ISD_STATION_STATE_TYPE * data = &tracker->data.Station[source->station];
                if( tracker->info.TrackerType == ISD_INTERTRAX_SERIES )
                {
                    float quat[4];
                    MathUtils::eulerToQuaternion(- data->Orientation[0] * MathUtils::GradToRad,
		                             data->Orientation[1] * MathUtils::GradToRad,
									 - data->Orientation[2] * MathUtils::GradToRad,
                                     MathUtils::YXZ,
                                     quat);
                    if( quat[0] != source->state.orientation[0] || 
                        quat[1] != source->state.orientation[1] ||
                        quat[2] != source->state.orientation[2] ||
                        quat[3] != source->state.orientation[3] )
                    {
                        source->state.orientation[0] = quat[0];
                        source->state.orientation[1] = quat[1];
                        source->state.orientation[2] = quat[2];
                        source->state.orientation[3] = quat[3];
                        source->state.timeStamp();
                        source->updateObservers( source->state );
                    }
                }
                else {
                    int changed = 0;
                    if( data->Orientation[0] != source->state.orientation[0] || 
                        data->Orientation[1] != source->state.orientation[1] ||
                        data->Orientation[2] != source->state.orientation[2] ||
                        data->Orientation[3] != -source->state.orientation[3] )
                    {
                        changed = 1;
                        source->state.orientation[0] = data->Orientation[0];
                        source->state.orientation[1] = data->Orientation[1];
                        source->state.orientation[2] = data->Orientation[2];
                        source->state.orientation[3] = -data->Orientation[3];
                    }
                    if( data->Position[0] != source->state.position[0] || 
                        data->Position[1] != source->state.position[1] ||
                        data->Position[2] != source->state.position[2] )
                    {
                        changed = 1;
                        source->state.position[0] = data->Position[0];
                        source->state.position[1] = data->Position[1];
                        source->state.position[2] = data->Position[2];
                    }
                    unsigned short button = 0;
                    for( int i = 0; i < ISD_MAX_BUTTONS; i++ )
                    {
                        button |= data->ButtonState[i];
                    }
                    if( button != source->state.button )
                    {
                        changed = 1;
                        source->state.button = button;
                    }
                    if( changed == 1 )
                    {
                        source->state.timeStamp();
                        source->updateObservers( source->state );
                    }
                }       
            } // for all sinks
            
        } 
    } // for all trackers       
}

} // namespace ot
