#include <iostream>
#include <math.h>
#include <string>
#include <GL3/gl3.h>
#ifndef VEC2F
#define VEC2F

/** This class is used in the ObjLoader to store the info
 * from the .obj files. Very basic class more or less the
 * same as vec3d.
 * The + - * operator create new Objects on the Heap so
 * nead to delete those.
 */
class vec2f {
	private:
	public:
		GLfloat x;
		GLfloat y;

		vec2f(GLfloat x_n, GLfloat y_n);

		//addition subtraction
		vec2f* operator+ (const vec2f &second) const;
		void operator+= (const vec2f &second);
		vec2f* operator- (const vec2f &second) const;
		void operator-= (const vec2f &second);

		//corssprocut
		vec2f* operator* (const vec2f &second) const;
		void operator*= (const vec2f &second);

		//scalar multiplcation
		vec2f* operator* (const float &scalar) const;
		void operator*= (const float &scalar);
		float length() const;

		std::string* toXML() const;
};
#endif
