'' RTTTL Player for Micromite using PWM pin.  Pin 4.
'' Name and Default values have been removed from the songs.
'' By Jeff Ledger
''
'' MMC Instructions.  Jumper Micromite Pin 4(PWM1) to P10/Aux or P11/Aux with a signal diode
'' Unremark the song you want to play and run the program.

''The Simpsons
SONG$ = "c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c,8c6,8c6,c6"

''Entertainer
'SONG$="8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6"

''Looney
'SONG$="32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f"

''MASH
'SONG$="4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b"

''A-Team
'SONG$="4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#"

''Smurfs
'SONG$="4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#"

'' RTTTL Player for Micromite
song$=song$+","
length=LEN(song$)
pointer=1
PRINT song$

songplay:
IF pointer > length THEN END
'' Process note duration
IF MID$(song$,pointer,2) = "32" THEN SETDUR 42,2
IF MID$(song$,pointer,2) = "16" THEN SETDUR 95,2
IF MID$(song$,pointer,1) = "8" THEN SETDUR 188,1
IF MID$(song$,pointer,1) = "4" THEN SETDUR 375,1
IF MID$(song$,pointer,1) = "2" THEN SETDUR 750,1
IF MID$(song$,pointer,1) = "1" THEN SETDUR 900,1

'' Process sharps
IF MID$(song$,pointer,2) = "a#" THEN SETNOTE 466.08,2
IF MID$(song$,pointer,2) = "c#" THEN SETNOTE 277.18,2
IF MID$(song$,pointer,2) = "d#" THEN SETNOTE 311.13,2
IF MID$(song$,pointer,2) = "f#" THEN SETNOTE 369.13,2
IF MID$(song$,pointer,2) = "g#" THEN SETNOTE 415.30,2

'' Process notes
IF MID$(song$,pointer,1) = "a" THEN SETNOTE 440,1
IF MID$(song$,pointer,1) = "b" THEN SETNOTE 493,1
IF MID$(song$,pointer,1) = "c" THEN SETNOTE 261.63,1
IF MID$(song$,pointer,1) = "d" THEN SETNOTE 293.66,1
IF MID$(song$,pointer,1) = "e" THEN SETNOTE 329.63,1
IF MID$(song$,pointer,1) = "f" THEN SETNOTE 349.23,1
IF MID$(song$,pointer,1) = "g" THEN SETNOTE 392,1
IF MID$(song$,pointer,1) = "p" THEN SETNOTE 0,1

IF MID$(song$,pointer,1) = "." THEN
  notedur=notedur+128 : pointer=pointer+1
ENDIF

'' Process Octaves
oct=4
IF MID$(song$,pointer,1) = "5" THEN SETOCTAVE 5,1
IF MID$(song$,pointer,1) = "6" THEN SETOCTAVE 6,1
IF MID$(song$,pointer,1) = "7" THEN SETOCTAVE 7,1

IF MID$(song$,pointer,1) = "," THEN play (note,oct,notedur)
IF MID$(song$,pointer,1) = "." THEN pointer=pointer+1
GOTO songplay

SUB play (n,o,d)
  'Estimate note ranges for other octaves.
  IF o = 5 THEN n = n * 2
  IF o = 6 THEN n = n * 4
  IF o = 7 THEN n = n * 8
  IF n > 0 THEN PWM 1,n,5
  IF n = 0 THEN d = 10
  PAUSE d + 25
  PWM 1,stop
  pointer=pointer+1
END SUB

SUB SETDUR ndur,adv
  notedur=ndur
  pointer=pointer+adv
END SUB

SUB SETNOTE tone,adv
  note=tone
  pointer=pointer+adv
END SUB

SUB SETOCTAVE o,adv
  oct=o
  pointer=pointer+adv
END SUB                                                                                          
