CAMERA Point(50, 50, 200) LookAt(50, 25, 0) FOV(60) Up(UP)

LIGHT Type(OMNI) Point(50, 95, 50) Color(155, 155, 155)
LIGHT Type(OMNI) Point(5, 95, 100) Color(255, 220, 200)
LIGHT Type(OMNI) Point(95, 5, 100) Color(50, 50, 100)

MATERIAL Color(255, 255, 255) Type(ORIGINAL) Metal(0) Specular(0) Lambert(0.85) Ambient(0.05) Exponent(0)
MATERIAL Color(255, 255, 255) Type(ORIGINAL) Metal(0) Specular(0) Lambert(0.9) Ambient(0.05) Exponent(0)
MATERIAL Color(255, 90, 90) Type(ORIGINAL) Metal(0) Specular(0) Lambert(0.9) Ambient(0.1) Exponent(0)
MATERIAL Color(90, 90, 255) Type(ORIGINAL) Metal(0) Specular(0) Lambert(0.9) Ambient(0.1) Exponent(0)
MATERIAL Color(255, 255, 255) Type(ORIGINAL) Metal(0) Specular(0.9) Lambert(0.1) Ambient(0) Exponent(0)
MATERIAL Color(200, 170, 60) Type(PHONG) Metal(1) Specular(0.5) Lambert(0.4) Ambient(0.1) Exponent(2)


#Specified in degrees
SPHERE Point(70, 25, 50) Radius(25) Material(2) RotateZ(30) ScaleR(0.5) Translate(20, 20, 20)
#SPHERE Point(70, 25, 50) Radius(25) Material(2)
#SPHERE Point(20, 10, 50) Radius(10) Material(3)
#SPHERE Point(50, 10, 25) Radius(10) Material(1)


TRIANGLE P1(100, 0, 0) P2(100, 100, 0) P3(0, 100, 0) Material(1)
TRIANGLE P1(0, 0, 0) P2(0, 0, 100) P3(100, 0, 100) Material(1)
TRIANGLE P1(100, 0, 100) P2(100, 0, 0) P3(0, 0, 0) Material(1)
TRIANGLE P1(0, 100, 0) P2(100, 100, 0) P3(100, 100, 100) Material(1)
TRIANGLE P1(100, 100, 100) P2(0, 100, 100) P3(0, 100, 0) Material(1)
TRIANGLE P1(0, 0, 0) P2(0, 100, 0) P3(0, 100, 100) Material(2)
TRIANGLE P1(0, 100, 100) P2(0, 0, 100) P3(0, 0, 0) Material(2)
TRIANGLE P1(100, 0, 0) P2(100, 0, 100) P3(100, 100, 100) Material(3)
TRIANGLE P1(100, 100, 100) P2(100, 100, 0) P3(100, 0, 0) Material(3)


#INCLUDE Mesh(sce/humanoid.sce) Material(2) Scale(3,3,3) RotateY(20) Translate(50, 10, 50) 
INCLUDE Mesh(sce/bunny.sce) Material(2) 