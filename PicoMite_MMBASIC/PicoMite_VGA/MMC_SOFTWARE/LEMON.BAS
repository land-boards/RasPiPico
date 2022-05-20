'' Lemonade Stand converted to VT100 for Micromite
'' By Oldbitcollector and Mindrobots
'' Tested on Micromite Companion
'' Sept 15th, 2014
10 Dim W1(7),W2(7),W0$(7),WF(6),DF(7)
20 GoSub 1010
30 Randomize Timer
40 Rem set prices and max sales
50 COLOR 14,0:CLS:D=1000:PL=75 :PS=106:PC=98:MAX=100
60 F=14:B=0:S=0:L=0:C=0:S1=0:L1=0:C1=0
70 W$="Sunny        Partly CloudyCloudy       Rainy        cloudy       Partly Cloudy"
80 GoSub 1200
90 COLOR 3,0:GoSub 820:GoSub 900
100 Rem do weeks
110 For WK=1 To 10
120 TEMP=60+5*Int(10*Rnd)
130 COLOR 2,0:LOCATE 3,70:Print WK
140 WTHR=1+13*Int(6*Rnd):LOCATE 4,45:Print Mid$(W$,WTHR,13);
150 LOCATE 4,60:Print TEMP;Chr$(135);"  "
160 COLOR F,B:LOCATE 13,42:Print"Purchases:";
170 GoSub 740:GoSub 580:GoSub 630:GoSub 680
180 GoSub 740:LOCATE 19,1:Print"Do you want to make any changes (enter number)?"
190 LOCATE 20,10:Print"0 - no changes"
200 LOCATE 21,10:Print"1 - change sugar"
210 LOCATE 22,10:Print"2 - change lemonade"
220 LOCATE 23,10:Print"3 - change cups"
230 LOCATE 24,8:Input;I
240 LOCATE 19,1:Print Space$(75):Print Space$(75):Print Space$(75)
250 Print Space$(75):Print Space$(75):Print Space$(75);
260 If I=0 Then 320
270 On I GoTo 290,300,310
280 BEEP: GoTo 180
290 D=D+PS*S1:GoSub 940:GoSub 580:GoTo 180
300 D=D+PL*L1:GoSub 940:GoSub 630:GoTo 180
310 D=D+PC*C1:GoSub 940:GoSub 680:GoTo 180
320 GoSub 780:GoSub 900:If AMT<>0 Then 340
330 COLOR 4,0:BEEP:LOCATE 24,1:Print"You have no supplies!!!";:GoTo 160
340 GoSub 740:Input"How much will you charge for lemonade (1 - 99) ",PR
350 If PR>0 And PR<100 Then 370
360 COLOR 4,0:BEEP:Print"enter a number between 1 and 99!!!";:COLOR F,B:GoTo 340
370 GoSub 990:COLOR 0,7:LOCATE 14,10:Print"           ";
380 LOCATE 14,11:Print PR;"cents";
390 Rem do each day
400 For DAY=1 To 7:GoSub 1420:COLOR 0,0:GoSub 740:COLOR 9,0:Print W0$(DAY)
410 COLOR 0,0:For J=13 To 4 Step -1:LOCATE J,33:Print B$;:Next J
420 COLOR 4,0:J=13:K=55
430 LOCATE J,33:Print B$;:J=J-1:K=K+5:If K<W1(DAY) Then 430
440 GoSub 1510:GoSub 1690
450 For I=1 To SALES:COLOR 14,0
460 LOCATE 23,20:Print"Sales ==> ";I;
470 D=D+PR:C=C-1:L=L-10:S=S-4:GoSub 900
480 If AMT<>0 Then 510
490 I=SALES:DAY=7:LOCATE 24,1:COLOR 4,0
500 BEEP: Print"You ran out of supplies!!!";
510 Next I:Next DAY
520 Next WK
530 LOCATE 24,40:COLOR 4,0:Print"Do you want to play again (y/n)?";
540 A$=Inkey$:If A$="" Then 540
550 If Left$(A$,1)="y" Or Left$(A$,1)="Y" Then 50
560 If Left$(A$,1)<>"n" And Left$(A$,1)<>"N" Then 540
570 GoSub 1140:End
580 GoSub 740:Print"How much sugar do you want ($ ";
590 Print PS/100;:Input" / lb.) ",S1
600 If D-S1*PS<0 Then GoSub 750:GoTo 580
610 LOCATE 14,45:Print "lbs. of sugar = ";S1;"  "
620 D=D-S1*PS:GoSub 940:Return
630 GoSub 740:Print"How much lemonade ($ ";
640 Print PL/100;:Input" / quart) ",L1
650 If D-L1*PL<0 Then GoSub 750:GoTo 630
660 LOCATE 15,45:Print"quarts of lemonade = ";L1;"  "
670 D=D-L1*PL:GoSub 940:Return
680 GoSub 740:Print"How many packages of 100 cups ($ ";
690 Print PC/100;:Input" / package) ",C1
700 If D-C1*PC<0 Then GoSub 750:GoTo 680
710 LOCATE 16,45:Print"packages of 100 cups = ";C1;"  "
720 D=D-C1*PC:GoSub 940:Return
730 Rem clear question line
740 LOCATE 23,1:Print Space$(75):LOCATE 23,1:Return
750 COLOR 4,0:BEEP:LOCATE 24,1:Print"You don't have that much!!!";
760 COLOR F,B:Return
770 Rem add purchases to supplies
780 S=S+100*S1:L=L+100*L1:C=C+100*C1:S1=0:L1=0:C1=0
790 LOCATE 13,42:Print Space$(35):LOCATE 1,42:Print Space$(35)
800 LOCATE 1,42:Print Space$(35):LOCATE 1,42:Print Space$(35):Return
810 Rem print headings
820 LOCATE 1,42:Print"You have";
830 LOCATE 3,42:Print"Weather prediction for week";
840 LOCATE 6,42:Print"Supplies:"
850 LOCATE 7,45:Print"lbs. of sugar "
860 LOCATE 8,45:Print"quarts of lemonade "
870 LOCATE 9,45:Print"number of cups "
880 Return
890 Rem update values
900 COLOR 4,0
910 LOCATE 7,70:Print S/100
920 LOCATE 8,70:Print L/100
930 LOCATE 9,70:Print C;"  "
940 COLOR 4,0:LOCATE 1,70:Print "$";:Print D/100
950 COLOR 2,0:X1=Int((S+S1*100)/4):X2=Int((L+L1*100)/10):X3=C+(C1*100)
960 AMT=X1:If X2<X1 Then AMT=X2
970 If X3<AMT Then AMT=X3
980 LOCATE 11,42:Print"You can make";AMT;" cups of lemonade   ";
990 LOCATE 24,1:Print Space$(75);
1000 COLOR F,B:Return
1010 Rem switch output to the color monitor - removed uneeded
1070 Rem init. arrays
1080 W0$(1)="Sunday   ":W0$(2)="Monday   ":W0$(3)="Tuesday  "
1090 W0$(4)="Wednesday":W0$(5)="Thursday ":W0$(6)="Friday   "
1100 W0$(7)="Saturday ":WF(1)=1:WF(2)=.8:WF(3)=.6:WF(4)=.3:WF(5)=.6:WF(6)=.8
1110 DF(1)=.9:DF(2)=.5:DF(3)=.7:DF(4)=.6:DF(5)=.7:DF(6)=.8:DF(7)=1
1120 Return
1130 Rem switch to monochrome - removed uneeded
1180 Return
1190 Rem draw lemonade stand
1200 LOCATE 1,1:COLOR 14,1:B$=Chr$(127):C$=B$+B$:D$=C$+C$:E$=D$+D$
1210 For I=1 To 4:Print Space$(30):Next I
1220 Print"    "B$E$D$E$B$"    "
1230 Print"   "C$E$D$E$C$"   "
1240 Print"  "B$C$E$D$E$C$B$"  "
1250 COLOR 6,1
1260 For I=1 To 4:Print"   "B$"                      "B$"   ":Next I
1270 COLOR 14,1
1280 For I=1 To 4:Print"  "B$C$E$D$E$C$B$"  ":Next I
1290 COLOR 1,2:Print Space$(30)
1300 COLOR 5,15:LOCATE 6,6:Print"ROG'S LEMONADE STAND";
1310 COLOR 0,7:LOCATE 13,10:Print" $ PRICE $ "
1320 LOCATE 14,10:Print"           "
1330 COLOR 7,0:T=105:LOCATE 2,32:Print "TEMP."
1340 LOCATE 3,32:Print Chr$(127);:Print B$;Chr$(127)
1350 For I=1 To 10:LOCATE i+3,32:Print Chr$(127)" "Chr$(127);T
1360 T=T-5:Next I:COLOR 7,4
1370 LOCATE 14,32:Print B$;:Print" ";B$
1380 LOCATE 15,32:Print Chr$(127);" ";Chr$(127)
1390 LOCATE 16,32:Print B$;:Print Chr$(127);:Print B$
1400 Return
1410 Rem figure weather for week
1420 For K=1 To 7:W1(K)=TEMP-10+5*Int(5*Rnd)
1430 If W1(K)<65 Then W1(K)=65 Else If W1(K)>105 Then W1(K)=105
1440 Next K:WTHR=1+Int(WTHR/13):For K=1 To 7:W2(K)=WTHR:Z=1+Int(10*Rnd)
1450 On Z GoTo 1460,1470,1470,1490,1490,1490,1490,1470,1470,1460
1460 W2(K)=WTHR+2:GoTo 1480
1470 W2(K)=WTHR+1
1480 If W2(K)>6 Then W2(K)=W2(K)-6
1490 Next K:Return
1500 Rem add weather
1510 F$=" ":WB=0:On W2(DAY) GoTo 1520,1520,1530,1540,1530,1520
1520 WF=1:GoTo 1550
1530 WF=8:WB=1:F$=Chr$(127):GoTo 1550 ''WAS CHR$(177)
1540 WF=8:WB=0:F$="/"
1550 COLOR WF,WB:For J=1 To 4:LOCATE J,1:For K=1 To 30:Print F$;:Next K,J
1560 LOCATE 5,1:Print F$F$F$F$;:LOCATE 5,27:Print F$F$F$F$;
1570 LOCATE 6,1:Print F$F$F$;:LOCATE 6,28:Print F$F$F$;
1580 LOCATE 7,1:Print F$F$;:LOCATE 7,29:Print F$F$;
1590 For J=8 To 11:LOCATE J,1:Print F$F$F$;:LOCATE J,5:For K=1 To 22:Print F$;
1600 Next K:LOCATE J,28:Print F$F$F$;:Next J
1610 For J=12 To 15:LOCATE J,1:Print F$F$;:LOCATE J,29:Print F$F$;:Next J
1620 If W2(DAY)=1 Then Return
1630 F$=Chr$(127)+Chr$(127):WF=15:If W2(DAY)>2 And W2(DAY)<6 Then WF=7
1640 For K=9 To 20:LOCATE 1,K:If Rnd>.7 Then Print F$;
1650 Next K:For K=3 To 27:LOCATE 2,K:If Rnd>.3 Then Print F$;
1660 Next K:For K=7 To 22:LOCATE 3,K:If Rnd>.6 Then Print F$;
1670 Next K:Return
1680 Rem find sales for day
1690 TF=((W1(DAY)-55)*2)/100
1700 PF=(100-PR)/100
1710 SALES=1+Int(MAX*TF*PF*WF(W2(DAY))*DF(DAY))
1720 Return

'' Some GWBASIC replacement commands for VT100

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
 Pause 10
End Sub

Sub BEEP
 SetPin 4,dout
 PWM 1,500,5
 Pause 200
 PWM 1,stop
End Sub                                                                   
