#include "Renderer.h"
#include "Focus.h"

std::vector<ObjIns*> Renderer::inFrustum;
std::stack<glm::mat4> Renderer::viewStack;
//glm::mat4 Renderer::Projection;
float Renderer::angle;
int Renderer::width;
int Renderer::height;

void Renderer::init(float angle, int height, int width) {
	Renderer::angle = angle;
	Renderer::height = height;
	Renderer::width = width;
}

void Renderer::appendObjInsToRender(ObjIns *obIns) {
	if(Renderer::inFrustum.empty()) {
		Renderer::inFrustum.push_back(obIns);
		return;
	}
	//obIns->distanceToCam(Camera::getCurrentPos());
	std::vector<ObjIns*>::iterator it = Renderer::inFrustum.begin();
	while(it != Renderer::inFrustum.end()) {
		//if allready in list
		if((*it)->id == obIns->id) {
			return;	
		}/* else if((*it)->id > obIns->id) {
			break;
		}*/
		it++;
	}
	Renderer::inFrustum.push_back(obIns);
}

void Renderer::pushViewMatrix(glm::mat4 mat){
	Renderer::viewStack.push(mat);
}

glm::mat4 Renderer::popViewMatrix(){
	glm::mat4 View = Renderer::viewStack.top();
	Renderer::viewStack.pop();
	return View;
}

void CalcDistance(ObjIns *a) {
	a->distanceToCam(Camera::getCurrentPos());
}

void PrintId(const ObjIns *a) {
	std::cout<<"id = "<<a->id<<std::endl;
}

void PrintDistance(const ObjIns *a) {
	std::cout<<"dis = "<<a->distance<<": radius = "<<a->radius<<std::endl;
}

void Renderer::render() {
	std::stringstream s;
	s<<Renderer::inFrustum.size()<<"/"<<ObjInsManager::getNumObjIns();
	FontWriter::write(std::string("ObjIns"), s.str());
	//std::cout<<"\n\nNew Frame"<<std::endl;
	//std::for_each(inFrustum.begin(), inFrustum.end(), PrintId);
	std::for_each(inFrustum.begin(), inFrustum.end(), CalcDistance);
	Renderer::sortByDistance();
	//std::for_each(inFrustum.begin(), inFrustum.end(), PrintDistance);
	glm::mat4 view = Camera::sGetCurrentCamMatrix(false);
	vec3d camPos = Camera::getCurrentPos();
	std::vector<ObjIns*>::iterator it;
	glm::mat4 Projection = Camera::sGetProjectionMatrix();
	float near, far;
	for(it = Renderer::inFrustum.begin(); it != Renderer::inFrustum.end(); it++) {
		glClear(GL_DEPTH_BUFFER_BIT);
		near = (*it)->distance - (*it)->radius;
		far = (*it)->distance + (*it)->radius;
		//near = (*it)->distance - 10.0f;
		far = (*it)->distance + 10.0f;
		//std::cout<<near<<" "<<far<<std::endl;
		Renderer::pushViewMatrix(view);
		Projection = Camera::sGetProjectionMatrix(0.01f, 20000.0f);
		(*it)->render(Projection, view, &camPos);
		view = Renderer::popViewMatrix();
		//std::cout<<(*it)->distance<<std::endl;
	}
	Renderer::inFrustum.clear();
}

void Renderer::sortByDistance() {
	//if(Renderer::inFrustum.size() > 1) {
	//	quick_sort<std::vector<ObjIns*>::iterator>(
	//		Renderer::inFrustum.begin(), Renderer::inFrustum.end(), ObjDistanceCompare);	
	//}
	std::sort(Renderer::inFrustum.begin(), Renderer::inFrustum.end(), ObjDistanceCompare);
}

bool ObjDistanceCompare(const ObjIns *a, const ObjIns *b) {
	//std::cout<<a->distance<<" "<<b->distance<<std::endl;	
	//a->printInfo(); b->printInfo();
	return a->distance <= b->distance;
}
