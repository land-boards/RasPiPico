' Digital Clock (DigClk.bas)
' From The PicoMiteVGA MMBASIC Ver5.07.04 User Manual p 23

MODE 2

RTC gettime

Const DBlue = RGB(0,0,128)
Colour RGB(green), RGB(black)
Font 1, 2

CLS

Box 0, 0, MM.HRes-3, MM.VRes/2, 3, RGB(red), DBlue

Do
  Text MM.HRes/2, MM.VRes/4, Time$, "CM", 1, 4, RGB(CYAN), DBlue
  Text MM.HRes/2, MM.VRes*3/4, Date$, "CM"
  If Inkey$ <> "" Then GoTo atEnd
Loop
atEnd:
MODE 1
