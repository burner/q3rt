#include "vec3q.h"
#include <math.h>
#include <iostream>
#include "../util/geomUtil.h"
#include "../util/infoPrint.h"

vec3q::vec3q(double x_n, double y_n, double z_n) {
	x = x_n;
	y = y_n;
	z = z_n;
}

vec3q::vec3q() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

vec3q::vec3q(const vec3q &o) {
	*this = o;
}

vec3q::vec3q(const vec3d *o) {
	*this = o;
}

vec3q::vec3q(const vec3d o) {
	*this = o;
}

vec3d* vec3q::getVec3d() {
	return new vec3d(x,y,z);
}

vec3q& vec3q::operator=(const vec3q &o) {
	if(this != &o) {
		this->x = o.x;		
		this->y = o.y;		
		this->z = o.z;		
	}
	return *this;
}

vec3q& vec3q::operator=(const vec3d *o) {
	this->x = o->x;		
	this->y = o->y;		
	this->z = o->z;		
	return *this;
}

vec3q& vec3q::operator=(const vec3d o) {
	this->x = o.x;		
	this->y = o.y;		
	this->z = o.z;		
	return *this;
}

vec3q::~vec3q() {
	//std::cout<<"vec3q delete"<<std::endl;
}

bool vec3q::compare(const vec3q &second) {
	//if() std::cout<<"alarm"<<std::endl;
	return floor(x) == floor(second.x) &&
	   floor(y) == floor(second.y) &&
	   floor(z) == floor(second.z);
}

bool vec3q::operator==(const vec3q &second) {
	return this->compare(second);
}

void vec3q::print() const {
	std::cout<<std::fixed<<x<<" "<<y<<" "<<z<<std::endl;
}

bool vec3q::operator!=(const vec3q &second) {
	std::cout<<x<<" "<<y<<" "<<z<<" second "<<second.x<<" "<<second.y<<" "<<second.z<<std::endl;
	return !this->compare(second);
}

vec3q vec3q::operator+ (const vec3q &second) const {
	double tx = this->x + second.x;
	double ty = this->y + second.y;
	double tz = this->z + second.z;
	return vec3q(tx, ty, tz);
}

void vec3q::operator+= (const vec3q &second) {
	x += second.x;
	y += second.y;
	z += second.z;
}

vec3q vec3q::operator- (const vec3q &second) const {
	return vec3q(x-second.x, y-second.y, z-second.z);
}

vec3q& vec3q::normalize() {
	double l = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	//std::cout<<"l = "<<l<<std::endl;
	if(equals(l, 0.0)) {
		//std::cout<<"found equal"<<std::endl;
		return *this;
	}
	x /= l;
	y /= l;
	z /= l;
	return *this;
}

double vec3q::dot(const vec3q &second) {
	return x * second.x + y * second.y + z *second.z;
}

vec3q& vec3q::cubeSphere() {
	//dprint<<x<<" "<<y<<" "<<z<<std::endl;
	double nx = x * sqrt(1.0 - (y * y * 0.5) - (z * z * 0.5) + (y * y * z * z / 3.0));
	double ny = y * sqrt(1.0 - (z * z * 0.5) - (x * x * 0.5) + (z * z * x * x / 3.0));
	double nz = z * sqrt(1.0 - (x * x * 0.5) - (y * y * 0.5) + (x * x * y * y / 3.0));
	this->x = nx;
	this->y = ny;
	this->z = nz;
	return *this;
}

vec3q vec3q::normReturn() const {
	double l = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	double nx = x / l;
	double ny = y / l;
	double nz = z / l;
	return vec3q(nx, ny, nz);
}

void vec3q::operator-= (const vec3q &second) {
	x -= second.x;
	y -= second.y;
	z -= second.z;
}

vec3q vec3q::operator* (const vec3q &second) const {
	return vec3q( (y * second.z)-(z * second.y),
					(z * second.x)-(x * second.z),
					(x * second.y)-(y * second.x));
}

	
void vec3q::operator*= (const vec3q &second) {
	double tx = (y * second.z)-(z * second.y);
	double ty = (z * second.x)-(x * second.z);
	double tz = (x * second.y)-(y * second.x);
	x = tx; y = ty; z = tz;
}

vec3q vec3q::operator* (const double &scalar) const {
	return vec3q(x*scalar, y*scalar, z*scalar);
}
	
void vec3q::operator*= (const double &scalar) {
	x*=scalar;
	y*=scalar;
	z*=scalar;
}

double vec3q::operator[](int i) const {
	if(i == 0) return x;
	else if(i == 1) return y;
	else if(i == 2) return z;
	else return 0.0;
}

double vec3q::length() const {
	return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

std::string* vec3q::toXML() const {
	std::string* tmp = new std::string("x=\"");
	tmp->push_back(x);
	tmp->append("\" y=\"");
	tmp->push_back(y);
	tmp->append("\" z=\"");
	tmp->push_back(z);
	tmp->append("\"");
	return tmp;
}

bool vec3q::test() const {
	return x == 0.0 && y == 0.0 && z == 0.0;
}
