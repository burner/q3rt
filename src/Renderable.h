#ifndef RENDERABLE
#define RENDERABLE

#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#include <string>

class RenderAble {
	public:
		virtual void draw(const GLfloat *projection) = 0;
		virtual float getRadius() = 0;
		virtual void update() = 0;
		virtual std::string getName() = 0;
		virtual bool isFacing() = 0;
};
#endif
