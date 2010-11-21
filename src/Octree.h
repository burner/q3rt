#ifndef OCTREE
#define OCTREE

#include <list>
#include <vector>
#include <stack>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <typeinfo>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_projection.hpp"
#include "../glm/gtx/type_ptr.hpp"
#include "../glm/GLM_GTX_transform.h"
class Octree;

class vec3d;
class vec3f;
class RenderAble;
/** ObjIns as well as Octree inherivate
 * this calls so they can be display in
 * the tree on the left side of the gtk
 * info center
 */
class GtkTreeType {
	public:
		virtual std::string getType() = 0;
		virtual std::string getID() = 0;
		virtual std::string getPos() = 0;
		virtual double getRadius() = 0;
};

class ObjInsAbstract {

};

class ObjIns : GtkTreeType {
	private:
		std::vector<vec3d*> *splinePoints;
		void initSpline();
		//static const unsigned nPoints = 6;

		/** This variable is used to give all ObjIns a global
		 * time for all timetaking. This is useful because the update
		 * function of each ObjIns not nessescarily at the same time
		 * but the time needs to be same for consistency reasons.
		 *
		 * See ObjIns::generalUpdate() for more information
		 */
		static unsigned long time;

		/** The next two variables are used for the bSpline
		 * compution. 
		 *
		 * Starttime represents, well the start or 0.0 for the 
		 * spline.
		 * TimeSlice tells you how long it takes from 0.0 to 1.0
		 */
		unsigned startTime;
		unsigned timeSlice;
		
		///counter is need for the spline as well
		unsigned counter;
		
		///only if moveable is set the spline stuff is done
		bool moveable;

		/** New vec3d's for the next spline are only
		 * creates within the limits of the positiv
		 * and negativ splineBox
		 */
		vec3d *splineBox;

		/** This long is used to count the time
		 * this objins has been rendered
		 */
		unsigned long rendered;

	public:
		/** distance to the camera. This is used in the 
		 * Renderer to sort the ObjIns so the manuell
		 * Painter algorithm can work.
		 */
		double distance;

		/** Computes all updates need by all ObjIns Objects. 
		 * The imported stuff is that this needs to be called before
		 * the virtual Update function.
		 */
		static void generalUpdate();

		/** This std::vector stores a pointer to all Octree Nodes
		 * this ObjIns touches or is in. This is needed to run the
		 * Bottom-Up Update described in the thesis.
		 */
		std::vector<Octree*> *node;

		///The position of the ObjIns.
		vec3d *pos;

		///The rotation angles of the ObjIns.
		vec3f *rot;

		/** This float and the position can be used to create
		 * an Sphere where every part of the ObjIns Mesh is 
		 * contained in. This is used to compute the place of
		 * the ObjIns within the Octree.
		 */
		float radius;

		/** The currentId is used to assign a unique id to every
		 * ObjIns. Every time you create an ObjIns you run
		 * this->id = currentId++
		 */
		static unsigned long currentId;
		unsigned long long id;
		unsigned long long getIDint() { return id; };

		/** The Obj class Object that will be rendered as a
		 * the ObjIns. It is imported to know that the Obj
		 * object is not nessescarily unique to the ObjIns
		 * so never modify the Obj from within the ObjIns.
		 */
		RenderAble *object;

		ObjIns(vec3d *pos, RenderAble *object, bool moveable = true);
		ObjIns(vec3d *pos, std::string eob, vec3d *splineBox, bool moveable = true);
		ObjIns(double nx, double ny, double nz, float radius);
		ObjIns(double nx, double ny, double nz, float radius, Octree *nNode);
		ObjIns(double nx, double ny, double nz, float radius, unsigned long nMass);
		ObjIns(vec3d *npos, float nRadius);
		~ObjIns();

		/** Whenever the ObjIns is assigned to a new Octree
		 * Node, this function is used to notify the ObjIns
		 * about this. The Octree pointer is than added to the
		 * node std::vector. Every Octreee is only stored once.
		 */
		void setOctree(Octree *nNode);

		/// This deletes an Octree form the node std::vector.
		void removeFromOctree(unsigned long long id);

		/** Checks if the ObjIns is know to the Octree with
		 * the given id.
		 */
		bool isKnownToOctree(unsigned long long octreeId);

		/** This function is called from the ObjInsManader. The
		 * fact that this can happen in parrallel, you have to make
		 * sure that you don't call function from within, that can
		 * cause racecondition some else.
		 */
		void update(float dt);

		/** You have to call this to acctually render the 
		 * object. This function more or less only delegates the
		 * parameter to the Obj object.
		 */
		void render(glm::mat4 proj, glm::mat4 view, vec3d *camPos);

		///prints info about the objIns to stdout.
		void printInfo() const;
		void printRenderCount() const;

		/** Before you sort the render::inFrustum vector
		 * call this function so the distance variable in the
		 * ObjIns Object is correct.
		 */
		void distanceToCam(vec3d cam);

		/** These function are used to display the Information
		 * within in gtk Info center.
		 */
		std::string getType();
		std::string getID();
		std::string getPos();
		std::string getRot();
		double getRadius(); 
		std::string getObjectName();
};

class Octree : GtkTreeType {
	private:
		enum Type {
			LeftTopBack,
			LeftTopFront,
			LeftBottomBack,
			LeftBottomFront,
			RightTopBack,
			RightTopFront,
			RightBottomBack,
			RightBottomFront,
			Root
		};
		static const Type Typelist[];

		/// A pointer to the Octree root
		static Octree *current;
		///root only
		std::vector<Octree*> *marked;
		pthread_spinlock_t *removeLock;

