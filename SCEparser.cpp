//SCEparser.cpp										Wendy Chen, 2/2017

//Implements scene parser, as originally defined in SCEparser.py with some modifications.

//Sources:
//http://stackoverflow.com/questions/1856468/how-to-output-ieee-754-format-integer-as-a-float

#include "SCEparser.h"

#include <iostream>
#include <stdlib.h> //has exit, EXIT_FAILURE
#include <errno.h>

/////////////////////
//KDnode
////////////////////
KDnode::KDnode(vec3 min, vec3 max, int depth){
	this->min = min;
	this->max = max;
	this->depth = depth;
	this->left = NULL;
	this->right = NULL;
};
void KDnode::init_root(SCEscene* scene){
	//set root objects to scene objects
	vector<struct Object>* scene_objects = (*scene).get_objects();
	int i;
	int size = (*scene_objects).size();
	for(i=0; i<size; i++){
		this->objects.push_back(&(*scene_objects)[i]);
	}
};
// void KDnode::getLongestAxis(){
// 	//assumes that this->min and this->max have been initialized
// 	float xdiff = fabs(max.x - min.x);
// 	float ydiff = fabs(max.y - min.y);
// 	float zdiff = fabs(max.z - min.z);
// 	if(xdiff >= ydiff && xdiff >= zdiff){
// 		this->split_axis = X;
// 	}
// 	else if(ydiff >= xdiff && ydiff >= zdiff){
// 		this->split_axis = Y;
// 	}
// 	else{
// 		this->split_axis = Z;
// 	}
// };
// // void KDnode::getMid(axisType axis){
// // 	//assumes that min, max, and split_axis are initialized
// // };
// void KDnode::getNaiveMid(){
// 	//assumes that min, max, and split_axis are initialized
// 	vec3 split = {
// 		.x = (this->max.x + this->min.x)/2.0,
// 		.y = (this->max.y + this->min.y)/2.0,
// 		.z = (this->max.z + this->min.z)/2.0
// 	};
// 	this->split_point = split;
// }
// pair<vec3, vec3> KDnode::getBoundsFromMid(){
// 	//midMin and midMax inherit values
// 	vec3 midMin = this->min;
// 	vec3 midMax = this->max;
// 	switch(this->split_axis){
// 		case X:
// 			midMin.x = this->split_point.x;
// 			midMax.x = this->split_point.x;
// 			break;
// 		case Y:
// 			midMin.y = this->split_point.y;
// 			midMax.y = this->split_point.y;
// 			break;
// 		case Z:
// 			midMin.z = this->split_point.z;
// 			midMax.z = this->split_point.z;
// 			break;
// 		default:
// 			break;
// 	};
// 	return pair<vec3, vec3> (midMin, midMax);
// }
// void KDnode::populate(KDnode* parent){
// 	//for each object in parent, add to this node if intersects
// 	int i;
// 	int size = parent->objects.size();
// 	for(i=0; i<size; i++){
// 		Object obj = parent->objects[i];
// 		if(objectBB(obj, this->min, this->max)){
// 			this->objects.push_back(obj);
// 		}
// 	}
// };
// void KDnode::split(){
// 	if(this->objects.size() > 10){
// 		//For current node, get long axis and mid point
// 		(*this).getLongestAxis();
// 		(*this).getNaiveMid();
// 		//get the bounds for left and right children
// 		pair<vec3, vec3> midBounds = (*this).getBoundsFromMid();
// 		vec3 midMin = midBounds.first;
// 		vec3 midMax = midBounds.second;
// 		//create the left and right children
// 		this->left = new KDnode(this->min, midMax, this->depth+1);
// 		this->right = new KDnode(midMin, this->max, this->depth+1);
// 		//populate left child and right child
// 		(*this->left).populate(this);
// 		(*this->right).populate(this);
// 		//call split on left child and right child
// 		(*this->left).split();
// 		(*this->right).split();
// 	}
// };
// //print node and its children
// void KDnode::print(){
// 	printf("Node depth: %d =============\n", this->depth);
// 	printf("Min bound(%.2f, %.2f, %.2f), Max bound(%.2f, %.2f, %.2f)\n",
// 		(this->min).x, (this->min).y, (this->min).z,
// 		(this->max).x, (this->max).y, (this->max).z);
// 	//print the objects
// 	int i;
// 	int size = this->objects.size();
// 	for(i=0; i<size; i++){
// 		Object obj = this->objects[i];
// 		if(obj.type == SPHERE){
// 			Sphere* sph = (Sphere*) obj.object;
// 			printf("Sphere point(%.2f, %.2f, %.2f), radius %.2f\n",
// 				(*sph).point.x, (*sph).point.y, (*sph).point.z,
// 				(*sph).radius);

