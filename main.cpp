//main.cpp                                                      Wendy Chen, 2017
/***************************** Compilation notes *******************************/
// Use provided Makefile
// ***OR****
// Install cairo on MacOSX:  brew install cairo
// Installs to directory: /usr/local/Cellar/cairo/1.14.2_1
// Compile in C++ on MacOSX
// clang++ -I/usr/local/include/cairo/ -L/usr/local/lib -o main main.cpp -lcairo
// After moving into src:
// clang++ -I/usr/local/include/cairo/ -L/usr/local/lib -o main src/main.cpp -lcairo
// Source: http://askubuntu.com/questions/706512/cant-access-cairo-h


// For c compilation on OSX: (Change file extension to c)
// gcc -std=c99 -o main -L/usr/local/lib/cairo/ -lcairo main.c -I/usr/local/include/cairo/
// For cpp compilation on OSX:
// clang++ -std=c++11 -stdlib=libc++ -o main -L/usr/local/lib/cairo/ -lcairo main.cpp -I/usr/local/include/cairo/

//For debugging with gdb
// clang++ -std=c++11 -stdlib=libc++ -g -o main -L/usr/local/lib/cairo/ -lcairo main.cpp -I/usr/local/include/cairo/

/********************************** Sources **************************************/
//Serial Timing:
//http://stackoverflow.com/questions/459691/best-timing-method-in-c
//Threaded timing:
//http://stackoverflow.com/questions/2962785/c-using-clock-to-measure-time-in-multi-threaded-programs
//http://stackoverflow.com/questions/5167269/clock-gettime-alternative-in-mac-os-x
//https://gist.github.com/jbenet/1087739
//Determining if a ray has intersected an object in the scene.
//http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
//https://blog.frogslayer.com/kd-trees-for-faster-ray-tracing-with-triangles/
/***************************** Headers and Structs *******************************/

#include "main.h"
#include "SCEparser.h"

#include <stack>
#include <thread>

using namespace std;

//Default Global Vars settings
static const int NUM_THREADS = 8;
bool LOW_SAMPLE_FLAG = true;

//determined from user input sce
int HEIGHT;
int WIDTH;

//calculated from HEIGHT, WIDTH, and NUM_THREADS
int HEIGHT_CHUNK;
int IMG_CHUNK_SIZE;

/********************************** Timing ***************************************/

#include <time.h>
#include <sys/time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif


void current_utc_time(struct timespec *ts) {

#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, ts);
#endif

}

/***************************** Forward declarations *******************************/

vec3 objectNormal(Object* object, vec3 point);
float sphereIntersection(Sphere* sphere, Ray* ray);
vec3 sphereNormal(Sphere* sphere, vec3 pos);
vec3 triNormal(Triangle* tri);
float triIntersection(Triangle* tri, Ray* ray);
Dist intersectObjectList(Ray* ray, vector<struct Object*>* shortList);
float intersectBox(Ray* ray, KDnode* node);
void intersectKD(Ray *ray, KDnode* node, vector<struct Object*>* shortList);
Dist intersectSceneAccel(Ray *ray, KDnode* node);
Visible isLightVisible(vec3 point, Scene* scene, KDtree* tree, Light light);
vec3 surface(Ray* ray, Scene* scene, KDtree* tree, Object* object, vec3 pointAtTime, vec3 normal, float *contribution);
vec3 trace(Ray* ray, Scene* scene, KDtree* tree, vec3 *pointAtTime, float *contribution);
void renderThreaded(Scene* scene, KDtree* tree, float* img, int lo_index, int hi_index, bool low_sample_flag);
void stitch(float *img, float *chunk, int width, int lo_index, int chunk_size);


/***************************** Path tracing functions *******************************/
vec3 sphereNormal(Sphere* sphere, vec3 pos){
  return unitVector(subtract(pos, sphere->point));
}

vec3 triNormal(Triangle* tri) {
    return unitVector(
        crossProduct(
          subtract(tri->point2, tri->point1),
          subtract(tri->point3, tri->point1))
         );
}

vec3 objectNormal(Object* object, vec3 point){
  if (object->type == SPHERE){
    return sphereNormal((Sphere*)object->object, point);
  }
  else if (object->type == TRIANGLE) {
    return (triNormal((Triangle*)object->object));
  }
  else{
    fprintf(stderr, "objectNormal broke\n");
    return ZERO;
  }
}

