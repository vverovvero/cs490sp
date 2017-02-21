############################################################################################
# Read in SCE file that contains a user friendly scene description.
# Use:
# python ./SCEparser.py <sce_file> <h_file>

# Scene contains lists such as:
# scene.lights = [<instance1>, <instance2>, etc]
# Each instance is a dict with the appropriate attributes
# ie. <light instance> = {'type': 'OMNI', 'point': (50, 50, 50), 'color': (255, 255, 125)}

# remember that material indices start at 0!

############################################################################################

# TO DO
# add support for transformations (Translate, Rotate, Scale)
# add support for INCLUDE flag

import re
import sys

print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)


#################################
# Scene Object (Intermediate)
#################################

class Scene(object):

    #initialize lists
    def __init__(self):
    	self.cameras = list()
    	self.n_cameras = 0
    	self.lights = list()
    	self.n_lights = 0
    	self.materials = list()
    	self.n_materials = 0
    	self.spheres = list()
    	self.n_spheres = 0
    	self.triangles = list()
    	self.n_triangles = 0

    #add a camera
    def add_camera(self, point, toPoint, fieldOfView, up):
    	camera = {'point': point, 'toPoint': toPoint, 'fieldOfView': fieldOfView, 'up': up}
    	self.cameras.append(camera)
    	self.n_cameras += 1

    #add a light
    def add_light(self, type, point, color):
    	light = {'type': type, 'point': point, 'color': color}
    	self.lights.append(light)
    	self.n_lights += 1

    #add a material
    def add_material(self, color, type, metal, specular, lambert, ambient, exponent):
    	material = {'color': color, 'type': type, 'metal': metal, 'specular': specular, 'lambert': lambert, 'ambient': ambient, 'exponent': exponent}
    	self.materials.append(material)
    	self.n_materials += 1

    #add a sphere
    def add_sphere(self, point, radius, materialIndex):
    	sphere = {'point': point, 'radius': radius, 'materialIndex': materialIndex}
    	self.spheres.append(sphere)
    	self.n_spheres += 1

    #add a triangle
    def add_triangle(self, point1, point2, point3, materialIndex):
    	triangle = {'point1': point1, 'point2': point2, 'point3': point3, 'materialIndex': materialIndex}
    	self.triangles.append(triangle)
    	self.n_triangles += 1

    #sanity check functions###############################
    #call print)list by more specific functions below
    def print_list(self, self_list):
    	for item in self_list:
    		for key in item:
    			print key, ":", item[key]
    		print "------"

    def print_cameras(self):
    	print "Cameras======================"
    	Scene.print_list(self, self.cameras)

    def print_lights(self):
    	print "Lights======================="
    	Scene.print_list(self, self.lights)

    def print_materials(self):
	    print "Materials===================="
	    Scene.print_list(self, self.materials)

    def print_spheres(self):
	    print "Spheres======================"
	    Scene.print_list(self, self.spheres)

    def print_triangles(self):
	    print "Triangles===================="
	    Scene.print_list(self, self.triangles)    	


#################################
# Scene -> .h file
#################################

