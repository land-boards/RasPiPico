' boxes.bas - Draw square boxes
' Runs under MMBASIC on PicoMiteVGA
MODE 2:' 320x240, 16-colours
bs=16:' Box size
' Pre-calc loop boundaries
xM=Int(320/bs)-1
yM=Int(240/bs)-1
nF=10:'number of frames to display
' Get start time (mS)
'bC=RGB(128,128+64,128)
tS=Timer
For fc=1 To nF
For y=0 To yM
For x=0 To xM
r=Int(256*Rnd)
g=Int(256*Rnd)
b=Int(256*Rnd)
bC=RGB(r,g,b)
Box bs*x,bs*y,bs,bs,0,bC,bC
Next x
Next y
Next fc
dT = Timer-tS
CLS
Print
Print 1/(dT/1000/nF);
Print " fps, box size";bs                                                                                                                              