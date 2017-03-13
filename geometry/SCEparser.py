############################################################################################
# Read in SCE file that contains a user friendly scene description.
# Use:
# python ./SCEparser.py <sce_file> <h_file>

# Scene contains lists such as:
# scene.lights = [<instance1>, <instance2>, etc]
# Each instance is a dict with the appropriate attributes
# ie. <light instance> = {'type': 'OMNI', 'point': (50, 50, 50), 'color': (255, 255, 125)}

# remember that material indices start at 0!
# Angles are specified in degrees, so we must convert to Radians when parsing
# INCLUDE only supports spheres and triangles in the mesh
# Supported transforms are Translate(x, y, z), ScaleR(s), Scale(x, y, z),  RotateX(a), RotateY(a), RotateZ(a)

# Sources:
# http://stackoverflow.com/questions/642154/how-to-convert-strings-into-integers-in-python
# http://stackoverflow.com/questions/14607640/rotating-a-vector-in-3d-space

# Dependencies:
# bitstring module http://pythonhosted.org/bitstring/index.html
#	 http://scott-griffiths.github.io/bitstring/
# 	 easy_install bitstring

############################################################################################

# TO DO
# add support for transformations (Translate, Rotate, Scale)
# add support for INCLUDE flag
# update README

import re
import sys
import math
import bitstring
from bitstring import BitArray, BitStream

print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)


#################################
# Scene Object (Intermediate)
#################################

# Notes on conversions:

# Camera Up vector is converted to (0, 1, 0)

#lightType
#0	OMNI
#1	SPOT

#materialType
#0	ORIGINAL
#1	PHONG

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
# Scene -> binary file
#################################

#Write a binary stream of 32-bits at a time (ints or floats)
# The format of the binary file is:
# -send camera
# -send light(s)
# -send material(s)
# -send sphere(s)
# -send triangle(s)
# -send END code

# -each is a command followed by args
# ie. CAM_CMD CAM_ARG_1 CAM_ARG_2 CAM_ARG_3 CAM_ARG_4

#Command codes:
#0	Camera
#1	Light
#2	Material
#3	Sphere
#4 	Triangle

#Parser in cpp needs: (F for FLOAT_32, I for INT_32)
#CMD_CAM F_Px F_Py F_Pz F_Tx F_Ty F_Tz F_FOV F_Ux F_Uy F_Uz
#CMD_LGT I_Type F_Px F_Py F_Pz F_Cx F_Cy F_Cz
#CMD_MAT F_Cx F_Cy F_Cz I_Type I_Metal F_Spec F_Lam F_Amb F_exp
#CMD_SPH F_Px F_Py F_Pz F_R	I_matIndex
#CMD_TRI F_P1x F_P1y F_P1z F_P2x F_P2y F_P2z F_P3x F_P3y F_P3z I_matIndex

#lightType
#0	OMNI
#1	SPOT

#materialType
#0	ORIGINAL
#1	PHONG

def writeB(scene, b_name): 
	#Sanitization checks
	#if no camera, error:
	if scene.n_cameras == 0:
		print "Error: Scene needs a camera."
		return

	#Create bitstring
	s = BitArray()

	#Camera
	camera = scene.cameras[0]
	t = BitArray()
	t = bitstring.pack("int:32, 10*float:32", 0,
		camera['point'][0], camera['point'][1], camera['point'][2], 
		camera['fieldOfView'],
		camera['toPoint'][0], camera['toPoint'][1], camera['toPoint'][2], 
		camera['up'][0], camera['up'][1], camera['up'][2])
	# print t.unpack("int:32, 10*float:32")
	s = s + t

	#Lights
	for i in range(scene.n_lights):
		light = scene.lights[i]
		t = BitArray()
		t = bitstring.pack("2*int:32, 6*float:32", 1,
			light['type'], 
			light['point'][0], light['point'][1], light['point'][2], 
			light['color'][0], light['color'][1], light['color'][2])
		# print t.unpack("2*int:32, 6*float:32")
		s = s + t

	#Materials
	for i in range(scene.n_materials):
		mat = scene.materials[i]
		t = BitArray()
		t = bitstring.pack("int:32, 3*float:32, 2*int:32, 4*float:32", 2,
			mat['color'][0], mat['color'][1], mat['color'][2], 
			mat['type'], mat['metal'], 
			mat['specular'], mat['lambert'], mat['ambient'], mat['exponent'])
		# print t.unpack("int:32, 3*float:32, 2*int:32, 4*float:32")
		s = s + t

	#Spheres
	for i in range(scene.n_spheres):
		sphere = scene.spheres[i]
		t = BitArray()
		t = bitstring.pack("int:32, 4*float:32, int:32", 3,
			sphere['point'][0], sphere['point'][1], sphere['point'][2], sphere['radius'],
			sphere['materialIndex'])
		# print t.unpack("int:32, 4*float:32, int:32")
		s = s + t

	#Triangles
	for i in range(scene.n_triangles):
		tri = scene.triangles[i]
		t = BitArray()
		t = bitstring.pack("int:32, 9*float:32, int:32", 4,
			tri['point1'][0], tri['point1'][1], tri['point1'][2],
			tri['point2'][0], tri['point2'][1], tri['point2'][2],
			tri['point3'][0], tri['point3'][1], tri['point3'][2],
			tri['materialIndex'])
		# print t.unpack("int:32, 9*float:32, int:32")
		s = s + t

	#Write to file
	with open(b_name, "wb") as f:
		s.tofile(f)

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