		/** This spinlock is used to lock the Node,
		 *  so only one thread can insert data into it
		 */
		pthread_spinlock_t *lock;

		std::vector<unsigned long long> *childIds;
		void initChildIds();

		/** A pointer to the parent of the Node.
		 *  The root node has 0x0 or NULL as parent.
		 */
		Octree *parent;

		/** Every Octree Node has a pointer to the root
		 *  node, so it can mark itself for removel.
		 */
		Octree *root;

		///The depth of the node
		int depth;
		static int maxDepth;

		/** This is a list of all the ObjIns contained in the
		 *  Octree Node. Only leaf nodes have objIns's.
		 *  A List is because insertion allways costs O(1)
		 */
		std::list<ObjIns*> *objIns;
		double reach;

		/** This std::vector contains pointer to all
		 *  children of the Octree. New children get inserted
		 *  here. And children are removed from here if they no
		 *  longer have children of their own or hold no more
		 *  ObjIns.
		 */
		std::vector<Octree*> childs;

		/** If an Octree Node is about to hold more ObjIns
		 *  than this variable, it will be split.
		 */
		unsigned maxObjIns;

		/** This is a not thread safe function to remove 
		 *  a child node from the Node.
		 */
		void removeChild(unsigned long long id);

		/** This functions markes yourself for deletion
		 *  by the root node. So you get deleted next time
		 *  the root cleans up.
		 */
		void notifyRootToDelete();

		/** Returns the next free child id.
		 *  Im not using this any more.
		 */
		unsigned long long getFreeChildId(); 
		
		/** Release the Id, so it can be used again
		 *  by another octree node.
		 */
		void setFreeChildId(unsigned long long id);

		/** This creates and tests all ObjIns in an Octree
		 *  Node against all eight possible childs.
		 *  Call this only if you have no children attached
		 *  to the node allready.
		 */
		void split();

		/** By calling this function, you can insert
		 *  an ObjIns into the std::list of ObjIns and be 
		 *  sure that the ObjIns is only once in that list.
		 *  This is usally done if you want to insert a 
		 *  single ObjIns.
		 */
		void insertIntoObjInsList(ObjIns *toInsert);

		/** By passing the origin vec3d to this function
		 *  this does check if the Octree child does allready
		 *  exists.
		 */
		bool testIfChildExists(Type toTestOri) const;

		/** This checks if frustum and the passed
		 *  sphere touch.
		 */
		static bool sphereInFrustum(const float frustum[6][4],
			 const vec3d *pos, float radius);

		/** Creates a sphere from the Octree Box.
		 *  This is later used to test with the
		 *  sphereInFrustum function.
		 */
		void computeSphere();
	public:

		Type type;

		/** The id of the Octree Node
		 */
		unsigned long long id;

		/** The two vectors make up the cube
		 *  to the Octree ori is the top left back
		 *  corner of the cube.
		 */
		vec3d *ori, *dim;

		/** The middle vector of the cube. With
		 *  the radius is creates a sphere for
		 *  collisions detection.
		 */
		vec3d *mid;	
		double radius;

		static int maxUsedDepth;

		/** This function is used to make all ObjIns
		 *  insert after a split, that they are in an
		 *  new node.
		 */
		void makeObjInsAware();

		Octree(Octree *parent, int dpth, unsigned long long id, vec3d *n_pos, 
			vec3d *n_dim, unsigned n_maxObjIns);
		Octree(Octree *root, Octree *parent, Type type, std::list<ObjIns*> *nObjIns, 
			int dpth, unsigned long long id, vec3d *n_pos, vec3d *n_dim, unsigned n_maxObjIns);
		~Octree();

		/** This runs the Octree recursively while
		 *  checking if the Node lies in the Frustum.
		 */
		void render(float frustum[6][4]);

		/** Prints infos about the Node and
		 *  than calls the print function of all
		 *  child nodes.
		 *
		 *  The next function prints the info to a 
		 *  given Stream.
		 */
		void print();
		void print(std::ofstream &file);

		void printObjInsListAdr();

		/** This function inserts an ObjIns into the
		 *  Octree Node. If the Node would have more than
		 *  the allowed number of ObjIns it splits itself.
		 */
		void insert(ObjIns *toIns);

		/** Searches itself and all children for the given ObjIns
		 *  and removes the ObjIns. This could led to an deletion
		 *  of the Octree Node.
		 */
		void remove(unsigned long long id);

		/** RemoveDirect does the same as remove but not thread safe.
		 */
		void removeDirect(unsigned long long id);

		/** By calling this function you mark yourself
		 *  to be removed by the Root node.
		 */
		void markForRemovel(Octree *toMark);

		/** DoRemovel does the actual removal of the Node,
		 *  by calling removeYourself on the node.
		 */
		void doRemovel();
		void removeYourself();

		unsigned getMaxObjIns() { return maxObjIns; }

		/** An ObjIns calls this one to check whether or
		 *  not it still is in the Node completely.
		 */
		void update(ObjIns*);

		/** Debug method's to check the tree structure.
		 */
		void validate(std::vector<unsigned long long> *usedIds, 
			std::list<ObjIns*> *obIns);

		/** These method's are used to display the Octree
		 *  in the gtk info view.
		 */
		Octree* getWithId(unsigned long long toFind);	
		static Octree* sGetWithId(unsigned long long toFind);	
		std::vector<Octree*>* getChildren();
		std::list<ObjIns*>* getHoldedObjIns();
		std::string getType();
		std::string getID();
		std::string getPos();
		std::string getDim();
		std::string getChildrenInfo();
		std::string getObjIns();

		/** Returns the radius of the Octree Node Cube.
		 */
		double getRadius(); 

};
#endif

void* octreeChildBuilder(void *args);
