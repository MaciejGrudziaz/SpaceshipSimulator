from sys import *

xMax = input("max x: ")
yMax = input("max y: ")
zMax = input("max z: ")
xMin = input("min x: ")
yMin = input("min y: ")
zMin = input("min z: ")

class Vec:
	def __init__(self, x, y, z):
		self.x = x
		self.y = y
		self.z = z
	def write(self):
		print("("+self.x+", "+self.y+", "+self.z+")")

minVec = Vec(xMin, yMin, zMin)
maxVec = Vec(xMax, yMax, zMax)

controlPt = []

controlPt.append(Vec(minVec.x, minVec.y, minVec.z))
controlPt.append(Vec(maxVec.x, minVec.y, minVec.z))
controlPt.append(Vec(maxVec.x, maxVec.y, minVec.z))
controlPt.append(Vec(minVec.x, maxVec.y, minVec.z))

controlPt.append(Vec(minVec.x, minVec.y, maxVec.z))
controlPt.append(Vec(maxVec.x, minVec.y, maxVec.z))
controlPt.append(Vec(maxVec.x, maxVec.y, maxVec.z))
controlPt.append(Vec(minVec.x, maxVec.y, maxVec.z))

indices = [ 0, 1, 1, 2, 2, 3, 3, 0,
		    0, 4, 1, 5, 2, 6, 3, 7,
			4, 5, 5, 6, 6, 7, 7, 4 ]

normals = [ 0, 0, -1,
			0, 0, 1,
			1, 0, 0,
			0, 1, 0,
			-1, 0, 0,
			0, -1, 0 ]

filename = input("output file: ")
file = open(filename,"w")
file.write("vertices\n")

for vec in controlPt:
	file.write(vec.x+" "+vec.y+" "+vec.z+"\n")

file.write("indices\n")

cnt = 0
for i in indices:
	if cnt%2 == 0:
		file.write(str(i)+" ")
	else:
		file.write(str(i)+"\n")
	cnt += 1

file.write("normals\n")
cnt = 0
for n in normals:
	if cnt%3 == 2:
		file.write(str(n)+"\n")
	else: 
		file.write(str(n)+" ")
	cnt+=1

file.close()