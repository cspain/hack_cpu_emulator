
SCREEN_SIZE_BYTES = 1024
SCREEN_WIDTH_PELS = 40
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
	
	

infile = open("C:\Users\Chris2.chris-HP-Select\Documents\GitHub\hack_cpu_emulator\screenDump.txt",'r')

currentScreen = readNlines(150,infile)
print currentScreen

bitVal = getScreenVal([14,9],currentScreen)
	
print "Bit at (14,7) is " 
print bitVal


raw_input("PRESS ENTER TO QUIT")



