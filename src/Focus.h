#ifndef FOCUS
#define FOCUS

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_projection.hpp"
#include "../glm/GLM_GTX_transform.h"

class vec3q;
class vec3d;
class vec3f;

class Focus {
	private:
		std::string name;
		unsigned int vertexCount;
		unsigned int floatCount;
		unsigned int shaderProgram;
		unsigned int vaoP, vboP;
		unsigned int vaoS, vboS;
		std::string verRef, fragRef;
		char *verSource, *fragSource;
		unsigned int vSize;
		unsigned int verShader, fragShader;
		int numV;
		int numPaint;
		float *vec;
		void initSphere();

	public:
		Focus();
		~Focus();
		void print();
		void drawSphere(const vec3d *middle, const vec3f *rot, glm::mat4 view);
};				
#endif
