'OPTION EXPLICIT  ' all variables must be defined before use
'
' CrateAway
' Push crates onto targets, but don't push too far.
'
' by vegipete, November 2020
'   version 1.5   Original release, added file reading, graphics tweaks
'   version <     development
'
' Use the arrow keys to move
' Z - undo
' R - restart level
' Q - quit
' L - load levels file
' N - 'undocumented' skip to next level
'
' Find more levels here:
' https: //www.sourcecode.se/sokoban/levels
' or search the internet for "sokoban levels"
' Standard level files with 'txt' format are expected to work.
' Levels that are too wide (>25) or too tall (>18) will be skipped.
'
' The format of a level is
' #######
' #  @  #
' # #.# #
' #  *  #
' # #$# #
' #     #
' #######
' ;Level name or info or whatever."
'
' where:
' Wall                    #
' Player                  @
' Player on goal square   +
' Crate                   $
' Crate on goal square    *
' Goal square             .
' Floor                (Space)
' End of level marker     ; (Following text is name)
'
'===========================================
option default integer
dim cputype%=0, f4%=4, cmm2%=0, ls$, ipsFlag%=0, ky$, pass%=0, imageNo%, xy%, fnt%
if MM.Device$="ARMmite F407" then cputype%=f4%

if cputype%<>cmm2% then ' armmite F4, etc.
else ' CMM2
  mode 1,8 ' 800x600 (F4 with IPS LCD is 800x480)
#Include "GetFileDialog.inc"
endif
cls
' change dialog colours
d_shadow = &h1D3659   ' dark blue brick
d_frame =  &h406086   ' blue brick

dim xMax=25,yMax=18 ' note: F4 is memory-constrained
if mm.hres=320 then
  xMax=fix(mm.hres/19)
  yMax=fix(mm.vres/19)
elseif mm.hres=480 then
  xMax=fix(mm.hres/30)
  yMax=fix(mm.vres/30)
endif
dim board(xMax,yMax) as integer  ' max valid size is (25,18)
dim boardstart(xMax,yMax) as integer  ' copy of board for restart
dim boardx, boardy
dim found       ' non-zero if a level was found (and loaded into board()
dim px, py
dim movx(8) = (0,0,-1,0,1,0,0,0,0)
dim movy(8) = (0,-1,0,0,0,0,0,0,1)
dim undo(1000)    ' how many moves should we store?
dim movecount
dim keypress
dim fname$
dim levelnum

if cputype%=cmm2% then
  page write 2 : cls
  load png "CrateAwaySprites.png"

  page write 1 : cls  
  page write 0 : cls
else
  if mm.hres=320 then: pass=1: else: F4_setup(pass): endif
endif

TitleScreen(1)  ' show title screen with delays, return with user selection

' menu loop
do
  select case keypress
    case 66     ' "B"   start built-in levels
      restore builtin
      if cputype%<>cmm2% then F4_setup(pass) ' free memory, get other sprites
      PlayGame(0)  ' play with levels from data statements
    case 76     ' "L"   load levels from file
      if cputype%=cmm2% then
        fname$ = GetFileName("",16)
        if fname$ <> "" then
          open fname$ for input as #3
          PlayGame(1)  ' play with levels from file
          close #3
        endif
      else
        a$="External files not supported on F4--space to continue"
        text mm.hres/2,mm.vres-mm.fontheight,a$,c,2,,rgb(white),rgb(black)
        do: a$=inkey$: loop while a$=""
      endif
    case 81     ' "Q"   quit
      exit do   ' break out of menu loop
  end select
  TitleScreen(0)
loop

on error skip   ' ignore error from try to close file if none is open
close #3        ' try to close file even if not open

end

'===========================================
sub LevelsAllDone

          ' blit 302,392,MM.HRES/2,MM.VRES/2,194,74,2  ' "THAT"S ALL"

end sub

'===========================================
sub PlayGame(source)
  levelnum = 0
  do  ' loop through all the levels in 'source'
    if ReadLevel(source) then  ' read the next level from somewhere
      ShowLevel
      if not PlayLevel() then exit do
    else
      exit do
    endif
  loop   ' break out of game play loop
end sub

