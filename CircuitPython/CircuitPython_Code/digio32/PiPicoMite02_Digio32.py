###############################################################################
# PiPicoMite02_Digio32.py
# CircuitPython library
# Module gets imported from application code
# Control the (2) MCP23017 parts on the PiPicoMite02
# Provides Arduino-like functions
# digitalWrite(bit,value)
#    bit = 0-31
#    value = 0,1
# digitalRead(bit)
#    bit = 0-31
#    returns 0,1
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

class OnBoardDigio():

    def initMCP23017x2(self,i2c):
        global chipAddr
        chipAddr=0x20
        self.writeRegister(i2c,MCP23017_IODIRA,0xFF)
        self.writeRegister(i2c,MCP23017_IODIRB,0xFF)
        self.writeRegister(i2c,MCP23017_IPOLA, 0x00)
        self.writeRegister(i2c,MCP23017_IPOLB, 0x00)
        self.writeRegister(i2c,MCP23017_GPINTENA, 0x00)
        self.writeRegister(i2c,MCP23017_GPINTENB, 0x00)
        self.writeRegister(i2c,MCP23017_INTCONA, 0x00)
        self.writeRegister(i2c,MCP23017_INTCONB, 0x00)
        self.writeRegister(i2c,MCP23017_GPPUA, 0x00)
        self.writeRegister(i2c,MCP23017_GPPUB, 0x00)
        self.writeRegister(i2c,MCP23017_IOCONA,0x64)
        chipAddr=0x21
        self.writeRegister(i2c,MCP23017_IODIRA,0xFF)
        self.writeRegister(i2c,MCP23017_IODIRB,0xFF)
        self.writeRegister(i2c,MCP23017_IPOLA, 0x00)
        self.writeRegister(i2c,MCP23017_IPOLB, 0x00)
        self.writeRegister(i2c,MCP23017_GPINTENA, 0x00)
        self.writeRegister(i2c,MCP23017_GPINTENB, 0x00)
        self.writeRegister(i2c,MCP23017_INTCONA, 0x00)
        self.writeRegister(i2c,MCP23017_INTCONB, 0x00)
        self.writeRegister(i2c,MCP23017_GPPUA, 0x00)
        self.writeRegister(i2c,MCP23017_GPPUB, 0x00)
        self.writeRegister(i2c,MCP23017_IOCONA,0x64)
        return

    def digitalWrite(self,i2c,bit,value):     # Writes to a single bit
        global chipAddr
        chipAddr = MCP23017_BASEADDR | ((bit & 0x10) >> 4)
        if ((bit & 0x08) == 0):
            regAdr=MCP23017_OLATA
        else:
            regAdr=MCP23017_OLATB
        rwValue=self.readRegister(i2c,regAdr)
        if (value == 0):
            rwValue &= ~(1 << (bit&0x7))
        else:
            rwValue |= (1 << (bit&0x7))
        self.writeRegister(i2c,regAdr,rwValue)
        return

    def digitalRead(self,i2c,bit):            # Reads a single bit
        global chipAddr
        chipAddr = MCP23017_BASEADDR | ((bit & 0x10) >> 4)
        if ((bit & 0x08) == 0):
            regAdr=MCP23017_GPIOA
        else:
            regAdr=MCP23017_GPIOB
        rdVal=self.readRegister(i2c,regAdr)
        return ((rdVal>>(bit&7))&0x01)

    def pinMode(self,i2c,bit,value):            # Set the single bit direction (INPUT, INPUT_PULLUP, OUTPUT)
        global chipAddr
        chipAddr = MCP23017_BASEADDR | ((bit & 0x10) >> 4)
        changeBit = 1 << (bit & 0x7)
        if ((bit & 0x08) == 0):
            puRegAdr=MCP23017_GPPUA
            dirRegAdr=MCP23017_IODIRA
        else:
            puRegAdr=MCP23017_GPPUB
            dirRegAdr=MCP23017_IODIRB
        rdPuVal=self.readRegister(i2c,puRegAdr)
        rdDirVal=self.readRegister(i2c,dirRegAdr)
        if (value == INPUT_PULLUP): 
            rdPuVal |= changeBit
            self.writeRegister(i2c,puRegAdr,rdPuVal)
            rdDirVal |= changeBit
            self.writeRegister(i2c,dirRegAdr,rdDirVal)
        elif (value == INPUT):
            rdPuVal &= ~changeBit
            self.writeRegister(i2c,puRegAdr,rdPuVal)
            rdDirVal |= changeBit
            self.writeRegister(i2c,dirRegAdr,rdDirVal)
        elif (value == OUTPUT):
            rdDirVal &= ~changeBit
            self.writeRegister(i2c,dirRegAdr,rdDirVal)

    def readRegister(self,i2c,reg):
        global chipAddr
        result = bytearray(1)
        i2c.writeto_then_readfrom(chipAddr, bytes([reg]), result)
        return result[0]

    def writeRegister(self,i2c, reg, val):
        global chipAddr
        passVal = bytearray([reg, val])
        i2c.writeto(chipAddr, passVal)
