#include "Camera.h"
std::map<std::string, Camera*> Camera::cameras;
unsigned Camera::newId = 0;
Camera* Camera::current;
bool Camera::KEY_STATE_FORWARD_BOOL;
bool Camera::KEY_STATE_BACKWARD_BOOL;
bool Camera::KEY_STATE_LEFT_BOOL;
bool Camera::KEY_STATE_RIGHT_BOOL;
bool Camera::KEY_STATE_UP_BOOL;
bool Camera::KEY_STATE_DOWN_BOOL;
bool Camera::KEY_STATE_RLEFT_BOOL;
bool Camera::KEY_STATE_TUP_BOOL;
bool Camera::KEY_STATE_RRIGHT_BOOL;
bool Camera::KEY_STATE_TLEFT_BOOL; 
bool Camera::KEY_STATE_TDOWN_BOOL;
bool Camera::KEY_STATE_TRIGHT_BOOL;

bool Camera::KEY_STATE_INCREASE_CAM_SPEED_BOOL;
bool Camera::KEY_STATE_DECREASE_CAM_SPEED_BOOL;
bool Camera::KEY_STATE_INCREASE_CAM_FOV_BOOL;
bool Camera::KEY_STATE_DECREASE_CAM_FOV_BOOL;
float Camera::nearGlobal = 0.1f;
float Camera::farGlobal = 25000.0f;

Camera::Camera(vec3f p, float angleXZ, float angleyYZ){
	//this->dis = Timer::reset(std::string("Camera"));
	this->pos = p;
	this->angleX = angleX;
	this->angleY = angleY;
	this->angleX = 0.0f;
	this->mouseState = false;
	this->keyState = 0;
	this->camSpeedFactor = 1.0;
	this->camSpeed = exp(this->camSpeedFactor-10);
}

Camera::Camera(vec3d p, int width, int height){
//this->dis = Timer::reset(std::string("Camera"));
	this->position = p;
	this->width = width;
	this->height = height;
	this->angleX = 0.0;
	this->angleY = 0.0;
	this->angleZ = 0.0;
	this->fovVar = 100.0f;
	ProjectionMatrix = glm::perspective(Camera::fovConst/fovVar, (GLfloat)width/(GLfloat)height, 
		nearGlobal, farGlobal);
	this->viewDir = glm::vec4(0.0, 0.0, 1.0, 0.0);	
	this->rightVec = glm::vec4(1.0, 0.0, 0.0, 0.0);	
	this->upVec = glm::vec4(0.0, 1.0, 0.0, 0.0);	
	this->mouseState = false;
	this->keyState = 0;
	this->camSpeedFactor = 5.0;
	this->camSpeed = exp(this->camSpeedFactor-10);
}

void Camera::setPos(double x, double y, double z) {
	//this->dis = Timer::reset(std::string("Camera"));
	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}

void Camera::setAngle(double x, double y, double z) {
	//this->dis = Timer::reset(std::string("Camera"));
	this->angleX = x;
	this->RotateX(x);
	this->angleY = y;
	this->RotateY(y);
	this->angleZ = z;
	this->RotateZ(z);
}

