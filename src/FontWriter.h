#include <map>
#include <math.h>
#include <string>
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "../prim/vec3f.h"
#include <SDL/SDL.h>

#include "../util/ShaderHandle.h"
#include "../util/utils.h"

#include "Obj.h"

#include "../../logger/Logger.h"

//#include "../defines.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_projection.hpp"
#include "../glm/gtx/type_ptr.hpp"
#include "../glm/GLM_GTX_transform.h"

#ifndef FONTWRITER
#define FONTWRITER
class FontWriter {
	public:
		static void initFontWriter();
		static void write(std::string key, std::string text);
		static void render();

		static void toggleActive();
		static void toggleDeactive();
		static void toggle();

	private:
		static bool active;
		static TTF_Font *font;
		static Obj *quad;
		static std::map<std::string, std::string> text;
	

};
#endif 
