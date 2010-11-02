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
/** source file for P5GloveSource Node.
 *
 * @author Mathis Csisinko
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/input/P5GloveSource.h>

#ifdef USE_P5GLOVE

namespace ot {

	P5GloveSource::P5GloveSource(int finger,const std::vector<float> &vector,const std::vector<float> &axis) : Node(), finger(finger),vector(vector),axisAngle(axis)
	{
		axisAngle.resize(4);
	}

    Event & P5GloveSource::getEvent(P5Data &p5Data)
	{
		std::vector<float> quaternion(4);
		MathUtils::axisAngleToQuaternion(axisAngle,quaternion);
		axisAngle[3] = MathUtils::Pi / 180.f * p5Data.m_byBendSensor_Data[finger];
		event.getAttribute<float>(std::string("bending"),0.f) = p5Data.m_byBendSensor_Data[finger];
		event.getButton() = (p5Data.m_byBendSensor_Data[finger] > BEND_THRESHOLD) ? 1: 0;
		MathUtils::rotateVector(quaternion,vector,event.getPosition());
		event.getOrientation() = quaternion;
		return event;
	}

}  // namespace ot

#endif