#ifndef GEOMUTIL
#define GEOMUTIL

#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>

#include "vec3d.h"
#include "vec3q.h"
#include "vec3f.h"

#define PI 3.1415926535

class Quad;

bool testSphereBox(double sph[4], double bxC[3], double bxS[3]);
float distance(vec3f *a, vec3f *b);
double distance(const vec3d *a, const vec3d *b);
double distance(const vec3q *a, const vec3q *b);
double distance(const vec3q& a, const vec3q& b);
double distance(vec3d *a, vec3d *b);
vec3d* middleVec(const vec3d *a, const vec3d *b);
bool testAngle(vec3q angle, Quad *quad);

/** Tests if the Sphere is in the box or touches it.
* boxC is not the center instead it is one conner
* of the box
*/
bool testSphereBox(vec3d sphP, double sphRad, vec3d boxC, vec3d boxS);

/** Tests if the Sphere is in the box.
* boxC is not the center instead it is one conner
* of the box
*/
bool testSphereInBox(vec3d sphP, double sphRad, vec3d boxC, vec3d boxS);
bool testSphereInBox(const vec3d *sphP, double sphRad, const vec3d *boxC, const vec3d *boxS);

/** Computes a bSpline from the two given vec3d's and the float between 0.0
* and 1.0. The returned vec3d needs to deleted if no longer used.
*/
vec3d* bSpline(vec3d *a, vec3d *b, vec3d *c, float t);
vec3d* calcSphereProj(vec3d *tl, vec3d *br, vec3d *mid, double scale);

/** returns the crossProduct of to given vec3d
* returns the vec3d on the stack
 */
vec3d crossProduct(vec3d *u, vec3d *v);

/** skalarMulti of an vec3d with a given double
 * wrote this because the vec3d class creates an
 * new object on the heap when using the buildin
 * scalarmultiplication.
 */
vec3d skalarMulti(vec3d a, double ska);

/** normalization of an vec3d on the
 * stack.
 */
vec3d normalize(vec3d a);

/** subtraction of to vec3d passed on the
 * stack with the result returned on the 
 * stack as well.
 */
vec3d subtract(vec3d a, vec3d b);

/** addition of to vec3d passed on the
 * stack with the result returned on the 
 * stack as well.
 */
vec3d add(vec3d a, vec3d b);

/** returns a double describing the angle
 * between the two given angle on the stack
 */
double angle(vec3d a, vec3d b);
int getExpoBase2(double d);

/** tests if the two given double values
 * are bit equal
 */
bool equals(double d1, double d2);

/** this functio is used within the camera to
 * add new values to the angle.
 * it is checked if the values are still between
 * -180 and 180
 */
double reAngle(double old, double pp, char an);
#endif
