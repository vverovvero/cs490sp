#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>

// #include "cairo.h"
// usr/local/Cellar/cairo/1.14.2_1

#include "vec3.h"

#define HEIGHT 512
#define WIDTH 512
#define PI 3.14159f

typedef struct Camera{
  vec3 point;
  float fieldOfView;
  vec3 toPoint;
  vec3 up;
} Camera;

typedef enum {OMNI, SPOT} lightType;

typedef struct Light{
  lightType type;
  vec3 point;
  vec3 color;
} Light;

typedef struct Sphere{
  vec3 point;
  float radius;
} Sphere;

typedef struct Triangle{
  vec3 point1;
  vec3 point2;
  vec3 point3;
} Triangle;

typedef enum {PHONG, ORIGINAL} materialType;

typedef struct Material{
  vec3 color;
  materialType type;
  int metal;
  float specular;
  float lambert;
  float ambient;
  float exponent;
} Material;

typedef enum {SPHERE, TRIANGLE} objectType;

typedef struct Object{
  int matIndex;
  objectType type;
  void* object;
} Object;

typedef struct Ray{
  vec3 point;
  vec3 vector;
} Ray;

//Fill in SceneStruct later
typedef struct Scene{
  Camera* camera;
  Material* materials;
  Object* objects;
  Light* lights;
  int n_lights;
  int n_objects;
  int n_materials;
} Scene;

typedef struct Dist{
  float distance;
  Object* object;
} Dist;

typedef struct Pixel{
  unsigned char A;
  unsigned char R;
  unsigned char G;
  unsigned char B;
} Pixel;
