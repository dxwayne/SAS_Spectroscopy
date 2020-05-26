/*
 * This sketch was written by Thomas C. Smith on Janyary 11, 2017
 * The purpose for this sketch is to control relays and therefore the
 *    switches on the LHiRes spectrograph in the modes of "Flat" and "Calibrate".
 * 
 * The pinout for this configuration is as follows:
 *    Arduino PIN 6 = Relay #1 trigger or TTL input
 *    Arduino PIN 7 = Relay #2 trigger or TTL input
 *    Arduino PIN 8 = LED #1 (optional visual remote Relay #1 condition)
 *    Arduino PIN 9 = LED #2 (optional visual remote Relay #2 condition)
 *    Arduino PIN X = GND
 *    Arduino PIN Y = +5 VDC
 *    
 * On the relay board the pinout is as follows:
 *    VCC = +5 VDC from Arduino PIN X
 *    GND = GND from Arduino PIN Y
 *    IN1 = Trigger for Relay #1 from Arduino PIN 6
 *    IN2 = Trigger for Relay #2 from Arduino PIN 7
 *    
 * Optional LEDs for remote viaual indication of the state of the relays:
 *    LED #1 with series 220 Ohm or greater resistor from Arduino PIN 8
 *    LED #2 with series 220 Ohm or greater resistor from Arduino PIN 9
 *    
 * We want to control the Ardion using a USB interface to a computer so
 * we will utilie the Serial component in the sketch and 9600 baud is
 * sufficent for communication speed. An external program can be written
 * to send and recieve serial commands to and from the Arduino.
 * 
 * ASCII values used to control the relays and/or LEDs
 *    "c" = (ON) calibration relay ONLY
 *    "C" = (ON) calibration relay with associated LED functional
 *    "d" or "D" = (OFF) calibration relay and LED 
 *    "f" = (ON) flat relay ONLY
 *    "f" = (ON) flat relay ONLY
 *    "F" = (ON) flat relay with associated LED functional
 *    "g" or "G" = (OFF) flat relay and LED
 *    "b" = (ON) both calibration and flat relays ONLY
 *    "B" = (ON) both calibration and flat relays with associated LEDs functional
 *    "a" or "A" = (OFF) both relays and LEDs
 *    "O" = everything OFF
 */

// Define constants for Arduino pins
const int relay1 = 6;
const int relay2 = 7;
const int led1 = 8;
const int led2 = 9;

void setup() {
  // code place here will run once on power up or reset:
  // configure pin modes on the Arduino
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  // set up for serial communications using the USB interface
  Serial.begin(9600);
  while(!Serial){
    // just wait for the USB connection to be made
  }
  Serial.println("Serial Communications port is OPEN...");
}

void loop() {
  // code placed here will run run repeatedly:
  char inChar = ""; // defining and populating the initial value of the variable to hold serial data
  if (Serial.available() > 0) {
    // get incoming byte:
    inChar = Serial.read();
    Serial.println("Data received is: " + inChar);
    switch(inChar){
      case 'c':   // cal relay only
        relayOnOff(1,0);
        Serial.println("Sent Relay 1 an ON signal");
        break;
      case 'C':   // cal relay and 
        relayOnOff(1,1);
        ledOnOff(1,1);
        Serial.println("Sent Relay1 an ON signal");
        break;
      case 'd':  // turn off the relay and LED
        relayOnOff(1,0);
        ledOnOff(1,0);
        Serial.println("Sent Relay1 an OFF signal");
        break;
      case 'D':
        relayOnOff(1,0);
        ledOnOff(1,0);
        Serial.println("Sent Relay1 an OFF signal");
        break;
      case 'f':   // flat relay only
        relayOnOff(2,1);
        Serial.println("Sent Relay2 an ON signal");
        break;
      case 'F':   // flat relay and led
        relayOnOff(2,1);
        ledOnOff(2,1);
        Serial.println("Sent Relay2 and LED2 an ON signal");
        break;
      case 'g':  // turn off the relay and LED
        relayOnOff(2,0);
        ledOnOff(2,0);
        Serial.println("Sent Relay2 and LED2 an OFF signal");
        break;
      case 'G':
        relayOnOff(2,0);
        ledOnOff(2,0);
        Serial.println("Sent Relay2 and LED2 an OFF signal");
        break;
      case 'b':   // both relays only
        relayOnOff(1,1);
        relayOnOff(2,1);
        Serial.println("Sent Relay1 and Relay2 an ON signal");
        break;
      case 'B':   // both relays and leds
        relayOnOff(1,1);
        relayOnOff(2,1);
        ledOnOff(1,1);
        ledOnOff(2,1);
        Serial.println("Sent Relay1, Relay2, LED1 and LED2 an OFF signal" );
        break;
      case 'a':  // turn off both relays and LEDs
        relayOnOff(1,0);
        relayOnOff(2,0);
        ledOnOff(1,0);
        ledOnOff(2,0);
        Serial.println("Sent Relay1, Relay2, LED1 and LED2 an OFF signal");
        break;
      case 'A':
        relayOnOff(1,0);
        relayOnOff(2,0);
        ledOnOff(1,0);
        ledOnOff(2,0);
        Serial.println("Sent Relay1, Relay2, LED1 and LED2 an OFF signal");
        break;
      case 'o':    // everything OFF not toggled
        relayOnOff(1,0);
        relayOnOff(2,0);
        ledOnOff(1,0);
        ledOnOff(2,0);
        Serial.println("Sent Relay1, Relay2, LED2 and LED2 an OFF signal");
        break;
      case 'O':
        relayOnOff(1,0);
        relayOnOff(2,0);
        ledOnOff(1,0);
        ledOnOff(2,0);
        Serial.println("Sent Relay1, Relay2, LED1 and LED2 an OFF signal");
        break;
      default:
        // do nothing and just loop back
        break;
    }

  } 

}

// proceedure calls to handle the various serial data conditions
void relayOnOff(int relayID, int relayState){
  // the relayID is the numerical value of the relay to be operated on
  // the relayState is an interger value indicating either ON (1) or OFF (0)
  switch(relayID){
    case 1: // use relay 1 as target
      if(relayState == 1){
        // turn the relay ON
        digitalWrite(relay1, HIGH);
        break;
      }
      else {
        // turn the relay OFF
        digitalWrite(relay1, LOW);
        break;
      }
      break;
    case 2: // use relay 2 as target
      if(relayState == 1){
        // turn the relay ON
        digitalWrite(relay2, HIGH);
        break;
      }
      else {
        // turn the relay OFF
        digitalWrite(relay2, LOW);
        break;
      }
      break;
  }
  
}

void ledOnOff(int ledID, int ledState){
  // the ledID is the numberical value of the relay to be operated on
  // the ledState is an interger value indicating either ON (1) or OFF (0)
  switch(ledID){
    case 1:
      // use LED1 as the target
      if(ledState == 1){
        // turn the led ON
        digitalWrite(led1, HIGH);
        break;
      }
      else {
        // turn the led OFF
        digitalWrite(led1, LOW);
        break;
      }
      break;
    case 2:
      // use LED2 as the target
      if(ledState == 1){
        // turn the led ON
        digitalWrite(led2, HIGH);
        break;
      }
      else {
        // turn the led OFF
        digitalWrite(led2, LOW);
        break; 
      }
      break;
  }
}

