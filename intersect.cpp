//intesect.cpp										Wendy Chen, 2017

//This file contains intersection test functions
//Modified from the following sources
//sources: https://www.nczonline.net/blog/2009/06/09/computer-science-in-javascript-binary-search-tree-part-1/
//sources: http://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/
//sources: http://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/tribox.txt
//sources: http://stackoverflow.com/questions/14418595/c-method-for-iterating-through-a-structs-members-like-an-array

#include "intersect.h"

//convention, vec3 has no underscore
//convention, vector will have underscore


vector<float> vecToVector(vec3 v){
	vector<float> _a;
	_a.push_back(v.x);
	_a.push_back(v.y);
	_a.push_back(v.z);
	return _a;
}


vec3 vectorToVec(vector<float> _a){
	vec3 v = {
		.x = _a[0],
		.y = _a[1],
		.z = _a[2]
	};
	return v;
}

///////////////////////////////////////////////////////////////////////////
// Triangle Intersection helper functions 
///////////////////////////////////////////////////////////////////////////
//Refactored from code found online from:
//http://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/

//The functions allow for intersections between a triangle and an
//axis-aligned bounding box to be detected



//given three floats, set the min and max
void findminmax(float x0, float x1, float x2, float* min, float* max){
	//init
	*min = x0;
	*max = x0;

	if(x1<*min){
		*min = x1;
	}
	if(x2<*min){
		*min = x2;
	}
	if(x1>*max){
		*max = x1;
	}
	if(x2>*max){
		*max = x2;
	}
}

//because helper, takes vectors instead of vec3
bool planeBoxOverlap(vector<float> _normal, vector<float> _vert, vector<float> _maxbox){
	int q;
	vector<float> _vmin(3);
	vector<float> _vmax(3);
	float v;

	//populate vmin and vmax
	for(q=0; q<=2; q++){
		v = _vert[q];
		if(_normal[q]>0.0f){
			_vmin[q]= -_maxbox[q] - v;
			_vmax[q]= _maxbox[q] - v;
		}
		else{
			_vmin[q] = _maxbox[q] - v;
			_vmax[q] = -_maxbox[q] - v;
		}
	}

	//convert vectors to vec3
	// vec3 vmin = vectorToVec(_vmin);
	// vec3 vmax = vectorToVec(_vmax);

	if(dotProduct(vectorToVec(_normal), vectorToVec(_vmin)) > 0.0f){
		return false; //0
	}
	if(dotProduct(vectorToVec(_normal), vectorToVec(_vmax)) >= 0.0f){
		return true; //1
	}
	return false; //0
}

/////////////////////
bool AXISTEST_X01(float a, float b, float fa, float fb,
	vector<float> _v0, vector<float> _v2, vector<float> _boxhalfsize){

	float p0 = a*_v0[1] - b*_v0[2];
	float p2 = a*_v2[1] - b*_v2[2];
	float min, max;

	if(p0<p2){min = p0; max = p2;} else {min = p2; max = p0;}
	float rad = fa*_boxhalfsize[1] + fb*_boxhalfsize[2];

	if(min>rad || max<-rad){
		return true; //early detection no intersection
	}
	else{
		return false;
	}
}

bool AXISTEST_X2(float a, float b, float fa, float fb,
	vector<float> _v0, vector<float> _v1, vector<float> _boxhalfsize){

	float p0 = a*_v0[1] - b*_v0[2];
	float p1 = a*_v1[1] - b*_v1[2];
	float min, max;

	if(p0<p1){min = p0; max = p1;} else {min = p1; max = p0;}
	float rad = fa*_boxhalfsize[1] + fb*_boxhalfsize[2];

	if(min>rad || max<-rad){
		return true; //early detection no intersection
	}
	else{
		return false;
	}
}

/////////////////////
bool AXISTEST_Y02(float a, float b, float fa, float fb,
	vector<float> _v0, vector<float> _v2, vector<float> _boxhalfsize){

	float p0 = -a*_v0[0] + b*_v0[2];
	float p2 = -a*_v2[0] + b*_v2[2];
	float min, max;

	if(p0<p2){min = p0; max = p2;} else {min = p2; max = p0;}
	float rad = fa*_boxhalfsize[0] + fb*_boxhalfsize[2];

	if(min>rad || max<-rad){
		return true; //early detection no intersection
	}
	else{
		return false;
	}
}