// 		}
// 		else if(obj.type == TRIANGLE){
// 			Triangle* tri = (Triangle*) obj.object;
// 			printf("Triangle p1(%.2f, %.2f, %.2f), p2(%.2f, %.2f, %.2f), p3(%.2f, %.2f, %.2f)\n",
// 				(*tri).point1.x, (*tri).point1.y, (*tri).point1.z,
// 				(*tri).point2.x, (*tri).point2.y, (*tri).point2.z,
// 				(*tri).point3.x, (*tri).point3.y, (*tri).point3.z);
// 		}
// 		else{
// 			printf("Impossible object\n");
// 		}
// 	}
// 	if(this->left != NULL){
// 		(*this->left).print();
// 	}
// 	if(this->right != NULL){
// 		(*this->right).print();
// 	}
// }
// //think about destruction
// KDnode::~KDnode(){};


// /////////////////////
// //KDtree
// ////////////////////
// //constructor should take arguments for KDnode init_root
// KDtree::KDtree(vec3 min, vec3 max, SCEscene* scene){
// 	printf("Creating tree!~~~~~~\n");
// 	//create root with maximum bounding box
// 	KDnode* root = new KDnode(min, max, 0);
// 	//copy all scene objects to root
// 	(*root).init_root(scene);
// 	//split
// 	(*root).split();
// 	//assign root to KDtree root
// 	this->root = root;
// 	printf("Finished tree!~~~~~~\n");
// };
// KDnode* KDtree::get_kdtree(){
// 	return this->root;
// };
// //think about destruction
// KDtree::~KDtree(){};


/////////////////////
// SCEscene
////////////////////

SCEscene::SCEscene():
	n_spheres(0), n_triangles(0), cameras(), lights(), 
	materials(), spheres(), triangles(), objects() {}

void SCEscene::add_camera(vec3 point, vec3 toPoint, float fieldOfView, vec3 up){
	Camera* camera; 
	camera = (Camera *) malloc(sizeof(struct Camera));
	camera->point = point;
	camera->toPoint = toPoint;
	camera->fieldOfView = fieldOfView;
	camera->up = up;

	this->cameras.push_back(*camera);
}

//not needed
void SCEscene::free_cameras(){
	int i, n;
	n = this->cameras.size();
	for(i=0; i<n; i++){
		free(&this->cameras[i]);
	}
	//call destructor for cameras?
}

void SCEscene::add_light(lightType type, vec3 point, vec3 color){
	Light* light;
	light = (Light *) malloc(sizeof(struct Light));
	light->type = type;
	light->point = point;
	light->color = color;

	this->lights.push_back(*light);
}

//not needed
void SCEscene::free_lights(){
	int i, n;
	n = this->lights.size();
	for(i=0; i<n; i++){
		free(&this->lights[i]);
	}
	//call destructor?
}

void SCEscene::add_material(vec3 color, materialType type, int metal, float specular, float lambert, float ambient, float exponent){
	Material* material;
	material = (Material *) malloc(sizeof(struct Material));
	material->color = color;
	material->type = type;
	material->metal = metal;
	material->specular = specular;
	material->lambert = lambert;
	material->ambient = ambient;
	material->exponent = exponent;
	
	this->materials.push_back(*material);
}

//not needed
void SCEscene::free_materials(){
	int i, n;
	n = this->materials.size();
	for(i=0; i<n; i++){
		free(&this->materials[i]);
	}
	//call destructor?
}

void SCEscene::add_sphere(vec3 point, float radius, int matIndex){
	Sphere* sphere;
	sphere = (Sphere *) malloc(sizeof(struct Sphere));
	sphere->point = point;
	sphere->radius = radius;
	sphere->matIndex = matIndex;

	this->spheres.push_back(sphere); //is this necessary?

	this->n_spheres += 1;

	Object* obj;
	obj = (Object *) malloc(sizeof(struct Object));
	obj->type = SPHERE;
	obj->matIndex = matIndex;
	obj->object = this->spheres[this->n_spheres - 1];

	this->objects.push_back(*obj);
}

void SCEscene::free_spheres(){
	int i, n;
	n = this->n_spheres;
	for(i=0; i<n; i++){
		free(this->spheres[i]);
	}
	//call destructor?
}

void SCEscene::add_triangle(vec3 point1, vec3 point2, vec3 point3, int matIndex){
	Triangle* tri;
	tri = (Triangle *) malloc(sizeof(struct Triangle));
	tri->point1 = point1;
	tri->point2 = point2;
	tri->point3 = point3;
	tri->matIndex = matIndex;

	this->triangles.push_back(tri); //is this necessary?

	this->n_triangles += 1;

	Object* obj;
	obj = (Object *) malloc(sizeof(struct Object));
	obj->type = TRIANGLE;
	obj->matIndex = matIndex;
	obj->object = this->triangles[this->n_triangles - 1];

	this->objects.push_back(*obj);
}


