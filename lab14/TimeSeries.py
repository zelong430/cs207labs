import numpy as np
from lazy import *

class TimeSeries():
    '''
    """
Help on package TimeSeries:

NAME
    TimeSeries

DESCRIPTION
    TimeSeries
    =====
    
    Provides
      1. An sequence or any iterable objects
    
    How to use the documentation
    ----------------------------
    Documentation is available in two forms: docstrings provided
    with the code, and a loose standing reference guide, available from
    `the TimeSeries homepage <https://github.com/cs207-project>`_.
    
    We recommend exploring the docstrings using
    `IPython <http://ipython.scipy.org>`_, an advanced Python shell with
    TAB-completion and introspection capabilities.  See below for further
    instructions.
    
    The docstring examples assume that `numpy` has been imported as `np`::  
      
    
    
     |  Methods inherited from builtins.RuntimeWarning:
     |  
     |  __init__(self, *args, **kwargs)
     |      Initialize self.  See help(type(self)) for accurate signature.
     |      Stors a TimeSeries in self.TimeSeries_
     |    
     |  __repr__(self, /)
     |      Return a printable sequence shown in python list format containing all values in [self].
     |  
     |  __str__(self, /)
     |      Return a printable abbreviated sequence of maximum first 100 entrees.
     |  
     |  __getitem__(self, index)
     |      Return self[index]
     |
     |  __setitem__(self, index, values)
     |      Set self[index] = values
     |
     |  __len__(self)
     |      Return len(self.TimeSeries_)
     '''
    def __init__(self, times, values):
        if (iter(times) and iter(values)):
            # reorder according to Time step
            idx = np.argsort(times)
            times = np.array(times)[idx]
            values = np.array(values)[idx]

            self._TimeSeries=np.vstack((times,values))
            self._vindex = 0
            self._values = self._TimeSeries[1]
            self._times = self._TimeSeries[0]
    
    @property
    @lazy
    def lazy(self):
        return self

    def itervalues(self):
        for v in self._values:
            yield v

    def itertimes(self):
        for t in self._times:
            yield t

    def iteritems(self):
        for t,v in zip(self._times,self._values):
            yield (t,v)
            
    def __len__(self):
        return len(self._TimeSeries[0])
    
    def __contains__(self, time):
        index = np.where(self._TimeSeries[0]==time)
        return index[0].size>0
            
    
    def __getitem__(self,time):
        if (time in self):
            index = np.where(self._TimeSeries[0]==time)
            return self._TimeSeries[1][index]
        else:
            print ("no time point at t={0}".format(time))

    def __setitem__(self,time,value):
        if (time in self):
            index = np.where(self._TimeSeries[0]==time)
            self._TimeSeries[1][index]=value
        else:
            print ("no time point at t={0}".format(time))
            
    def __iter__(self):
        return iter(self._TimeSeries[1])
    
    def __repr__(self):
        return "%r"%(self._TimeSeries)
    
    def __str__(self):
        className = type(self).__name__
        if len(self)>100:
            return "%s" %('['+(str(self._values[:99]))[1:-1]+'...'+']')
        else:
            return "%s" %(self._TimeSeries)
        
    def __eq__(self, other):
        return np.array_equal(self._TimeSeries, other._TimeSeries)
        
    def values(self):
        return self._values
    
    def times(self):
        return self._times
    
    def mean(self):        
        if(len(self._values) == 0):
            raise ValueError("cant calculate mean of length 0 list")
        return np.mean(self._values)
    
    def median(self):
        if(len(self._values) == 0):
            raise ValueError("cant calculate median of length 0 list")
        return np.median(self._values)
    
    def interpolate(self, times):
        new_values = []
        for time in times:
            if time > self._times[-1]: # over the rightest boundary
                new_values.append(self._values[-1])
            elif time < self._times[0]: # over the leftest boundary
                new_values.append(self._values[0])
            elif time in self._times:
                new_values.append(self.__getitem__(time))
            else : #within boundary
                for i in range(len(self._times)):
                    if self._times[i] > time:
                        left_value = self._values[i-1]
                        right_value = self._values[i]
                        left_time = self._times[i-1]
                        right_time = self._times[i]
                        #interpolate
                        new_values.append(left_value + (right_value - left_value)/(right_time - left_time)*(time - left_time))
                        break
        return TimeSeries(times, new_values)
