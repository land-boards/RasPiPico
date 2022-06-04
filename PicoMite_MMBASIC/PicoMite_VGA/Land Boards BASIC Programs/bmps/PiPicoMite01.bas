' PiPicoMite01.bas
' Load Image to screen
; Wait for keypress to exit
;

MODE 2
Load image "PiPicoMite01.bmp"
loopToKey:
If Inkey$="" GoTo loopToKey
MODE 1
