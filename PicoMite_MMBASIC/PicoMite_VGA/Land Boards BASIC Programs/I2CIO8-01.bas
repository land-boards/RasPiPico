' Test I2CIO08 card
' Address set to H21
' GPIO0-3 = LEDs = OUT
' GPIO4-7 = Jumpers = IN
SetPin GP14, GP15, I2C2
I2C2 OPEN 400, 100
I2C2 WRITE &H21, 0, 2, &H00, &HF0
I2C2 WRITE &H21, 0, 2, &H01, &HF0
I2C2 WRITE &H21, 0, 2, &H05, &H42
I2C2 WRITE &H21, 0, 2, &H0A, &H05
Pause 1000
I2C2 WRITE &H21, 0, 2, &H0A, &H0A
Pause 1000
