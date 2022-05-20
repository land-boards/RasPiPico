1000 Rem  KALEIDOSCOPE
1010 Rem PATRICK LEABO--TUSCON
1020 Rem 3-14-82
1030 Rem Converted to Micromite Companion 9-29-14 by Jeff Ledger
1040 Dim MT(33)
1045 Randomize Val(Right$(Time$,2))
1046 Dim Ch$(10) : Dim dy(8) : Dim dx(8) : Dim nt(8)
1050 CH$(1)="":CH$(2)="":CH$(3)="":CH$(4)=""
1060 WIDE = 80
1065 color 7,0
1066 cls
1070 Rem START
1075 locate 0,0
1080 A= 31:B= 15:X1= 16:Y1= 8
1090 A= Int(Rnd(1)*20+19):B=Int(Rnd(1)*10+13)
1100 Restore
1110 For N= 0 To 7:Read DX(N):Read DY(N):Next
1120 Data 1,0,1,1,0,1,-1,1,-1,0,-1,-1,0,-1,1,-1
1130 For N= 0 To 7:Read NT(N)
1140 Next
1150 Data 1,3,5,6,8,10,12,13
1160 For N= 0 To 24:MT(N)=N+1:Next
1170 For C= 0 To 12:Next:CLS
1180 For NN= 1 To 50
1189 Rem DRAW LOOP
1190 DI=Int(8*Rnd(1))
1200 If Rnd(1) < .25 Then CH$=CH$(4*Rnd(1)+1):GoSub 1600
1205 If Rnd(1) < .25 Then CC=CC+1:If CC>7 Then CC=0 Else COLOR CC,0
1210 GoSub 1590
1220 For N= 1 To Int(Rnd(1)*8)
1230 X1= X1+ DX(DI):Y1= Y1+ DY(DI)
1240 If X1< 1 Then X1= A:GoTo 1260
1250 If X1> A Then X1= 1
1260 If Y1< 1 Then Y1= B:GoTo 1280
1270 If Y1> B Then Y1= 1
1280 X2= 40- X1:Y2= 24- Y1
1282 ch$=Chr$(127)
1290 aa=Int(Rnd*6)
1291 If aa = 5 Then color=Int(Rnd*7)
1293 color color,0
1295 LOCATE Y1,X1:Print CH$;:If WIDE = 80 Then LOCATE Y1,X1+40:Print CH$
1300 LOCATE Y1,X2:Print CH$;:If WIDE = 80 Then LOCATE Y1,X2+40:Print CH$
1310 LOCATE Y2,X1:Print CH$;:If WIDE = 80 Then LOCATE Y2,X1+40:Print CH$
1320 LOCATE Y2,X2:Print CH$;:If WIDE = 80 Then LOCATE Y2,X2+40:Print CH$
1330 Print Chr$(27)+"[f"
1340 Next :Next
1350 GoTo 1070
1360 Rem  VECTORS
1370 For CC= 0 To 12:GoSub 1720:Next
1380 CC= INT (63* Rnd (1))
1390 If Rnd (1)= .5 Then CC= 1
1400 If Rnd (1)> .5 Then CC= CCAND 15
1410 MA= CCAND 56
1420 PLOT 6,CC,12:CC= CCAND 15
1430 If CC> 12 Then CC= 12
1440 GoSub 1790
1450 A= INT (Rnd (1)* 50+ 60):B= INT (Rnd (1)* 50+ 60):GoSub 1580
1460 For N= 1 To 50
1470 X1= X3:X2= X4:Y1= Y3:Y2= Y4:GoSub 1580
1480 DI= INT ((X3/ A)* 8):GoSub 1790
1490 PLOT 2,253,X1,Y1,242,X3,Y3,255
1500 PLOT 2,253,X1,Y2,242,X3,Y4,255
1510 PLOT 2,253,X2,Y1,242,X4,Y3,255
1520 PLOT 2,253,X2,Y2,242,X4,Y4,255
1530 If Rnd (1)< .25 Then CC= INT (8* Rnd (1)):GoSub 1720:PLOT 6,CCOR MA
1540 If Rnd (1)< .1 Then PLOT 31
1550 Next
1560 For CC= 0 To 12:GoSub 1720:Next
1570 CC= INT (63* Rnd (1)):GoTo 1070
1580 X3= INT (A* Rnd (1)):Y3= INT (B* Rnd (1)):X4= 127- X3:Y4= 127- Y3:Return
1590 Rem
1600 OK= OK+ 1:If OK> 2 Then OK= 0
1610 On OK GoTo 1660,1690
1620 NS= 4:GoSub 1710
1630 NS= 0:GoSub 1710:NS= 2:GoSub 1710
1640 NS= 4:GoSub 1710:NS= 7:GoSub 1710
1650 Return
1660 NS= 1:GoSub 1710:NS= 3:GoSub 1710
1670 NS= 4:GoSub 1710:NS= 6:GoSub 1710
1680 Return
1690 NS= 0:GoSub 1710:NS= 3:GoSub 1710
1700 NS= 5:GoSub 1710:NS= 7:GoSub 1710:Return
1710 Return

Sub CLS
 Print Chr$(27)+"[f";:Print Chr$(27)+"[2J";
End Sub

Sub COLOR foreg,backg
 'Convert GWBASIC COLOR Commands To reasonable VT100 colors.
 If foreg < 8 Then  foreg = foreg +30
 If foreg > 8 And foreg < 15 Then foreg = foreg +22
 If backg < 8 Then backg=backg+40
 If yy > 8 And backg < 15 Then backg=backg+32
 Print Chr$(27)+"[0;"+Str$(foreg)+";"+Str$(backg)+";1m";
End Sub

Sub LOCATE XX,YY
 Print Chr$(27)+"["+Str$(XX)+";"+Str$(YY)+"f";
 Pause 7
End Sub

Sub Display
 For x = 14 To 255
 Print x;" "+Chr$(x)+" ";
 Next x
End                                                                                                  