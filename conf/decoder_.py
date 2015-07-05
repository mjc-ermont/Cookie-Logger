# -*- coding: utf-8 -*-

from reedsolo import RSCodec
import functools
import operator
import struct



def decode(data):
	config_file = "slimdecoder_conf.json"




def slimdecode(frame, config): 
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
