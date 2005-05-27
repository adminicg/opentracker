#ifndef SMALLCHANGE_SMCAMERACONTROLKIT_H
#define SMALLCHANGE_SMCAMERACONTROLKIT_H

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

#include <Inventor/nodekits/SoSubKit.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <SmallChange/basic.h>

class SmCameraControlKitP;
class SoCamera;
class SbMatrix;
class SbViewportRegion;
class SoEvent;

class SMALLCHANGE_DLL_API SmCameraControlKit : public SoBaseKit {
  typedef SoBaseKit inherited;

  SO_KIT_HEADER(SmCameraControlKit);

  SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
  SO_KIT_CATALOG_ENTRY_HEADER(camera);
  SO_KIT_CATALOG_ENTRY_HEADER(headlightNode);
  SO_KIT_CATALOG_ENTRY_HEADER(scene);
  
public:
  SmCameraControlKit(void);
  static void initClass(void);
  
  virtual void GLRender(SoGLRenderAction * action);
  virtual void handleEvent(SoHandleEventAction * action);

  enum AutoClippingStrategy {
    VARIABLE_NEAR_PLANE,
    CONSTANT_NEAR_PLANE
  };

  SbBool isAnimating(void);
  SbBool isBusy(void) const;
  SbBool seek(const SoEvent * event, const SbViewportRegion & vp);
  void viewAll(const SbViewportRegion & vp, const float slack = 1.0f);
  void pointDir(const SbVec3f & dir, const SbBool resetroll = TRUE);
  void resetCameraRoll(void);

  void resetCameraFocalDistance(const SbViewportRegion & vpr);

protected:

  virtual SbBool setAnyPart(const SbName & partname, SoNode * from,
                            SbBool anypart = TRUE);
  
  virtual void notify(SoNotList * list);
  virtual ~SmCameraControlKit();
  
public:
  SoSFBool headlight;
  SoSFBool autoClipping;
  SoSFEnum autoClippingStrategy;
  SoSFFloat autoClippingValue;
  SoSFNode eventHandler;
  SoSFVec3f viewUp;

private:
  void setClippingPlanes(void);
  void getCameraCoordinateSystem(SoCamera * camera,
                                 SoNode * root,
                                 SbMatrix & matrix,
                                 SbMatrix & inverse);

  friend class SmCameraControlKitP;
  SmCameraControlKitP * pimpl;
};

#endif // SMALLCHANGE_SMCAMERACONTROLKIT_H
