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

//writeH takes Scene instance and creates struct
//parser has functions to read sce text file and generate scene object
class Parse
{
private:
	//unordered map from string to lightType
	unordered_map<char *, lightType> toLightType;
	//unordered map from string to materialType
	unordered_map<char *, materialType> toMaterialType;
	//unordered map from string to vec3
	unordered_map<char *, vec3> toVec3;

	//unordered map for attributes, flush after each use
	vector<char *> attrList; //keep track of insertion order of attrs
	unordered_map<char *, char *> attrMap; //keep for look up of attr -> args

	//list for args for each attr: 
	vector<char *> argList;
	//need to convert: should be tuple of different fields?
	// first int is index into tuple for argument
	// char * -> int 
	// char * -> float
	// char * -> lightType
	// char * -> materialType
	// char * -> vector (ie. UP, or a list of size 3)
	tuple<int, int, float, lightType, materialType, vec3> argConvert; //no need to flush

	//unordered map for each SCEscene function
	unordered_map<char *, int> toCameraArgTypes; //so we know what each attr needs to be ocnverted to
	tuple<vec3, vec3, float, vec3> toCameraArgs;


public:
	Parse();
	//do we need attribute parsing functions?

	//when SCEscene object is made, can call this
	// struct Scene * writeH(SCEscene scene, char * h_name);
	void getAttrs(char * attributes); //for filling attrList, attrMap
	void flushAttrs(); //flush attrList and attrMap after use
	void printAttrs(); //print attrList and attrMap

	void getArgs(char * attr); //given token in attrMap, put args in argList
	void flushArgs(); //flush argsList after use
	void printArgs(); //print argList

	// void convertTypeArg();
	// void printArgConvert();
	// void convertArgList(); //new
	// void printConvertedArgList(); //new

	int getArgType(unordered_map<char *, int> primitiveMap, char * attribute);
	void convertTypeArg(argType convertType);

	//isVec() need to check a list of 3 ints and say it's a vector

	

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