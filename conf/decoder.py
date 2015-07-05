# -*- coding: utf-8 -*-

from reedsolo import RSCodec
import functools
import operator
import struct
import numpy as np



def decode(data):
	print("decoding "+str(data))
	return (True,np.array([1.,2.,3.,4.,5.]))
