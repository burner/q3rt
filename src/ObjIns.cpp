#include "Octree.h"
#include <math.h>
#include <cstdlib>
#include "ObjInsManager.h"
#include "vec3d.h"
#include "vec3f.h"

unsigned long ObjIns::currentId = 0;
unsigned long ObjIns::time = 0;

ObjIns::ObjIns(vec3d *pos, RenderAble *object, bool moveable) {
	this->pos = pos;
	this->splineBox = new vec3d(100.0, 100.0, 100.0);
	this->rot = new vec3f();
	this->node = new std::vector<Octree*>();
	this->radius = object->getRadius();
	this->id = ObjIns::currentId++;
	this->object = object;
	this->radius = this->object->getRadius();
	//std::cout<<"radius "<<this->radius<<std::endl;
	this->splinePoints = NULL;
	this->moveable = moveable;
	if(this->moveable) {
		this->initSpline();
	}
	this->rendered = 0;
}

ObjIns::ObjIns(vec3d *pos, std::string name, vec3d *splineBox, bool moveable) {
	this->pos = pos;
	this->splineBox = splineBox;
	this->rot = new vec3f();
	this->node = new std::vector<Octree*>();
	this->id = ObjIns::currentId++;
	std::string tmp = name;
	this->object = ObjInsManager::getObject(tmp);
	this->radius = this->object->getRadius();
	this->splinePoints = NULL;
	this->moveable = moveable;
	if(this->moveable) {
		this->initSpline();
	}
	this->rendered = 0;
}

ObjIns::ObjIns(double nx, double ny, double nz, float inRadius) {
	id = ObjIns::currentId++;
	pos = new vec3d(nx,ny,nz);
	this->splineBox = new vec3d(100.0, 100.0, 100.0);
	this->rot = new vec3f();
	node = new std::vector<Octree*>();
	radius = inRadius;
	this->moveable = true;
	this->splinePoints = NULL;
	if(this->moveable) {
		this->initSpline();
	}
	this->rendered = 0;
}

ObjIns::ObjIns(double nx, double ny, double nz, float inRadius, Octree *nNode) {
	this->id = ObjIns::currentId++;
	this->pos = new vec3d(nx,ny,nz);
	this->rot = new vec3f();
	node = new std::vector<Octree*>();
	radius = inRadius;
	node->push_back(nNode);
	this->splinePoints = NULL;
	this->moveable = true;
	//this->initSpline(ObjIns::nPoints);
	this->rendered = 0;
}

ObjIns::ObjIns(double nx, double ny, double nz, float inRadius, unsigned long nMass) {
	id = ObjIns::currentId++;
	pos = new vec3d(nx,ny,nz);
	this->rot = new vec3f();
	node = new std::vector<Octree*>();
	radius = inRadius;
	//mass = nMass;
	this->splinePoints = NULL;
	this->moveable = true;
	//this->initSpline(ObjIns::nPoints);
	this->rendered = 0;
}

ObjIns::ObjIns(vec3d *npos, float nRadius) {
	id = ObjIns::currentId++;
	node = new std::vector<Octree*>();
	radius = nRadius;
	this->pos = npos;
	this->rot = new vec3f();
	this->splinePoints = NULL;
	this->moveable = true;
	//this->initSpline(ObjIns::nPoints);
	this->rendered = 0;
}

void ObjIns::initSpline() {
	srand(this->id);
	this->timeSlice = rand() % 5000 + 1000;
	this->counter = 1;
	//this->startTime = Timer::getTicks();
	this->splinePoints = new std::vector<vec3d*>();
	this->splinePoints->push_back(new vec3d(this->pos));
	double dx, dy, dz;
	for(unsigned i = 0; i < 2; i++) {
		dx = (rand()/(double(RAND_MAX)+1))*this->splineBox->x;
		dy = (rand()/(double(RAND_MAX)+1))*this->splineBox->y;
		dz = (rand()/(double(RAND_MAX)+1))*this->splineBox->z;
		vec3d *tmp = new vec3d(rand() % 2 ? dx : -dx,
		   rand() % 2 ? dy : -dy,
		   rand() % 2 ? dz : -dz);
		this->splinePoints->push_back(tmp);
	}
	std::vector<vec3d*>::iterator it;
}

ObjIns::~ObjIns() {
	std::vector<Octree*>::iterator it;
	for(it = this->node->begin(); it != this->node->end(); it++) {
		//std::cout<<"remove from octree "<<(*it)->id<<std::endl;
		(*it)->removeDirect(this->id);
	}
	delete this->node;
	delete this->pos;
	delete this->splineBox;
	delete this->rot;
	if(this->splinePoints != NULL) {
		delete this->splinePoints;
	}
	//std::cout<<"ObjIns "<<this->id<<" has been rendered "<<this->rendered<<" times"<<std::endl;
}

void ObjIns::generalUpdate() {
	//ObjIns::time = Timer::getTicks();
}

void ObjIns::setOctree(Octree *nNode) {
	if(nNode == NULL) {
		std::cout<<"ObjIns "<<this->id<<" setOctree == NULL"<<std::endl;
	}
	std::vector<Octree*>::iterator it;
	//std::cout<<node.size()<<std::endl;
	//for(it = node.begin(); it != node.end() && node.size() > 0 && *it != NULL; it++) {
	for(it = node->begin(); it != node->end(); it++) {
		if( (*it)->id == nNode->id ) {
			return;
		}
	}
	node->push_back(nNode);
}


