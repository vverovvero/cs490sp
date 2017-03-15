//SCEparser.cpp										Wendy Chen, 2/2017

//Implements scene parser, as originally defined in SCEparser.py with some modifications.

//Sources:
//http://stackoverflow.com/questions/1856468/how-to-output-ieee-754-format-integer-as-a-float

#include "SCEparser.h"

#include <iostream>
#include <stdlib.h> //has exit, EXIT_FAILURE
#include <errno.h>

/////////////////////
// SCEscene
////////////////////

SCEscene::SCEscene():n_spheres(0), n_triangles(0){
	this->s_scene.camera = (Camera *) malloc(sizeof(struct Camera));
}

void SCEscene::add_camera(vec3 point, vec3 toPoint, float fieldOfView, vec3 up){
	Camera* camera; 
	camera = (Camera *) malloc(sizeof(struct Camera));
	camera->point = point;
	camera->toPoint = toPoint;
	camera->fieldOfView = fieldOfView;
	camera->up = up;

	this->cameras.push_back(camera);

	printf("Added camera==========\n");
	printf("camera->point: %f, %f, %f\n", camera->point.x, camera->point.y, camera->point.z);
	printf("camera->toPoint: %f, %f, %f\n", camera->toPoint.x, camera->toPoint.y, camera->toPoint.z);
	printf("camera->fov: %f\n", camera->fieldOfView);
	printf("camera->up: %f, %f, %f\n", camera->up.x, camera->up.y, camera->up.z);

	printf("Before returning, check camera in scene.cameras vector==========\n");
	printf("camera->point: %f, %f, %f\n", this->cameras[0]->point.x, this->cameras[0]->point.y, this->cameras[0]->point.z);
	printf("camera->toPoint: %f, %f, %f\n", this->cameras[0]->toPoint.x, this->cameras[0]->toPoint.y, this->cameras[0]->toPoint.z);
	printf("camera->fov: %f\n", this->cameras[0]->fieldOfView);
	printf("camera->up: %f, %f, %f\n", this->cameras[0]->up.x, this->cameras[0]->up.y, this->cameras[0]->up.z);


}

void SCEscene::add_light(lightType type, vec3 point, vec3 color){
	Light* light;
	light = (Light *) malloc(sizeof(struct Light));
	light->type = type;
	light->point = point;
	light->color = color;

	this->lights.push_back(*light);
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
	obj->object = (Sphere*) malloc(sizeof(struct Sphere)); //necessary?
	obj->object = this->spheres[this->n_spheres - 1];

	this->objects.push_back(*obj);
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
	obj->object = (Triangle*) malloc(sizeof(struct Triangle)); //necessary?
	obj->object = this->triangles[this->n_triangles - 1];

	this->objects.push_back(*obj);
}

void SCEscene::build_scene(){
	printf("BUILDING SCENE~~~~~~~~~\n");
	this->s_scene.camera = (Camera *) malloc(sizeof(struct Camera));
	this->s_scene.camera = this->cameras[0];
	printf("here1\n");
	this->s_scene.materials = this->materials;
	printf("here2\n");
	this->s_scene.objects = this->objects;
	printf("here3\n");
	this->s_scene.lights = this->lights;
	printf("here4\n");
	this->s_scene.n_lights = this->lights.size();
	printf("here5, light size: %lu\n", this->lights.size());
	this->s_scene.n_materials = this->materials.size();
	printf("here6\n");
	this->s_scene.n_objects = this->objects.size();
	printf("here7\n");

	//Check attribute here
	// printf("Building scene, check camera in scene.cameras vector==========\n");
	// printf("camera->point: %f, %f, %f\n", this->cameras[0]->point.x, this->cameras[0]->point.y, this->cameras[0]->point.z);
	// printf("camera->toPoint: %f, %f, %f\n", this->cameras[0]->toPoint.x, this->cameras[0]->toPoint.y, this->cameras[0]->toPoint.z);
	// printf("camera->fov: %f\n", this->cameras[0]->fieldOfView);
	// printf("camera->up: %f, %f, %f\n", this->cameras[0]->up.x, this->cameras[0]->up.y, this->cameras[0]->up.z);

}

Scene* SCEscene::get_scene(){
	return &(this->s_scene);
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
			printf("Parse camera attr: %f\n", a[i]);
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
		printf("Parse light type: %d\n", swapped);
	}
	//Get float args for light
	for(i=0;i<6;i++){
		if((retval = fread(&value, 4, 1, f)) > 0){
			swapped = swapEndian(value);
			a[i] = IEEEInttoFloat(swapped);
			printf("Parse light attr: %f\n", a[i]);
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
			printf("Parse material color attr: %f\n", c[i]);
		}
	}
	//Get material type
	if((retval = fread(&value, 4, 1, f)) > 0){
		swapped = swapEndian(value);
		m_type = this->toMaterialType[swapped];
		printf("Parse material type: %d\n", swapped);
	}
	//Get metal bool
	if((retval = fread(&value, 4, 1, f)) > 0){
		swapped = swapEndian(value);
		metal_bool = swapped;
		printf("Parse material metal bool: %d\n", swapped);
	}
	//Get remaining attrs
	for(i=0;i<4;i++){
		if((retval = fread(&value, 4, 1, f)) > 0){
			swapped = swapEndian(value);
			a[i] = IEEEInttoFloat(swapped);
			printf("Parse material attr: %f\n", a[i]);
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
			printf("Parse sphere attr: %f\n", a[i]);
		}
	}
	//Get sphere matIndex
	if((retval = fread(&value, 4, 1, f)) > 0){
		swapped = swapEndian(value);
		matIndex = swapped;
		printf("Parse matIndex: %d\n", swapped);
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
			printf("Parse tri attr: %f\n", a[i]);
		}
	}
	//Get matIndex
	if((retval = fread(&value, 4, 1, f)) > 0){
		swapped = swapEndian(value);
		matIndex = swapped;
		printf("Parse matIndex: %d\n", swapped);
	}
	//Make vec3
	vec3 point1={.x=a[0], .y=a[1], .z=a[2]};
	vec3 point2={.x=a[3], .y=a[4], .z=a[5]};
	vec3 point3={.x=a[6], .y=a[7], .z=a[8]};
	//Add triangle
	(*scene).add_triangle(point1, point2, point3, matIndex);

}

void Parse::parseSCE(char * infile, SCEscene *scene){
	FILE *f = fopen(infile, "rb");
	if(f == NULL){
		fprintf(stderr, "Unable to open input file\n");
		return;
	}

	int cmd;
	int retval;
	while(( retval = fread(&cmd, 4, 1, f)) > 0){
		
		// Flip to correct for byte order
		int swapped = swapEndian(cmd);
        printf("~~~~~~~~~~~~~~~~~~~~~~~~\n");
		printf("0x%08x\n", swapped);
		printf("%f\n", (float)swapped);
		printf("%d\n", swapped);

		// printf("++Float is: %f\n", IEEEInttoFloat(swapped));

		switch (swapped){
			case 0:
				printf("Adding Camera\n");
				Parse::camera(f, scene);
				break;
			case 1:
				printf("Adding Light\n");
				Parse::light(f, scene);
				break;
			case 2:
				printf("Adding Material\n");
				Parse::material(f, scene);
				break;
			case 3:
				printf("Adding Sphere\n");
				Parse::sphere(f, scene);
				break;
			case 4:
				printf("Adding Triangle\n");
				Parse::triangle(f, scene);
				break;
			case 5:
				printf("Reached end of binary file.\n");
				break;
			default:
				printf("Invalid cmd\n");
		}		
	}
	fclose(f);
}
