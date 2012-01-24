import numpy as np
import scipy as sp
import matplotlib.pyplot as pp
import aer_helper as ah
import time

"""
	* Why not using class to represent the layers ?
	This is only a prototyping code in order to prove that
	this approach can work. Eventually this code should be
	translated to C, OpenCL or any "fast" language. Most
	of them not supporting OO-programming, we don't use it
	here. In C, the Event class would be only a struct.

	* Bipolar model
	BP(x, y, t) = max( sum( p * ST(x-xi, y-yi) * TT(t-ti), i ), 0)
	where i loops over the "nearby" events
	- ST is the Spatial Transformation represented by a matrix
	The size of the matrix is used to define what "nearby" means
	- TT is the Time Transformation defined in bipolar_compute()
	The time window for the computation is defined in the parameter maxT
	that is passed to bipolar_event()
"""

class Event:
	""" Represents an aer event """
	x = 0
	y = 0
	p = 0
	t = 0
	def __init__(self, x, y, p, t):
         self.x = x
         self.y = y
         self.p = p
         self.t = t

def diff_gauss_kern(size = 2, sigmA = 0.9, sigmB = 1., ratio = 1.):
	""" Returns a normalized 2D kernel of a gaussian difference """
	x, y = sp.mgrid[-size:size+1, -size:size+1]
	sigmA = float(sigmA**2)
	sigmB = float(sigmB**2)
	g = sp.exp( -(x**2 + y**2) / sigmA ) - ratio * sp.exp( -(x**2 + y**2) / sigmB )
	return g / abs(g.sum())

def bipolar_init(BP, width):
	""" Create a 2d array, each cell containing an empty list representing a BP layer """
	for i in range(width):
		t = []
		BP.append(t)
		for j in range(width):
			t.append([])

def bipolar_event(BP, ST, e, maxT):
	""" Feed an event to a BP layer """
	width = len(BP)
	STwidth = ST.shape[0]
	shift = int(np.floor(STwidth / 2))
	for x in range(STwidth):
		for y in range(STwidth):
			bipolarX = e.x - shift + x
			bipolarY = e.y - shift + y
			# Check the boundaries
			if (bipolarX >= 0 and bipolarX < width and bipolarY >= 0 and bipolarY < width):
				cell = BP[e.x - shift + x][e.y - shift + y]
				# TODO: convert the list to a heap to avoid extensive search of outdated datas
				for d in cell:
					if (e.t - d[1] >= maxT):
						cell.remove(d)
				cell.append( (ST[x, y]*e.p, e.t) )

def bipolar_compute(BP, x, y, tau, t):
	temp = 0
	for e in BP[x][y]:
		temp += e[0] * (t - e[1]) * np.exp(-(t - e[1]) / tau)
	return max(temp, 0)

def bipolar_show(BP, t, fign):
	width = len(BP)
	temp = np.zeros(shape=(width, width), dtype=float)
	maxLen = 0;
	for x in range(width):
		for y in range(width):
			temp[x, y] = bipolar_compute(BP, x, y, 50000., t)
			if (len(BP[x][y]) > maxLen):
				maxLen = len(BP[x][y])
	print(maxLen)
	pp.matshow(temp, fignum=fign)
	pp.draw()


		
# Size of the BP layers
width = 128

# Init of the layers
BPone  = []
BPoni  = []
BPoffe = []
BPoffi = []
bipolar_init(BPone,  width)
bipolar_init(BPoni,  width)
bipolar_init(BPoffe, width)
bipolar_init(BPoffi, width)

# Defining the spatial transformations of an event to the BP layer
STone  = diff_gauss_kern(5, 0.2, 1, 1)
SToni  = diff_gauss_kern(3, 0.9, 1, 1)
SToffe = diff_gauss_kern(5, 1, 0.9, 1)
SToffi = diff_gauss_kern(3, 1, 0.9, 1)


# Read and convert the aer file
addrs, ts = ah.loadaerdat('demo.aer', 10000)
aer = []
for i in range(len(addrs)):
	x, y, p = ah.extractRetinaEventsFromAddr_one(addrs[i])
	aer.append(Event(x, y, p, ts[i] - ts[0]))

# Launch simu
delay = 10
maxT  = 200000


bipolar_show(BPone, 1, 1)
bipolar_show(BPoffe, 1, 2)
pp.show(block=False)


i = 0
sumTime = 0
temp = time.time()
for e in aer:
	

	delayed_e = Event(e.x, e.y, e.p, e.t + delay)

	bipolar_event(BPone,  STone,  e, maxT)
	#bipolar_event(BPoni,  SToni,  delayed_e, 100)
	bipolar_event(BPoffe, SToffe, e, maxT)
	#bipolar_event(BPoffi, SToffi, delayed_e, 100)

	i = i + 1
	if (i % 100 is 0):
		sumTime += time.time() - temp

		print(str(i) + "evt, " + str(e.t) + "us, " + str(sumTime / i) + "s/evt")
		temp = time.time()

	if (i % 100 is 0):
		bipolar_show(BPone, e.t, 1)
		bipolar_show(BPoffe, e.t, 2)

