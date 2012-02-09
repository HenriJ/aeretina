import numpy as np
import scipy as sp
import matplotlib.pyplot as pp
import aer_helper as ah
import time

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

class Event2d:
	""" A 2d aer event """
	x = 0
	y = 0
	p = 0
	t = 0


	def __init__(self, x, y, p, t):
		 self.x = x
		 self.y = y
		 self.p = p
		 self.t = t



class BPCell:
	""" A bipolar cell """
	u = 0
	v = 0 # The actual potential
	last_t = 0 # Time of the last event
	tau = 20000 # 


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
	cells = []
	ST = None


	def __init__(self, width, ST):
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


# Load datas
addrs, ts = ah.loadaerdat('ntest3.dat', 10000)
aer = []
for i in range(len(addrs)):
	x, y, p = ah.extractRetinaEventsFromAddr_one(addrs[i])
	aer.append(Event2d(x, y, p, ts[i] - ts[0]))


# Size of the BP layers
retina_size = 128

# Init of the layers
STone  = diff_gauss_kern(5, 0.2, 1, 1)
BPone  = BPLayer(retina_size, STone)

i = 0

for e in aer:
	BPone.addEvent(e)
	i += 1
	if i % 10 == 0:
		print(str(i) + " " + str(e.t))
		BPone.show(e.t, 1)
		pp.show(block=False)
	#raw_input("Press Enter to continue...")