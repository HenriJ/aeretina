import numpy as np
import scipy as sp
import matplotlib.pyplot as pp
import time
from aer import *

def gauss_kern(size = 2, sigma = 1):
	""" Returns a normalized 2D kernel of a gaussian """
	x, y = sp.mgrid[-size:size+1, -size:size+1]
	sigma = float(sigma**2)
	g = sp.exp( -(x**2 + y**2) / sigma )
	return g / abs(g.sum())

def diff_gauss_kern(size = 2, sigmA = 0.9, sigmB = 1., ratio = 1.):
	""" Returns a normalized 2D kernel of a gaussian difference """
	x, y = sp.mgrid[-size:size+1, -size:size+1]
	sigmA = float(sigmA**2)
	sigmB = float(sigmB**2)
	g = sp.exp( -(x**2 + y**2) / sigmA ) - ratio * sp.exp( -(x**2 + y**2) / sigmB )
	return g / abs(g.sum())



a = np.zeros((1, 200000), dtype='float64');