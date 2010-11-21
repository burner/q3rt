#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#include "ObjLoader.h"
#include "vec3f.h"
#include "ShaderHandle.h"

#ifndef OBJ
#define OBJ
#include "Renderable.h"

class Obj : public RenderAble {
	protected:
		bool created;
		bool geo;
		int vSize, nSize, tSize, cSize;
		GLuint vertexCount;
		GLuint floatCount;
		GLuint shaderProgram;
		GLuint vao, vbo[3];
		std::string objFile;
		std::string verRef, fragRef, geomRef;
		GLchar *verSource, *fragSource, *geomSource;
		GLuint verShader, fragShader, geomShader;
		GLuint diffuse0, diffuse1, diffuse2, diffuse3;
		std::string diffuse0fn, diffuse1fn, diffuse2fn, *diffuse3fn;
		bool radiusCreated;

	public:
		float radius;
		std::string name;
		vec3f *pos;
		vec3f *rot;
		Obj(const std::string file);
		Obj();
		~Obj();
		void draw(const GLfloat *projection);
		GLuint getShaderHandle() const;
		GLuint getVertexCount() const;
		float getRadius();
		std::string getString();
		std::string getName();
		void update();
		virtual bool isFacing();
};
#endif		
