#include "Octree.h"
#include "../display/Renderer.h"
#include "../display/Obj.h"
#include "../prim/vec3d.h"
#include "../prim/vec3f.h"
#include "../util/geomUtil.h"
#include "../display/Sphere/Quad.h"

int Octree::maxDepth = 12;
int Octree::maxUsedDepth = 0;
Octree* Octree::current;
const Octree::Type Octree::Typelist[] = {
			LeftTopBack,
			LeftTopFront,
			LeftBottomBack,
			LeftBottomFront,
			RightTopBack,
			RightTopFront,
			RightBottomBack,
			RightBottomFront,
			Root};

Octree::Octree(Octree *parent, Octree *root, Type type, std::list<ObjIns*> *nObjIns, 
				int dpth, unsigned long long nId, vec3d *n_pos, vec3d *n_dim, unsigned n_maxObjIns) {
	this->root = root;
	this->marked = NULL;
	this->lock = new pthread_spinlock_t();
	this->removeLock = NULL;
	pthread_spin_init(this->lock, PTHREAD_PROCESS_SHARED);
	this->parent = parent;
	this->type = type;
	this->ori = n_pos;
	this->dim = n_dim;
	this->depth = dpth;
	this->computeSphere();
	this->id = nId;
	this->objIns = nObjIns;
	this->maxObjIns = n_maxObjIns;
	if(n_dim == NULL) std::cout<<this->id<<std::endl;
	this->makeObjInsAware();
	this->initChildIds();
	
	//if(depth >= Octree::maxDepth || objIns->size() <= maxObjIns) {
	if(this->objIns->size() > maxObjIns) {
		this->split();
	}
}

Octree::Octree(Octree *parent, int dpth, unsigned long long nId, vec3d *n_pos, vec3d *n_dim, unsigned n_maxObjIns) {
	this->current = this;
	this->root = this;
	this->marked = new std::vector<Octree*>();
	this->lock = new pthread_spinlock_t();
	this->removeLock = new pthread_spinlock_t();
	pthread_spin_init(this->removeLock, PTHREAD_PROCESS_SHARED);

	pthread_spin_init(this->lock, PTHREAD_PROCESS_SHARED);
	this->parent = parent;
	this->ori = n_pos;
	this->dim = n_dim;
	this->depth = dpth;
	this->computeSphere();
	this->id = nId;
	this->objIns = new std::list<ObjIns*>();
	this->maxObjIns = n_maxObjIns;
	if(n_dim == NULL) std::cout<<this->id<<std::endl;
	this->initChildIds();
}

Octree::~Octree() {
	//std::cout<<"delete id "<<id<<std::endl;
	if(this->marked != NULL) delete this->marked;
	delete this->ori;
	delete this->dim;
	delete this->mid;
	delete this->objIns;
	delete this->childIds;
	pthread_spin_destroy(this->lock);
	delete this->lock;
	if(this->removeLock != NULL) delete this->removeLock;
	std::vector<Octree*>::iterator it;
	//std::cout<<"calling childs"<<std::endl;
	for(it = this->childs.begin(); it != this->childs.end(); it++) {
		delete (*it);
	}
	//std::cout<<std::endl;
}

void Octree::computeSphere() {
	this->mid = new vec3d(this->ori->x + this->dim->x/2,
		this->ori->y + this->dim->y/2,
		this->ori->z + this->dim->z/2);

	this->radius = distance(this->ori, this->mid);
}