def writeH(scene, h_name):
	h_file = open(h_name, 'w+')
	
	#Camera###########################
	#if no camera, error
	if scene.n_cameras == 0:
		print "Error: Scene needs a camera."
		h_file.close
		return
	camera = scene.cameras[0]
	if isinstance(camera['up'], basestring):
		up = camera['up']
	else:
		up = "{\n\t\t.x = %ff,\n\t\t.y = %ff,\n\t\t.z = %ff\n\t},\n" % (camera['up'][0], camera['up'][1], camera['up'][2])
	buf_camera = ("Camera s_camera = {\n"
		    "\t.point = {\n\t\t.x = %ff,\n\t\t.y = %ff,\n\t\t.z = %ff\n\t},\n"
			"\t.fieldOfView = %ff,\n"
			"\t.toPoint = {\n\t\t.x = %ff,\n\t\t.y = %ff,\n\t\t.z = %ff\n\t},\n"
			"\t.up = %s\n"
			"};\n\n"
		% (camera['point'][0], camera['point'][1], camera['point'][2], camera['fieldOfView'],
			camera['toPoint'][0], camera['toPoint'][1], camera['toPoint'][2], up))
	h_file.write(buf_camera)

	#Lights###########################
	buf_lights = "Light s_lights[%d] = {\n" % scene.n_lights
	for i in range(scene.n_lights):
		light = scene.lights[i]
		buf_light = ("\t{\n"
			"\t\t.type = %s,\n"
			"\t\t.point = {\n\t\t\t.x = %ff,\n\t\t\t.y = %ff,\n\t\t\t.z = %ff\n\t\t},\n"
			"\t\t.color = {\n\t\t\t.x = %ff,\n\t\t\t.y = %ff,\n\t\t\t.z = %ff\n\t\t}\n"
			"\t},\n"
			% (light['type'], light['point'][0], light['point'][1], light['point'][2], 
				light['color'][0], light['color'][1], light['color'][2]))
		buf_lights += buf_light
	buf_lights += "};\n\n"
	h_file.write(buf_lights)

	#Materials########################
	buf_mats = "Material s_materials[%d] = {\n" % scene.n_materials
	for i in range(scene.n_materials):
		mat = scene.materials[i]
		buf_mat = ("\t{\n"
			"\t\t.color = {\n\t\t\t.x = %ff,\n\t\t\t.y = %ff,\n\t\t\t.z = %ff\n\t\t},\n"
			"\t\t.type = %s,\n"
			"\t\t.metal = %d,\n"
			"\t\t.specular = %ff,\n"
			"\t\t.lambert = %ff,\n"
			"\t\t.ambient = %ff,\n"
			"\t\t.exponent = %ff,\n"
			"\t},\n"
			% (mat['color'][0], mat['color'][1], mat['color'][2], mat['type'],
				mat['metal'], mat['specular'], mat['lambert'], mat['ambient'], mat['exponent']))
		buf_mats += buf_mat
	buf_mats += "};\n\n"
	h_file.write(buf_mats)

	#Spheres##########################
	buf_spheres = "Sphere s_spheres[%d] = {\n" % scene.n_spheres
	for i in range(scene.n_spheres):
		sphere = scene.spheres[i]
		buf_sphere = ("\t{\n"
			"\t\t.point = {\n\t\t\t.x = %ff,\n\t\t\t.y = %ff,\n\t\t\t.z = %ff\n\t\t},\n"
			"\t\t.radius = %ff\n"
			"\t},\n"
			% (sphere['point'][0], sphere['point'][1], sphere['point'][2], sphere['radius']))
		buf_spheres += buf_sphere
	buf_spheres += "};\n\n"
	h_file.write(buf_spheres)

	#Triangles########################
	buf_tris = "Triangle s_triangles[%d] = {\n" % scene.n_triangles
	for i in range(scene.n_triangles):
		tri = scene.triangles[i]
		buf_tri = ("\t{\n"
			"\t\t.point1 = {\n\t\t\t.x = %ff,\n\t\t\t.y = %ff,\n\t\t\t.z = %ff\n\t\t},\n"
			"\t\t.point2 = {\n\t\t\t.x = %ff,\n\t\t\t.y = %ff,\n\t\t\t.z = %ff\n\t\t},\n"
			"\t\t.point3 = {\n\t\t\t.x = %ff,\n\t\t\t.y = %ff,\n\t\t\t.z = %ff\n\t\t}\n"
			"\t},\n"
			% (tri['point1'][0], tri['point1'][1], tri['point1'][2],
				tri['point2'][0], tri['point2'][1], tri['point2'][2],
				tri['point3'][0], tri['point3'][1], tri['point3'][2]))
		buf_tris += buf_tri
	buf_tris += "};\n\n"
	h_file.write(buf_tris)

	#Objects##########################
	n_objects = scene.n_spheres + scene.n_triangles
	buf_objs = "Object s_objects[%d] = {\n" % n_objects
	#append spheres
	for i in range(scene.n_spheres):
		obj = scene.spheres[i]
		buf_obj = ("\t{\n"
			"\t\t.type = SPHERE,\n"
			"\t\t.matIndex = %d,\n"
			"\t\t.object = &(s_spheres[%d])\n"
			"\t},\n"
			% (obj['materialIndex'], i))
		buf_objs += buf_obj
	#append triangles
	for i in range(scene.n_triangles):
		obj = scene.triangles[i]
		buf_obj = ("\t{\n"
			"\t\t.type = TRIANGLE,\n"
			"\t\t.matIndex = %d,\n"
			"\t\t.object = &(s_triangles[%d])\n"
			"\t},\n"
			% (obj['materialIndex'], i))
		buf_objs += buf_obj
	buf_objs += "};\n\n"
	h_file.write(buf_objs)

	#Scene############################
	buf_scene = ("Scene s_scene = {\n"
		"\t.camera = &s_camera,\n"
		"\t.materials = s_materials,\n"
		"\t.objects = s_objects,\n"
		"\t.lights = s_lights,\n"
		"\t.n_lights = %d,\n"
		"\t.n_materials = %d,\n"
		"\t.n_objects = %d,\n"
		"};\n"
		% (scene.n_lights, scene.n_materials, scene.n_spheres + scene.n_triangles))
	h_file.write(buf_scene)

	h_file.close


