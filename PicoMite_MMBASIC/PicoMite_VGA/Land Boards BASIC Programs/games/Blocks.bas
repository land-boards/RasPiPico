' Blocks V1.0
' Geoff Graham March 2022

' Written for the PicoMiteVGA although it will also
' run on the Colour Maximite 2 and MMBasic For Windows Beta
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

Option Explicit
Option Default Integer

' determine the platform and do any special setup needed
Dim RevertMode
If MM.Device$ = "PicoMiteVGA" Then
  If MM.Ver < 5.0704 Then Print "Requires PicoMiteVGA V5.07.04 or greater" : End
  ' make sure that when the program exits it returns to the original colour mode
  If MM.HRes > 320 Then RevertMode = 1 Else RevertMode = 2
  MODE 2      ' full colour mode with 320x240 pixels
  Randomize Timer
  Const TitleF = 2, TitleFS = 1        ' font and font scaling used for the title
  Const GeneralF = 1, GeneralFS = 1    ' font and font scaling used for everything else
ElseIf Left$(MM.Device$, 17) = "Colour Maximite 2" Then
  Const TitleF = 5, TitleFS = 1        ' font and font scaling used for the title
  Const GeneralF = 4, GeneralFS = 1    ' font and font scaling used for everything else
  MODE 1,8
ElseIf MM.Device$ = "MMBasic for Windows" Then
  Randomize Timer
  Const TitleF = 5, TitleFS = 1        ' font and font scaling used for the title
  Const GeneralF = 5, GeneralFS = 1    ' font and font scaling used for everything else
Else
  Print "This program not supported on this device" : End
EndIf


'''''''''''''''''''''''''''''''''''''''''''''''''''''
' These constants define the look of the game
' ===========================================
'
' width and height of the board in block size
' note these must be one less than the actual size
' so this is a board made up of 10 x 21 blocks
Const Bdw = 9, Bdh = 20
'
' background colour
Const BackColour = &H4000
'
'''''''''''''''''''''''''''''''''''''''''''''''''''''


' the falling pieces are made up of these basic blocks
' define the width and height in pixels
Const Bh = Int(MM.VRes/(Bdh + 1)), Bw = Bh - 1


' the top left origin of the board in pixels
Const Bdx = MM.HRes/2 - (Bw * Bdw)/2), Bdy = Bh/4

' centre of the left and right side material in pixels
Const LeftX = Bdx / 2)
Const RightX = ((((MM.HRes/6) * 5) \ Bw) * Bw) + Bw


' define globals
Dim Board(Bdw, Bdh)
Dim Pieces(6, 3, 3, 3)
Dim PieceColour(6)
Dim p, r, x, y
Dim key$
Dim NewPiece, NextPiece, NextRotation
Dim Score, Difficulty = 3

' vertical position of the window showing the next piece
Const NextY = (Bh * 3)/Bh - 3.5

LoadPieces
Font GeneralF, GeneralFS
Colour RGB(white), BackColour
DrawScreen
ClearBoard

'''''''''''''''''''''''''''''''''''''''''''''''''''''''
' main program loop