'===========================================
function PlayLevel()
  do
    k = asc(ucase$(_inkey$()))
    select case k
      case 128    ' up
        if board(px,py - 1) = 6 or board(px,py - 1) = 1 then
          MovePlayer(1)     ' just move
          AddMove(1)        ' save move into undo buffer
        elseif board(px,py - 1) = 4 or board(px,py - 1) = 8 then
          if board(px,py - 2) = 6 or board(px,py - 2) = 1 then
            MovePlayer(1)   ' push box
            AddMove(-1)     ' save push move into undo buffer
          endif
        endif
      case 129    ' down
        if board(px,py + 1) = 6 or board(px,py + 1) = 1 then
          MovePlayer(8)     ' just move
          AddMove(8)
        elseif board(px,py + 1) = 4 or board(px,py + 1) = 8 then
          if board(px,py + 2) = 6 or board(px,py + 2) = 1 then
            MovePlayer(8) ' push box
            AddMove(-8)
          endif
        endif
      case 130    ' left
        if board(px - 1,py) = 6 or board(px - 1,py) = 1 then
          MovePlayer(2)     ' just move
          AddMove(2)
        elseif board(px - 1,py) = 4 or board(px - 1,py) = 8 then
          if board(px - 2,py) = 6 or board(px - 2,py) = 1 then
            MovePlayer(2) ' push box
            AddMove(-2)
          endif
        endif
      case 131    ' right
        if board(px + 1,py) = 6 or board(px + 1,py) = 1 then
          MovePlayer(4)     ' just move
          AddMove(4)
        elseif board(px + 1,py) = 4 or board(px + 1,py) = 8 then
          if board(px + 2,py) = 6 or board(px + 2,py) = 1 then
            MovePlayer(4) ' push box
            AddMove(-4)
          endif
        endif
      case  73 ' do save image
        save image "ca"+str$(imageNo%)+".bmp": imageNo%=imageNo%+1
      case  78 ' n/N - skip to next level - CHEATER!
        PlayLevel = 1   ' finished this level - well, not really
        exit do
      case  81 ' q/Q - return to main menu
        PlayLevel = 0   ' quit the level/game
        exit do
      case  82 ' r/R - restart
        math add boardstart(),0,board()   ' restore starting board
        math set 0,undo()   ' clear undo history
        ShowLevel           ' display starting position, reset player position
      case  90 ' z/Z - undo
        if undo(0) then
          um = undo(undo(0))
          undo(0) = undo(0) - 1
          if um > 0 then    ' simple move is positive
            MovePlayer(8 / um)  ' opposite direction
          else              ' push move is negative
            MovePlayer(-8 / um)
            PullBox(-um)
          endif
        endif
    end select

    if mm.hres<481 then: fnt%=1: else: fnt%=3: endif
    text MM.HRES,MM.VRES,"Moves: "+str$(undo(0)),"RB",fnt%

    if TestSolved() then
      ShowSuccess
      PlayLevel = 1   ' finished this level
      exit do
    endif
  loop
  do : loop until _inkey$() = ""  ' clear input buffer
end function

'===========================================
' add a move to the undo buffer
' 1:up 2:left 4:right 8:down
' negative indicates push
sub AddMove(dir)
  undo(0) = undo(0) + 1
  undo(undo(0)) = dir   ' store this move
end sub

'===========================================
'pull a box opposite the given direction - for undo purposes
sub PullBox(dir)
  local sx, sy  ' starting x and y - becomes empty
  local ex, ey  ' ending x and y   - will contain box

  ex = px + movx(dir) : ey = py + movy(dir)
  sx = ex + movx(dir) : sy = ey + movy(dir)

  ' draw box in new location
  if board(ex,ey) = 1 then ' goal square?
    DrawTile(ex,ey,8)       ' box on goal
    board(ex,ey) = 8
  else
    DrawTile(ex,ey,4)       ' box on floor
    board(ex,ey) = 4
  endif

  ' draw blank in old location
  if board(sx,sy) = 8 then ' goal square?
    DrawTile(sx,sy,1)       ' goal square
    board(sx,sy) = 1
  else
    DrawTile(sx,sy,6)       ' blank floor
    board(sx,sy) = 6
  endif
end sub