float sphereIntersection(Sphere* sphere, Ray* ray){
  vec3 eye_to_center = subtract(sphere->point, ray->point);
  float v = dotProduct(eye_to_center, ray->vector);
  float eoDot = dotProduct(eye_to_center, eye_to_center);
  float discriminant = (sphere->radius * sphere->radius) - eoDot + (v * v);

  if ((fabsf(length(eye_to_center)-sphere->radius) / sphere->radius) < .001f) {
    return -1.0f;
  }
  // Negative discriminant means no hit
  if (discriminant < 0.0f) {
      return -1.0f;
  } else {
      // otherwise, return distance from camera point to sphere
      // `Math.sqrt(dotProduct(a, a))` is the length of a vector, so
      // `v - Math.sqrt(discriminant)` means the length of the the vector
      // just from the camera to the intersection point.
      return v - sqrtf(discriminant);
  }
}

float triIntersection(Triangle* tri, Ray* ray) {
    // compute triangle normal and d in plane equation
    vec3 triNorm = triNormal(tri);
    float d = -1.0f * dotProduct(tri->point1, triNorm);

    // compute where ray intersects plane
    float dist = -1.0f * (dotProduct(ray->point, triNorm) + d) / dotProduct(ray->vector, triNorm);

    // if behind the ray starting point, we are done -- no intersection
    if (dist < 0.001f) {
      return -1.0f;
    }

    vec3 P = add(ray->point, scale(ray->vector, dist));

    // do inside test, edge by edge on triangle
    vec3 v1 = subtract(tri->point1, ray->point);
    vec3 v2 = subtract(tri->point2, ray->point);

    vec3 n1 = unitVector(crossProduct(v2, v1));
    float d1 = -1.0f * dotProduct(ray->point, n1);
    if((d1 + dotProduct(P, n1)) < 0.0f) {
      return -1.0f;
    }

    vec3 v3 = subtract(tri->point3, ray->point);
    n1 = unitVector(crossProduct(v3, v2));
    d1 = -1.0f * dotProduct(ray->point, n1);
    if((d1 + dotProduct(P, n1)) < 0.0f) {
      return -1.0f;
    }

    n1 = unitVector(crossProduct(v1, v3));
    d1 = -1.0f * dotProduct(ray->point, n1);
    if((d1 + dotProduct(P, n1)) < 0.0f) {
      return -1.0f;
    }

    return dist;
}

//search for intersection between ray and object on shortened list
Dist intersectObjectList(Ray* ray, vector<struct Object*>* shortList) {
  Dist closest = { .distance = INFINITY, .object = NULL};
  // Find closest intersecting object
  int i;
  int size = (*shortList).size();
  for (i = 0; i < size; i++) {

    Object* object = (*shortList)[i];

    float dist = -1.0f;

    if (object->type == SPHERE)  {
      dist = sphereIntersection((Sphere*)object->object, ray);
    }

    if (object->type == TRIANGLE) {
      dist = triIntersection ((Triangle*)object->object, ray);
    }

    if (dist > .0f && dist < closest.distance) {
      closest.distance = dist;
      closest.object = object;
    }
  }

  if (closest.distance == INFINITY) {
    closest.distance = -1.0f;
  }
  return closest;
}
//intersectBox, adapted from scratchapixel, the non-optimized code
//return infinity or tmin
float intersectBox(Ray* ray, KDnode* node){
  //Get values from node
  vec3 nodeMin = (*node).getMin();
  vec3 nodeMax = (*node).getMax();

  float tmin, tmax, old_tmin, old_tmax;
  float tymin, tymax, old_tymin, old_tymax;
  float tzmin, tzmax, old_tzmin, old_tzmax;

  tmin = (nodeMin.x - ray->point.x)/ray->vector.x;
  tmax = (nodeMax.x - ray->point.x)/ray->vector.x;

  if(tmin > tmax){
    old_tmin = tmin;
    old_tmax = tmax;
    tmin = old_tmax;
    tmax = old_tmin;
  }

  tymin = (nodeMin.y - ray->point.y)/ray->vector.y;
  tymax = (nodeMax.y - ray->point.y)/ray->vector.y;

  if(tymin > tymax){
    old_tymin = tymin;
    old_tymax = tymax;
    tymin = old_tymax;
    tymax = old_tymin;
  }

  //check for misses
  if((tmin > tymax) || (tymin > tmax)){
    return INFINITY;
  }

  //pick a min and max
  if(tymin > tmin){
    tmin = tymin;
  }
  if(tymax < tmax){
    tmax = tymax;
  }

  //factor in z
  tzmin = (nodeMin.z - ray->point.z)/ray->vector.z;
  tzmax = (nodeMax.z - ray->point.z)/ray->vector.z;

  if(tzmin > tzmax){
    old_tzmin = tzmin;
    old_tzmax = tzmax;
    tzmin = old_tzmax;
    tzmax = old_tzmin;
  }

  if((tmin > tzmax) || (tzmin > tmax)){
    return INFINITY;
  }

  if(tzmin > tmin){
    tmin = tzmin;
  }

  if(tzmax < tmax){
    tmax = tzmax;
  }

  return tmin;
}

