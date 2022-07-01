' SWLEDX8.bas - Read Switches, write LEDs
' (8) LEDs on PORT A
' (8) Switches on PORT B
' Using http://land-boards.com/blwiki/index.php?title=SWLEDX8
'
' There is a lot of extra initialization code that is not necessary
'   for this simple application. It is included as a code base for expansion.
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

Pause 100

Dim buf(1)
loopBack:
I2C2 WRITE MCP23017_I2CADR, 1, 1, MCP23017_GPIOB
I2C2 read MCP23017_I2CADR, 0, 1, buf()
Pause 50
I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATA, buf(0)
Pause 50
If Inkey$ <> "" GoTo Done
GoTo loopBack

Done:
  End

Sub initMCP23017()
'Set all pins of PORTA to output 
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_IODIRA, &H00
'Set all pins of PORTB to input
  I2C2 WRITE MCP23017_I2CADR, 0, 2, MCP23017_IODIRB, &HFF

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
