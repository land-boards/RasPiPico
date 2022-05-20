I2C OPEN 10,100
SendFilename "test.txt"

writefile:
For x = 65 To 122
PropellerVAR 1,x
PropellerCMD 185
Next x
PropellerCMD 189
End

Sub SendFilename file$
 PropellerCMD 230 'Clear all the i2c registers
 For x = 1 To Len(file$)
 I2C write &h42,0,2,x+1,Asc(Mid$(file$,x,1))
 Next x
 PropellerCMD 180  'Tell Propeller to copy filename from registers
 PropellerCMD 182  'Tell Propeller to open the file for writing
 Pause 400
End Sub

Sub PropellerCMD cmd
  I2C WRITE &h42,0,2,1,cmd
End Sub

Sub PropellerVAR var,data
  I2C WRITE &h42,0,2,var,data
End Sub

Sub CLS
  propellerCMD 204
End Sub                                                                                            
