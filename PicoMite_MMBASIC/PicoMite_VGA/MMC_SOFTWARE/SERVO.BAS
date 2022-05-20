Print Chr$(27)+"[f";:Print Chr$(27)+"[2J";
Print "Servo Test"
Do
  Servo 1,0.8
  Pause 2000
  Servo 1,2.2
  Pause 2000
Loop                                                                                                                              