bool AXISTEST_Y1(float a, float b, float fa, float fb,
	vector<float> _v0, vector<float> _v1, vector<float> _boxhalfsize){

	float p0 = -a*_v0[0] + b*_v0[2];
	float p1 = -a*_v1[0] + b*_v1[2];
	float min, max;

	if(p0<p1){min = p0; max = p1;} else {min = p1; max = p0;}
	float rad = fa*_boxhalfsize[0] + fb*_boxhalfsize[2];

	if(min>rad || max<-rad){
		return true; //early detection no intersection
	}
	else{
		return false;
	}
}


/////////////////////
bool AXISTEST_Z12(float a, float b, float fa, float fb,
	vector<float> _v1, vector<float> _v2, vector<float> _boxhalfsize){

	float p1 = a*_v1[0] - b*_v1[1];
	float p2 = a*_v2[0] - b*_v2[1]; //bug fix
	float min, max;

	if(p2<p1){min = p2; max = p1;} else {min = p1; max = p2;}
	float rad = fa*_boxhalfsize[0] + fb*_boxhalfsize[1];

	if(min>rad || max<-rad){
		return true; //early detection no intersection
	}
	else{
		return false;
	}
}


bool AXISTEST_Z0(float a, float b, float fa, float fb,
	vector<float> _v0, vector<float> _v1, vector<float> _boxhalfsize){

	float p0 = a*_v0[0] - b*_v0[1];
	float p1 = a*_v1[0] - b*_v1[1];
	float min, max;

	if(p0<p1){min = p0; max = p1;} else {min = p1; max = p0;}
	float rad = fa*_boxhalfsize[0] + fb*_boxhalfsize[1];

	if(min>rad || max<-rad){
		return true; //early detection no intersection
	}
	else{
		return false;
	}
}


///////////////////////////////////
//triverts are a vector
bool triBoxOverlap(vec3 boxcenter, vec3 boxhalfsize, vector<vec3> triverts){
	vector<float> _boxcenter = vecToVector(boxcenter);
	vector<float> _boxhalfsize = vecToVector(boxhalfsize);

	vector<float> _v0(3);
	vector<float> _v1(3);
	vector<float> _v2(3);

	float min, max;
	float p0, p1, p2;
	float rad;
	float fex, fey, fez;

	vector<float> _normal(3);
	vector<float> _e0(3);
	vector<float> _e1(3);
	vector<float> _e2(3);

	/* move everything so that the boxcenter is in (0,0,0) */

	_v0 = vecToVector(subtract(triverts[0], boxcenter));
	_v1 = vecToVector(subtract(triverts[1], boxcenter));
	_v2 = vecToVector(subtract(triverts[2], boxcenter));

	/* compute triangle edges */

	_e0 = vecToVector(subtract(vectorToVec(_v1), vectorToVec(_v0)));
	_e1 = vecToVector(subtract(vectorToVec(_v2), vectorToVec(_v1)));
	_e2 = vecToVector(subtract(vectorToVec(_v0), vectorToVec(_v2)));

	/*  test the 9 tests first (this was faster) */

	fex = fabs(_e0[0]);
	fey = fabs(_e0[1]);
	fez = fabs(_e0[2]);

	if(AXISTEST_X01(_e0[2], _e0[1], fez, fey, _v0, _v2, _boxhalfsize)) return false;
	if(AXISTEST_Y02(_e0[2], _e0[0], fez, fex, _v0, _v2, _boxhalfsize)) return false;
	if(AXISTEST_Z12(_e0[1], _e0[0], fey, fex, _v1, _v2, _boxhalfsize)) return false;

	fex = fabs(_e1[0]);
	fey = fabs(_e1[1]);
	fez = fabs(_e1[2]);

	if(AXISTEST_X01(_e1[2], _e1[1], fez, fey, _v0, _v2, _boxhalfsize)) return false;
	if(AXISTEST_Y02(_e1[2], _e1[0], fez, fex, _v0, _v2, _boxhalfsize)) return false;
	if(AXISTEST_Z0(_e1[1], _e1[0], fey, fex, _v0, _v1, _boxhalfsize)) return false;

	fex = fabs(_e2[0]);
	fey = fabs(_e2[1]);
	fez = fabs(_e2[2]);

	if(AXISTEST_X2(_e2[2], _e2[1], fez, fey, _v0, _v1, _boxhalfsize)) return false;
	if(AXISTEST_Y1(_e2[2], _e2[0], fez, fex, _v0, _v1, _boxhalfsize)) return false;
	if(AXISTEST_Z12(_e2[1], _e2[0], fey, fex, _v1, _v2, _boxhalfsize)) return false;


	/*  first test overlap in the {x,y,z}-directions */
	/*  find min, max of the triangle each direction, and test for overlap in */
	/*  that direction -- this is equivalent to testing a minimal AABB around */
	/*  the triangle against the AABB */

	/* test in X-direction */
	findminmax(_v0[0], _v1[0], _v2[0], &min, &max);
	if(min>_boxhalfsize[0] || max<-_boxhalfsize[0]){
		return false;
	}

	/* test in Y-direction */
	findminmax(_v0[1], _v1[1], _v2[1], &min, &max);
	if(min>_boxhalfsize[1] || max<-_boxhalfsize[1]){
		return false;
	}

	/* test in Z-direction */
	findminmax(_v0[2], _v1[2], _v2[2], &min, &max);
	if(min>_boxhalfsize[2] || max<-_boxhalfsize[2]){
		return false;
	}

	/*  test if the box intersects the plane of the triangle */
   	/*  compute plane equation of triangle: normal*x+d=0 */

	_normal = vecToVector(crossProduct(vectorToVec(_e0), vectorToVec(_e1)));
	if(!planeBoxOverlap(_normal, _v0, _boxhalfsize)){
		return false;
	}

	//Found Overlap
	return true;
}

