#include "Adafruit_TinyUSB.h"

//Gearshift, will contain a two axis gimbal with either hall effect sensors or potentiometers to detects where the stick is, and
//two switches for when you are driving a truck.

//Will be modified to use an ADS1115 I2C ADC, as the internal ADC of the Pico is not the best.
enum
{
  RID_GAMEPAD = 2,
};

//Generic Gamepad descriptor, may be modified with further development.
uint8_t const desc_hid_report[] =
    {
        TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(RID_GAMEPAD))};

Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);

hid_gamepad_report_t gp;

int X;
int Y;

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  // X Potentiometer on pin 26
  pinMode(A0, INPUT);
  // Y Potentiometer on pin 27
  pinMode(A1, INPUT);
  // Faceplate Detect on pin 28
  pinMode(A2, INPUT);
  // Range Switch
  pinMode(20, INPUT_PULLUP);
  // Splitter Switch
  pinMode(21, INPUT_PULLUP);
  //Power supply toggle pin
  pinMode(23, INPUT_PULLUP);
  digitalWrite(23, HIGH);

  usb_hid.setStringDescriptor("PicoShift");
  USBDevice.setID(0x2e8a, 0x000b); // Sets the Product ID to be different to the default, so that windows respects the name given, and not the name of the first device plugged in.

  usb_hid.begin();

  // wait until device mounted
  while (!TinyUSBDevice.mounted())
    delay(1);

  Serial.println("PicoShift");
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (!usb_hid.ready())
    return;
  // Read X pin
  X = analogRead(26);

  // Repeat with Y pin
  Y = analogRead(27);
  
  if (analogRead(A2) > 260 && analogRead(A2) < 320) // For the 5+1 Speed faceplate, 1k + 330 voltage divider
  {

    if (X < 100 && Y < 100)
    {
      gp.buttons = 2; // 1st
      Serial.println("1");
    }
    else if (X < 100 && Y > 900)
    {
      gp.buttons = 4; // 2nd
      Serial.println("2");
    }
    else if (X > 400 && X < 600 && Y < 100)
    {
      gp.buttons = 8; // 3rd
      Serial.println("3");
    }
    else if (X > 400 && X < 600 && Y > 900)
    {
      gp.buttons = 16; // 3rd
      Serial.println("4");
    }
    else if (X > 900 && Y < 100)
    {
      gp.buttons = 32; // 3rd
      Serial.println("5");
    } /* Not yet implemented, will need the actual shifter assembly to get it correct
     else if (X > 900 && Y < 100) {
       gp.buttons = 64;//3rd
       Serial.println("6");
     }
     else if (X > 900 && Y < 100) {
       gp.buttons = 128;//3rd
       Serial.println("7");
     }
     else if (X > 900 && Y < 100) {
       gp.buttons = 256;//3rd
       Serial.println("8");
     }*/
    else if (X > 900 && Y > 900)
    {
      gp.buttons = 1; // R
      Serial.println("R");
    }
    else
    {
      gp.buttons = 0;
      Serial.println("N");
    }
  }
  if (analogRead(A2) > 80 && analogRead(A2) < 120)//For another faceplate, but needs the ADS working before it can be implemented properly.
  {
  }
  if (digitalRead(20) == LOW)
  { // Done so that the range/split switches are permenant, rather than resetting each cycle
    gp.buttons = gp.buttons + 512;
  }
  if (digitalRead(21) == LOW)
  {
    gp.buttons = gp.buttons + 1024;
  }
  Serial.println(gp.buttons);

  usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(100);
}
