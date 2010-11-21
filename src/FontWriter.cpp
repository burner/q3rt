#include "FontWriter.h"

std::map<std::string, std::string> FontWriter::text;

bool FontWriter::active;

Obj* FontWriter::quad = NULL;
TTF_Font* FontWriter::font = NULL;

void FontWriter::initFontWriter() {
	if(TTF_Init()) {
		LOG("Fontwriter")<<"failed to init SDL_TTF"<<Logger::endl;
	}
	FontWriter::font = TTF_OpenFont("font/FreeSans.ttf", 16);
	if(FontWriter::font == NULL) {
		LOG("FontWriter")<<"Font couldn't be opened"<<Logger::endl;
	}
	FontWriter::quad = new Obj("font/a.eob");
	LOG("FontWriter")<<"Fontwriter init done"<<Logger::endl;
}

void FontWriter::toggleActive() {
	FontWriter::active = true;
}

void FontWriter::toggleDeactive() {
	FontWriter::active = false;
}

void FontWriter::toggle() {
	FontWriter::active = !FontWriter::active;
}

void FontWriter::write(std::string key, std::string text) {
	FontWriter::text.insert(std::make_pair(key, text ));
}

void FontWriter::render() {
	std::string toRender("Info");
	
	SDL_Surface *surface;
    SDL_Color foregroundColor, backgroundColor;
    
    /* White text on a black background */
    foregroundColor.r = 255;
    foregroundColor.g = 255;
    foregroundColor.b = 255;
    
    backgroundColor.r = 0;
    backgroundColor.g = 0;
    backgroundColor.b = 0;

	int posCount = 0;
	std::map<std::string, std::string>::iterator it;
	for(it = FontWriter::text.begin(); it != FontWriter::text.end() && FontWriter::active; it++) {
		//surface = TTF_RenderUTF8_Shaded(font, ((*it).first +" " + (*it).second).c_str(), foregroundColor);
		std::string text = (*it).first + " " + (*it).second;
		surface = TTF_RenderUTF8_Shaded(font, text.c_str(), foregroundColor, backgroundColor);
		
		int w = nextpoweroftwo(surface->w);
		int h = nextpoweroftwo(surface->h);
		SDL_Surface *intermediary = SDL_CreateRGBSurface(0, w, h, 24,
	            0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
		);
		SDL_BlitSurface(surface, 0, intermediary, 0);
		FontWriter::quad->setTexture(intermediary);
		SDL_FreeSurface(surface);
		SDL_FreeSurface(intermediary);
		float angle = 0.0f;
		GLfloat scale = 0.5f;
		glm::mat4 Projection = glm::perspective(45.0f, (GLfloat)1200/(GLfloat)800, 0.1f, 20000.0f);
		glm::mat4 ViewTranslate = glm::translate(-0.55f+(text.size()*0.008f),0.37f-(posCount*0.055f),-1.0f+(posCount*0.01f));
		glm::mat4 ViewRotateX = glm::rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 ViewRotateY = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 ViewRotateZ = glm::rotate(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 Scale = glm::scale(glm::vec3(scale*text.size(),2.0,2.0));
		glm::mat4 View =ViewTranslate * ViewRotateX * ViewRotateY * ViewRotateZ * Scale;
	
		glUseProgram(FontWriter::quad->getShaderHandle());
		glUniformMatrix4fv(glGetUniformLocation(FontWriter::quad->getShaderHandle(), "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(Projection*View));
	
		FontWriter::quad->draw((GLfloat*)glm::value_ptr(Projection*View));
		posCount++;
	}
	FontWriter::text.clear();
    
	
	//surface = TTF_RenderUTF8_Shaded(font, toRender.c_str(), foregroundColor, backgroundColor);
	//surface = TTF_RenderText_Blended(font, toRender.c_str(), foregroundColor);


	//SDL_Surface *t = IMG_Load("fontrender.bmp");
	//FontWriter::quad->setTexture(t);
	//SDL_FreeSurface(t);
}
