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
/** Header file for MathUtils class, a collection of mathematical functions.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/MathUtils.h,v 1.10 2003/05/21 15:06:54 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _MATHUTILS_H
#define _MATHUTILS_H

#include "../dllinclude.h"

/* for accessing the elements of Quaternion and Vector3 */
#define Q_X    0
#define Q_Y    1
#define Q_Z    2
#define Q_W    3

/* tolerance for quaternion operations */
#define  Q_EPSILON   (1e-10)

/**
 * This class implements some utility classes for matrix and quaternion
 * calculations. These are static methods, I just used a class to have
 * a common place for such things, like in Java. These do not handle any
 * memory issues, all arrays etc. have to be created and managed by the
 * calling code.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API MathUtils
{
public:
    /// 3x3 double matrix type
    typedef double Matrix3x3[3][3];
    /// 4x4 double matrix type
    typedef double Matrix4x4[4][4];
    /// 3 double vector
    typedef double Vector3[3];
    /// 4 double vector used as quaternion
    typedef double Quaternion[4];

    /** converts an axis angle representation into a quaternion
     * @param axisa float[4] containing axis and angle in radiants
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* axisAngleToQuaternion(float* axisa, float* qResult);
    /** computes a quaternion from euler angles representing a rotation.
     * @param roll rotation around looking axis
     * @param pitch rotation around up axis
     * @param yaw rotation around side axis
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* eulerToQuaternion(float roll, float pitch, float yaw,
                                    float* qResult);
    /** inverts a quaternion.
     * @param q float[4] storing the quaternion
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* invertQuaternion(float* q, float* qResult);
    /** converts a rotational matrix to a quaternion.
     * @param matrix float[3][3] storing the rotational matrix
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* matrixToQuaternion(float matrix[3][3], float* qResult);
    /** multiplies two quaternions and stores result in a third.
     * @param q1 float[4] storing first quaternion
     * @param q2 float[4] storing second quaternion
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* multiplyQuaternion(float* q1, float* q2, float* qResult);
    /** normalizes quaternion to unit length. Here the computation is
     * done in place and the parameter is changed !
     * @param q float[4] storing quaternion
     * @return pointer to result array */
    static float* normalizeQuaternion(float* q);
    /** rotates a vector using a given unit quaternion. It does
     * not normalize the quaternion or check for unit length !
     * @param q float[4] storing quaternion
     * @param v float[3] storing vector
     * @param vResult float[3] where the result is stored
     * @return pointer to result array */
    static float* rotateVector(float* q, float* v, float* vResult);
	/** computes the determinant of a 3x3 matrix.
	 * @param matrix the 3x3 matrix to use
	 * @return determinant of the matrix */
	static float determinant( float matrix[3][3] );
    
	static void matrixMultiply(const Matrix4x4 m1, const Matrix4x4 m2, Matrix4x4 &m);
	static void matrixMultiply(const Matrix3x3 m1, const Matrix3x3 m2, Matrix3x3 &m);
	
    /// mirror at the x-z-plane
    static const Matrix4x4 matrix4x4_flipY;
    /// identity matrix
	static const Matrix4x4 matrix4x4_identity;

    /*****************************************************************************
     * MatrixToEuler - convert a column matrix to euler angles    
     *
     *  input:
     *	- vector to hold euler angles
	 *  - src column matrix
     *  Vector3  	angles    :      Holds outgoing roll, pitch, yaw
     *  Matrix4x4 	colMatrix :      Holds incoming rotation 
     *
     *  output:
     *  - euler angles in radians in the range -pi to pi;
	 *  vec[0] = yaw, vec[1] = pitch, vec[2] = roll
	 *  yaw is rotation about Z axis, pitch is about Y, roll -> X rot.
     *
     * notes:
     *	- originally written by Gary Bishop
     *
     *****************************************************************************/
	static void MatrixToEuler(Vector3 angles, const Matrix4x4 colMatrix);

    /// the nice constant Pi
    static const double Pi;

    /// the also nice constant e
    static const double E;

	/// another nice constant to transform grad to radiants
	static const double GradToRad;

    double angle( float * v1, float * v2, int dim );

    float * slerp( float * q1, float *q2, float t, float * qResult );

};

#endif
