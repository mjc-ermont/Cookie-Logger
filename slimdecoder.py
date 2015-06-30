# -*- coding: utf-8 -*-

from reedsolo import RSCodec
import functools
import operator
import struct

def slimdecode(frame, config): 
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
		val, =  struct.unpack("!" + fmt, frame[:l])
		frame = frame[l:]
		out.append(val)


	print(len(frame))

	return out
