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
/** source file for FileModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/FileModule.cxx,v 1.8 2002/09/26 13:56:25 bornik Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "FileModule.h"
#include "FileSink.h"
#include "FileSource.h"
#include <stdio.h>

#if defined (WIN32) || (GCC3)
#include <iostream>
#else
#include <iostream.h>
#endif

using namespace std;

// Destructor method, clears nodes member

FileModule::~FileModule()
{
    // clear containers
    nodes.clear();
	files.clear();
}

// This method is called to construct a new Node

Node * FileModule::createNode( const string& name, StringTable& attributes)
{
    if( name.compare("FileSink") == 0 )
    {
        string id = attributes.get("file");
        int station;
        if( sscanf( attributes.get("station").c_str()," %i", &station ) == 0 ) 
            station = 0;                    
        // search for File
        map<string,File *>::iterator it = files.find( id );
        File * file;
        if( it != files.end()) // found one or create a new one ? 
        {
            file = (*it).second;
            
        } else // create a new one
        {
            file = new File( id, File::OUT );
            files[id] = file;        
        }
        if( file->mode == File::OUT ) // test for right direction and add to store
        {            
            NodeVector & vector = nodes[id];
            NodeVector::iterator nodeIt = vector.begin();             
            FileSink * sink = new FileSink(*file, station );
            vector.push_back( sink );
            cout << "Built FileSink node writting into " << id << " with station " 
                 << station << endl;       
            return sink;
        } 
        cout << "FileSink referencing input file " << id << endl;

    } else if( name.compare("FileSource") == 0 )
    {
		string id = attributes.get("file");
        int station;
        if( sscanf( attributes.get("station").c_str()," %i", &station ) == 0 ) 
            station = 0;                    
        // search for File
        map<string,File *>::iterator it = files.find( id );
        File * file;
        if( it != files.end()) // found one
        {
            file = (*it).second;

        } else // create a new one
        {
            file = new File( id, File::IN );
            files[id] = file;
        }
        if( file->mode == File::IN ) // test for right direction and add to store
        {
            NodeVector & vector = nodes[id];
            NodeVector::iterator nodeIt = vector.begin();    
            for( ; nodeIt != vector.end(); nodeIt++ )  // test for another node with the same station
                if(((FileSource *)(*nodeIt))->station == station )
                    break;
            if( nodeIt != vector.end())
            {
                cout << "Allready another FileSource node for station " << station << endl;
                return NULL;
            }
            FileSource * source = new FileSource( station );
            vector.push_back( source );
            cout << "Built FileSource node reading from " << id << " with station " 
                 << station << endl;       
            return source;
        } 
        cout << "FileSource referencing output file " << id << endl;
    }
    return NULL;
}

// reads from the input files and fires new events, if necessary

void FileModule::pushState()
{
    State state;
    int station;
    
	for( map<string, File*>::iterator it = files.begin(); it != files.end(); it++ )
    {
        if((*it).second->mode == File::IN )
        {
            NodeVector & vector = nodes[(*it).first];
            NodeVector::iterator jt;
            for( jt = vector.begin(); jt != vector.end(); jt++ )
            {
                ((FileSource*)(*jt))->changed = 0;
            }            
            while( 1 ){
                if( (*it).second->read( state, &station ) == 0 )
                {
                    break;
                }
                jt = vector.begin();
                while( jt != vector.end())
                {
                    if(((FileSource*)(*jt))->station == station && 
                       ((FileSource*)(*jt))->changed == 0 )
                        break;
                    jt++;
                }        
                if( jt != vector.end())
                {
                    ((FileSource*)(*jt))->changed = 1;
                    ((FileSource*)(*jt))->updateObservers( state );
                }                    
                else 
                    break;
           }
        }
    }
}

// Closes the files and cleans up datastructures

void FileModule::close()
{
    for( map<string, File*>::iterator it = files.begin(); it != files.end(); it ++ )
	{
        
		File * file = (File*)(*it).second;
		delete file;
	}
    files.clear();
}