//intersectKD, return a list of objects
void intersectKD(Ray *ray, KDnode* node, vector<struct Object*>* shortList){
  stack<KDnode*> nodeStack;
  nodeStack.push(node);

  while(!nodeStack.empty()){
    KDnode* current = nodeStack.top();
    nodeStack.pop();

    KDnode* left = current->getLeft();
    KDnode* right = current->getRight();

    //check if current is a child
    //if it is child, check if there is an intersection, and add objects to shortList
    if(left == NULL && right == NULL){
      if(intersectBox(ray, current) != INFINITY){
        int i;
        vector<struct Object*>* objects = current->getObjects();
        int size = objects->size();
        for(i=0; i<size; i++){
          (*shortList).push_back((*objects)[i]);
        }
      }
    }

    //check if ray intersects current box
    //if it does, push the children onto the stack
    if(intersectBox(ray, current) != INFINITY){
      if(left != NULL){
        nodeStack.push(left);
      }
      if(right != NULL){
        nodeStack.push(right);
      }
    }
  }
}

//intersectScene
Dist intersectSceneAccel(Ray *ray, KDnode* node){
  vector<struct Object*> shortList;
  //fill the short list
  intersectKD(ray, node, &shortList);
  //call intersectObjectList
  return intersectObjectList(ray, &shortList);
}

Visible isLightVisible(vec3 point, Scene* scene, KDtree* tree, Light light) {
  vec3 lightPoint = light.point;
  vec3 full_vector = subtract(lightPoint,point);
  vec3 vector = unitVector(subtract(lightPoint, point));
  Ray ray = { .point = point, .vector = vector};

  Dist distObject = intersectSceneAccel(&ray, (*tree).get_kdtree());
  Visible vis;

  //get visibility from light's origin point, same for all lights
  if (distObject.distance > 0.0f && distObject.distance < (length(full_vector) -.005)) {
    vis.isVisible = 0; // False case where an object is found between the point and the light
  }
  else {
    vis.isVisible = 1; // Either negative (no collision at all) or >length(vector) (light is closer than collision)
  }

  if(light.type == OMNI){
    vis.intensity = 1.0;
    vis.lightPoint = light.point;
    return vis;
  }

  //if spot, recalculate visibility? scale the intensity
  else if(light.type == SPOT){
    vec3 toPoint = light.toPoint;
    vec3 spotVector = subtract(lightPoint, toPoint);

    float A = dotProduct(spotVector, full_vector);
    float B = length(spotVector) * length(full_vector);
    float C = A/B;
    float angle = (180 * acos(C))/PI;
    float angle_radians = acos(C);

    vis.isVisible = (vis.isVisible && (angle <= light.angle));

    //set intensity by scaling if beyond falloff (from PBRT p. 611 - 614)
    if(angle > light.fallOffAngle){
      float light_angle_radians = light.angle * PI/180;
      float light_fallOffAngle_radians = light.fallOffAngle * PI/180;
      float delta = (cos(angle_radians) - cos(light_angle_radians))/(cos(light_fallOffAngle_radians) - cos(light_angle_radians));
      vis.intensity = delta * delta *delta * delta;
    }
    else{
      vis.intensity = 1.0;
    }
    vis.lightPoint = light.point;
    return vis;
  }

  //if spherical, generate light sample by ray-sphere intersection
  else if(light.type == SPHERICAL){
    //sample a point on the sphere's surface
    float u1 = drand48();
    float u2 = drand48();
    float xy_term = 2.0 * sqrtf(u1 * (1-u1));
    float x = cos(2.0*PI*u2)*xy_term;
    float y = sin(2.0*PI*u2)*xy_term;
    float z = 1.0 - 2.0*u1;
    vec3 sphereSample = {.x=x, .y=y, .z=z};
    vis.lightPoint = add(scale(sphereSample, light.radius), light.point);

    //calculate dist object from constructed array
    full_vector = subtract(vis.lightPoint, point);
    ray = { .point = point, .vector = unitVector(full_vector)};
    distObject = intersectSceneAccel(&ray, (*tree).get_kdtree());
    
    //set visibility
    if (distObject.distance > 0.0f && distObject.distance < (length(full_vector) -.005)) {
      vis.isVisible = 0; // False case where an object is found between the point and the light
    }
    else {
      vis.isVisible = 1; // Either negative (no collision at all) or >length(vector) (light is closer than collision)
    }

    //set intensity
    vis.intensity = 1.0;

    return vis;
  }

  else{
    fprintf(stderr, "Unimplemented light type!\n");
    return vis;
  }
  
}