Do
  ClearNext
  GetDifficulty
  NextPiece = Int(Rnd * 6)
  NextRotation = Int(Rnd * 4)
  NewPiece = 1
  Score = 0

  ' the game runs within this loop
  ' get a character from the keyboard and act on it
  ' also implement the automatic descent of the piece.
  Do
    ' if a flag has been set, introduce
    ' a new piece at the top of the board
    If NewPiece Then
      NewPiece = 0
   ' identify the new piece, its rotation and location
      p = NextPiece
      r = NextRotation
      x = Bdw/2 - 2 + Int(Rnd * 2)
      y = 0
      If Not CheckValidMove(p, r, x, y) Then
        Text LeftX, MM.VRes/4, "GAME FINISHED", c, , , RGB(red), BackColour
        Exit
      EndIf
   ' draw the new piece
      DrawPiece p, r, x, y, 0
   ' update the window showing the next piece to be launched
      ErasePiece NextPiece, NextRotation, ((RightX - Bdx) / Bw) -2 , NextY/Bh + 4, BackColour
      NextPiece = Int(Rnd * 6)
      NextRotation = Int(Rnd * 4)
      DrawPiece NextPiece, NextRotation, ((RightX - Bdx) / Bw) -2 , NextY/Bh + 4, BackColour
    EndIf

    key$ = Inkey$

    ' process any keystrokes
    Select Case Asc(key$)
      Case &H82       ' left arrow
        If CheckValidMove(p, r, x - 1, y) Then
          ErasePiece p, r, x, y
          x = x - 1
          DrawPiece p, r, x, y
        EndIf
      Case &H83       ' right arrow
        If CheckValidMove(p, r, x + 1, y) Then
          ErasePiece p, r, x, y
          x = x + 1
          DrawPiece p, r, x, y
        EndIf
      Case &H80       ' up arrow
        If CheckValidMove(p, (r + 1) Mod 4, x, y) Then
          ErasePiece p, r, x, y
          r = (r + 1) Mod 4
          DrawPiece p, r, x, y
        EndIf
      Case &H81       ' down arrow
        Do While CheckValidMove(p, r, x, y + 1)
          ErasePiece p, r, x, y
          y = y + 1
          DrawPiece p, r, x, y
          Pause 5
          Score = Score + 1
        Loop
     ' we have reached the bottom or hit another piece
        SavePieceToBoard p, r, x, y
        RemoveBridges
        NewPiece = 1
        Timer = 0
     Case &H50, &H70   ' the letter P for pause
        Text LeftX, MM.VRes/4, "GAME PAUSED", c, , , RGB(red), BackColour
     Do
       key$ = Inkey$
          If Asc(key$) = &H1B Then
            If RevertMode Then MODE RevertMode
            Colour RGB(white), 0
            CLS
            End
          EndIf
       Loop Until key$ <> ""
        Text LeftX, MM.VRes/4, "GAME PAUSED", c, , , BackColour, BackColour
       Timer = 0
      Case &H1B        ' Esc
        Text LeftX, MM.VRes/4, "GAME ABORTED", c, , , RGB(red), BackColour
        Exit
    End Select

    ' cause the piece to automatically drop down one row
    If Timer > 550 - Difficulty * 50 Then
      If CheckValidMove(p, r, x, y + 1) Then
        ErasePiece p, r, x, y
        y = y + 1
        DrawPiece p, r, x, y
        Timer = 0
      Else
     ' we have reached the bottom or hit anouther piece
        SavePieceToBoard p, r, x, y
        RemoveBridges
        NewPiece = 1
      EndIf
    EndIf

    ' update the score
    Text LeftX, (MM.VRes/4)*3, "Score", c,,,,BackColour
    Text LeftX, (MM.VRes/4)*3 + MM.Info(FONTWIDTH)*2, Str$(Score * Difficulty), c,,2,,BackColour
  Loop
Loop


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' General subroutines and functions
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

' search through the array Board(x,y) which represents the on screen display
' detect any completely filled horizontal lines and remove them
' then redraw the on screen image
Sub RemoveBridges
  Const flash = 20
  Local x, y, oy, count
  For y = Bdh To 0 Step -1
  Do While y > 0
    count = 0
    For x = 0 To Bdw
      If Board(x, y) Then count = count + 1
    Next x
    If count > Bdw Then
      ' found a full row - flash the row in various colours
      For x = 0 To Bdw : DrawBlock(RGB(red), 0 , x, y) : Next x
      Pause flash
      For x = 0 To Bdw : DrawBlock(RGB(rust), 0, x, y) : Next x
      Pause flash
      For x = 0 To Bdw : DrawBlock(RGB(brown), 0, x, y) : Next x
      Pause flash
      For x = 0 To Bdw : DrawBlock(RGB(black), 0, x, y) : Next x
      Pause flash * 3
      ' copy down the board to delete the row
      For oy = y To 1 Step -1
        For x = 0 To Bdw
          Board(x, oy) = Board(x, oy - 1)
        Next x
      Next oy
      ' erase the top row
      For x = 0 To Bdw : Board(x, 0) = 0 : Next x
      ' redraw the entire board
      For y = 0 To Bdh
        For x = 0 To Bdw
          DrawBlock Board(x, y), 0, x, y
        Next x
      Next y
      Score = Score + 40
    EndIf
    y = y - 1
  Loop
End Sub


