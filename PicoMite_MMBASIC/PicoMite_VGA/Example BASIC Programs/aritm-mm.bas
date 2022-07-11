1000 ' Aritm v0.3.4 for MMB4L & PicoMite MMBasic Version 5.07.03 is FOSS.
1020 ' (C) 1992-2022 by Mikael O. Bonnier, Lund, Sweden.
1030 ' E-mail: <mikael.bonnier@gmail.com>.
1040 ' Absolutely no warranty.
1050 ' License GPLv3+, see
1060 ' <http://www.gnu.org/licenses/gpl.html>.
1070 ' More programs at
1080 ' <http://www.df.lth.se.orbin.se/~mikaelb/basic/>.
1090 ' Documentation:
1100 ' 876543210, 8:Type, 7-5:Op1, 4-3:Op2, 2:Res'd, 1-0:#Todo.
1110 ' Program:
1112 If MM.DEVICE$="PicoMite" Then SetPin GP28,DOUT:' WS2812B RGB LED on GP28
1113 Function FNMODULUS(X,Y) As INTEGER
FNMODULUS=Cint(X Mod Y)
End Function
1114 Function FNDIV(X,Y)
FNDIV=Cint(X\Y)
End Function
1115 Randomize Timer
1117 Dim AF(590)
1120 ' Lbl C
1130 GoSub 3410:' ClS
1140 A=0
1150 If L>0 And M<>0 Then GoTo 3000:' ContMenu
1160 If 0=M Then M=10
1170 GoTo 3030:' Menu
1180 ' Lbl S
1190 GoSub 3410:' ClS
1200 Print "Generating"
1210 Print "Problems..."
1220 U=1
1230 'Dim AF(N)
1240 ' Lbl Add1
1250 If 0=FNMODULUS(FNDIV(M,10),10) Then GoTo 1320:' Add2
1260 For I=0 To 9
1270 For J=0 To 9
1280 AF(U)=100000000+I*100000+J*1000+1
1290 U=U+1
1300 Next J
1310 Next I
1320 ' Lbl Add2
1330 If 0=FNMODULUS(FNDIV(M,100),10) Then GoTo 1410:' Sub1
1340 For I=0 To 9
1350 For J=0 To 9
1360 T=10*(Int(8*Rnd)+1)
1370 AF(U)=200000000+(T+I)*100000+J*1000+1
1380 U=U+1
1390 Next J
1400 Next I
1410 ' Lbl Sub1
1420 If 0=FNMODULUS(FNDIV(M,1000),10) Then GoTo 1490:' Sub2
1430 For I=0 To 9
1440 For J=I To 9+I
1450 AF(U)=300000000+J*100000+I*1000+1
1460 U=U+1
1470 Next J
1480 Next I
1490 ' Lbl Sub2
1500 If 0=FNMODULUS(FNDIV(M,10000),10) Then GoTo 1580:' Mul
1510 For I=0 To 9
1520 For J=I To 9+I
1530 T=10*(Int(9*Rnd)+1)
1540 AF(U)=400000000+(T+J)*100000+I*1000+1
1550 U=U+1
1560 Next J
1570 Next I
1580 ' Lbl Mul
1590 If 0=FNMODULUS(FNDIV(M,100000),10) Then GoTo 1660:' Div
1600 For I=0 To 9
1610 For J=0 To 9
1620 AF(U)=500000000+I*100000+J*1000+1
1630 U=U+1
1640 Next J
1650 Next I
1660 ' Lbl Div
1670 If 0=FNMODULUS(FNDIV(M,1000000),10) Then GoTo 1750:' EndIf
1680 For I=0 To 9
1690 For J=1 To 9
1700 T=I*J+Int(J*Rnd)
1710 AF(U)=600000000+T*100000+J*1000+1
1720 U=U+1
1730 Next J
1740 Next I
1750 ' Lbl EndIf
1760 U=U-1
1770 L=U
1780 ' Lbl U
1790 Print "Shuffling..."
1800 For I=U To 2 Step -1
1810 J=Int(I*Rnd)+1
1820 TF=AF(I)
1830 AF(I)=AF(J)
1840 AF(J)=TF
1850 Next I
1860 K=1
1870 ' Lbl K
1880 GoSub 3410:' ClS
1890 Print Mid$(Str$(L),1);" problems left. -1 Esc"
1900 TF=AF(K)
1910 C=FNDIV(TF,100000000)
1920 If 6=C Then Print "Integer part of ";
1930 I=FNMODULUS(FNDIV(TF,100000),1000)
1940 Print Mid$(Str$(I),1);
1950 GoSub 3280:' Sign
1960 J=FNMODULUS(FNDIV(TF,1000),100)
1970 Print Mid$(Str$(J),1);
1980 Input " = ",A
1990 If -1=A Then GoTo 1120:' C
2000 If 1=C Or 2=C Then R=I+J:GoTo 2040:' EndIf
2010 If 3=C Or 4=C Then R=I-J:GoTo 2040:' EndIf
2020 If 5=C Then R=I*J:GoTo 2040:' EndIf
2030 If 6=C Then R=FNDIV(I,J)
2040 ' Lbl EndIf
2050 If Abs(R-A)>1E-6 Then GoTo 2140:' Wrong
2060 Print "Right! ";
2062 If MM.DEVICE$="PicoMite" Then Bitbang WS2812 B,GP28,1,RGB(green)
2065 Pause 100
2068 If MM.DEVICE$="PicoMite" Then Bitbang WS2812 B,GP28,1,RGB(black)
2070 If 0>=FNMODULUS(TF,100) Then GoTo 2100:' EndIf
2080 L=L-1
2090 AF(K)=TF-1
2100 ' Lbl EndIf
2110 K=K+1
2120 If 0<L Then D=500:GoSub 3402:' Delay
2130 GoTo 2250:' EndIf 2
2140 ' Lbl Wrong
2150 Print "Wrong. ";
2160 Print I;
2170 GoSub 3280:' Sign
2180 Print Mid$(Str$(J),1);" is";R;"."
2190 If 99<=FNMODULUS(TF,100) Then GoTo 2220:' EndIf
2200 L=L+1
2210 AF(K)=TF+1
2220 ' Lbl EndIf
2230 K=K+1
2235 GoSub 3610:' Sound
2240 D=1000:GoSub 3402:' Delay
2250 ' Lbl EndIf 2
2260 If K<=U GoTo 1870:' K
2270 If 0>=L GoTo 2390:' W
2280 Print "Checking..."
2290 N=1
2300 For K=1 To U
2310 TF=AF(K)
2320 If 0=FNMODULUS(TF,100) Then GoTo 2350:' EndIf
2330 AF(N)=TF
2340 N=N+1
2350 ' EndIf
2360 Next K
2370 U=N-1
2380 GoTo 1780:' U
2390 ' Lbl W
2400 Print "Good!!! Well done!"
2410 D=5000:GoSub 3402:' Delay
2420 GoTo 1120:' C
2430 ' Lbl E
2440 GoSub 3410:' ClS
2450 A=0
2460 N=0
2470 TF=M
2480 GoSub 2640:' Sub
2490 Print "Addition 1"
2500 GoSub 2640:' Sub
2510 Print "Addition 2"
2520 GoSub 2640:' Sub
2530 Print "Subtraction 1"
2540 GoSub 2640:' Sub
2550 Print "Subtraction 2"
2560 GoSub 2640:' Sub
2570 Print "Multiplication"
2580 GoSub 2640:' Sub
2590 Print "Division | -1 Esc"
2600 Print "0 OK and Go";
2605 Print N
2610 GoTo 2780:' Input
2640 ' Sub
2650 A=A+1
2660 TF=FNDIV(TF,10)
2670 Print Mid$(Str$(A),1);
2680 If 0=FNMODULUS(TF,10) Then GoTo 2720:' Else
2690 Print "*";
2700 N=N+100-10*(6=A)
2710 GoTo 2740:' EndIf
2720 ' Lbl Else
2730 Print " ";
2740 ' Lbl EndIf
2750 Return
2780 ' Lbl Input
2790 Input "Toggle item 1-6, or choose 0 or -1: ",A
2800 If -1>A Or 6<A Or 0=A And 0=N Then GoTo 2780:' Input
2810 If 0=A Then GoTo 1180:' S
2820 If -1=A Then GoTo 1120:' C
2830 GoSub 3350:' 10^A
2840 If 0=FNMODULUS(FNDIV(M,R),10) Then GoTo 2870:' Else
2850 M=M-R
2860 GoTo 2890:' EndIf
2870 ' Lbl Else
2880 M=M+R
2890 ' EndIf
2900 GoTo 2430:' E
2910 ' Lbl H
2920 GoSub 3470:' Help
2930 GoTo 1120:' C
2940 ' Lbl A
2950 GoSub 3540:' About
2960 GoTo 1120:' C
2970 ' Lbl Q
2980 GoSub 3410:' ClS
2990 End
3000 ' Lbl ContMenu
3010 GoSub 3240:' MenuItem
3020 Print "Continue"
3030 ' Lbl Menu
3040 GoSub 3240:' MenuItem
3050 Print "Setup and Go"
3060 GoSub 3240:' MenuItem
3070 Print "Help"
3080 GoSub 3240:' MenuItem
3090 Print "About"
3100 GoSub 3240:' MenuItem
3110 Print "Exit"
3120 ' Lbl Input
3130 Print "Choose 1-";Mid$(Str$(4+(L>0)),1);
3140 Input ": ",S
3150 If 1>S Or 4+(L>0)<S Then GoTo 3120:' Input
3160 S=S+(L<=0)
3170 ' On S GoTo K,E,H,A,Q
3180 On S GoTo 1870,2430,2910,2940,2970
3230 End
3240 ' Sub MenuItem
3250 A=A+1
3260 Print Mid$(Str$(A),1);" ";
3270 Return
3280 ' Sub Sign
3290 If 1=C Or 2=C Then Print "+";:GoTo 3330:' EndIf
3300 If 3=C Or 4=C Then Print "-";:GoTo 3330:' EndIf
3310 If 5=C Then Print "*";:GoTo 3330:' EndIf
3320 If 6=C Then Print "/";
3330 ' Lbl EndIf
3340 Return
3350 ' Sub 10^A, A>0
3360 R=1
3370 For I=1 To A
3380 R=R*10
3390 Next I
3400 Return
3402 ' Delay
3405 Pause D
3408 Return
3410 ' Sub ClS
3420 For T=1 To 40
3430 Print
3440 Next T
3450 Print "Aritm"
3460 Return
3470 ' Sub Help
3480 GoSub 3410:' ClS
3490 Print "Help"
3500 Print "You can mix problems anyway you like."
3510 Print "More help on <http://aritm.orbin.se/>."
3520 D=5000:GoSub 3402:' Delay
3530 Return
3540 ' Sub About
3550 GoSub 3410:' ClS
3560 Print "About"
3570 Print "Aritm 0.3 (C) 1992-2022 by"
3575 Print "Mikael O. Bonnier, Lund, Sweden."
3580 Print "Absolutely no warranty."
3585 Print "FOSS, see license GPLv3+."
3590 D=5000:GoSub 3402:' Delay
3600 Return
3610 ' Sub Sound
3620 ' For Maker Pi Pico do on command line: Option Audio GP18,GP19
3640 If MM.DEVICE$="PicoMite" Then Bitbang WS2812 B,GP28,1,RGB(red)
3665 Play tone 880,880,200
3670 Pause 200
3680 If MM.DEVICE$="PicoMite" Then Bitbang WS2812 B,GP28,1,RGB(black)
3690 Return
