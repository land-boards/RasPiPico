Print "Hit key to continue"
MODE 2
Load image "PiPicoMite01.bmp"
loopRead:
If Inkey$="" GoTo loopRead
MODE 1               