//SCEparser.cpp										Wendy Chen, 2/2017

//Implements scene parser, as originally defined in SCEparser.py with some modifications.

#include "SCEparser.h"

#include <iostream>
#include <stdlib.h> //has exit, EXIT_FAILURE

// vector<struct Camera> cameras;
// vector<struct Light> lights;
// vector<struct Material> materials;
// vector<struct Sphere> spheres;
// vector<struct Triangle> triangles;
// vector<struct Object> objects;
// Scene s_scene;

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

// #include <sstream>
// #include <string>

// void Parse::parse(char * infile){
// 	while(std::getline(infile, line)){
// 		std::istringstream iss(line);
// 	}

// }

////////////////////

// vec3 cam_point = {.x=50.0, .y=50.0, .z=400.0};
// vec3 cam_toPoint = {.x=50.0, .y=50.0, .z=0.0};
// vec3 light_point = {.x=50.0, .y=95.5, .z=50.0};
// vec3 light_color = {.x=155.0, .y=155.0, .z=155.0};
// vec3 mat0_color = {.x=255.0, .y=255.0, .z=255.0};
// vec3 mat1_color = {.x=255.0, .y=90.0, .z=90.0};
// vec3 sphere_point = {.x=50.0, .y=25.0, .z=50.0};
// vec3 tri0_p1 = {.x=0.0, .y=0.0, .z=0.0};
// vec3 tri0_p2 = {.x=0.0, .y=0.0, .z=100.0};
// vec3 tri0_p3 = {.x=100.0, .y=0.0, .z=100.0};
// vec3 tri1_p1 = {.x=100.0, .y=0.0, .z=100.0};
// vec3 tri1_p2 = {.x=100.0, .y=0.0, .z=0.0};
// vec3 tri1_p3 = {.x=0.0, .y=0.0, .z=0.0};


// Scene* test(void){
// 	SCEscene scene = SCEscene();
// 	scene.add_camera(cam_point, cam_toPoint, 40, UP);
// 	scene.add_light(OMNI, light_point, light_color);
// 	scene.add_material(mat0_color, ORIGINAL, 0, 0.0, 0.9, 0.05, 0.0);
// 	scene.add_material(mat1_color, ORIGINAL, 0, 0.0, 0.9, 0.1, 0.0);
// 	scene.add_sphere(sphere_point, 25.0, 1);
// 	scene.add_triangle(tri0_p1, tri0_p1, tri0_p2, 0);
// 	scene.add_triangle(tri1_p1, tri1_p1, tri1_p2, 0);
// 	scene.build_scene();

// 	printf("Sanity check the scene\n");
// 	scene.print_scene();

// 	printf("Returning the scene pointer\n");
// 	return scene.get_scene();
// }