void Octree::split() {
	//std::cout<<"Octree::split of "<<this->id<<" lock"<<std::endl;

	//create child pos and scale vectors
	vec3d *chPos[8], *chDim[8];
	int f = 0;
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			for(int k = 0; k < 2; k++) {
				chPos[f] = new vec3d(ori->x+(dim->x-(dim->y-((dim->z/2)*i))),
									  ori->y+(dim->y-(dim->y-((dim->y/2)*j))),
			  						  ori->z+(dim->z-(dim->z-((dim->z/2)*k))));
	
				chDim[f] = new vec3d(dim->x/2, dim->y/2, dim->z/2);
				f++;
			}
		}
	}
	std::list<ObjIns*>::iterator it;
	std::list<ObjIns*> *tmp;
	for(int i = 0; i < 8; i++) {
		tmp = new std::list<ObjIns*>();
		for(it = objIns->begin(); it != objIns->end(); it++) {
			//std::cout<<"i "<<i<<std::endl;
			//std::cout<<(*it)->pos->x<<std::endl;
			//std::cout<<(chPos[i])->x<<std::endl;
			//std::cout<<(chDim[i])->x<<std::endl;
			if(testSphereBox(*(*it)->pos, (*it)->radius, *(chPos[i]), *(chDim[i]))) {
				tmp->push_back(*it);
			}
		}
		//unsigned tmpSize = tmp->size();
		if(tmp->size() > 0) {
			//id is calculated 8*id+1+i 
			//see binary heap for further information
			//std::cout<<"id "<<(8*id)+1+i<<std::endl;
			childs.push_back(new Octree(this, this->root, Octree::Typelist[i], 
				tmp, depth+1, this->getFreeChildId(), chPos[i], chDim[i], maxObjIns));
		} else {
			delete tmp;
			delete chPos[i];
			delete chDim[i];
		}
	}
	for(it = objIns->begin(); it != objIns->end(); it++) {
		(*it)->removeFromOctree(this->id);
	}
	this->objIns->clear();
	//std::cout<<"Octree::split of "<<this->id<<" unlock"<<std::endl;
}

void Octree::initChildIds() {
	this->childIds = new std::vector<unsigned long long>();
	for(int i = 0; i < 8; i++) {
		this->childIds->push_back( (8*id)+1+i );
		//std::cout<<"childID "<<childIds->top()<<std::endl;
	}
}

unsigned long long Octree::getFreeChildId() {
	long tmp = -1;
	tmp = this->childIds->back();
	this->childIds->pop_back();
	return tmp;
}

void Octree::setFreeChildId(unsigned long long id) {
	this->childIds->push_back(id);
}

void Octree::print() {
	for(int i = 0; i < depth; i++) {
		std::cout<<"  ";
	}
	std::cout.precision(2);
	std::cout.setf(std::ios::fixed);
	std::cout<<"id = "<<id<<" "<<ori->x<<" "<<ori->y<<" "<<ori->z<<" dim "<<dim->x<<" "<<dim->y<<" "<<dim->z<<std::left<<" objIns.size() "<<objIns->size()<<"\t ";

	std::list<ObjIns*>::iterator it;
	std::cout<<std::right<<"holds id's ";
	for(it = objIns->begin(); it != objIns->end(); it++) {
		std::cout<<(*it)->id<<" ";
	}
	std::cout<<std::endl;
	
	for(unsigned i = 0; i < childs.size(); i++) {
		childs[i]->print();
	}
}

void Octree::print(std::ofstream &file) {
	for(int i = 0; i < depth; i++) {
		file<<"\t";
	}
	file<<"id = "<<id<<"; ori = "<<ori->x<<" "<<ori->y<<" "<<ori->z<<"; objIns.size() "<<objIns->size()<<std::endl;
	
	for(unsigned i = 0; i < childs.size(); i++) {
		childs[i]->print(file);
	}
}

void Octree::render(float frustum[6][4]) {
	//render all childs first because only leaves have ObjIns
	if(!this->childs.empty()) {
		std::vector<Octree*>::iterator it;
		for(it = this->childs.begin(); it != this->childs.end(); it++) {
			(*it)->render(frustum);
		}
		return;
	}

	//if no childs exists render all objIns in node
	std::list<ObjIns*>::iterator it;
	for(it = this->objIns->begin(); it != this->objIns->end(); it++) {
		if(sphereInFrustum(frustum, (*it)->pos, (*it)->radius)) {
			if(typeid(*(*it)->object) == typeid(Quad) && (*it)->object->isFacing()) {
				Renderer::appendObjInsToRender((*it));
			} else if(typeid(*(*it)->object) != typeid(Quad)) {
				Renderer::appendObjInsToRender((*it));
			}
		}
	}
}

void Octree::makeObjInsAware() {
	std::list<ObjIns*>::iterator it;
	//std::cout<<"\t\t"<<this<<std::endl;
	for(it = objIns->begin(); it != objIns->end(); it++) {
		(*it)->setOctree(this);
	}
}

