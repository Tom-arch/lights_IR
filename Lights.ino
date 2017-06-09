/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */


#include <IRremote.h>
#define IN_A 12 //yellow cable
#define IN_B 13 //green cable
#define PC_IN 2 //connected to 5v molex

long colour = 1;
int pc_on = 0;
int lights_on = 0;
int off_after = 0;

IRsend irsend;

/*
 * INPUT MAP : A-B
 * 0-0 -> IDLE: DON'T DO ANYTHING
 * 1-0 -> change color
 * 1-1 -> turn on
 * 0-1 -> turn off
 * The command pulse should last 60 ms to be sure that the Arduino can read it (read every 40ms).
 * After the execution of every command there is a delay to make sure it is not executed twice and the Odroid has returned to 0-1.
*/

void setup() {
  //Serial//Serial.begin(9600);
  randomSeed(analogRead(A0));
  pinMode(IN_A, INPUT);
  pinMode(IN_B, INPUT);
  colour = random(1, 16);
  //Serial//Serial.println(colour);
  delay(120000); //Initial delay to allow the odroid to connect to the internet and sync the time
}

void loop() {
  
  //Read the input from the computer
  int DATA_PC = digitalRead(PC_IN);
  
  //If the pc was off and becomes on
  if (pc_on == 0 && DATA_PC == 1) {
    //Serial.println("pc becomes on");
    colour = 11;
    //If the lights were off, turn them on, else change colour to green
    if (lights_on == 0) {
      startup_sequence();
      off_after = 1; //Were off, will become off again
    } else {
      change_colour();
    }
    pc_on = 1;
  } else if (pc_on == 1 && DATA_PC == 0) { //If the pc was on and becomes off
  ////Serial.println("pc becomes off");
    pc_on = 0;
    //If the shutdown is scheduled, turn off. Else change colour
    if (off_after == 1) {
     shutdown_sequence();
     off_after = 0;
    } else {
     change_colour(); 
    }
  }
  
  //Read the input from the odroid and sort out the command
  int DATA_A = digitalRead(IN_A);
  int DATA_B = digitalRead(IN_B);
  //Parse the commands
  if (DATA_A==0 && DATA_B==0) {//Idle
    delay(40);
  } else if (DATA_A==1 && DATA_B==0) {//Change colour
    change_colour();
  } else if (DATA_A==1 && DATA_B==1) {//Turn on
    startup_sequence();
  } else if (DATA_A==0 && DATA_B==1) {//Turn off
    shutdown_sequence();
  }
}

//This function is the startup and brightness adjustement sequence
void startup_sequence() {
  //Actually turn on only if the lights are off
  //Serial.println("On called");
  if (pc_on == 0) {
    //Serial//Serial.println("Done");
    irsend.sendNEC(0x02FD, 32);
    delay(500);
    int i = 0;
    for (i=0; i<8; i++) {
      irsend.sendNEC(0xBA45, 32);
      delay(100);
    }
    lights_on = 1;
    delay(200);
  }
  off_after = 0;
  change_colour(); //Change the colour for the first time
  delay(100); //after delay
}

//This function is the shutdown sequence
void shutdown_sequence() {
  //Serial//Serial.println("Off called");
  //Actually turn off only if the pc is off and the lights are on
  if (pc_on == 0) {
    //Serial//Serial.println("Done");
    irsend.sendNEC(0x22DD, 32);
    delay(500);
    irsend.sendNEC(0x02FD, 32);
    lights_on = 0;
  } else if (pc_on == 1) {
    //Schedule the turn off
    //Serial//Serial.println("Scheduled");
    off_after = 1;
  }
  delay(100);//After delay
}

//This function changes the colour
void change_colour() {
  //Serial//Serial.println("Ch called");
  //Serial//Serial.println(colour);
  //Actually change the colour only if the pc is off
  if (pc_on == 0) {
  //Serial//Serial.println("Done");
  switch (colour) {
    case 1:
      irsend.sendNEC(0x1AE5, 32);
      break;
    case 2:
      irsend.sendNEC(0x2AD5, 32);
      break;
    case 3:
      irsend.sendNEC(0x0AF5, 32);
      break;
    case 4:
      irsend.sendNEC(0x38C7, 32);
      break;
    case 5:
      irsend.sendNEC(0x18E7, 32);
      break;
    case 6:
      irsend.sendNEC(0x9A65, 32);
      break;
    case 7:
      irsend.sendNEC(0xAA55, 32);
      break;
    case 8:
      irsend.sendNEC(0x8A75, 32);
      break;
    case 9:
      irsend.sendNEC(0xB847, 32);
      break;
    case 10:
      irsend.sendNEC(0x9867, 32);
      break;
    case 11:
      irsend.sendNEC(0xA25D, 32);
      break;
    case 12:
      irsend.sendNEC(0x926D, 32);
      break;
    case 13:
      irsend.sendNEC(0xB24D, 32);
      break;
    case 14:
      irsend.sendNEC(0x7887, 32);
      break;
    case 15:
      irsend.sendNEC(0x58A7, 32);
      break;
  }
  }
  //Scale the colour numbers anyways
  if (colour < 15) {
    colour += 1;
  } else {
    colour = 1;
  }
  delay(500); //After delay
}
