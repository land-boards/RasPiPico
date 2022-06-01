'
' ODAS EEPROM Control (ODASEEPROM.bas)
'
' LAND BOARDS LLC, 2022
' Douglas Gilliland
'
' Details on the Personality EEPROM
' http://land-boards.com/blwiki/index.php?title=Open_Data_Acquisition_System
'

Print "ODAS EEPROM Tool v0.0.1"

'Setup I2C pins
SetPin GP0, GP1, I2C:'Uses I2C0
I2C OPEN 400, 100:'400 KHz, 100mS timeout

' Check for EEPROM
' Scan addresses from 0x50 to 0x57
EEWrSpeed=15:'24LC024 Datasheet shows 10 mS write cycle time - padded
i2cAdr=0
Dim BUF(33)
For i2cTestAdr=&H50 To &H57
  I2C READ i2cTestAdr,0,1,BUF()
  If MM.I2C<>0 GoTo SkipAdr:'Check status of poll operation
  i2cAdr=i2cTestAdr
SkipAdr:
  Next i2cTestAdr
If i2cAdr=0 Then Print "No EEPROM found"
If i2cAdr<>0 Then Print "Found device at 0x";Hex$(i2cAdr) Else GoTo exitProg

'Read the first 16-bytes of the EEPROM
I2C WRITE i2cAdr, 1, 1, 0:'Set pointer to the first location
I2C READ i2cAdr, 0, 1, BUF():'Read the first byte of the EEPROM

'If the first byte is 255 then part is unprogrammed
If BUF(0)=255 Then Print "EEPROM is unprogrammed"
If BUF(0)=255 Then GoTo skipDump

'Dump the EEPROM first 16-bytes
Print "Personality EEPROM contents"
blockAddr=0
I2C WRITE i2cAdr, 1, 1, blockAddr:'Set pointer to the first location
I2C READ i2cAdr, 0, 4, BUF():'Read the signature of the EEPROM
Print " Signature = ";
For buffOff=0 To 3
  If ((BUF(bufOff)>31) Or (BUF(buffOff)<120)) Then Print Chr$(BUF(buffOff));
  If ((BUF(bufOff)<32) Or (BUF(buffOff)>127)) Then Print ".";
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
I2C READ i2cAdr, 0, 2, BUF()
Print " Product ID =";;(buf(0)+(256*buf(1)));" ";
If BUF(0)=1 Then Print "(DigIO16-I2C)"
If BUF(0)=2 Then Print "(DIGIO-128)"
If BUF(0)=3 Then Print "(DIGIO-128/64)"
If BUF(0)=4 Then Print "(OptoIn8-I2C)"
If BUF(0)=5 Then Print "(OptoOut8-I2C)"
If BUF(0)=6 Then Print "(DIGIO32-I2C)"
If BUF(0)=7 Then Print "(PROTO16-I2C)"
If BUF(0)=8 Then Print "(ODAS-PSOC5)"
If BUF(0)=9 Then Print "(ODAS-RELAY16)"
If BUF(0)>9 Then Print "Unknown Card"

I2C READ i2cAdr, 0, 1, BUF()
Print " vslen =";:Print BUF(0)
I2C READ i2cAdr, 0, 1, BUF()
Print " pslen =";:Print BUF(0)
I2C READ i2cAdr, 0, 32, BUF()
Print " Vendor String = ";
buffOff=0
nextStr1:
If ((BUF(buffOff) > &H1F) And (BUF(buffOff) < &H7F)) Then Print Chr$(BUF(buffOff));
buffOff = buffOff+1
If buffOff<32 GoTo nextStr1
I2C READ i2cAdr, 0, 32, BUF()
Print ""
Print " Product String = ";
buffOff=0
nextStr2:
If ((BUF(buffOff) > &H1F) And (BUF(buffOff) < &H7F)) Then Print Chr$(BUF(buffOff));
buffOff = buffOff+1
If buffOff<32 GoTo nextStr2
Print ""

Input "Re-write (Y/N)";rewrFlag$
If ((rewrFlag$="N") Or (rewrFlag$="n")) GoTo exitProg

