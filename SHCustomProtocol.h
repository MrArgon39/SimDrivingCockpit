#ifndef __SHCUSTOMPROTOCOL_H__
#define __SHCUSTOMPROTOCOL_H__

#include <Arduino.h>
#include <TimerOne.h>
#include <BeanMPX.h>

unsigned long t;
float f = 0; // 10f = 15 speed
float speed = 0;//1.5 speed = 1 frequency

BeanMPX bean;
uint32_t timer = 0;
uint32_t current_millis = 0;
uint8_t powerSteering[] =        {0x62, 0x21, 0x02}; //Power Steering Fault Clear
uint8_t SRS[] =                  {0x62, 0x7A, 0x01}; //SRS clear
uint8_t stabilityControl[] =     {0x62, 0x91, 0x40}; //ABS and TC clear
int sendIndex = 1;

//Custom Protocol Format within SimHub - 

class SHCustomProtocol {
  private:

  public:

    /*
      CUSTOM PROTOCOL CLASS
      SEE https://github.com/zegreatclan/SimHub/wiki/Custom-Arduino-hardware-support

      GENERAL RULES :
      - ALWAYS BACKUP THIS FILE, reinstalling/updating SimHub would overwrite it with the default version.
      - Read data AS FAST AS POSSIBLE in the read function
      - NEVER block the arduino (using delay for instance)
      - Make sure the data read in "read()" function READS ALL THE DATA from the serial port matching the custom protocol definition
      - Idle function is called hundreds of times per second, never use it for slow code, arduino performances would fall
      - If you use library suspending interrupts make sure to use it only in the "read" function when ALL data has been read from the serial port.
        It is the only interrupt safe place

      COMMON FUNCTIONS :
      - FlowSerialReadStringUntil('\n')
        Read the incoming data up to the end (\n) won't be included
      - FlowSerialReadStringUntil(';')
        Read the incoming data up to the separator (;) separator won't be included
      - FlowSerialDebugPrintLn(string)
        Send a debug message to simhub which will display in the log panel and log file (only use it when debugging, it would slow down arduino in run conditions)

    */




    // Called when starting the arduino (setup method in main sketch)
    void setup() {
      pinMode(11, OUTPUT);
      //Timer1.initialize(t);
      bean.ackMsg((const uint8_t[]) {0xFE}, 1);
      bean.begin(46, 47);
    }

    // Called when new data is coming from computer
    void read() {
      // EXAMPLE 1 - read the whole message and sent it back to simhub as debug message
      // Protocol formula can be set in simhub to anything, it will just echo it
      // -------------------------------------------------------
      speed = FlowSerialReadStringUntil('\n').toFloat();
      
      //speedConv();
      if (speed <= 70){
        f = (speed - 2) * 0.6666666;
    }
    else if (speed <=140){
        f = (speed - 1) * 0.6666666;
    }
    else{
        f = speed * 0.6666666;
    }
    
      t=1000000/f;
      Timer1.initialize(t);
      Timer1.pwm(11, 512); // This code will change, will use a 4017 with the default speedo tone.
      /*
        // -------------------------------------------------------
        // EXAMPLE 2 - reads speed and gear from the message
        // Protocol formula must be set in simhub to
        // format([DataCorePlugin.GameData.NewData.SpeedKmh],'0') + ';' + isnull([DataCorePlugin.GameData.NewData.Gear],'N')
        // -------------------------------------------------------

        int speed = FlowSerialReadStringUntil(';').toInt();
        String gear = FlowSerialReadStringUntil('\n');

        FlowSerialDebugPrintLn("Speed : " + String(speed));
        FlowSerialDebugPrintLn("Gear : " + gear);
      */
    }

    // Called once per arduino loop, timing can't be predicted,
    // but it's called between each command sent to the arduino
    void loop() {
      current_millis = millis();

  
      if (current_millis - timer > 50) {
        if (!bean.isBusy()) {
          switch (sendIndex){
           case 1:
            bean.sendMsg(powerSteering, sizeof(powerSteering));
            sendIndex++;
            break;
            case 2:
            bean.sendMsg(SRS, sizeof(SRS));
            sendIndex++;
            break;
            case 3:
            bean.sendMsg(stabilityControl, sizeof(stabilityControl));
            sendIndex = 1;
            break;
          }
      timer = current_millis;
    }    
  }
    }

    // Called once between each byte read on arduino,
    // THIS IS A CRITICAL PATH :
    // AVOID ANY TIME CONSUMING ROUTINES !!!
    // PREFER READ OR LOOP METHOS AS MUCH AS POSSIBLE
    // AVOID ANY INTERRUPTS DISABLE (serial data would be lost!!!)
    void idle() {
    }
};

#endif
