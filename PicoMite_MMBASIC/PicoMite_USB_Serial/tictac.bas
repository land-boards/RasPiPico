' TIC-TAC-TOE 1 PLAYER
Mode 4
f=7
background:
  b=random(6)
  If b=6 Or b=3 Or b=2 Then f=0
Color f,b
Dim pk(9)
Cls
For x=0 To 99:t=Val(Inkey$):Next x
Font 2,1:Print "   Tic-Tac-Toe"
Font 1,1:Print "       X = You    O = Computer"
Print:Print "        Enter your move (1-9)"
Print @(75,80) "7  ";
Print CLR$(Black,White) " ";
Print "8  ";
Print CLR$(Black,White) " ";
Print "9"
Print @(93,90);:GoSub spaces
Print @(93,100);:GoSub spaces
Print @(75,110);:GoSub bars
Print @(75,120) "4  ";
Print CLR$(Black,White) " ";
Print "5  ";CLR$(Black,White) " ";
Print "6  "
Print @(93,130);:GoSub spaces
Print @(93,140);:GoSub spaces
Print @(75,150);:GoSub bars
Print @(75,160) "1  ";
Print CLR$(Black,White) " ";
Print "2  ";
Print CLR$(Black,White) " ";
Print "3  "
Print @(93,170);:GoSub spaces
Print @(93,180);:GoSub spaces
T=1

' X'S TURN

xkeyinput:
     P=Val(Inkey$)
     If P<1 Or P>9 GoTo xkeyinput
     If PK(P)<>0 Then Tone 500,500,200:Pause(200):Tone 500,500,200:GoTo xkeyinput
     PK(P)=88:GoSub display
If T=5 GoTo done

' O'S TURN

 O=158:If T>1 GoTo checkall
 If PK(5)=0 Then P=5:GoTo omove
 P=2*Int(Rnd(2)*2)+1:GoTo omove

checkall:  'First check for winning position
 If PK(1)+PK(2)=O And PK(3)=0 Then P=3:GoTo omove
 If PK(1)+PK(3)=O And PK(2)=0 Then P=2:GoTo omove
 If PK(2)+PK(3)=O And PK(1)=0 Then P=1:GoTo omove
 If PK(4)+PK(5)=O And PK(6)=0 Then P=6:GoTo omove
 If PK(4)+PK(6)=O And PK(5)=0 Then P=5:GoTo omove
 If PK(5)+PK(6)=O And PK(4)=0 Then P=4:GoTo omove
 If PK(7)+PK(8)=O And PK(9)=0 Then P=9:GoTo omove
 If PK(7)+PK(9)=O And PK(8)=0 Then P=8:GoTo omove
 If PK(8)+PK(9)=O And PK(7)=0 Then P=7:GoTo omove
 If PK(1)+PK(4)=O And PK(7)=0 Then P=7:GoTo omove
 If PK(1)+PK(7)=O And PK(4)=0 Then P=4:GoTo omove
 If PK(4)+PK(7)=O And PK(1)=0 Then P=1:GoTo omove
 If PK(2)+PK(5)=O And PK(8)=0 Then P=8:GoTo omove
 If PK(2)+PK(8)=O And PK(5)=0 Then P=5:GoTo omove
 If PK(5)+PK(8)=O And PK(2)=0 Then P=2:GoTo omove
 If PK(3)+PK(6)=O And PK(9)=0 Then P=9:GoTo omove
 If PK(3)+PK(9)=O And PK(6)=0 Then P=6:GoTo omove
 If PK(6)+PK(9)=O And PK(3)=0 Then P=3:GoTo omove
 If PK(1)+PK(5)=O And PK(9)=0 Then P=9:GoTo omove
 If PK(1)+PK(9)=O And PK(5)=0 Then P=5:GoTo omove
 If PK(5)+PK(9)=O And PK(1)=0 Then P=1:GoTo omove
 If PK(3)+PK(5)=O And PK(7)=0 Then P=7:GoTo omove
 If PK(3)+PK(7)=O And PK(5)=0 Then P=5:GoTo omove
 If PK(5)+PK(7)=O And PK(3)=0 Then P=3:GoTo omove
 If O=158 Then O=176:GoTo checkall: 'Now check to block
 If PK(8)+PK(6)+PK(3)=O And PK(9)=0 Then P=9:GoTo omove
 If PK(8)+PK(4)+PK(1)=O And PK(7)=0 Then P=7:GoTo omove
 If PK(2)+PK(6)+PK(9)=O And PK(3)=0 Then P=3:GoTo omove
 If PK(2)+PK(4)+PK(7)=O And PK(1)=0 Then P=1:GoTo omove
 If PK(6)+PK(8)+PK(7)=O And PK(9)=0 Then P=9:GoTo omove
 If PK(6)+PK(1)+PK(2)=O And PK(3)=0 Then P=3:GoTo omove
 If PK(4)+PK(2)+PK(3)=O And PK(1)=0 Then P=1:GoTo omove
 If PK(4)+PK(8)+PK(9)=O And PK(7)=0 Then P=7:GoTo omove
' smart code makes x winning impossible:
  If pk(5)+pk(9)=o And pk(3)=0 Then p=3:GoTo omove
  If pk(5)+pk(9)=o And pk(7)=0 Then p=7:GoTo omove
  If pk(5)+pk(7)=o And pk(1)=0 Then p=1:GoTo omove
  If pk(5)+pk(7)=o And pk(9)=0 Then p=9:GoTo
  If pk(1)+pk(9)=o Or pk(3)+pk(7)=o Then p=even(4):GoTo omove

getrndmove:  'get a random move for O
 P=random(9):If PK(P) GoTo getrndmove
omove:
 PK(P)=79:GoSub display
 If T=5 Then GoTo done
 T=T+1:GoTo xkeyinput

done:  'end of game
  Print " Game";
  For x=0 To 50
Tone Rnd(1)*3000+100:Pause 50:Next x
Run

display:
 If P=1 Then Print @(80,170);
 If P=2 Then Print @(106,170);
 If P=3 Then Print @(132,170);
 If P=4 Then Print @(80,130);
 If P=5 Then Print @(106,130);
 If P=6 Then Print @(132,130);
 If P=7 Then Print @(80,90);
 If P=8 Then Print @(106,90);
 If P=9 Then Print @(132,90);
 Print Chr$(PK(P))
 X=PK(1)+PK(2)+PK(3):GoSub win
 X=PK(4)+PK(5)+PK(6):GoSub win
 X=PK(7)+PK(8)+PK(9):GoSub win
 X=PK(1)+PK(4)+PK(7):GoSub win
 X=PK(2)+PK(5)+PK(8):GoSub win
 X=PK(3)+PK(6)+PK(9):GoSub win
 X=PK(1)+PK(5)+PK(9):GoSub win
 X=PK(3)+PK(5)+PK(7):GoSub win
 Return

spaces: Print CLR$(Black,White);" ";
        Print "   ";
        Print CLR$(Black,White);" ":Return

bars:
  Print CLR$(Black,White);"           ":Return

win:   'check for win
  If X=264 Then T=5:Print @(150,120) " X Wins";
  If T<5 And X=237 Then T=5:Print @(150,120) " O Wins";
  Return

Function random (z)   'random # from 1 to z
  q=Int(Rnd(1)*(z+2))
  If q>z Then q=z
  If q<1 Then q=1
  random=q
  End Function

Function even (z)    'returns random even # from 2 to z
  another:
  q=2*random(z)
  If PK(q)<>0 Then another
  even=q
  End Function
