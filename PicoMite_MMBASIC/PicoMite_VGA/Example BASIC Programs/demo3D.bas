Option explicit
Option default none
MODE 2
Dim float phi=(1+Sqr(5))/2
' data for location of vertices for truncated icosahedron of edge length 2
Data 0,1,3*phi
Data 0,1,-3*phi
Data 0,-1,3*phi
Data 0,-1,-3*phi
Data 1,3*phi,0
Data 1,-3*phi,0
Data -1,3*phi,0
Data -1,-3*phi,0
Data 3*phi,0,1
Data 3*phi,0,-1
Data -3*phi,0,1
Data -3*phi,0,-1
Data 2,(1+2*phi),phi
Data 2,(1+2*phi),-phi
Data 2,-(1+2*phi),phi
Data 2,-(1+2*phi),-phi
Data -2,(1+2*phi),phi
Data -2,(1+2*phi),-phi
Data -2,-(1+2*phi),phi
Data -2,-(1+2*phi),-phi
Data (1+2*phi),phi,2
Data (1+2*phi),phi,-2
Data (1+2*phi),-phi,2
Data (1+2*phi),-phi,-2
Data -(1+2*phi),phi,2
Data -(1+2*phi),phi,-2
Data -(1+2*phi),-phi,2
Data -(1+2*phi),-phi,-2
Data phi,2,(1+2*phi)
Data phi,2,-(1+2*phi)
Data phi,-2,(1+2*phi)
Data phi,-2,-(1+2*phi)
Data -phi,2,(1+2*phi)
Data -phi,2,-(1+2*phi)
Data -phi,-2,(1+2*phi)
Data -phi,-2,-(1+2*phi)
Data 1,(2+phi),2*phi
Data 1,(2+phi),-2*phi
Data 1,-(2+phi),2*phi
Data 1,-(2+phi),-2*phi
Data -1,(2+phi),2*phi
Data -1,(2+phi),-2*phi
Data -1,-(2+phi),2*phi
Data -1,-(2+phi),-2*phi
Data (2+phi),2*phi,1
Data (2+phi),2*phi,-1
Data (2+phi),-2*phi,1
Data (2+phi),-2*phi,-1
Data -(2+phi),2*phi,1
Data -(2+phi),2*phi,-1
Data -(2+phi),-2*phi,1
Data -(2+phi),-2*phi,-1
Data 2*phi,1,(2+phi)
Data 2*phi,1,-(2+phi)
Data 2*phi,-1,(2+phi)
Data 2*phi,-1,-(2+phi)
Data -2*phi,1,(2+phi)
Data -2*phi,1,-(2+phi)
Data -2*phi,-1,(2+phi)
Data -2*phi,-1,-(2+phi)
' 12 faces with 5 sides
Data 0,28,36,40,32
Data 33,41,37,29,1
Data 34,42,38,30,2
Data 3,31,39,43,35
Data 4,12,44,45,13
Data 15,47,46,14,5
Data 17,49,48,16,6
Data 7,18,50,51,19
Data 8,20,52,54,22
Data 23,55,53,21,9
Data 26,58,56,24,10
Data 25,57,59,27,11
' 20 faces with 6 sides
Data 32,56,58,34,2,0
Data 0,2,30,54,52,28
Data 29,53,55,31,3,1
Data 1,3,35,59,57,33
Data 13,37,41,17,6,4
Data 4,6,16,40,36,12
Data 5,7,19,43,39,15
'data 14,38,42,18,7,5
Data 22,46,47,23,9,8
Data 8,9,21,45,44,20
Data 10,11,27,51,50,26
Data 24,48,49,25,11,10
Data 36,28,52,20,44,12
Data 13,45,21,53,29,37
Data 14,46,22,54,30,38
Data 39,31,55,23,47,15
Data 16,48,24,56,32,40
Data 41,33,57,25,49,17
Data 42,34,58,26,50,18
Data 19,51,27,59,35,43
'
Dim float q1(4)
Dim float yaw=Rad(1),pitch=Rad(2),roll=Rad(0.5)
Dim integer i, j, nf=31, nv=60, camera=1
Dim float vertices(2,59)
' read in the coordinates of the verticies and scale
For j=0 To 59
For i=0 To 2
 Read vertices(i,j)
 vertices(i,j)=vertices(i,j)*20
Next i
Next j
'math scale vertices(),50,vertices()
'
Dim integer faces(173)
For i=0 To 173
Read faces(i)
Next i

Dim integer fc(30)=  (5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6)
Dim integer colours(2)=(RGB(red),RGB(white),RGB(black))
Dim integer edge(30)=(2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2)
Dim integer fill(30)=(0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
Math q_create Rad(2),1,0.5,0.25,q1()
Draw3D create 1, nv,nf, camera, vertices(), fc(), faces(), colours(), edge(),fill()
Draw3D camera 1,800,0,0
'draw3d diagnose 1,mm.hres\2,mm.vres\2,1000
FRAMEBUFFER create
FRAMEBUFFER write f
Draw3D show 1,0,0,1000,1
Do
Math q_euler yaw,pitch,roll,q1()
Inc yaw,Rad(1)
Inc pitch,Rad(2)
Inc roll,Rad(0.5)
Draw3D rotate q1(),1
Draw3D show 1,0,0,1000,1
FRAMEBUFFER copy f,n,b
Loop