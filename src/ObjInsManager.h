#ifndef OBJINSMANAGER
#define OBJINSMANAGER

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include "Camera.h"
#include "Focus.h"
#include "Octree.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_projection.hpp"
#include "../glm/gtx/type_ptr.hpp"
#include "../glm/GLM_GTX_transform.h"

class ObjIns;
class Octree;
class Focus;

class ObjInsManager {
	private:
		/** This map is used to store all Obj's (Mesh).
		 * This way every mesh only needs to be stored once
		 * in the ram and once on the gpu.
		 */
		static std::map<std::string, Obj*> objects;

		/** According to this number threads will be spawned
		 * to process the update function of the ObjIns.
		 * For more infos see the header of the ObjIns class
		 * in management/Octree.h
		 */
		unsigned numThreads;

		float currentTime;

		/** A pointer to the Octree which handles the space
		 * portioning of the room.
		 */
		Octree *root;

		/** This class handles a display of
		 * a viewable pointer to the selected
		 * ObjIns. This is for finding the ObjIns
		 * faster in the space.
		 */	
		Focus *focus;

		bool focusBool;
		unsigned long long focusId;

	public:
		void printRenderCount() const;
		static ObjInsManager *current;
		std::vector<ObjIns*> *objIns;
		ObjInsManager(Octree *nRoot, unsigned numThreads);
		~ObjInsManager();
		void insert(ObjIns *nObject);
		static void sInsert(ObjIns *nObject);
		void remove(ObjIns *toRemove);
		void render(float proj[16]);
		void remove(unsigned long id);
		void update(float time);
		static Obj* getObject(std::string name);
		std::list<ObjIns*>* getElementsAsList();
		static unsigned getNumObjIns();
		static Octree* sGetCurrentOctree();
		Octree* getCurrentOctree();
		static std::vector<ObjIns*>* sGetCurrentObjIns();
		std::vector<ObjIns*>* getCurrentObjIns();
		static ObjIns* sGetObjIns(unsigned long long id);
		ObjIns* getObjIns(unsigned long long id);

		static void sSetFocused(unsigned long long id);
		static void sDisableFocus();
		void setFocused(unsigned long long id);
		void disableFocus();

		void renderFocus();

		void printInfo();
		void printOctree();
};

struct UpdateJob {
	unsigned l;
	unsigned h;
	float time;
	std::vector<ObjIns*> *vec;
	UpdateJob(std::vector<ObjIns*> *v, unsigned nl, unsigned nh, float nTime) {
		this->vec = v;
		this->l = nl;
		this->h = nh;
		this->time = nTime;
	}
};

void* updateFunction(void *p);
#endif
