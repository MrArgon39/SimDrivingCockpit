#include "Adafruit_TinyUSB.h"

//This is for the physical steering wheel, to handle buttons that can be interacted with. Will be changed to a matrix for efficiency of
//inputs, and to allow for even more buttons.

//Button names are respective to their usual use, and what they are marked as on the refernece photo.
// d = D-Pad, m = Media, c = Cruise, s = shift; o = Other assorted buttons
byte dUP = 2;
byte dDOWN = 3;
byte dLEFT = 4;
byte dRIGHT = 5;
byte dOK = 6;
byte mVolUP = 7;
byte mVolDOWN = 8;
byte mSkipFWD = 9;
byte mSkipBACK = 10;
byte mPausePlay = 11;
byte cSet = 12;
byte cMinus = 13;
byte cPlus = 14;
byte cRes = 15;
byte cCan = 16; // Only used as cancel in BeamNG
byte sUp = 17;
byte sDown = 18;
byte oPreset = 19;
int pressedButtons = 0; //Stores the values of the buttons that are currently depressed.

enum
{
  RID_GAMEPAD = 1,
  RID_CONSUMER_CONTROL, // Media, volume etc ..
};

uint8_t const desc_hid_report[] =
    {
        TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(RID_GAMEPAD)),
        TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(RID_CONSUMER_CONTROL))};

Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);

hid_gamepad_report_t gp;

void setup()
{
  // Many inputs, many inputs
  pinMode(dUP, INPUT_PULLUP);
  pinMode(dDOWN, INPUT_PULLUP);
  pinMode(dLEFT, INPUT_PULLUP);
  pinMode(dRIGHT, INPUT_PULLUP);
  pinMode(dOK, INPUT_PULLUP);
  pinMode(mVolUP, INPUT_PULLUP);
  pinMode(mVolDOWN, INPUT_PULLUP);
  pinMode(mSkipFWD, INPUT_PULLUP);
  pinMode(mSkipBACK, INPUT_PULLUP);
  pinMode(mPausePlay, INPUT_PULLUP);
  pinMode(cSet, INPUT_PULLUP);
  pinMode(cMinus, INPUT_PULLUP);
  pinMode(cPlus, INPUT_PULLUP);
  pinMode(cRes, INPUT_PULLUP);
  pinMode(cCan, INPUT_PULLUP);
  pinMode(sUp, INPUT_PULLUP);
  pinMode(sDown, INPUT_PULLUP);
  pinMode(oPreset, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);

  usb_hid.setStringDescriptor("PicoWheel");//Tell windows that the device has a different name
  usb_hid.begin();
  Serial.begin(115200);

  while (!TinyUSBDevice.mounted())
    delay(1);

  Serial.println("PicoWheel");
  gp.x = 0;
  gp.y = 0;
  gp.z = 0;
  gp.rz = 0;
  gp.rx = 0;
  gp.ry = 0;
  gp.hat = 0;
  gp.buttons = 0;
  usb_hid.sendReport(0, &gp, sizeof(gp)); //Send a blank report to ensure everything is at zero.
}

