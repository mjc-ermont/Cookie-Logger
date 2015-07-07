# -*- coding: utf-8 -*-

import numpy as np
import functools
import operator
import struct
from reedsolo import RSCodec



def decode(data):
	print("decoding "+str(data))
	return (True,np.array([1.,2.,3.,4.,5.]))
