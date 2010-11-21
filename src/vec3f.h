#ifndef VEC3F
#define VEC3F
#include <iostream>
#include <math.h>
#include <string>
#include <GL3/gl3.h>

/** The basic idea behind this is to provide a basic
 * vector class containing three floats.
 * The forward declaration in this header file should
 * give you all the info you need.
 */
class vec3f {
	private:
	public:
		GLfloat x;
		GLfloat y;
		GLfloat z;

		vec3f(GLfloat x_n, GLfloat y_n, GLfloat z_n);
		vec3f();

		//addition subtraction
		vec3f* operator+ (const vec3f &second) const;
		vec3f& operator+= (const vec3f &second);
		vec3f* operator- (const vec3f &second) const;
		vec3f& operator-= (const vec3f &second);
		vec3f* operator+ (const vec3f *second) const;
		vec3f& operator+= (const vec3f *second);
		vec3f* operator- (const vec3f *second) const;
		vec3f& operator-= (const vec3f *second);

		//corssprocut
		vec3f* operator* (const vec3f &second) const;
		vec3f& operator*= (const vec3f &second);
		vec3f* operator* (const vec3f *second) const;
		vec3f& operator*= (const vec3f *second);

		//scalar multiplcation
		//vec3f* operator* (float scalar) const;
		//vec3f& operator*= (float scalar);
		vec3f* operator* (const GLfloat &scalar) const;
		vec3f& operator*= (const GLfloat &scalar);
		float length() const;

		std::string* toXML() const;
};
#endif
