' DIGIO32I2C-02.bas - Test external DIGIO32-I2C on PiPicoMite02
' Test the (2) MCP23017 parts on the DIGIO32-I2C card
' Loops pins from interbal 32-bit Digio to external DIGIO32-I2C card
'
' I2C is the UUT
'    UUT_I2CADR0, UUT_I2CADR1
' I2C2 is the Test Station
'    TEST_STN_I2CADR0, TEST_STN_I2CADR1
'
' Runs on CPU card
'   http://land-boards.com/blwiki/index.php?title=PiPicoMite02
'
' UUT
'   http://land-boards.com/blwiki/index.php?title=DIGIO32-I2C
'
' MCP23017 Datasheet
'   https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf
'
' Doug Gillland
' Land Boards LLC 2022
'

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

Print "Looping through 32 lines"
Print "Hit a key to stop"

' Pin setups

' Setup the Interrupt pin
' Install J4 jumper to use interrupt line
' Comment/Un-comment out one of the two next lines depending on board rev
SetPin GP2,DIN,PULLUP:'GP2 on PiPicoMite02

' Setup the I2C port for the MCP23017
' Applies to all rev cards
' Pins are set by OPTION SYSTEM commmand
' SetPin GP14, GP15, I2C
' I2C OPEN 400, 100

' Setup the I2C port for the internal MCP23017 parts
SetPin GP14, GP15, I2C2
I2C2 OPEN 400, 100

TEST_STN_I2CADR0 = &H20
TEST_STN_I2CADR1 = &H21

UUT_I2CADR0 = 0
UUT_I2CADR1 = 0

delayTime = 1

' Detect the I2C addresses of the external DIGIO32-I2C card
Dim buf(1)
For i2cAddr = &H20 To &H27
  I2C read i2cAddr,0,1,buf()
  If (MM.I2C = 0) And (UUT_I2CADR0 <> 0) Then UUT_I2CADR1 = i2cAddr
  If (MM.I2C = 0) And (UUT_I2CADR0 = 0) Then UUT_I2CADR0 = i2cAddr
Next i2cAddr
If (UUT_I2CADR0 <> 0) And (UUT_I2CADR1<>0) GoTo setupMCP
Print "Did not find 2 MCP parts"

setupMCP:
Print "Found UUT on I2C bus"

I2C2 read TEST_STN_I2CADR0,0,1,buf()
If MM.I2C = 0 GoTo testStnOK1
Print "Test station not working (1)"
End

testStnOK1:
I2C2 read TEST_STN_I2CADR1,0,1,buf()
If MM.I2C = 0 GoTo testStnOK2
Print "Test station not working (2)"
End

testStnOK2:

' Initialize the internal MCP23017 parts
initIntDigios()

' Initialize the MCP23017 parts on the DIGIO32-I2C card
initExtDIGIO32()

loopCount = 0
doAgain:
  setIntDigiosIn()
  setUUTDigiosOut()
  wrVal% = 1
loopVals1:
  WriteUUTDIGIO32(wrVal%)
  Pause delayTime
  rdVal% = ReadIntlMCP23017()
  If rdVal% = wrVal% Then GoTo rwOK1
  If rdVal% <> wrVal% Then Print "Bad: Wrote";wrVal%;", Read";rdVal%
  End
rwOK1:
  wrVal% = wrVal% * 2
  If wrVal% < &H100000000 Then GoTo loopVals1
  If Inkey$ <> "" GoTo Done
  wrVal% = 1


  setUUTDigiosIn()
  setIntDigiosOut()
  wrVal% = 1
loopVals2:
  WriteIntDIGIO32(wrVal%)
  Pause delayTime
  rdVal% = ReadUUTMCP23017()
  If rdVal% = wrVal% Then GoTo rwOK2
  If rdVal% <> wrVal% Then Print "Bad: Wrote";wrVal%;", Read";rdVal%
  End
rwOK2:
  wrVal% = wrVal% * 2
  If wrVal% < &H100000000 Then GoTo loopVals2
  If Inkey$ <> "" GoTo Done
  wrVal% = 1

  loopCount = loopCount + 1
  Print "Passed";loopCount

GoTo doAgain

Function ReadIntlMCP23017()
  I2C2 WRITE TEST_STN_I2CADR0, 1, 1, MCP23017_GPIOB
  I2C2 READ TEST_STN_I2CADR0, 0, 1, BUF():'DO THE READ
  rdVal% = buf(0)
  I2C2 WRITE TEST_STN_I2CADR0, 1, 1, MCP23017_GPIOA
  I2C2 READ TEST_STN_I2CADR0, 0, 1, BUF():'DO THE READ
  rdVal% = (rdVal% * 256) + buf(0)
  I2C2 WRITE TEST_STN_I2CADR1, 1, 1, MCP23017_GPIOB
  I2C2 READ TEST_STN_I2CADR1, 0, 1, BUF():'DO THE READ
  rdVal% = (rdVal% * 256) + buf(0)
  I2C2 WRITE TEST_STN_I2CADR1, 1, 1, MCP23017_GPIOA
  I2C2 READ TEST_STN_I2CADR1, 0, 1, BUF():'DO THE READ
  ReadIntlMCP23017 = (rdVal% * 256) + buf(0)
End Function

Function ReadUUTMCP23017()
  I2C WRITE UUT_I2CADR0, 1, 1, MCP23017_GPIOB
  I2C READ UUT_I2CADR0, 0, 1, BUF():'DO THE READ
  rdVal% = buf(0)
  I2C WRITE UUT_I2CADR0, 1, 1, MCP23017_GPIOA
  I2C READ UUT_I2CADR0, 0, 1, BUF():'DO THE READ
  rdVal% = (rdVal% * 256) + buf(0)
  I2C WRITE UUT_I2CADR1, 1, 1, MCP23017_GPIOB
  I2C READ UUT_I2CADR1, 0, 1, BUF():'DO THE READ
  rdVal% = (rdVal% * 256) + buf(0)
  I2C WRITE UUT_I2CADR1, 1, 1, MCP23017_GPIOA
  I2C READ UUT_I2CADR1, 0, 1, BUF():'DO THE READ
  ReadUUTMCP23017 = (rdVal% * 256) + buf(0)
