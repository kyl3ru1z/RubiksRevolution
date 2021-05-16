# RubiksRevolution
Rubik's Revolution clone made using the Arduino UNO. The main game loop for Rubik's Revolution consisted of searching for the lit LED on the cube and then pushing the button on the corresponding face of the cube. After a button has been pushed the corresponding LED will turn off and find a random LED to turn on. Each button push will shorten the time needed to find the next lit LED. The game ends when the user can't find the LED or can't react fast enough to push the button. Also implemented Arduino tones to add sound effects like clock ticking and buzzers.

# Parts List
- 6 Buttons (16mm Illuminated Momentary Pushbuttons)
- On/Off Pushbuttun toggle switch
- 6 ten ohm resistors 
- 6 two hundred and twenty ohm resistor
- Speaker (3" Diameter - 8 Ohm 1 Watt)
- Solderable Bread Board
- Arduino UNO
- Jumper Wires

# What I Learned 
- Implementing timers
- Creating Arduino Pitches 
- Soldering Wires
- Handling and debouncing multiple buttons