void Octree::printObjInsListAdr() {
	//std::cout<<"ID "<<id<<" ObjInsListAdr "<<&(*objIns)<<std::endl;
	std::vector<Octree*>::iterator it;
	for(it = childs.begin(); it != childs.end(); it++) {
		(*it)->printObjInsListAdr();
	}
}

void Octree::insertIntoObjInsList(ObjIns *toInsert) {
	std::list<ObjIns*>::iterator it;
	for(it = objIns->begin(); it != objIns->end(); it++) {
		if( (*it)->id == toInsert->id) {
			return;
		}
	}
	objIns->push_back(toInsert);
	toInsert->setOctree(this);
}

void Octree::insert(ObjIns *toIns) {
	pthread_spin_lock(this->lock);
	//std::cout<<"Octree::insert of "<<this->id<<" lock"<<std::endl;
	//if this node is a leaf push ObjIns pack unless the node
	//has enough ObjIns to be split
	if(childs.empty()) {
		this->insertIntoObjInsList(toIns);
		if(this->objIns->size() >= maxObjIns) {
			//std::cout<<"Octree::insert of "<<this->id<<" unlock"<<std::endl;
			this->split();
			pthread_spin_unlock(this->lock);
			return;
		} else {
			//std::cout<<"Octree::insert of "<<this->id<<" unlock"<<std::endl;
			pthread_spin_unlock(this->lock);
			return;
		}
	} else {
		//if the node isn't a leaf test all existing childs	
		std::vector<Octree*>::iterator it;
		for(it = childs.begin(); it != childs.end(); it++) {
			if(testSphereBox(*toIns->pos, toIns->radius, *(*it)->ori, *(*it)->dim)) {
				(*it)->insert(toIns);
			}
		}
		//as well as all childs that has not been created yet
		//but only if not all childs exists
		if(childs.size() == 8) {
			//std::cout<<"Octree::insert of "<<this->id<<" unlock"<<std::endl;
			pthread_spin_unlock(this->lock);
			return;
		}
		vec3d *p, *d;
		int typeCnt = 0;
		for(int i = 0; i < 2; i++) {
			for(int j = 0; j < 2; j++) {
				for(int k = 0; k < 2; k++) {
					p = new vec3d(ori->x+(dim->x-(dim->y-((dim->z/2)*i))),
										  ori->y+(dim->y-(dim->y-((dim->y/2)*j))),
			  							  ori->z+(dim->z-(dim->z-((dim->z/2)*k))));
	
					d = new vec3d(dim->x/2, dim->y/2, dim->z/2);
					
					//test if the child allready exists
					if(testIfChildExists(Octree::Typelist[typeCnt])) {
						delete p;
						delete d;
					} else if(testSphereBox(*toIns->pos, toIns->radius, *p, *d)) {
						std::list<ObjIns*> *nList = new std::list<ObjIns*>();
						nList->push_back(toIns);
						childs.push_back(new Octree(this, this->root, Octree::Typelist[typeCnt], nList, depth+1, 
							this->getFreeChildId(), p, d, maxObjIns));
					} else {
						delete p;
						delete d;
					}
					typeCnt++;
				}
			}
		}
	}
	pthread_spin_unlock(this->lock);
	//std::cout<<"Octree::insert of "<<this->id<<" unlock"<<std::endl;
}

bool Octree::testIfChildExists(Type toTestOri) const {
	std::vector<Octree*>::const_iterator it;
	for(it = childs.begin(); it != childs.end(); it++) {
		if((*it)->type == toTestOri) {
			//std::cout<<"testIfChildExists "<<toTestOri<<std::endl;
			return true;
		}
	}
	return false;
}

void Octree::update(ObjIns *toUpdate) {
	//early break if the ObjIns is still completely within the node
	//if(dim == NULL) std::cout<<"id "<<this->id<<" dim is NULL"<<std::endl;
	//if(this->ori == NULL) this->root->print();
	//std::cout<<"foo"<<std::endl;
	//std::cout<<(int)dim<<std::endl;
	//std::cout<<"bar"<<std::endl;
	if(testSphereInBox(toUpdate->pos, toUpdate->radius, this->ori, this->dim)) {
		this->insert(toUpdate);
		return;
	} else {
		//std::cout<<"Octree "<<id<<" calls Parent with id "<<this->parent->id<<std::endl;
		this->remove(toUpdate->id);
		this->parent->update(toUpdate);
	}
}