'===========================================
' move player in given direction
' update player position and board array
sub MovePlayer(dir)
  local tx,ty

  ' erase player at old position
  if board(px,py) = 7 then    ' standing on goal?
    DrawTile(px,py,1)  ' goal
    board(px,py) = 1
  else
    DrawTile(px,py,6)  ' floor
    board(px,py) = 6
  endif
  px = px + movx(dir) : py = py + movy(dir)
  tx = px + movx(dir) : ty = py + movy(dir)
  ' draw player at new position
  if board(px,py) = 1 then    ' standing on goal?
    DrawTile(px,py,7)  ' player on goal
    board(px,py) = 7
  elseif board(px,py) = 6 then    ' standing on floor?
    DrawTile(px,py,2)  ' player on floor
    board(px,py) = 2
  ' push a box
  elseif board(px,py) = 4 then    ' standing on box?
    DrawTile(px,py,2)  ' player on floor
    board(px,py) = 2
    if board(tx,ty) = 1 then  ' goal square?
      DrawTile(tx,ty,8)  ' box on goal
      board(tx,ty) = 8
    else
      DrawTile(tx,ty,4)  ' box
      board(tx,ty) = 4
    endif
  else 'if board(px,py) = 8 then    ' standing on box on goal? (Pushed box)
    DrawTile(px,py,7)  ' player on goal
    board(px,py) = 7
    if board(tx,ty) = 1 then  ' goal square?
      DrawTile(tx,ty,8)  ' box on goal
      board(tx,ty) = 8
    else
      DrawTile(tx,ty,4)  ' box
      board(tx,ty) = 4
    endif
  endif
end sub

