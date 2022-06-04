' Test I2CIO08 card (I2CIO8-Bounce-0.bas)
'
' Bounce a LED across the LEDs
'
' Runs on PiPicoMite01 card running PiciMiteVGA BASIC
' PiPicoMite01 card Wiki page
'   http://land-boards.com/blwiki/index.php?title=PiPicoMite01
'
' I2CIO-8 card Wiki page
' http://land-boards.com/blwiki/index.php?title=I2CIO-8
'
' MCP23008 I2C Expander
' I2C Address jumpers set to H20
' Uses I2C0, PINS GP4, GP5
'
' I2CIO-8 to MCP23008 port mapping
'   GPIO0-3 = LEDs = OUT
'   GPIO4-7 = Jumpers = IN
'     Jumpers are low when inserted
'     Program inverts the inputs
'
' Doug Gilliland
' Land Boards, 2022
'

Print "Bounce a 1 across the LEDs (I2CIO8-Bounce-0.bas)"
Print "Hit a key to stop"

MCP23008_I2CADR  = &H20
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

SetPin GP4, GP5, I2C
I2C OPEN 400, 100:'400 KHz, 100 mS timeout
initMCP23008(MCP23008_I2CADR)

' Do the program loop

' Bounce left across LEDs
forever:
ledval = 1
nextled1:
I2C WRITE MCP23008_I2CADR, 0, 2, MCP23008_OLAT, ledval
Pause waitTime
ledval = ledval * 2
If Inkey$ <> "" GoTo done
If ledval < 16 GoTo nextled1

' Bounce right across LEDs
ledval = 4
nextled2:
I2C WRITE MCP23008_I2CADR, 0, 2, MCP23008_OLAT, ledval
Pause waitTime
ledval = ledval / 2
If Inkey$ <> "" GoTo done
If ledval > 1 GoTo nextled2
GoTo forever

Sub initMCP23008(i2cadr)
  I2C WRITE i2cadr, 0, 2, MCP23008_IODIR, &HF0:'IODIR
  If MM.I2C = 0 GoTo skipFail
  If MM.I2C = 1 Then Print "I2C failed - Received NACK responce"
  GoTo done
skipFail:
  I2C WRITE i2cadr, 0, 2, MCP23008_IPOL, &HF0:'IPOL INVERT INPUTS
  I2C WRITE i2cadr, 0, 2, MCP23008_GPINTEN, &HF0:'Interrupt enables
  I2C WRITE i2cadr, 0, 2, MCP23008_INTCON, &H00:'COMPARE TO PREVIOUS VAL
  I2C WRITE i2cadr, 0, 2, MCP23008_IOCON, &H24:'NOT SEQ, ODR/LOW
End Sub

Sub WriteLEDs(LEDVAL)
  I2C WRITE MCP23008_I2CADR, 0, 2, &H0A, LEDVAL:'OLAT WRITE OUT
End Sub

done:
Print "Done bouncing the LEDs"