void loop()
{
  // put your main code here, to run repeatedly:
  delay(10);
  pressedButtons = 0; //Chack if any button was pressed, then excute the related action.
  bool mVolDOWN_pressed = (digitalRead(mVolDOWN) == false);
  bool mVolUP_pressed = (digitalRead(mVolUP) == false);
  bool mSkipFWD_pressed = (digitalRead(mSkipFWD) == false);
  bool mSkipBACK_pressed = (digitalRead(mSkipBACK) == false);
  bool mPausePlay_pressed = (digitalRead(mPausePlay) == false);
  bool dUP_pressed = (digitalRead(dUP) == false);
  bool dDOWN_pressed = (digitalRead(dDOWN) == false);
  bool dLEFT_pressed = (digitalRead(dLEFT) == false);
  bool dRIGHT_pressed = (digitalRead(dRIGHT) == false);
  bool dOK_pressed = (digitalRead(dOK) == false);
  bool cSet_pressed = (digitalRead(cSet) == false);
  bool cMinus_pressed = (digitalRead(cMinus) == false);
  bool cPlus_pressed = (digitalRead(cPlus) == false);
  bool cRes_pressed = (digitalRead(cRes) == false);
  bool cCan_pressed = (digitalRead(cCan) == false);
  bool sUp_pressed = (digitalRead(sUp) == false);
  bool sDown_pressed = (digitalRead(sDown) == false);
  bool oPreset_pressed = (digitalRead(oPreset) == false);

  gp.x = 0;
  gp.y = 0;
  gp.z = 0;
  gp.rz = 0;
  gp.rx = 0;
  gp.ry = 0;
  gp.hat = 0;
  gp.buttons = 0;
  if (usb_hid.ready())
  {
    Serial.println("IS YOUR FRIDGE RUNNING?");
    // Consumer Control is used to control Media playback, Volume, Brightness etc ...
    // Consumer report is 2-byte containing the control code of the key
    // For list of control check out https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h

    if (mVolDOWN_pressed)//Media controls on back of wheel/Extra buttons if a switch is pressed on boot
    {
      // send volume down (0x00EA)
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_DECREMENT);
      //Serial.println("Volume Down Pressed");
      delay(100);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
    }
    else if (mVolUP_pressed)
    {
      // send volume down (0x00E9)
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_INCREMENT);
      //Serial.println("Volume Up Pressed");
      delay(100);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
    }
    else if (mSkipFWD_pressed)
    {
      // send volume down (0x00B5)
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_SCAN_NEXT);
      //Serial.println("Skip Forward Pressed");
      delay(100);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
    }
    else if (mSkipBACK_pressed)
    {
      // send volume down (0x00B6)
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_SCAN_PREVIOUS);
      //Serial.println("Skip back Pressed");
      delay(100);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
    }
    else if (mPausePlay_pressed)
    {
      // send volume down (0x00CD)
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_PLAY_PAUSE);
      //Serial.println("Play Pause Pressed");
      delay(300);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
    }
    if (dUP_pressed)//Left Side Controls
    {
      // Send D-Pad up
      gp.hat = 1;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      Serial.println("D-Pad Up Pressed");
      //delay(300);
      //gp.hat = 0;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    else if (dDOWN_pressed)
    {
      // Send D-Pad down
      gp.hat = 5;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      Serial.println("D-Pad Down Pressed");
      //delay(300);
      //gp.hat = 0;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    else if (dLEFT_pressed)
    {
      // Send D-Pad Left
      gp.hat = 7;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      Serial.println("D-Pad Left Pressed");
      //delay(300);
      //gp.hat = 0;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    else if (dRIGHT_pressed)
    {
      // Send D-Pad Right
      gp.hat = 3;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      Serial.println("D-Pad Right Pressed");
      //delay(300);
      //gp.hat = 0;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    else{
      gp.hat = 0;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      //delay(100);
    }
    if (dOK_pressed)
    {
      // Send Button 1
      //gp.buttons = 1;
      pressedButtons= pressedButtons + 1;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      //Serial.println("D-Pad OK Pressed");
      //delay(300);
      //gp.buttons = 0;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    if (cSet_pressed)//Right Side Pad
    {
      // Send Button 2
      //gp.buttons = 2;
      pressedButtons= pressedButtons + 2;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      //Serial.println("Cruise Set Pressed");
      //delay(300);
      //gp.buttons = 0;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    if (cMinus_pressed)
    {
      // Send Button 3
      //gp.buttons = 4;
      pressedButtons= pressedButtons + 4;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      //Serial.println("Cruise Set Pressed");
      //delay(300);
      //gp.buttons = 0;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    if (cPlus_pressed)
    {
      // Send Button 4
      //gp.buttons = 8;
      pressedButtons= pressedButtons + 8;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      //Serial.println("Cruise Set Pressed");
      //delay(300);
      //gp.buttons = 0;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    if (cRes_pressed)
    {
      // Send Button 5
      //gp.buttons = 16;
      pressedButtons= pressedButtons + 16;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      //Serial.println("Cruise Set Pressed");
      //delay(300);
      //gp.buttons = 0;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    if (cCan_pressed)
    {
      // Send Button 6
      //gp.buttons = 32;
      pressedButtons = pressedButtons + 32;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      //Serial.println("Cruise Set Pressed");
      //delay(300);
      //gp.buttons = 0;
      //usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    if (sUp_pressed)
    {
      // Send Button 7
      pressedButtons = pressedButtons + 64;
      Serial.println("Shift Up Pressed");
    }
    if (sDown_pressed)
    {
      // Send Button 8
      pressedButtons = pressedButtons + 128;
      Serial.println("Shift Down Pressed");
    }
    if (oPreset_pressed)
    {
      // Send Button 9?
      //pressedButtons = pressedButtons + 256;
      Serial.println("Preset Pressed");
    }
    gp.buttons = pressedButtons;
    usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    //delay(100);
  }
}