void Camera::applyCamMatrix() {
//	this->dis = Timer::reset(std::string("Camera"));
	bool wr = false;
	if(Camera::KEY_STATE_FORWARD_BOOL) {
		//std::cout<<"FORWARD ";
		wr = true;
		this->MoveForward((float)this->camSpeed);
	}
	if(Camera::KEY_STATE_BACKWARD_BOOL) {
		//std::cout<<"BACKWARD ";
		wr = true;
		this->MoveForward(-(float)this->camSpeed);
	}
	if(Camera::KEY_STATE_LEFT_BOOL) {
		//std::cout<<"LEFT ";
		wr = true;
		this->StrafeRight((float)this->camSpeed);	
	}
	if(Camera::KEY_STATE_RIGHT_BOOL) {
		//std::cout<<"RIGHT ";
		wr = true;
		this->StrafeRight(-(float)this->camSpeed);	
	}
	if(Camera::KEY_STATE_UP_BOOL) {
		//std::cout<<"UP ";
		wr = true;
		this->MoveUpward(-(float)this->camSpeed);
	}
	if(Camera::KEY_STATE_DOWN_BOOL) {
		//std::cout<<"DOWN ";
		wr = true;
		this->MoveUpward((float)this->camSpeed);
	}
	if(Camera::KEY_STATE_RLEFT_BOOL) { //g
		//std::cout<<"ROTATE_LEFT ";
		wr = true;
		this->RotateY(-1.0f);
	}
	if(Camera::KEY_STATE_TUP_BOOL) { //h
		//std::cout<<"TURN_UP ";
		wr = true;
		this->RotateX(1.0f);
	}
	if(Camera::KEY_STATE_RRIGHT_BOOL) { //j
		//std::cout<<"ROTATE_RIGHT ";
		wr = true;
		this->RotateY(1.0f);
	}
	if(Camera::KEY_STATE_TLEFT_BOOL) { //v
		//std::cout<<"TURN_LEFT ";
		wr = true;
		this->RotateZ(1.0f);
	}
	if(Camera::KEY_STATE_TDOWN_BOOL) { //b
		//std::cout<<"TURN_DOWN ";
		wr = true;
		this->RotateX(-1.0f);
	}
	if(Camera::KEY_STATE_TRIGHT_BOOL) { //n
		//std::cout<<"TURN_RIGHT ";
		wr = true;
		this->RotateZ(-1.0f);
	}
	if(Camera::KEY_STATE_INCREASE_CAM_SPEED_BOOL) {
		//std::cout<<"INCREASE"<<std::endl;
		this->camSpeedFactor += 0.01*dis;
		if(this->camSpeedFactor > 25.0) this->camSpeedFactor = 25.0;
		this->camSpeed = exp(this->camSpeedFactor-10);
	}
	if(Camera::KEY_STATE_DECREASE_CAM_SPEED_BOOL) {
		//std::cout<<"DECREASE"<<std::endl;
		this->camSpeedFactor -=0.01*dis;
		if(this->camSpeedFactor < 0.0) this->camSpeedFactor = 0.0;
		this->camSpeed = exp(this->camSpeedFactor-10);
	}
	if(Camera::KEY_STATE_INCREASE_CAM_FOV_BOOL) {
		this->fovVar -= 1.0f*dis;
		if(this->fovVar < 50.0f) {
			this->fovVar = 50.0f;
		}
		ProjectionMatrix = glm::perspective(Camera::fovConst/fovVar, (GLfloat)width/(GLfloat)height, 
			Camera::nearGlobal, Camera::farGlobal);
	}
	if(Camera::KEY_STATE_DECREASE_CAM_FOV_BOOL) {
		this->fovVar += 1.0f*dis;
		if(this->fovVar > 100000.0f) {
			this->fovVar = 100000.0f;
		}
		ProjectionMatrix = glm::perspective(Camera::fovConst/fovVar, (GLfloat)width/(GLfloat)height, 
			Camera::nearGlobal, Camera::farGlobal);
	}
	if(mouseState) {
		//std::cout<<"MOUSE "<<this->newAngleX<<" "<<this->newAngleY<<" ";
		wr = true;
		this->RotateX(this->newAngleX);
		this->RotateY(this->newAngleY);
	}
	if(wr) {
		//std::cout<<std::endl;
		////this->position.print();
		//this->upVec.print();
		//this->rightVec.print();
	}
	//return Camera::e3rtLookAt(this->position, this->viewDir, this->upVec);
	//glm::mat4 retMat = glm::rotate(this->angleY, glm::vec3(0.0f, 1.0f, 0.0f));	
	//retMat *= glm::rotate(this->angleZ, glm::vec3(0.0f, 0.0f, 1.0f));	
	//retMat *= glm::rotate(this->angleX, glm::vec3(1.0f, 0.0f, 0.0f));	
	//retMat *= glm::translate((float)(this->position.x), (float)(this->position.y), (float)(this->position.z));
	//retMat *= glm::translate(0.f, 0.f, 0.f);
	//return retMat;
}

