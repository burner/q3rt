#include "Focus.h"
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#include "Camera.h"
//#include "../util/geomUtil.h"
#include "../logger/Logger.h"
#include "../util/ShaderHandle.h"
#include "../prim/vec3q.h"
#include "../prim/vec3f.h"
#include "../tinyxml/tinyxml.h"

Focus::Focus() {
	std::string file("./content/eob/Focus.eob");
	TiXmlDocument doc(file);
	LOG("ObjInit")<<file<<Logger::endl;
	doc.LoadFile();
	TiXmlElement *root = doc.RootElement();
	
	//get name
	(*this).name = root->Attribute("name");

	//get shader stuff
	TiXmlElement *shaderRef = root->FirstChild("Shader")->ToElement();
	verRef = shaderRef->Attribute("Vertex");
	fragRef = shaderRef->Attribute("Fragment");
	initSphere();
}

void Focus::initSphere() {
	unsigned seg = 20;
	std::vector<vec3q> list;
	double f = 0.0;
	for(unsigned i = 0; i <= seg; i++) {
		list.push_back(vec3q(cos(f), sin(f), 0.0));
		f = f + (2 * PI/seg);
	}

	this->numV = list.size()*3;
	this->numPaint = list.size();
	this->vec = new float[this->numV];
	std::vector<vec3q>::const_iterator it = list.begin();
	int vecArrPoi = 0;
	for(; it != list.end(); it++) {
		this->vec[vecArrPoi + 0] = (*it).x;	
		this->vec[vecArrPoi + 1] = (*it).y;	
		this->vec[vecArrPoi + 2] = (*it).z;	
		LOG("ObjInit")<<(*it).x<<" "<<(*it).y<<" "<<(*it).z<<Logger::endl;
		vecArrPoi += 3;
	}
	LOG("ObjInit")<<"Object Create BEGIN " + name<<Logger::endl;
	
	//read shader source
	verSource = readShader(verRef);
	LOG("ObjInit")<<verRef + " read"<<Logger::endl;
	fragSource = readShader(fragRef);
	LOG("ObjInit")<<fragRef + " read"<<Logger::endl;
	LOG("ObjInit")<<"read Shader done"<<Logger::endl;
	
	//create shader pointer
	verShader = glCreateShader(GL_VERTEX_SHADER);
	LOG("ObjInit")<<"Vertex Shader created"<<Logger::endl;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	LOG("ObjInit")<<"Fragment Shader created"<<Logger::endl;

	//bind source to pointer
	glShaderSource(verShader, 1, (const GLchar**)&verSource, 0);
	glShaderSource(fragShader, 1, (const GLchar**)&fragSource, 0);
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

	//attach shader
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, verShader);
	LOG("ObjInit")<<"Vertex Shader attached"<<Logger::endl;
	glAttachShader(shaderProgram, fragShader);
	LOG("ObjInit")<<"Fragment Shader attached"<<Logger::endl;

	//link shader
	glLinkProgram(shaderProgram);
	LOG("ObjInit")<<"Shader linked"<<Logger::endl;

	GLuint uniformDiffuse = glGetAttribLocation(shaderProgram, "Diffuse");
	glUniform1i(uniformDiffuse,0);
	
	//create vertex array
	glGenVertexArrays(1, &vaoS);
	LOG("ObjInit")<<"Vertex Array Created"<<Logger::endl;
	glGenBuffers(1, &vboS);
	LOG("ObjInit")<<"Vertex Buffer Created"<<Logger::endl;
	glBindVertexArray(vaoS);
	LOG("ObjInit")<<"Vertex Array bound"<<Logger::endl;
	
	//position vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboS);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(obj.vec), obj.vec, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, numV*sizeof(GLfloat), vec, GL_STATIC_DRAW);
	const GLuint positionIdx = glGetAttribLocation(shaderProgram, "in_Position");
	glVertexAttribPointer(positionIdx, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionIdx);
	LOG("ObjInit")<<"Vertex Array Filled and Attached"<<Logger::endl;
	LOG("ObjInit")<<"Focus created"<<Logger::endl;
}

Focus::~Focus() {
	delete[] this->vec;
}

void Focus::drawSphere(const vec3d *middle, const vec3f *rot, glm::mat4 view) {
	vec3q tmp = middle;
	vec3q sub = Camera::getCurrentPos();
	vec3q angle = Camera::getCurrentAngle();
	
	tmp+=sub;
	tmp.normalize() *= 20.0f;
	//tmp.normalize() *= 15.0;	
	//glm::mat4 ViewTranslate = glm::translate((float)tmp.x,(float)tmp.y,(float)tmp.z);
	//glm::mat4 ViewTranslate = glm::translate((float)0.0,(float)0.0,(float)-10.0);
	//glm::mat4 ViewRotateX = glm::rotate((float)-upAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::mat4 ViewRotateY = glm::rotate((float)-sideAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 ViewRotateZ = glm::rotate(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	view *= glm::translate((float)tmp.x, (float)tmp.y, (float)tmp.z);
	view *= glm::rotate((float)-angle.x, glm::vec3(1.0f, 0.0f, 0.0f)); 
	view *= glm::rotate((float)-angle.y, glm::vec3(0.0f, 1.0f, 0.0f));
	view *= glm::rotate((float)-angle.z, glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4 ViewRotateX = glm::rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::mat4 ViewRotateY = glm::rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 ViewRotateZ = glm::rotate(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4 View = ViewTranslate * ViewRotateX * ViewRotateY * ViewRotateZ;
	
	glUseProgram(this->shaderProgram);
	glm::mat4 Projection = Camera::sGetProjectionMatrix(0.1f, 20000.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->shaderProgram, "mvpmatrix"), 1, 
		GL_FALSE, glm::value_ptr(Projection*view));
	
	glBindVertexArray(vaoS);
	//LineWidth(0.5f);
	//std::cout<<"Focus drawn"<<std::endl;
	glDrawArrays(GL_LINE_LOOP,0,numPaint);
}
