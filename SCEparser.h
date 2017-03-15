//SCEparser.h													Wendy Chen, 2/2017

//Contains #include, using, and functions for the graph class

//Workflow.  sce -> scene object for intermediate parsing, using parser object
//			scene object -> internal structs, using scene object


#ifndef SCEPARSER_H
#define SCEPARSER_H

#include "main.h"
#include "vec3.h"

#include <stdio.h>

#include <vector>
#include <unordered_map>
#include "boost/tuple/tuple.hpp"

using std::vector;
using std::unordered_map;
using boost::tuple;

Scene* test(void);


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

	// void free_camera();
	// void print_camera();

	void build_scene();
	Scene * get_scene();

	void print_scene();

};


typedef enum {INT, FLOAT, LIGHTTYPE, MATERIALTYPE, VEC3} argType;

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

	void camera(FILE *f, SCEscene scene);
	void light(FILE *f, SCEscene scene);
	void material(FILE *f, SCEscene scene);
	void sphere(FILE *f, SCEscene scene);
	void triangle(FILE *f, SCEscene scene);
	void parseSCE(char *infile, SCEscene scene);
};


#endif