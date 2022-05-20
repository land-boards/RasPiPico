'ROBODEMO.BAS : Requires ROBODEMO.SPR, ROBODEMO.FNT
'By Jeff Ledger
I2C OPEN 400,100
prop=&h42
I2C write &h42,0,3,1,2,4 'switch to mode 4
Do While A$ <> "*" : a$=Inkey$ : Loop

LoadResource "robodemo.spr",180
LoadResource "robodemo.fnt",181

'' Establish the variables
Dim RDBuff(10)
wii=&h52  'I2C address of Wii Nunchuck
x=64 'starting X position
y=48 'starting Y position

''Define some movement
move=0 : right=1 : left=2 : up=3 : down=4
WiiInit 'Calibrate Wii Initial Settings
Text 1,2,5,"1111111111111111111"
Text 1,3,5,"1000000000000000001"
Text 1,4,5,"1000000000000000001"
Text 1,5,5,"1000000000000000001"
Text 1,6,5,"1000000000000000001"
Text 1,7,5,"1111111100111111111"
Text 8,14,4,"ANIMATION DEMO"
speed=30
leap=2

Main:
ReadWii
count=count+1
If move = 0 Then LoadSpr 1,0,y,x
If move = 0 And count = 100 Then
   LoadSpr 1,9,y,x : Pause 100 : count=0
EndIf

If move = up Then
   x=x-leap : LoadSpr 1,3,y,x : Pause speed
   x=x-leap : LoadSpr 1,2,y,x : Pause speed
   x=x-leap : LoadSpr 1,8,y,x : Pause speed
   count=0
EndIf

If move = down Then
   x=x+leap : LoadSpr 1,1,y,x : Pause speed
   x=x+leap : LoadSpr 1,0,y,x : Pause speed
   x=x+leap : LoadSpr 1,7,y,x : Pause speed
   count=0
EndIf

If move = right Then
  y=y+leap : LoadSpr 1,4,y,x : Pause speed
  y=y+leap : LoadSpr 1,5,y,x : Pause speed
  y=y+leap : LoadSpr 1,6,y,x : Pause speed
  count=0
EndIf

If move = left Then
 y=y-leap : LoadSpr 1,4,y,x,1 : Pause speed
 y=y-leap : LoadSpr 1,5,y,x,1 : Pause speed
 y=y-leap : LoadSpr 1,6,y,x,1 : Pause speed
 count=0
EndIf

GoTo Main

Sub ReadWii
  move=0
  I2C write wii,0,1,&h0
  I2C read wii,0,6,RDBuff(0)
  If RDbuff(0) > CaliX+25 Then move=right
  If RDbuff(0) < CaliX-25 Then move=left
  If RDbuff(1) > CaliY+25 Then move=up
  If RDBuff(1) < CaliY-25 Then move=down
  Pause 3
End Sub

Sub WiiInit
  I2C write wii,0,2,&hf0,&h55 'init Wii
  I2C write wii,0,1,&h0
  I2C read wii,0,6,RDBuff(0)
  CaliX=rdbuff(0)
  CaliY=rdbuff(1)
  Pause 1000
End Sub

Sub Propeller cmd
  I2C write prop,0,2,1,cmd
End Sub

Sub LoadSpr sprite,graphic,x,y,mirror
 I2C WRITE prop,0,7,1,202,sprite,graphic,x,y,mirror
End Sub

Sub LoadResource file$,cmd
  I2C WRITE prop,0,2,1,230
  For X = 1 To Len(file$)
    I2C WRITE prop,0,2,x+1,Asc(Mid$(file$,x,1)) : Next x
  I2C WRITE prop,0,2,1,cmd
  a$="":Do While a$ <> "*" : a$=Inkey$ : Loop
End Sub

Sub MoveSpr ax,ay,bx,by,cx,cy,dx,dy,ex,ey,fx,fy,gx,gy,hx,hy
 I2C write prop,0,18,1,210,ax,ay,bx,by,cx,cy,dx,dy,ex,ey,fx,fy,gx,gy,hx,hy
End Sub

Sub Text textx,texty,color,text$
 For xxx = 1 To Len(text$)
   char = Asc(Mid$(text$,xxx,1))
   I2C write prop,0,6,1,200,textx+xxx,texty,color,char
 Next xxx
End Sub                                                     