#include "vec3d.h"
#include <math.h>
#include <iostream>
#include "../util/geomUtil.h"

vec3d::vec3d(double x_n, double y_n, double z_n) {
	x = x_n;
	y = y_n;
	z = z_n;
	v[0] = x_n;
	v[1] = y_n;
	v[2] = z_n;
}

vec3d::vec3d() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

vec3d::vec3d(vec3d *o) {
	v[0] = x = o->x;
	v[1] = y = o->y;
	v[2] = z = o->z;
}

vec3d::~vec3d() {
	//std::cout<<"vec3d delete"<<std::endl;
}

bool vec3d::compare(const vec3d &second) {
	//if() std::cout<<"alarm"<<std::endl;
	return floor(x) == floor(second.x) &&
	   floor(y) == floor(second.y) &&
	   floor(z) == floor(second.z);
}

bool vec3d::operator==(const vec3d &second) {
	return this->compare(second);
}

void vec3d::print() {
	std::cout<<std::fixed<<x<<" "<<y<<" "<<z<<std::endl;
}

bool vec3d::operator!=(const vec3d &second) {
	std::cout<<x<<" "<<y<<" "<<z<<" second "<<second.x<<" "<<second.y<<" "<<second.z<<std::endl;
	return !this->compare(second);
}

vec3d* vec3d::operator+ (const vec3d &second) {
	return new vec3d(x+second.x, y+second.y, z+second.z);
}

vec3d* vec3d::operator+ (const vec3d *second) {
	return new vec3d(x+second->x, y+second->y, z+second->z);
}

void vec3d::operator+= (const vec3d &second) {
	x += second.x;
	y += second.y;
	z += second.z;
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

vec3d* vec3d::operator- (const vec3d &second) {
	return new vec3d(x-second.x, y-second.y, z-second.z);
}

void vec3d::normalize() {
	double l = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	//std::cout<<"l = "<<l<<std::endl;
	if(equals(l, 0.0)) {
		//std::cout<<"found equal"<<std::endl;
		return;
	}
	x /= l;
	y /= l;
	z /= l;
}

vec3d vec3d::normReturn() {
	double l = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	double nx = x / l;
	double ny = y / l;
	double nz = z / l;
	return vec3d(nx, ny, nz);
}

void vec3d::operator-= (const vec3d &second) {
	x -= second.x;
	y -= second.y;
	z -= second.z;
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

vec3d* vec3d::operator* (const vec3d &second) {
	return new vec3d( (y * second.z)-(z * second.y),
					(z * second.x)-(x * second.z),
					(x * second.y)-(y * second.x));
}

vec3d* vec3d::cubeSphere() {
	x = x * sqrt(1.0f - y * y * 0.5f - z * z * 0.5f + y * y * z * z / 3.0f);
	y = y * sqrt(1.0f - z * z * 0.5f - x * x * 0.5f + z * z * x * x / 3.0f);
	z = z * sqrt(1.0f - x * x * 0.5f - y * y * 0.5f + x * x * y * y / 3.0f);
	return this;
}
	
void vec3d::operator*= (const vec3d &second) {
	double tx = (y * second.z)-(z * second.y);
	double ty = (z * second.x)-(x * second.z);
	double tz = (x * second.y)-(y * second.x);
	x = tx; y = ty; z = tz;
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

vec3d* vec3d::operator* (const double &scalar) {
	return new vec3d(x*scalar, y*scalar, z*scalar);
}
	
void vec3d::operator*= (const double &scalar) {
	x*=scalar;
	y*=scalar;
	z*=scalar;
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

double vec3d::operator[](int i) {
	return v[i];
}

double vec3d::length() {
	return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

std::string* vec3d::toXML() {
	std::string* tmp = new std::string("x=\"");
	tmp->push_back(x);
	tmp->append("\" y=\"");
	tmp->push_back(y);
	tmp->append("\" z=\"");
	tmp->push_back(z);
	tmp->append("\"");
	return tmp;
}

bool vec3d::test() {
	return x == 0.0 && y == 0.0 && z == 0.0;
}
