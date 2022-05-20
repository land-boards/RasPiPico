Open "Com1:38400" As #1
terminal:
a$=""
b$=""
a$=Inkey$
If a$ <> "" Then Print #1,a$;
If Loc(#1) >=1 Then b$=Input$(1,#1)
char = Asc(b$)
If char >31 Then Print b$;
If char =13 Then Print " "
GoTo terminal                                          