glm::mat4 Camera::sGetCurrentCamMatrix(bool withPos) {
	return Camera::current->getCurrentCamMatrix(withPos);
}

glm::mat4 Camera::getCurrentCamMatrix(bool withPos) {
	if(withPos) {
		glm::mat4 retMat = glm::rotate(this->angleY, glm::vec3(0.0f, 1.0f, 0.0f));	
		retMat *= glm::rotate(this->angleZ, glm::vec3(0.0f, 0.0f, 1.0f));	
		retMat *= glm::rotate(this->angleX, glm::vec3(1.0f, 0.0f, 0.0f));	
		retMat *= glm::translate((float)(this->position.x), (float)(this->position.y), (float)(this->position.z));
		return retMat;
	} else {
		glm::mat4 retMat = glm::rotate(this->angleY, glm::vec3(0.0f, 1.0f, 0.0f));	
		retMat *= glm::rotate(this->angleZ, glm::vec3(0.0f, 0.0f, 1.0f));	
		retMat *= glm::rotate(this->angleX, glm::vec3(1.0f, 0.0f, 0.0f));	
		//retMat *= glm::translate(0.f, 0.f, 0.f);
		return retMat;
	}
}

void Camera::setKeyState(int key){
	//std::cout<<key<<std::endl;
	//keyState=key;
	switch(key) {
		case 'w':
			Camera::KEY_STATE_FORWARD_BOOL = !Camera::KEY_STATE_FORWARD_BOOL;
			break;
		case 's':
			Camera::KEY_STATE_BACKWARD_BOOL = !Camera::KEY_STATE_BACKWARD_BOOL;
			break;
		case 'a':
			Camera::KEY_STATE_LEFT_BOOL = !Camera::KEY_STATE_LEFT_BOOL;
			break;
		case 'd':
			Camera::KEY_STATE_RIGHT_BOOL = !Camera::KEY_STATE_RIGHT_BOOL;
			break;
		case 'e':
			Camera::KEY_STATE_UP_BOOL = !Camera::KEY_STATE_UP_BOOL;
			break;
		case 'q':
			Camera::KEY_STATE_DOWN_BOOL = !Camera::KEY_STATE_DOWN_BOOL ;
			break;
		case 'g':
			Camera::KEY_STATE_RLEFT_BOOL = !Camera::KEY_STATE_RLEFT_BOOL ;
			break;
		case 'h':
			Camera::KEY_STATE_TUP_BOOL = !Camera::KEY_STATE_TUP_BOOL ;
			break;
		case 'j':
			Camera::KEY_STATE_RRIGHT_BOOL = !Camera::KEY_STATE_RRIGHT_BOOL ;
			break;
		case 'v':
			Camera::KEY_STATE_TLEFT_BOOL = !Camera::KEY_STATE_TLEFT_BOOL ;
			break;
		case 'b':
			Camera::KEY_STATE_TDOWN_BOOL = !Camera::KEY_STATE_TDOWN_BOOL ;
			break;
		case 'n':
			Camera::KEY_STATE_TRIGHT_BOOL = !Camera::KEY_STATE_TRIGHT_BOOL ;
			break;
		case ',':
			Camera::KEY_STATE_INCREASE_CAM_SPEED_BOOL = !Camera::KEY_STATE_INCREASE_CAM_SPEED_BOOL;
			break;
		case 'm':
			Camera::KEY_STATE_DECREASE_CAM_SPEED_BOOL = !Camera::KEY_STATE_DECREASE_CAM_SPEED_BOOL;
			break;
		case Camera::KEY_STATE_INCREASE_CAM_FOV:
			Camera::KEY_STATE_INCREASE_CAM_FOV_BOOL = !Camera::KEY_STATE_INCREASE_CAM_FOV_BOOL;
			break;
		case Camera::KEY_STATE_DECREASE_CAM_FOV:
			Camera::KEY_STATE_DECREASE_CAM_FOV_BOOL = !Camera::KEY_STATE_DECREASE_CAM_FOV_BOOL;
			break;
	}
}

