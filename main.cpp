/***************************** Compilation notes *******************************/
// Update:  
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

/***************************** Headers and Structs *******************************/

#include "main.h"
#include "SCEparser.h"


/***************************** Forward declarations *******************************/

vec3 objectNormal(Object* object, vec3 point);
float sphereIntersection(Sphere* sphere, Ray* ray);
vec3 sphereNormal(Sphere* sphere, vec3 pos);
vec3 triNormal(Triangle* tri);
float triIntersection(Triangle* tri, Ray* ray);
float* render(Scene* scene);
vec3 trace(Ray* ray, Scene* scene, vec3 *pointAtTime, float *contribution);
Dist intersectScene(Ray* ray, Scene* scene);
int isLightVisible(vec3 point, Scene* scene, vec3 light);
vec3 surface(Ray* ray, Scene* scene, Object* object, vec3 pointAtTime, vec3 normal, float *contribution);


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

  if (object->type == TRIANGLE) {
    return (triNormal((Triangle*)object->object));
  }

  printf("objectNormal broke\n");
  return ZERO;

}

float sphereIntersection(Sphere* sphere, Ray* ray){

  vec3 eye_to_center = subtract(sphere->point, ray->point);

  float v = dotProduct(eye_to_center, ray->vector);
  float eoDot = dotProduct(eye_to_center, eye_to_center);
  float discriminant = (sphere->radius * sphere->radius) - eoDot + (v * v);

  if ((fabsf(length(eye_to_center)-sphere->radius) / sphere->radius) < .001f) {
    return -1.0f;
  }

  // If the discriminant is negative, that means that the sphere hasn't
  // been hit by the ray
  if (discriminant < 0.0f) {
      return -1.0f;
  } else {
      // otherwise, we return the distance from the camera point to the sphere
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

Dist intersectScene(Ray* ray, Scene* scene) {

  Dist closest = { .distance = FLT_MAX, .object = NULL};

  // Find closest intersecting object
  for (int i = 0; i < scene->n_objects; i++) {

    Object* object = &(scene->objects[i]);

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

  if (closest.distance == FLT_MAX) {
    closest.distance = -1.0f;
  }

  return closest;
}

int isLightVisible(vec3 point, Scene* scene, vec3 light) {

  vec3 full_vector = subtract(light,point);
  vec3 vector = unitVector(subtract(light, point));
  Ray ray = { .point = point, .vector = vector};

  Dist distObject =  intersectScene(&ray, scene);

  //Edit?  Is this comparing length of unit vector
  if (distObject.distance > 0.0f && distObject.distance < (length(full_vector) -.005)) {
    return 0; // False case where an object is found between the point and the light
  }
  else {
    return 1; // Either negative (no collision at all) or >length(vector) (light is closer than collision)
  }
}

vec3 surface(Ray* ray, Scene* scene, Object* object, vec3 pointAtTime, vec3 normal, float* contribution) {

  Material* material = &(scene->materials[object->matIndex]);
  vec3 objColor = material->color;
  vec3 c = ZERO;
  vec3 lambertAmount = ZERO;

  // lambert shading
  if (material->lambert > 0.0f) {
    for (int i = 0; i < scene->n_lights; i++) {
      vec3 lightPoint = scene->lights[i].point;

      if (isLightVisible(pointAtTime, scene, lightPoint)){
        // lambertian reflectance
        *contribution = dotProduct(unitVector(subtract(lightPoint, pointAtTime)), normal);

        if(*contribution > 0.0f) {
          lambertAmount = add(lambertAmount, scale(scene->lights[i].color, *contribution));
        }

      }
    }
  }
    // for assn 6, adjust lit color by object color and divide by 255 since light color is 0 to 255
  lambertAmount = compScale(lambertAmount, objColor);
  lambertAmount = scale(lambertAmount, material->lambert);
  lambertAmount = scale(lambertAmount, 1./255.);

  return add3(c, lambertAmount, scale(objColor, material->ambient));
}

//need to keep track of path_length and what bounce we are currently on
//no!  don't want to recalculate.  store throughput
vec3 trace(Ray* ray, Scene* scene, vec3 *pointAtTime, float* contribution) {

  Dist distObject = intersectScene(ray, scene);

  // If we don't hit anything, fill this pixel with the background color -
  // in this case, white.
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
  // printf("trace intersected pointAtTime(%f, %f, %f)\n", (*pointAtTime).x, (*pointAtTime).y, (*pointAtTime).z);

  return surface(ray, scene, object, *pointAtTime, objectNormal(object, *pointAtTime), contribution);
}

float* render(Scene* scene) {

  float* img = (float*)malloc(sizeof(float) * HEIGHT * WIDTH * 4);

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

  // int path_i; //keep track of bounce i

  Ray ray;
  // ray.point = camera->point;


  for (int x = 0; x < WIDTH; x++) {
    printf("Rendering x row: %d\n", x);
    for (int y = 0; y < HEIGHT; y++) {

        // For Assign 6, brute-force antialiasing with 25 samples/pixel
        vec3 color = ZERO;

        // for (float s = -.4f; s < .6f; s+=.2f) {
        //   for (float r = -.4f; r < .6; r +=.2f) {
        //
            // vec3 xcomp = scale(vpRight, ((x+s) * pixelWidth) - halfWidth);
            // vec3 ycomp = scale(vpUp, ((y+r) * pixelHeight) - halfHeight);
        
            // ray.vector = unitVector(add3(eyeVector, xcomp, ycomp));
        //
        //     // use the vector generated to raytrace the scene, returning a color
        //     // as a `{x, y, z}` vector of RGB values
        //     color = add(color, trace(&ray, scene, 0));

            //One ray per pixel; no antialiasing
            ray.point = camera->point;
            vec3 xcomp = scale(vpRight, ((x) * pixelWidth) - halfWidth);
            vec3 ycomp = scale(vpUp, ((y) * pixelHeight) - halfHeight);
            ray.vector = unitVector(add3(eyeVector, xcomp, ycomp));
            // printf("Ray point (%f, %f, %f); ray vector (%f, %f, %f)\n", ray.point.x, ray.point.y, ray.point.z, ray.vector.x, ray.vector.y, ray.vector.z);
            //Direct lighting
            vec3 pointAtTime = ray.point; //first intersection point is ray origin
            float contribution = 0.0; //we assign roulette value as contribution
            float throughput = 1.0;
            color = add(color, trace(&ray, scene, &pointAtTime, &contribution));

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
              // printf("Russian roulette passed !\n");
              //Construct indirect ray
              //what is indirect_ray?
              ray.point = pointAtTime;
              //Monte carlo sample the new direction:
              u1 = drand48();
              u2 = drand48();
              indirect_direction.x = ray.point.x + cos(2*PI*u2) * sqrt(1-u1*u1);
              indirect_direction.y = ray.point.y + sin(2*PI*u2) * sqrt(1-u1*u1);
              indirect_direction.z = ray.point.z + u1;
              //Get new ray's direction
              ray.vector = unitVector(subtract(indirect_direction, ray.point)); 
              // printf("INDIRECT Ray point (%f, %f, %f); ray vector (%f, %f, %f)\n", ray.point.x, ray.point.y, ray.point.z, ray.vector.x, ray.vector.y, ray.vector.z);
              //what is current throughput?
              throughput = (1.0/PI) * throughput; //throughput is based off of diffuse reflectance

              // printf("new ray.point = (%f, %f, %f); new ray.vector = (%f, %f, %f)\n", ray.point.x, ray.point.y, ray.point.z, ray.vector.x, ray.vector.y, ray.vector.z);
              
              //Scale the first two bounces by throughput
              if(num_samples < 3){
                // indirect_color = add(indirect_color, trace(&ray, scene, &pointAtTime));
                indirect_color = add(indirect_color, scale(trace(&ray, scene, &pointAtTime, &contribution), throughput));
              }
              //don't scale roulette, due to averaging later
              else{
                indirect_roulette = add(indirect_roulette, trace(&ray, scene, &pointAtTime, &contribution));
              }
              num_samples++;

              //Calculate roulette value for next go
              Russian_Roulette = contribution;
              // printf("Russian_Roulette = %f\n", Russian_Roulette);
            }
            if(num_samples > 2){
              // printf("at x=%d y=%d, BEFORE SCALING Indirect Color is (%f, %f, %f)\n", x, y, indirect_color.x, indirect_color.y, indirect_color.z);
              float indirect_scale = 1.0/(num_samples-2.0);
              // printf("indirect_scale = %f\n", indirect_scale);
              indirect_roulette = scale(indirect_roulette, indirect_scale);
              if(indirect_roulette.x != 0 && indirect_roulette.y != 0 && indirect_roulette.z != 0){
                // printf("at x=%d y=%d, Indirect roulette is (%f, %f, %f)\n", x, y, indirect_roulette.x, indirect_roulette.y, indirect_roulette.z);
              }
              //Total color is:
              color = add(color, indirect_color); //direct plus 2 bounces
              color = add(color, indirect_roulette); //for roulette passes

            }
            ////////////////////////////////////////////
        //   }
        // }

        // printf("at x=%d y=%d, Color is (%f, %f, %f)\n", x, y, color.x, color.y, color.z);
        color = scale(color, 0.04);

        int index = (x * 4) + (y * width * 4);
        img[index + 0] = 0.0f;
        img[index + 1] = color.x;
        img[index + 2] = color.y;
        img[index + 3] = color.z;

    }
  }
  return img;
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

int main (int argc, char const *argv[]){
  //Confirm there is a scene file
  if(argc != 3){
    fprintf(stderr, "Usage: ./main <.bin file> <.png name>\n");
    return -1;
  }

  //Grab command line args
  char infile[50], outfile[50];
  strcpy(infile, argv[1]);
  strcpy(outfile, argv[2]);

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
  // s_scene_ptr = (Scene *) malloc(sizeof(struct Scene));
  s_scene_ptr = scene.get_scene();

  //initalize random number generator
  srand48(time(NULL));

  // This is MALLOCED!!
  printf("Getting the scene pointer\n");
  float* img = render(s_scene_ptr);
  

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
  //Scene is destructed automatically ~SCEscene()

  return 0;
}