#################################
# .sce file -> Scene
#################################

# classes for parsing input
# Camera
# Lights
# Material
# Sphere
# Triangle

#CAMERA Point(50, 50, 50) LookAt(50, 25, 0) FOV(60) Up(UP)

#sample token_string
#Type(OMNI) Point(50, 95, 50) Color(155, 155, 155)

#[string] -> ['Point(50, 50, 50)', 'LookAt(20, 50, 60)', ...]
def getAttrList(tokens):
	if not tokens:
		return []
	else:
		token_string = tokens[0]
		attrList = list()
		m = re.match(r"(\w*\([^\)]*\))(\s*)(.*)", token_string)
		while(m != None):
			attrList.append(m.group(1))
			m = re.match(r"(\w*\([^\)]*\))(\s*)(.*)", m.group(3))
		return attrList

#modified from:
#http://stackoverflow.com/questions/642154/how-to-convert-strings-into-integers-in-python
def is_int(var):
    try:
        int(var)
        return True
    except Exception:
        return False

def is_float(var):
    try:
        float(var)
        return True
    except Exception:
        return False

#'Point(50, 50, 50)' -> ['Point', [50, 50, 50]]
def getAttr(attribute):
	m = re.match(r"(\w*)\(([^\)]*)\)", attribute)
	if(m != None):
		function = m.group(1)
		args = m.group(2)
		argString_list = [x.strip() for x in args.split(',')]
		args_list = list()
		for arg in argString_list:
			if is_int(arg):
				args_list.append(int(arg))
			elif is_float(arg):
				args_list.append(float(arg))
			else:
				args_list.append(arg)
		return (function, args_list)
	return ()

