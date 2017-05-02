//SCEparser.h													Wendy Chen, 2017

//Contains classes SCEscene, Parse, KDnode, KDtree

//Workflow.  sce -> scene object for intermediate parsing, using parser object
//			scene object -> internal structs, using scene object
//			internal structs -> kd-tree

#ifndef SCEPARSER_H
#define SCEPARSER_H

#include "main.h"
#include "vec3.h"
#include "intersect.h"

#include <stdio.h>
#include <math.h>

#include <vector>
#include <unordered_map>
#include "boost/tuple/tuple.hpp"

using std:: pair;
using std::vector;
using std::unordered_map;
using boost::tuple;



/************************************/
/*   Scene Object (Intermediate)    */
/************************************/

class SCEscene
{
private:
	//intialize lists
	vector<struct Camera> cameras;
	vector<struct Light> lights;
	vector<struct Material> materials;
	vector<struct Sphere*> spheres;
	int n_spheres;
	vector<struct Triangle*> triangles;
	int n_triangles;
	vector<struct Object> objects;
	vector<struct Film> films;
	vector<struct BoundBox> boxes;
	Scene s_scene;
	
public:
	SCEscene();
	//add_camera, add_light, to a list in private
	void add_film(int width, int height);
	void add_camera(vec3 point, vec3 toPoint, float fieldOfView, vec3 up, float lensRadius, float focalDepth);
	void add_light(lightType type, vec3 point, vec3 color);
	void add_material(vec3 color, materialType type, int metal, float specular, float lambert, float ambient, float exponent);
	void add_sphere(vec3 point, float radius, int matIndex);
	void add_triangle(vec3 point1, vec3 point2, vec3 point3, int matIndex);
	void add_boundbox(vec3 min, vec3 max);
	//build the scene
	void build_scene();
	Scene * get_scene();
	vector<struct Object>* get_objects();
	//sanity check
	void print_scene();
	//destructors
	void free_spheres();
	void free_triangles();
	~SCEscene();
};


//parser reads from a binary file and adds objects to SCEscene
class Parse
{
private:
	//unordered map from string to lightType
	unordered_map<int, lightType> toLightType;
	//unordered map from string to materialType
	unordered_map<int, materialType> toMaterialType;

public:
	Parse();
	//constructors for updating SCEscene scene
	void film(FILE *f, SCEscene *scene);
	void camera(FILE *f, SCEscene *scene);
	void light(FILE *f, SCEscene *scene);
	void material(FILE *f, SCEscene *scene);
	void sphere(FILE *f, SCEscene *scene);
	void triangle(FILE *f, SCEscene *scene);
	void boundbox(FILE *f, SCEscene *scene);
	int parseSCE(char *infile, SCEscene *scene);
};


/************************************/
/*           Acceleration           */
/************************************/

typedef enum {X, Y, Z} axisType;

class KDnode{
private:
	vec3 min;
	vec3 max;
	vector<struct Object*> objects;
	int n_objects;
	vec3 split_point;
	axisType split_axis;
	int depth;
	KDnode* left;
	KDnode* right;
public:
	KDnode(vec3 min, vec3 max, int depth);
	void init_root(SCEscene* scene);
	void getLongestAxis();
	void getCenterMid(); //unused
	void getNaiveMid();
	pair<vec3, vec3> getBoundsFromMid();
	void populate(KDnode* parent);
	void split();
	void print();
	vector<struct Object*>* getObjects();
	vec3 getMin();
	vec3 getMax();
	KDnode* getLeft();
	KDnode* getRight();	
	//think about destruction
	~KDnode();
};

class KDtree{
private:
	KDnode* root;
public:
	//constructor should take arguments for KDnode init_root
	KDtree(vec3 min, vec3 max, SCEscene* scene);
	KDnode* get_kdtree();
	//think about destruction
	~KDtree();
};


#endif