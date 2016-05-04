#!/usr/bin/python

import serial

ser = serial.Serial('/dev/tty.usbserial-A104IC2U', 9600)
ack='A'

while True:
	incoming = ser.readline().strip()
	if incoming != 'A':
		print '%s' % incoming
		ser.write('%s\n' % ack)