void SCEscene::free_triangles(){
	int i, n;
	n = this->n_triangles;
	for(i=0; i<n; i++){
		free(this->triangles[i]);
	}
	//call destructor
}

//not needed
void SCEscene::free_objects(){
	int i, n;
	n = this->n_triangles + this->n_spheres;
	for(i=0; i<n; i++){
		free(&this->objects[i]);
	}
	//call destructor?
}

void SCEscene::build_scene(){
	this->s_scene.camera = &(this->cameras[0]);
	this->s_scene.materials = this->materials;
	this->s_scene.objects = this->objects;
	this->s_scene.lights = this->lights;
	this->s_scene.n_lights = this->lights.size();
	this->s_scene.n_materials = this->materials.size();
	this->s_scene.n_objects = this->objects.size();
}

Scene* SCEscene::get_scene(){
	return &(this->s_scene);
}

vector<struct Object>* SCEscene::get_objects(){
	return &(this->objects);
}

void SCEscene::print_scene(){
	Scene s_scene = this->s_scene;
	//Camera
	Camera camera = *s_scene.camera;
	printf("Camera point=(%f, %f, %f), fieldOfView=%f, toPoint=(%f, %f, %f), up=(%f, %f, %f)\n", 
		camera.point.x, camera.point.y, camera.point.z, 
		camera.fieldOfView, 
		camera.toPoint.x, camera.toPoint.y, camera.toPoint.z, 
		camera.up.x, camera.up.y, camera.up.z);
	//Lights
	for(int i=0; i<this->s_scene.n_lights; i++){
		Light light = this->s_scene.lights[i];
		printf("Light type=%d, point=(%f, %f, %f), color=(%f, %f, %f)\n",
			light.type, light.point.x, light.point.y, light.point.z,
			light.color.x, light.color.y, light.color.z);
	}
	//Materials
	for(int i=0; i<this->s_scene.n_materials; i++){
		Material material = this->s_scene.materials[i];
		printf("Material type=%d, color=(%f, %f, %f), metal=%d, specular=%f, lambert=%f, ambient=%f, exponent=%f\n",
			material.type, 
			material.color.x, material.color.y, material.color.z,
			material.metal, material.specular, material.lambert, material.ambient, material.exponent);
	}
	//Objects
	for(int i=0; i<this->s_scene.n_objects; i++){
		Object object = this->s_scene.objects[i];
		if(object.type == TRIANGLE){
			Triangle* triangle = (Triangle *) object.object;
			printf("Triangle p1(%f, %f, %f), p2(%f, %f, %f), p3(%f, %f, %f), matIndex=%d\n",
				triangle->point1.x, triangle->point1.y, triangle->point1.z,
				triangle->point2.x, triangle->point2.y, triangle->point2.z,
				triangle->point3.x, triangle->point3.y, triangle->point3.z, object.matIndex);
		}
		else if (object.type == SPHERE){
			Sphere* sphere = (Sphere *) object.object;
			printf("Sphere point(%f, %f, %f), radius=%f, matIndex=%d\n",
				sphere->point.x, sphere->point.y, sphere->point.z, 
				sphere->radius, sphere->matIndex);
		}
		else{
			printf("Invalid!!!\n");
		}
	}
}


/////////////////////
// Parse
////////////////////

Parse::Parse(){
	//initialize toLightType hash
	toLightType[0] = OMNI;
	toLightType[1] = SPOT;

	//initialize toMaterialType hash
	toMaterialType[0] = ORIGINAL;
	toMaterialType[1] = PHONG;
}

// #Command codes:
// #0	Camera
// #1	Light
// #2	Material
// #3	Sphere
// #4 	Triangle
// #5 	END

float IEEEInttoFloat(int value) {
	union
	{	
	    unsigned long ul;
	    float f;
	} u;

	u.ul = value;
	return u.f;
}

int swapEndian(int value){
	// Flip to correct for byte order
	return	((value>>24)&0xff) | // move byte 3 to byte 0
            ((value<<8)&0xff0000) | // move byte 1 to byte 2
            ((value>>8)&0xff00) | // move byte 2 to byte 1
            ((value<<24)&0xff000000); // byte 0 to byte 3
}

//read the camera arguments using fread, construct camera
void Parse::camera(FILE *f, SCEscene *scene){
	//Get camera inputs from binary file
	int i, value, retval, swapped;
	float a[10];
	for(i=0;i<10;i++){
		if((retval = fread(&value, 4, 1, f)) > 0){
			swapped = swapEndian(value);
			a[i] = IEEEInttoFloat(swapped);
		}
	}
	//Construct vecs
	vec3 point = {.x=a[0], .y=a[1], .z=a[2]};
	float fov = a[3];
	vec3 toPoint = {.x=a[4], .y=a[5], .z=a[6]};
	vec3 up = {.x=a[7], .y=a[8], .z=a[9]};

	//Add camera to scene
	(*scene).add_camera(point, toPoint, fov, up);
}

