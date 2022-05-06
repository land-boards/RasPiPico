import machine
import time

# Create I2C object
i2c = machine.I2C(0, scl=machine.Pin(17), sda=machine.Pin(16), freq=400000)

# Print out any addresses found
# devices = i2c.scan()
# 
# if devices:
#     print("I2C device(s) found at: ",end='')
#     for d in devices:
#         print(hex(d),end=' ')
#     print()
# else:
#     print("No I2C devices found")

MCP23008_BASEADDR=0x20

MCP23008_IODIR=0x00
MCP23008_IPOL=0x01
MCP23008_GPINTEN=0x02
MCP23008_DEFVAL=0x03
MCP23008_INTCON=0x04
MCP23008_IOCON=0x05
MCP23008_GPPU=0x06
MCP23008_INTF=0x07
MCP23008_INTCAP=0x08
MCP23008_GPIO=0x09
MCP23008_OLAT=0x0A

INPUT=0x0
OUTPUT=0x1
INPUT_PULLUP=0x2

outdata=bytearray(b'\x00') 				# IOCON value lowest bit output
i2c.writeto_mem(MCP23008_BASEADDR,MCP23008_IOCON,outdata) 		# Set the IOCON to output for lowest bit

def readRegister(regAdr):
	#print("readRegister() - regAdr=",regAdr)
	readbackVal=bytearray(1)	# Allow buffer space
	i2c.readfrom_mem_into(MCP23008_BASEADDR,regAdr,readbackVal)	# Do the read
	rwValue=readbackVal[0]	# Pull the first byte
	#print("readRegister() - rwValue=",rwValue)
	return rwValue				# Return value

def writeRegister(regAdr,wrValue):
	#print("writeRegister() - regAdr=",regAdr)
	#print("writeRegister() - wrValue=",wrValue)
	outBuff=bytearray(1)
	outBuff[0]=wrValue
	#print("writeRegister() - outBuff=",outBuff)
	i2c.writeto_mem(MCP23008_BASEADDR,regAdr,outBuff)	# Write to OLATA register
	return

def pinMode(bit,value):			# Set the single bit direction (INPUT, INPUT_PULLUP, OUTPUT)
	changeBit = 1 << (bit & 0x7)
	puRegAdr=MCP23008_GPPU
	dirRegAdr=MCP23008_IODIR
	rdPuVal=readRegister(puRegAdr)
	rdDirVal=readRegister(dirRegAdr)
	if (value == INPUT_PULLUP): 
		rdPuVal |= changeBit
		writeRegister(puRegAdr,rdPuVal)
		rdDirVal |= changeBit
		writeRegister(dirRegAdr,rdDirVal)
	elif (value == INPUT):
		rdPuVal &= ~changeBit
		writeRegister(puRegAdr,rdPuVal)
		rdDirVal |= changeBit
		writeRegister(dirRegAdr,rdDirVal)
	elif (value == OUTPUT):
		rdDirVal &= ~changeBit
		writeRegister(dirRegAdr,rdDirVal)

def digitalWrite(bit,value): 	# Writes to a single bit
	# Need to do a read-modify-write to not mess up other bits
	rwValue=readRegister(MCP23008_OLAT)
	if (value == 0):
		rwValue &= ~(1 << (bit&0x7))
	else:
		rwValue |= (1 << (bit&0x7))
	writeRegister(MCP23008_OLAT,rwValue)
	return

def digitalRead(bit):			# Reads a single bit
	rdVal=readRegister(MCP23008_GPIO)
	return ((rdVal>>(bit&7))&0x01)

def doLEDs():
    for _ in range(10):
        for i in range(4):
            digitalWrite(i,1)
            time.sleep(0.1)
            digitalWrite(i,0)

def loopBack():
    for i in range(4):
        digitalWrite(i,digitalRead(i))

pinMode(0,OUTPUT)
pinMode(1,OUTPUT)
pinMode(2,OUTPUT)
pinMode(3,OUTPUT)
pinMode(4,INPUT)
pinMode(5,INPUT)
pinMode(6,INPUT)
pinMode(7,INPUT)

