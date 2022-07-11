' Sokoban VT100 PicoMite
' from FreeBASIC QB mode/Qbasic/QB64
' web: http://rudih.info
' converted to MMBasic (VT100) by M. Herhaus
Dim sc$(22)
ML=5:'Number of Levels
' initial statements
Const el$=Chr$(27)+"["
menu:
cur_off
 TCLS
AT 10, 30
Print "---SOKOBAN---"
at 12, 5
Print "Arrow keys move - SPACE quits - <R> retry."
at  13, 5
Print "To play, cover the '*' with 'O's."
AT 14, 5
Print "<L> load text file as level."
For g = 1 To 22
sc$(g) = ""
Next g
 x1$ = ""
Do While  x1$ = ""
   x1$ = Inkey$
Loop
 If  x1$ = " " Then cur_on: End
 If  x1$ = "l" Or  x1$ = "L" Then
 tCls
 Input "file"; filen$
 Open filen$ For INPUT As #1
 l = 1
 While Not Eof(1)
   Line Input #1, sc$(l)
   l = l + 1
 Wend
 Close #1
 level = 0
 GoTo lev2
 EndIf
 level = 1

 ' load level
 lev:
 TCLS
 For g = 1 To 22
 sc$(g) = ""
 Next g
 lev2:
If level = 1 Then
sc$(3) = "         88888"
sc$(4) = "         8   8"
sc$(5) = "         8O  8"
sc$(6) = "       888  O88"
sc$(7) = "       8  O O 8"
sc$(8) = "     888 8 88 8   888888"
sc$(9) = "     8   8 88 88888  **8"
sc$(10) = "     8 O  O          **8"
sc$(11) = "     88888 888 8X88  **8"
sc$(12) = "         8     888888888"
sc$(13) = "         8888888"
EndIf
If level = 2 Then
sc$(5) = "     8888888888888"
sc$(6) = "     8**  8      888"
sc$(7) = "     8**  8 O  O   8"
sc$(8) = "     8**  8O8888   8"
sc$(9) = "     8**    X 88   8"
sc$(10) = "     8**  8 8  O  88"
sc$(11) = "     888888 88O O  8"
sc$(12) = "       8 O  O O O  8"
sc$(13) = "       8    8      8"
sc$(14) = "       8888888888888"
EndIf
If level = 3 Then
sc$(3) = "         88888888"
sc$(4) = "         8     X8"
sc$(5) = "         8 O8O 88"
sc$(6) = "         8 O  O8"
sc$(7) = "  888888 88O O 8"
sc$(8) = "  8**  8 8 O 8 888"
sc$(9) = "  8**  888 O  O  8"
sc$(10) = "  8**        O   8"
sc$(11) = "  8**  88888888888"
sc$(12) = "  8**  8"
sc$(13) = "  888888"
EndIf
If level = 4 Then
sc$(4) = "    888888"
sc$(5) = "    8**  8  888"
sc$(6) = "    8**  8 88X88"
sc$(7) = "    8**  888   8"
sc$(8) = "    8**     OO 8"
sc$(9) = "    8**  8 8 O 8"
sc$(10) = "    888888 8 O 8"
sc$(11) = "       8 O 8O  8"
sc$(12) = "       8  O8 O 8"
sc$(13) = "       8 O  O  8"
sc$(14) = "       8  88   8"
sc$(15) = "       888888888"
EndIf
If level = 5 Then
sc$(4) = "      8888888888"
sc$(5) = "      8**  8   8"
sc$(6) = "      8**      8"
sc$(7) = "      8**  8  8888"
sc$(8) = "     8888888  8  88"
sc$(9) = "     8            8"
sc$(10) = "     8  8  88  8  8"
sc$(11) = "   8888 88  8888 88"
sc$(12) = "   8  O  88888 8  8"
sc$(13) = "   8 8 O  O  8 O  8"
sc$(14) = "   8 XO  O   8   88"
sc$(15) = "   8888 88 8888888"
sc$(16) = "      8    8"
sc$(17) = "      888888"
EndIf
For g = 1 To 22
    x = Instr(sc$(g), "X")
    If x > 0 Then
        y = g
        GoTo main
    EndIf
Next g
' main loop
main:
found = 0
For g = 1 To 22
f = Instr(sc$(g), "*")
If f > 0 Then found = found + 1
Next g
If found = 0 And aster = 0 Then
TCls
Print "level complete..."
Print "Steps ";Steps
Steps=0
Pause 3000
If level = 0 Then GoTo menu
level = level + 1
If level > ML Then GoTo menu
GoTo lev
EndIf
HOME
 cur_off