# CAMERA parsing object
class Parser(object):

	def __init__(self):
		pass

	def camera(self, scene, attributes):
		# print attributes
		camera_args = dict()
		for attribute in attributes:
			#Get [function, args_list]for each attribute
			(function, args_list) = getAttr(attribute)
			if function == "Point":
				camera_args["Point"] = (args_list[0], args_list[1], args_list[2])
			elif function == "LookAt":
				camera_args["LookAt"] = (args_list[0], args_list[1], args_list[2])
			elif function == "FOV":
				camera_args["FOV"] = args_list[0]
			elif function == "Up":
				camera_args["Up"] = args_list[0]
			else:
				print "Invalid Camera attribute: " + function
		# print camera_args
		scene.add_camera(camera_args["Point"], camera_args["LookAt"], camera_args["FOV"], camera_args["Up"])

	def light(self, scene, attributes):
		light_args = dict()
		for attribute in attributes:
			(function, args_list) = getAttr(attribute)
			if function == "Type":
				light_args["Type"] = args_list[0]
			elif function == "Point":
				light_args["Point"] = (args_list[0], args_list[1], args_list[2])
			elif function == "Color":
				light_args["Color"] = (args_list[0], args_list[1], args_list[2])
			else:
				print "Invalid Light attribute: " + function
		scene.add_light(light_args["Type"], light_args["Point"], light_args["Color"])

	def material(self, scene, attributes):
		mat_args = dict()
		for attribute in attributes:
			(function, args_list) = getAttr(attribute)
			if function == "Color":
				mat_args["Color"] = (args_list[0], args_list[1], args_list[2])
			elif function == "Type":
				mat_args["Type"] = args_list[0]
			elif function == "Metal":
				mat_args["Metal"] = args_list[0]
			elif function == "Specular":
				mat_args["Specular"] = args_list[0]
			elif function == "Lambert":
				mat_args["Lambert"] = args_list[0]
			elif function == "Ambient":
				mat_args["Ambient"] = args_list[0]
			elif function == "Exponent":
				mat_args["Exponent"] = args_list[0]
			else:
				print "Invalid Material attribute: " + function
		scene.add_material(mat_args["Color"], mat_args["Type"], mat_args["Metal"], mat_args["Specular"], mat_args["Lambert"],
			mat_args["Ambient"], mat_args["Exponent"])

	def sphere(self, scene, attributes):
		sphere_args = dict()
		for attribute in attributes:
			(function, args_list) = getAttr(attribute)
			if function == "Point":
				sphere_args["Point"] = (args_list[0], args_list[1], args_list[2])
			elif function == "Radius":
				sphere_args["Radius"] = args_list[0]
			elif function == "Material":
				sphere_args["Material"] = args_list[0]
			else:
				print "Invalid Sphere attribute: " + function			
		scene.add_sphere(sphere_args["Point"], sphere_args["Radius"], sphere_args["Material"])

	def triangle(self, scene, attributes):
		tri_args = dict()
		for attribute in attributes:
			(function, args_list) = getAttr(attribute)
			if function == "P1":
				tri_args["P1"] = (args_list[0], args_list[1], args_list[2])
			elif function == "P2":
				tri_args["P2"] = (args_list[0], args_list[1], args_list[2])
			elif function == "P3":
				tri_args["P3"] = (args_list[0], args_list[1], args_list[2])
			elif function == "Material":
				tri_args["Material"] = args_list[0]
			else:
				print "Invalid Triangle attribute: " + function
		scene.add_triangle(tri_args["P1"], tri_args["P2"], tri_args["P3"], tri_args["Material"])

#open SCE, iterate through each line and call correct primitive construction
#use regular expression to get dict fields
def parseSCE(input_file):
	#initialize scene object#############
	scene = Scene()

	#initialize parser##################
	Parse = Parser()

	#parse .sce -> scene object##########
	sce = open(input_file)
	for line in sce:
		tokens = line.split(' ', 1)
		keyword = tokens[0]
		attributes = getAttrList(tokens[1:])
		if keyword == "CAMERA":
			# print "CAMERA PARSER"
			Parse.camera(scene, attributes)
		elif keyword == "LIGHT":
			# print "LIGHT PARSER"
			Parse.light(scene, attributes)
		elif keyword == "MATERIAL":
			# print "MATERIAL PARSER"
			Parse.material(scene, attributes)
		elif keyword == "SPHERE":
			# print "SPHERE PARSER"
			Parse.sphere(scene, attributes)
		elif keyword == "TRIANGLE":
			# print "TRIANGLE PARSER"
			Parse.triangle(scene, attributes)
		elif keyword == "INCLUDE":
			print "INCLUDE PARSER"
		elif keyword == "\n":
			pass
		elif keyword[0] == "#":
			pass
		else:
			print "Error Invalid Keyword: " + keyword
	sce.close

	#write scene object -> .h############
	writeH(scene, sys.argv[2])



##############################################

parseSCE(sys.argv[1])




# Testing ####################################

# scene = Scene()
# scene.add_camera((0,0,0), (6,6,6), 60, 'UP')
# scene.add_light('OMNI', (66, 66, 66), (125, 125, 125))
# scene.add_light('SPOT', (42, 21, 93), (255, 255, 255))
# scene.add_material((255, 120, 185), 'ORIGINAL', 0, 0.0, 0.85, 0.05, 0)
# scene.add_sphere((1,2,3), 25, 1)
# scene.add_triangle((1,2,3), (4,5,6), (7,8,9), 3)

# # scene.print_cameras()
# # scene.print_lights()
# # scene.print_materials()
# # scene.print_spheres()
# # scene.print_triangles()


# writeH(scene, 'test.h')













