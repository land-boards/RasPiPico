' i2cScan.bas
' Scan i2c addresses on I2C0
'
' Doug Gillland
' Land Boards LLC 2022
'

Print "Scan I2C0..."


' Pin setups
SetPin GP0, GP1, I2C:'Uses I2C0
I2C OPEN 400, 100:'400 MHz, 100mS timeout

Dim BUF(2):'Read buffer

i2cAdr=0

goBack:
I2C READ i2cAdr, 0, 1, BUF()
' Print "testing";i2cAdr
If MM.I2C=0 Then Print "Found device at I2C adr=0x";Hex$(i2cAdr)
i2cAdr=i2cAdr+1
If i2cAdr < 128 GoTo goBack

progEnd:
