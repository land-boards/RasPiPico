' GPIO32_Bits.bas - MCP23017 bit-wise control
' Provide Arduino style DigitalRead, DigitalWrite, PinMode functions
'
' MCP23017 Datasheet
'   https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf
'
' Doug Gillland
' Land Boards LLC 2022
'

MCP23017_I2CADR0   = &H20
MCP23017_I2CADR1   = &H21

' MCP23017 internal registers
MCP23017_IODIRA   = &H00
MCP23017_IODIRB   = &H01
MCP23017_IPOLA    = &H02
MCP23017_IPOLB    = &H03
MCP23017_GPINTENA = &H04
MCP23017_GPINTENB = &H05
MCP23017_DEFVALA  = &H06
MCP23017_DEFVALB  = &H07
MCP23017_INTCONA  = &H08
MCP23017_INTCONB  = &H09
MCP23017_IOCON    = &H0A
MCP23017_GPPUA    = &H0C
MCP23017_GPPUB    = &H0D
MCP23017_INTFA    = &H0E
MCP23017_INTFB    = &H0F
MCP23017_INTCAPA  = &H10
MCP23017_INTCAPB  = &H11
MCP23017_GPIOA    = &H12
MCP23017_GPIOB    = &H13
MCP23017_OLATA    = &H14
MCP23017_OLATB    = &H15

Print "Read Switches, Write LEDs"
Print "Hit a key to stop"

' Pin setups

' Setup the Interrupt pin
' Install H2 jumper to use interrupt line
SetPin GP2,DIN,PULLUP

' Setup the I2C port for the MCP23017
' Applies to all rev cards
SetPin GP14, GP15, I2C2
I2C2 OPEN 400, 100

initMCP23017x2()

BIT_OUT = 0
BIT_IN = 1

Dim ioBuf(1)

' Set all bits to outputs
For outBit = 0 To 31
  PinMode(outBit,BIT_OUT)
Next outBit

' Alt pattern
I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATA, &H55
I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATB, &H55
I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATA, &H55
I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATB, &H55
Pause 500
I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATA, &HAA
I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATB, &HAA
I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATA, &HAA
I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATB, &HAA
Pause 500
I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATA, &H00
I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATB, &H00
I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATA, &H00
I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATB, &H00

Dim buf(1)

loop1:
  For loopVal = 0 To 31
    DigitalWrite(loopVal,1)
    Pause 100
    DigitalWrite(loopVal,0)
    If Inkey$ <> "" GoTo Done
  Next loopVal
GoTo loop1

Done:
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATA, &H00
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATB, &H00
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATA, &H00
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATB, &H00
  I2C2 Close
  End

' Read a bit
'   bit = 0-31
Function DigitalRead(readBit)
  If readBit < 8 GoTo readC0A
  If readBit < 16 GoTo readC0B
  If readBit < 24 GoTo readC1A
  If readBit < 32 GoTo readC1B
  Print "Error in bit value"
  GoTo doneRead
readC0A:
  I2C2 WRITE MCP23017_I2CADR0, 1, 1, MCP23017_GPIOA
  I2C2 read MCP23017_I2CADR0, 0, 1, iobuf()
  DigitalRead = (iobuf(0) >> readBit) And 1
  GoTo doneRead
readC0B:
  I2C2 WRITE MCP23017_I2CADR0, 1, 1, MCP23017_GPIOB
  I2C2 read MCP23017_I2CADR0, 0, 1, iobuf()
  DigitalRead = (iobuf(0) >> (readBit-8)) And 1
  GoTo doneRead
readC1A:
  I2C2 WRITE MCP23017_I2CADR1, 1, 1, MCP23017_GPIOA
  I2C2 read MCP23017_I2CADR1, 0, 1, iobuf()
  DigitalRead = (iobuf(0) >> readBit) And 1
  GoTo doneRead
readC1B:
  I2C2 WRITE MCP23017_I2CADR1, 1, 1, MCP23017_GPIOB
  I2C2 read MCP23017_I2CADR1, 0, 1, iobuf()
  DigitalRead = (iobuf(0) >> (readBit-8)) And 1
  GoTo doneRead
doneRead:
End Function

' Write a bit
Sub DigitalWrite(writeBit,val)
  If writeBit < 8 GoTo writeC0A
  If writeBit < 16 GoTo writeC0B
  If writeBit < 24 GoTo writeC1A
  If writeBit < 32 GoTo writeC1B
  Print "Error in bit value"
  GoTo doneWrite
writeC0A:
  I2C2 WRITE MCP23017_I2CADR0, 1, 1, MCP23017_OLATA
  I2C2 read MCP23017_I2CADR0, 0, 1, iobuf()
  wrVal = iobuf(0)
  bitVal = 1 << writeBit
  If val = 0 Then wrVal = wrVal And (INV bitVal)
  If val = 1 Then wrVal = wrVal Or bitVal
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATA, (wrVal And 255)
  GoTo doneWrite
writeC0B:
  I2C2 WRITE MCP23017_I2CADR0, 1, 1, MCP23017_OLATB
  I2C2 read MCP23017_I2CADR0, 0, 1, iobuf()
  wrVal = iobuf(0)
  bitVal = 1 << (writeBit - 8)
  If val = 0 Then wrVal = wrVal And (INV bitVal)
  If val = 1 Then wrVal = wrVal Or bitVal
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATB, (wrVal And 255)
  GoTo doneWrite
