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
/** source file for TestModule module.
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/core/OtLogger.h>
#include <cstdlib>

#include <OpenTracker/core/TestModule.h>


#ifndef OT_NO_TESTMODULE_SUPPORT


#include <OpenTracker/core/TestSource.h>

#include <OpenTracker/core/MathUtils.h>

#include <cstdio>
#include <cstdlib>
#include <algorithm>
// #include <iostream> FIXME Why was this included ?

#include <ace/Log_Msg.h>
#include <ace/OS.h>


#include <OpenTracker/core/Context.h>
//using namespace std;

// Destructor method

namespace ot {

	OT_MODULE_REGISTER_FUNC(TestModule){
		OT_MODULE_REGISTRATION_DEFAULT(TestModule, "TestConfig");
	}
                  
    void TestModule::removeNode(Node * node) {
        logPrintI("TestModule::removeNode\n");
        logPrintI("TestModule deleting node %s\n", node->get("ID").c_str());
        NodeVector::iterator result = std::find( nodes.begin(), nodes.end(), node );
        if( result != nodes.end())
        {
            nodes.erase( result );
            return;
        } else {
            logPrintE("Node with ID %s not in TestModule nodes vector\n", node->get("ID").c_str());
        }
    }



    TestModule::~TestModule()
    {
        nodes.clear();
    }

    // This method is called to construct a new Node.

    Node * TestModule::createNode( const std::string& name, StringTable& attributes)
    {
        if( name.compare("TestSource") == 0 )
        {
            double frequency;
            int offset;
            int num = sscanf(attributes.get("frequency").c_str(), " %lf", &frequency );
            if( num <= 0 ){
                frequency = 1;
            }
            num = sscanf(attributes.get("offset").c_str(), " %i", &offset );

            if( num <= 0){
                offset = 0;
            }

            TestSource * source = new TestSource( frequency, offset );
            attributes.get("position", source->event.getPosition(), 3);
            attributes.get("orientation", source->event.getOrientation(), 4);
            sscanf( attributes.get("button").c_str(), " %hu", &(source->event.getButton()) );
            sscanf( attributes.get("confidence").c_str(), " %f", &(source->event.getConfidence()) );
            if( attributes.containsKey("noise") )
            {
                attributes.get("noise",&source->noise);
            }
            else
            {
                source->noise = -1;
            }

            nodes.push_back( source );
            logPrintI("Built TestSource node \n");
            initialized = 1;
            return source;
        }
        return NULL;
    }

    void TestSource::calculateEvent()
    {
        if( noise > 0 )
        {
            lock();                
            perturbed.setAttribute("intAttr", cycle++);
            perturbed.setAttribute("chrAttr", (char)(33 + cycle % 92));
            perturbed.setAttribute("dblAttr", (double)(cycle / 7.3));
            perturbed.setAttribute("fltAttr", (float)(cycle / 7.3));
                
            int i;
            for( i = 0; i < 3; i++ )
            {
                perturbed.getPosition()[i] = (float)(event.getPosition()[i] + ((float)rand()/RAND_MAX)*noise - noise / 2.0);
                perturbed.getOrientation()[i] = (float)(event.getOrientation()[i] + (float)(rand()/RAND_MAX)*noise - noise / 2.0);
            }
            perturbed.getOrientation()[3] = (float)(event.getOrientation()[3] + ((float)rand()/RAND_MAX)*noise - noise / 2.0);
            MathUtils::normalizeQuaternion( perturbed.getOrientation() );
            if( ((float)rand()/RAND_MAX) < noise  )
            {
                perturbed.getOrientation()[0] = -perturbed.getOrientation()[0];
                perturbed.getOrientation()[1] = -perturbed.getOrientation()[1];
                perturbed.getOrientation()[2] = -perturbed.getOrientation()[2];
                perturbed.getOrientation()[3] = -perturbed.getOrientation()[3];
            }
            perturbed.setConfidence(static_cast<float>((rand()/RAND_MAX)*noise - noise / 2.0));
            perturbed.setButton(cycle % 16);


            perturbed.timeStamp();
            unlock();
        }
        else
        {
            lock();
            perturbed.timeStamp();
            unlock();
        }

    }

    // pushes events into the tracker tree.

    void TestModule::start()
    {
        if( isInitialized() && !nodes.empty())
	    ThreadModule::start();
    }

  void TestModule::close()
    {
	lockLoop();
	stop = 1;
	unlockLoop();
    }

    TestSource::TestSource( double frequency_, int offset_ ) :
        Node(), cycle( 1 )
    {
        using namespace std;
        double tau = 1.0/static_cast<double>(frequency_);
        long taus(static_cast<long>(tau));
        long tauu(static_cast<long>((tau-static_cast<double>(taus))*1000000.0));
        sleeptime = ACE_Time_Value(taus,tauu);
        sleepoffset = ACE_Time_Value(offset_/1000000, offset_);
        if (taus == 0 && tauu == 0)
        {
            cerr << "frequency: " << frequency_ << endl;
            cerr << "tau: " << tau << endl;
            cerr << "taus: " << taus << endl;
            cerr << "tauu: " << tauu << endl;
            cerr << "sleeptime.sec : "<< sleeptime.sec() << endl;
            cerr << "sleeptime.usec: "<< sleeptime.usec() << endl;
            cerr << "sleepoffs.sec : "<< sleepoffset.sec() << endl;
            cerr << "sleepoffs.usec: "<< sleepoffset.usec() << endl;  
            exit(1);
        }
    }

    void TestModule::run()
    {
        using namespace std;
        cout << "TestModule::run()" << endl;
        starttime = ACE_OS::gettimeofday();
        
        // trigger all nodes

        NodeVector::iterator it;
        for (it = nodes.begin(); it != nodes.end(); it++)
        {
#ifndef USE_LIVE
            TestSource * src = dynamic_cast<TestSource*>(it->item());
#else
            TestSource * src = dynamic_cast<TestSource*>((*it));
#endif

            if (src)
            {
                src->resetCycleCounter();

                ACE_Time_Value newtime(starttime + 
                                       src->sleeptime +
                                       src->sleepoffset);
                cerr << "sleeptime.sec : "<< src->sleeptime.sec() << endl;
                cerr << "sleeptime.usec: "<< src->sleeptime.usec() << endl;
                cerr << "newtime.sec : "<< newtime.sec() << endl;
                cerr << "newtime.usec: "<< newtime.usec() << endl;   
           
                pqueue.push(make_pair(newtime, (*it)));
            }
            else
            {
                ACE_DEBUG((LM_ERROR, 
                           ACE_TEXT("ot:Error: not a TestSource !\n")));
                exit( -1 );
            }
            
        }

        // central loop
        while (1)
        {
            lockLoop();
            if(stop)
            {
                unlockLoop();
                break;
            }
            else
            {
                unlockLoop();
            }

            // update event value
            TestSource * tsrc = 
#ifndef USE_LIVE
                dynamic_cast<TestSource*>(pqueue.top().second.item());
#else
                dynamic_cast<TestSource*>(pqueue.top().second);
#endif

            // calculate time to wait for next traversal

            ACE_Time_Value looptime(ACE_OS::gettimeofday());
            ACE_Time_Value sleeptime(pqueue.top().first);

            if (sleeptime > looptime)
            {
                ACE_OS::sleep(sleeptime - looptime);
                tsrc->calculateEvent();
                ACE_Time_Value newtime(starttime + 
                                       (tsrc->sleeptime * tsrc->cycle) +
                                       tsrc->sleepoffset);
                pqueue.push(make_pair(newtime, pqueue.top().second));
                
                pqueue.pop();            
                
                if (context != NULL)
                {
                    context->dataBroadcast();
                }
            }
            else if ( (sleeptime - looptime).sec() == 0 &&
                      abs((sleeptime - looptime).usec()) < 5000)
            {
                // if we are a bit too slow -> skip sleep
                tsrc->calculateEvent();
                ACE_Time_Value newtime(starttime + 
                                       (tsrc->sleeptime * tsrc->cycle) +
                                       tsrc->sleepoffset);
                pqueue.push(make_pair(newtime, pqueue.top().second));
                
                pqueue.pop();            
                
                if (context != NULL)
                {
                    context->dataSignal();
                }
            }
            else
            {     
                // if we are far too slow, some cycles are just skipped
                // by calculating the next future cycle
               
                while (ACE_OS::gettimeofday() >=
                       ( starttime+tsrc->sleepoffset + 
                         (tsrc->cycle*tsrc->sleeptime)) )
                {
                    ++(tsrc->cycle);
                }
                
                /*
                  cerr << "jumping to cycle " << tsrc->cycle << endl;
                */

                pqueue.push(make_pair(starttime + 
                                      (tsrc->cycle * tsrc->sleeptime) +
                                      tsrc->sleepoffset,
                                      pqueue.top().second));
                /*
                  cerr << "cycle:        : " << tsrc->cycle << endl;
                  cerr << "ssleep.sec:   : " << tsrc->sleeptime.sec() << endl;
                  cerr << "ssleep.usec:  : " << tsrc->sleeptime.usec() << endl;
                  cerr << "starttime.sec : " << starttime.sec() << endl;
                  cerr << "starttime.usec: " << starttime.usec() << endl;
                  cerr << "looptime.sec  : " << looptime.sec() << endl;
                  cerr << "looptime.usec : " << looptime.usec() << endl;
                  cerr << "sleeptime.sec : " << sleeptime.sec() << endl;
                  cerr << "sleeptime.usec: " << sleeptime.usec() << endl;
                  exit (1);
                */
                pqueue.pop(); 
            }

            //if (tsrc->cycle > 5) exit(1);
        }
    }   
    void TestModule::pushEvent()
    {
        /// nothing to do
    }

    void TestSource::pullEvent()
    {
        /// nothing to do
    }

    void TestSource::pushEvent(void)
    {
        lock();
        event = perturbed;
        unlock();
        updateObservers( event );
    }


} //namespace ot


#endif //OT_NO_TESTMODULE_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of TestModule.h
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
