/*
 * This code is intented to run on an Arduino Mega (called Master), 
 * using two buttons and a MAX485, for the RS485 communication link.
 *  - press the motion button : the Remote Adruino will spin the motor 
 *    for "2" seconds
 *  - press the direction button : next time the motion is pressed, 
 *    the motor will move to the opposite direction
 * 
 * Copyright (C) 2018  Alex Tasikas <alextasikas@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*-----( import libraries )-----*/
#include <SoftwareSerial.h>

/*-----( declare constants and pin numbers )-----*/
const int RS485rx = 10;  // serial receive pin
const int RS485tx = 11;  // serial transmit pin
const int RS485TxControl = 4;   // RS485 transmission control pin

const int RS485Transmit = HIGH;
const int RS485Receive = LOW;
// const int ledPIN = LED_BUILTIN;
const int ledPIN = 13;
const int baudRate = 9600;  // set the connection speed

const int motionPIN = 2;  // attach the motion button input to pin D2
const int directionPIN = 3;  // attach the direction button input to pin D3

/*-----( declare objects )-----*/
SoftwareSerial RS485(RS485rx, RS485tx); // (RX, TX)

///*-----( declare variables )-----*/

// ****** test *******
int dState = 0;
int ldState = 0;
int mState = 0;
int lmState = 0;
// ****** test *******

void setup() {
  // debugging
  Serial.begin(9600);
  Serial.println("...debugging on Master...");

  // set pins for led and RS485
  pinMode(ledPIN, OUTPUT);   
  pinMode(RS485TxControl, OUTPUT);

  // set pins to control the Remote's ESC
  pinMode(motionPIN, INPUT);
  pinMode(directionPIN, INPUT);

  // ensure the on-board LED is off
  digitalWrite(ledPIN, LOW); // ??????? high or LOW ??????????

  // set RS485 device to read initially
  digitalWrite(RS485TxControl, RS485Receive);
  
  // set the baud rate.
  // the longer the wire, the slower the transmission rate should be
  // 300, 600, 1200, 2400, 4800, 9600, 14400,
  // 19200, 28800, 31250, 38400, 57600, 115200
  // MUST BE THE SAME ON ALL UNITS !!
  RS485.begin(baudRate);
}

void loop() {
  // directionPIN Edge Detection
    dState = digitalRead(directionPIN);
  // compare the direction State to its previous state
  if (dState != ldState) {
    // when pressed but no released
    if (dState == HIGH) {
      // send via RS485
      sendRS485('-');
      delay(1);  // delay a little bit to avoid bouncing ?????????????????? 
    }
    // when released
    else {
      // send via RS485
      sendRS485('+');
      delay(1);  // delay a little bit to avoid bouncing ?????????????????? 
    }
  }
  // save the current state as last, for next loop
  ldState = dState;
  
  // motionPIN Edge Detection
    mState = digitalRead(motionPIN);
  // compare the motion State to its previous state
  if (mState != lmState) {
    // when pressed but no released
    if (mState == HIGH) {
      // send via RS485
      sendRS485('1');
      delay(1);  // delay a little bit to avoid bouncing ?????????????????? 
    }
  }
  // save the current state as last, for next loop
  lmState = mState;
}

void sendRS485(char msg) {
  // led on
  digitalWrite(ledPIN, HIGH); // show activity on led
  
  // debugging
  Serial.print("sending direction : ");
  Serial.println(msg);
  
  // set the RS485 to transmit mode and send the value
  digitalWrite(RS485TxControl, RS485Transmit);
  RS485.write(msg);
  
  // small delay to allow transmission to complete
  delay(80);
  
  // led off
  digitalWrite(ledPIN, LOW);  // show activity on led
  
  // set RS485 device to read again
  digitalWrite(RS485TxControl, RS485Receive);
}
