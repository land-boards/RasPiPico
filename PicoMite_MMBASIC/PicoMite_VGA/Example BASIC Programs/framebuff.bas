MODE 2
'clear the main screen and create a grid
CLS
For i=0 To MM.HRes-1 Step 10
Line i,0,i,MM.VRes-1
Next
For i=0 To MM.VRes-1 Step 10
Line 0,i,MM.HRes-1,i
Next

'Create a framebuffer and a layer buffer
'set graphics commands to write to the framebuffer
FRAMEBUFFER create
FRAMEBUFFER layer
FRAMEBUFFER write f

'create a moving sine wave in the framebuffer
' then copy the framebuffer to the layer buffer in frame blanking
' so it appears on top of the grid
j=0
Do
y=Sin(Rad(0+j))*MM.VRes\3+MM.VRes\2
For i=1 To MM.HRes-1
  yp=Sin(Rad(i+j))*MM.VRes\3+MM.VRes\2
  Line i,y,i,yp,1,RGB(green)
  y=yp
Next
Inc j
FRAMEBUFFER copy f,l,b
CLS
Loop