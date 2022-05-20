' LCD.bas
' sept. 12, 2014

' simple demo program for
' Parallax part #27977-RT
'   2x16 serial LCD
' using default com1 speed of 9600 baud


'--------   LCD connections   -------------------
'  Power pin to 5 Vdc                           |
'  GND pin to 0 Vdc (Vss)                       |
'  RX pin to micromite pin 21 (com1 transmit)   |
'    no resistors needed.                       |
'------------------------------------------------


Print " DEMO LCD part number 277977-RT"
Print " press b key to toggle backlight ON/OFF"

Open "COM1:" As #5
tx(12):tx(22):tx(17) ' clear screen: display on(no cursor), backlight on

On KEY backlight  ' interupt to toggle backlight

tx(128) ' home position
Print #5, "  Parallax LCD"
tx(148)
Pause 2000
Print #5, "      Demo"
Pause 2000
tx(148)
Print #5, " part# 27977-RT"
Pause 2000


tx(128)
For j = 0 To 15
tx(32)
Pause 150
Next j



tx(148)
Print #5, " Custom Chars  "
tx(128)

tx(250) ' define custom character #2
tx(0):tx(4):tx(14):tx(31): :tx(14):tx(4):tx(0):tx(0)

For idx = 0 To 2
  tx(128)
  For j = 0 To 15
    tx(2)
    Pause 50
  Next j
  Pause 100

  tx(128)
  For j = 0 To 15
    tx(32)
    Pause 50
  Next j
Next idx
  tx(148)
  Pause 500


' count down HEX digits
tx(148):Print #5, "count down (Hex)":Pause 500
x = 15
For j = 143 To 128 Step -1
    tx(j)
    Print #5, Right$(Hex$(x), 1);
    Pause 200
    x = x-1
Next j

Pause 1000

tx(148)
Print #5, Space$(16)
Pause 1000
tx(148)
For count = -10 To 10

  Print #5, Str$(count,9)
  Pause 500

Next count

Pause 1000

tx(18)
Close #5

Sub tx(byte)   ' send control bytes to LCD screen
Print #5, Chr$(byte);
Pause(5)
End Sub


Sub backlight
  vari$ = Inkey$

  If vari$ = "b" Then
      If toggle = 0 Then
          toggle = 1
          Print "backlight OFF"
          tx(18)
      Else
          toggle = 0
          Print "backlight ON"
          tx(17)
      EndIf
  EndIf

End Sub                                    