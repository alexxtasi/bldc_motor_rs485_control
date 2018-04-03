/*
 * This code is intented to run on an Arduino Uno (called Remote), 
 * using a MAX485 for the RS485 communication link, a Brushless Motor
 * and an ESC. The Remote recieves an rxValue from the RS485 link.
 *  - rxValue == '0' : no move
 *  - rxValue == '1' : motor moves for 2 sec
 *  - rxValue == '-' : direction is set to counter-clockwise
 *  - rxValue == '+' : direction is set to clockwise
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
#include <Servo.h>

/*-----( declare constants and pin numbers )-----*/
const int RS485rx = 10;  // serial receive pin
const int RS485tx = 11;  // serial transmit pin
const int RS485TxControl = 3;   // RS485 transmission control pin

const int RS485Transmit = HIGH;
const int RS485Receive = LOW;
// const int ledPIN = LED_BUILTIN;
const int ledPIN = 13;
const int baudRate = 9600;  // set the connection speed

const int escPIN = 9;  // attach the esc control to pin D9 (PWM)
const int cc_Speed = 70;  // motor's counter-clockwise speed
const int c_Speed = 98;  // motor's clockwise speed
const int zeroSpeed = 90;  // motor's shaft stands still
const int throttle = 90;  // set the throttle - "regulate" the motor

/*-----( declare objects )-----*/
SoftwareSerial RS485(RS485rx, RS485tx); // (RX, TX)
Servo esc;  // create servo object to control the ESC

/*-----( declare variables )-----*/
byte rxValue;
byte txValue;
bool clockwise = true; // default motor's direction set to clockwise
int motion = LOW; // default is no motion

void setup() {
  // debugging
  Serial.begin(9600);
  Serial.println("...debugging on Remote...");

  // set pins for led and RS485
  pinMode(ledPIN, OUTPUT);   
  pinMode(RS485TxControl, OUTPUT);
  
  // set pins for ESC
  pinMode(escPIN, OUTPUT);
  esc.attach(escPIN); // attach ESC control pin
  esc.write(throttle);  // set the throttle - "regulate" the motor
  
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
  // when data are available....
  // recieve motion direction from master
  if (RS485.available()) {
    // led on
    digitalWrite(ledPIN, HIGH); // show activity on led
    
    // read RS485 data
    rxValue = RS485.read();
    
    // debugging
    Serial.print("received : ");
    Serial.println(char(rxValue));
    
    // small delay to allow transmission to complete
    delay(80);
    
    // led off
    digitalWrite(ledPIN, LOW);  // show activity on led  
  }
  else {
    // if no data send ... there is no motion
    rxValue = '0';
  }

  // debugging
  //Serial.print("rxValue : ");
  //Serial.println(char(rxValue));
    
  // decode control-bytes from master
  // set motion and clockwise
  switch (char(rxValue)) {
    case '1': // motion
      motion = HIGH;
      break;
    case '0': // no motion
      motion = LOW;
      break;
    case '-': // counter-clockwise
      clockwise = false;
      break;
    case '+': // clockwise
      clockwise = true;
      break;
    default:
      // default values -- no motion
      motion = LOW;
  }
  
  // move the motor
  if (motion == HIGH && clockwise) {
    esc.write(c_Speed);
    
    // debugging
    Serial.print("motor set to clockwise = ");// debug-log - print speed
    Serial.println(c_Speed);
    
    // keep this speed for 2 second
    delay(2000);
  }
  else if (motion == HIGH && !clockwise) {
    esc.write(cc_Speed);
    
    // debugging
    Serial.print("motor set to counter-clockwise = ");// debug-log - print speed
    Serial.println(cc_Speed);
    
    // keep this speed for 2 second
    delay(2000);
  }
  else {
    esc.write(zeroSpeed);
    
    // debugging
    Serial.print("motor set to zeroSpeed = ");  // debug-log - print speed
    Serial.println(zeroSpeed);
  }
}