vec3 surface(Ray* ray, Scene* scene, KDtree* tree, Object* object, vec3 pointAtTime, vec3 normal, float* contribution) {
  Material* material = &(scene->materials[object->matIndex]);
  vec3 objColor = material->color;
  vec3 c = ZERO;
  vec3 lambertAmount = ZERO;
  vec3 specReflect = ZERO;
  Visible vis;

  //check glass type first and return

  // lambert shading
  if (material->lambert > 0.0f) {
    for (int i = 0; i < scene->n_lights; i++) {
      //pass in light and calculate Visible struct
      vis = isLightVisible(pointAtTime, scene, tree, scene->lights[i]);
      if (vis.isVisible){
        // lambertian reflectance
        *contribution = dotProduct(unitVector(subtract(vis.lightPoint, pointAtTime)), normal);
        //scale the contribution by vis intensity
        *contribution = (*contribution) * vis.intensity;

        if(*contribution > 0.0f) {
          lambertAmount = add(lambertAmount, scale(scene->lights[i].color, *contribution));
        }

      }
    }
  }
  //adjust lit color by object color and divide by 255 since light color is 0 to 255
  lambertAmount = compScale(lambertAmount, objColor);
  lambertAmount = scale(lambertAmount, material->lambert);
  lambertAmount = scale(lambertAmount, 1./255.);

  //Specular pass
  if(material->specular > 0.0f){
    Ray reflectedRay = {.point=pointAtTime, .vector=reflectThrough(scale(ray->vector, -1), normal)};
    vec3 reflectedColor = trace(&reflectedRay, scene, tree, &pointAtTime, contribution);
    if(length(reflectedColor) > 0.0f){
      c = add(c, scale(reflectedColor, material->specular));
    }

    //Phong/metal pass
    if(material->type == PHONG){
      for(int i=0; i<scene->n_lights; i++){
        vis = isLightVisible(pointAtTime, scene, tree, scene->lights[i]);
        if(vis.isVisible){
          vec3 eye = scene->camera->point;
          vec3 vectorV = unitVector(subtract(pointAtTime, eye));
          vec3 vectorL = unitVector(subtract(pointAtTime, vis.lightPoint));
          vec3 vectorN = unitVector(normal);
          vec3 vectorH = unitVector(add(vectorV, vectorL));

          float dotHN = dotProduct(vectorH, vectorN);
          float dotHNX = pow(dotHN, material->exponent);
          float scalar = material->specular * dotHNX;
          vec3 vectorC;
          vec3 irradiance;

          //update c, based off metal
          if(material->metal == 0){
            vectorC = scene->lights[i].color;
            irradiance = scale(vectorC, scalar);
            c = add(c, irradiance);
          }
          else{
            vectorC = material->color;
            irradiance = scale(vectorC, scalar);
            c = add(c, irradiance);
          }

        }
      }
    }
  }

  //Ambient pass
  return add3(c, lambertAmount, scale(objColor, material->ambient));
}