void Camera::setMouseState(bool s){
	this->mouseState = s;
}

void Camera::Move(vec3d direction) {
	this->position.x += direction.x;
	this->position.y += direction.y;
	this->position.z += direction.z;
}

void Camera::RotateX(float angle) {
	float aAngle = angle * dis / 10.0f;
	angleX = reAngle(angleX, aAngle, 'x');
	glm::mat4 rot;
	rot *= glm::rotate(-this->angleX, glm::vec3(1.0f, 0.0f, 0.0f));	
	rot *= glm::rotate(-this->angleY, glm::vec3(0.0f, 1.0f, 0.0f));	
	rot *= glm::rotate(-this->angleZ, glm::vec3(0.0f, 0.0f, 1.0f));	

	this->viewDir = rot*glm::vec4(0.0, 0.0, 1.0, 0.0);
	this->rightVec = rot*glm::vec4(1.0, 0.0, 0.0, 0.0);
	this->upVec = rot*glm::vec4(0.0, 1.0, 0.0, 0.0);
}

void Camera::RotateZ(float angle) {
	float aAngle = angle * dis / 10.0f;
	angleZ = reAngle(angleZ, aAngle, 'z');
	glm::mat4 rot;
	rot *= glm::rotate(-this->angleX, glm::vec3(1.0f, 0.0f, 0.0f));	
	rot *= glm::rotate(-this->angleY, glm::vec3(0.0f, 1.0f, 0.0f));	
	rot *= glm::rotate(-this->angleZ, glm::vec3(0.0f, 0.0f, 1.0f));	

	this->viewDir = rot*glm::vec4(0.0, 0.0, 1.0, 0.0);
	this->rightVec = rot*glm::vec4(1.0, 0.0, 0.0, 0.0);
	this->upVec = rot*glm::vec4(0.0, 1.0, 0.0, 0.0);
}

void Camera::RotateY(float angle) {
	float aAngle = angle * dis / 10.0f;
	angleY = reAngle(angleY, aAngle, 'y');

	glm::mat4 rot;
	rot *= glm::rotate(-this->angleX, glm::vec3(1.0f, 0.0f, 0.0f));	
	rot *= glm::rotate(-this->angleY, glm::vec3(0.0f, 1.0f, 0.0f));	
	rot *= glm::rotate(-this->angleZ, glm::vec3(0.0f, 0.0f, 1.0f));	

	this->viewDir = rot*glm::vec4(0.0, 0.0, 1.0, 0.0);
	this->rightVec = rot*glm::vec4(1.0, 0.0, 0.0, 0.0);
	this->upVec = rot*glm::vec4(0.0, 1.0, 0.0, 0.0);
}

void Camera::MoveForward(float distance) {
	//if(angleZ > 90.0f || angleZ < -90.0f) {
	//	distance = distance;
	//} 
	glm::vec4 ska = this->viewDir * (distance * dis / 10.0f);
	//std::cout<<ska.x<<" "<<ska.y<<" "<<ska.z<<std::endl;
	
	this->position += vec3d(ska.x, ska.y, ska.z);
}

void Camera::StrafeRight(float distance) {
	glm::vec4 ska = this->rightVec * (distance * dis / 10.0f);
	this->position += vec3d(ska.x, ska.y, ska.z);
}

void Camera::MoveUpward(float distance) {
	glm::vec4 ska = this->upVec * (distance * dis / 10.0f);
	this->position += vec3d(ska.x, ska.y, ska.z);
}

glm::mat4 Camera::e3rtLookAt(vec3d eyePos, vec3d lookPos, vec3d upVector) {
	vec3d forward = normalize(subtract(lookPos, eyePos));
	vec3d side = normalize(crossProduct(&forward, &upVector));
	vec3d up = crossProduct(&side, &forward);

	glm::mat4 ma;
	ma[0][0] = side[0];
	ma[1][0] = side[1];
	ma[2][0] = side[2];
	ma[3][0] = 0.0;

	ma[0][1] = up[0];
	ma[1][1] = up[1];
	ma[2][1] = up[2];
	ma[3][1] = 0.0;
	
	ma[0][2] = -forward[0];
	ma[1][2] = -forward[1];
	ma[2][2] = -forward[2];
	ma[3][2] = 0.0;

	ma[0][3] = 0.0;
	ma[1][3] = 0.0;
	ma[2][3] = 0.0;

	ma *= glm::translate((float)-eyePos.x, (float)-eyePos.y, (float)-eyePos.z);	

	return ma;
}