' draw a piece on the screen
' p = index of the piece, r = rotation, ox & oy top left origin
' bc = background colour
Sub DrawPiece p, r, ox, oy, bc
  Local x, y
  For y = 0 To 3
    For x = 0 To 3
      If Pieces(p, r, x, y) Then
       DrawBlock(PieceColour(p), bc, ox + x, oy + y)
      EndIf
    Next x
  Next y
End Sub


' draw a block (square portion of a piece) on the screen
' c = colour, bc = background colour, ox & oy top left origin
Sub DrawBlock c, bc, ox, oy
  Local x = (ox * Bw) + Bdx
  Local y = (oy * Bh) + Bdy
  If c Then
    Box x - 1, y - 1, Bw + 1, Bh + 1, 1, bc, c
    If MM.HRes < 500 Then
        Line x + 1, y + 1, x + Bw - 3, y + 1, 1, bc          ' top shadow
        Line x + Bw - 3, y + 1, x + Bw - 3, y + Bh - 3, 1, bc ' right shadow
    ElseIf MM.HRes < 800 Then
        Line x + 2, y + 3, x + Bw - 5, y + 3, 1, bc          ' top shadow
        Line x + Bw - 5, y + 3, x + Bw - 5, y + Bh - 5, 1, bc ' right shadow
    Else
        Line x + 3, y + 4, x + Bw - 6, y + 4, 2, bc          ' top shadow
        Line x + Bw - 6, y + 4, x + Bw - 6, y + Bh - 6, 2, bc ' right shadow
    EndIf
  Else
    Box x, y, Bw, Bh, 1, c, c  ' fast erase for blank block
  EndIf
End Sub


' erase a piece from the screen
' p = index of the piece, r = rotation, ox & oy top left origin
' bc = background colour
Sub ErasePiece p, r, ox, oy, bc
  Local x, y
  For y = 0 To 3
    For x = 0 To 3
      If Pieces(p, r, x, y) Then
        DrawBlock(bc, bc, ox + x, oy + y)
      EndIf
    Next x
  Next y
End Sub


' check if this piece is valid, returns true if yes
' valid means within borders and not overlapping another piece
' p = index of the piece, r = rotation, ox & oy top left origin
Function CheckValidMove(p, r, ox, oy)
  Local x, y
  For y = 0 To 3
    For x = 0 To 3
      If Pieces(p, r, x, y) Then
        If ox + x < 0 Then Exit Function
        If ox + x > Bdw Then Exit Function
        If oy + y > Bdh Then Exit Function
        If Board(ox + x, oy + y) Then Exit Function
      EndIf
    Next x
  Next y
  CheckValidMove = 1
End Function


' copy the colour of a piece to Board(x,y) representing the screen
' this is only done when a falling piece has stopped falling
' p = index of the piece, r = rotation, ox & oy top left origin
Sub SavePieceToBoard p, r, ox, oy
  Local x, y
  For y = 0 To 3
    For x = 0 To 3
      If Pieces(p, r, x, y) Then
        Board(ox + x, oy + y) = PieceColour(p)
      EndIf
    Next x
  Next y
End Sub


' initialise the screen and draw the static elements
Sub DrawScreen
  Local x, y

  CLS
  Text LeftX, 5, "Blocks", c, TitleF, TitleFS, RGB(white), BackColour

  Colour RGB(cyan)
  y = MM.VRes - MM.Info(FONTHEIGHT) * 9.6
  Text RightX, y, "L/R ARROWS", c
  y = y + MM.Info(FONTHEIGHT)
  Text RightX, y, "MOVE PIECE", c
  y = y + MM.Info(FONTHEIGHT) * 1.5
  Text RightX, y, "UP ARROW", c
  y = y + MM.Info(FONTHEIGHT)
  Text RightX, y, "ROTATE", c
  y = y + MM.Info(FONTHEIGHT) * 1.5
  Text RightX, y, "DOWN ARROW", c
  y = y + MM.Info(FONTHEIGHT)
  Text RightX, y, "DROP", c
  y = y + MM.Info(FONTHEIGHT) * 1.5
  Text RightX, y, "P = PAUSE", c
  y = y + MM.Info(FONTHEIGHT)
  Text RightX, y, "ESC = QUIT", c
  Colour RGB(white)
