#ifndef COIN_SOGLCACHELIST_H
#define COIN_SOGLCACHELIST_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbList.h>

class SoGLRenderAction;
class SoGLRenderCache;
class SoGLCacheListP;

/* Make the class available in the public API (i.e. activate the
   COIN_DLL_API keyword) for next minor release -- class is public. */
#if COIN_MINOR_VERSION > 3
#error The SoGLCacheList class should be added to the API properly from Coin 2.4.
#endif

class /*COIN_DLL_API*/ SoGLCacheList {
public:
  SoGLCacheList(int numcaches = 2);
  ~SoGLCacheList();

  SbBool call(SoGLRenderAction * action);

  void open(SoGLRenderAction * action, SbBool autocache = TRUE);
  void close(SoGLRenderAction * action);

  void invalidateAll(void);

private:
  SoGLCacheListP * pimpl;
};

#endif // !COIN_SOGLCACHELIST_H
