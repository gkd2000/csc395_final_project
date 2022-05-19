Benjamin, Skye, and Grace

We started with Paul and Grace's kernel code. 

Our project adds graphics and mouse functionality, which are then used to run a paint user program. The first thing
that will display when you run the project is a screen with the user programs that are equipped to run in graphics 
mode. They are paint and helloword. You will see a number associated with each. Press a number on the keyboard to 
run the associated user program. Once a user program is running, press 'q' at anytime to return to the menu screen
where user programs are displayed. 

The user program helloworld should print "Hello world!" to the screen. It demonstrates the functionality of the gwrite
system call, which is the graphics equivalent of write. 

The user program paint is the main part of the project. In the paint program, click and drag the mouse to draw in the 
selected color. Click on any of the colors at the top of the screen to change the brush color. The rectangle at the 
top left of the screen displays the current brush color, as well as its associated hexadecimal code. The clear button
at the top right erases anything that the user has drawn on the screen, bringing the canvas back to a fresh state. 