void Octree::removeDirect(unsigned long long givenId) {
	std::list<ObjIns*>::iterator it;
	for(it = objIns->begin(); it != objIns->end(); it++) {
		if((*it)->id == givenId) {
			objIns->erase(it);
			break;
		}
	}
	if(objIns->empty() && this->childs.empty() && this->parent != NULL) {
		this->parent->removeChild(this->id);
		return;
	}
}

void Octree::remove(unsigned long long givenId) {
	pthread_spin_lock(this->lock);
	//std::cout<<"Octree::remove of "<<this->id<<" lock"<<std::endl;
	std::list<ObjIns*>::iterator it;
	for(it = objIns->begin(); it != objIns->end(); it++) {
		if((*it)->id == givenId) {
			(*it)->removeFromOctree(this->id);
			objIns->erase(it);
			break;
		}
	}
	if(objIns->empty() && this->childs.empty()) {
		pthread_spin_unlock(this->lock);
		this->notifyRootToDelete();
		return;
	}
	pthread_spin_unlock(this->lock);
	//std::cout<<"Octree::remove of "<<this->id<<" unlock"<<std::endl;
}

//Should be called only by root
void Octree::doRemovel() {
	std::vector<Octree*>::iterator it;
	for(it = this->marked->begin(); it != this->marked->end(); it++) {
		(*it)->removeYourself();
	}
	this->marked->clear();
}

void Octree::removeYourself() {
	if(!this->childs.empty() || !this->objIns->empty()) return;
	if(this->parent != NULL) {
		this->parent->removeChild(this->id);
	}
}

void Octree::removeChild(unsigned long long givenId) {
	//std::cout<<"Octree::removeChild of "<<this->id<<" lock"<<std::endl;
	//std::cout<<this->id<<" should remove "<<id<<std::endl;
	if(this->childs.empty()) {
		return;
	}
	std::vector<Octree*>::iterator it;
	for(it = childs.begin(); it != childs.end(); it++) {
		if((*it)->id == givenId) {
			this->setFreeChildId((*it)->id);
			delete (*it);
			childs.erase(it);
			break;
		}
	}
	if(childs.empty() && this->parent != NULL) {
		this->parent->removeChild(this->id);
	}	
	//std::cout<<"Octree::removeChild of "<<this->id<<" unlock"<<std::endl;
}

void Octree::markForRemovel(Octree *toMark) {
	pthread_spin_lock(this->removeLock);
	/*std::cout<<"mark "<<toMark->id<<std::endl;

	std::vector<Octree*>::iterator it;
	for(it = this->marked->begin(); it != this->marked->end(); it++) {
		if( toMark->id == (*it)->id ) {
			pthread_spin_unlock(this->removeLock);
			return;
		}
	}*/
	this->marked->push_back(toMark);

	pthread_spin_unlock(this->removeLock);
}

void Octree::notifyRootToDelete() {
	if(this->parent != NULL) {
		this->root->markForRemovel(this);
	}
}

void Octree::validate(std::vector<unsigned long long> *usedIds, std::list<ObjIns*> *objects) {
	if(this->ori->test()) {
		std::cout<<"ERROR id "<<this->id<<" ori "<<this->ori->test()<<std::endl;
	}

	if(this->dim->test()) {
		std::cout<<"ERROR id "<<this->id<<" dim "<<this->dim->test()<<std::endl;
	}
	//test if id is allready in use
	for(std::vector<unsigned long long>::iterator it = usedIds->begin();
		it != usedIds->end(); it++) {
		if( (*it) == this->id ) {
			std::cout<<"ERROR id "<<this->id<<" allready used"<<std::endl;
		}
	}
	usedIds->push_back(this->id);

	if(childs.size() > 0 && objIns->size() > 0) {
		std::cout<<"ERROR id "<<this->id<<" has child nodes and objIns"<<std::endl;
	}

	std::list<ObjIns*>::iterator it;
	std::list<ObjIns*>::iterator jt;
	bool bJT = false;
	bool bIT = false;
	for(jt = objects->begin(); jt != objects->end(); jt++) {
		bJT = (*jt)->isKnownToOctree(this->id);
		for(it = objIns->begin(); it != objIns->end(); it++) {
			if( (*jt)->id == (*it)->id ) {
				bIT = (*jt)->isKnownToOctree(this->id);
				if( bJT && bIT ) {
					break;
				} else if( !bJT && bIT ) {
					std::cout<<"ObjIns "<<(*jt)->id<<" knows "<<this->id<<" but not the other way round"<<std::endl;
				} else if( bJT && !bIT ) {
					std::cout<<"Octree "<<this->id<<" knows ObjIns "<<(*jt)->id<<"but not the other way round"<<std::endl;
				}
				break;
			}
		}
		bJT = false;
		bIT = false;
	}

	std::vector<Octree*>::iterator ht;
	for(ht = childs.begin(); ht != childs.end(); ht++) {
		(*ht)->validate(usedIds, objects);
		if(this->depth < 2) {
			std::cout<<"Validate child "<<(*ht)->id<<" done"<<std::endl;
		}
	}
}