writeC1A:
  I2C2 WRITE MCP23017_I2CADR1, 1, 1, MCP23017_OLATA
  I2C2 read MCP23017_I2CADR1, 0, 1, iobuf()
  wrVal = iobuf(0)
  bitVal = 1 << (writeBit - 16)
  If val = 0 Then wrVal = wrVal And (INV bitVal)
  If val = 1 Then wrVal = wrVal Or bitVal
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATA, (wrVal And 255)
  GoTo doneWrite
writeC1B:
  I2C2 WRITE MCP23017_I2CADR1, 1, 1, MCP23017_OLATB
  I2C2 read MCP23017_I2CADR1, 0, 1, iobuf()
  wrVal = iobuf(0)
  bitVal = 1 << (writeBit - 24)
  If val = 0 Then wrVal = wrVal And (INV bitVal)
  If val = 1 Then wrVal = wrVal Or bitVal
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATB, (wrVal And 255)
  GoTo doneWrite
doneWrite:
End Sub

' Set an I/O direction bit
'   bit = 0-31
'   dir, 1=in, 0=out
Sub PinMode(bit,dir)
  If bit < 8 GoTo SetDirC0A
  If bit < 16 GoTo SetDirC0B
  If bit < 24 GoTo SetDirC1A
  If bit < 32 GoTo SetDirC1B
  Print "Error in bit value"
  GoTo doneSetBit
SetDirC0A:
  I2C2 WRITE MCP23017_I2CADR0, 1, 1, MCP23017_IODIRA
  I2C2 read MCP23017_I2CADR0, 0, 1, iobuf()
  ioRegVal = iobuf(0)
  bitVal = 1 << bit
  If dir = 1 Then ioRegVal = (bitVal Or ioRegVal) And 255
  If dir = 0 Then ioRegVal = ((INV bitVal) And ioRegVal) And 255
'  Print "c0a bitVal",bitVal,"ioRegVal",ioRegVal
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IODIRA, ioRegVal
  GoTo doneSetBit
SetDirC0B:
  I2C2 WRITE MCP23017_I2CADR0, 1, 1, MCP23017_IODIRB
  I2C2 read MCP23017_I2CADR0, 0, 1, iobuf()
  ioRegVal = iobuf(0)
  bitVal = 1 << (bit-8)
  If dir = 1 Then ioRegVal = (bitVal Or ioRegVal) And 255
  If dir = 0 Then ioRegVal = ((INV bitVal) And ioRegVal) And 255
'  Print "c0b bitVal",bitVal,"ioRegVal",ioRegVal
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IODIRB, ioRegVal
  GoTo doneSetBit
SetDirC1A:
  I2C2 WRITE MCP23017_I2CADR1, 1, 1, MCP23017_IODIRA
  I2C2 read MCP23017_I2CADR1, 0, 1, iobuf()
  ioRegVal = iobuf(0)
  bitVal = 1 << (bit-16)
  If dir = 1 Then ioRegVal = (bitVal Or ioRegVal) And 255
  If dir = 0 Then ioRegVal = ((INV bitVal) And ioRegVal) And 255
'  Print "c1a bitVal",bitVal,"ioRegVal",ioRegVal
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IODIRA, ioRegVal
  GoTo doneSetBit
SetDirC1B:
  I2C2 WRITE MCP23017_I2CADR1, 1, 1, MCP23017_IODIRB
  I2C2 read MCP23017_I2CADR1, 0, 1, iobuf()
  ioRegVal = iobuf(0)
  bitVal = 1 << (bit-24)
  If dir = 1 Then ioRegVal = (bitVal Or ioRegVal) And 255
  If dir = 0 Then ioRegVal = ((INV bitVal) And ioRegVal) And 255
'  Print "c1b bitVal",bitVal,"ioRegVal",ioRegVal
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IODIRB, ioRegVal
  GoTo doneSetBit
doneSetBit:
End Sub

Sub initMCP23017x2()
  'Set all pins to inputs
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IODIRA, &HFF:'inputs
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IODIRB, &HFF:'inputs
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IODIRA, &HFF:'inputs
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IODIRB, &HFF:'inputs

  'Don't invert inputs
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IPOLA, &H00
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IPOLB, &H00
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IPOLA, &H00
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IPOLB, &H00

  'Don't interrupt on any pins
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_GPINTENA, &H00
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_GPINTENB, &H00
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_GPINTENA, &H00
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_GPINTENB, &H00

  'Interrupt compare against previous pin
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_INTCONA, &H00
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_INTCONB, &H00
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_INTCONA, &H00
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_INTCONB, &H00

  'GPIO Pull-ups disabled
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_GPPUA, &H00
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_GPPUB, &H00
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_GPPUA, &H00
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_GPPUB, &H00

  'Bit 7 - BANK = 0 - Bank addresses are sequential
  'Bit 6 - MIRROR = 1 - Connect INTA and INTB internally
  'Bit 5 - SEQOP = 1 - Disable sequential
  'Bit 4 - DISSLW = 0 - SDA slew rate enabled
  'Bit 3 - HAEN - X - MCP23S017 only
  'Bit 2 - ODR = 1 - Interrup is Open Drain so INT can be "shared"
  'Bit 1 - INTPOL = 0 - Active Low Interrupt level
  'Bit 0 = X - Unused
  I2C2 WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IOCON, &H64
  I2C2 WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IOCON, &H64
End Sub            