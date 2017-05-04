############################################################################################
# Read in TXT file that contains vertex and index array.
# Use:
# python ./TXTparser.py <txt_file> <sce_file>
############################################################################################
import re
import sys

print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)


def getVertices(fyle):
	vertexList = list()
	for lyne in fyle:
		m = re.match(r"var vertices = \[", lyne)
		if(m != None):
			break;
	for lyne in fyle:
		n = re.match("]", lyne)
		if(n == None):
			m = re.match(r"(\s*)(-*[0-9]*[.][0-9]*),(\s*)(-*[0-9]*[.][0-9]*),(\s*)(-*[0-9]*[.][0-9]*),", lyne)
			if(m != None):
				x = float(m.group(2))
				y = float(m.group(4)) 
				z = float(m.group(6))
				# print x, y, z
				vertexList.append((x, y, z))
		if(n != None):
			break
	return vertexList

def getIndices(fyle):
	indexList = list()
	for lyne in fyle:
		m = re.match(r"var triangleIndices = \[", lyne)
		if(m != None):
			# print "here"
			break;
	for lyne in fyle:
		n = re.match("]", lyne)
		if(n == None):
			m = re.match(r"(\s*)(-*[0-9]*),(\s*)(-*[0-9]*),(\s*)(-*[0-9]*),", lyne)
			if(m != None):
				x = int(m.group(2))
				y = int(m.group(4)) 
				z = int(m.group(6))
				# print x, y, z
				indexList.append((x, y, z))
		if(n != None):
			# print "here2"
			break
	return indexList


def parseTXT(input_file):
	# Parse input TXT
	txt = open(input_file)
	vertexList = getVertices(txt)
	indexList = getIndices(txt)
	txt.close

	# Create output file (.sce)
	output_file = sys.argv[2]
	sce = open(output_file, 'w+')

	#iterate through index array, three at a time
	#adjust index by subtracting 0
	#generate a line in TRIANGLE P1, P2, P3
	for (p1, p2, p3) in indexList:
		p1x = vertexList[p1-1][0]
		p1y = vertexList[p1-1][1]
		p1z = vertexList[p1-1][2]
		p2x = vertexList[p2-1][0]
		p2y = vertexList[p2-1][1]
		p2z = vertexList[p2-1][2]
		p3x = vertexList[p3-1][0]
		p3y = vertexList[p3-1][1]
		p3z = vertexList[p3-1][2]

		strP1 = "P1("+str(p1x)+", "+str(p1y)+", "+str(p1z)+")"
		strP2 = "P2("+str(p2x)+", "+str(p2y)+", "+str(p2z)+")"
		strP3 = "P3("+str(p3x)+", "+str(p3y)+", "+str(p3z)+")"

		line = "TRIANGLE " + strP1 + " " + strP2 + " " + strP3 + "\n"
		sce.write(line)

	sce.close


####
parseTXT(sys.argv[1])


