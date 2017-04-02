//SCEparser.h													Wendy Chen, 2/2017

//Contains #include, using, and functions for the graph class

//Workflow.  sce -> scene object for intermediate parsing, using parser object
//			scene object -> internal structs, using scene object


#ifndef SCEPARSER_H
#define SCEPARSER_H

#include "main.h"
#include "vec3.h"
#include "intersect.h"

#include <stdio.h>

#include <vector>
#include <unordered_map>
#include "boost/tuple/tuple.hpp"

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
	Scene s_scene;
	


public:
	SCEscene();
	//add_camera, add_light, to a list in private
	void add_camera(vec3 point, vec3 toPoint, float fieldOfView, vec3 up);
	void add_light(lightType type, vec3 point, vec3 color);
	void add_material(vec3 color, materialType type, int metal, float specular, float lambert, float ambient, float exponent);
	void add_sphere(vec3 point, float radius, int matIndex);
	void add_triangle(vec3 point1, vec3 point2, vec3 point3, int matIndex);

	void build_scene();
	Scene * get_scene();

	void print_scene();

	void free_cameras(); //not needed
	void free_lights();  //not needed
	void free_materials();  //not needed
	void free_spheres();
	void free_triangles();
	void free_objects();  //not needed

	~SCEscene(){
		//free the vectors that contain pointers
		free_spheres();
		free_triangles();
	}
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

	void camera(FILE *f, SCEscene *scene);
	void light(FILE *f, SCEscene *scene);
	void material(FILE *f, SCEscene *scene);
	void sphere(FILE *f, SCEscene *scene);
	void triangle(FILE *f, SCEscene *scene);
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
	vec3 split_point;
	int depth;
	KDnode* left;
	KDnode* right;
public:
	KDnode(){};
	KDnode* init_root(vec3 min, vec3 max, SCEscene* scene);
	axisType getLongestAxis();
	vec3 getMid(axisType axis);
	void populate(KDnode* parent){};
	void split();

	~KDnode(){};
};

class KDtree{
private:
	KDnode* root;
public:
	//constructor should take arguments for KDnode init_root
	KDtree(){};
	KDnode* get_kdtree(){
		return this->root;
	};
	void print();
	~KDtree(){};
};


#endif