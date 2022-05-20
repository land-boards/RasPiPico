' Test I2CIO08 card (I2CIO8-01.bas)
' Bounce a LED across the outputs
'
' MCP23008 I2C Expander
' I2C Address jumpers set to H21
' GPIO0-3 = LEDs = OUT
' GPIO4-7 = Jumpers = IN
'
' Doug Gilliland
' Land Boards, 2022
'

Print "Bounce a 1 across the LEDs (I2CIO8-01.bas)"
Print "Hit a key to stop"

MCP23008_I2CADR  = &H21
MCP23008_IODIR   = &H00
MCP23008_IPOL    = &H01
MCP23008_GPINTEN = &H02
MCP23008_DEFVAL  = &H03
MCP23008_INTCON  = &H04
MCP23008_IOCON   = &H05
MCP23008_GPPU    = &H06
MCP23008_INTF    = &H07
MCP23008_INTCAP  = &H08
MCP23008_GPIO    = &H09
MCP23008_OLAT    = &H0A

waitTime = 100

SetPin GP14, GP15, I2C2
I2C2 OPEN 400, 100:'400 KHz, 100 mS timeout
I2C2 WRITE MCP23008_I2CADR, 0, 2, MCP23008_IODIR, &HF0
I2C2 WRITE MCP23008_I2CADR, 0, 2, MCP23008_IPOL, &HF0
I2C2 WRITE MCP23008_I2CADR, 0, 2, MCP23008_IOCON, &H42
I2C2 WRITE MCP23008_I2CADR, 0, 2, MCP23008_GPINTEN, &HF0
I2C2 WRITE MCP23008_I2CADR, 0, 2, MCP23008_DEFVAL, &H00
I2C2 WRITE MCP23008_I2CADR, 0, 2, MCP23008_INTCON, &H00
I2C2 WRITE MCP23008_I2CADR, 0, 2, MCP23008_GPPU, &HF0

' Do the program loop

' Bounce left across LEDs
forever:
ledval = 1
nextled1:
I2C2 WRITE MCP23008_I2CADR, 0, 2, MCP23008_OLAT, ledval
Pause waitTime
ledval = ledval * 2
If Inkey$ <> "" GoTo done
If ledval < 16 GoTo nextled1

' Bounce right across LEDs
ledval = 4
nextled2:
I2C2 WRITE MCP23008_I2CADR, 0, 2, MCP23008_OLAT, ledval
Pause waitTime
ledval = ledval / 2
If Inkey$ <> "" GoTo done
If ledval > 1 GoTo nextled2
GoTo forever

done:
Print "Done bouncing the LEDs"
