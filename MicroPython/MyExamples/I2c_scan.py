import machine
import time

# Create I2C object
i2c = machine.I2C(0, scl=machine.Pin(17), sda=machine.Pin(16), freq=400000)

# Print out any addresses found
devices = i2c.scan()

if devices:
    print("I2C device(s) found at: ",end='')
    for d in devices:
        print(hex(d),end=' ')
    print()
else:
    print("No I2C devices found")
