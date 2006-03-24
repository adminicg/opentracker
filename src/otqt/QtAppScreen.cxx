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
/**
 * @file   QtAppScreen.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class \c QtAppScreen
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <dllinclude.h>
#if USE_OTQT

#include "QtAppScreen.h"
#include "OTQtLog.h"
#include <qapplication.h>
#include <math.h>

namespace ot {

//--------------------------------------------------------------------------------
QtAppScreen::QtAppScreen(StringTable & table)
  : ConfigNode(table)
{
  ///// init calib out structure

  attributes.get("CSOrientationQuat", calib_out_.as_cs_orient, 4);
  attributes.get("CSRoot2ScreenRootVec", calib_out_.as_cs_root_to_screen_root, 3);
  attributes.get("ASWidthVec", calib_out_.as_width_vec, 3);
  attributes.get("ASHeightVec", calib_out_.as_height_vec, 3);

  ///// init app screen data

  // CS root / screen root position
  for (int i = 0; i < 4; i++) {
    as_data_init_.as_cs_root.orientation[i] = calib_out_.as_cs_orient[i];
    as_data_init_.as_screen_root.orientation[i] = calib_out_.as_cs_orient[i];
  }
  // CS root / screen root position, screen plane span vectors
  for (int i = 0; i < 3; i++) {
    as_data_init_.as_cs_root.position[i] = 0;
    as_data_init_.as_screen_root.position[i] = calib_out_.as_cs_root_to_screen_root[i];
    as_data_init_.as_width_vec[i] = calib_out_.as_width_vec[i];
    as_data_init_.as_height_vec[i] = calib_out_.as_height_vec[i];
  }

  // compute screen depth scale
  float tracking_one_meter;
  attributes.get("TrackingSystemScaleOneMeter", &tracking_one_meter);
  float as_depth_in_meter;
  attributes.get("ScreenDepthInMeter", &as_depth_in_meter);
  as_data_init_.as_depth_scalar_front = tracking_one_meter * as_depth_in_meter;
  as_data_init_.as_depth_scalar_back =
    OTQtMath::fsignum(as_data_init_.as_depth_scalar_front) *
    AS_DEPTH_SCALAR_BACK_DEFAULT;

  ///// assign current data the initial values

  as_data_.as_cs_root = as_data_init_.as_cs_root;
  as_data_.as_screen_root = as_data_init_.as_screen_root;
  as_data_.as_depth_scalar_front = as_data_init_.as_depth_scalar_front;
  as_data_.as_depth_scalar_back = as_data_init_.as_depth_scalar_back;
  for (int i = 0; i < 3; i++) {
    as_data_.as_width_vec[i] = as_data_init_.as_width_vec[i];
    as_data_.as_height_vec[i] = as_data_init_.as_height_vec[i];
  }

  ///// init mouse position object data

  updateMPD(State::null);
}

//--------------------------------------------------------------------------------
void QtAppScreen::convert(CalibOutputData const & out, StringTable & table)
{
  // generate attributes from calibration output data
  table.put("CSOrientationQuat", out.as_cs_orient, 4);
  table.put("CSRoot2ScreenRootVec", out.as_cs_root_to_screen_root, 3);
  table.put("ASWidthVec", out.as_width_vec, 3);
  table.put("ASHeightVec", out.as_height_vec, 3);
}


//--------------------------------------------------------------------------------
void QtAppScreen::convert(CalibInputData const & in, CalibOutputData & out)
{
  ///// transform input corners to common coordinate system

  CalibInputData in_final;
  in_final.as_cs_root = in.as_cs_root;

  // top / left
  OTQtMath::transformVectorFromCSToCS(in.top_left.local_cs_root, in_final.as_cs_root,
                                      in.top_left.corner, in_final.top_left.corner);
  in_final.top_left.local_cs_root = in_final.as_cs_root;
  // top / right
  OTQtMath::transformVectorFromCSToCS(in.top_right.local_cs_root, in_final.as_cs_root,
                                      in.top_right.corner, in_final.top_right.corner);
  in_final.top_right.local_cs_root = in_final.as_cs_root;
  // bottom / right
  OTQtMath::transformVectorFromCSToCS(in.bottom_right.local_cs_root, in_final.as_cs_root,
                                      in.bottom_right.corner, in_final.bottom_right.corner);
  in_final.bottom_right.local_cs_root = in_final.as_cs_root;
  // bottom / left
  OTQtMath::transformVectorFromCSToCS(in.bottom_left.local_cs_root, in_final.as_cs_root,
                                      in.bottom_left.corner, in_final.bottom_left.corner);
  in_final.bottom_left.local_cs_root = in_final.as_cs_root;


  ///// save screen coordinate system orientation

  for (int i = 0; i < 4; i++) {
    out.as_cs_orient[i] = in_final.as_cs_root.orientation[i];
  }

  ///// difference vector between screen coordinate system root and screen root

  for (int i = 0; i < 3; i++) {
    out.as_cs_root_to_screen_root[i] =
      in_final.top_left.corner.position[i] - in_final.as_cs_root.position[i];
  }

  ///// compute screen base vectors


  // +++ Coordinate System
  //
  //   ASP (coordinate system root (pos/orient)
  //   /
  //  /
  // SR (screen root == top/left)
  //
  // +++ Screen plane and corners
  //
  // P1 (top/left) ----- P2 (top/right)
  // |                   |
  // |   screen plane    |
  // |                   |
  // P4 (bottom/left) -- P3 (bottom/right)
  //
  // +++ Base Vectors
  //
  // P1 -> P2, P4 -> P3: "x-axis" : v1 (vec_tl_tr)
  // P1 -> P4, P2 -> P3 : "y-axis" : v2 (vec_tl_tr)

  // corners of screen
  RowVector tl(3); // P1
  tl << in_final.top_left.corner.position;
  RowVector tr(3); // P2
  tr << in_final.top_right.corner.position;
  RowVector br(3); // P3
  br << in_final.bottom_right.corner.position;
  RowVector bl(3); // P4
  bl << in_final.bottom_left.corner.position;

  // vectors which span screen plane (cuboid)
  RowVector vec_tl_tr(3); // x - direction, v1
  RowVector vec_tl_bl(3); // y - direction, v2

  // base vector v1: top/left -> top/right
  vec_tl_tr = tr - tl;

  // diagonal vector: top/left -> bottom/right
  RowVector diag_tl_br = br - tl;
  // length parameter for diagonal vector, resets bottom/right corner position, difference
  // vector between bottom/right and top/right is normal on v1
  float k = DotProduct(vec_tl_tr, vec_tl_tr) / DotProduct(vec_tl_tr, diag_tl_br);
  // base vector v2: top/right -> (transformed) bottom/right
  vec_tl_bl = k * diag_tl_br - vec_tl_tr;

  for (int i = 0; i < 3; i++) {
    out.as_width_vec[i] = vec_tl_tr(i+1);
    out.as_height_vec[i] = vec_tl_bl(i+1);
  }
}

//--------------------------------------------------------------------------------
void QtAppScreen::updateASPD(State const & as_cs_root_curr)
{
  ///// transform screen position

  State as_screen_root_curr;
  OTQtMath::transformVectorFromCSToCS(as_data_init_.as_cs_root, as_cs_root_curr,
                                      as_data_init_.as_screen_root, as_screen_root_curr);
  as_data_.as_screen_root = as_screen_root_curr;

  ///// transform screen plane base vectors
  OTQtMath::Vector3 as_width_vec_curr;
  OTQtMath::rotateVectorFromCSToCS(as_data_init_.as_cs_root, as_cs_root_curr,
                                   as_data_init_.as_width_vec, as_width_vec_curr);
  OTQtMath::Vector3 as_height_vec_curr;
  OTQtMath::rotateVectorFromCSToCS(as_data_init_.as_cs_root, as_cs_root_curr,
                                   as_data_init_.as_height_vec, as_height_vec_curr);
  for (int i = 0; i < 3; i++) {
    as_data_.as_width_vec[i] = as_width_vec_curr[i];
    as_data_.as_height_vec[i] = as_height_vec_curr[i];
  }

  // set current CS root
  as_data_.as_cs_root = as_cs_root_curr;
}

//--------------------------------------------------------------------------------
void QtAppScreen::updateMPD(State const & mpd_pos)
{

  // save current MPD pos
  mp_data_.position = mpd_pos;

  ///// init vectors

  // base vector "x-axis"
  RowVector width(3);
  width << as_data_.as_width_vec;
  // base vector "y-axis"
  RowVector height(3);
  height << as_data_.as_height_vec;

  // base vector "z-axis" as cross product width x height
  RowVector depth = - OTQtMath::crossProductR3(width, height);
  RowVector depth1 = depth / depth.NormFrobenius();
  // add negative depth (back side of screen depth)
  depth = depth1 * (as_data_.as_depth_scalar_front + as_data_.as_depth_scalar_back);

  // screen root
  RowVector screen_root(3);
  screen_root << as_data_.as_screen_root.position;
  RowVector depth_screen_root = screen_root - (depth1 * as_data_.as_depth_scalar_back);

  // given MPD position
  RowVector mpos(3);
  mpos << mpd_pos.position;

  ///// check position within screen cuboid

  RowVector distances_width(2);
  RowVector distances_height(2);
  RowVector distances_depth(2);
  if (!OTQtMath::isWithinPlaneBorders(mpos, width, screen_root, distances_width) ||
      !OTQtMath::isWithinPlaneBorders(mpos, height, screen_root, distances_height) ||
      !OTQtMath::isWithinPlaneBorders(mpos, depth, depth_screen_root, distances_depth))
  {
    // transition inside -> outside: MPD location changed
    bool mpd_loc_changed_this_cycle = (mp_data_.mpd_loc_inside_screen_cuboid);
    // MPD is located outside SC
    mp_data_.mpd_loc_inside_screen_cuboid = false;

    if (OTQT_DEBUG_ON && mpd_loc_changed_this_cycle) {
      OTQT_INFO("QtAppScreen::updateMPD(): depth = %f %f %f\n",
		  depth(1), depth(2), depth(3));
      OTQT_INFO("QtAppScreen::updateMPD(): width = %f, distances_width() = %f %f\n",
		  width.NormFrobenius(), distances_width(1), distances_width(2));
      OTQT_INFO("QtAppScreen::updateMPD(): height = %f, distances_height() = %f %f\n",
		  height.NormFrobenius(), distances_height(1), distances_height(2));
      OTQT_INFO("QtAppScreen::updateMPD(): depth = %f, distances_depth() = %f %f\n",
		  depth.NormFrobenius(), distances_depth(1), distances_depth(2));
    }
    return;
  }

  OTQT_INFO("QtAppScreen::updateMPD(): width = %f, distances_width() = %f %f\n",
            width.NormFrobenius(), distances_width(1), distances_width(2));
  OTQT_INFO("QtAppScreen::updateMPD(): height = %f, distances_height() = %f %f\n",
            height.NormFrobenius(), distances_height(1), distances_height(2));
  OTQT_INFO("QtAppScreen::updateMPD(): depth = %f, distances_depth() = %f %f\n",
            depth.NormFrobenius(), distances_depth(1), distances_depth(2));

  // NOTE: MPD position is within screen cuboid

  ///// convert mouse position to desktop coordinates

  RowVector mpos_screen_coords(2);
  mpos_screen_coords(1) = distances_width(1);
  mpos_screen_coords(2) = distances_height(1);
  OTQT_DEBUG("QtAppScreen::isMPDWithinScreenCuboid(): mpos_screen_coords = %f %f\n",
             mpos_screen_coords(1), mpos_screen_coords(2));

  // convert to Qt desktop coordinates
  QPoint desktop_coords_new;
  QRect const & desktop = QApplication::desktop()->screenGeometry();
  OTQT_DEBUG("QtAppScreen::isMPDWithinScreenCuboid(): desktop = %d %d\n",
             desktop.width(), desktop.height());
#ifdef WIN32
  desktop_coords_new.setX((int)floor((mpos_screen_coords(1) / width.NormFrobenius()) * (float)desktop.width()) - 1);
  desktop_coords_new.setY((int)floor((mpos_screen_coords(2) / height.NormFrobenius()) * (float)desktop.height()) - 1);
#else
  desktop_coords_new.setX((int)round((mpos_screen_coords(1) / width.NormFrobenius()) * (float)desktop.width()) - 1);
  desktop_coords_new.setY((int)round((mpos_screen_coords(2) / height.NormFrobenius()) * (float)desktop.height()) - 1);
#endif

  /////  save MPD data

  // transition outside -> inside: MPD location changed
  bool mpd_loc_changed_this_cycle = !(mp_data_.mpd_loc_inside_screen_cuboid);
  // MPD is located INSIDE SC
  mp_data_.mpd_loc_inside_screen_cuboid = true;
  // desktop mouse cursor coordinates
  mp_data_.desktop_coords = desktop_coords_new;

  if (OTQT_DEBUG_ON && mpd_loc_changed_this_cycle) {
    OTQT_INFO("QtAppScreen::updateMPD(): width = %f, distances_width() = %f %f\n",
              width.NormFrobenius(), distances_width(1), distances_width(2));
    OTQT_INFO("QtAppScreen::updateMPD(): height = %f, distances_height() = %f %f\n",
              height.NormFrobenius(), distances_height(1), distances_height(2));
    OTQT_INFO("QtAppScreen::updateMPD(): depth = %f, distances_depth() = %f %f\n",
              depth.NormFrobenius(), distances_depth(1), distances_depth(2));
  }

  return;
}

//--------------------------------------------------------------------------------
QtAppScreen::MPDLocationState
QtAppScreen::getMPDLocation() const
{
  MPDLocationState ret = MPD_LOC_UNKNOWN;

  if (mp_data_.mpd_loc_inside_screen_cuboid)
    ret = MPD_LOC_INSIDE_SC;
  else
    ret = MPD_LOC_OUTSIDE_SC;

  return ret;
}

} // namespace ot

#endif // USE_OTQT
