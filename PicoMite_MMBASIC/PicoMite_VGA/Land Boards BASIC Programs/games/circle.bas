'circle, a 2 player game for VGA picomite GAME

'features PIONES - PIO SPI for dual NES controller
'VGApicomiteGAME board revision 1.4

'---------------------------- IO pin configuration -------------------------
'pin asignment K8/sm0 K7/sm1
'data (in)     gp1    gp4
'latch (out)   gp2    gp5
'clk (out)     gp3    gp22

'reserve pins for PIO1 sm0
SetPin gp1,pio1:SetPin gp2,pio1:SetPin gp3,pio1  'sm0
SetPin gp4,pio1:SetPin gp5,pio1:SetPin gp22,pio1 'sm1

'power the 2 ports K8 and K7 if needed
SetPin gp14,dout:Pin(gp14)=1
SetPin gp15,dout:Pin(gp15)=1


'------------------------------ PIO configuration --------------------------
'PIO1 execute frequency
f=100000

'pio config PIO1 sm0
s0=2^19                            'default with IN shift left, OUT shift right
p0=Pio(pinctrl 1,1,1,GP1,GP3,GP2,GP3)  'GP1=IN base GP2=SET GP3=OUT/SIDESET

'pio config PIO1 sm1
s1=2^19                           'default with IN shift left, OUT shift right
p1=Pio(pinctrl 1,1,1,GP4,GP22,GP5,GP22) 'GP4=IN base, GP5=SET GP22=OUT/SIDESET


'------------------------------ PIO program (comment) -----------------------
'pio1 sm0 program, identical code for sm0 and sm1, clock is side set pin
'adr/instr/comment
'0 E081 'set pindir latch, out     [side 0]
'1 E000 'set latch low             [side 0]
'2 A0EB 'MOV null inverted to OSR  [side 0]
'3 6081 'OUT 1 bit OSR to pindirs  [side 0] set clock output

'4 E001 'latch high                [side 0] latch pulse
'5 E000 'latch low                 [side 0]
'6 E027 'load X with value 7       [side 0]
'7 A0C3 'mov NULL to ISR           [side 0]

'8 4001 'shift 1 bit data in ISR   [side 0]
'9 1048 'JMP X-- to &h8            [side 1] this is the clock pulse
'A 8000 'push ISR                  [side 0] 8 bits to fifo
'B 0004 'jmp to &h4                [side 0] next cycle

'&h0C....&h1F not used
'------------------------- END PIO program (comment) --------------------------

'pio1 program in data statements
Dim a%(7)=(&h6081A0EBE000E081,&hA0C3E027E000E001,&h0004800010484001,0,0,0,0,0)

'program and start the PIO1 state machines
PIO program 1,a%()      'program PIO1
PIO init machine 1,0,f,p0,,s0,0       'init sm0 from address &h00
PIO init machine 1,1,f,p1,,s1,0       'init sm1 from address &h00 (same code)
PIO start 1,0                         'start PIO1 sm0
PIO start 1,1                         'start PIO1 sm1

Dim h%(5)

'------------------------------ MAIN level -----------------------------------

MODE 2:CLS

'initial values for the players and the target food
'target (green) index 0, left player (K8/port A/blue) index 1
'right player (K7/port B/ red) index 2
sz=MM.HRes/40

'x,y are coordinates, r=radius, c=color, s=speed
x0=MM.HRes/2:y0=MM.VRes/3:r0=sz:c0=RGB(green)
x1=MM.HRes/3:y1=2*MM.VRes/3:r1=sz:c1=RGB(blue):s1=5
x2=2*MM.HRes/3:y2=2*MM.VRes/3:r2=sz:c2=RGB(red):s2=5

introscreen
'u1=0:u2=0  'scores player 1 and 2

'the game uses framebuffer to prevent screen drawing artefacts
FRAMEBUFFER create
FRAMEBUFFER write f

'initial target
Circle x0,y0,r0,,,c0,c0


'---------------------- this is the main game loop --------------------------
'the game stops when one player size exceeds the screen limits

