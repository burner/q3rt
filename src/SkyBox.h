#ifndef SKYBOX
#define SKYBOX

#include "Obj.h"
#include <string>

class SkyBox : public Obj {
	public:
		SkyBox();
		virtual void draw(const GLfloat *projection);
		void draw();
		float getRadius();
		void update();
		std::string getName();
		bool isFacing();
};
#endif