void Camera::turnCamera(float thetaX, float thetaY){
	if(this->mouseState) {
		this->newAngleX = 0.0f;
		this->newAngleY = 0.0f;
		this->newAngleX = (0.02f * thetaY);
		this->newAngleY = (0.02f * thetaX);
	}
}

void Camera::getFrustum(const float proj[16], const float modl[16], float frustum[6][4]) {
	//float proj[16];
	//float modl[16];
	float clip[16];
	float t;

	/* Get the current PROJECTION matrix from OpenGL */
	//glGetFloatv(GL_PROJECTION_MATRIX, proj, 0);

	/* Get the current MODELVIEW matrix from OpenGL */
	//glGetFloatv(GL_MODELVIEW_MATRIX, modl, 0);

	/* Combine the two matrices (multiply projection by modelview) */
	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
	/* Extract the numbers for the RIGHT plane */
	frustum[0][0] = clip[ 3] - clip[ 0];
	frustum[0][1] = clip[ 7] - clip[ 4];
	frustum[0][2] = clip[11] - clip[ 8];
	frustum[0][3] = clip[15] - clip[12];

	/* Normalize the result */
	t = (float) sqrt(frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2]);
	frustum[0][0] /= t;
	frustum[0][1] /= t;
	frustum[0][2] /= t;
	frustum[0][3] /= t;

	/* Extract the numbers for the LEFT plane */
	frustum[1][0] = clip[ 3] + clip[ 0];
	frustum[1][1] = clip[ 7] + clip[ 4];
	frustum[1][2] = clip[11] + clip[ 8];
	frustum[1][3] = clip[15] + clip[12];

	/* Normalize the result */
	t = (float) sqrt(frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2]);
	frustum[1][0] /= t;
	frustum[1][1] /= t;
	frustum[1][2] /= t;
	frustum[1][3] /= t;

	/* Extract the BOTTOM plane */
	frustum[2][0] = clip[ 3] + clip[ 1];
	frustum[2][1] = clip[ 7] + clip[ 5];
	frustum[2][2] = clip[11] + clip[ 9];
	frustum[2][3] = clip[15] + clip[13];

	/* Normalize the result */
	t = (float) sqrt(frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2]);
	frustum[2][0] /= t;
	frustum[2][1] /= t;
	frustum[2][2] /= t;
	frustum[2][3] /= t;

	/* Extract the TOP plane */
	frustum[3][0] = clip[ 3] - clip[ 1];
	frustum[3][1] = clip[ 7] - clip[ 5];
	frustum[3][2] = clip[11] - clip[ 9];
	frustum[3][3] = clip[15] - clip[13];

	/* Normalize the result */
	t = (float) sqrt(frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2]);
	frustum[3][0] /= t;
	frustum[3][1] /= t;
	frustum[3][2] /= t;
	frustum[3][3] /= t;

	/* Extract the FAR plane */
	frustum[4][0] = clip[ 3] - clip[ 2];
	frustum[4][1] = clip[ 7] - clip[ 6];
	frustum[4][2] = clip[11] - clip[10];
	frustum[4][3] = clip[15] - clip[14];

	/* Normalize the result */
	t = (float) sqrt(frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2]);
	frustum[4][0] /= t;
	frustum[4][1] /= t;
	frustum[4][2] /= t;
	frustum[4][3] /= t;

	/* Extract the NEAR plane */
	frustum[5][0] = clip[ 3] + clip[ 2];
	frustum[5][1] = clip[ 7] + clip[ 6];
	frustum[5][2] = clip[11] + clip[10];
	frustum[5][3] = clip[15] + clip[14];

	/* Normalize the result */
	t = (float) sqrt(frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2]);
	frustum[5][0] /= t;
	frustum[5][1] /= t;
	frustum[5][2] /= t;
	frustum[5][3] /= t;
}

