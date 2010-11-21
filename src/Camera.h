#ifndef CAMERA
#define CAMERA
#include <stack>
#include <iostream>
#include <map>
#include <string>
#include "geomUtil.h"

#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#include <glm/glm.hpp>

#include "vec3f.h"
#include "vec3d.h"
#include "vec3q.h"

class Camera {
	private:
		static unsigned newId;
		static std::map<std::string, Camera*> cameras;
		static Camera *current;

		std::string name;
		vec3f pos;
	
		//new cam
		vec3d position;
		glm::vec4 viewDir;
		glm::vec4 rightVec;
		glm::vec4 upVec;

		float angleX;
		float angleY;
		float angleZ;

		float newAngleX;
		float newAngleY;
		int keyState;
		int mouseState;
		float dis;

		glm::mat4 ProjectionMatrix;
		int width;
		int height;
		static const float fovConst = 4500.0f;
		static float nearGlobal;
		static float farGlobal; 
		float fovVar;

		static const int STATE_STOP = 0;
		static const int STATE_MOVING_FORWARD = 1;
		static const int STATE_MOVING_BACKWARD = 2;
		static const int STATE_MOVING_LEFT = 3;
		static const int STATE_MOVING_RIGHT = 4;
		static const int MOUSE_STATE_UP = 0;
		static const int MOUSE_STATE_DOWN = 1;
 		static const int KEY_STATE_FORWARD = 'w';
 		static const int KEY_STATE_BACKWARD = 's';
 		static const int KEY_STATE_LEFT = 'a';
 		static const int KEY_STATE_RIGHT = 'd';
 		static const int KEY_STATE_UP = 'e';
 		static const int KEY_STATE_DOWN = 'q';
 		static const int KEY_STATE_INCREASE_CAM_SPEED = ',';
 		static const int KEY_STATE_DECREASE_CAM_SPEED = 'm';
 		static const int KEY_STATE_INCREASE_CAM_FOV = 'y';
 		static const int KEY_STATE_DECREASE_CAM_FOV = 'x';
 		static bool KEY_STATE_FORWARD_BOOL;
 		static bool KEY_STATE_BACKWARD_BOOL;
 		static bool KEY_STATE_LEFT_BOOL;
 		static bool KEY_STATE_RIGHT_BOOL;
 		static bool KEY_STATE_UP_BOOL;
 		static bool KEY_STATE_DOWN_BOOL;
		static bool KEY_STATE_RLEFT_BOOL;
		static bool KEY_STATE_TUP_BOOL;
		static bool KEY_STATE_RRIGHT_BOOL;
		static bool KEY_STATE_TLEFT_BOOL; 
		static bool KEY_STATE_TDOWN_BOOL;
		static bool KEY_STATE_TRIGHT_BOOL;

		static bool KEY_STATE_INCREASE_CAM_SPEED_BOOL;
		static bool KEY_STATE_DECREASE_CAM_SPEED_BOOL;
		static bool KEY_STATE_INCREASE_CAM_FOV_BOOL;
		static bool KEY_STATE_DECREASE_CAM_FOV_BOOL;

		double camSpeed;
		double camSpeedFactor;

		static const float MOVE_STEP = 0.0005f;
		static const float MAX_VELOCITY = 5.0000f;
		//static const float PI = 3.14159f;
		static glm::mat4 e3rtLookAt(vec3d eyePos, vec3d lookPos, vec3d upVector);

	public:

		unsigned id;
		void setPos(double x, double y, double z);
		void setAngle(double x, double y, double z);
		void displayCamPos();
		Camera(vec3f p, float angleXY, float angleyYZ);
		Camera(vec3d p, int width, int height);
		Camera();
		void applyCamMatrix();
		void setKeyState(int key);
		void setMouseState(bool s);
		void turnCamera(float thetaX, float thetaY);
		static glm::mat4 sGetCurrentCamMatrix(bool withPos);
		glm::mat4 getCurrentCamMatrix(bool withPos);

		//new cam methods
		void Move(vec3d direction);
		void RotateX(float angle);
		void RotateY(float angle);
		void RotateZ(float angle);
		void MoveForward(float distance);
		void StrafeRight(float distance);
		void MoveUpward(float distance);

		static void getFrustum(const float proj[16], const float model[16], float frustum[6][4]);
		static void sDisplayCamPos();
		static void sApplyCamMatrix();
		static void sSetKeyState(int key);
		static void sSetMouseState(bool s);
		static void sGetFrustum(const float proj[16], const float model[16], float frustum[6][4]);
		static void sSetPos(double x, double y, double z);
		static void sSetAngle(double x, double y, double z);
		static glm::mat4 sGetProjectionMatrix();
		glm::mat4 getProjectionMatrix();
		static glm::mat4 sGetProjectionMatrix(float near, float far);
		glm::mat4 getProjectionMatrix(float near, float far);
		static vec3d getCurrentPos();
		static vec3q getViewDir();
		static vec3d getCurrentAngle();
		static vec3d getCurrentSpeed();
		static vec3d getCurrentFov();
		static void sTurnCamera(float thetaX, float thetaY);
		static void setCurrent(const std::string& name);
		static void insertCamera(const std::string name, Camera *cam);
		static std::string getCurrent();
};
#endif
