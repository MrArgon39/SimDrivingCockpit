#include <PicoGamepad.h>

PicoGamepad gamepad;

// 16 bit integer for holding input values
int X;
int Y;

void setup() {  
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);

  // X Potentiometer on pin 26
  pinMode(A0, INPUT);
  // Y Potentiometer on pin 27
  pinMode(A1, INPUT);
}

void loop() {

  // Get input value from Pico analog pin
  X = analogRead(26);

  // Repeat with Y pin
  Y = analogRead(27);

  if (X < 100 && Y < 100){
    gamepad.SetButton(1, 1);//1st
    Serial.println("1");
  }
  else if (X < 100 && Y > 900){
    gamepad.SetButton(2, 1);//2nd
    Serial.println("2");
  }
  else if (X > 400 && X < 600 && Y < 100){
    gamepad.SetButton(3, 1);//3rd
    Serial.println("3");
  }
  else if (X > 400 && X < 600 && Y > 900){
    gamepad.SetButton(4, 1);//3rd
    Serial.println("4");
  }
  else if (X > 900 && Y < 100){
    gamepad.SetButton(5, 1);//3rd
    Serial.println("5");
  }
  else if (X > 900 && Y > 900){
    gamepad.SetButton(0, 1);//R
    Serial.println("R");
  }
  else{
    gamepad.SetButton(0, 0);
    gamepad.SetButton(1, 0);
    gamepad.SetButton(2, 0);
    gamepad.SetButton(3, 0);
    gamepad.SetButton(4, 0);
    gamepad.SetButton(5, 0);
    Serial.println("N");
  }
//  gamepad.SetZ(val);
//  gamepad.SetRx(val);
//  gamepad.SetRy(val);
//  gamepad.SetRz(val);
//  gamepad.SetS0(val);
//  gamepad.SetS1(val);

  // Set button 0 of 128 by reading button on digital pin 28
  //gamepad.SetButton(0, !digitalRead(28));

  // Set hat direction, 4 hats available. direction is clockwise 0=N 1=NE 2=E 3=SE 4=S 5=SW 6=W 7=NW 8=CENTER 
  // gamepad.SetHat(0, 8);


  // Send all inputs via HID 
  // Nothing is send to your computer until this is called.
  gamepad.send_update();

  // Flash the LED just for fun
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); 
  delay(100);
}
