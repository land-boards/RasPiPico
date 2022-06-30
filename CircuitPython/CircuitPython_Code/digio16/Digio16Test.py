###############################################################################
# Digio16Test.py
# CircuitPython application to bounce an LED across 16-outputs
# Attached LED-32 card
#    http://land-boards.com/blwiki/index.php?title=LED-32
# Uses the PiPicoMite01_Digio16 module
###############################################################################

import board
import busio
import time
from PiPicoMite01_Digio16 import OnBoardDigio

INPUT=0x0
OUTPUT=0x1
INPUT_PULLUP=0x2

i2c = busio.I2C(board.GP15, board.GP14, frequency=400000)    # Pi Pico RP2040

# Lock the I2C device before we try to scan
while not i2c.try_lock():
    pass

# Print the addresses found once
i2cScanList = i2c.scan()
# print("I2C addresses found:", [hex(device_address) for device_address in i2cScanList])
if len(i2cScanList) < 2:
    assert False,"Bad count"
if (0x20 not in i2cScanList) or (0x21 not in i2cScanList):
    print("i2cScanList",i2cScanList)
    assert False,"Didn't find 2x MCP23017 parts at expected locations"

digio16 = OnBoardDigio()

digio16.initMCP23017(i2c)

for bit in range(16):
    digio16.pinMode(i2c,bit,OUTPUT)

initSpeed = 0.25
speed = initSpeed
while True:
    for bit in range(16):
        digio16.digitalWrite(i2c,15-bit,1)
        time.sleep(speed)
        digio16.digitalWrite(i2c,15-bit,0)
    speed -= 0.01
    if speed <= 0.00:
        speed = initSpeed
    
i2c.unlock()

