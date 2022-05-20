' Sine Wave
' From Creative Computng by David Ahl
' Coverted to Micromite by Jeff Ledger
b=0
For t= 0 To 40 Step .25
a=Int(26+25*Sin(t))
Print Tab(a);
Pause 10
If b=1 Then GoTo secondword
Print "Creative"
b=1
GoTo startover
secondword:
Print "Computing"
b=0
startover:
Next t                                                                                             