//need to keep track of path_length and what bounce we are currently on
//no!  don't want to recalculate.  store throughput
vec3 trace(Ray* ray, Scene* scene, KDtree* tree, vec3 *pointAtTime, float* contribution) {
  Dist distObject = intersectSceneAccel(ray, (*tree).get_kdtree());

  // If we don't hit anything, fill this pixel with the background color -
  if (distObject.distance < 0.0f) {
      return ZERO;
  }

  float dist = distObject.distance;
  Object* object = distObject.object;

  // The `pointAtTime` is another way of saying the 'intersection point'
  // of this ray into this object. We compute this by simply taking
  // the direction of the ray and making it as long as the distance
  // returned by the intersection check.
  *pointAtTime = add(ray->point, scale(ray->vector, dist));

  return surface(ray, scene, tree, object, *pointAtTime, objectNormal(object, *pointAtTime), contribution);
}

/***************************** Threaded Render *******************************/
void renderThreaded(Scene* scene, KDtree* tree, float* img, int lo_index, int hi_index, bool LOW_SAMPLE_FLAG){
  printf("renderThreaded, from lo_index %d to hi_index %d\n", lo_index, hi_index);

  Camera* camera = scene->camera;

  vec3 eyeVector = unitVector(subtract(camera->toPoint, camera->point));
  vec3 vpRight = unitVector(crossProduct(eyeVector, camera->up));
  vec3 vpUp = unitVector(crossProduct(vpRight, eyeVector));

  float height = (float) HEIGHT;
  float width = (float) WIDTH;

  float fovRadians = PI * (camera->fieldOfView / 2.0f) / 180.0f;
  float heightWidthRatio = height / width;
  float halfWidth = tanf(fovRadians);
  float halfHeight = heightWidthRatio * halfWidth;
  float camerawidth = halfWidth * 2.0f;
  float cameraheight = halfHeight * 2.0f;
  float pixelWidth = camerawidth / (width - 1.0f);
  float pixelHeight = cameraheight / (height - 1.0f);

  Ray ray;
  vec3 color;

  for (int x = 0; x < WIDTH; x++) {
    printf("Rendering x row: %d\n", x);
    //reset img_y counter
    int img_y = 0;
    for (int y = lo_index; y < hi_index; img_y++, y++) {
        color = ZERO;
        //antialiasing done by computing 25 stratified samples per pixel
        for (float s = -.4f; s < .6f; s+=.2f) {
          for (float r = -.4f; r < .6; r +=.2f) {

            if(LOW_SAMPLE_FLAG){
              s = 0.0;
              r = 0.0;
            }

            //Depth of field calculation
            if(camera->lensRadius > 0.0){
              //generate random sample, 
              float disk_u1 = drand48();
              float disk_u2 = drand48();
              float uniform_r = sqrt(disk_u1);
              float uniform_theta = 2.0*PI*disk_u2;
              //transform polar to cartesian
              float lensU = uniform_r*cos(uniform_theta);
              float lensV = uniform_r*sin(uniform_theta);
              //scale by lens radius
              lensU *= camera->lensRadius;
              lensV *= camera->lensRadius;
              //get disk sample
              vec3 uniform_diskSample = {
                .x = lensU,
                .y = lensV,
                .z = 0.0
              };
              //Update ray.point with lens sample
              ray.point = add(camera->point, uniform_diskSample);
              //Set standard ray direction
              vec3 xcomp = scale(vpRight, ((x+s) * pixelWidth) - halfWidth);
              vec3 ycomp = scale(vpUp, ((y+r) * pixelHeight) - halfHeight);
              ray.vector = unitVector(add3(eyeVector, xcomp, ycomp));
              //Compute point on plane of focus
              float ft = fabs(camera->focalDepth - camera->point.z)/ray.vector.z;
              vec3 Pfocus = subtract(camera->point, scale(ray.vector, ft));
              //update ray.vector
              ray.vector = unitVector(subtract(Pfocus, ray.point));
            }
            //if lens is pinhole, then standard ray.point and direction calculation
            else{
              ray.point = camera->point;
              vec3 xcomp = scale(vpRight, ((x+s) * pixelWidth) - halfWidth);
              vec3 ycomp = scale(vpUp, ((y+r) * pixelHeight) - halfHeight);
              ray.vector = unitVector(add3(eyeVector, xcomp, ycomp));
            }

            //Direct lighting
            vec3 pointAtTime = ray.point; //first intersection point is ray origin
            float contribution = 0.0; //we assign roulette value as contribution
            float throughput = 1.0;
            color = add(color, trace(&ray, scene, tree, &pointAtTime, &contribution));

            // color = ZERO;//for Indirect test only
            //////////////Indirect lighting/////////////////////
            vec3 indirect_color = ZERO;
            vec3 indirect_roulette = ZERO;
            vec3 indirect_direction = ZERO;
            int num_samples = 0.0;
            float u1, u2;
            float Russian_Roulette = contribution; //calculate based on reflectance value
            // Ray indirect_ray;
            //While random chance > termination probability, shoot more rays
            while((num_samples < 3) || (drand48() < Russian_Roulette)){
              //Construct indirect ray
              ray.point = pointAtTime;
              //Monte carlo sample the new direction:
              u1 = drand48();
              u2 = drand48();
              indirect_direction.x = ray.point.x + cos(2*PI*u2) * sqrt(1-u1*u1);
              indirect_direction.y = ray.point.y + sin(2*PI*u2) * sqrt(1-u1*u1);
              indirect_direction.z = ray.point.z + u1;
              //Get new ray's direction
              ray.vector = unitVector(subtract(indirect_direction, ray.point)); 
              //what is current throughput?
              throughput = (1.0/PI) * throughput; //throughput is based off of diffuse reflectance              
              //Scale the first two bounces by throughput
              if(num_samples < 3){
                indirect_color = add(indirect_color, scale(trace(&ray, scene, tree, &pointAtTime, &contribution), throughput));
              }
              //don't scale roulette, due to averaging later
              else{
                indirect_roulette = add(indirect_roulette, trace(&ray, scene, tree, &pointAtTime, &contribution));
              }
              num_samples++;

              //Calculate roulette value for next go
              Russian_Roulette = contribution;
            }
            if(num_samples > 2){
              float indirect_scale = 1.0/(num_samples-2.0);
              indirect_roulette = scale(indirect_roulette, indirect_scale);
              //Total color is:
              color = add(color, indirect_color); //direct plus 2 bounces
              color = add(color, indirect_roulette); //for roulette passes
            }
            ////////////////////////////////////////////
            //break out early if low sample
            if(LOW_SAMPLE_FLAG){
              break;
            }
          }
          if(LOW_SAMPLE_FLAG){
            break;
          }
        }
        //Write final color to image buffer
        color = scale(color, 0.04);
        int index = (x * 4) + (img_y * width * 4);
        img[index + 0] = 0.0f;
        img[index + 1] = color.x;
        img[index + 2] = color.y;
        img[index + 3] = color.z;
    }
  }
}

