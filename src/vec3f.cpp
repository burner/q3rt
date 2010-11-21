#include "vec3f.h"

vec3f::vec3f(GLfloat x_n, GLfloat y_n, GLfloat z_n) {
	x = x_n;
	y = y_n;
	z = z_n;
}

vec3f::vec3f() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

vec3f* vec3f::operator+ (const vec3f &second) const {
	return new vec3f(x+second.x, y+second.y, z+second.z);
}

vec3f& vec3f::operator+= (const vec3f &second) {
	x += second.x;
	y += second.y;
	z += second.z;
	return *this;
}

vec3f* vec3f::operator- (const vec3f &second) const {
	return new vec3f(x-second.x, y-second.y, z-second.z);
}

vec3f& vec3f::operator-= (const vec3f &second) {
	x -= second.x;
	y -= second.y;
	z -= second.z;
	return *this;
}

vec3f* vec3f::operator* (const vec3f &second) const {
	return new vec3f( (y * second.z)-(z * second.y),
					(z * second.x)-(x * second.z),
					(x * second.y)-(y * second.x));
}

	
vec3f& vec3f::operator*= (const vec3f &second) {
	GLfloat tx = (y * second.z)-(z * second.y);
	GLfloat ty = (z * second.x)-(x * second.z);
	GLfloat tz = (x * second.y)-(y * second.x);
	x = tx; y = ty; z = tz;
	return *this;
}
/*
vec3f* vec3f::operator* (float scalar) const {
	return new vec3f(x*scalar, y*scalar, z*scalar);
}

vec3f& vec3f::operator*= (GLfloat scalar) {
	x*=scalar;
	y*=scalar;
	z*=scalar;
	return *this;
}*/

vec3f* vec3f::operator* (const float &scalar) const {
	return new vec3f(x*scalar, y*scalar, z*scalar);
}

vec3f& vec3f::operator*= (const GLfloat &scalar) {
	x*=scalar;
	y*=scalar;
	z*=scalar;
	return *this;
}

vec3f* vec3f::operator+ (const vec3f *second) const {
	return new vec3f(x+second->x, y+second->y, z+second->z);
}

vec3f& vec3f::operator+= (const vec3f *second) {
	x += second->x;
	y += second->y;
	z += second->z;
	return *this;
}

vec3f* vec3f::operator- (const vec3f *second) const {
	return new vec3f(x-second->x, y-second->y, z-second->z);
}

vec3f& vec3f::operator-= (const vec3f *second) {
	x -= second->x;
	y -= second->y;
	z -= second->z;
	return *this;
}

vec3f* vec3f::operator* (const vec3f *second) const {
	return new vec3f( (y * second->z)-(z * second->y),
					(z * second->x)-(x * second->z),
					(x * second->y)-(y * second->x));
}

	
vec3f& vec3f::operator*= (const vec3f *second) {
	GLfloat tx = (y * second->z)-(z * second->y);
	GLfloat ty = (z * second->x)-(x * second->z);
	GLfloat tz = (x * second->y)-(y * second->x);
	x = tx; y = ty; z = tz;
	return *this;
}

GLfloat vec3f::length() const {
	return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

std::string* vec3f::toXML() const {
	std::string* tmp = new std::string("x=\"");
	tmp->push_back(x);
	tmp->append("\" y=\"");
	tmp->push_back(y);
	tmp->append("\" z=\"");
	tmp->push_back(z);
	tmp->append("\"");
	return tmp;
}
