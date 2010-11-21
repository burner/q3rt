#include "SkyBox.h"

SkyBox::SkyBox() {
	created = false;
	geo = false;
	name = "SkyBox";
	verRef = "shader/skybox.vert";
	fragRef = "shader/skybox.frag";
	diffuse0fn = "content/textures/starfield2.jpg";	

	LOG("ObjInit")<<"Skybox Create BEGIN " + name<<Logger::endl;

	//read shader source
	verSource = readShader(verRef);
	LOG("ObjInit")<<verRef + " read"<<Logger::endl;
	fragSource = readShader(fragRef);
	LOG("ObjInit")<<fragRef + " read"<<Logger::endl;
	if(geo) {
		geomSource = readShader(geomRef);
		LOG("ObjInit")<<geomRef<<Logger::endl;
	}
	LOG("ObjInit")<<"read Shader done"<<Logger::endl;
	
	//create shader pointer
	verShader = glCreateShader(GL_VERTEX_SHADER);
	LOG("ObjInit")<<"Vertex Shader created"<<Logger::endl;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	LOG("ObjInit")<<"Fragment Shader created"<<Logger::endl;
	if(geo) {
		geomShader = glCreateShader(GL_GEOMETRY_SHADER);
		LOG("ObjInit")<<"Geometry Shader created"<<Logger::endl;
	}

	//bind source to pointer
	glShaderSource(verShader, 1, (const GLchar**)&verSource, 0);
	glShaderSource(fragShader, 1, (const GLchar**)&fragSource, 0);
	if(geo) {
		glShaderSource(geomShader, 1, (const GLchar**)&geomSource, 0);
	}
	LOG("ObjInit")<<"Shader bind"<<Logger::endl;

	//compile shader
	glCompileShader(verShader);
	LOG("ObjInit")<<"Vertex Shader compiled"<<Logger::endl;
	GLsizei len;
	GLchar logf[10000];
	glGetShaderInfoLog(verShader, ((GLsizei)10000), &len, logf);
	for(GLsizei i = 0; i < len; i++) std::cout<<logf[i];
	glCompileShader(fragShader);
	LOG("ObjInit")<<"Fragment Shader compiled"<<Logger::endl;
	glGetShaderInfoLog(fragShader, ((GLsizei)10000), &len, logf);
	for(GLsizei i = 0; i < len; i++) std::cout<<logf[i];
	if(geo) {
		glCompileShader(geomShader);
		LOG("ObjInit")<<"Geometry Shader compiled"<<Logger::endl;
	}

	//attach shader
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, verShader);
	LOG("ObjInit")<<"Vertex Shader attached"<<Logger::endl;
	glAttachShader(shaderProgram, fragShader);
	LOG("ObjInit")<<"Fragment Shader attached"<<Logger::endl;
	if(geo) {
		glAttachShader(shaderProgram, geomShader);
		LOG("ObjInit")<<"Geometry Shader attached"<<Logger::endl;
	}

	//link shader
	glLinkProgram(shaderProgram);
	LOG("ObjInit")<<"Shader linked"<<Logger::endl;

	GLuint uniformDiffuse = glGetAttribLocation(shaderProgram, "Diffuse");
	glUniform1i(uniformDiffuse,0);

	float exp = 500;
	vSize = 108;
	float vec[] = {
		//left
		-exp, exp, exp, -exp, exp, -exp, -exp, -exp, exp,
		-exp, exp, -exp, -exp, -exp, -exp, -exp, -exp, exp,
		
		//back
		-exp, exp,-exp, exp, exp, -exp, -exp, -exp, -exp,
		exp, exp, -exp, exp, -exp, -exp, -exp, -exp, -exp,

		//right
		exp, exp, -exp, exp, exp, exp, exp, -exp, -exp,
		exp, exp, exp, exp, -exp, exp, exp, -exp, -exp,

		//front
		exp, exp, exp, -exp, exp, exp, exp, -exp, exp,
		-exp, exp, exp, -exp, -exp, exp, exp, -exp, exp,

		//top
		-exp, exp, exp, exp, exp, exp, -exp, exp, -exp,
		exp, exp, exp, exp, exp, -exp, -exp, exp, -exp,
		
		//bottom
		-exp, -exp, -exp, exp, -exp, -exp, -exp, -exp, exp,
		exp, -exp, -exp, exp, -exp, exp, -exp, -exp, exp };

	tSize = 96;
	float tex[] = {
		//left
		0.0f, 0.25f, 0.25f, 0.25f, 0.0f, 0.5f,
		0.5f, 0.25f, 0.25f, 0.5f, 0.0f, 0.5f,	
		
		//back
		0.25f, 0.25f, 0.5f, 0.25f, 0.25f, 0.5f,
		0.5f, 0.25f, 0.5f, 0.5f, 0.25f, 0.5f,	

		//right
		0.5f, 0.25f, 0.75f, 0.25f, 0.5f, 0.5f,
		0.75f, 0.25f, 0.75f, 0.5f, 0.5f, 0.5f,

		//front
		0.75f, 0.25f, 1.0f, 0.25f, 0.75f, 0.5f,
		1.0f, 0.25f, 1.0f, 0.5f, 0.75f, 0.5f,

		//top
		0.25f, 0.0f, 0.5f, 0.0f, 0.25f, 0.25f,
		0.5f, 0.0f, 0.5f, 0.25f, 0.25f, 0.25f,

		//bottom
		0.25f, 0.5f, 0.5f, 0.5f, 0.25f, 0.75f,
		0.5f, 0.5f, 0.5f, 0.75f, 0.25f, 0.75f,
	}; 

	//floatCount = obj.floatCount;
	//create vertex array
	glGenVertexArrays(1, &vao);
	LOG("ObjInit")<<"Vertex Array Created"<<Logger::endl;
	glGenBuffers(3, vbo);
	LOG("ObjInit")<<"Vertex Buffer Created"<<Logger::endl;
	glBindVertexArray(vao);
	LOG("ObjInit")<<"Vertex Array bound"<<Logger::endl;
	
	//position vertices
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(obj.vec), obj.vec, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vSize*sizeof(GLfloat), vec, GL_STATIC_DRAW);
	const GLuint positionIdx = glGetAttribLocation(shaderProgram, "in_Position");
	glVertexAttribPointer(positionIdx, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionIdx);
	LOG("ObjInit")<<"Vertex Array Filled and Attached"<<Logger::endl;

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(obj.col), obj.col, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vSize*sizeof(GLfloat), vec, GL_STATIC_DRAW);
	const GLuint norIdx = glGetAttribLocation(shaderProgram, "in_Nor");
	glVertexAttribPointer(norIdx, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(norIdx);
	LOG("ObjInit")<<"Normal Array Filled and Attached"<<Logger::endl;
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, tSize*sizeof(GLfloat), tex, GL_STATIC_DRAW);
	const GLuint texIdx = glGetAttribLocation(shaderProgram, "in_Tex");
	glVertexAttribPointer(texIdx, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texIdx);
	LOG("ObjInit")<<"Texture Array Filled and Attached"<<Logger::endl;
	
	SDL_Surface *texImage = IMG_Load(diffuse0fn.c_str());
	SDL_Surface *texImageDis = SDL_DisplayFormat(texImage);
	//SDL_PixelFormat *pixf = SDL_GetVideoSurface()->format;
	//int w = texImage->w;
	//int h = texImage->h;
	//SDL_Surface *image = SDL_CreateRGBSurface( SDL_SWSURFACE, w, h, 32, pixf->Bmask, pixf->Gmask, pixf->Rmask, pixf->Amask );
	//SDL_BlitSurface(texImage, NULL, image, NULL);
	// get the number of channels in the SDL surface
	GLint nOfColors = texImage->format->BytesPerPixel;
	GLenum texture_format;
	if (nOfColors == 4) {   // contains an alpha channel
	        if (texImage->format->Rmask == 0x000000ff)
	                texture_format = GL_RGBA;
	        else
	                texture_format = GL_BGRA;
	} else if (nOfColors == 3) {   // no alpha channel
	        if (texImage->format->Rmask == 0x000000ff)
	                texture_format = GL_RGB;
	        else
	                texture_format = GL_BGR;
	} else {
	        LOG("ObjIns")<<"warning: the image is not truecolor..  this will probably break\n"<<Logger::endl;
	        // this error should not go unhandled
	}
	std::cout<<nOfColors<<" "<<texture_format<<std::endl;

	//SDL_FreeSurface(texImage);
	LOG("ObjInit")<<"Texture loaded = " + diffuse0fn<<Logger::endl;

	//TTF_Font *font = TTF_OpenFont("FreeSans.ttf", 20);
	//SDL_Color co;
	//co.r = 255;
	//co.g = 155;
	//co.b = 255;
	//SDL_Surface *fontSurface = TTF_RenderText_Blended(font, "H", co);
	//double a = log(fontSurface->w)/log(2);
	//double b = log(fontSurface->h)/log(2);

	//GLint w =(int) ((pow(2,ceil(a)))+0.5);
	//GLint h =(int) ((pow(2,ceil(b)))+0.5);

	//SDL_Surface *fontD = SDL_CreateRGBSurface(0,w, h,32,
	//	0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	//SDL_BlitSurface(fontSurface, 0, fontD, 0);

	//SDL_SaveBMP(fontSurface, "font.bmp");
	//SDL_SaveBMP(fontD, "font1.bmp");

	glGenTextures( 1, &diffuse0 );
	glActiveTexture(GL_TEXTURE0);
	glBindTexture( GL_TEXTURE_2D, diffuse0 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, texImage->w, texImage->h, 
	0, GL_RGB, GL_UNSIGNED_BYTE, texImage->pixels);
	//glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, texImageDis->w, texImageDis->h, 
	//0, texture_format, GL_UNSIGNED_BYTE, texImageDis->pixels);
	LOG("ObjInit")<<"Texture bound"<<Logger::endl;
	
	created = true;
	LOG("ObjInit")<<"Skybox Create END " + name<<Logger::endl;
}

float SkyBox::getRadius() {
	return 500.0f;
}

void SkyBox::update() {

}

std::string SkyBox::getName() {
	return std::string("SkyBox");
}

void SkyBox::draw(const GLfloat *proj) {
	//std::cout<<"Skybox renderpass"<<std::endl;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse0);
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvpmatrix"), 1, GL_FALSE, proj);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,vSize);
}

bool SkyBox::isFacing() {
	return true;
}