//read light arguments using fread, add light to scene
void Parse::light(FILE *f, SCEscene *scene){
	int i, value, retval, swapped;
	lightType l_type;
	float a[6];
	//Get light type
	if((retval = fread(&value, 4, 1, f)) > 0){
		swapped = swapEndian(value);
		l_type = this->toLightType[swapped];
	}
	//Get float args for light
	for(i=0;i<6;i++){
		if((retval = fread(&value, 4, 1, f)) > 0){
			swapped = swapEndian(value);
			a[i] = IEEEInttoFloat(swapped);
		}
	}
	//Construct vecs
	vec3 point = {.x=a[0], .y=a[1], .z=a[2]};
	vec3 color = {.x=a[3], .y=a[4], .z=a[5]};

	//Add light
	(*scene).add_light(l_type, point, color);
}

void Parse::material(FILE *f, SCEscene *scene){
	int i, value, retval, swapped;
	materialType m_type;
	int metal_bool;
	float c[3];
	float a[4];
	//Get color
	for(i=0;i<3;i++){
		if((retval = fread(&value, 4, 1, f)) > 0){
			swapped = swapEndian(value);
			c[i] = IEEEInttoFloat(swapped);
		}
	}
	//Get material type
	if((retval = fread(&value, 4, 1, f)) > 0){
		swapped = swapEndian(value);
		m_type = this->toMaterialType[swapped];
	}
	//Get metal bool
	if((retval = fread(&value, 4, 1, f)) > 0){
		swapped = swapEndian(value);
		metal_bool = swapped;
	}
	//Get remaining attrs
	for(i=0;i<4;i++){
		if((retval = fread(&value, 4, 1, f)) > 0){
			swapped = swapEndian(value);
			a[i] = IEEEInttoFloat(swapped);
		}
	}
	//Make vecs and args
	vec3 color = {.x=c[0], .y=c[1], .z=c[2]};
	float specular = a[0];
	float lambert = a[1];
	float ambient = a[2];
	float exponent = a[3];
	//Add material
	(*scene).add_material(color, m_type, metal_bool, specular, lambert, ambient, exponent);
}

//Get sphere args from binary file, add sphere to scene
void Parse::sphere(FILE *f, SCEscene *scene){
	int i, value, retval, swapped;
	float a[4];
	float radius;
	int matIndex;
	//Get sphere args
	for(i=0;i<4;i++){
		if((retval = fread(&value, 4, 1, f)) > 0){
			swapped = swapEndian(value);
			a[i] = IEEEInttoFloat(swapped);
		}
	}
	//Get sphere matIndex
	if((retval = fread(&value, 4, 1, f)) > 0){
		swapped = swapEndian(value);
		matIndex = swapped;
	}
	//Make vec3 and args
	vec3 point = {.x=a[0], .y=a[1], .z=a[2]};
	radius = a[3];
	//Add sphere
	(*scene).add_sphere(point, radius, matIndex);
}

//Get triangle args from binary file, add triangle to scene
void Parse::triangle(FILE *f, SCEscene *scene){
	int i, value, retval, swapped;
	float a[9];
	int matIndex;
	//Get tri args
	for(i=0;i<9;i++){
		if((retval = fread(&value, 4, 1, f)) > 0){
			swapped = swapEndian(value);
			a[i] = IEEEInttoFloat(swapped);
		}
	}
	//Get matIndex
	if((retval = fread(&value, 4, 1, f)) > 0){
		swapped = swapEndian(value);
		matIndex = swapped;
	}
	//Make vec3
	vec3 point1={.x=a[0], .y=a[1], .z=a[2]};
	vec3 point2={.x=a[3], .y=a[4], .z=a[5]};
	vec3 point3={.x=a[6], .y=a[7], .z=a[8]};
	//Add triangle
	(*scene).add_triangle(point1, point2, point3, matIndex);

}

int Parse::parseSCE(char * infile, SCEscene *scene){
	FILE *f = fopen(infile, "rb");
	if(f == NULL){
		fprintf(stderr, "Unable to open input file\n");
		return -1;
	}

	int cmd;
	int retval;
	while(( retval = fread(&cmd, 4, 1, f)) > 0){
		
		// Flip to correct for byte order
		int swapped = swapEndian(cmd);
		switch (swapped){
			case 0:
				Parse::camera(f, scene);
				break;
			case 1:
				Parse::light(f, scene);
				break;
			case 2:
				Parse::material(f, scene);
				break;
			case 3:
				Parse::sphere(f, scene);
				break;
			case 4:
				Parse::triangle(f, scene);
				break;
			case 5:
				break;
			default:
				printf("Invalid cmd\n");
		}		
	}
	fclose(f);

	return 0;
}