'===========================================
' Wall                    #   3
' Player                  @   2
' Player on goal square   +   7
' Box                     $   4
' Box on goal square      *   8
' Goal square             .   1
' Floor             (Space)   6
'===========================================
' Read next level from somewhere
' If source = 0, read built-in level, o/w read from file
' Returns 1 if level found, 0 if none found
function ReadLevel(source)
  local integer x,y,flag,i,gotlevel, ny

  boardx = 0
  y = 0
  ny = 0
  gotlevel = 0
  math set 0, board()
  levelnum = levelnum + 1
  
  do  ' read each line of this level
    x = 0
    
    ' where we gonna read from?
    if source then  ' read from file fname$
      if not eof(#3) then
        line input #3, a$
      else
        exit do  ' we've run out of file to read
      endif
    else    ' read board from data statements
      on error skip
      read a$   ' read next data statement
      if MM.ERRNO then  ' error if no more data
        exit do
      end if
    endif

    ' evaluate line just read
    if left$(a$,1) = ";" then   ' end of level defn
      ' ignore leading comment lines - they belong to previous level
      if gotlevel then
'        if y < 19 and boardx < 26 then
        if ny <= ymax and boardx <= xmax then
          levelname$ = mid$(a$,2)
        else
          ' too big so re-init and try next one
          boardx = 0
          y = 0
          ny            = 0
          gotlevel = 0
          math set 0, board()
          levelnum = levelnum + 1
          continue do
        endif
        exit do
      endif
    elseif a$ <> "" then ' (another) line of this level
      gotlevel = 1
      x = 0
      flag = 0
      for i = 1 to len(a$)
        select case mid$(a$,i,1)
          case "#"     ' Wall
            board(x,y) = 3
            flag = 1
          case "@"     ' Player
            board(x,y) = 2
          case "+"     ' Player on goal square
            board(x,y) = 7
          case "$"     ' Crate
            board(x,y) = 4
          case "*"     ' Crate on goal
            board(x,y) = 8
          case "."     ' Goal
            board(x,y) = 1
          case " "     ' Floor
            if flag and (y > 0) then board(x,y) = 6
        end select
        x = min(x + 1, xmax)
      next i
      boardx = max(boardx,x-1)  ' find widest line
      y = min(y + 1, ymax)    ' next line, within limit
      ny=ny+1 ' count to see if excess of limits
    endif
  loop
  
  if gotlevel then
    boardy = y ' - 1

    ' remove external floors
    ' any floor that has a l/r/u/d neighbour = 0 should be changed to 0
    do
      flag = 1
      for y = 1 to boardy
        for x = 1 to boardx
          if board(x,y) = 6 then  ' floor tile?
            if board(x+movx(2),y) = 0 then board(x,y) = 0 : flag = 0
            if board(x+movx(4),y) = 0 then board(x,y) = 0 : flag = 0
            if board(x,y+movy(1)) = 0 then board(x,y) = 0 : flag = 0
            if board(x,y+movy(8)) = 0 then board(x,y) = 0 : flag = 0
          end if
        next x
      next y
    loop until flag
  else  ' we've run out of levels from this source
    ShowComplete
  endif

  if cputype%=cmm2% then
    math add board(),0,boardstart()   ' make copy for restart
  else
    for i=0 to xMax: for j=0 to yMax: boardstart(i,j)=board(i,j): next j: next i
  endif
'  memory
  
  ReadLevel = gotlevel
end function

'===========================================
' display message that this level source has been completed
sub ShowComplete
  circle MM.HRES/2,MM.VRES/2,125,10,1,rgb(red),0
  if cputype%=cmm2% then
    circle MM.HRES/2,MM.VRES/2,125,10,1,rgb(red),0
    blit 302,394,303,263,194,74,2
    dy=50:dy2=65
  else
    cls
    circle MM.HRES/2,MM.VRES/2,120,10,1,rgb(red),0
    text MM.HRES/2,MM.VRES/2,"THAT'S ALL!","CM",4 
    dy=160:dy2=200
  endif
  text MM.HRES/2,MM.VRES/2+dy,"No more levels.","CT",4
  text MM.HRES/2,MM.VRES/2+dy2,"from here.","CT",4
  do : loop until _inkey$() = ""   ' clear input buffer
  do : loop until _inkey$() <> ""  ' wait for key press
  do : loop until _inkey$() = ""   ' clear input buffer
end sub

'===========================================
sub ShowLevel
  local i,j

  cls
  undo(0) = 0
  px = 0
  py = 0
  for j = 0 to boardy
    for i = 0 to boardx
      DrawTile(i,j,board(i,j))
      if board(i,j) = 2 or board(i,j) = 7 then
        px = i
        py = j
      end if
    next i
  next j
  if cputype%=cmm2% then: y=17: else: y=15: font 1: endif
  if len(levelname$)*mm.fontwidth+4 > mm.hres then ' split it in two
    i=instr(len(levelname$)/2,levelname$," ")
    if i>0 then
      a$=mid$(levelname$,1,i-1): b$=mid$(levelname$,i+1)
      j=MM.VRES-(mm.fontheight+2)*2
      text MM.HRES/2,j,str$(levelnum) + ": " + a$,"CT"
      j=j+mm.fontheight+2
      text MM.HRES/2,j, b$,"CT"
    endif
  else
    text MM.HRES/2,MM.VRES-(mm.fontheight+2)*2,str$(levelnum) + ": " + levelname$,"CT"
  endif
end sub

'===========================================
sub DrawTile(x,y,n)
  local xloc, yloc

 if cputype%=cmm2% then
  yloc = MM.VRES/2 - boardy * 16 + y * 32 - 16
  xloc = MM.HRES/2 - boardx * 16 + x * 32 - 16
  select case n
    case 3     ' Wall
      blit 64, 0,xloc,yloc,32,32,2
    case 2     ' Player
      blit  0, 0,xloc,yloc,32,32,2
    case 7     ' Player on goal square
      blit 96, 0,xloc,yloc,32,32,2
    case 4     ' Crate
      blit  0,32,xloc,yloc,32,32,2
    case 8     ' Crate on goal
      blit 32,32,xloc,yloc,32,32,2
    case 1     ' Goal
      blit 64,32,xloc,yloc,32,32,2
    case 6     ' Floor
      blit 32, 0,xloc,yloc,32,32,2
  end select
 else
  if mm.hres>320 then
    yloc = MM.VRES/2 - boardy * 15 + y * xy% - 15
    xloc = MM.HRES/2 - boardx * 15 + x * xy% - 15
  else
    yloc = MM.VRES/2 - boardy * 8 + (y-1) * xy% - 8
    xloc = MM.HRES/2 - boardx * 8 + x * xy% - 8
  endif
    if mm.hres=320 or mm.hres=480 then
     select case n
      case 3     ' Wall
        blit write 3,xloc,yloc,xy%,xy%
      case 2     ' Player
        blit write 1,xloc,yloc,xy%,xy%
      case 7     ' Player on goal square
        blit write 2,xloc,yloc,xy%,xy%
      case 4     ' Crate
        blit write 4,xloc,yloc,xy%,xy%
      case 8     ' Crate on goal
        blit write 5,xloc,yloc,xy%,xy%
      case 1     ' Goal
        blit write 7,xloc,yloc,xy%,xy%
      case 6     ' Floor
        blit write 6,xloc,yloc,xy%,xy%
     end select
    else
     select case n
      case 3     ' Wall
        blit write 21,xloc,yloc,xy%,xy%
      case 2     ' Player
        blit write 19,xloc,yloc,xy%,xy%
      case 7     ' Player on goal square
        blit write 20,xloc,yloc,xy%,xy%
      case 4     ' Crate
        blit write 22,xloc,yloc,xy%,xy%
      case 8     ' Crate on goal
        blit write 23,xloc,yloc,xy%,xy%
      case 1     ' Goal
        blit write 25,xloc,yloc,xy%,xy%
      case 6     ' Floor
        blit write 24,xloc,yloc,xy%,xy%
     end select
    endif
 endif

end sub

'===========================================
function TestSolved()
  local i,j

  for j = 0 to boardy
    for i = 0 to boardx
      if board(i,j) = 4 then ' found a box not on a goal
        TestSolved = 0
        exit function
      endif
    next i
  next j
  TestSolved = 1
end function

'===========================================
sub ShowSuccess
  local x,y

 if cputype%=cmm2% then 
  page write 3
  rbox 225,240,350,120,60,0,0   ' black backdrop
  rbox 227,242,346,116,58,&hFFFFFF,&hFFFFFF   ' white border
  rbox 232,247,336,106,54,rgb(notblack),rgb(notblack)   ' very dark background
  blit 138,0,274,254,251,91,2,4   ' copy basic "SUCCESS" text
  RenderCount(x,y)
  blit 0,y,361-x/2,316,x,29,3,4
  
  sprite read 1,225,240,350,120,3   ' turn success message into a sprite
  
  page copy 0,1
  page write 1
  for y = MM.VRES-1 to 239 step -18    ' scroll it up from below
    sprite show 1,225,y,1
    page copy 1,0,B
  next y
 else ' not cmm2
  if mm.hres<481 then 
    if mm.hres=320 then font 1 else font 2
    y=(mm.fontheight+2)*3
    box 0,mm.vres-y,mm.hres,y,,rgb(black),rgb(black)
    y=mm.vres-(mm.fontheight+2)*4
  else
    font 4
    y=mm.vres-(mm.fontheight+2)*5
    box 0,y,mm.hres,mm.vres-y,,0,0   ' black backdrop
    rbox 225,y,350,120,60,0,0   ' black backdrop
    rbox 227,y+2,346,116,58,&hFFFFFF,&hFFFFFF   ' white border
    rbox 232,y+7,336,106,54,0,0   ' very dark background
    y=y+mm.fontheight/2+1
  endif
  text mm.hres/2,y,"SOLVED!",c,,,rgb(white),rgb(black)
  a$ = "IN "+str$(undo(0))+" MOVES"
  text mm.hres/2,y+mm.fontheight+2,a$,c,,,rgb(white),rgb(black)
  a$ = "<SPACE> to Continue"
  text mm.hres/2,y+(mm.fontheight+2)*2,a$,c,,,rgb(white),rgb(black)
 endif

  pause 250
  do : loop until _inkey$() = ""   ' clear input buffer
  do : loop until _inkey$() <> ""  ' wait for key press
  do : loop until _inkey$() = ""   ' clear input buffer
 if cputype%=cmm2% then 
  sprite hide 1
  page write 0
 else
 endif
end sub

'===========================================
sub RenderCount(x,y)
  local v$
  local i

  ' build image on page 2 of value of movecount
  v$ = str$(undo(0))
  if undo(0) > 9999 then v$ = "9999"  ' set a maximum value
  y = 500
  x = 0
  'page write 2
  for i = 1 to len(v$)
    ' proportional width digits so treat each independently
    select case mid$(v$,i,1)
      case "0"
        blit 2,180,x,y,25,29,2
        x = x + 25
      case "1"
        blit 26,180,x,y,11,29,2
        x = x + 11
      case "2"
        blit 37,180,x,y,22,29,2
        x = x + 22
      case "3"
        blit 59,180,x,y,22,29,2
        x = x + 22
      case "4"
        blit 81,180,x,y,22,29,2
        x = x + 22
      case "5"
        blit 104,180,x,y,21,29,2
        x = x + 21
      case "6"
        blit 124,180,x,y,20,29,2
        x = x + 20
      case "7"
        blit 145,180,x,y,21,29,2
        x = x + 21
      case "8"
        blit 165,180,x,y,23,29,2
        x = x + 23
      case "9"
        blit 188,180,x,y,20,29,2
        x = x + 20
    end select
  next i
end sub

'===========================================
' show title screen, animate actor
sub TitleScreen(spd)

  if cputype%=cmm2% then 
    page write 0
    cls
  
    blit 0,96,164,161,472,78,2    ' title text CRATEAWAY
    pause spd * 2000

    ' show info
    blit 0,210,150,275,500,54,2
    ' show keys
    blit 0,275,200,380,400,120,2
    ' walk the worker until user presses B,L or Q
  else
'    if pass>1 then pass=0: F4_setup(pass)
    if pass>1 then
      if mm.hres>320 then
        a$="Not enough Memory to continue--F2 to rerun"
      else
        a$="DONE--F2 to rerun"
      endif
      if mm.hres=480 then fnt%=1 else fnt%=3
      cls
      text mm.hres/2,mm.vres/2,a$,cm,fnt%,,rgb(white),rgb(black)
      end
    endif
    cls
    font 5
    if mm.hres=320 then: font 3: elseif mm.hres=480 then: font 3: endif
    y=10:x=mm.hres/2
    text x,y,"CRATEAWAY",c,,,rgb(white),rgb(black)
    y=y+mm.fontheight+10
    font 5
    if mm.hres=320 then: font 1: elseif mm.hres=480 then: font 2: endif
    y=(mm.vres-(mm.fontheight+10)*6)/2 ' center
    text x,y,"PUSH THE CRATES ONTO THE GREEN SQUARES.",c,,,rgb(white),rgb(black)
    y=y+mm.fontheight+10
    text x,y,"BUT BE CAREFUL!  YOU CAN NOT PULL THEM!",c,,,rgb(white),rgb(black)
    y=y+mm.fontheight+10
    text x,y,"USE THE ARROW KEYS TO MOVE",c,,,rgb(white),rgb(black)
    y=y+mm.fontheight+10
    text x,y,"Z - UNDO    R -RESTART LEVEL",c,,,rgb(white),rgb(black)
    y=y+mm.fontheight+10
    text x,y,"Q -QUIT    L - LOAD LEVELS FILE",c,,,rgb(white),rgb(black)
    y=y+mm.fontheight+10
    text x,y,"PRESS  B  TO START BUILT-IN LEVELS",c,,,rgb(white),rgb(black)
    y=y+mm.fontheight+10
'do: a$=inkey$: loop while a$=""
    
  endif
'?"??": do: a$=inkey$: loop while a$=""
  do
    y = 335
    for i = -72 to 468 step 36
      for j = 0 to 8
        if cputype%=cmm2% then
          blit j*32,435,i+4*j,y,32,34,2  ' pushing man
          blit 464,4,32+i+4*j,y+6,28,28,2  ' box
        else
          blit write j+10,i+4*(j+1),y,32,33
          blit write 28,32+i+4*(j+1),y,32,33
        endif
        DelayKey(100,keypress)
        if keypress then exit sub
        'pause 100
      next j
    next i
    for i = 468 to -36 step -36
      for j = 0 to 8
        if cputype%=cmm2% then
          blit j*32,400,i+4*(8-j),y,32,34,2,1   ' walking man - left
        else
          blit write j+1,i+4*(8-j+1),y,32,33
        endif
        DelayKey(75,keypress)
        if keypress then exit sub
        'pause 75
      next j
    next i
    pause 500
    for i = 812 to 560 step -36
      for j = 0 to 8
        if cputype%=cmm2% then
          blit j*32,400,i+4*(8-j),y,32,34,2,1   ' walking man - left
        else
          blit write j+1,i+4*(8-j+1),y,32,33
        endif
        DelayKey(75,keypress)
        if keypress then exit sub
        'pause 75
      next j
    next i
    pause 20
    for i = 524 to -52 step -36
      for j = 0 to 8
        if cputype%=cmm2% then
          blit j*32,435,i+4*(8-j),y,32,34,2,1   ' pushing man - left
          blit 464,4,-32+i+4*(8-j),y+6,28,28,2      ' box
        else ' not enough memory in F4 for man pushing left
'          box 0,y,mm.hres,33,,rgb(black),rgb(black)
            blit write j+19,i+4*(8-j+1),y,32,33
            blit write 28,-32+i+4*(8-j+1),y,32,33
        endif
        DelayKey(100,keypress)
        if keypress then exit sub
        'pause 100
        next j
      next i
  loop
end sub

'===========================================
' delay for the specified time, or until a valid keypress is detected
sub DelayKey(tim, key)
  local t, kp
  key = 0
  t = timer
  do
    kp = asc(ucase$(_inkey$()))
    if kp = 66 or kp = 76 or kp = 81 then  ' "B" | "L" | "Q"
      if kp=73 then ' save image (OOPS, not enough memory
'        save image "ca"+str$(imageNo%)+".bmp": imageNo%=imageNo%+1
      else
        key = kp
        exit sub
      endif
    endif
  loop until timer > t + tim
end sub

Function _inkey$()
  local ky$
  ky$=inkey$
  if asc(ky$)=27 then ' escape sequence
    ky$=inkey$: ky$=inkey$: ky$=ucase$(ky$)
    If ky$="A" Then: ky$=chr$(128) ' up arrow
    ElseIf ky$="B" Then: ky$=chr$(129) ' down arrow
    ElseIf ky$="C" Then: ky$=chr$(131) ' right arrow
    ElseIf ky$="D" Then: ky$=chr$(130) ' left arrow
    ElseIf ky$="5" Then: ky$=chr$(136) ' pgdn
    ElseIf ky$="6" Then: ky$=chr$(137) ' pgup
    ' Function keys are ???
    EndIf
  endif
  _inkey$=ky$
end function

sub F4_setup(int pass)
  local integer k, pxl
'  print "memory before pass ";pass: memory
 if pass=0 then ' walking/pushing man
  load image "CrateAwaySprites.bmp"
'  box 500,0,300,480,,rgb(black),rgb(black)
  y=383: y2=0: x=1:szx=32:szy=33
  for i=1 to 33 ' mirror walking man to walk left
    for j=1 to 145 ' row of pixels
      pxl=pixel(j,i+y)
      pixel j,i+y,pixel(288-j,i+y)
      pixel 288-j,i+y,pxl
    next j
  next i
'  do: a$=inkey$: loop until a$<>""
  for i=1 to 28: on error skip: blit close i:next
  for i=1 to 9
    blit read i,x,y,szx,szy:x=x+szx
'  blit write i,500,y2,szx,szy
'  text 550,y2,"500-"+str$(y2)+","+str$(szx)+","+str$(szy)
'  y2=y2+szy
  next
  y=416:y2=0:x=1
  for i=10 to 18
    blit read i,x,y,szx,szy:x=x+szx
'    blit write i,720,y2,szx,szy
'    y2=y2+szy
  next
  y=416: y2=0: x=1:szx=32:szy=33
  for i=1 to 33 ' mirror pushing man to push left
    for j=1 to 145 ' row of pixels
      pxl=pixel(j,i+y)
      pixel j,i+y,pixel(288-j,i+y)
      pixel 288-j,i+y,pxl
    next j
  next i
  for i=19 to 27
    blit read i,x,y,szx,szy:x=x+szx
'    blit write i,720,y2,szx,szy
'    y2=y2+szy
  next
  blit read 28,1,30,30,30 ' box 1
'  memory
'  do: a$=inkey$: loop until a$<>""
 else ' game
  if mm.hres=320 then ' 320x240 screen on F4, PicoMite
    xy%=19 ' tile size 19x19
    load image "caMan1.bmp"
    blit read 1,1,0,xy%,xy% ' man
    load image "caMan2.bmp"
    blit read 2,1,0,xy%,xy% ' man2
    load image "caBricks.bmp"
    blit read 3,1,0,xy%,xy% ' brick wall
    load image "caBox1.bmp"
    blit read 4,1,0,xy%,xy% ' box 1
    load image "caBox2.bmp"
    blit read 5,1,0,xy%,xy% ' box on goal
    load image "caFloor.bmp"
    blit read 6,1,0,xy%,xy% ' floor
    load image "caTarget.bmp"
    blit read 7,1,0,xy%,xy% ' goal
  else
    xy%=30 ' tile size 30x30
    for i=1 to 28: on error skip: blit close i:next ' free F4 memory
    load image "CrateAwaySprites.bmp"
'    print " pass ";pass;" memory after BLIT CLOSE": memory
    blit read 19,1,0,30,30 ' man
    blit read 20,97,0,30,30 ' man2
    blit read 21,63,0,30,30 ' brick wall
    blit read 22,1,30,30,30 ' box 1
    blit read 23,31,30,30,30 ' box on goal
    blit read 24,31,0,30,30 ' floor
    blit read 25,63,30,30,30 ' goal
'blit read 24,34,30,30,30 ' box 2
'blit read 25,34,62,30,30 ' boxx 2
  endif
 endif
'  print "memory after pass ";pass: memory
  pass=pass+1
end sub
'===========================================
' Wall                    #   3
' Player                  @   2
' Player on goal square   +   7
' Box                     $   4
' Box on goal square      *   8
' Goal square             .   1
' Floor             (Space)   6
builtin:

data "#########"
data "# @ $ . #"
data "#########"
data ";Push the crate onto the green square."

data "########"
data "#.@$   #"
data "####   #"
data "   #####"
data ";Push the crate out of the way, but not too far!"

data "######"
data "#.   #"
data "# #  #"
data "# $@ #"
data "##  ##"
data " ####"
data ";Need space to go round corners."

data "#######"
data "#  @  #"
data "# #.# #"
data "#  *  #"
data "# #$# #"
data "#     #"
data "#######"
data ";Move crates to make room for others."

data "###  ####"
data "#.# ##  #"
data "# ### $ ##"
data "# ##  .  #"
data "#  $ ##$ #"
data "#        #"
data "#+########"
data "###"
data ";Into the corners"

data "         ####"
data "##########  ###"
data "#+$           #"
data "########### # #"
data "#.         $  #"
data "###############"
data ";Get that crate out of the way."

data "####    "
data "#  #    "
data "#  #####"
data "#.$#   #"
data "#    @ #"
data "#.$#   #"
data "#  #####"
data "#  #    "
data "####    "
data ";Out and back in."

game8:
data "  #######"
data "###     #"
data "#  $  $ #"
data "# $  ####"
data "### @#"
data "  # .##"
data "  #  .#"
data "  #  .#"
data "  #####"
data ";Which one first?"

data "############"
data "#    @     #"
data "# $ $ $ $ $#"
data "#  $ $ $ $ #"
data "########## ##"
data "#...#  ###  #"
data "#...        #"
data "#...   ######"
data "########"
data ";Pick the order with care."

data "############"
data "#          #"
data "# $ $ $ $ $#"
data "#  $@$ $ $ #"
data "########## ##"
data "#...#. ###  #"
data "#...$       #"
data "#...   ######"
data "########"
data ";Pick the starting moves with care."

data " ####         #### "
data "##  ###########  ##"
data "#                 #"
data "#  #############  #"
data "## ##  #####  ## ##"
data " # #           # # "
data " # #  #######  # # "
data " # ## #  $ .# ## # "
data " # ## #  ####  # # "
data " # ## #        # # "
data " # ## ##  ##  ## ##"
data " #$#  ##########  #"
data " #@#              #"
data " #.##  ########  ##"
data " #######      #### "
data ";So near and yet so far."

data "   ####"
data "   #  #"
data "   #  #"
data "   #  #"
data "#### ##"
data "# $ $@#"
data "#..   #"
data "#######"
data "; Out and back in again."


