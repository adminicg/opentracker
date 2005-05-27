#ifndef SMALLCHANGE_SMCOORDINATESYSTEM_H
#define SMALLCHANGE_SMCOORDINATESYSTEM_H

/**************************************************************************\
 *
 *  This file is part of the SmallChange extension library for Coin.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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

#include <SmallChange/basic.h>

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoTransformation.h>

class SbMatrix;


class SMALLCHANGE_DLL_API SmCoordinateSystem : public SoTransformation {
  typedef SoTransformation inherited;

  SO_NODE_HEADER(SmCoordinateSystem);

public:
  static void initClass(void);
  SmCoordinateSystem(void);
  
  SoSFVec3f xAxis;
  SoSFVec3f yAxis;
  SoSFVec3f zAxis;
 
  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  
protected:
  virtual ~SmCoordinateSystem();

private:
  void calcMatrix(SbMatrix & m) const;
};


#endif //SMALLCHANGE_SMCOORDINATESYSTEM_H 
