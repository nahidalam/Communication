#! /usr/bin/python

# This is an example of asyncronous receive
# What it actually does is fork off a new process
# to do the XBee receive.  This way, the main
# code can go do somthing else and hand waiting
# for the XBee messages to come in to another
# process.

from xbee import ZigBee
from apscheduler.scheduler import Scheduler
import time
import serial
import Queue

PORT = '/dev/tty.usbserial-A104IC2U'
BAUD_RATE = 9600

# The XBee addresses I'm dealing with
BROADCAST = '\x00\x00\x00\x00\x00\x00\xff\xff'
WHERE = '\x00\x13\xA2\x00\x40\xF7\x0A\x50'
UNKNOWN = '\xff\xfe' # This is the 'I don't know' 16 bit address


packets = Queue.Queue()

# Open serial port
ser = serial.Serial(PORT, BAUD_RATE)


# this is a call back function.  When a message
# comes in this function will get the data
def message_received(data):
        packets.put(data, block=False)
        print 'gotta packet'

def sendPacket(where, what):
        zb.send('tx',
                dest_addr_long = where,
                # I always use the 'unknown' value for this
                # it's too much trouble to keep track of two
                # addresses for the device
                dest_addr = UNKNOWN,
                data = what)

def sendQueryPacket():
        print 'sending query packet'
        sendPacket(BROADCAST, '?\r')

# OK, another thread has caught the packet from the XBee network,
# put it on a queue, this process has taken it off the queue and
# passed it to this routine, now we can take it apart and see
# what is going on ... whew!
def handlePacket(data):
        print 'In handlePacket: ',
        print data['id'],
        if data['id'] == 'tx_status':
                print data['deliver_status'].encode('hex')
        elif data['id'] == 'rx':
                print 'rx type\n'
                print data['source_addr']
                print data['rf_data']
        elif data['id'] == 'rx_long_addr':
                print 'rx_long_addr type\n'
                print data['source_addr']
                print data['rf_data']
        else:
                print 'Unimplemented frame type'



# Create XBee library API object, which spawns a new thread
zb = ZigBee(ser, callback=message_received)

sendsched = Scheduler()
sendsched.start()

# every 30 seconds send a house query packet to the XBee network
sendsched.add_interval_job(sendQueryPacket, seconds=30)

# Do other stuff in the main thread
while True:
        try:
                time.sleep(0.1)
                if packets.qsize() > 0:
                        print 'packet availabe'
                        # got a packet from recv thread
                        # See, the receive thread gets them
                        # puts them on a queue and here is
                        # where I pick them off to use
                        newPacket = packets.get_nowait()
                        # now go dismantle the packet
                        # and use it.
                        handlePacket(newPacket)
        except KeyboardInterrupt:
                break

# halt() must be called before closing the serial
# port in order to ensure proper thread shutdown
zb.halt()
ser.close()
