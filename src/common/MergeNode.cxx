 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for Merge Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/MergeNode.cxx,v 1.1 2001/01/28 16:48:14 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "MergeNode.h"

// adds a child to the Node.

void MergeNode::addChild( Node& node)
{
    WrapperNode wrapper = node.isWrapperNode();
    if( wrapper != NULL )
    {
        NodeVector nodes = &wrapper.getChildren();
        if( wrapper.getTagName().compare("MergeDefault") == 0 )
        {            
            for( NodeVector::iterator it = nodes->begin();
                 it != nodes->end(); it++ )
            {
                EventGenerator generator = (*it)->isEventGenerator();
                if( generator != NULL )
                {
                    generator.addEventObserver( *this );
                    mergeFlags[generator] |= DEFAULT;
                }
            }
        } else if(  wrapper.getTagName().compare("MergePosition") == 0 )
        {
            for( NodeVector::iterator it = nodes->begin();
                 it != nodes->end(); it++ )
            {
                EventGenerator generator = (*it)->isEventGenerator();
                if( generator != NULL )
                {
                    generator.addEventObserver( *this );
                    mergeFlags[generator] |= POSITION;
                    defaultFlags &= ~POSITION;
                }
            }
        } else if(  wrapper.getTagName().compare("MergeOrientation") == 0 )
        {
            for( NodeVector::iterator it = nodes->begin();
                 it != nodes->end(); it++ )
            {
                EventGenerator generator = (*it)->isEventGenerator();
                if( generator != NULL )
                {
                    generator.addEventObserver( *this );
                    mergeFlags[generator] |= ORIENTATION;
                    defaultFlags &= ~ORIENTATION;
                }
            }
        } else if(  wrapper.getTagName().compare("MergeButton") == 0 )
        {
            for( NodeVector::iterator it = nodes->begin();
                 it != nodes->end(); it++ )
            {
                EventGenerator generator = (*it)->isEventGenerator();
                if( generator != NULL )
                {
                    generator.addEventObserver( *this );
                    mergeFlags[generator] |= BUTTON;
                    defaultFlags &= ~BUTTON;
                }
            }
        } else if(  wrapper.getTagName().compare("MergeConfidence") == 0 )
        {
            for( NodeVector::iterator it = nodes->begin();
                 it != nodes->end(); it++ )
            {
                EventGenerator generator = (*it)->isEventGenerator();
                if( generator != NULL )
                {
                    generator.addEventObserver( *this );
                    mergeFlags[generator] |= CONFIDENCE;
                    defaultFlags &= ~CONFIDENCE;
                }
            }
        } else if(  wrapper.getTagName().compare("MergeTime") == 0 )
        {
            for( NodeVector::iterator it = nodes->begin();
                 it != nodes->end(); it++ )
            {
                EventGenerator generator = (*it)->isEventGenerator();
                if( generator != NULL )
                {
                    generator.addEventObserver( *this );
                    mergeFlags[generator] |= TIME;
                    defaultFlags &= ~TIME;
                }
            }
        }
    }
}
