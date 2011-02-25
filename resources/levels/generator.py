import random

print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
print "<map>"

x, y, z = 0, 0, 0
types = ["accelerate", "decelerate"]
jump_x_range = [-7, 7]
jump_y_range = [0, 30]
jump_z_range = [-3, 3]

for i in range(1, 100):

	length = random.randint(20, 100)
	height = 0.3
	width = random.randint(2, 5)

	print "<cuboid type=\"%s\">" % (types[random.randint(0,1)],)
	print "\t<position x=\"%f\" y=\"%d\" z=\"%d\" />" % (x - (width / 2.0), y, z)
	print "\t<size x=\"%d\" y=\"%d\" z=\"%f\" />" % (width, length, height)	
	print "</cuboid>"

	x += random.randint(jump_x_range[0], jump_x_range[1])
	y += 30 + random.randint(jump_y_range[0], jump_y_range[1])
	z += height + random.randint(jump_z_range[0], jump_z_range[1])
	

print "</map>"
