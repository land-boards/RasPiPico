'
' ODAS EEPROM Control (ODASEEPROM.bas)
'
' LAND BOARDS LLC, 2022
' Douglas Gilliland
'

'Setup I2C pins
SetPin GP0, GP1, I2C:'Uses I2C0
I2C OPEN 400, 100:'400 KHz, 100mS timeout

' Check for EEPROM
' Scan addresses from 0x50 to 0x57
i2cAdr=0
Dim BUF(16)
For i2cTestAdr=&H50 To &H57
  I2C READ i2cTestAdr,0,1,BUF()
  If MM.I2C<>0 GoTo SkipAdr:'Check status of poll operation
  i2cAdr=i2cTestAdr
SkipAdr:
  Next i2cTestAdr
If i2cAdr=0 GoTo exitProg
If i2cAdr<>0 Then Print "Found device at 0x";Hex$(i2cAdr)

'Read the first 16-bytes of the EEPROM
I2C WRITE i2cAdr, 1, 1, 0:'Set pointer to the first location
I2C READ i2cAdr, 0, 1, BUF():'Read the first byte of the EEPROM

'If the first byte is 255 then part is unprogrammed
If BUF(0)=255 Then Print "EEPROM is unprogrammed"
If BUF(0)=255 Then GoTo skipDump

'Dump the EEPROM first 16-bytes
Print "Dump EEPROM"
blockAddr=0
I2C WRITE i2cAdr, 1, 1, blockAddr:'Set pointer to the first location
I2C READ i2cAdr, 0, 4, BUF():'Read the signature of the EEPROM
Print " Signature = ";
For buffOff=0 To 3
  If ((BUF(bufOff)>31) Or (BUF(buffOff)<120)) Then Print Chr$(BUF(buffOff));
  If ((BUF(bufOff)<32) Or (BUF(buffOff)>127)) Then Print ".";
'  Print Hex$(BUF(buffOff));:Print " ";
  Next buffOff
Print ""
I2C READ i2cAdr, 0, 1, BUF()
Print " Version =";:Print BUF(0)
I2C READ i2cAdr, 0, 1, BUF()
Print " Rsvd =";:Print BUF(0)
I2C READ i2cAdr, 0, 2, BUF()
Print " NumAtoms =";:Print ((BUF(1)*256)+BUF(0))
I2C READ i2cAdr, 0, 4, BUF()
Print " eeplen =";:Print BUF(0)+(256*BUF(1))+(65536*BUF(2))
I2C READ i2cAdr, 0, 16, BUF()
Print " uuid = ";
For buffOff=0 To 15
  Print Chr$(BUF(buffOff));
  Next buffOff
Print ""
I2C READ i2cAdr, 0, 1, BUF()
Print " Product ID = ";
If BUF(0)=1 Then Print "DIGIO16-I2C"
If BUF(0)=2 Then Print "DIGIO128"
If BUF(0)=3 Then Print "OptoIn8-I2C"
If BUF(0)=4 Then Print "OptoOut8-I2C"
If BUF(0)=5 Then Print "DIGIO32-I2C"
If BUF(0)=6 Then Print "PROTO16-I2C"
If BUF(0)>6 Then Print "Unknown PID"
I2C READ i2cAdr, 0, 1, BUF()
Print " vslen =";:Print BUF(0)
I2C READ i2cAdr, 0, 1, BUF()
Print " pslen =";:Print BUF(0)
GoTo exitProg

skipDump:


exitProg:
End
