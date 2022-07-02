' GPIO16_Bits.bas - MCP23017 bit-wise control
' Provide Arduino style DigitalRead, DigitalWrite, PinMode functions
' Using http://land-boards.com/blwiki/index.php?title=SWLEDX8
' Read switch on GPB0, echo to LED on GPA0
'
' MCP23017 Datasheet
'   https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf
'
' Doug Gillland
' Land Boards LLC 2022
'

MCP23017_I2CADR   = &H20

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
' Install J1 jumper to use interrupt line on Rev 1
' Install H2 jumper to use interrupt line on Rev 2 and higher card(s)
' Comment/Un-comment out one of the two next lines depending on board rev
'SetPin GP0,DIN,PULLUP:'GP0 on Rev 1 card
SetPin GP2,DIN,PULLUP:'GP2 on Rev 2 and higher card(s)

' Setup the I2C port for the MCP23017
' Applies to all rev cards
SetPin GP14, GP15, I2C2
I2C2 OPEN 400, 100

initMCP23017()

BIT_OUT = 0
BIT_IN = 1

Dim ioBuf(1)

' Port A wired to LEDs in this example
For outBit = 0 To 7
  PinMode(outBit,BIT_OUT)
Next outBit

' Port B wired to switches in this case
For outBit = 8 To 15
  PinMode(outBit,BIT_IN)
Next outBit

Pause 100

Dim buf(1)

' Read Port B bit 0
' Write Port A bit 0
loop1:
inVal = DigitalRead(8)
Pause 10
DigitalWrite(0,inVal)
If Inkey$ <> "" GoTo Done
GoTo loop1

Done:
  I2C2 Close
  End

' Read a bit
'   bit = 0-15
Function DigitalRead(readBit)
  If readBit < 8 GoTo readA
  If readBit < 16 GoTo readB
  Print "Error in bit value"
  GoTo doneRead
readA:
  I2C2 WRITE MCP23017_I2CADR, 1, 1, MCP23017_GPIOA
  I2C2 read MCP23017_I2CADR, 0, 1, iobuf()
  DigitalRead = (iobuf(0) >> readBit) And 1
  GoTo doneRead
readB:
  I2C2 WRITE MCP23017_I2CADR, 1, 1, MCP23017_GPIOB
  I2C2 read MCP23017_I2CADR, 0, 1, iobuf()
  DigitalRead = (iobuf(0) >> (readBit-8)) And 1
doneRead:
End Function

' Write a bit
Sub DigitalWrite(writeBit,val)
  If writeBit < 8 GoTo writeA
  If writeBit < 16 GoTo writeB
  Print "Error in bit value"
  GoTo doneWrite
writeA:
  I2C2 WRITE MCP23017_I2CADR, 1, 1, MCP23017_OLATA
  I2C2 read MCP23017_I2CADR, 0, 1, iobuf()
  wrVal = iobuf(0)
  bitVal = 1 << writeBit
  If val = 0 Then wrVal = wrVal And (INV bitVal)
  If val = 1 Then wrVal = wrVal Or bitVal
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATA, wrVal
  GoTo doneWrite
writeB:
  I2C2 WRITE MCP23017_I2CADR, 1, 1, MCP23017_OLATB
  I2C2 read MCP23017_I2CADR, 0, 1, iobuf()
  wrVal = iobuf(0)
  bitVal = 1 << (writeBit - 8)
  If val = 0 Then wrVal = wrVal And (INV bitVal)
  If val = 1 Then wrVal = wrVal Or bitVal
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATB, wrVal
doneWrite:
End Sub

' Set an I/O direction bit
'   bit = 0-15
'   dir, 1=in, 0=out
Sub PinMode(bit,dir)
  If bit < 8 GoTo SetDirA
  If bit < 16 GoTo SetDirB
  Print "Error in bit value"
  GoTo doneSetBit
SetDirA:
  I2C2 WRITE MCP23017_I2CADR, 1, 1, MCP23017_IODIRA
  I2C2 read MCP23017_I2CADR, 0, 1, iobuf()
  ioRegVal = iobuf(0)
'  Print "ioRegVal",ioRegVal
  bitVal = 1 << bit
'  Print "bitVal",bitVal
'  Print "Inv bitVal",INV bitVal
  If dir = 1 Then ioRegVal = (bitVal Or ioRegVal) And 255
  If dir = 0 Then ioRegVal = ((INV bitVal) And ioRegVal)
'  Print "ioRegVal",ioRegVal
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_IODIRA, ioRegVal
  GoTo doneSetBit
SetDirB:
  I2C2 WRITE MCP23017_I2CADR, 1, 1, MCP23017_IODIRB
  I2C2 read MCP23017_I2CADR, 0, 1, iobuf()
  ioRegVal = iobuf(0)
  bitVal = 1 << (bit-8)
  If dir = 1 Then ioRegVal = (bitVal Or ioRegVal) And 255
  If dir = 0 Then ioRegVal = ((INV bitVal) And ioRegVal)
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_IODIRB, ioRegVal
  GoTo doneSetBit
doneSetBit:
End Sub

Sub initMCP23017()
  'Set all pins to inputs
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_IODIRA, &HFF:'inputs
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_IODIRB, &HFF:'inputs

  'Don't invert inputs
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_IPOLA, &H00
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_IPOLB, &H00

  'Don't interrupt on any pins
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_GPINTENA, &H00
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_GPINTENB, &H00

  'Interrupt compare against previous pin
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_INTCONA, &H00
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_INTCONB, &H00

  'GPIO Pull-ups disabled
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_GPPUA, &H00
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_GPPUB, &H00

  'Bit 7 - BANK = 0 - Bank addresses are sequential
  'Bit 6 - MIRROR = 1 - Connect INTA and INTB internally
  'Bit 5 - SEQOP = 1 - Disable sequential
  'Bit 4 - DISSLW = 0 - SDA slew rate enabled
  'Bit 3 - HAEN - X - MCP23S017 only
  'Bit 2 - ODR = 1 - Interrup is Open Drain so INT can be "shared"
  'Bit 1 - INTPOL = 0 - Active Low Interrupt level
  'Bit 0 = X - Unused
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_IOCON, &H64
End Sub
