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


using std::vector;
using std::unordered_map;

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


//writeH takes Scene instance and creates struct
//parser has functions to read sce text file and generate scene object
class Parse
{
private:
	//unordered map for attributes, flush after each use
	vector<char *> attrList; //keep track of insertion order of attrs
	unordered_map<char *, char *> attrMap; //keep for look up of attr -> args
public:
	//do we need attribute parsing functions?

	//when SCEscene object is made, can call this
	// struct Scene * writeH(SCEscene scene, char * h_name);
	void getAttrs(char * attributes); //for filling attrList, attrMap
	void flushAttrs(); //flush attrList and attrMap after use

	void printAttrs(); //print attrList and attrMap

	// void camera(SCEscene scene, char * attributes);
	// void light(SCEscene scene, char * attributes);
	// void material(SCEscene scene, char * attributes);
	// void sphere(SCEscene scene, char * attributes);
	// void triangle(SCEscene scene, char * attributes);
	// void include(SCEscene scene, char * attributes);
	void parseSCE(char *infile);

	//getline: input text file -> one line
	//parse getPrimitive : one line -> identify primitive and call with remainder of line
	//get attributes: remainder of line -> tokenize into an unordered map
	//makePrimitive: unordered map of attributes -> adding primitive to the scene


};


#endif