//vec3.h                                                    Wendy Chen, 2017
//Sources:

#ifndef VEC3_H
#define VEC3_H

#include <math.h>

typedef struct vec3{
  float x;
  float y;
  float z;
} vec3;

extern vec3 UP;
extern vec3 ZERO;
extern vec3 WHITE;

static inline float dotProduct(vec3 a, vec3 b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

static inline float length(vec3 a) {
  return sqrtf(dotProduct(a, a));
}

static inline vec3 crossProduct(vec3 a, vec3 b) {
  vec3 ret_vec = {
    .x = (a.y * b.z) - (a.z * b.y),
    .y = (a.z * b.x) - (a.x * b.z),
    .z = (a.x * b.y) - (a.y * b.x)
  };
  return ret_vec;
}

static inline vec3 scale(vec3 a, float t) {
  vec3 ret_vec =  {
    .x = a.x * t,
    .y = a.y * t,
    .z = a.z * t
  };
  return ret_vec;
}

static inline vec3 compScale(vec3 a, vec3 b) {
  vec3 ret_vec =  {
    .x = a.x * b.x,
    .y = a.y * b.y,
    .z = a.z * b.z
   };
   return ret_vec;
}

static inline vec3 unitVector(vec3 a) {
  return scale(a, 1.0f / length(a));
}

static inline vec3 add(vec3 a, vec3 b) {
  vec3 ret_vec =  {
      .x = a.x + b.x,
      .y = a.y + b.y,
      .z = a.z + b.z
  };
  return ret_vec;
}

static inline vec3 add3(vec3 a, vec3 b, vec3 c) {
  vec3 ret_vec =  {
      .x = a.x + b.x + c.x,
      .y = a.y + b.y + c.y,
      .z = a.z + b.z + c.z
  };
  return ret_vec;
}

static inline vec3 subtract(vec3 a, vec3 b) {
  vec3 ret_vec =  {
      .x = a.x - b.x,
      .y = a.y - b.y,
      .z = a.z - b.z
  };
  return ret_vec;
}

static inline vec3 reflectThrough(vec3 a, vec3 normal) {
    vec3 d = scale(normal, dotProduct(a, normal));
    return subtract(scale(d, 2.0f), a);
}


//may need
static inline float clip(float n, float lower, float upper) {
  return fmax(lower, fmin(n, upper));
}

//consider:
//https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
//return refracted ray, or zero for no refraction
static inline vec3 refractThrough(vec3 I, vec3 N, float ior){
  float cosi = clip(dotProduct(I, N), -1, 1);
  float etai = 1;
  float etat = ior;
  float temp;
  vec3 n = N;
  if(cosi < 0){
    cosi = -cosi;
  } else{
    //swap
    etai = temp;
    etai = etat;
    etat = temp;
    n = scale(N, -1);
  }
  float eta = etai/etat;
  float k = 1 - eta * eta * (1 - cosi * cosi);
  return k < 0 ? ZERO : add(scale(I, eta), scale(n, (eta * cosi - sqrtf(k))));
}


// static inline vec3 refractThrough(vec3 V, vec3 N, float nr){
//   float discriminant = 1 - pow(nr, 2) * (1 - pow(dotProduct(N, V), 2));
//   if(discriminant < 0){
//     return ZERO;
//   }
//   else{
//     float scalar = (nr*dotProduct(N, V) - sqrtf(discriminant));
//     vec3 first_term = scale(N, scalar);
//     vec3 T = subtract(first_term, scale(V, nr));
//     return T;
//   }
// }
#endif
