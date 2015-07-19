# -*- coding: utf-8 -*-

from reedsolo import RSCodec
import functools
import operator
import struct
import json
import os

import numpy as np



config_file = "slimdecoder_conf.json"
config = json.load(open(config_file, "r"))

def decode(data):
	print("Config")
	print(config)

	print("Decoding "+str(data))

	res = slimdecode(data)
	print(res)

	if(res != False):
		float_res = []

		for datatuple in res:
                	datalist = list(datatuple)
                	datalist = [float(x) for x in datalist]
                	datatuple = tuple(datalist)
                	float_res.append(datatuple)


		return (True, np.array(float_res))
	else:
		return (False, 0)

def calcframelength():
	global config
	fl = 2
	for fmt in config["sensors"]: 
		fl += struct.calcsize(fmt)
	fl += config["trame"]["ecc"]["length"]
	return fl
	


def slimdecode(frame): 
	global config

	print(len(frame))
	rs = RSCodec(config["trame"]["ecc"]["length"])
	frame = rs.decode(frame)

	cs = frame.pop()

	if cs != functools.reduce(operator.xor, frame): 
		print("Checksum failed")
		return False

	frame.pop(0) # Stripping start byte

	out = []

	for fmt in config["sensors"]: 
		l = struct.calcsize(fmt)
		val =  struct.unpack("!" + fmt, frame[:l])
		frame = frame[l:]
		out.append(val)



	return out

