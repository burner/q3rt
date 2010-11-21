#ifndef RENDERER
#define RENDERER

#include <vector>
#include <stack>
#include "Obj.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_projection.hpp"
#include "../glm/GLM_GTX_transform.h"
#include "../management/Octree.h"
#include "FontWriter.h"
#include <algorithm>
//#include "../util/util.h"
#include "Camera.h"
#include "../management/ObjInsManager.h"
#include <GL3/gl3.h>

class ObjIns;
bool ObjDistanceCompare(const ObjIns *a, const ObjIns *b);


class Renderer {
	private:
		static std::stack<glm::mat4> viewStack;
		//static glm::mat4 Projection;
		static std::vector<ObjIns*> inFrustum;
		static float angle;
		static int width;
		static int height;
		static void pushViewMatrix(glm::mat4 mat);
		static glm::mat4 popViewMatrix();
		static void sortByDistance();

	public:
		static void init(float angle, int height, int width);
		static void appendObjInsToRender(ObjIns *obIns);
		static void render();
};
#endif
