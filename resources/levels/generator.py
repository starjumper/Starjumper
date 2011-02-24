import random

print "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
print "<map>"

x, y, z = 0, 0, 0
jump_x_range = [-10, 10]
jump_y_range = [0, 30]
jump_z_range = [-4, 4]

for i in range(1, 30):

	length = random.randint(20, 100)
	height = 1.1
	width = random.randint(2, 5)

	print "<cuboid>"
	print "\t<position x=\"%f\" y=\"%d\" z=\"%d\" />" % (x - (width / 2.0), y, z)
	print "\t<size x=\"%d\" y=\"%d\" z=\"%d\" />" % (width, length, height)	
	print "</cuboid>"

	x += random.randint(jump_x_range[0], jump_x_range[1])
	y += length + random.randint(jump_y_range[0], jump_y_range[1])
	z += height + random.randint(jump_z_range[0], jump_z_range[1])
	

print "</map>"
