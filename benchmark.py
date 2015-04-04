#!/usr/bin/python
from subprocess import call

for i in range(1, 10):
	for j in range(10000):
		call(["./a.out", str(10 - i)])
