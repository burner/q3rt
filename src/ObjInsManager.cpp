#include "ObjInsManager.h"

std::map<std::string, Obj*> ObjInsManager::objects;
ObjInsManager* ObjInsManager::current;

ObjInsManager::ObjInsManager(Octree *nRoot, unsigned numThreads) {
	this->numThreads = numThreads;
	this->root = nRoot;
	this->focus = new Focus();
	this->objIns = new std::vector<ObjIns*>();
	ObjInsManager::current = this;
	this->focusBool = false;
}

ObjInsManager::~ObjInsManager() {
	std::vector<ObjIns*>::iterator it;
	//for(it = this->objIns->begin(); it != this->objIns->end(); it++) {
	//	std::cout<<(*it)->id<<std::endl;
	//	delete (*it);
	//	this->objIns->erase(it);
	//}
	ObjIns *tmp;
	while(!this->objIns->empty()) {
		tmp = this->objIns->back();
		this->objIns->pop_back();
		delete tmp;
		//std::cout<<this->objIns->size()<<std::endl;
	}
	delete this->objIns;
	delete this->focus;
}

void ObjInsManager::sInsert(ObjIns *nObject) {
	ObjInsManager::current->insert(nObject);
}

void ObjInsManager::insert(ObjIns *nObject) {
	this->objIns->push_back(nObject);
	this->root->insert(nObject);
	//this->printInfo();
}

void ObjInsManager::remove(unsigned long id) {
	std::vector<ObjIns*>::iterator it;
	for(it = this->objIns->begin(); it != this->objIns->end(); it++) {
		if( (*it)->id == id) {
			ObjIns *tmp = *it;
			this->objIns->erase(it);
			delete tmp;
			return;
		}
	}
}

void ObjInsManager::remove(ObjIns *toRemove) {
	this->remove(toRemove->id);
}

void ObjInsManager::update(float time) {
	ObjIns::generalUpdate();
	//std::cout<<"ObjInsManager update"<<std::endl;
	unsigned n = this->objIns->size() / this->numThreads;
	unsigned r = this->objIns->size() % this->numThreads;
	//std::cout<<" / = "<<n<<"; % = "<<r<<std::endl;

	unsigned lo = 0;
	UpdateJob **jobs = new UpdateJob*[this->numThreads];
	jobs[0] = new UpdateJob(this->objIns, lo, n+r, time);
	lo = n+r;
	for(unsigned i = 1; i < this->numThreads; i++) {
		jobs[i] = new UpdateJob(this->objIns, lo, lo+n, time); //+(i+1 == this->numThreads ? 1 : 0));
		lo+=n;
	}
	//std::cout<<"Size of ObjIns "<<this->objIns->size()<<std::endl;
	for(unsigned i = 0; i < this->numThreads; i++) {
		//std::cout<<"lo "<<jobs[i]->l<<" hi "<<jobs[i]->h<<std::endl;
	}

	pthread_t **worker = new pthread_t*[this->numThreads];
	for(unsigned i = 0; i < this->numThreads; i++) {
		worker[i] = new pthread_t();
		pthread_create(worker[i], NULL, updateFunction, (void*)jobs[i]);
	}

	for(unsigned i = 0; i < this->numThreads; i++) {
		pthread_join(*worker[i], NULL);
	}

	for(unsigned i = 0; i < this->numThreads; i++) {
		delete jobs[i];
	}
	delete [] jobs;

	for(unsigned i = 0; i < this->numThreads; i++) {
		delete [] worker[i];
	}
	delete [] worker;
	
	//std::cout<<"before remove"<<std::endl;
	//this->root->print();
	this->root->doRemovel();
	//std::cout<<"after remove"<<std::endl;
	//this->root->print();
	//std::for_each(objIns->begin(), objIns->end(), print);
	//this->root->print();
}

void ObjInsManager::printInfo() {
	this->root->print();
	std::vector<ObjIns*>::iterator it;
	for(it = this->objIns->begin(); it != this->objIns->end(); it++) {
		(*it)->printInfo();
	}
}

void ObjInsManager::printRenderCount() const {
	std::vector<ObjIns*>::iterator it;
	for(it = this->objIns->begin(); it != this->objIns->end(); it++) {
		(*it)->printRenderCount();
	}
}

void ObjInsManager::printOctree() {
	this->root->print();
	std::cout<<std::endl;
}

unsigned ObjInsManager::getNumObjIns() {
	return ObjInsManager::current->objIns->size();
}

std::list<ObjIns*>* ObjInsManager::getElementsAsList() {
	return new std::list<ObjIns*>(this->objIns->begin(), this->objIns->end());
}

void ObjInsManager::render(float proj[16]) {
	glm::mat4 tmp = Camera::sGetCurrentCamMatrix(true);
	const float *model = glm::value_ptr( tmp );
	float frustum[6][4];
	Camera::getFrustum(proj, model, frustum);
	this->root->render(frustum);
}

void ObjInsManager::renderFocus() {
	if(this->focusBool) {
		//std::cout<<"ObjInsManager size() "<<this->objIns->size()<<std::endl;
		std::vector<ObjIns*>::const_iterator it = this->objIns->begin();
		for(; it != this->objIns->end(); it++) {
			if(this->focusId == (*it)->id) {
				glm::mat4 view = Camera::sGetCurrentCamMatrix(false);
				this->focus->drawSphere((*it)->pos, (*it)->rot, view);
				return;
			}
		}
	}
}

Obj* ObjInsManager::getObject(std::string name) {
	std::map<std::string, Obj*>::iterator it = ObjInsManager::objects.find(name);
	if(it != ObjInsManager::objects.end()) {
		return (*it).second;
	}
	Obj *n = new Obj("./content/eob/" + name + ".eob");	
	ObjInsManager::objects.insert(std::make_pair(name, n));
	return n;
}

void* updateFunction(void *p) {
	UpdateJob *j = static_cast<UpdateJob*>(p);
	std::vector<ObjIns*> *vec = j->vec;
	for(unsigned i = j->l; i < j->h; i++) {
		(*vec)[i]->update(j->time);
	}
	return 0;
}

void ObjInsManager::sSetFocused(unsigned long long id) {
	ObjInsManager::current->setFocused(id);
}

void ObjInsManager::sDisableFocus() {
	ObjInsManager::current->disableFocus();
}

void ObjInsManager::setFocused(unsigned long long id) {
	this->focusId = id;
	this->focusBool = true;
}

void ObjInsManager::disableFocus() {
	this->focusBool = false;
}

Octree* ObjInsManager::sGetCurrentOctree() {
	return ObjInsManager::current->getCurrentOctree();
}

Octree* ObjInsManager::getCurrentOctree() {
	return this->root;
}

std::vector<ObjIns*>* ObjInsManager::sGetCurrentObjIns() {
	return ObjInsManager::current->getCurrentObjIns();
}

std::vector<ObjIns*>* ObjInsManager::getCurrentObjIns() {
	return this->objIns;
}

ObjIns* ObjInsManager::sGetObjIns(unsigned long long id) {
	return ObjInsManager::current->getObjIns(id);
}

ObjIns* ObjInsManager::getObjIns(unsigned long long id) {
	std::vector<ObjIns*>::iterator it = this->objIns->begin();
	for(;it != this->objIns->end(); it++) {
		if((*it)->id == id) {
			return (*it);
		}
	}
	return NULL;
}
