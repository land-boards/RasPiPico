' Test I2CIO-8 Rev 3 card (I2CIO8-ReadWriteV3.bas)
'
' Runs on PiPicoMite01 card running PiciMiteVGA BASIC
' PiPicoMite01 card Wiki page
'   http://land-boards.com/blwiki/index.php?title=PiPicoMite01
'
' Reads 4 I2CIO-8 Jumpers, writes back out to 4 I2CIO-8 LEDs
' Insert corresponding jumper to light LED
'
' I2CIO-8 card Wiki page
' http://land-boards.com/blwiki/index.php?title=I2CIO-8
'
' Address set to H20
' Uses I2C0, PINS GP4, GP5
' Uses GP3 as "polled" interrupt line for jumper pin changed
' Compatible with both Rev 1 and Rev 2 PiPicoMite01 cards
'
' I2CIO-8 to MCP23008 port mapping
'   MCP23008 GPIO0-3 = LEDs = OUT
'   MCP23008 GPIO4-7 = Jumpers = IN
'     Jumpers are low when inserted
'     Program inverts the inputs
'
' LAND BOARDS, 2022
'

'MCP23008 REGISTER ADDRESSES
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

' MCP23008_IOCON_DEF
' BIT 5 - SEQOP = 1 - DISABLE SEQUENTIAL OPERATIONS
' BIT 4 - DISSLW = 1 - DISABLE SLEW RATE
' BIT 2 - ODR = 1 - OPEN DRAIN OUTPUT (EXTERNAL PULL-UP)
' BIT 1 - IPOL = 0 - ACTIVE LOW INTERRUPT OUTPUT
MCP23008_IOCON_DEF = &H3C

Dim BUF(2):'Allocate receive buffer space
Print "Loop Jumpers to LEDs (I2CIO8-ReadWriteV3.bas)"
Print "Hit a key to exit"

' Setup MCP23008 registers
SetPin GP4, GP5, I2C:'Uses I2C2 (I2C1 on part)
I2C OPEN 400, 100:'400KHz, 100 mS timeout
initMCP23008(MCP23008_I2CADR)

'Setup the Interrupt pin
SetPin GP3,DIN,PULLUP

' Do the loop, CTRL-C to exit

'Set initial LEDS from Jumpers
ODat=READJUMPERS()
WRITEJUMPERS(ODat)

LoopBack:
' Check the interrupt pin for received char
  If CheckIntPin() = 1 GoTo skipRW
' Read Jumpers
' Write out to LEDs
  ODat=READJUMPERS()
  WRITEJUMPERS(ODat)
  skipRW:
If Inkey$ <> "" GoTo Done
GoTo LoopBack

Sub initMCP23008(i2cadr)
  I2C WRITE i2cadr, 0, 2, MCP23008_IODIR, &HF0:'IODIR
  I2C WRITE i2cadr, 0, 2, MCP23008_IPOL, &HF0:'IPOL INVERT INPUTS
  I2C WRITE i2cadr, 0, 2, MCP23008_GPINTEN, &HF0:'Interrupt enables
  I2C WRITE i2cadr, 0, 2, MCP23008_INTCON, &H00:'COMPARE TO PREVIOUS VAL
  I2C WRITE i2cadr, 0, 2, MCP23008_IOCON, &H24:'NOT SEQ, ODR/LOW
End Sub

Function CheckIntPin()
  INTPIN = Pin(GP3)
End Function

Function READJUMPERS()
  I2C WRITE MCP23008_I2CADR, 1, 1, &H09:'SET TO READ jumpers
  I2C READ MCP23008_I2CADR, 0, 1, BUF():'DO THE READ
  READJUMPERS=(BUF(0) And 240) / 16:'MASK OFF OUT BITS, SHIFT RIGHT 4 BITS
End Function

Sub WRITEJUMPERS(LEDVAL)
  I2C WRITE MCP23008_I2CADR, 0, 2, &H0A, LEDVAL:'OLAT WRITE OUT
End Sub

Done:
End
