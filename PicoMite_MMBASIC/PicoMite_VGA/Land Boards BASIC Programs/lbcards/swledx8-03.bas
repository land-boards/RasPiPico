' SWLEDX8-03.bas
' Write alternating pattern to LEDs then read/writte switches to LEDs
'
' Doug Gillland
' Land Boards LLC 2022
'

MCP23017_I2CADR   = &H20
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

Print "Read Switches, Write LEDs..."
Print "Hit a key to stop"

' Pin setups
SetPin GP0, GP1, I2C
I2C OPEN 400, 100
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_IODIRA, &HFF
If MM.I2C = 0 GoTo skipFail
If MM.I2C = 1 Then Print "I2C failed - Received NACK responce"
GoTo progEnd

skipFail:
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_IODIRB, &H00

' Alternating patterns to LEDs
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATB, &HAA
Pause 1000
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATB, &H55
Pause 200
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATB, &H00
Pause 00

Dim BUF(2)

goBack:
I2C WRITE MCP23017_I2CADR, 1, 1, MCP23017_GPIOA
I2C READ MCP23017_I2CADR, 0, 1, BUF()
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATB, BUF(0)
If Inkey$ <> "" GoTo progEnd
GoTo goBack

progEnd:
