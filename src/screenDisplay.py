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

#constants
WINSIZE = [512, 256]
WINCENTER = [320, 240]
NUMSTARS = 9900
SCREEN_SIZE_BYTES = WINSIZE[0]*WINSIZE[1]/8
SCREEN_WIDTH_PELS = WINSIZE[0]



#############################
# Read in n lines from a file
#############################
def readNlines(numLines, fileToRead):
	readInLines = []
	for i in range(numLines):
		value = int(fileToRead.readline())
		readInLines.append(value)
	
	
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
	print "INSIDE getScreenVAL"
	#First get the row
	row = coord[0]
	#Now col
	col = coord[1]
	#Get the correct byte out
	byteNum = (SCREEN_WIDTH_PELS * row + col)/8
	bitNum = (SCREEN_WIDTH_PELS * row + col) % 8
	print "byteNum is " + str(byteNum) + " bitNum is " + str(bitNum)
	#Ok now we know which byte and which bit in that byte, we need to get that bit
	byteData = screenData[byteNum];
	print "-------------- byte data is " + str(byteData)
	
	if ((byteData & (1<<bitNum)) > 0):
		return 1
	else:
		return 0
	

def init_star():
    "creates new star values"
    dir = random.randrange(100000)
    velmult = random.random()*.6+.4
    vel = [math.sin(dir) * velmult, math.cos(dir) * velmult]
    return vel, WINCENTER[:]


def initialize_stars():
    "creates a new starfield"
    stars = []
    for x in range(NUMSTARS):
        star = init_star()
        vel, pos = star
        steps = random.randint(0, WINCENTER[0])
        pos[0] = pos[0] + (vel[0] * steps)
        pos[1] = pos[1] + (vel[1] * steps)
        vel[0] = vel[0] * (steps * .09)
        vel[1] = vel[1] * (steps * .09)
        stars.append(star)
    move_stars(stars)
    return stars
  

def draw_stars(surface, stars, color):
    "used to draw (and clear) the stars"
    for vel, pos in stars:
        pos = (int(pos[0]), int(pos[1]))
        surface.set_at(pos, color)

		
def draw_display(surface, pels, color):
    "used to draw (and clear) the display"
    for vel, pos in stars:
        pos = (int(pos[0]), int(pos[1]))
        surface.set_at(pos, color)


def move_stars(stars):
    "animate the star values"
    for vel, pos in stars:
        pos[0] = pos[0] + vel[0]
        pos[1] = pos[1] + vel[1]
        if not 0 <= pos[0] <= WINSIZE[0] or not 0 <= pos[1] <= WINSIZE[1]:
            vel[:], pos[:] = init_star()
        else:
            vel[0] = vel[0] * 1.05
            vel[1] = vel[1] * 1.05
  

def main():
    "This is the starfield code"
    #create our starfield
    random.seed()
    stars = initialize_stars()
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
        #draw_stars(screen, stars, black) # Overwrite previously drawn star with black to make it disappear
        #move_stars(stars)
		draw_display(screen, pels, black)
		update_(pels)
        #draw_stars(screen, stars, white) # Redraw star in new position
		draw_display(screen, pels, white)
        pygame.display.update()
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


