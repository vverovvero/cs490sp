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
	vector<struct Object>* get_objects();

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
	vector<struct Object> objects;
	vec3 split_point;
	axisType split_axis;
	int depth;
	KDnode* left;
	KDnode* right;
public:
	KDnode(vec3 min, vec3 max, int depth){
		this->min = min;
		this->max = max;
		this->depth = depth;
		this->left = NULL;
		this->right = NULL;
	};
	void init_root(SCEscene* scene){
		//set root objects to scene objects
		vector<struct Object>* scene_objects = (*scene).get_objects();
		this->objects = (*scene_objects);
	};
	void getLongestAxis(){
		//assumes that this->min and this->max have been initialized
		float xdiff = fabs(max.x - min.x);
		float ydiff = fabs(max.y - min.y);
		float zdiff = fabs(max.z - min.z);
		if(xdiff >= ydiff && xdiff >= zdiff){
			this->split_axis = X;
		}
		else if(ydiff >= xdiff && ydiff >= zdiff){
			this->split_axis = Y;
		}
		else{
			this->split_axis = Z;
		}
	};
	// void getMid(axisType axis){
	// 	//assumes that min, max, and split_axis are initialized
	// };
	void getNaiveMid(){
		//assumes that min, max, and split_axis are initialized
		vec3 split = {
			.x = (this->max.x + this->min.x)/2.0,
			.y = (this->max.y + this->min.y)/2.0,
			.z = (this->max.z + this->min.z)/2.0
		};
		this->split_point = split;
	}
	pair<vec3, vec3> getBoundsFromMid(){
		//midMin and midMax inherit values
		vec3 midMin = this->min;
		vec3 midMax = this->max;
		switch(this->split_axis){
			case X:
				midMin.x = this->split_point.x;
				midMax.x = this->split_point.x;
				break;
			case Y:
				midMin.y = this->split_point.y;
				midMax.y = this->split_point.y;
				break;
			case Z:
				midMin.z = this->split_point.z;
				midMax.z = this->split_point.z;
				break;
			default:
				break;
		};
		return pair<vec3, vec3> (midMin, midMax);
	}
	void populate(KDnode* parent){
		//for each object in parent, add to this node if intersects
		int i;
		int size = parent->objects.size();
		for(i=0; i<size; i++){
			Object obj = parent->objects[i];
			if(objectBB(obj, this->min, this->max)){
				this->objects.push_back(obj);
			}
		}
	};
	void split(){
		if(this->objects.size() > 10){
			//For current node, get long axis and mid point
			(*this).getLongestAxis();
			(*this).getNaiveMid();
			//get the bounds for left and right children
			pair<vec3, vec3> midBounds = (*this).getBoundsFromMid();
			vec3 midMin = midBounds.first;
			vec3 midMax = midBounds.second;
			//create the left and right children
			this->left = new KDnode(this->min, midMax, this->depth+1);
			this->right = new KDnode(midMin, this->max, this->depth+1);
			//populate left child and right child
			(*this->left).populate(this);
			(*this->right).populate(this);
			//call split on left child and right child
			(*this->left).split();
			(*this->right).split();
		}
	};
	//print node and its children
	void print(){
		printf("Node depth: %d =============\n", this->depth);
		printf("Min bound(%.2f, %.2f, %.2f), Max bound(%.2f, %.2f, %.2f)\n",
			(this->min).x, (this->min).y, (this->min).z,
			(this->max).x, (this->max).y, (this->max).z);
		//print the objects
		int i;
		int size = this->objects.size();
		for(i=0; i<size; i++){
			Object obj = this->objects[i];
			if(obj.type == SPHERE){
				Sphere* sph = (Sphere*) obj.object;
				printf("Sphere point(%.2f, %.2f, %.2f), radius %.2f\n",
					(*sph).point.x, (*sph).point.y, (*sph).point.z,
					(*sph).radius);

			}
			else if(obj.type == TRIANGLE){
				Triangle* tri = (Triangle*) obj.object;
				printf("Triangle p1(%.2f, %.2f, %.2f), p2(%.2f, %.2f, %.2f), p3(%.2f, %.2f, %.2f)\n",
					(*tri).point1.x, (*tri).point1.y, (*tri).point1.z,
					(*tri).point2.x, (*tri).point2.y, (*tri).point2.z,
					(*tri).point3.x, (*tri).point3.y, (*tri).point3.z);
			}
			else{
				printf("Impossible object\n");
			}
		}
		if(this->left != NULL){
			(*this->left).print();
		}
		if(this->right != NULL){
			(*this->right).print();
		}
	}
	//think about destruction
	~KDnode(){};
};

class KDtree{
private:
	KDnode* root;
public:
	//constructor should take arguments for KDnode init_root
	KDtree(vec3 min, vec3 max, SCEscene* scene){
		printf("Creating tree!~~~~~~\n");
		//create root with maximum bounding box
		KDnode* root = new KDnode(min, max, 0);
		//copy all scene objects to root
		(*root).init_root(scene);
		//split
		(*root).split();
		//assign root to KDtree root
		this->root = root;
		printf("Finished tree!~~~~~~\n");
	};
	KDnode* get_kdtree(){
		return this->root;
	};
	//think about destruction
	~KDtree(){};
};


#endif