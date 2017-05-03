FILM Width(512) Height(512)

CAMERA Point(50, 50, 200) LookAt(50, 25, 0) FOV(60) Up(UP) LensRadius(0.0) FocalDepth(50)

#LIGHT Type(SPOT) Point(50, 95, 50) Color(155, 155, 155) ToPoint(50, 50, 50) Angle(15) FallOffAngle(10)
LIGHT Type(OMNI) Point(50, 95, 50) Color(180, 180, 180) 
#LIGHT Type(SPHERICAL) Point(50, 95, 50) Color(180, 180, 180) Radius(3)
#LIGHT Type(OMNI) Point(5, 95, 100) Color(255, 220, 200)
#LIGHT Type(OMNI) Point(95, 5, 100) Color(50, 50, 100)

#0
MATERIAL Color(255, 255, 255) Type(ORIGINAL) Metal(0) Specular(0) Lambert(0.85) Ambient(0.05) Exponent(0)
#1
MATERIAL Color(255, 255, 255) Type(ORIGINAL) Metal(0) Specular(0) Lambert(0.9) Ambient(0.05) Exponent(0)
#2
MATERIAL Color(255, 90, 90) Type(ORIGINAL) Metal(0) Specular(0) Lambert(0.9) Ambient(0.1) Exponent(0)
#3
MATERIAL Color(90, 90, 255) Type(ORIGINAL) Metal(0) Specular(0) Lambert(0.9) Ambient(0.1) Exponent(0)
#4
MATERIAL Color(255, 90, 90) Type(ORIGINAL) Metal(0) Specular(0.1) Lambert(0.9) Ambient(0) Exponent(0)
#5
MATERIAL Color(200, 170, 60) Type(PHONG) Metal(1) Specular(0.5) Lambert(0.4) Ambient(0.1) Exponent(2)


SPHERE Point(70, 25, 50) Radius(25) Material(5)
SPHERE Point(20, 10, 50) Radius(10) Material(3)
#SPHERE Point(50, 10, 25) Radius(10) Material(1)


TRIANGLE P1(0, 0, 0) P2(100, 0, 0) P3(0, 100, 0) Material(1)
TRIANGLE P1(100, 0, 0) P2(100, 100, 0) P3(0, 100, 0) Material(1)
TRIANGLE P1(0, 0, 0) P2(0, 0, 100) P3(100, 0, 100) Material(1)
TRIANGLE P1(100, 0, 100) P2(100, 0, 0) P3(0, 0, 0) Material(1)
TRIANGLE P1(0, 100, 0) P2(100, 100, 0) P3(100, 100, 100) Material(1)
TRIANGLE P1(100, 100, 100) P2(0, 100, 100) P3(0, 100, 0) Material(1)
TRIANGLE P1(0, 0, 0) P2(0, 100, 0) P3(0, 100, 100) Material(2)
TRIANGLE P1(0, 100, 100) P2(0, 0, 100) P3(0, 0, 0) Material(2)
TRIANGLE P1(100, 0, 0) P2(100, 0, 100) P3(100, 100, 100) Material(3)
TRIANGLE P1(100, 100, 100) P2(100, 100, 0) P3(100, 0, 0) Material(3)

BOUNDINGBOX Min(-500, -500, -500) Max(500, 500, 500)
