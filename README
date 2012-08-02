This is a basic project for me to learn more about emulators. I decided to start by emulating the simple Hack CPU described in The Elements of Computing Systems (TECS): http://www1.idc.ac.il/tecs/

I got the book because although I have a background in computer engineering, it has been about three or four years since I messed around at the level of logic gates. I also wanted to learn more about the other end of the stack - how assemblers and compilers work at a fundamental level.

I followed the book and created the Hack CPU using their version of a HDL. I got frustrated with some deficiencies with the TECS HDL and I wondered about emulating in C. If I get to the assembler and compiler, I might wimp out and go with Python!

I realized I didn't know how to write an emulator and found this awesome site:

http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-The-CPU

As I currently know nothing about emulators, I am using the Gameboy emulator as a template, only I plan to write mine in C.

The Hack CPU is a somewhat unusual architecture, and my implementation is a bit odd too :) I wanted to have a function for every single instruction even though a few general functions would have been more efficient to write. In other words I currently have separate functions for 
mov A,B
mov B,A
mov D,A
etc

Where I could just have had a general:

void move(dest, src) 

For some reason I thought the former had more of a 'hardwareness' about it :)

At the time of writing I have a barebones outline with lots of bugs and missing parts. 

1) The most important next step is to read in some actual hack machine code, then debug running that. I have a real Hack emulator provided by the authors of the book.
2) Next, I need to emulate the screen and keyboard peripherals.
3) Once everything is working, I might follow the book and write a simple assembler and OS. I'll probably never make it this far having been distracted by some new book or new language or idea!

Making this public on github will hopefully motivate me to work on this more regularly and provide a nice place to keep this all backed up.

-CS