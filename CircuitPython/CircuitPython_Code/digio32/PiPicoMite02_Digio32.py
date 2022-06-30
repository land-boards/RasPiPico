###############################################################################
# PiPicoMite02_Digio32.py
# Arduino-like functions
# digitalWrite(bit,value)
#    bit = 0-31
#    value = 0, 1
# digitalRead(bit)
#    bit = 0-31
#    returns 0, 1
# pinMode(bit,value)
#    bit = 0-31
#    value = INPUT, OUTPUT, INPUT_PULLUP
# pinMode(bit,value)
# CircuitPython
# https://docs.circuitpython.org/en/latest/shared-bindings/busio/#busio.I2C
###############################################################################

import board
import busio
import time

MCP23017_BASEADDR=0x20

MCP23017_IODIRA=0x00
MCP23017_IPOLA=0x02
MCP23017_GPINTENA=0x04
MCP23017_DEFVALA=0x06
MCP23017_INTCONA=0x08
MCP23017_IOCONA=0x0A
MCP23017_GPPUA=0x0C
MCP23017_INTFA=0x0E
MCP23017_INTCAPA=0x10
MCP23017_GPIOA=0x12
MCP23017_OLATA=0x14

MCP23017_IODIRB=0x01
MCP23017_IPOLB=0x03
MCP23017_GPINTENB=0x05
MCP23017_DEFVALB=0x07
MCP23017_INTCONB=0x09
MCP23017_IOCONB=0x0B
MCP23017_GPPUB=0x0D
MCP23017_INTFB=0x0F
MCP23017_INTCAPB=0x11
MCP23017_GPIOB=0x13
MCP23017_OLATB=0x15

INPUT=0x0
OUTPUT=0x1
INPUT_PULLUP=0x2

def initMCP23017x2():
    global chipAddr
    chipAddr=0x20
    writeRegister(MCP23017_IODIRA,0x00)
    writeRegister(MCP23017_IODIRB,0x00)
    writeRegister(MCP23017_IPOLA, 0x00)
    writeRegister(MCP23017_IPOLB, 0x00)
    writeRegister(MCP23017_GPINTENA, 0x00)
    writeRegister(MCP23017_GPINTENB, 0x00)
    writeRegister(MCP23017_INTCONA, 0x00)
    writeRegister(MCP23017_INTCONB, 0x00)
    writeRegister(MCP23017_GPPUA, 0x00)
    writeRegister(MCP23017_GPPUB, 0x00)
    writeRegister(MCP23017_IOCONA,0x64)
    chipAddr=0x21
    writeRegister(MCP23017_IODIRA,0x00)
    writeRegister(MCP23017_IODIRB,0x00)
    writeRegister(MCP23017_IPOLA, 0x00)
    writeRegister(MCP23017_IPOLB, 0x00)
    writeRegister(MCP23017_GPINTENA, 0x00)
    writeRegister(MCP23017_GPINTENB, 0x00)
    writeRegister(MCP23017_INTCONA, 0x00)
    writeRegister(MCP23017_INTCONB, 0x00)
    writeRegister(MCP23017_GPPUA, 0x00)
    writeRegister(MCP23017_GPPUB, 0x00)
    writeRegister(MCP23017_IOCONA,0x64)
    return

def digitalWrite(bit,value):     # Writes to a single bit
    global chipAddr
    chipAddr = MCP23017_BASEADDR | ((bit & 0x10) >> 4)
    if ((bit & 0x08) == 0):
        regAdr=MCP23017_OLATA
    else:
        regAdr=MCP23017_OLATB
    rwValue=readRegister(regAdr)
    if (value == 0):
        rwValue &= ~(1 << (bit&0x7))
    else:
        rwValue |= (1 << (bit&0x7))
    writeRegister(regAdr,rwValue)
    return

def digitalRead(bit):            # Reads a single bit
    global chipAddr
    chipAddr = MCP23017_BASEADDR | ((bit & 0x10) >> 4)
    if ((bit & 0x08) == 0):
        regAdr=MCP23017_GPIOA
    else:
        regAdr=MCP23017_GPIOB
    rdVal=readRegister(regAdr)
    return ((rdVal>>(bit&7))&0x01)

def pinMode(bit,value):            # Set the single bit direction (INPUT, INPUT_PULLUP, OUTPUT)
    global chipAddr
#     print("pinMode: bit,value",hex(bit),hex(value))
    chipAddr = MCP23017_BASEADDR | ((bit & 0x10) >> 4)
#     print("pinMode: chipAddr",hex(chipAddr))
    changeBit = 1 << (bit & 0x7)
    if ((bit & 0x08) == 0):
        puRegAdr=MCP23017_GPPUA
        dirRegAdr=MCP23017_IODIRA
    else:
        puRegAdr=MCP23017_GPPUB
        dirRegAdr=MCP23017_IODIRB
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

def readRegister(reg):
    global chipAddr
#     print("readRegister: reg",hex(reg))
    result = bytearray(1)
    i2c.writeto_then_readfrom(chipAddr, bytes([reg]), result)
    return result[0]

def writeRegister(reg, val):
    global chipAddr
    passVal = bytearray([reg, val])
    i2c.writeto(chipAddr, passVal)

i2c = busio.I2C(board.GP15, board.GP14, frequency=400000)    # Pi Pico RP2040

# Lock the I2C device before we try to scan
while not i2c.try_lock():
    pass

# Print the addresses found once
print("I2C addresses found:", [hex(device_address) for device_address in i2c.scan()])
if len(i2c.scan()) != 2:
    assert False,"Bad count"

chipAddr = MCP23017_BASEADDR

initMCP23017x2()

for bit in range(32):
    pinMode(bit,OUTPUT)

speed = 0.20
while True:
    for bit in range(32):
        digitalWrite(bit,1)
        time.sleep(speed)
        digitalWrite(bit,0)
    speed -= 0.01
    if speed <= 0.00:
        speed = 0.20
    
# for bit in range(32):
#     digitalWrite(bit,0)
#     time.sleep(0.1)
    
i2c.unlock()