bool Octree::sphereInFrustum(const float frustum[6][4], const vec3d *pos, float radius) {
	int p;
    double d = 0.0f;

    for (p = 0; p < 6; p++) {
        d = frustum[p][0] * pos->x + frustum[p][1] * pos->y + frustum[p][2] * pos->z + frustum[p][3];
        if (d <= -radius) {
            return 0;
        }
    }
    return d + radius;
}

std::vector<Octree*>* Octree::getChildren() {
	if(this->childs.size() == 0) {
		return NULL;
	} else {
		std::vector<Octree*> *tmp = new std::vector<Octree*>();
		std::vector<Octree*>::iterator it = this->childs.begin();
		for(;it != this->childs.end(); it++) {
			tmp->push_back(*it);
		}
		return tmp;
	}
	return NULL;
}

std::list<ObjIns*>* Octree::getHoldedObjIns() {
	return this->objIns;
}

std::string Octree::getType() {
	return std::string("Octree");
}

std::string Octree::getID() {
	std::stringstream poss;
	poss<<this->id;
	return poss.str();
}

std::string Octree::getPos() {
	std::stringstream poss;
	poss<<this->ori->x;
	poss<<" ";
	poss<<this->ori->y;
	poss<<" ";
	poss<<this->ori->z;
	return poss.str();
}

std::string Octree::getDim() {
	std::stringstream poss;
	poss<<this->dim->x;
	poss<<" ";
	poss<<this->dim->y;
	poss<<" ";
	poss<<this->dim->z;
	return poss.str();
}

std::string Octree::getChildrenInfo() {
	std::stringstream poss;
	if(this->childs.empty()) {
		poss<<"No Children";
		return poss.str();
	}
	std::vector<Octree*>::iterator it = this->childs.begin();
	int cnt = 0;
	for(; it != this->childs.end(); it++) {
		poss<<(*it)->getID();
		if(cnt == 2 || cnt == 5 || cnt == 7 || cnt == 9) {
			poss<<"\n";
		} else {
			poss<<", ";
		}
		cnt++;
	}
	return poss.str();
}

std::string Octree::getObjIns() {
	std::stringstream poss;
	if(this->objIns->empty()) {
		poss<<"No ObjIns";
		return poss.str();
	}
	std::list<ObjIns*>::iterator it = this->objIns->begin();
	int cnt = 0;
	for(; it != this->objIns->end(); it++) {
		poss<<(*it)->getID();
		if(cnt == 2 || cnt == 5 || cnt == 7 || cnt == 9) {
			poss<<"\n";
		} else {
			poss<<", ";
		}
		cnt++;
	}
	return poss.str();
}

double Octree::getRadius() {
	return this->radius;
}

Octree* Octree::getWithId(unsigned long long toFind) {
	if(this->id == toFind) {
		return this;
	} else {
		Octree *tmp = NULL;
		std::vector<Octree*>::iterator it = this->childs.begin();
		for(; it != this->childs.end(); it++) {
			tmp = (*it)->getWithId(toFind);	
			if(tmp != NULL) {
				return tmp;
			}
		}
		return NULL;
	}
}

Octree* Octree::sGetWithId(unsigned long long toFind) {
	return Octree::current->getWithId(toFind);
}	
