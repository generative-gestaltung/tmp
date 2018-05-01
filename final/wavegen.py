import math

len = 1024
PI = 3.14159

string = "WAVEDATA_SIN = {"
for i in range (0,len):

	string += str(int(math.floor(math.sin(i*2*PI/len)*2048+2048)))
	string += ", "
string += "\n}"

print string

