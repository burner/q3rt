#include "geomUtil.h"

bool testAngle(vec3q angle, Quad *quad) {
	return true;
}

bool testSphereBox(double sph[4], double bxC[3], double bxS[3]) {
	double dmin = 0.0;
	for(int i = 0; i < 3; i++) {
		if(sph[i] < bxC[i]) {
			dmin += (sph[i] - bxC[i]) * (sph[i] - bxC[i]);
		} else if(sph[i] > (bxC[i]+ bxS[i])) {
			dmin += (sph[i] - (bxC[i] + bxS[i])) * (sph[i] - (bxC[i] + bxS[i]));
		}
	}
	return (dmin <= (sph[4] * sph[4]));
}

bool testSphereBox(vec3d sphP, double sphRad, vec3d boxC, vec3d boxS) {
	double dmin = 0.0;
	for(int i = 0; i < 3; i++) {
		if(sphP[i] < boxC[i]) {
			dmin = dmin + ((sphP[i] - boxC[i]) * (sphP[i] - boxC[i]));
		} else if(sphP[i] > (boxC[i]+ boxS[i])) {
			dmin = dmin + ((sphP[i] - (boxC[i] + boxS[i])) * (sphP[i] - (boxC[i] + boxS[i])));
		}
	}
	return (dmin <= (sphRad * sphRad));
}

bool testSphereInBox(vec3d sphP, double sphRad, vec3d boxC, vec3d boxS) {
	for(int i = 0; i < 3; i++) {
		if( (boxC[i]+boxS[i]) < (sphP[i] + sphRad) ||
			(boxC[i]) > (sphP[i] - sphRad) ) {
				return false;
		}
	}
	return true;
}

bool testSphereInBox(const vec3d *sphP, double sphRad, const vec3d *boxC, const vec3d *boxS) {
	vec3q sph = sphP;
	vec3q boC = boxC;
	vec3q boS = boxS;
	for(int i = 0; i < 3; i++) {
		if( (boC[i]+boS[i]) < (sph[i] + sphRad) ||
			(boC[i]) > (sph[i] - sphRad) ) {
				return false;
		}
	}
	return true;
}

vec3d* middleVec(const vec3d *a, const vec3d *b) {
	double x = (a->x + b->x) / 2.0;
	double y = (a->y + b->y) / 2.0;
	double z = (a->z + b->z) / 2.0;
	return new vec3d(x,y,z);
}

float distance(vec3f *a, vec3f *b) {
	return sqrt(pow(b->x - a->x, 2) + pow(b->y - a->y, 2) + pow(b->z - a->z, 2));
}

double distance(const vec3d *a, const vec3d *b) {
	return sqrt(pow(b->x - a->x, 2) + pow(b->y - a->y, 2) + pow(b->z - a->z, 2));
}

double distance(const vec3q& a, const vec3q& b) {
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
}

double distance(const vec3q *a, const vec3q *b) {
	return sqrt(pow(b->x - a->x, 2) + pow(b->y - a->y, 2) + pow(b->z - a->z, 2));
}

double distance(vec3d *a, vec3d *b) {
	return sqrt(pow(b->x - a->x, 2) + pow(b->y - a->y, 2) + pow(b->z - a->z, 2));
}

// S(t) = (1 - t)^2 * P_0 + 2t(1 - t)P_1 + t^2 * P_2; t E [0,1]
vec3d* bSpline(vec3d *a, vec3d *b, vec3d *c, float t) {
	vec3d *a1 = (*a) * pow(1 - t, 2);
	vec3d *b1 = (*b) * (2 * t * (1 - t));
	vec3d *c1 = (*c) * pow(t, 2);
	vec3d *r1 = (*a1) + (*b1);
	vec3d *r2 = (*r1) + (*c1);
	delete a1;
	delete b1;
	delete c1;
	delete r1;

	return r2;	
}

vec3d* calcSphereProj(vec3d *tl, vec3d *br, vec3d *mid, double scale) {
	//midpoint P = A + (B-A)*t where t 0.0-1.0
	double x = tl->x + ((br->x - tl->x) * 0.5);
	double y = tl->y + ((br->y - tl->y) * 0.5);
	double z = tl->z + ((br->z - tl->z) * 0.5);
	x = x * sqrt(1.0f - y * y * 0.5f - z * z * 0.5f + y * y * z * z / 3.0f);
	y = y * sqrt(1.0f - z * z * 0.5f - x * x * 0.5f + z * z * x * x / 3.0f);
	z = z * sqrt(1.0f - x * x * 0.5f - y * y * 0.5f + x * x * y * y / 3.0f);
	x *= scale;
	y *= scale;
	z *= scale;
	x += mid->x;	           
	y += mid->y;	           
	z += mid->z;	           
	std::cout<<x<<" "<<y<<" "<<z<<std::endl;

	//directionb vector between point P and mid of sphere
	return new vec3d(mid->x + x, mid->y + y, mid->z + z);
}

vec3d crossProduct(vec3d *u, vec3d *v) {
	vec3d res;
	res.x = u->y*v->z - u->z * v->y;
	res.y = u->z*v->x - u->x*v->z;
	res.z = u->x*v->y - u->y*v->x;
	return res;
}

vec3d skalarMulti(vec3d a, double ska) {
	return vec3d(a.x * ska,
		a.y * ska,
		a.z * ska);
}

vec3d normalize(vec3d a) {
	double l = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
	return vec3d(a.x / l, a.y / l, a.z / l);
}

vec3d subtract(vec3d a, vec3d b) {
	return vec3d(a.x - b.x, a.y - a.y, a.z - a.z);
}

vec3d add(vec3d a, vec3d b) {
	return vec3d(a.x + b.x, a.y + a.y, a.z + a.z);
}

double angle(vec3d a, vec3d b) {
	//double xy = a.x * b.x + a.y * b.y + a.z * b.z;
	//return acos(xy / (a.length() * b.length())) * PI / 180;
	double vn = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
	double wn = sqrt(pow(b.x, 2) + pow(b.y, 2) + pow(b.z, 2));
	double vw = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	//std::cout<<std::fixed<<"vn "<<vn<<": wn "<<wn<<": vw "<<vw<<std::endl;
	double t = vw / (vn * wn);
	//std::cout<<"t "<<t<<std::endl;
	return acos(t) * 180 / 3.1415;
}

int getExpoBase2(double d) {
	int i = 0;
	((short *)(&i))[0] = (((short *)(&d))[3] & (short)32752); // _123456789ab____ & 0111111111110000
	return (i >> 4) - 1023;
}

bool equals(double d1, double d2) {
	if (d1 == d2)
		return true;
	int e1 = getExpoBase2(d1);
	int e2 = getExpoBase2(d2);
	int e3 = getExpoBase2(d1 - d2);
	if ((e3 - e2 < -48) && (e3 - e1 < -48))
		return true;
	return false;
}

double reAngle(double old, double pp, char an) {
	old += pp;
	switch(an) {
		case 'x':
		case 'z':
		case 'y':
			if(old > 180.0) {
				old = -180.0 + (old - 180.0);
			} else if(old < -180) {
				old = 180.0 + (old + 180);
			}
			return old;
		/*case 'y':
			if(old > 90.0) {
				old = -90.0 + (old - 90.0);
			} else if(old < -90) {
				old = 90.0 + (old + 90);
			}
			return old;
		*/
	}	
	return old;
}
