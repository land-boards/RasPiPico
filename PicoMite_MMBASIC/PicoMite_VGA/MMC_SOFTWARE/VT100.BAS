' Useful VT100 control subroutines

color 32,40
cls
locate 10,10
Print "Hello World!"

Sub CLS
 Print Chr$(27)+"[f"; : Print Chr$(27)+"[2J";
End Sub

Sub LOCATE x,y
 Print Chr$(27)+"["+Str$(x)+";"+Str$(y)+"f";
End Sub

Sub COLOR f,b
 Print Chr$(27)+"[0;"+Str$(f)+";"+Str$(b)+";1m";
End Sub                                                                              