Do
 'read the FIFO's that contain the NES controller keys
 PIO read 1,0,5,h%()  'read 5 words from FIFO sm0
 p1=255-h%(4)         'converto to value 0-15
 PIO read 1,1,5,h%()  'read 5 words from FIFO sm1
 p2=255-h%(4)         'converto to value 0-15

 'wipe old positions players
 Circle x1,y1,r1,,,0,0
 Circle x2,y2,r2,,,0,0

 'move players
 v1=0:v2=0
 If (p1 And 2) Then v1=v1+s1:x1=x1-s1
 If (p1 And 1) Then v1=v1+s1:x1=x1+s1
 If (p2 And 2) Then v2=v2+s2:x2=x2-s2
 If (p2 And 1) Then v2=v2+s2:x2=x2+s2
 If (p1 And 8) Then v1=v1+s1:y1=y1-s1
 If (p1 And 4) Then v1=v1+s1:y1=y1+s1
 If (p2 And 8) Then v2=v2+s2:y2=y2-s2
 If (p2 And 4) Then v2=v2+s2:y2=y2+s2

 'allow wrap around
 If x1<0 Then x1=x1+MM.HRes
 If x1>MM.HRes Then x1=x1-MM.HRes
 If y1<0 Then y1=y1+MM.VRes
 If y1>MM.VRes Then y1=y1-MM.VRes
 If x2<0 Then x2=x2+MM.HRes
 If x2>MM.HRes Then x2=x2-MM.HRes
 If y2<0 Then y2=y2+MM.VRes
 If y2>MM.VRes Then y2=y2-MM.VRes

 'calculate distances
 d12=Sqr((x1-x2)^2 + (y1-y2)^2)
 d10=Sqr((x1-x0)^2 + (y1-y0)^2)
 d20=Sqr((x0-x2)^2 + (y0-y2)^2)

 'game rules, collision between players is punished
 'player who moves is culprit
 If d12<(r1+r2) Then
   If v1>0 Then r1=r1/2
   If v2>0 Then r2=r2/2
   r1=Max(r1,1)
   r2=Max(r2,1)
 EndIf

 'you eat, you grow....
 If d10<(r1+r0) Then r1=r1*2:newfood
 If d20<(r0+r2) Then r2=r2*2:newfood

 'write new player /target positions and sizes
 Circle x1,y1,r1,,,c1,c1
 Circle x2,y2,r2,,,c2,c2
 Circle x0,y0,r0,,,c0,c0

 'decide winner
 If r1>MM.VRes/2 Then
   Text MM.HRes/2,MM.VRes/2,"Blue Wins","CM",1,1,RGB(yellow)
   u1=u1+1
   prestart
 EndIf
 If r2>MM.VRes/2 Then
   Text MM.HRes/2,MM.VRes/2,"Red Wins","CM",1,1,RGB(yellow)
   u2=u2+1
   prestart
 EndIf

 'score update
 Text 0,0,Str$(u1),"LT",1,1,RGB(blue)
 Text MM.HRes,0,Str$(u2),"RT",1,1,RGB(red)

 'update screen
 FRAMEBUFFER copy f,n,b
 Pause 50
Loop Until p1=&h40 'loop until B key press

CLS
Text MM.HRes/2, MM.VRes/2, "Bye","CM",1,1,RGB(yellow)
FRAMEBUFFER copy f,n,b
End


'show player info and hold restart until controller key pressed
Sub prestart
 Text MM.HRes/2,30+MM.VRes/2,"A=restart, B=stop","CM",1,1,RGB(yellow)
 FRAMEBUFFER copy f,n,b

 Do
   Pause 50
   PIO read 1,0,5,h%()  'read 5 words from FIFO sm0
   p1=255-h%(4)         'convert to value 0-255
 Loop Until p1=&h80 Or p1=&h40

 CLS
 'x,y are coordinates, r=radius, c=color, s=speed
 x0=MM.HRes/2:y0=MM.VRes/3:r0=sz:c0=RGB(green)
 x1=MM.HRes/3:y1=2*MM.VRes/3:r1=sz:c1=RGB(blue):s1=5
 x2=2*MM.HRes/3:y2=2*MM.VRes/3:r2=sz:c2=RGB(red):s2=5

 'initial target
 Circle x0,y0,r0,,,c0,c0
End Sub


'seed new green circle
Sub newfood
 Circle x0,y0,r0,,,0,0
 x0=MM.HRes*Rnd()
 y0=MM.VRes*Rnd()
End Sub


'the introscreen
Sub introscreen
 CLS
 Text MM.HRes/2,MM.VRes/2-50,"CIRCLE","CM",1,2,RGB(yellow)
 Pause 2000
 Text MM.HRes/2,MM.VRes/2,"a 2 player game","CM",1,1,RGB(yellow)
 Pause 1500
 Text MM.HRes/2,MM.VRes/2+20,"for VGA picomite GAME","CM",1,1,RGB(yellow)
 Pause 1500
 Text MM.HRes/2,MM.VRes/2+40,"and 2 NES controllers","CM",1,1,RGB(yellow)
 Pause 1500
 Box 0,MM.VRes/2-10,MM.HRes,MM.VRes,,0,0
 Pause 1500
 Text MM.HRes/2,MM.VRes/2,"arrows to move, B to stop","CM",1,1,RGB(yellow)
 Pause 1500
 Text MM.HRes/2,MM.VRes/2+20,"eat green to grow and win","CM",1,1,RGB(yellow)
 Pause 1500
 Text MM.HRes/2,MM.VRes/2+40,"avoid collisions","CM",1,1,RGB(yellow)
 Pause 1500
 Text MM.HRes/2,MM.VRes/2+60,"--- GET READY ---","CM",1,1,RGB(yellow)
 Pause 2000
End Sub