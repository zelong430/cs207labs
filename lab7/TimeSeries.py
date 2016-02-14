# class named TimeSeries that stores a single, ordered set of numerical data
class TimeSeries:

	def __init__(self, val):
		self.__collection = []
		for i in val:
			self.__collection.append(val)

	def __len__(self):
		return len(self.collection)

	def __getitem__(self, index):
		if index < 0 or index > self.__len__:
			return None
		else:
			return self.collection[index]

	def __setitem__(self, index, val):
		if index < 0 or index > self.__len__:
			return None
		else:
			return self.collection[index] = val	

	def __repr__(self):
		if self.__len__ < 6:
			return "%d Elements in this TimeSeries: " + self.collection.__repr__ % self.__len__
		else 
			return "%d Elements in this TimeSeries: [%f, %f, %f ... %f, %f,%f]" % (self.__len__, self.collection[0],self.collection[1],self.collection[2], self.collection[-3],self.collection[-2],self.collection[-1])


