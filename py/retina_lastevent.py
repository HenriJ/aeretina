import numpy as np
import scipy as sp
import matplotlib.pyplot as pp

tMax = 1000000 #1s

BPcell = []
# Random events
for i in range(tMax):
	if np.random.random_integers(0, 1000000) > 999978:
		BPcell.append((np.random.random() * 2 - 1, i))
		# 1 evt/ms

deltaT = 1000
# Launch simu
def bpshow(cell, maxT, tau):
	tau = float(tau)
	vals = []
	for t in range(0, tMax, deltaT):
		temp = 0
		for e in cell:
			if e[1] > t:
				break
			elif e[1] < t - maxT:
				continue
			else:
				temp += e[0] * ((t - e[1]) / tau) * np.exp(-(t - e[1]) / tau)
		vals.append(temp)

		if (t % 10000 is 0):
			print(t)

	return vals

cols = ['k', 'b', 'g', 'r', 'c', 'm', 'y', 'w']

x = range(0, tMax, deltaT)

pp.hold(True)

y = bpshow(BPcell, 1*20000, 20000)
pp.plot(x, y, 'k')

y = bpshow(BPcell, 5*20000, 20000)
pp.plot(x, y, 'r')

y = bpshow(BPcell, 9*20000, 20000)
pp.plot(x, y, 'b')

y = bpshow(BPcell, 10*20000, 20000)
pp.plot(x, y, 'g')

pp.hold(False)

pp.show()
	
