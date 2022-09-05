#include "Adafruit_TinyUSB.h"

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
  // put your setup code here, to run once:
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
  pinMode(LED_BUILTIN, OUTPUT);

  usb_hid.setStringDescriptor("PicoWheel");
  usb_hid.begin();
  Serial.begin(115200);

  while (!TinyUSBDevice.mounted())
    delay(1);

  Serial.println("Adafruit TinyUSB HID Composite example");
  gp.x = 0;
  gp.y = 0;
  gp.z = 0;
  gp.rz = 0;
  gp.rx = 0;
  gp.ry = 0;
  gp.hat = 0;
  gp.buttons = 0;
  usb_hid.sendReport(0, &gp, sizeof(gp));
}

void loop()
{
  // put your main code here, to run repeatedly:
  delay(10);

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

    if (mVolDOWN_pressed)
    {
      // send volume down (0x00EA)
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_DECREMENT);
      Serial.println("Volume Down Pressed");
      delay(100);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
    }
    else if (mVolUP_pressed)
    {
      // send volume down (0x00E9)
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_INCREMENT);
      Serial.println("Volume Up Pressed");
      delay(100);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
    }
    else if (mSkipFWD_pressed)
    {
      // send volume down (0x00B5)
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_SCAN_NEXT);
      Serial.println("Skip Forward Pressed");
      delay(100);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
    }
    else if (mSkipBACK_pressed)
    {
      // send volume down (0x00B6)
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_SCAN_PREVIOUS);
      Serial.println("Skip back Pressed");
      delay(100);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
    }
    else if (mPausePlay_pressed)
    {
      // send volume down (0x00CD)
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_PLAY_PAUSE);
      Serial.println("Play Pause Pressed");
      delay(300);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
    }
    if (dUP_pressed)
    {
      // Send D-Pad up
      gp.hat = 1;
      usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      Serial.println("D-Pad Up Pressed");
      delay(300);
      gp.hat = 0;
      usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    else if (dDOWN_pressed)
    {
      // Send D-Pad down
      gp.hat = 5;
      usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      Serial.println("D-Pad Down Pressed");
      delay(300);
      gp.hat = 0;
      usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    else if (dLEFT_pressed)
    {
      // Send D-Pad Left
      gp.hat = 7;
      usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      Serial.println("D-Pad Left Pressed");
      delay(300);
      gp.hat = 0;
      usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    else if (dRIGHT_pressed)
    {
      // Send D-Pad Right
      gp.hat = 3;
      usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      Serial.println("D-Pad Right Pressed");
      delay(300);
      gp.hat = 0;
      usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    if (dOK_pressed)
    {
      // Send Button 1
      gp.buttons = 1;
      usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
      Serial.println("D-Pad OK Pressed");
      delay(300);
      gp.buttons = 0;
      usb_hid.sendReport(RID_GAMEPAD, &gp, sizeof(gp));
    }
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(100);
  }
}
