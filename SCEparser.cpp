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

SCEscene::SCEscene():n_spheres(0), n_triangles(0){}

void SCEscene::add_camera(vec3 point, vec3 toPoint, float fieldOfView, vec3 up){
	Camera* camera; 
	camera = (Camera *) malloc(sizeof(struct Camera));
	camera->point = point;
	camera->toPoint = toPoint;
	camera->fieldOfView = fieldOfView;
	camera->up = up;

	this->cameras.push_back(*camera);
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
	obj->object = this->triangles[this->n_triangles - 1];

	this->objects.push_back(*obj);
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


void Parse::parseSCE(char * infile, SCEscene scene){
	FILE *f = fopen(infile, "rb");
	if(f == NULL){
		fprintf(stderr, "Unable to open input file\n");
		return;
	}

	int cmd;


	printf("size of int: %lu\n", sizeof(int));
	printf("size of float: %lu\n", sizeof(float));
	printf("size of pointer: %lu\n", sizeof(float*));

	//check file size
	fseek (f , 0 , SEEK_END);
  	long lSize = ftell (f);
  	rewind (f);

  	float test_float = 50.0f;
  	printf("On this system, 50.0f is rep as: 0x%08x\n", test_float);


  	printf("size of file: %lu\n", lSize);

	int retval;
	while(( retval = fread(&cmd, 4, 1, f)) > 0){
		
		// Flip to correct for byte order
		int swapped = ((cmd>>24)&0xff) | // move byte 3 to byte 0
                    ((cmd<<8)&0xff0000) | // move byte 1 to byte 2
                    ((cmd>>8)&0xff00) | // move byte 2 to byte 1
                    ((cmd<<24)&0xff000000); // byte 0 to byte 3
        printf("~~~~~~~~~~~~~~~~~~~~~~~~\n");
		printf("0x%08x\n", swapped);
		printf("%f\n", (float)swapped);
		printf("%d\n", swapped);

		printf("++Float is: %f\n", IEEEInttoFloat(swapped));

		switch (swapped){
			case 0:
				printf("Adding Camera\n");
				// for (int i=0; i<10; i++) {
				// 	fread(&cmd, 4, 1, f);
				// 	printf("float %d \n", cmd);
				// }
				fseek(f, 40, SEEK_CUR);
				break;
			case 1:
				printf("Adding Light\n");
				fseek(f, 28, SEEK_CUR);
				break;
			case 2:
				printf("Adding Material\n");
				fseek(f, 36, SEEK_CUR);
				break;
			case 3:
				printf("Adding Sphere\n");
				fseek(f, 20, SEEK_CUR);
				break;
			case 4:
				printf("Adding Triangle\n");
				fseek(f, 40, SEEK_CUR);
				break;
			case 5:
				printf("Found END\n");
				break;
			default:
				printf("Invalid cmd\n");
		}

		// printf("cmd = %d\n", cmd);

		if(cmd == 5){
			break;
		}
		else{
			continue;
		}
		
	}

	fclose(f);
}
