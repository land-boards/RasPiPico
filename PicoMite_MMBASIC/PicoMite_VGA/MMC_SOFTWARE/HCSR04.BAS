''    ________________
''    |  __      __  |  HCSR04 UltraSonic Sensor
''    | /  \    /  \ |  Connect 5v/Gnd
''    | \__/    \__/ |  Trigger Micromite 21
''    |______________|  Echo Micromite 22
''          ||||

start:
d=Distance(21,22)
If d < 190 Then Print "Distance: ";d
Pause 40
GoTo start

                                                                        