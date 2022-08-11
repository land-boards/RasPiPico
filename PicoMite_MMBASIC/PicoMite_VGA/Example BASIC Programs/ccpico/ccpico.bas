' ChemiChaos
'OPTION EXPLICIT  ' all variables must be defined before use - HAH! Bite me.
'
' by vegipete, December 2020
'
' required support files (place in same directory):
' ChemiChaosSprites.png
' CCRedHand.spr
'
' Sort the Chemical Balls in the Test ;
' Move with the arrow keys
' Pick and place with the space bar
' Restart level with R  (upper or lower case for letter commands)
' Toggle stats with C
' Quit with Esc
' Replace picked up ball with Z (undocumented)
'
'   v 1.2   Mouse improvements, more levels, key press & mouse click counting
'   v 1.1   Mouse control, change to level numbering
'   v 1.0   Original release, graphics tweaks, more levels + counter, intro text
'   v <     development
'
'===========================================
'option console screen
const UP = 128
const DOWN = 129
const LEFT = 130
const RIGHT = 131
const HOME = 134
const ENDE = 135
dim cputype%=0, f4%=4, cmm2%=0, ls$, fnt, nBalls=7, blankSprite=12, ballRad=13

if instr(MM.Device$,"Colour Maximite") = 0 then cputype%=f4% ' or picomite, MM+, etc.

if cputype%=f4% then ' armmite F4, picomite, etc
  F4_setup ' establish 12 sprites (blit buffers #1-12)
else ' CMM2
  mode 1,16   ' more colours for better ball shading (800x600)
endif

dim keypress
dim fname$
dim levelnum
dim movingball
dim vials(9,4)     ' 9 vials, each can hold 4 balls
dim vialsbak(9,4)  ' 9 vials, restore copy
dim ballcols(6) = (0,1,2,3,4,5,6) ' allow shuffling of ball colours
dim m$
dim integer movecount(8)
dim countvisible = 0

