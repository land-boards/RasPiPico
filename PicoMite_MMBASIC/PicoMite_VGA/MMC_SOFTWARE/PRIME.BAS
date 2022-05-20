10 REM Save as PRIME-ID.BAS
20 PRINT CHR$(27)+"[f";:PRINT CHR$(27)+"[2J";
30 PRINT "The book 'THE CURIOUS INCIDENT OF THE DOG IN THE NIGHT-TIME'"
40 PRINT "tells how Jesus Christ, Sherlock Holmes and Doctor Watson"
50 PRINT "are prime numbers if their letters are replaced by numbers." :PRINT
60 PRINT "If you enter your name (upper or lower case, or mixed)"
70 PRINT "the program will tell you if it is a prime number." :PRINT
80 INPUT "What is your name" ;NM$
90 TOTAL=0
100 FOR K=1 TO LEN(NM$)
110   CHR=ASC(MID$(NM$,K,1))
120   IF CHR=32 THEN 160
130   IF CHR > 90 THEN  X=96 ELSE X=64
140   N=CHR-X
150   TOTAL=TOTAL+N
160 NEXT K
170 PRINT :PRINT TOTAL
180 ROOT=INT(SQR(TOTAL))
190 FOR J=2 TO ROOT
200   FACTOR=TOTAL/J
210   IF FACTOR=INT(FACTOR) THEN PRINT "Your name is not prime." :GOTO 240
220 NEXT J
230 PRINT "Your name is a prime number!"
240 END               