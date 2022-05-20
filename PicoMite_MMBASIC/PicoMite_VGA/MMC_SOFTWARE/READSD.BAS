i2c open 10,100

SendFilename "readsd.bas"
cls
readfile:
PropellerCMD 186 'Read byte from SD into I2C register
i2c WRITE &h42,0,1,&h0
i2c READ &h42,0,1,a$ 'Read register 1 in A
if asc(a$) = 255 then end
print a$;
goto readfile

sub SendFilename file$
 PropellerCMD 230 'Clear all the i2c registers
 for x = 1 to len(file$)
 i2c write &h42,0,2,x+1,asc(mid$(file$,x,1))
 next x
 PropellerCMD 180  'Tell Propeller to copy filename from registers
 PropellerCMD 181  'Tell Propeller to open the file for reading
end sub

sub PropellerCMD cmd
  i2c WRITE &h42,0,2,1,cmd
end sub

sub PropellerVAR var,data
  i2c WRITE &h42,0,2,var,data
end sub

sub CLS
  propellerCMD 204
end sub                                  
