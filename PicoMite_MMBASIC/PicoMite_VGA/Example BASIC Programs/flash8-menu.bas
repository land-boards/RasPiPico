'init menu
MODE 2
Font 3
CLS RGB(0,0,130)
Arc 85,57,27,29,120,60,RGB(magenta)
Arc 85,57,31,32,116,65,RGB(magenta)
Colour 0,RGB(blue)
Text 65,50,"Pico","LT",3,1,RGB(yellow)
Text 128,50,"GAME","LT",3,1,RGB(orange)
Text 188,50, " V","LT",3,1,RGB(red)
Text 218,50,"G","LT",3,1,RGB(green)
Text 232,50,"A","LT",3,1,RGB(cyan)
Colour RGB(cyan)
Box 65,48,183,20
Font 7
Text 1,100,""
Print
Flash list
Print
Print "  Flash slot to run or Q to exit ";
Line Input k$
Select Case k$
 Case "1" To "7"
  Flash chain Val(k$)
 Case "q","Q"
  End
 Case Else
  Run
End Select
