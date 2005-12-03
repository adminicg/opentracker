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
/** source file for Group Gate Node
  *
  * @author Michael Knapp
  *
  * $Id$
  *
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include <stdlib.h>
#include "GroupGateNode.h"

#include <stdio.h>
#include <iostream>

#include <ace/Log_Msg.h>

//using namespace std;

namespace ot {

// constructor method
GroupGateNode::GroupGateNode(const char *name, GroupGateGroup *owner)
{
    Number = -1;
    Name = name;
    Owner = owner;
    IsActive = false;
} 

GroupGateNode::~GroupGateNode()
{
    Neighbors.clear();
} 

void
GroupGateNode::setNumber(int num)
{
    Number = num;
}

int
GroupGateNode::getNumber()
{
    return Number;
}

void 
GroupGateNode::addNeighbor(const char *neighbor)
{
    Neighbors.push_back(neighbor);
}

bool
GroupGateNode::isActive()
{
    return IsActive;
}

void
GroupGateNode::activate()
{
    IsActive = true;
}

void
GroupGateNode::deactivate()
{
    IsActive = false;
}

const char *
GroupGateNode::getGroupGateName()
{
    return (Name.c_str());
}

void 
GroupGateNode::onEventGenerated(State &event, Node &generator)
{
	if (generator.isNodePort() == 1)
	{
		NodePort &wrap = (NodePort &)generator;
		if (wrap.getType().compare("Override") == 0)
        {
            Owner->deactivateAll();
            activate();
            Owner->setActiveGroupGate(this);
            Owner->notifyActiveGate();
   		    updateObservers(event);
            return;
        }
    }
    if (isActive())
    {
        activate();
        Owner->notifyActiveGate();
      	updateObservers(event);
        return;
    }
    else
    {
        if (Neighbors.size() != NeighborPtrs.size())
        {
            for (NeighborsVector::iterator it = Neighbors.begin(); it != Neighbors.end(); it++)
            {
                GroupGateNode *node = (GroupGateNode *)(Owner->getNode((* it).c_str()));
                NeighborPtrs.push_back(node);
            }
            if (Neighbors.size() != NeighborPtrs.size())				
				ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:ERROR: Problem with GroupGateNode\n")));
        }
        for (NeighborPtrsVector::iterator it = NeighborPtrs.begin(); it != NeighborPtrs.end(); it++)
        {
            GroupGateNode *node = (GroupGateNode *)(* it);
            if (node->isActive())
            {
                node->deactivate();
                activate();
                Owner->setActiveGroupGate(this);
                Owner->notifyActiveGate();
        		updateObservers(event);
                return;
            }
        }
/*
        for (NeighborsVector::iterator it = Neighbors.begin(); it != Neighbors.end(); it++)
        {
            GroupGateNode *node = (GroupGateNode *)(Owner->getNode((* it).c_str()));
            if (node->isActive())
            {
                node->deactivate();
                activate();
                Owner->setActiveGroupGate(this);
                Owner->notifyActiveGate();
        		updateObservers(event);
                return;
            }
        }
*/
    }
}

} // namespace ot {