void Camera::displayCamPos() {
	std::stringstream strs;
	std::cout.precision(2);
	strs<<position.x;
	FontWriter::write(std::string("POS X"), strs.str());
	strs.str("");
	strs<<position.y;
	FontWriter::write(std::string("POS Y"), strs.str());
	strs.str("");
	strs<<position.z;
	FontWriter::write(std::string("POS Z"), strs.str());
	strs.str("");
	strs<<angleX;
	FontWriter::write(std::string("ANGLE X"), strs.str());
	strs.str("");
	strs<<angleY;
	FontWriter::write(std::string("ANGLE Y"), strs.str());
	strs.str("");
	strs<<angleZ;
	FontWriter::write(std::string("ANGLE Z"), strs.str());
}

void Camera::sDisplayCamPos() {
	Camera::current->displayCamPos();
}

void Camera::sApplyCamMatrix() {
	Camera::current->applyCamMatrix();
}

void Camera::sSetKeyState(int key) {
	//std::cout<<"a "<<key<<std::endl;
	Camera::current->setKeyState(key);
}

vec3d Camera::getCurrentPos() {
	return Camera::current->position;
}

vec3q Camera::getViewDir() {
	return vec3q(Camera::current->viewDir.x,
		Camera::current->viewDir.y,
		Camera::current->viewDir.z);
}

vec3d Camera::getCurrentAngle() {
	return vec3d(Camera::current->angleX,
		Camera::current->angleY,
		Camera::current->angleZ);
}

vec3d Camera::getCurrentSpeed() {
	return vec3d(Camera::current->camSpeed,
		Camera::current->camSpeedFactor, 0.0);
}

vec3d Camera::getCurrentFov() {
	return vec3d(Camera::current->fovVar,
		Camera::fovConst/Camera::current->fovVar, 0.0);
}

void Camera::sSetMouseState(bool s) {
	Camera::current->setMouseState(s);
}

void Camera::sGetFrustum(const float proj[16], const float model[16], float frustum[6][4]) {
	Camera::current->getFrustum(proj, model, frustum);
}

void Camera::sSetPos(double x, double y, double z) {
	Camera::current->setPos(x, y, z);
}

void Camera::sSetAngle(double x, double y, double z) {
	Camera::current->setAngle(x, y, z);
}

void Camera::sTurnCamera(float thetaX, float thetaY) {
	Camera::current->turnCamera(thetaX, thetaY);
}

glm::mat4 Camera::sGetProjectionMatrix() {
	return Camera::current->getProjectionMatrix();
}

glm::mat4 Camera::getProjectionMatrix() {
	return this->ProjectionMatrix;
}

glm::mat4 Camera::sGetProjectionMatrix(float near, float far) {
	return Camera::current->getProjectionMatrix(near, far);
}

glm::mat4 Camera::getProjectionMatrix(float near, float far) {
	return glm::perspective(Camera::fovConst/fovVar, (GLfloat)width/(GLfloat)height, 
			near, far);
}

void Camera::setCurrent(const std::string& name) {
	std::map<std::string, Camera*>::iterator it;
	for(it = Camera::cameras.begin(); it != Camera::cameras.end(); it++) {
		if((*it).first.compare(name) == 0) {
			Camera::current = (*it).second;
		}
	}
}

void Camera::insertCamera(const std::string name, Camera *cam) {
	Camera::cameras.insert(std::make_pair(name, cam));
	Camera::current = cam;	
}

std::string Camera::getCurrent() {
	std::map<std::string, Camera*>::iterator it;
	for(it = Camera::cameras.begin(); it != Camera::cameras.end(); it++) {
		if((*it).second->id == Camera::current->id) {
			return (*it).first;
		}
	}
	return (*Camera::cameras.begin()).first;
}