For g = 1 To 22
displ sc$(g):Print
Next g
Print "level:"; level,"Steps ";Steps
user$ = ""
Do While user$ = ""
  user$ = Inkey$
Loop

If Mid$(sc$(y), x + 1, 3) = "OO8" Or Mid$(sc$(y), x - 3, 3) = "8OO" Then aster = 0
If Mid$(sc$(y), x + 1, 3) = "O*8" Or Mid$(sc$(y), x - 3, 3) = "8*O" Then aster = 0
If user$ = " " GoTo menu
If user$ = "r" Or user$ = "R"Then  steps=0:GoTo lev
' up
If user$ <> Chr$(128) Then GoTo CDOWN
    'collision detection
    dest$ = Mid$(sc$(y - 1), x, 1)
    If Mid$(sc$(y + 2), x, 1) = "O" Then aster = 0:EndIf
    If dest$ = "8" GoTo main
    If dest$ = "O" Then
        If Mid$(sc$(y - 2), x, 1) = " " Or Mid$(sc$(y - 2), x, 1) = "*" Then
            le$ = Left$(sc$(y), x - 1)
            If aster = 2 Then
                mi$ = "*"
                aster = 0
            EndIf
            If aster = 1 Then
                mi$ = "*"
                aster = 2
            Else
                mi$ = " "
            EndIf
            If Mid$(sc$(y - 2), x, 1) = "*" Then aster = 1 Else aster = 0
            ri$ = Mid$(sc$(y), x + 1, 22)
            sc$(y) = le$ + mi$ + ri$
            le$ = Left$(sc$(y - 1), x - 1)
            mi$ = "X"
            ri$ = Mid$(sc$(y - 1), x + 1)
            sc$(y - 1) = le$ + mi$ + ri$
            le$ = Left$(sc$(y - 2), x - 1)
            mi$ = "O"
            ri$ = Mid$(sc$(y - 2), x + 1, 22)
            sc$(y - 2) = le$ + mi$ + ri$
        Else
            GoTo main
        EndIf
    EndIf
    If dest$ = " " Or dest$ = "*" Then
        le$ = Left$(sc$(y), x - 1)
        If aster = 1 Then
            mi$ = "*"
            aster = 0
        Else
            mi$ = " "
        EndIf
        If dest$ = "*" Then aster = 1 Else aster = 0
        ri$ = Mid$(sc$(y), x + 1, 22)
        sc$(y) = le$ + mi$ + ri$
        le$ = Left$(sc$(y - 1), x - 1)
        mi$ = "X"
        ri$ = Mid$(sc$(y - 1), x + 1)
        sc$(y - 1) = le$ + mi$ + ri$
    EndIf
    y = y - 1:Steps=steps+1
    GoTo main
' down
CDOWN:
If user$ <> Chr$(129) Then GoTo CLEFT
    'collision detection
    dest$ = Mid$(sc$(y + 1), x, 1)
    If Mid$(sc$(y - 2), x, 1) = "O" Then aster = 0
    If dest$ = "8" GoTo main
    If dest$ = "O" Then
        If Mid$(sc$(y + 2), x, 1) = " " Or Mid$(sc$(y + 2), x, 1) = "*" Then
            le$ = Left$(sc$(y), x - 1)
            If aster = 2 Then
                mi$ = "*"
                aster = 0
            EndIf
            If aster = 1 Then
                mi$ = "*"
                aster = 2
            Else
                mi$ = " "
            EndIf
            If Mid$(sc$(y + 2), x, 1) = "*" Then aster = 1 Else aster = 0
            ri$ = Mid$(sc$(y), x + 1, 22)
            sc$(y) = le$ + mi$ + ri$
            le$ = Left$(sc$(y + 1), x - 1)
            mi$ = "X"
            ri$ = Mid$(sc$(y + 1), x + 1)
            sc$(y + 1) = le$ + mi$ + ri$
            le$ = Left$(sc$(y + 2), x - 1)
            mi$ = "O"
            ri$ = Mid$(sc$(y + 2), x + 1, 22)
            sc$(y + 2) = le$ + mi$ + ri$
        Else
            GoTo main
        EndIf
    EndIf
    If dest$ = " " Or dest$ = "*" Then
        le$ = Left$(sc$(y), x - 1)
        If aster = 1 Then
            mi$ = "*"
            aster = 0
        Else
            mi$ = " "
        EndIf
        If dest$ = "*" Then aster = 1 Else aster = 0
        ri$ = Mid$(sc$(y), x + 1, 22)
        sc$(y) = le$ + mi$ + ri$
        le$ = Left$(sc$(y + 1), x - 1)
        mi$ = "X"
        ri$ = Mid$(sc$(y + 1), x + 1)
        sc$(y + 1) = le$ + mi$ + ri$
    EndIf
    y = y + 1:Steps=steps+1
    GoTo main
