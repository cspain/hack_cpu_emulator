@SCREEN //load initial screen location
D=A  //transfer to D
@0   //load address location (pointer to pointer ha!)
M=D //Store this

(FILL)
@0   // Get current screen address
A=M     
M=M+1 // make M=1
A=A+1 // inc screen
D=A  // put in D
@0  // get screen address store
M=D  //write new screen
@FILL//
0;JMP




        

