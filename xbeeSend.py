#!/usr/bin/python


#this code runs on the xbee coordinator that is set to API mode 2

import serial
from xbee import ZigBee
from xbee.helpers.dispatch import Dispatch
import time

PORT = '/dev/tty.usbserial-A104IC2U'
BAUD_RATE = 9600

UNKNOWN = '\xff\xfe' 
WHERE = '\x00\x13\xA2\x00\x40\xF7\x0A\x50'
#WHERE='0013A20040F70A50'
dataString='Hello\n'

# Open serial port
ser = serial.Serial(PORT, BAUD_RATE)

zb = ZigBee(ser)

#sends data to xbee address
def sendData(address, datatosend):
	zb.send('tx', dest_addr_long = address, dest_addr = UNKNOWN, data = datatosend)


#test data sending method
while True:
	try:
		sendData(WHERE, dataString)
	except KeyboardInterrupt:
		break
	

zb.halt()
ser.close()