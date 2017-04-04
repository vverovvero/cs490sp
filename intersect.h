//intersect.h											Wendy Chen, 2/2017

//Contains intersections tests
//Used for building the kd-tree in SCEparser.h

//May also had rayBB intersection tests


#ifndef INTERSECT_H
#define INTERSECT_H

#include "main.h"
#include "vec3.h"

#include <stdio.h>
#include <math.h>

#include <vector>
// #include <unordered_map>
// #include "boost/tuple/tuple.hpp"

using std::vector;
// using std::unordered_map;
// using boost::tuple;


/************************************/
/*           Intersection          */
/************************************/

//for kd tree construction
bool sphereBB(Sphere sphere, vec3 min, vec3 max);
bool triangleBB(Triangle tri, vec3 min, vec3 max);
bool objectBB(Object obj, vec3 min, vec3 max);


void TEST_INTERSECT();


#endif