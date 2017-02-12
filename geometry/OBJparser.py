############################################################################################
# Modified from OBJparser.py (Fall 2015, for accelerated JS ray tracer)

# Use:
# python ./OBJparser.py <obj_file> <txt_file>
############################################################################################
#Read in OBJ text file, produce new text file with vertices array and triangle indices array
import re
import sys

print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)

def parseVertices(obj, txt):
	fyle = open(obj)
	for lyne in fyle:
		m = re.match(r"v(\s*)(-*[0-9]*[.][0-9]*)(\s*)(-*[0-9]*[.][0-9]*)(\s*)(-*[0-9]*[.][0-9]*)", lyne)
		if(m != None):
			x = m.group(2)
			y = m.group(4) 
			z = m.group(6) 
			txt.write("   "+x+", "+y+", "+z+", "+"\n")
	fyle.close 

def parseTriangleIndices(obj, txt):
	fyle = open(obj)
	for lyne in fyle:
		m = re.match(r"f(\s*[0-9]*)//[0-9]*(\s*[0-9]*)//[0-9]*(\s*[0-9]*)//[0-9]*", lyne)
		if(m != None):
			txt.write("   "+m.group(1)+","+m.group(2)+","+m.group(3)+","+"\n")
	fyle.close 


def parseOBJ(obj):
	name = sys.argv[2]
	txt = open(name, 'w+')
	txt.write(name)
	txt.write("\n\n")
	txt.write("var vertices = [\n")

	parseVertices(obj, txt)

	txt.write("]\n")

	txt.write("var triangleIndices = [\n")

	parseTriangleIndices(obj, txt)

	txt.write("]\n")

	txt.close


##########

#parseOBJ("obj/humanoid.obj")
parseOBJ(sys.argv[1])


