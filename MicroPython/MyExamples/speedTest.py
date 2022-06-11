# speedTest.py

import time

i=0
start = time.ticks_ms() # get millisecond counter
while i < 100000:
    i=i+1
delta = time.ticks_diff(time.ticks_ms(), start) # compute time differenceprint("start")
print("end",delta)