skipDump:
Print "Select Product"
Print "1 - DigIO16-I2C"
Print "2 - DIGIO-128"
Print "3 - DIGIO-128/64"
Print "4 - OptoIn8-I2C"
Print "5 - OptoOut8-I2C"
Print "6 - DIGIO32-I2C"
Print "7 - PROTO16-I2C"
Print "8 - ODAS-PSOC5"
Print "9 - ODAS-RELAY16"
Print "127 - Unknown"
Input pid

BUF(0)=0:'EEPROM start address of write
sig$="ODAS"
BUF(1)=Asc(Mid$(sig$,1,1)):'0-3 = signature(4)
BUF(2)=Asc(Mid$(sig$,2,1))
BUF(3)=Asc(Mid$(sig$,3,1))
BUF(4)=Asc(Mid$(sig$,4,1))
BUF(5)=1:'4 = fmt_version
BUF(6)=0:'5 = rsvd
BUF(7)=2:'5-7 = numatoms(2)
BUF(8)=0
BUF(9)=96:'8-11 = eeplen(4)
BUF(10)=0
BUF(11)=0
BUF(13)=0
BUF(13)=48:'12-15 = uuid[0-3] of 16
BUF(14)=48
BUF(15)=48
BUF(16)=48
I2C WRITE i2cAdr, 0, 17, BUF():'Set pointer to the start
If MM.I2C<>0 Then Print "STATUS error";MM.I2C
Pause EEWrSpeed

BUF(0)=16:'Offset of 16 bytes in EEPROM
For buffOff=1 To 12:'16-28 = uuid[4-15]
  BUF(buffOff)=48
Next buffOff
BUF(13)=pid:'pid
BUF(14)=0
BUF(15)=32:'vslen
BUF(16)=32:'pslen
I2C WRITE i2cAdr, 0, 17, BUF():'Set pointer to the next block
If MM.I2C<>0 Then Print "STATUS error";MM.I2C
Pause EEWrSpeed

'Vendor String
BUF(0)=32:'Offset of 16 bytes in EEPROM
vendorStr$="LAND-BOARDS.COM"
buffOff=1
strOff=1
loopMove1:
  BUF(buffOff)=Asc(Mid$(vendorStr$,strOff,1))
  buffOff=buffOff+1
  strOff=strOff+1
If buffOff<16 GoTo loopMove1
BUF(16)=0
I2C WRITE i2cAdr, 0, 17, BUF():'Set pointer to the next block
If MM.I2C<>0 Then Print "STATUS error";MM.I2C
Pause EEWrSpeed

BUF(0)=48:'Offset of 16 bytes in EEPROM
For buffOff=1 To 16
  BUF(buffOff)=0
Next buffOff
I2C WRITE i2cAdr, 0, 17, BUF():'Set pointer to the next block
If MM.I2C<>0 Then Print "STATUS error";MM.I2C
Pause EEWrSpeed

productID$="            "
If pid=1 Then productID$="DigIO16-I2C "
If pid=2 Then productID$="DIGIO-128   "
If pid=3 Then productID$="DIGIO-128/64"
If pid=4 Then productID$="OptoIn8-I2C "
If pid=5 Then productID$="OptoOut8-I2C"
If pid=6 Then productID$="DIGIO32-I2C "
If pid=7 Then productID$="PROTO16-I2C "
If pid=8 Then productID$="ODAS-PSOC5  "
If pid=9 Then productID$="ODAS-RELAY16"
If pid=127 Then productID$="UNKNOWN     "
Print "productID = ";productID$

BUF(0)=64:'Offset of 16 bytes in EEPROM
buffOff=1
strOff=1
loopMove2:
  BUF(buffOff)=Asc(Mid$(productID$,strOff,1))
  buffOff=buffOff+1
  strOff=strOff+1
If buffOff<13 GoTo loopMove2
BUF(13)=0
BUF(14)=0
BUF(15)=0
BUF(16)=0

I2C WRITE i2cAdr, 0, 17, BUF():'Set pointer to the next block
If MM.I2C<>0 Then Print "STATUS error";MM.I2C
Pause EEWrSpeed

BUF(0)=80:'Offset of 16 bytes in EEPROM
For buffOff=1 To 16
  BUF(buffOff)=0
Next buffOff
I2C WRITE i2cAdr, 0, 17, BUF():'Set pointer to the next block
If MM.I2C<>0 Then Print "STATUS error";MM.I2C
Pause EEWrSpeed

Print "Write Complete"
exitProg:
End
