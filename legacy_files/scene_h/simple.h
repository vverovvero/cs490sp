//simple.h

#include <vector>

using std::vector;

////////////Vector definitions///////////////
vector<struct Light> s_lights;
vector<struct Material> s_materials;
vector<struct Sphere> s_spheres;
vector<struct Triangle> s_triangles;
vector<struct Object> s_objects;

/////////////Camera///////////////////////

Camera s_camera = {
    .point = {
        .x = 50.0f,
        .y = 50.0f,
        .z = 400.0
    },
    .fieldOfView = 40.0f,
    .toPoint = {
        .x = 50.0f,
        .y = 50.0f,
        .z = 0.0f
    },
    .up = UP
};

/////////////Lights////////////////////

Light light0 = 
{
  .type = OMNI,
  .point = {
    .x = 50.0f,
    .y = 95.0f,
    .z = 50.0f
  },
  .color = {
    .x = 155.0f,
    .y = 155.0f,
    .z = 155.0f
  },
};

s_lights.push_back(light0);

/////////////////Materials/////////////////////

Material material0 = 
{
  .color =
    {
      .x = 255.0f,
      .y = 255.0f,
      .z = 255.0f,
    },
  .type = ORIGINAL,
  .metal = 0,
  .specular = 0.0f,
  .lambert = 0.9f,
  .ambient = 0.05f,
  .exponent = 0.0f,
};

Material material1 = 
{
  .color =
    {
      .x = 255.0f,
      .y = 90.0f,
      .z = 90.0f,
    },
  .type = ORIGINAL,
  .metal = 0,
  .specular = 0.0f,
  .lambert = 0.9f,
  .ambient = 0.1f,
  .exponent = 0.0f,
};

s_materials.push_back(material0);
s_materials.push_back(material1);

/////////////////Spheres///////////////////////

Sphere sphere0 =
{
  .point =
    {
      .x = 50.0f,
      .y = 25.0f,
      .z = 50.0f
      // .z = 75.0f
    },
  .radius = 25.0f,
  .matIndex = 1
};

s_spheres.push_back(sphere0);

/////////////////////Triangles///////////////////

Triangle tri0 = 
{
  .point1 =
    {
      .x = 0.0f,
      .y = 0.0f,
      .z = 0.0f,
    },
  .point2 =
    {
      .x = 0.0f,
      .y = 0.0f,
      .z = 100.0f,
    },
  .point3 =
    {
      .x = 100.0f,
      .y = 0.0f,
      .z = 100.0f,
    },
    .matIndex = 0
};

Triangle tri1 = 
{
  .point1 =
    {
      .x = 100.0f,
      .y = 0.0f,
      .z = 100.0f,
    },
  .point2 =
    {
      .x = 100.0f,
      .y = 0.0f,
      .z = 0.0f,
    },
  .point3 =
    {
      .x = 0.0f,
      .y = 0.0f,
      .z = 0.0f,
    },
    .matIndex = 0
};

s_triangles.push_back(tri0);
s_triangles.push_back(tri1);

///////////////////Objects////////////////////

Object obj0 = 
{
  .type = SPHERE,
  .matIndex = 1,
  .object = &(s_spheres[0])
};

Object obj1 = 
{
  .type = TRIANGLE,
  .matIndex = 0,
  .object = &(s_triangles[0])
};

Object obj2 = 
{
  .type = TRIANGLE,
  .matIndex = 0,
  .object = &(s_triangles[1])
};

s_objects.push_back(obj0);
s_objects.push_back(obj1);
s_objects.push_back(obj2);


///////////////Scene/////////////////

Scene s_scene = {
  .camera = &s_camera,
  .materials = s_materials,
  .objects = s_objects,
  .lights = s_lights,
  .n_lights = s_lights.size(),
  .n_materials = s_materials.size(),
  .n_objects = s_objects.size() - 1
};