//for stitching in an image chunk
void stitch(float *img, float *chunk, int width, int lo_index, int chunk_size){
  int img_index = width * lo_index * 4;
  for(int i=0; i<chunk_size; i++, img_index++){
    img[img_index] = chunk[i];
  }

}

/***************************** Tone map *******************************/

void tone_map(float* img, int size){

  float max = img[0];

  for (int i=0; i<size; i++){
    if (img[i] > max){
      max = img[i];
    }
  }

  for (int i=0; i<size; i++){
    img[i] = img[i] / max * 255.0f;
  }

}

/***************************** Main loop *******************************/
int main (int argc, char *argv[]){

  char infile[50], outfile[50];

  //Argument sanitization
  if((argc != 5) && (argc != 3)){
    fprintf(stderr, "Usage: ./main -r low <.bin file> <.png name>\n");
    return -1;
  }

  //fetch arguments
  int c;
  while(optind < argc){
    if((c = getopt(argc, argv, "r:")) != -1){
      switch (c) {
        printf("Resolution: %s\n", optarg);
        case 'r':
          if(strcmp(optarg, "low") == 0){
            LOW_SAMPLE_FLAG = true;
          }
          else if (strcmp(optarg, "high") == 0){
            LOW_SAMPLE_FLAG = false;
          }
          else {
            fprintf(stderr, "r flag only accepts low (1 sample per pixel) or high (25 sample per pixel)\n");
            return 01;
          }
          break;
        default:
          break;
      }
    }
    else{
      while(optind < argc){
        //handle regular arguments
        strcpy(infile, argv[optind]);
        printf("infile: %s\n", infile);
        optind++;
        strcpy(outfile, argv[optind]);
        printf("outfile: %s\n", outfile);
        optind++;
      }
      break;
    }
  }

  //Confirm num threads is okay
  if(HEIGHT % NUM_THREADS != 0){
    fprintf(stderr, "Image pixel height not divisible by number of threads.\n");
    return -1;
  }

  //begin timing
  struct timespec start, end;
  current_utc_time(&start);

  // Initialize SCEscene and Parser
  SCEscene scene = SCEscene();
  Parse parse = Parse();

  //Call parser
  if(parse.parseSCE(infile, &scene) != 0){
    fprintf(stderr, "Parsing failed\n");
    return -1;
  }

  // Build the scene
  scene.build_scene();

  printf("Sanity check the scene\n");
  scene.print_scene();

  //load scene via a function
  Scene * s_scene_ptr;
  s_scene_ptr = scene.get_scene();

  //set film dimensions from user sce
  HEIGHT = s_scene_ptr->film->height;
  WIDTH = s_scene_ptr->film->width;

  //build kd tree
  KDtree kdtree = KDtree(s_scene_ptr->box->min, s_scene_ptr->box->max, &scene);
  KDnode* root = kdtree.get_kdtree();

  //initalize random number generator
  srand48(time(NULL));

  /////////THREADED RENDER////////
  //render each image chunk, tone map each chunk
  printf("Starting threaded render!  NUM_THREADS: %d\n", NUM_THREADS);

  thread t[NUM_THREADS];
  float *img_chunk[NUM_THREADS];

  //set Height chunk
  HEIGHT_CHUNK = HEIGHT/NUM_THREADS;
  IMG_CHUNK_SIZE = HEIGHT_CHUNK * WIDTH * 4;

  int height_lo_index = 0;
  int height_hi_index = HEIGHT_CHUNK;

  //render chunks pass
  for(int i=0; i<NUM_THREADS; i++){
    //malloc the image space
    img_chunk[i] = (float*)malloc(sizeof(float) * IMG_CHUNK_SIZE);

    //spin off thread
    t[i] = thread(renderThreaded, s_scene_ptr, &kdtree, img_chunk[i], height_lo_index, height_hi_index, LOW_SAMPLE_FLAG);

    //increment height indices
    height_lo_index += HEIGHT_CHUNK;
    height_hi_index += HEIGHT_CHUNK;
  }
  for(int i=0; i<NUM_THREADS; i++){
    t[i].join();
  }

  //stitch the image
  float *img = (float*)malloc(sizeof(float) * HEIGHT * WIDTH * 4);
  height_lo_index = 0;
  for(int i=0; i<NUM_THREADS; i++){
    stitch(img, img_chunk[i], WIDTH, height_lo_index, IMG_CHUNK_SIZE);
    height_lo_index += HEIGHT_CHUNK;
  }

  //tone map
  tone_map(img, HEIGHT * WIDTH * 4);
  printf("Rendered! \n");

  //////////////////CAIRO///////////////////////
  Pixel* imgData = (Pixel *) malloc(sizeof(Pixel) * WIDTH * HEIGHT);

  for (int y=0; y<HEIGHT; y++){
    for (int x=0; x<WIDTH; x++){
      
      int indexOld = (y * WIDTH * 4) + (x * 4);
      int indexNew = ((HEIGHT - y - 1) * WIDTH) + x;
      imgData[indexNew].B = (unsigned char) img[indexOld + 0];
      imgData[indexNew].G = (unsigned char) img[indexOld + 1];
      imgData[indexNew].R = (unsigned char) img[indexOld + 2];
      imgData[indexNew].A = (unsigned char) img[indexOld + 3];

    }
  }

  int stride = cairo_format_stride_for_width (CAIRO_FORMAT_RGB24, WIDTH);

  cairo_surface_t* surface_cairo =
    cairo_image_surface_create_for_data ((unsigned char*)imgData, CAIRO_FORMAT_RGB24, WIDTH, HEIGHT, stride);


  cairo_surface_write_to_png (surface_cairo, outfile);
  cairo_surface_destroy (surface_cairo);

  free(imgData);
  /////////////////////////////////////////
  free(img);
  for(int i=0; i<NUM_THREADS; i++){
    free(img_chunk[i]);
  }
  root->~KDnode();
  //scene.~SCEscene() is auto called at end of scope

  //end timing
  current_utc_time(&end);
  double elapsed;
  elapsed = (end.tv_sec - start.tv_sec);
  elapsed += (end.tv_nsec - start.tv_nsec)/1000000000.0;
  printf("Time elapsed %f seconds\n", elapsed);

  return 0;
}
