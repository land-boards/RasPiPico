I2C OPEN 400,100
I2C write &h42,0,3,1,2,3 'switch to mode 3
Pause 2000
color 63,0
start:
PropellerCMD 204
r=Int(Rnd*2)+2
For t = 1 To 90 Step r
  line 1,1,90,t,c
  line 1,1,t,90,c
Next t
Pause 500 : Rem it a chance to be displayed
c=Int(Rnd*63)+1
color c,0
GoTo start

Sub color a,b
 I2C write &h42,0,4,1,205,a,b
End Sub

Sub line a,b,c,d,e
 I2C write &h42,0,7,1,202,a,b,c,d,e
 Pause 20
End Sub

Sub PROPELLER register,data
  I2C WRITE &h42,0,2,register,data
End Sub

Sub PropellerCMD command
  propeller 1,command
End Sub                                                                                       