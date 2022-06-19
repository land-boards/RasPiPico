' DIGIO32I2C-01.bas - Test external DIGIO32I2C on PiPicoMite02
' Test the (2) MCP23017 parts on the DIGIO32-I2C card
' Light all of the output pins one at a time
' Alternate patterns then bounce a LED
'
' Runs on
'   http://land-boards.com/blwiki/index.php?title=PiPicoMite02
'
' Attached to LED-32 xard
'   http://land-boards.com/blwiki/index.php?title=LED-32
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

Print "Looping through 32 LEDs"
Print "Hit a key to stop"

' Pin setups

' Setup the Interrupt pin
' Install J4 jumper to use interrupt line
' Comment/Un-comment out one of the two next lines depending on board rev
SetPin GP2,DIN,PULLUP:'GP2 on PiPicoMite02

' Setup the I2C port for the MCP23017
' Applies to all rev cards
' SetPin GP14, GP15, I2C
' I2C OPEN 400, 100

delayTime = 500

I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IODIRA, &H00
If MM.I2C = 0 GoTo skipFail1
Print "Problem accessing MCP23017 at 0x20"
End
skipFail1:
I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IODIRB, &H00
If MM.I2C = 0 Then GoTo skipFail2
Print "Problem accessing MCP23017 at 0x21"
End
skipFail2:
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IODIRA, &H00
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IODIRB, &H00

' Alternating patterns
I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATA, &H55
I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATB, &H55
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATA, &H55
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATB, &H55
Pause delayTime
I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATA, &HAA
I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATB, &HAA
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATA, &HAA
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATB, &HAA
Pause delayTime
I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATA, &H00
I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATB, &H00
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATA, &H00
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATB, &H00
Pause delayTime

loopBack:
loopVal = &H01
loop1:
I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATA, loopVal
Pause delayTime
If Inkey$ <> "" GoTo Done
loopVal = loopVal * 2
If loopVal < 256 GoTo loop1
I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATA, 0

loopVal = &H01
loop2:
I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATB, loopVal
Pause delayTime
If Inkey$ <> "" GoTo Done
loopVal = loopVal * 2
If loopVal < 256 GoTo loop2
I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATB, 0

loopVal = &H01
loop3:
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATA, loopVal
Pause delayTime
If Inkey$ <> "" GoTo Done
loopVal = loopVal * 2
If loopVal < 256 GoTo loop3
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATA, 0

loopVal = &H01
loop4:
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATB, loopVal
Pause delayTime
If Inkey$ <> "" GoTo Done
loopVal = loopVal * 2
If loopVal < 256 GoTo loop4
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATB, 0
GoTo loopBack

Done:
I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATA, &H00
I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_OLATB, &H00
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATA, &H00
I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_OLATB, &H00
Print "Done"
End

Sub initMCP23017()
  'Set all pins to outputs
  I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IODIRA, &H00
  I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IODIRB, &H00
  I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IODIRA, &H00
  I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IODIRB, &H00

  'Don't invert inputs
  I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IPOLA, &H00
  I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IPOLB, &H00
  I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IPOLA, &H00
  I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IPOLB, &H00

  'Don't interrupt on any pins
  I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_GPINTENA, &H00
  I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_GPINTENB, &H00
  I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_GPINTENA, &H00
  I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_GPINTENB, &H00

  'Interrupt compare against previous pin
  I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_INTCONA, &H00
  I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_INTCONB, &H00
  I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_INTCONA, &H00
  I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_INTCONB, &H00

  'GPIO Pull-ups disabled
  I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_GPPUA, &H00
  I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_GPPUB, &H00
  I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_GPPUA, &H00
  I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_GPPUB, &H00

  'Bit 7 - BANK = 0 - Bank addresses are sequential
  'Bit 6 - MIRROR = 1 - Connect INTA and INTB internally
  'Bit 5 - SEQOP = 1 - Disable sequential
  'Bit 4 - DISSLW = 0 - SDA slew rate enabled
  'Bit 3 - HAEN - X - MCP23S017 only
  'Bit 2 - ODR = 1 - Interrup is Open Drain so INT can be "shared"
  'Bit 1 - INTPOL = 0 - Active Low Interrupt level
  'Bit 0 = X - Unused
  I2C WRITE MCP23017_I2CADR0, 0, 2, MCP23017_IOCON, &H64
  I2C WRITE MCP23017_I2CADR1, 0, 2, MCP23017_IOCON, &H64
End Sub                                           