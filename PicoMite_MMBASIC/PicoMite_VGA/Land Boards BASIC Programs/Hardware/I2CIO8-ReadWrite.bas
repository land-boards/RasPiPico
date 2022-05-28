' Test I2CIO-8 card (I2CIO8-ReadWrite.bas)
' Read  4 jumpers, write out to 4 LEDs
' Move jumper to see LED move
'
' Address set to H21
' GPIO0-3 = LEDs = OUT
' GPIO4-7 = Jumpers = IN
'
' LAND BOARDS, 2022
'

Dim BUF(2):'Allocate receive buffer space
Print "Lopp Jumpers to LEDs (I2CIO8-02.bas)"
Print "Hit a key to exit"

' Setup MCP23008 registers
SetPin GP14, GP15, I2C2:'Uses I2C2 (I2C1 on part)
I2C2 OPEN 400, 100:'400KHz, 100 mS timeout
I2C2 WRITE &H21, 0, 2, &H00, &HF0:'IODIR
I2C2 WRITE &H21, 0, 2, &H01, &HF0:'IPOL INVERT INPUTS
I2C2 WRITE &H21, 0, 2, &H05, &H42:'IOCON NO SEQ, INT HIGH

' Do the loop, CTRL-C to exit

LoopBack:
' Read Jumpers
I2C2 WRITE &H21, 1, 1, &H09:'SET TO READ GPIO
I2C2 READ &H21, 0, 1, BUF():'DO THE READ
ODat=(BUF(0)And 240 ) / 16:'MASK OFF OUT BITS, SHIFT RIGHT 4 BITS
' Write out to LEDs
I2C2 WRITE &H21, 0, 2, &H0A, ODat:'OLAT WRITE OUT
If Inkey$ <> "" GoTo Done
GoTo LoopBack
Done:
