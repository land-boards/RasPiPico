' MMBASIC Speed Test
'
' Doug Gilliland

i=0

Print "started at ";Time$

i=0
loopHere:
  i=i+1
  If i<100000 GoTo loopHere
Print "ended at ";Time$
