' I2CIO16-DB25.bas - Test I2CIO16-DB25.bas card
' Alternate patterns Then bounce a LED
'
' Doug Gillland
' Land Boards LLC 2022
'

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

Print "Looping through LEDs"
Print "Hit a key to stop"

' Pin setups
' SetPin GP0, GP1, I2C

Dim buf(1)

i2cadr = &H20
goback:
I2C read i2cadr, 0, 1, buf()
If MM.I2C=0 Then MCP23017_I2CADR=i2cadr
i2cadr=i2cadr+1
If i2cadr<&H28 GoTo goback

If i2cadr <> 0 Then GoTo i2cIsOK

' Failed to find card
Print "Failed to find I2C card"
GoTo doneRunning

' I2C OPEN 400, 100

i2cIsOK:
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_IODIRA, &H00
If MM.I2C <> 0 Then Print "i2c error"
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_IODIRB, &H00

' Alternating patterns
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATA, &H55
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATB, &HAA
Pause 1000
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATA, &HAA
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATB, &H55
Pause 1000
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATA, &H00
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATB, &H00
Pause 100

loopBack:
loopVal = &H01
loop1:
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATA, loopVal
Pause 200
If Inkey$ <> "" GoTo Done
loopVal = loopVal * 2
If loopVal < 256 GoTo loop1
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATA, 0

loopVal = &H80
loop2:
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATB, loopVal
Pause 200
If Inkey$ <> "" GoTo Done
loopVal = loopVal / 2
If loopVal >= 1 GoTo loop2
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATB, 0
GoTo loopBack

Done:
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATA, &H00
I2C WRITE MCP23017_I2CADR, 0, 2, MCP23017_OLATB, &H00
Print "Done"

doneRunning:
End                                               