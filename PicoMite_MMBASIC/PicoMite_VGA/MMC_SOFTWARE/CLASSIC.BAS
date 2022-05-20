'' CLASSIC.BAS V1.1 - By Jeff Ledger
'' Returns Left Controller & Digital Buttons from Wii Classic Controller.
'' Tested with licensed controller, may not work on clone controllers.

I2C OPEN 400,100
i2caddr = &h52 'Wii Classic Controller Address
Dim RDbuff(7)
I2C write i2caddr,0,2,&hf0,&h55
I2C write i2caddr,0,2,&hfb,&h0
I2C write i2caddr,0,1,&h0
Pause 100

Readwii:
I2C write i2caddr,0,1,&h0
I2C read i2caddr,0,6,RDBuff(0)
Print Chr$(27)+"[f"; :Print Chr$(27)+"[2J";
rt=0:lt=0:up=0:down=0:right=0:left=0:home=0:select=0:start=0:btX=0:btY=0:btA=0:btB=0
LeftX = (RDBuff(0) And &b00111111)
LeftY = (RDBuff(1) And &b00111111)
If (RDBuff(3) And &b00011111) = 31 Then rt=1
If (RDBuff(3) And &b11100000) = 224 Then lt=1

If Mid$(Bin$(RDBuff(5)),8,1) = "0" Then up = 1
If Mid$(Bin$(RDBuff(4)),2,1) = "0" Then down=1
If Len(Bin$(RDBuff(4))) = 7 Then right=1
If Mid$(Bin$(RDBuff(5)),7,1) = "0" Then left=1
If Mid$(Bin$(RDBuff(4)),4,1) = "0" Then select=1
If Mid$(Bin$(RDBuff(4)),5,1) = "0" Then home=1
If Mid$(Bin$(RDBuff(4)),6,1) = "0" Then start=1
If Mid$(Bin$(RDBuff(5)),5,1) = "0" Then BtX=1
If Mid$(Bin$(RDBuff(5)),3,1) = "0" Then BtY=1
If Mid$(Bin$(RDBuff(5)),2,1) = "0" Then BtB=1
If Mid$(Bin$(RDBuff(5)),4,1) = "0" Then BtA=1

Print "LX:";LeftX;
Print " LY:";LeftY;
Print " LT:";lt;
Print " RT:";rt;
Print " Up:";up;
Print " Down:";down;
Print " Left:";left;
Print " Right:";right
Print "Select:";select;
Print " Home:";home;
Print " Start:";start;
Print " BX:";BtX;
Print " BY:";BtY;
Print " BA:";BtA;
Print " BB:";BtB
Pause 100
GoTo readwii                                                                      