End Sub


' clear the on screen board and the array Board(x, y)
Sub ClearBoard
  Local x, y
  Box Bdx - 2, Bdy - 2, ((Bdw + 1) * Bw) + 3, ((Bdh + 1) * Bh) + 3, 1, RGB(white), 0
  For x = 0 To Bdw
    For y = 0 To Bdh
      Board(x, y) = 0
  Next y, x
End Sub


' clear the box that shows the next piece to be introduced
Sub ClearNext
  Box RightX - Bw*3, MM.Info(FONTHEIGHT) * 3, Bw*6, (MM.Info(FONTHEIGHT) * 3) + (4 * Bh), 0, , BackColour
  Text RightX, MM.Info(FONTHEIGHT), "NEXT", c
End Sub


' this is responsible for getting the difficulty level
' from the user.  It then sets up the left hand
' side of the screen for the game.
Sub GetDifficulty
  Local x, y, key$
  y = MM.VRes/3
  Text LeftX, y, "Difficulty: ", c
  y = y + MM.Info(FONTHEIGHT)
  Text LeftX, y, " From 1 to 9 ", c
  y = y + MM.Info(FONTHEIGHT)
  Text LeftX, y, "Default is "+Str$(Difficulty), c
  y = y + MM.Info(FONTHEIGHT)
  Text LeftX, y, "ESC to Exit", c
  x = LeftX + MM.Info(FONTWIDTH) * 5.5
  y = MM.VRes/3
  Text x, y, " ", , , , , 0
  y = y + MM.Info(FONTHEIGHT)
  Do
    If Timer > 300 Then
      Timer = 0
      Line x, y, x + MM.Info(FONTWIDTH) - 1, y, 1, RGB(white)
    EndIf
    If Timer > 200 Then Line x, y, x + MM.Info(FONTWIDTH) - 1, y, 1, 0
    key$ = Inkey$
    If Asc(key$) = &H0D Then key$ = Str$(Difficulty)
    If key$ >= "1" And key$ <= "9" Then
      Difficulty = Asc(key$) - &H30
      Box 0, MM.Info(FONTHEIGHT) * 2, Bdx - 2, MM.HRes - MM.Info(FONTHEIGHT) * 2, 1, BackColour, BackColour
      Text LeftX, MM.VRes/3 + MM.Info(FONTHEIGHT) * 2, "Difficulty", c
      Text LeftX, MM.VRes/3 + MM.Info(FONTHEIGHT) * 3, Str$(Difficulty), c
      ClearBoard
      Exit Sub
    EndIf
    If Asc(key$) = &H1B Then
      If RevertMode Then MODE RevertMode
      Colour RGB(white), 0
      CLS
      End
    EndIf
  Loop
End Sub


' read the data statements and load the pieces into the
' array Pieces((p, r, x, y) where p = index of the piece,
' r = rotation, x & y are the blocks that make up the piece
Sub LoadPieces
  Local p, r, x, y
  For p = 0 To 6
    Read PieceColour(p)
    For r = 0 To 3
      For y = 0 To 3
        For x = 0 To 3
          Read Pieces(p, r, x, y)  ' 1 = block
        Next x
      Next y
    Next r
  Next p
End Sub


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' these define each piece which is represented as a 4x4 array
' with 1 = a block to be drawn & 0 = nothing (empty).
' the four lines represent the block rotated 0, 90, 180 & 270 degrees

' define piece 1
Data RGB(white)
Data 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0
Data 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0
Data 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0
Data 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0

' define Piece 2
Data RGB(magenta)
Data 0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0
Data 0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0
Data 0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0
Data 0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0

' define piece 3
Data &H80FF   'rgb(cerulean)
Data 0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0
Data 0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0
Data 0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0
Data 1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0

' define piece 4
Data RGB(cyan)
Data 0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0
Data 0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0
Data 1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0
Data 0,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0

' define piece 5
Data RGB(yellow)
Data 1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0
Data 1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0
Data 1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0
Data 1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0

' define piece 6
Data RGB(red)
Data 0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0
Data 1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0
Data 0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0
Data 1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0

' define piece 7
Data RGB(green)
Data 1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0
Data 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0
Data 1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0
Data 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0
