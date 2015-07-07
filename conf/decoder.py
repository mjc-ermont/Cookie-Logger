# -*- coding: utf-8 -*-

from reedsolo import RSCodec
import functools
import operator
import struct
import json
import os
import trame
import numpy as np



def decode(data):
	config_file = "slimdecoder_conf.json"


	conf = json.load(open(config_file, "r"))
	print("Decoding "+str(data))

	


	res = slimdecode(data, config_file)
	print(res)
	if(res != False):
		return (True, np.array(res))
	else:
		return (False, 0)


def slimdecode(frame, config_file): 
	config = json.load(open(config_file, "r"))

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

