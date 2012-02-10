import numpy as np
import scipy as sp
import matplotlib.pyplot as pp
import aer_helper as ah
import time
from aer import *

"""
	* Bipolar model
	BP(x, y, t) = max( sum( p * ST(x-xi, y-yi) * TT(t-ti), i ), 0)
	where i loops over the "nearby" events
	- ST is the Spatial Transformation represented by a matrix
	The size of the matrix is used to define what "nearby" means
	- TT is the Time Transformation defined in bipolar_compute()
	The time window for the computation is defined in the parameter maxT
	that is passed to bipolar_event()
"""

class BPCell:
	""" A bipolar cell """

	def __init__(self):
		self.u = 0
		self.v = 0
		self.last_t = 0
		self.tau = 20000

	def addEvent(self, e_v, e_t):
		assert(e_t >= self.last_t)

		delta_t = e_t - self.last_t
		expd = np.exp(-delta_t / self.tau)

		self.v = expd * (self.v + delta_t*self.u)
		self.u = expd * self.u + e_v

		self.last_t = e_t


	def compute(self, t):
		assert(t >= self.last_t)

		val = self.v
		if (t > self.last_t):
			delta_t = t - self.last_t
			expd = np.exp(-delta_t / self.tau)

			val = expd * (self.v + delta_t*self.u)

		if (val > 0):
			return val
		else:
			return 0



class BPLayer:
	""" A bipolar layer """

	def __init__(self, width, ST):
		self.cells = []
		self.ST = ST
		for i in range(width):
			line_of_cells = []
			self.cells.append(line_of_cells)
			for j in range(width):
				line_of_cells.append(BPCell())


	def addEvent(self, e):
		width = len(self.cells)
		STwidth = self.ST.shape[0]
		shift = int(np.floor(STwidth / 2))
		for x in range(STwidth):
			for y in range(STwidth):
				bipolarX = e.x - shift + x
				bipolarY = e.y - shift + y
				# Check the boundaries
				if (bipolarX >= 0 and bipolarX < width and bipolarY >= 0 and bipolarY < width):
					cell = self.cells[e.x - shift + x][e.y - shift + y]
					cell.addEvent(self.ST[x, y] * e.p, e.t)


	def show(self, t, fign):
		width = len(self.cells)
		temp = np.zeros(shape=(width, width), dtype=float)
		for x in range(width):
			for y in range(width):
				temp[x, y] = self.cells[x][y].compute(t)
		pp.matshow(temp, fignum=fign)
		pp.draw()



def diff_gauss_kern(size = 2, sigmA = 0.9, sigmB = 1., ratio = 1.):
	""" Returns a normalized 2D kernel of a gaussian difference """
	x, y = sp.mgrid[-size:size+1, -size:size+1]
	sigmA = float(sigmA**2)
	sigmB = float(sigmB**2)
	g = sp.exp( -(x**2 + y**2) / sigmA ) - ratio * sp.exp( -(x**2 + y**2) / sigmB )
	return g / abs(g.sum())



reader = FileEvent2dReader('ntest3.dat')


# Size of the BP layers
retina_size = 128

# Init of the layers
STone   = diff_gauss_kern(5, 0.9, 1, 1)
SToffe  = diff_gauss_kern(5, 1, 0.9, 1)

bpONe   = BPLayer(retina_size, STone)
bpOFFe  = BPLayer(retina_size, SToffe)

i = 0

while reader.hasNext():
	i += 1

	e = reader.readEvent2d()
	print(e.toString())

	bpONe.addEvent(e)
	bpOFFe.addEvent(e)


	if i % 1000 == 0:
		bpONe.show(e.t, 1)
		bpOFFe.show(e.t, 2)
		pp.show(block=False)

	#raw_input("...")
