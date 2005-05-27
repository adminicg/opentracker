#ifndef COIN_SBCUBICSPLINE_H
#define COIN_SBCUBICSPLINE_H

/**************************************************************************\
 *
 *  This file is part of the SmallChange extension library for Coin.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using SmallChange with software that can not be combined with the
 *  GNU GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/lists/SbList.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>

#include <SmallChange/basic.h>


class SMALLCHANGE_DLL_API SbCubicSpline {
public:
  SbCubicSpline(const int approxcount = 256);
  ~SbCubicSpline();

  enum Type {
    CATMULL_ROM = 0,
    B_SPLINE,
    BEZIER
  };
  
  void setBasisMatrix(const Type type);
  void setBasisMatrix(const SbMatrix & m);
  const SbMatrix & getBasisMatrix(void) const;
  void setLoop(const SbBool loop);
  void setControlPoints(const SbVec3f * ptr, const int num);
  void setControlPoints(const SbVec4f * pts, const int num);

  // runtime

  SbVec3f getPoint(const float t);
  SbVec3f getTangent(const float t);
  int getSegmentInfo(const float t, float & segt) const;
  float getSegmentLength(const int idx);

private:

  int getSegdata(const float t, float & segt);
  SbVec3f getPoint(const SbMatrix & m, const float t);
  SbVec3f getTangent(const SbMatrix & m, const float t);
  void initMatrix(const int seg, SbMatrix & m);
  void initialize(void);
  int getSegnum(const float time) const;
  int clampSegnum(const int q) const;

  SbList <SbVec4f> ctrlpts;
  SbList <float> segstarttimes;
  SbList <float> segdurations;
  SbList <float> seglens;

  SbBool needinit;
  SbBool loop;
  int approxcount;
  int currsegment;
  SbMatrix currmatrix;
  SbMatrix basismatrix;
};


#endif // COIN_SBCUBICSPLINE_H
