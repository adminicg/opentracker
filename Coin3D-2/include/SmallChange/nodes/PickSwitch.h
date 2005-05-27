#ifndef PICKSWITCH_H
#define PICKSWITCH_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>

#include <SmallChange/basic.h>


class SMALLCHANGE_DLL_API PickSwitch : public SoSwitch {
  typedef SoSwitch inherited;

  SO_NODE_HEADER(PickSwitch);

public:
  static void initClass(void);
  PickSwitch(void);
  
  SoSFBool isActive;
  SoSFInt32 activeChild;
  SoSFInt32 inactiveChild;
  SoSFBool toggle;
  
protected:
  virtual ~PickSwitch();
  virtual void handleEvent(SoHandleEventAction * action);

private:
};

#endif // !PICKSWITCH_H
