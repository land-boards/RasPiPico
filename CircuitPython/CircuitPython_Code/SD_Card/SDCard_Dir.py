import os

import board
import busio
import sdcardio
import storage

# OPTION SDCARD GP13, GP10, GP11, GP12
# CS = D13
# SCK = D10
# MOSI = D11
# MISO = D12

spi = busio.SPI(board.GP10, MOSI=board.GP11, MISO=board.GP12)

# For breakout boards, you can choose any GPIO pin that's convenient:
cs = board.GP13
# Boards with built in SPI SD card slots will generally have a
# pin called SD_CS:
#cs = board.SD_CS

sdcard = sdcardio.SDCard(spi, cs)
vfs = storage.VfsFat(sdcard)

storage.mount(vfs, "/sd")

def print_directory(path, tabs=0):
    for file in os.listdir(path):
        stats = os.stat(path + "/" + file)
        filesize = stats[6]
        isdir = stats[0] & 0x4000

        if filesize < 1000:
            sizestr = str(filesize) + " by"
        elif filesize < 1000000:
            sizestr = "%0.1f KB" % (filesize / 1000)
        else:
            sizestr = "%0.1f MB" % (filesize / 1000000)

        prettyprintname = ""
        for _ in range(tabs):
            prettyprintname += "   "
        prettyprintname += file
        if isdir:
            prettyprintname += "/"
        print('{0:<40} Size: {1:>10}'.format(prettyprintname, sizestr))

        # recursively print directory contents
        if isdir:
            print_directory(path + "/" + file, tabs + 1)


print("Files on filesystem:")
print("====================")
print_directory("/sd")
