'CHICKRUN.BAS - Requires CHICKRUN.SPR
'By Jeff Ledger
I2C open 400,100
PC = &h42
I2C write PC,0,3,1,2,4 'Switch to mode4.mde
DO WHILE ASC(a$) <> 42 : a$=INKEY$ : LOOP
LoadResource "chickrun.spr",180 'Load Sprites

REM ** Start Chicken Run Demo **
x=10
speed = 30

forward:
LoadSpr 1,3,20,x,0
PAUSE speed : x=x+2
LoadSpr 1,4,20,x,0
PAUSE speed : x=x+2
LoadSpr 1,5,20,x,0
PAUSE speed : x=x+2
IF x > 200 THEN  x=20 : GOTO walkright
GOTO forward

walkright:
LoadSpr 1,9,x,200,1
PAUSE speed : x=x+2
LoadSpr 1,10,x,200,1
PAUSE speed : x=x+2
LoadSpr 1,11,x,200,1
PAUSE speed : x=x+2
IF x > 200 THEN GOTO backward
GOTO walkright

backward:
LoadSpr 1,6,200,x,0
PAUSE speed : x=x-2
LoadSpr 1,7,200,x,0
PAUSE speed : x=x-2
LoadSpr 1,8,200,x,0
PAUSE speed : x=x-2
IF x < 20 THEN  x = 200 : GOTO walkleft
GOTO backward

walkleft:
LoadSpr 1,9,x,20,0
PAUSE speed : x=x-2
LoadSpr 1,10,x,20,0
PAUSE speed : x=x-2
LoadSpr 1,11,x,20,0
PAUSE speed : x=x-2
IF x < 20 THEN GOTO forward
GOTO walkleft


SUB LoadSpr sprite,graphic,x,y,mirror
  I2C WRITE PC,0,7,1,202,sprite,graphic,x,y,mirror
END SUB

SUB LoadResource file$,cmd
  I2C WRITE PC,0,2,1,230 'Clear the I2C registers
  FOR X = 1 TO LEN(file$) 'Send filename
     I2C WRITE PC,0,2,x+1,ASC(MID$(file$,x,1)) : NEXT X
  I2C WRITE PC,0,2,1,cmd 'Initate Load 180 or 181
  a$ ="": DO WHILE a$ <> "*" : a$=INKEY$ : LOOP
END SUB


SUB DisplayText x,y,color,text$
 FOR x = 1 TO LEN(text$) : char = ASC(MID$(text$,x,1))
   I2C write PC,0,6,1,200,textx+x,texty,color,char : NEXT x
END SUB

SUB DisplayChar x,y,color,char
  I2C write PC,0,6,1,200,charx,chary,color,char
END SUB                                                                                                                     