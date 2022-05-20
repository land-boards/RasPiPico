I2C OPEN 10,100
i2caddr = &h52 'Nunchuck Address
Dim RDbuff(255)
I2C write i2caddr,0,2,&hf0,&h55
I2C write i2caddr,0,2,&hfb,&h0
readwii:
I2C write i2caddr,0,1,&h0
I2C read i2caddr,0,6,RDBuff(0)
Print Chr$(27)+"[f";:Print Chr$(27)+"[2J";
Print "  Joy X:";RDBuff(0)
Print "  Joy Y:";RDBuff(1)
Print " AccelX:";RDBuff(2)
Print " AccelY:";RDBuff(3)
Print " AccelZ:";RDBuff(4)
Print "ButtonZ:";(RDbuff(5) And &h01)
Print "ButtonC:";(RDbuff(5) And &h02)
Pause 200
GoTo readwii                         