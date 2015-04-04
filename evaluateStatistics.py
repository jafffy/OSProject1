#!/usr/bin/python
import math

for i in range(1, 10):
	f = open('timestamp' + str(i), 'r')
	o = open('statistics' + str(i), 'w')

	mean = 0.0
	stddev = 0.0
	numberOfTimeStamp = 0

	for line in f:
		mean 	+= float(line)
		stddev	+= float(line)*float(line)
		numberOfTimeStamp += 1

	mean /= numberOfTimeStamp
	stddev /= numberOfTimeStamp
	stddev += mean*mean
	stddev = math.sqrt(stddev)

	o.write(str(mean) + '\n')
	o.write(str(stddev) + '\n')
