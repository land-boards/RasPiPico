I2CADDR = &h42
I2C OPEN 400, 100
I2C write I2CADDR,0,3,1,2,3 'Switch to mode 3
Pause 2000 : 'Give the Propeller time to switch to MODE 3
I2C write I2CADDR,0,4,1,205,63,1
PropellerCMD 204 :' CLS clear the screen
Randomize 1000

Start:
 a=Int(125*Rnd(1))
 b=Int(100*Rnd(1))
 c=Int(100*Rnd(1))
 d=Int(100*Rnd(1))
 e=Int(63*Rnd(1))

I2C WRITE &h42,0,7,1,202,a,b,c,d,e
GoTo Start

Sub PropellerCMD cmd
  I2C WRITE I2CADDR,0,2,1,cmd
  Pause 100
End Sub                                             