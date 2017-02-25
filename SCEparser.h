//SCEparser.h													Wendy Chen, 2/2017

//Contains #include, using, and functions for the graph class

//Workflow.  sce -> scene object for intermediate parsing, using parser object
//			scene object -> internal structs, using scene object


#ifndef SCEPARSER_H
#define SCEPARSER_H

#include "main.h"
// #include "vec3.h"

#include <list>
#include <unordered_map>


using std::list;
using std::unordered_map;

void test(void);

/************************************/
/*   Scene Object (Intermediate)    */
/************************************/

class SCEscene
{
private:
	//intialize lists
	list<struct Camera> cameras;
	int n_cameras;
	list<struct Light> lights;
	int n_lights;
	list<struct Material> materials;
	int n_materials;
	list<struct Sphere> spheres;
	int n_spheres;
	list<struct Triangle> triangles;
	int n_triangles;
	


public:
	//add_camera, add_light, to a list in private
	void add_camera(vec3 point, vec3 toPoint, float fieldOfView, vec3 up);
	void add_light(lightType type, vec3 point, vec3 color);
	void add_material(vec3 color, materialType type, int metal, float specular, float lambert, float ambient, float exponent);
	void add_sphere(vec3 point, float radius, int materialIndex);
	void add_triangle(vec3 point1, vec3 point2, vec3 point3, int materialIndex);

};


//writeH takes Scene instance and creates struct
//parser has functions to read sce text file and generate scene object
class Parse
{
private:
	//
public:
	//do we need attribute parsing functions?

	//when SCEscene object is made, can call this
	void writeH(SCEscene scene, char * h_name);
};


#endif