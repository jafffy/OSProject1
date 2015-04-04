#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np

x = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
y = []
e = []

for i in range(1, 10):
	print i
	f = open('statistics' + str(i), 'r')

	data = f.readline()
	print float(data)
	y = y + [float(data)]
	data = f.readline()
	print float(data)
	e = e + [float(data)]

y2 = np.array(y)
e2 = np.array(e)

plt.errorbar(x, y2, e2, linestyle='None', marker='^')

plt.show()