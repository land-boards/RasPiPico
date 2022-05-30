' Colour Bars

MODE 2
CLS
Font 7
For y = 0 To 240 - 240/15 Step 240/16
  Read s$, c
  Text 0, y + 4, s$ + " = RGB(" + Str$(c>>16) + "," + Str$((c>>8) And &B11111111) + "," + Str$(c And &B11111111) + ")"
  Box 155, y, 320 - 155, 240/16, 1, c, c
Next y

Data "white", RGB(white)
Data "yellow", RGB(yellow)
Data "lilac", RGB(lilac)
Data "brown", RGB(brown)
Data "fuchsia", RGB(fuchsia)
Data "rust", RGB(rust)
Data "magenta", RGB(magenta)
Data "red", RGB(red)
Data "cyan", RGB(cyan)
Data "green", RGB(green)
Data "cerulean", RGB(cerulean)
Data "midgreen", RGB(midgreen)
Data "cobalt", RGB(cobalt)
Data "myrtle", RGB(myrtle)
Data "blue", RGB(blue)
Print @(0,y);
