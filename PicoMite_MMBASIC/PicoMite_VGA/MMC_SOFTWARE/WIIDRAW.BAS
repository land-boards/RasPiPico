'' WIIDRAW.BAS
'' WIIDRAW BY JEFF LEDGER
'' WITH IMPROVEMENTS BY STEDDYMAN

I2C OPEN 50,100
i2caddr = &h52 'Nunchuck Address
I2C write &h42,0,3,1,2,3
Pause 2000
Dim RDbuff(20)
I2C write i2caddr,0,2,&hf0,&h55
I2C write i2caddr,0,2,&hfb,&h0
Pause 500
'Calibrate Starting point for x,y
I2C write i2caddr,0,1,&h0
I2C read i2caddr,0,6,RDBuff(0)
I2C WRITE &h42,0,2,1,204
xx=rdbuff(0)
yy=rdbuff(1)
x=64
y=48
c=22
z=20

ReadWii:
I2C WRITE i2caddr,0,1,&h0
I2C READ i2caddr,0,6,RDBuff(0)
If RDBuff(0) > xx+z Then x=x+1
If RDBuff(0) < xx-z Then x=x-1
If RDBuff(1) > yy+z Then y=y-1
If RDBuff(1) < yy-z Then y=y+1
If (RDBuff(5) And &h01) = 0 Then c=c+1
If (RDBuff(5) And &h02) = 0 Then c=0

I2C WRITE &h42,0,5,1,201,x,y,c
Pause 10

GoTo readwii

Sub PropellerCMD cmd
  I2C WRITE &h42,0,2,1,cmd
End Sub

Sub PropellerVAR register,data
  I2C WRITE &h42,0,2,register,data
End Sub                                                                                  