# Parsing object
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
				if len(args_list) == 3:
					camera_args["Up"] = (args_list[0], args_list[1], args_list[2])
				elif isinstance(args_list[0], basestring):
					vector = args_list[0]
					if vector == "UP":
						camera_args["Up"] = (0, 1, 0)
					else:
						"Cannot interpret vector macro for camera Up attribute."
				else:
					print "Cannot interpret argument for camera Up attribute."
			else:
				print "Invalid Camera attribute: " + function
		# print camera_args
		scene.add_camera(camera_args["Point"], camera_args["LookAt"], camera_args["FOV"], camera_args["Up"])

	def light(self, scene, attributes):
		light_args = dict()
		for attribute in attributes:
			(function, args_list) = getAttr(attribute)
			if function == "Type":
				type = args_list[0]
				if type == "OMNI":
					light_args["Type"] = 0
				elif type == "SPOT":
					light_args["Type"] = 1
				else:
					print "Unconverted light type: " + type
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
				type = args_list[0]
				if type == "ORIGINAL":
					mat_args["Type"] = 0
				elif type == "PHONG":
					mat_args["Type"] = 1
				else:
					print "Unconverted Material type: " + type
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

	def sphere(self, scene, attributes, defaultMaterial, defaultTransforms):
		sphere_args = dict()
		sphere_transforms = list()
		for attribute in attributes:
			(function, args_list) = getAttr(attribute)
			if function == "Point":
				sphere_args["Point"] = (args_list[0], args_list[1], args_list[2])
			elif function == "Radius":
				sphere_args["Radius"] = args_list[0]
			elif function == "Material":
				sphere_args["Material"] = args_list[0]
			elif function == "Translate":
				sphere_transforms.append(("Translate", (args_list[0], args_list[1], args_list[2])))
			elif function == "ScaleR":
				sphere_transforms.append(("ScaleR", args_list[0]))
			elif function == "RotateX":
				sphere_transforms.append(("RotateX", args_list[0]))
			elif function == "RotateY":
				sphere_transforms.append(("RotateY", args_list[0]))
			elif function == "RotateZ":
				sphere_transforms.append(("RotateZ", args_list[0]))
			else:
				print "Invalid Sphere attribute: " + function	
		if defaultMaterial:
			sphere_args["Material"] = defaultMaterial
		#Override sphere_transforms if default transforms
		if defaultTransforms:
			sphere_transforms = defaultTransforms
		#apply transforms
		if sphere_transforms:
			for sphere_transform in sphere_transforms:
				#fetch sphere point
				(x,y,z) = sphere_args["Point"]
				#apply transform
				(transform, args) = sphere_transform
				if transform == "Translate":
					(xt, yt, zt) = args
					sphere_args["Point"] = (x+xt, y+yt, z+zt)
				elif transform == "ScaleR":
					s = args 
					r = sphere_args["Radius"]
					sphere_args["Radius"] = r*s
				elif transform == "RotateX":
					a = args 
					X = x*math.cos(math.radians(a)) - y*math.sin(math.radians(a))
					Y = x*math.sin(math.radians(a)) + y*math.cos(math.radians(a))
					Z = z
					sphere_args["Point"] = (X, Y, Z)
				elif transform == "RotateY":
					a = args
					X = x*math.cos(math.radians(a)) + z*math.sin(math.radians(a))
					Y = y
					Z = -x*math.sin(math.radians(a)) + z*math.cos(math.radians(a))
					sphere_args["Point"] = (X, Y, Z)
				elif transform == "RotateZ":
					a = args 
					X = x
					Y = y*math.cos(math.radians(a)) - z*math.sin(math.radians(a))
					Z = y*math.sin(math.radians(a)) + z*math.cos(math.radians(a))
					sphere_args["Point"] = (X, Y, Z)
				else:
					print "Invalid Transform: " + transform
		scene.add_sphere(sphere_args["Point"], sphere_args["Radius"], sphere_args["Material"])

	def triangle(self, scene, attributes, defaultMaterial, defaultTransforms):
		tri_args = dict()
		tri_transforms = list()
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
			elif function == "Translate":
				tri_transforms.append(("Translate", (args_list[0], args_list[1], args_list[2])))
			elif function == "Scale":
				tri_transforms.append(("Scale", (args_list[0], args_list[1], args_list[2])))
			elif function == "RotateX":
				tri_transforms.append(("RotateX", args_list[0]))
			elif function == "RotateY":
				tri_transforms.append(("RotateY", args_list[0]))
			elif function == "RotateZ":
				tri_transforms.append(("RotateZ", args_list[0]))
			else:
				print "Invalid Triangle attribute: " + function
		#override with defaults
		if defaultMaterial:
			tri_args["Material"] = defaultMaterial
		#override tri_transforms with default transforms
		if defaultTransforms:
			tri_transforms = defaultTransforms
		#apply transforms:
		if tri_transforms:
			for tri_transform in tri_transforms:
				(transform, args) = tri_transform
				#iterate through the points and apply
				for key in ["P1", "P2", "P3"]:
					(x, y, z) = tri_args[key]
					if transform == "Translate":
						(xt, yt, zt) = args
						tri_args[key] = (x+xt, y+yt, z+zt)
					elif transform == "Scale":
						(xt, yt, zt) = args
						tri_args[key] = (x*xt, y*yt, z*zt)
					elif transform == "RotateX":
						a = args
						X = x*math.cos(math.radians(a)) - y*math.sin(math.radians(a))
						Y = x*math.sin(math.radians(a)) + y*math.cos(math.radians(a))
						Z = z
						tri_args[key] = (X, Y, Z)
					elif transform == "RotateY":
						a = args
						X = x*math.cos(math.radians(a)) + z*math.sin(math.radians(a))
						Y = y
						Z = -x*math.sin(math.radians(a)) + z*math.cos(math.radians(a))
						tri_args[key] = (X, Y, Z)
					elif transform == "RotateZ":
						a = args
						X = x
						Y = y*math.cos(math.radians(a)) - z*math.sin(math.radians(a))
						Z = y*math.sin(math.radians(a)) + z*math.cos(math.radians(a))
						tri_args[key] = (X, Y, Z)
					else:
						print "Invalid Transform: " + transform
		#add triangle to scene
		scene.add_triangle(tri_args["P1"], tri_args["P2"], tri_args["P3"], tri_args["Material"])
		
	def include(self, scene, attributes):
		inc_args = dict()
		inc_transforms = list()
		for attribute in attributes:
			# print getAttr(attribute)
			(function, args_list) = getAttr(attribute)
			if function == "Mesh":
				inc_args["Mesh"] = args_list[0]
			elif function == "Material":
				inc_args["Material"] = args_list[0]
			elif function == "Translate":
				inc_transforms.append(("Translate", (args_list[0], args_list[1], args_list[2])))
			elif function == "Scale":
				inc_transforms.append(("Scale", (args_list[0], args_list[1], args_list[2])))
			elif function == "RotateX":
				inc_transforms.append(("RotateX", args_list[0]))
			elif function == "RotateY":
				inc_transforms.append(("RotateY", args_list[0]))
			elif function == "RotateZ":
				inc_transforms.append(("RotateZ", args_list[0]))
			else:
				print "Invalid Include attribute: " + function

		##After getting all arguments, then add mesh
		if inc_args:
			handle = open(inc_args["Mesh"])
			for line in handle:
				tokens = line.split(' ', 1)
				keyword = tokens[0]
				attributes = getAttrList(tokens[1:])
				if keyword == "SPHERE":
					# print "SPHERE PARSER"
					self.sphere(scene, attributes, inc_args["Material"], inc_transforms)
				elif keyword == "TRIANGLE":
					# print "TRIANGLE PARSER"
					self.triangle(scene, attributes, inc_args["Material"], inc_transforms)
				elif keyword == "\n":
					pass
				elif keyword[0] == "#":
					pass
				else:
					print "Unsupported Include Directive: " + keyword
			handle.close


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
			Parse.sphere(scene, attributes, None, [])
		elif keyword == "TRIANGLE":
			# print "TRIANGLE PARSER"
			Parse.triangle(scene, attributes, None, [])
		elif keyword == "INCLUDE":
			# print "INCLUDE PARSER"
			Parse.include(scene, attributes)
		elif keyword == "\n":
			pass
		elif keyword[0] == "#":
			pass
		else:
			print "Error Invalid Keyword: " + keyword
	sce.close

	#write scene object -> binary############
	writeB(scene, sys.argv[2])

##############################################

parseSCE(sys.argv[1])