///////////////////////////////////////////////////////////////////////////
// Intersection tests (User can see)
///////////////////////////////////////////////////////////////////////////

//given sphere and bounding box
//approximages the bounding box as sphere
bool sphereBB(Sphere sphere, vec3 min, vec3 max){
	//get box center
	vec3 boxCenter = {
		.x= (max.x - min.x)/2.0 + min.x,
		.y = (max.y - min.y)/2.0 + min.y,
		.z = (max.z - min.z)/2.0 + min.z
	};
	//get box radius
	float boxRadius = length(subtract(max, min));
	//compare radius
	float radiusSum = boxRadius + sphere.radius;
	float distance = length(subtract(sphere.point, boxCenter));

	if(radiusSum >= distance){
		return true; 
	}
	else{
		return false;
	}
}

//given triangle and bounding box
//packages arguments and calls triBoxOverlap
bool triangleBB(Triangle tri, vec3 min, vec3 max){
	//get box center
	vec3 boxCenter = {
		.x = (max.x - min.x)/2.0 + min.x,
		.y = (max.y - min.y)/2.0 + min.y,
		.z = (max.z - min.z)/2.0 + min.z
	};

	//get box half size
	vec3 boxHalfSize = {
		.x = (max.x - min.x)/2.0,
		.y = (max.y - min.y)/2.0,
		.z = (max.z - min.z)/2.0
	};

	//collect triangle vertices as vector
	vector<vec3> triverts;
	triverts.push_back(tri.point1);
	triverts.push_back(tri.point2);
	triverts.push_back(tri.point3);

	//boxCenter is a vec3, boxHalfSize is a vec3
	//triverts is a vector of vec3
	if(triBoxOverlap(boxCenter, boxHalfSize, triverts)){
		return true;
	}
	else{
		return false;
	}
}

bool objectBB(Object obj, vec3 min, vec3 max){
	if(obj.type == SPHERE){
		Sphere* sph = (Sphere*) obj.object;
		return sphereBB(*sph, min, max);
	}
	else{
		Triangle *tri = (Triangle*) obj.object;
		return triangleBB(*tri, min, max);
	}
}