void ObjIns::update(float dt) {
	if(this->moveable) {
		//std::cout<<"moveing "<<this->id<<std::endl;
		volatile long timeDiff = ObjIns::time - this->timeSlice * this->counter - this->startTime;
		if((this->startTime + (this->timeSlice * this->counter)) <= ObjIns::time) {
			vec3d *tmp = this->splinePoints->back();
			this->splinePoints->clear();
			this->splinePoints->push_back(tmp);
				
			double dx, dy, dz;
			for(unsigned i = 0; i < 2; i++) {
				dx = (rand()/(double(RAND_MAX)+1))*this->splineBox->x;
				dy = (rand()/(double(RAND_MAX)+1))*this->splineBox->y;
				dz = (rand()/(double(RAND_MAX)+1))*this->splineBox->z;
				vec3d *tmp = new vec3d(rand() % 2 ? dx : -dx,
				   rand() % 2 ? dy : -dy,
				   rand() % 2 ? dz : -dz);
				this->splinePoints->push_back(tmp);
			}
			this->counter++;
		}
		
		delete this->pos;
		timeDiff = timeDiff < 0 ? -timeDiff : timeDiff;
		volatile float lamda = 1.0 - (float)timeDiff / (float)this->timeSlice;
		this->pos = bSpline( this->splinePoints->at(0), 
			this->splinePoints->at(1), 
			this->splinePoints->at(2),
			lamda);
		/*this->pos = bSpline( this->splinePoints->at( (3 * this->counter - 3) % this->splinePoints->size()), 
			this->splinePoints->at( (3 * this->counter - 2) % this->splinePoints->size()), 
			this->splinePoints->at( (3 * this->counter - 1) % this->splinePoints->size()),
			lamda);*/
		//this->pos->print();
	}
	std::vector<Octree*> toIter(this->node->begin(), this->node->end());
	std::vector<Octree*>::iterator it = toIter.begin();
	for(; it != toIter.end(); it++) {
		(*it)->update(this);
	}
}

void ObjIns::removeFromOctree(unsigned long long id) {
	//std::cout<<"RemoveFromOctree "<<this->id<<std::endl;
	std::vector<Octree*>::iterator it;
	for(it = node->begin(); it != node->end(); it++) {
		if((*it)->id == id) {
			//std::cout<<(*it)->id<<std::endl;
			//(*it)->remove(this->id);
			node->erase(it);
			break;
		}
	}
}

void ObjIns::render(glm::mat4 proj, glm::mat4 view, vec3d *camPos) {
	this->rendered++;
	double relX = this->pos->x + camPos->x;
	double relY = this->pos->y + camPos->y;
	double relZ = this->pos->z + camPos->z;
	//view *= glm::translate((float)pos->x, (float)pos->y, (float)pos->z);
	view *= glm::translate((float)relX, (float)relY, (float)relZ);
	view *= glm::rotate(rot->x, glm::vec3(1.0f, 0.0f, 0.0f)); 
	view *= glm::rotate(rot->y, glm::vec3(0.0f, 1.0f, 0.0f));
	view *= glm::rotate(rot->z, glm::vec3(0.0f, 0.0f, 1.0f));
	this->object->draw(glm::value_ptr(proj*view));
}

void ObjIns::printRenderCount() const {
	std::cout<<"id "<<this->id<<" : rendered "<<rendered<<std::endl;
}

void ObjIns::printInfo() const {
	std::cout<<"ObjIns id = "<<this->id<<" is known to "<<node->size()
		<<" Octree's at: "<<pos->x<<" "<<pos->y<<" "<<pos->z<<" PosAdr: "<<this->pos<<std::endl;
	std::cout<<"Known to Octree ";
	std::vector<Octree*>::iterator it;
	for(it = node->begin(); it != node->end(); it++) {
		std::cout<<(*it)->id<<" ";
	}
	std::cout<<"Obj Adr: "<<this->object;
	std::cout<<std::endl;

	std::cout<<std::endl;
}

bool ObjIns::isKnownToOctree(unsigned long long octreeId) {
	std::vector<Octree*>::iterator it;
	for(it = node->begin(); it != node->end(); it++) {
		if( (*it)->id == octreeId ) {
			return true;
		}
	}
	return false;
}

void ObjIns::distanceToCam(vec3d cam) {
	this->distance = sqrt( pow(this->pos->x - cam.x, 2) +
							pow(this->pos->y - cam.y, 2) +
							pow(this->pos->z - cam.z, 2));
	//std::cout<<"id "<<this->id<<" distance "<<this->distance<<std::endl;
}

std::string ObjIns::getType() {
	return std::string("ObjIns");
}

std::string ObjIns::getID() {
	std::stringstream poss;
	poss<<this->id;
	return poss.str();
}

std::string ObjIns::getPos() {
	std::stringstream poss;
	poss<<this->pos->x;
	poss<<" ";
	poss<<this->pos->y;
	poss<<" ";
	poss<<this->pos->z;
	return poss.str();
}

std::string ObjIns::getRot() {
	std::stringstream poss;
	poss<<this->rot->x;
	poss<<" ";
	poss<<this->rot->y;
	poss<<" ";
	poss<<this->rot->z;
	return poss.str();
}

double ObjIns::getRadius() {
	return this->radius;
}

std::string ObjIns::getObjectName() {
	return this->object->getName();
}
