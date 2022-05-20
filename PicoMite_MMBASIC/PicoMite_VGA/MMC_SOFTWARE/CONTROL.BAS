'CONTROL.BAS - Requires CONTROL.SCR, CONTROL.SPR, CONTROL.FNT
'By Jeff Ledger
I2C open 400,100
PC = &h42 : Wii = &h52
I2C write PC,0,3,1,2,4 'Switch to mode4.mde
DO WHILE ASC(a$) <> 42 : a$=INKEY$ : LOOP
SETPIN 2,DOUT
DIM RDBuff(25)

'' Load Resources.  New resources can be loaded at any time.
LoadResource "control.scr",182 'Load Background Screen
LoadResource "control.spr",180 'Load Sprites
LoadResource "control.fnt",181 'Load fonts/tiles

'' Control #1 position
c1x = 60
c1y = 100
c1pos = c1y + 40

'' Control #2 position
c2x = 90
c2y = 90
c2pos = c2x + 8

' Load Sprites: Higher numbered sprites layer on top of lower numbered sprites.
LoadSpr 15,0,100,175,0,0       'Load Pointer onto top layer
LoadSpr 2,2,c1x,c1y,0,0        'Load top of slider
LoadSpr 3,1,c1x,c1y+16,0,0     'Load center of slider
LoadSpr 4,1,c1x,c1y+32,0,0     'Load center of slider
LoadSpr 6,3,c1x,c1y+48,0,0     'Load bottom of slider
LoadSpr 7,4,c1x,c1pos,0,0      'Load button onto layer above sliders

LoadSpr 8,7,c2x,c2y,0,0        'Load Left side of slider
LoadSpr 9,8,c2x+16,c2y,0,0     'Load Center of slider
LoadSpr 10,8,c2x+32,c2y,0,0    'Load Center of slider
LoadSpr 11,8,c2x+48,c2y,0,0    'Load Center of slider
LoadSpr 12,9,c2x+64,c2y,0,0    'Load Right side of Slider
LoadSpr 13,10,c2pos,c2y,0,0    'Load button onto slider

LoadSpr 14,11,0,0,0,0

WiiInit
x=100 : y=175 'default position of the pointer
DefPaletteColor 1,0,20
'' Toss some text on the screen DisplayText x,y,palette,string$
DisplayText 6,3,1,"MODE4 CONTROL DEMO"

'' Start Program Logic

main:
ReadWii
LoadSpr 15,0,x,y,0,0
counter = counter + 1

'' Slider Control #1
IF bt=1 AND y < c1y+44 AND y > c1y+10 AND x < c1x+20 AND x > c1x-10 THEN
   c1pos=y
   LoadSpr 7,4,c1x,y,0,0
   rate$=STR$(c1pos)
   DisplayText 6,20,1,rate$
ENDIF

'' Slider Control #2
IF bt=1 AND x < c2x+55 AND x > c2x+8 AND y < c2y+20 AND y > c2x-10 THEN
   c2pos=x
   LoadSpr 13,10,x,c2y,0,0
   rate$=STR$(c2pos)
   DisplayText 14,10,1,rate$
ENDIF

'' Flash an LED. Speed depends on position of control #1
IF counter > c1pos - 110 THEN
   counter =0
   PIN(2) = 1
   PAUSE 5
   PIN(2) = 0
   PAUSE 5
ENDIF

PAUSE 20

GOTO main

SUB ReadWii
  bt=0
  I2C write Wii,0,1,&h0
  I2C read Wii,0,6,RDBuff(0)
  IF RDbuff(0) > CaliX+25 THEN x=x+3
  IF RDbuff(0) < CaliX-25 THEN x=x-3
  IF RDbuff(1) > CaliY+25 THEN y=y-3
  IF RDbuff(1) < CaliY-25 THEN y=y+3
  '' Check for button press / Demo does not check for x position.
  IF (RDbuff(5) AND &h01) = 0 THEN bt=1
  IF (RDbuff(5) AND &h02) = 0 THEN
      animate 14,11,12,30
      MoveSpeed 14,100,250,2,2
  ENDIF
  PAUSE 3
END SUB

SUB WiiInit
  I2C write Wii,0,2,&hf0,&h55
  I2C write Wii,0,1,&h0
  I2C read Wii,0,6,RDBuff(0)
  PAUSE 1000
  CaliX = RDBuff(0)
  CaliY = RDBuff(1)
  PAUSE 1000
END SUB


SUB LoadResource file$,cmd
  I2C WRITE PC,0,2,1,230 'Clear the I2C registers
  FOR X = 1 TO LEN(file$) 'Send filename
     I2C WRITE PC,0,2,x+1,ASC(MID$(file$,x,1)) : NEXT X
  I2C WRITE PC,0,2,1,cmd 'Initate Load 180 or 181
  a$ ="": DO WHILE a$ <> "*" : a$=INKEY$ : LOOP
END SUB

SUB Animate sprite,start,end,delay
 I2C WRITE PC,0,6,1,219,sprite,start,end,delay

END SUB

SUB LoadSpr sprite,graphic,x,y,mirror,palette
  I2C WRITE PC,0,8,1,202,sprite,graphic,x,y,mirror,palette
END SUB

SUB MoveSpeed sprite,xdelay,ydelay,xinc,yinc
  I2C Write PC,0,7,1,220,sprite,xdelay,ydelay,xinc,yinc
END SUB

SUB MoveSprite sprite,xpos,ypos
  I2C Write PC,0,5,1,203,sprite,xpos,ypos
END SUB

SUB DisplayText textx,texty,color,text$
 FOR distext = 1 TO LEN(text$) : char = ASC(MID$(text$,distext,1))
   I2C write PC,0,6,1,200,textx+distext,texty,color,char
   NEXT distext
END SUB

SUB DisplayChar charx,chary,color,char
  I2C write PC,0,6,1,200,charx,chary,color,char
END SUB

SUB DefPaletteColor PalNum,Index,color
  I2C Write PC,0,5,1,240,PalNum,Index,color
END SUB   
