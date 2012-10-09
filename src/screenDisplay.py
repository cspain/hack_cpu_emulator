#!/usr/bin/env python
# Example taken from the pygame library - good intro to drawing a screen and setting pixel
# values.

#Step 1: Add a reader function to get in the data
#Step 2: Try displaying the first page
#NOTE: probably should only read in a few frames at a time into a buffer
# 		otherwise we might be trying to hold on to MB of screen data uneccessarily.


"""A simple starfield example. Note you can move the 'center' of
the starfield by leftclicking in the window. This example show
the basics of creating a window, simple pixel plotting, and input
event management"""


import random, math, pygame
from pygame.locals import *
from numpy  import *
import time

#constants
WINSIZE = [512, 256]
WINCENTER = [320, 240]
NUMPELS = 9900
SCREEN_SIZE_BYTES = WINSIZE[0]*WINSIZE[1]/8
SCREEN_WIDTH_PELS = WINSIZE[0]
infile = open('C:\Users\Chris2.chris-HP-Select\Documents\GitHub\hack_cpu_emulator\screenDump.txt','r')
SCREEN = zeros((256, 512))

#############################
# Read in n lines from a file
#############################
def readNlines(numLines, fileToRead):
	readInLines = []
	for i in range(numLines):
		value = fileToRead.readline()
		if value == '':
			print 'ERROR EOF? ' + str(i)
			break
		
		readInLines.append(int(value))
	
	
	return readInLines

#######################################
# Get value of the given screen coord
# Return 0 or 1 depending on what that
# pixel should be based on the memory
# screen dump
#
#Let's pretend we have a 5*8 x 4 screen - 20 bytes per screenful:
#012,123,156,232,111
#045,232,221,232.123
#134,021,242,000,121
#121,044,221,127,209
#First line:
#00001100.01111011.10011100.11101000.11011110
#01011100.10001011.10101100.10101000.11010011
#Maybe interate over whole screen line by line, for each pixel coord check the corresponding byte to see if it is 0 or 1
#(1)First 8 pels map to byte 1, next 8 to byte 2 - so which byte is col-coord/8 ?
# So in the 1 line example coord 2 would be 2/8 = 0 so byte 0 and coord 21 would be 21/8 = 2 so byte 2
#(2) Now we know which byte in the row, we need to know which bit in that byte represents our current pel
# Get this using bitN = coord mod 8, so coord 21 would be 21 mod 8 = 5, so bit 5 in byte 2
# MUST INDEX BYTES AND COORDS FROM 0
####################################### (1,31) = 0 and is in row 1, byte 3

def getScreenVal(coord, screenData):
	#print "INSIDE getScreenVAL"
	#First get the row
	row = coord[0]
	#Now col
	col = coord[1]
	#Get the correct byte out
	byteNum = (SCREEN_WIDTH_PELS * row + col)/8
	bitNum = (SCREEN_WIDTH_PELS * row + col) % 8
	#print "byteNum is " + str(byteNum) + " bitNum is " + str(bitNum)
	#Ok now we know which byte and which bit in that byte, we need to get that bit
	byteData = screenData[byteNum];
	#print "-------------- byte data is " + str(byteData)
	
	if ((byteData & (1<<bitNum)) > 0):
		return 1
	else:
		return 0
	

def init_pel():
    "creates new star values"
    dir = random.randrange(100000)
    velmult = random.random()*.6+.4
    vel = [math.sin(dir) * velmult, math.cos(dir) * velmult]
    return vel, WINCENTER[:]


def initialize_pels():
    "creates a new starfield"
    

def clear_display(surface):
	color = [255,255,255]
	
	"used to draw (and clear) the display"
      
	for col in range(512):
		for row in range(256):
			surface.set_at((row, col), color)
		
	
	
def draw_display(surface):
	color = [0,0,0]
	
	"used to draw (and clear) the display"
      
	for col in range(512):
		for row in range(256):
			color[0] = 255*SCREEN[row,col]
			color[1] = 0
			color[2] = 0
			surface.set_at((col, row), color)
		

def update_pels(pels):
	"Read in the next screen of data"
	
	
	#Read raw file data
	rawScreenData = readNlines(16384, infile)
	#Convert raw file data to pels  - pos is x and y, vel[0] is color
	#Want to represent pels on screen = need a 2D array - each element is a color, its r-c coords are x y vals
	for col in range(512):
		for row in range(256):
			SCREEN[row,col] = getScreenVal((row,col), rawScreenData)
	
	#print SCREEN
	
  

def main():
    "This is the starfield code"
    #create our starfield
    random.seed()
    pels = initialize_pels()
    clock = pygame.time.Clock()
    #initialize and prepare screen
    pygame.init()
    screen = pygame.display.set_mode(WINSIZE)
    pygame.display.set_caption('pygame Stars Example')
    white = 255, 0, 200
    black = 10, 10, 10
    red = [200, 10, 10]
    screen.fill(black)

    #main game loop
    done = 0
    while not done:
        #draw_pels(screen, pels, black) # Overwrite previously drawn star with black to make it disappear
        #move_pels(pels)
		print 'Drawing display...\n'
		draw_display(screen)
		print 'Updating pels...\n'
		update_pels(pels)
		print 'pygame display update\n'
        #draw_pels(screen, pels, white) # Redraw star in new position
		pygame.display.update()
		print 'sleeeeeping....'
		time.sleep(1)
		print 'wakey!'
		#clear_display(screen)
		pygame.display.update()
		print 'press enter...\n'
		a = raw_input()
		
		for e in pygame.event.get():
		
			if e.type == QUIT or (e.type == KEYUP and e.key == K_ESCAPE):
				done = 1
				break
			elif e.type == MOUSEBUTTONDOWN and e.button == 1:
				WINCENTER[:] = list(e.pos)
		clock.tick(50)

# if python says run, then we should run
if __name__ == '__main__':
	main()
	infile.close()

