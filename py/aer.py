import os

class Event2d:
	""" A 2d aer event """
	def __init__(self, x, y, p, t):
		 self.x = x
		 self.y = y
		 self.p = p
		 self.t = t

	def toString(self):
		return 'Event2d: (' + str(self.x) + ', ' + str(self.y) + ', ' + str(self.p) + ', ' + str(self.t) + ')'



class Event2dReader:
	"""Event2dReader is an abstract class representing a way to read event from a source"""

	def hasNext(self):
		raise "Not implemented"


	def readEvent2d(self):
		raise "Not implemented" 


	def bufferToEvent2d(self, buf):
		i = 0

		part_1 = 0x000000FF&ord(buf[i])
		part_2 = 0x000000FF&ord(buf[i+1])
		part_3 = 0x000000FF&ord(buf[i+2])
		part_4 = 0x000000FF&ord(buf[i+3])

		blob = (part_4)|(part_3<<8)|(part_2<<16)|(part_1<<24)

		x = (blob & 0x000000fE)>>1
		y = (blob & 0x00007f00)>>8
		if (((blob & 0x00000001)>>0)==0):
			p = -1
		else:
			p = 1

		part_5 = 0x000000FF&ord(buf[i+4])
		part_6 = 0x000000FF&ord(buf[i+5])
		part_7 = 0x000000FF&ord(buf[i+6])
		part_8 = 0x000000FF&ord(buf[i+7])

		t = (part_8)|(part_7<<8)|(part_6<<16)|(part_5<<24)

		return Event2d(x, y, p, t)



class FileEvent2dReader(Event2dReader):
	"""This class allows you to sequentially read the events from an aer dump file"""

	def __init__(self, path):
		self.f = open(path, 'r')
		self.shift_t = 0
		self.last_t  = 0

		while(self.f.read(1) == "#"):
			self.f.readline()

		self.f.seek(-1, os.SEEK_CUR)

		e = self.rawEvent2d()

		self.shift_t = -e.t
		self.last_t = e.t


	def hasNext(self):
		if (len(self.f.read(1)) == 1):
			self.f.seek(-1, os.SEEK_CUR)
			return True
		return False

	
	def rawEvent2d(self):
		return self.bufferToEvent2d(self.f.read(8))


	def readEvent2d(self):
		buf = self.f.read(8)

		e = self.bufferToEvent2d(buf)

		if (e.t < self.last_t):
			self.shift_t += self.last_t

		self.last_t = e.t

		e.t += self.shift_t

		return e;
