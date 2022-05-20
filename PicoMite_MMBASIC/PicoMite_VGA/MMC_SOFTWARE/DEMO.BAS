' DEMO.BAS : REQUIRES DEMO.SCR, DEMO.SPR, DEMO.FNT
' By Jeff Ledger
I2C open 400,100
PC = &h42
I2C write PC,0,3,1,2,4 'Switch to mode4.mde
DO WHILE ASC(a$) <> 42 : a$=INKEY$ : LOOP

'' Load Resources.  New resources can be loaded at any time.
LoadResource "demo.scr",182 'Load Background Screen
LoadResource "demo.spr",180 'Load Sprites
LoadResource "demo.fnt",181 'Load fonts/tiles

'' Load Sprites 0-15 can be used.
'' LoadSpr sprite,image,x,y,mirror,palette
LoadSpr 1,4,100,175,0,0   'Load Ballon Sprite #1
LoadSpr 2,0,100,100,0,0  'Load Bird Sprite #2
LoadSpr 3,4,100,125,1,0  'Load Ballon Sprte #3
LoadSpr 4,0,125,50,1,0   'Load Bird Sprite #4
LoadSpr 5,4,125,75,0,0   'Load Ballon Sprite #5
LoadSpr 6,0,125,100,0,0  'Load Bird Sprite #6
LoadSpr 7,4,125,125,1,0  'Load Ballon Sprite #7
LoadSpr 8,0,125,100,1,0  'Load Bird Sprite #8
LoadSpr 9,4,125,125,1,0  'Load Ballon Sprite #9

'' Animate a few sprites
'' Animate sprite,start_spr,end_spr,speed
Animate 1,4,7,30 'Animate Ballon  #1 frames 4-7, speed 30
Animate 2,0,3,40 'Animate Bird #2 frames 0-3, speed 40
Animate 3,4,7,10 'Animate Ballon #3 frames 4-7, speed 10
Animate 4,0,3,25 'Animate Bird #4 frames 0-3, speed 25
Animate 5,4,7,15 'Animate Ballon #5 frames 4-7, speed 15
Animate 6,0,3,30 'Animate Bird #6 frames 4-7, speed 30
Animate 7,4,7,20 'Animate Ballon #7 frames 4-7, speed 20
'' Did not animate Sprites 8 and 9 -Sprites will be motionless.

'' Move some sprites around the screen.
'' MoveSpeed sprite,x_delay,y_delay,x_direction,y_direction
MoveSpeed 2,3,5,1,-1  'Move Sprite Bird #2
MoveSpeed 4,5,5,-1,2  'Move Sprite Bird #4
MoveSpeed 6,7,5,1,2   'Move Sprite Bird #6
MoveSpeed 7,2,5,-1,1  'Move Sprite Balloon #7
'' Did not move Sprites 1,3,5,8,9 - Five sprites will be still.

'' Toss some text on the screen DisplayText x,y,palette,string$
DisplayText 2,3,6,"ABCDEFGHIJKLMNOPQRSTUVWXYZ"

'' Start Program Logic
'' All resources will continue until acted upon again below.
main:

PAUSE 20

GOTO main


SUB LoadResource file$,cmd
  I2C WRITE PC,0,2,1,230 'Clear the I2C registers
  FOR X = 1 TO LEN(file$) 'Send filename
     I2C WRITE PC,0,2,x+1,ASC(MID$(file$,x,1)) : NEXT X
  I2C WRITE PC,0,2,1,cmd 'Initate Load 180 or 181
  a$ ="": DO WHILE a$ <> "*" : a$=INKEY$ : LOOP : PAUSE 1000
END SUB

SUB Animate sprite,start,end,delay
 I2C WRITE PC,0,6,1,219,sprite,start,end,delay
 PAUSE 5
END SUB

SUB LoadSpr sprite,graphic,x,y,mirror,palette
  I2C WRITE PC,0,8,1,202,sprite,graphic,x,y,mirror,palette
END SUB

SUB MoveSpeed sprite,xdelay,ydelay,xinc,yinc
  I2C Write PC,0,7,1,220,sprite,xdelay,ydelay,xinc,yinc
END SUB

SUB DisplayText textx,texty,color,text$
 FOR x = 1 TO LEN(text$) : char = ASC(MID$(text$,x,1))
   I2C write PC,0,6,1,200,textx+x,texty,color,char
   PAUSE 1 :NEXT x
END SUB

SUB DisplayChar charx,chary,color,char
  I2C write PC,0,6,1,200,charx,chary,color,char
END SUB

SUB DefPaletteColor PalNum,Index,color
  I2C Write PC,0,5,1,240,PalNum,Index,color
END SUB                             