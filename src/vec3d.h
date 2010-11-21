#ifndef VEC3D
#define VEC3D
#include <string>
#include <stdlib.h>
//#include "../util/geomUtil.h"

/** The basic idea behind this is to provide a basic
 * vector class containing three doubles.
 * The forward declaration in this header file should
 * give you all the info you need.
 */
class vec3d {
	private:
		double v[3];
		bool compare(const vec3d &second);
	public:
		double x;
		double y;
		double z;

		vec3d(double x_n, double y_n, double z_n);
		vec3d(vec3d *o);
		vec3d();
		~vec3d();

		//addition subtraction
		vec3d* operator+ (const vec3d &second);
		vec3d* operator+ (const vec3d *second);
		void operator+= (const vec3d &second);
		vec3d* operator- (const vec3d &second);
		void operator-= (const vec3d &second);

		//corssprocut
		vec3d* operator* (const vec3d &second);
		void operator*= (const vec3d &second);

		//scalar multiplcation
		vec3d* operator* (const double &scalar);
		void operator*= (const double &scalar);
		double operator[](int i);
		double length();

		void normalize();
		vec3d normReturn();
		vec3d* cubeSphere();

		bool operator!=(const vec3d &second);
		bool operator==(const vec3d &second);

		std::string* toXML();

		void print();
		bool test();
};
#endif