End Function

' Write a 32-bit value to the Internal 32-bits
Sub WriteIntDigio32(writeVal%)
'  Print writeVal
  llByte = writeVal% And &H000000FF
  lmByte = (writeVal% And &H0000FF00) / 256
  umByte = (writeVal% And &H00FF0000) / 65536
  uuByte = (writeVal% And &HFF000000) / 16777216
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_OLATA, llByte
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_OLATB, lmByte
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_OLATA, umByte
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_OLATB, uuByte
End Sub

' Write a 32-bit value to the UUT DIGIO32-I2C card
Sub WriteUUTDIGIO32(writeVal%)
'  Print writeVal%
  llByte = writeVal% And &H000000FF
  lmByte = (writeVal% And &H0000FF00) / 256
  umByte = (writeVal% And &H00FF0000) / 65536
  uuByte = (writeVal% And &HFF000000) / 16777216
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_OLATA, llByte
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_OLATB, lmByte
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_OLATA, umByte
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_OLATB, uuByte
End Sub

Sub setUUTDigiosIn()
  'Set all UUT pins to inputs
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_IODIRA, &HFF
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_IODIRB, &HFF
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_IODIRA, &HFF
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_IODIRB, &HFF
End Sub

Sub setUUTDigiosOut()
  'Set all UUT pins to outputs
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_IODIRA, &H00
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_IODIRB, &H00
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_IODIRA, &H00
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_IODIRB, &H00
End Sub

Sub initExtDIGIO32()
  Print "Initializing the external DIGIO32-I2C card"
  'Set all pins to outputs
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_IODIRA, &H00
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_IODIRB, &H00
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_IODIRA, &H00
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_IODIRB, &H00

  'Don't invert inputs
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_IPOLA, &H00
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_IPOLB, &H00
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_IPOLA, &H00
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_IPOLB, &H00

  'Don't interrupt on any pins
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_GPINTENA, &H00
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_GPINTENB, &H00
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_GPINTENA, &H00
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_GPINTENB, &H00

  'Interrupt compare against previous pin
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_INTCONA, &H00
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_INTCONB, &H00
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_INTCONA, &H00
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_INTCONB, &H00

  'GPIO Pull-ups disabled
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_GPPUA, &H00
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_GPPUB, &H00
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_GPPUA, &H00
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_GPPUB, &H00

  'Bit 7 - BANK = 0 - Bank addresses are sequential
  'Bit 6 - MIRROR = 1 - Connect INTA and INTB internally
  'Bit 5 - SEQOP = 1 - Disable sequential
  'Bit 4 - DISSLW = 0 - SDA slew rate enabled
  'Bit 3 - HAEN - X - MCP23S017 only
  'Bit 2 - ODR = 1 - Interrup is Open Drain so INT can be "shared"
  'Bit 1 - INTPOL = 0 - Active Low Interrupt level
  'Bit 0 = X - Unused
  I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_IOCON, &H64
  I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_IOCON, &H64
End Sub

Sub setIntDigiosIn()
  'Set all internal pins to inputs
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_IODIRA, &HFF
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_IODIRB, &HFF
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_IODIRA, &HFF
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_IODIRB, &HFF
End Sub

Sub setIntDigiosOut()
  'Set all internal pins to outputs
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_IODIRA, &H00
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_IODIRB, &H00
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_IODIRA, &H00
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_IODIRB, &H00
End Sub

Sub initIntDigios()
  Print "Initializing the internal Digios"
  'Set all pins to inputs
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_IODIRA, &HFF
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_IODIRB, &HFF
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_IODIRA, &HFF
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_IODIRB, &HFF

  'Don't invert inputs
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_IPOLA, &H00
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_IPOLB, &H00
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_IPOLA, &H00
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_IPOLB, &H00

  'Don't interrupt on any pins
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_GPINTENA, &H00
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_GPINTENB, &H00
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_GPINTENA, &H00
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_GPINTENB, &H00

  'Interrupt compare against previous pin
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_INTCONA, &H00
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_INTCONB, &H00
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_INTCONA, &H00
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_INTCONB, &H00

  'GPIO Pull-ups disabled
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_GPPUA, &H00
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_GPPUB, &H00
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_GPPUA, &H00
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_GPPUB, &H00

  'Bit 7 - BANK = 0 - Bank addresses are sequential
  'Bit 6 - MIRROR = 1 - Connect INTA and INTB internally
  'Bit 5 - SEQOP = 1 - Disable sequential
  'Bit 4 - DISSLW = 0 - SDA slew rate enabled
  'Bit 3 - HAEN - X - MCP23S017 only
  'Bit 2 - ODR = 1 - Interrup is Open Drain so INT can be "shared"
  'Bit 1 - INTPOL = 0 - Active Low Interrupt level
  'Bit 0 = X - Unused
  I2C2 WRITE TEST_STN_I2CADR0, 0, 2, MCP23017_IOCON, &H64
  I2C2 WRITE TEST_STN_I2CADR1, 0, 2, MCP23017_IOCON, &H64
End Sub

Done:
I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_OLATA, &H00
I2C WRITE UUT_I2CADR0, 0, 2, MCP23017_OLATB, &H00
I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_OLATA, &H00
I2C WRITE UUT_I2CADR1, 0, 2, MCP23017_OLATB, &H00
Print "Done"
End