CLEFT:
' left
If user$ <> Chr$(130) Then GoTo CRIGHT
    'collision detection
    dest$ = Mid$(sc$(y), x - 1, 1)
    If Mid$(sc$(y), x + 2, 1) = "O" Then aster = 0
    If dest$ = "8" GoTo main
    If dest$ = "O" Then
        If Mid$(sc$(y), x - 2, 1) = " " Or Mid$(sc$(y), x - 2, 1) = "*" Then
            le$ = Left$(sc$(y), x - 3)
            If aster = 2 Then
                mi$ = "OX*"
                aster = 0
            Else
                mi$ = "OX "
            EndIf
            If aster = 1 Then
                mi$ = "OX "
                aster = 2
            Else
                mi$ = "OX "
            EndIf
            If Mid$(sc$(y), x - 2, 1) = "*" Then aster = 1 Else aster = 0
            ri$ = Mid$(sc$(y), x + 1, 22)
            sc$(y) = le$ + mi$ + ri$
        Else
            GoTo main
        EndIf
    EndIf
    If dest$ = " " Or dest$ = "*" Then
        le$ = Left$(sc$(y), x - 2)
        If aster = 1 Then
            mi$ = "X*"
            aster = 0
        Else
            mi$ = "X "
            aster = 0
        EndIf
        If dest$ = "*" Then aster = 1 Else aster = 0
        ri$ = Mid$(sc$(y), x + 1, 22)
        sc$(y) = le$ + mi$ + ri$
    EndIf
    x = x - 1:Steps=steps+1
CRIGHT:
' right
If user$ <> Chr$(131) Then GoTo main
    'collision detection
    dest$ = Mid$(sc$(y), x + 1, 1)
    If Mid$(sc$(y), x - 2, 1) = "O" Then aster = 0
    If dest$ = "8" GoTo main
    If dest$ = "O" Then
        If Mid$(sc$(y), x + 2, 1) = " " Or Mid$(sc$(y), x + 2, 1) = "*" Then
            le$ = Left$(sc$(y), x - 1)
            If aster = 2 Then
                mi$ = "*XO"
                aster = 0
            Else
                mi$ = " XO"
            EndIf
            If aster = 1 Then
                mi$ = " XO"
                aster = 2
            Else
                mi$ = " XO"
            EndIf
            If Mid$(sc$(y), x + 2, 1) = "*" Then aster = 1 Else aster = 0
            ri$ = Mid$(sc$(y), x + 3, 22)
            sc$(y) = le$ + mi$ + ri$
        Else
            GoTo main
        EndIf
    EndIf
    If dest$ = " " Or dest$ = "*" Then
        le$ = Left$(sc$(y), x - 1)
        If aster = 1 Then
            mi$ = "*X"
            aster = 0
        Else
            mi$ = " X"
            aster = 0
        EndIf
        If dest$ = "*" Then aster = 1 Else aster = 0
        ri$ = Mid$(sc$(y), x + 2, 22)
        sc$(y) = le$ + mi$ + ri$
    EndIf
    x = x + 1:Steps=steps+1
GoTo main
'---------------------------
Sub TCLS
' clear Terminal Screen
 Print el$;"2J";
  HOME
End Sub

Sub HOME
  ' Cursor top left
   Print el$;"H";
End Sub

Sub AT Row,Col
   Print el$;;Str$(Row);"H";el$;Str$(Col);"G";
End Sub
Sub cur_off
   Print el$"?25l";
End Sub

Sub cur_on
   Print el$;"?25h";
End Sub
Sub displ ds$
 ll=Len(ds$)
 If ll=0 Then Exit Sub
Print Chr$(27);"#6";
 For l1=1 To ll
   tmp$=Mid$(ds$,l1,1)
   If tmp$="8" Then
     Print el$;"35m#";el$;"37m";
  ElseIf tmp$="*" Then
     Print el$;"33m*";el$;"37m";
  ElseIf tmp$="O" Then
     Print el$;"41m";el$;"30mX";el$;"37m";el$;"40m";
  Else
    Print tmp$;
  EndIf
 Next l1
End Sub                                                                                                                             