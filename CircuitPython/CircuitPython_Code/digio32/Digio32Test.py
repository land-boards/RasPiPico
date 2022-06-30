###############################################################################
# Digio32Test.py
# CircuitPython application to bounce an LED across 32-outputs
# Attached LED-32 card
#    http://land-boards.com/blwiki/index.php?title=LED-32
# Uses the PiPicoMite02_Digio32 module
###############################################################################

import board
import busio
import time
from PiPicoMite02_Digio32 import OnBoardDigio

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

digio32 = OnBoardDigio()

digio32.initMCP23017x2(i2c)

for bit in range(32):
    digio32.pinMode(i2c,bit,OUTPUT)

initSpeed = 0.25
speed = initSpeed
while True:
    for bit in range(32):
        digio32.digitalWrite(i2c,31-bit,1)
        time.sleep(speed)
        digio32.digitalWrite(i2c,31-bit,0)
    speed -= 0.01
    if speed <= 0.00:
        speed = initSpeed
    
i2c.unlock()
