#include "Adafruit_TinyUSB.h"

//Code for the pedalset, currently using the onboard ADC of the Pico, with further development may be moved to an external component.
//Currently uses potentiometers, option to use SS49E linear hall effect sensors with more development.
//Will aslo be pdated to use the ADS1115, same as the shifter for more reliability.

enum
{
  RID_GAMEPAD = 3,
};

uint8_t const desc_hid_report[] =
{
  TUD_HID_REPORT_DESC_GAMEPAD( HID_REPORT_ID(RID_GAMEPAD) )
};

Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);

hid_gamepad_report_t    gp;

int Clutch;
int Brake;
int Accel;

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  // Clutch Potentiometer on pin 26
  pinMode(A0, INPUT);
  // Brake Potentiometer on pin 27
  pinMode(A1, INPUT);
  // Accelerator Potentiometer on pin 27
  pinMode(A2, INPUT);
  //Power supply toggle pin
  pinMode(23, INPUT_PULLUP);
  digitalWrite(23, HIGH);


  usb_hid.setStringDescriptor("PicoPedal");
  USBDevice.setID( 0x2e8a, 0x000c ); // Sets the Product ID to be different to the default, so that windows respects the name given, and not the name of the first device plugged in.

  usb_hid.begin();

  // wait until device mounted
  while ( !TinyUSBDevice.mounted() ) delay(1);

  Serial.println("PicoPedal");

  analogReadResolution(10);
}

void loop() {
  if ( !usb_hid.ready() ) return;
  //Read in the axes.
  Clutch = analogRead(26);
  Brake = analogRead(27);
  Accel = analogRead(28);

  //Map the axes to the joystick input range.
  Clutch = map(Clutch,0,1023,-127,128);
  Brake = map(Brake,0,1023,-127,128);
  Accel = map(Accel,0,1023,-127,128);

  Serial.println(Brake);
  //Set the gamepad axes to the mapped values.
  gp.rx = Clutch;
  gp.ry = Brake;
  gp.rz = Accel;


  //Send to report to the host computer.
  usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(100); //Polling rate will be higher in a finalised design.
}
