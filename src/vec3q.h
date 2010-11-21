#ifndef VEC3Q
#define VEC3Q
#include "vec3d.h"
#include <cmath>
#include <string>
#include <stdlib.h>
//#include "../util/geomUtil.h"

/** This class is derivist from vec3d but you the
 * [] operator should be much slower because this class
 * is used the in planet creation below display/Sphere.
 *
 * Use this on the stack.
 */
class vec3q {
	private:
		bool compare(const vec3q &second);
	public:
		double x;
		double y;
		double z;

		vec3q(double x_n, double y_n, double z_n);
		vec3q(const vec3q &o);
		vec3q(const vec3d *o);
		vec3q(const vec3d o);
		vec3q();
		~vec3q();
		vec3q& operator= (const vec3q &o);
		vec3q& operator= (const vec3d *o);
		vec3q& operator= (const vec3d o);

		vec3d* getVec3d();

		//addition subtraction
		vec3q operator+ (const vec3q &second) const;
		void operator+= (const vec3q &second);
		vec3q operator- (const vec3q &second) const;
		void operator-= (const vec3q &second);

		//crossproduct
		vec3q operator* (const vec3q &second) const;
		void operator*= (const vec3q &second);

		//scalar multiplcation
		vec3q operator* (const double &scalar) const;
		void operator*= (const double &scalar);
		double operator[](int i) const;
		double length() const;

		vec3q& normalize();
		double dot(const vec3q &second);
		vec3q normReturn() const;
		vec3q& cubeSphere(); 

		bool operator!=(const vec3q &second);
		bool operator==(const vec3q &second);

		std::string* toXML() const;

		void print() const;
		bool test() const;
};
#endif