if cputype%=cmm2% then ' CMM2
  sp = 1    ' sprite page
  page write sp : cls
  load png "ChemiChaosSprites.png"
  sprite read 1,36,180,36,36,sp    ' up arrow
  open "CCRedHand.spr" for random as #1
  seek #1, 324
  movecount(7) = val(input$(12,#1))
  seek #1, 342
  movecount(8) = val(input$(12,#1))
  close #1
 gui cursor load "CCRedHand.spr"

  mport = mm.info(option mouse) + 1
  if mport then
    controller mouse open mport-1, LeftClick
    gui cursor on 2
    settick 50, PeriodInt   ' periodic interrupt to move mouse pointer
  else    ' no mouse set so try find one
    mport = 4
    do
      on error skip
      controller mouse open mport-1, LeftClick
      if MM.ERRNO then
        mport = mport - 1
      else
        gui cursor on 2
        settick 50, PeriodInt   ' periodic interrupt to move mouse pointer
        exit do
      endif
    loop until mport = 0  ' no mouse found anywhere
  endif
  pause 1000    ' screen sync time
  page write 0 : cls rgb(white)
endif

levels = 1
levelnum = 0
levelball = 0
restore gamelevels
ShuffleBCols
xpos = 1
gameover = 0
handx = MM.HRES/2
handy = MM.VRES/2
newclick = 0

charx = 245 : chary = 10
if mport then gui cursor hide
PutString("CHEMICAL CHAOS")
if cputype%=CMM2% then 
  line 240,37,545,37,4,0
  text 565,20,"v1.2    by vegipete, Jan 2021","LT",1,1,0,rgb(white)
else
  ls$="v1.2    by vegipete, Jan 2021 (F4 by lizby)"
  ly%=mm.fontheight+2
  if mm.device$="RP2040 PicoMite" then 
    font 1 
    blankSprite=7
  else 
    font 7
  endif
  lx%=mm.hres/2 - len(ls$)/2*mm.fontwidth
  text lx%,ly%,ls$,,,,,rgb(black),rgb(white)
  ly%=ly%+mm.fontheight+2
  if mm.device$="" then nBalls=6
endif
charx = 0 : chary = 50
'          12345678901234567890123456789012345678
PutString("There is chaos and confusion all")
charx = 0 : chary = chary + 30
PutString("through the chemistry lab! The")
charx = 0 : chary = chary + 30
PutString("chemicals have been mixed. Your task")
charx = 0 : chary = chary + 30
PutString("is to combat the chaos and restore")
charx = 0 : chary = chary + 30
PutString("order by sorting the chemicals.")
charx = 0 : chary = chary + 30
PutString("Move the coloured balls one by one")
charx = 0 : chary = chary + 30
PutString("until each test tube contains a single")
charx = 0 : chary = chary + 30
PutString("colour. However, you can not drop a")
charx = 0 : chary = chary + 30
PutString("ball on a different coloured one!")
charx = 0 : chary = chary + 30
PutString("Use the arrow keys to move. Select and")
charx = 0 : chary = chary + 30
PutString("drop with the space bar. Press R to")
charx = 0 : chary = chary + 30
PutString("restart if you get stuck.")
charx = 0 : chary = chary + 55
PutString("Press Esc to quit and call HazMat.")
if mm.device$="RP2040 PicoMite" then fnt=1 else fnt=7
text 30,470,"Press 'C' to toggle stats.","LT",fnt,1,0,-1

' add some colour to the info screen
if cputype%=CMM2% then
  blit 0,0*36, 20,560,36,36,sp,4 ' coloured ball
  blit 0,1*36,175, 10,36,36,sp,4 ' coloured ball
  blit 0,2*36,685, 70,36,36,sp,4 ' coloured ball
  blit 0,3*36,750,185,36,36,sp,4 ' coloured ball
  blit 0,4*36, 70,405,36,36,sp,4 ' coloured ball
  blit 0,5*36,540,380,36,36,sp,4 ' coloured ball
  blit 0,6*36,620,500,36,36,sp,4 ' coloured ball

  if mport then gui cursor show

  pause 500
else
  blit write #1,10,1,26,26
  blit write #3,290,1,26,26
  blit write #4,280,79,26,26
  blit write #5,10,215,26,26
  blit write #6,288,210,26,26
endif

charx = 0 : chary = 500
PutString("Press a key to begin...")

do while inkey$ <> "" : loop    ' clear any key presses
do : loop until (_inkey$() <> "") or newclick    ' wait for key press
newclick = 0
cls rgb(white)
charx = 245 : chary = 10
PutString("CHEMICAL CHAOS")
if cputype%=CMM2% then 
  line 240,37,545,37,4,0
  text 565,20,"v1.2    by vegipete, Jan 2021","LT",1,1,0,rgb(white)
else
  ls$="v1.2    by vegipete, Jan 2021"
  ly%=mm.fontheight+2
  if mm.device$="RP2040 PicoMite" then fnt=1 else fnt=7
  lx%=mm.hres/2 - len(ls$)/2*mm.fontwidth
  text lx%,ly%,ls$,,,,,rgb(black),rgb(white)
  ly%=ly%+mm.fontheight+2
endif
movecount(1) = 0  ' total keypress this run
movecount(4) = 0

'===========================================
do
  if MixVials() = 0 then exit do  ' no more levels
  ShowLevel
  movecount(2) = 0  ' total keypress this level
  movecount(3) = 0  ' total keypress this attempt
  movecount(5) = 0
  movecount(6) = 0
  ShowCounts
  do
    i = GetBall()
    if i then PutBall(i)
    if TestDone() then exit do  ' this level has been defused
    if gameover then exit do    ' player quit
  loop
  if cputype%=CMM2% then sprite hide 1
  if gameover then exit do      ' player quit
  ShowSafe
  ShuffleBCols
  levels = levels + 1
  do : loop until (_inkey$()<> "") or newclick ' wait for keypress
  do : loop until _inkey$() = ""   ' clear any keypresses
  newclick = 0
  if cputype%=f4% then 
    box 0,32,320,208,0,rgb(white),rgb(white)
    ly%=32 ' where "Level #" will be printed
  endif
loop

if mport then
  settick 0, PeriodInt
  controller mouse close mport-1
  pause 20
endif

open "CCRedHand.spr" for random as #1
seek #1, 324
print #1, left$(str$(movecount(7)) + space$(15), 12);
seek #1, 342
print #1, left$(str$(movecount(8)) + space$(15), 12);
close #1
if mport then gui cursor off

charx = 20
chary = 400
if gameover then      ' player quit
  PutString("Send in the HazMat team...")
else
  PutString("All is well. Chaos has been tamed.")
  pause 2500

  mode 1,8
endif
print "Type 'RUN' to play again."

end

'===========================================
sub LeftClick
  newclick = 1
  handx = mouse(x,mport-1)
  handy = mouse(y,mport-1)
  if mport then gui cursor handx,handy
end sub

'===========================================
sub PeriodInt
  if mport then gui cursor mouse(x,mport-1),mouse(y,mport-1)
end sub

'===========================================
' Draw the pth vial, erasing anything that might be there already
' p = [1,nvials]
sub ShowVial(p)
  if mport then gui cursor hide
  if cputype%=CMM2% then
    blit 36,0,80*p-40,200,66,168,sp
  else
    blit write #9,46*(p-1),100,46,120
  endif 
  if mport then gui cursor show
end sub

'===========================================
' Draw ball number n at yth level in xth vial
' n = [1-7] colour
' x = [1-9] vial
' y = [0-4] level (0 = above)
sub DrawBall(n,x,y)
  local ypos
  if cputype%=CMM2% then
    if y then
      ypos = 397-36*y   ' ball in vial
    else
      ypos = 120        ' ball above vial
    endif
    if mport then gui cursor hide
    if n then
      blit 0,ballcols(n-1)*36,80*x-25,ypos,36,36,sp,4 ' coloured ball
    else
      blit 36,216,80*x-25,ypos,36,36,sp,4  ' blank ball
    endif
  else
    if y then
      ypos = 238-26*y  ' ball in vial
    else
      ypos = 48        ' ball above vial
    endif
'    blit write #9,46*(x-1)+9,100,26,26
    if n then ' coloured ball
      blit write n,46*(x-1)+9,ypos,26,26
    else ' blank ball
      blit write blankSprite,46*(x-1)+9,ypos,26,26
    endif
  endif 
  if mport then gui cursor show
end sub

'===========================================
' Shuffle the ball colours around for variety
sub ShuffleBCols
  local i, tmp, n
  for i = 0 to nBalls-1
    n = int(rnd * nBalls)
    tmp = ballcols(i)
    ballcols(i) = ballcols(n)
    ballcols(n) = tmp
  next i
end sub

'===========================================
' Show current level
' nvials is of interest, as is vials(9,4)
sub ShowLevel
  local i,j

  if mport then gui cursor hide
  box 0,95,MM.HRES,310,0,rgb(white),rgb(white) ' erase vials and "SAFE" message
  for i = 1 to nvials
    ShowVial(i)
    for j = 1 to 4
      DrawBall(vials(i,j),i,j+1)
    next j
  next i
  if xpos > nvials then xpos = nvials
  charx = 100 : chary = 400
  if cputype%<>cmm2% then ly%=chary
  PutString("Level " + str$(levelball) + "." + str$(levelnum) + "   ")
  if mport then gui cursor show

end sub

'===========================================
function TestDone()
  local i,j

  TestDone = 0
  for i = 1 to nvials
    for j = 2 to 4
      if vials(i,j) <> vials(i,1) then exit function
    next j
  next i
  TestDone = 1
end function

'===========================================
' return number of ball selected,
' return 0 if no ball selected
function GetBall()
  local k,j,res

  if cputype%=CMM2% then
    sprite show 1,80*xpos-25,160,1,0  ' arrow up
  else
    blit write #8,46*(xpos-1)+9,72,26,26
  endif
  GetBall = 0
  do
    k = asc(ucase$(_inkey$()))
    CheckNewClick(k)
    select case k
      case 130    ' left arrow
        if xpos > 1 then
          AnotherKeyPress
          xpos = xpos - 1
          if cputype%=cmm2% then
            sprite show 1,80*xpos-25,160,1,0    ' draw up arrow
          else
            blit write blankSprite,46*(xpos)+9,72,26,26  ' draw blank
            blit write #8,46*(xpos-1)+9,72,26,26 ' draw up arrow
          endif
        endif
      case 131    ' right arrow
        if xpos < nvials then
          AnotherKeyPress
          xpos = xpos + 1
          if cputype%=cmm2% then
            sprite show 1,80*xpos-25,160,1,0    ' draw up arrow
          else
            blit write blankSprite,46*(xpos-2)+9,72,26,26  ' draw blank
            blit write #8,46*(xpos-1)+9,72,26,26 ' draw up arrow
          endif
        endif
      case  67    ' "C"   toggle count data on and off
        ToggleCounts
      case  27    ' [ESC]
          gameover = 1
          exit do
      case  32    ' [SPACE] - select top ball from this vial
        AnotherKeyPress
        j = 4
        res = 0
        do
          if vials(xpos,j) > 0 then ' found a ball
            res = xpos
            movingball = vials(xpos,j)    ' actual ball colour
            exit do
          endif
          j = j - 1
        loop until j = 0
        if res then
          vials(xpos,j) = 0   ' clear location
          DrawBall(0,res,j+1)   ' erase ball from vial
          GetBall = res
          exit do
        endif
      case 82     ' "R"   restart
        AnotherKeyPress
        movecount(3) = 0  ' total keypress this attempt
        movecount(6) = 0  ' total mouse click this attempt
        ShowCounts
        if cputype%=CMM2% then
          math scale vialsbak(),1,vials()  ' restore starting configuration
        else
          for li%=0 to 9 ' vials(9,4)
            for lj%=0 to 4
              vials(li%,lj%)=vialsbak(li%,lj%)
            next lj%
          next li%
        endif
        ShowLevel
        exit do
    end select
  loop
end function

'===========================================
' take top ball from src and put it somewhere
sub PutBall(src)
  local j,k

  if cputype%=CMM2% then
    sprite show 1,80*xpos-25,160,1,2  ' arrow down
  else
    blit write #10,46*(xpos-1)+9,72,26,26
  endif
  DrawBall(movingball,src,0)   ' coloured ball above vial
  do
    k = asc(ucase$(_inkey$()))
    CheckNewClick(k)
    select case k
      case 130    ' left arrow
        if xpos > 1 then
          AnotherKeyPress
          DrawBall(0,xpos,0)            ' erase coloured ball above vial
          xpos = xpos - 1
          DrawBall(movingball,xpos,0)   ' draw coloured ball above vial
          if cputype%=cmm2% then
            sprite show 1,80*xpos-25,160,1,2    ' draw down arrow
          else
            blit write blankSprite,46*(xpos)+9,46,26,26  ' draw blank over ball
            blit write blankSprite,46*(xpos)+9,72,26,26  ' draw blank over arrow
            blit write #10,46*(xpos-1)+9,72,26,26 ' draw down arrow
          endif
        endif
      case 131    ' right arrow
        if xpos < nvials then
          AnotherKeyPress
          DrawBall(0,xpos,0)            ' erase coloured ball above vial
          xpos = xpos + 1
          DrawBall(movingball,xpos,0)   ' draw coloured ball above vial
          if cputype%=cmm2% then
            sprite show 1,80*xpos-25,160,1,2    ' draw down arrow
          else
            blit write blankSprite,46*(xpos-2)+9,46,26,26  ' draw blank over ball
            blit write blankSprite,46*(xpos-2)+9,72,26,26  ' draw blank over arrow
            blit write #10,46*(xpos-1)+9,72,26,26 ' draw down arrow
          endif
        endif
      case  90    ' [Z/z]   'undocumented' undo command - replace wrongly selected ball
        AnotherKeyPress
        DrawBall(0,xpos,0)  ' erase coloured ball above pointer
        for j = 1 to 4
          if vials(src,j) = 0 then     ' find top spot
            vials(src,j) = movingball  ' put the ball back
            DrawBall(movingball,src,j+1) ' re-draw coloured ball in vial
            exit do
          endif
        next j
      case  67    ' "C"   toggle count data on and off
        ToggleCounts
      case  27    ' [ESC]
          gameover = 1
          exit do
      case  32    ' [SPACE] - place ball
        if cputype%=CMM2% then
          sprite show 1,80*xpos-25,160,1,2    ' draw down arrow
        else
          blit write #10,46*(xpos-1)+9,72,26,26
        endif
        if vials(xpos,4) = 0 then     ' there is room in this vial
          AnotherKeyPress
          top = 0
          for j = 1 to 4
            if vials(xpos,j) = 0 then   ' found a spot
              if (top = 0) or (top = movingball) then
                vials(xpos,j) = movingball    ' move the ball
                DrawBall(0,xpos,0)            ' erase coloured ball above vial
                DrawBall(movingball,xpos,j+1) ' draw coloured ball in vial
                exit do
              endif
            else
              top = vials(xpos,j)
            endif
          next j
        endif
      case 82     ' "R"   restart
        AnotherKeyPress
        movecount(3) = 0  ' total keypress this attempt
        movecount(6) = 0  ' total mouse click this attempt
        ShowCounts
        if cputype%=CMM2% then 
          math scale vialsbak(),1,vials()  ' restore starting configuration
        else ' vials(9,4)
          for li%=0 to 9: for lj%=0 to 4
            vialsbak(li%,lj%)=vials(li%,lj%)
          next lj%: next li%
        endif
        ShowLevel
        exit do
    end select
  loop
  movingball = 0
end sub

'===========================================
sub CheckNewClick(k)
  if newclick then
    newclick = 0
    if handy > 208 and handy < 350 then
      select case handx
        case  45 to  85
          PickVial(1,k)
        case 125 to 165
          PickVial(2,k)
        case 205 to 245
          PickVial(3,k)
        case 285 to 325
          PickVial(4,k)
        case 365 to 405
          PickVial(5,k)
        case 445 to 485
          PickVial(6,k)
        case 525 to 565
          PickVial(7,k)
        case 605 to 645
          PickVial(8,k)
        case 685 to 725
          PickVial(9,k)
      end select
    endif
  endif
end sub

'===========================================
sub PickVial(n,k)
  local c$
  if nvials >= n then
    if mport then gui cursor hide
    DrawBall(0,xpos,0)
    xpos = n
    DrawBall(movingball,xpos,0)
    if mport then gui cursor show
    movecount(1) = movecount(1) - 1  ' undo keypress this game
    movecount(2) = movecount(2) - 1  ' undo keypress this level
    movecount(3) = movecount(3) - 1  ' undo keypress this attempt
    movecount(7) = movecount(7) - 1  ' remove impending spurious keycount increment
    movecount(4) = movecount(4) + 1  ' another mouse click this game
    movecount(5) = movecount(5) + 1  ' another mouse click this level
    movecount(6) = movecount(6) + 1  ' another mouse click this attempt
    movecount(8) = movecount(8) + 1
    k = 32    ' simulate space bar to grab/release ball
  endif
end sub

'===========================================
sub AnotherKeyPress
  local c$
  movecount(1) = movecount(1) + 1  ' another keypress this level
  movecount(2) = movecount(2) + 1  ' another keypress this level
  movecount(3) = movecount(3) + 1  ' another keypress this attempt
  movecount(7) = movecount(7) + 1
  ShowCounts
end sub

'===========================================
sub ShowCounts
  if countvisible then
    text 165,572," "+str$(movecount(1))+" ","CT",7,1,0,&hFFFFFF
    text 165,562," "+str$(movecount(2))+" ","CT",7,1,0,&hFFFFFF
    text 165,552," "+str$(movecount(3))+" ","CT",7,1,0,&hFFFFFF
    text 165,582," "+str$(movecount(7))+" ","CT",7,1,0,&hFFFFFF
    text 235,572," "+str$(movecount(4))+" ","CT",7,1,0,&hFFFFFF
    text 235,562," "+str$(movecount(5))+" ","CT",7,1,0,&hFFFFFF
    text 235,552," "+str$(movecount(6))+" ","CT",7,1,0,&hFFFFFF
    text 235,582," "+str$(movecount(8))+" ","CT",7,1,0,&hFFFFFF
  end if
end sub

'===========================================
sub ToggleCounts
  if countvisible then
    countvisible = 0
    box 50,530,225,62,1,&hFFFFFF,&hFFFFFF
  else
    countvisible = 1
    text 165,530,"Key","CT",7,1,0,&hFFFFFF
    text 165,540,"Strokes","CT",7,1,0,&hFFFFFF
    text 235,530,"Mouse","CT",7,1,0,&hFFFFFF
    text 235,540,"Clicks","CT",7,1,0,&hFFFFFF
    text 125,550,"This attempt","RT",7,1,0,&hFFFFFF
    text 125,560,"This level","RT",7,1,0,&hFFFFFF
    text 125,570,"This game","RT",7,1,0,&hFFFFFF
    text 125,580,"Total","RT",7,1,0,&hFFFFFF
    box 130,550,141,41,1,0
    line 130,560,270,560,1,0
    line 130,570,270,570,1,0
    line 130,580,270,580,1,0
    line 200,550,200,589,1,0
    ShowCounts
  end if
end sub

'===========================================
sub PutChar(c$)
  local cn = asc(c$) - 32

  if cn < 0 then exit sub
  if cn > 95 then exit sub
  blit 102+(cn mod 16)*21,int(cn/16)*30,charx,chary,21,30,sp
  charx = charx + 21
  if charx > MM.HRES then
    charx = 0
    chary = chary + 30
  endif
end sub

'===========================================
sub PutString(s$)
  local i
  if cputype%=cmm2% then
    for i = 1 to len(s$)
      PutChar(mid$(s$,i,1))
    next i
  else ' Armmite F4
    if chary = 10 then ' first line
      font 2
      i=mm.hres/2 - len(s$)/2*mm.fontwidth
      text i,0,s$,,,,rgb(black),rgb(white)
      ly%=mm.fontheight+14
    else
      font 1
      text 0,ly%,s$,,,,rgb(black),rgb(white)
      ly%=ly%+mm.fontheight
    endif
  endif
end sub

'===========================================
sub ShowSafe
  if cputype%=CMM2% then
    rbox MM.HRES/2-220, 95,360,100,50,&h00FF21,&h00FF21   ' greenish frame
    rbox MM.HRES/2-210,105,340, 80,40,rgb(white),rgb(white)
    blit 180,180,MM.HRES/2-170,110,260,70,sp
  else
    box 0,44,320,54,,rgb(white),rgb(white)
    if mm.device$="RP2040 PicoMite" then
      text mm.hres/2,ballRad*3," S A F E ! ",C,2,3,rgb(yellow),rgb(green)
    else
      blit write #11,130,44,190,48 ' draw SAFE!
    endif
  endif
end sub

'===========================================
' Read another level from the data
function MixVials()
  local i,j,mix
  local c$

  if cputype%=CMM2% then
    math set 0,vials()    ' start with empty vials
  else
    for li%=0 to 9 ' vials(9,4)
      for lj%=0 to 4
        vials(li%,lj%)=0
      next lj%
    next li%
  endif
  read nballs
  if nballs = 0 then
    MixVials = 0
    exit function
  endif

  if cputype%=CMM2% then page write 2    ' don't want to see prints on screen
  print levels,   ' will appear on console if attached

  if nballs = levelball then
    levelnum = levelnum + 1
  else
    levelnum = 1
    levelball = nballs
  endif

  m$ = ""
  read nvials
  if nvials = -1 then
    nvials = nballs + 2
    for i = 1 to nballs
      m$ = m$ + string$(4,str$(i))
    next i
    for i = 1 to len(m$)  ' scramble m$
      c$ = mid$(m$,i,1)
      j = int(rnd * len(m$)) + 1
' LB      mid$(m$,i,1) = mid$(m$,j,1)
' LB      mid$(m$,j,1) = c$
          ls$ = mid$(m$,j,1)
          m$=mid$(m$,1,i-1)+ls$+mid$(m$,i+1)
          m$=mid$(m$,1,j-1)+c$+mid$(m$,j+1)
    next i
    print "random   ";
    for i = len(m$) to 1 step -1
      print mid$(m$,i,1);
    next i
    print
    for i = 1 to nvials   ' fill with data from m$
      for j = 1 to 4
        vials(i,j) = val(left$(m$,1))
        m$ = mid$(m$,2)
      next j
    next i

  else
    for i = 1 to nvials   ' fill with data
      read mix
      m$ = m$ + str$(mix)
      for j = 1 to 4
        vials(i,j) = mix mod 10
        mix = mix \ 10
      next j
    next i
    print "built in "; m$
  endif
  if cputype%=CMM2% then 
    math scale vials(),1,vialsbak()  ' save starting configuration
    page write 0
  else
    for li%=0 to 9 ' vials(9,4)
      for lj%=0 to 4
        vialsbak(li%,lj%)=vials(li%,lj%)
      next lj%
    next li%
  endif
  MixVials = 1
end function

'===========================================
sub F4_setup
  if mm.device$="RP2040 PicoMite" then k=10 else k=12
  Load IMAGE "ChemiChaosSprites320b.bmp"
'  for li%=1 to 12:  On error skip: Blit CLOSE li%: next li%
  On error skip: Blit CLOSE 1
  On error skip: Blit CLOSE 2
  On error skip: Blit CLOSE 3
  On error skip: Blit CLOSE 4
  On error skip: Blit CLOSE 5
  On error skip: Blit CLOSE 6
  On error skip: Blit CLOSE 7
  On error skip: Blit CLOSE 8
  On error skip: Blit CLOSE 9
  On error skip: Blit CLOSE 10
  Blit READ #1,0,0,26,26
  Blit READ #2,0,25,26,26
  Blit READ #3,0,51,26,26
  Blit READ #4,0,76,26,26
  Blit READ #5,0,102,26,26
  Blit READ #6,0,127,26,26
  if mm.device$<>"RP2040 PicoMite" then   
    Blit READ #7,0,152,26,26
    Blit read #11,130,130,190,48 ' "SAFE!"
  endif
  Blit READ #8,26,126,26,26 ' up arrow
  Blit READ #9,27,0,46,120 ' test tube
  Load IMAGE "DownArrow.bmp"
  Blit Read #10,0,0,26,26 ' down arrow
  CLS RGB(white)
  if mm.device$<>"RP2040 PicoMite" then 
    Blit read blankSprite,0,0,26,26 ' blank 
  else
    Blit read #7,0,0,26,26 ' blank sprite
  endif
  CLS RGB(white)
  vialTop%=100/320*mm.hres
  insertKy$=""
  if mm.device$<>"RP2040 PicoMite" then gui interrupt touchInt
end sub

Function _inkey$() ' translates PC console special keys
  local ky$
  if insertKy$<>"" then
    _inkey$=insertKy$ ' use touch character
    insertKy$=""
    exit function
  endif
  ky$=inkey$
  if asc(ky$)=27 then ' escape sequence
    ky$=inkey$: ky$=inkey$: ky$=ucase$(ky$)
'    if ky$<>"" then print ky$;"-";asc(ky$);" ";
    If ky$="A" Then: ky$= chr$(UP) ' chr$(128) ' up arrow
    ElseIf ky$="B" Then: ky$= chr$(DOWN) ' chr$(129) ' down arrow
    ElseIf ky$="C" Then: ky$= chr$(RIGHT) ' chr$(131) ' right arrow
    ElseIf ky$="D" Then: ky$= chr$(LEFT) ' chr$(130) ' left arrow
    ElseIf ky$="1" Then: ky$= chr$(HOME) ' chr$(134) ' home
    ElseIf ky$="4" Then: ky$= chr$(ENDE) 'chr$(135) ' end
    ElseIf ky$="5" Then: ky$=chr$(136) ' pgdn
    ElseIf ky$="6" Then: ky$=chr$(137) ' pgup
    ' Function keys are ???
    EndIf
  endif
'  if ky$<>"" then print asc(ky$);" ";
  _inkey$=ky$
end function

sub touchInt ' used with F4, picomite, etc.; resulting char inserted in _inkey$()
  if touch(X)<>-1 then tx%=touch(X)
  if touch(Y)<>-1 then ty%=touch(Y) 
  if tx%<>oldtx% or ty%<>oldty% then
' if ball selected, left/right of arrow moves ball, below drops; else above lifts
    if ty%<vialTop% then ' right or left
      if tx% > mm.hres/14*13 then ' off-screen vial
        ttx%=7
      else
        ttx%=tx%/(mm.hres/6)+1 ' which vial
      endif
      if ttx%>xpos then: insertKy$= chr$(RIGHT) ' chr$(131) ' right
      elseif ttx%<xpos then: insertKy$= chr$(LEFT) ' chr$(130) ' left
      endif
    else
      insertKy$=" " ' ball up or down if possible
    endif
    print asc(insertKy$);" ";
  endif
end sub

'===========================================
' data format:
' #balls, #vials, ball pattern for each vial
' if #vials = -1 then fill (#balls + 2) vials ly
gamelevels:
data 1,2,11,11    ' trivial one to get started
data 2,3,1221,2112,0
data 2,3,1212,2121,0
data 2,5,1121,1211,2111,1112,0
data 3,5,1231,3321,2231,0,0   ' this one can't be done in only 4 vials
data 3,-1
data 3,-1
data 3,-1
data 3,4,1213,2231,3213,0
data 3,4,2313,2123,2311,0
data 3,4,2313,2232,1113,0
data 3,4,1211,3323,1232,0
data 3,4,2132,3221,3311,0
data 3,4,2221,2133,1133,0
'data 3,4,1233,1212,2133,0
'data 3,4,1232,3121,1323,0
data 4,-1
data 4,-1
data 4,-1
data 4,-1
data 4,5,2321,1412,4412,4333,0
data 4,5,4232,1231,2443,3114,0
data 4,5,3124,2121,2444,3331,0
data 4,5,1224,4232,4311,4133,0
data 4,5,2141,4333,3242,4121,0
data 5,-1
data 5,-1
data 5,-1
data 5,-1
data 5,-1
data 5,-1
data 5,6,5154,3321,4222,3415,1534,0
data 5,6,4131,2422,5412,5313,4553,0
data 5,6,2154,2543,5414,2251,1333,0
data 5,6,3214,2453,2431,2135,5541,0
data 6,-1
data 6,-1
data 6,-1
data 6,-1
data 6,-1
data 6,7,1642,5624,4161,3623,5432,5315,0
data 6,7,6413,5124,3531,4265,6153,6224,0
data 6,7,6342,5265,2341,4534,6152,6131,0
data 6,7,2151,5634,6436,6514,3431,2225,0
data 6,7,1324,4636,5413,6132,5562,5241,0
data 7,-1
data 7,-1
data 7,-1
data 7,-1
data 7,-1
data 7,-1
data 7,-1
data 7,8,2127,4665,4231,3776,4335,6151,4572,0
data 7,8,4311,5562,2346,7773,6245,6124,3751,0
data 7,8,2715,2136,2761,7634,5744,6552,3431,0

data 0,0  ' indicate end of data
