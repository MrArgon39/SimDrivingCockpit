#ifndef __SHCUSTOMPROTOCOL_H__
#define __SHCUSTOMPROTOCOL_H__

#include <Arduino.h>
#include <TimerOne.h>

unsigned long t;
float f = 0; // 10f = 15 speed
float speed = 0;//1.5 speed = 1 frequency

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
      Timer1.initialize(t);
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
      Timer1.setPeriod(t);
      Timer1.pwm(11, 512);

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
    }

    // Called once between each byte read on arduino,
    // THIS IS A CRITICAL PATH :
    // AVOID ANY TIME CONSUMING ROUTINES !!!
    // PREFER READ OR LOOP METHOS AS MUCH AS POSSIBLE
    // AVOID ANY INTERRUPTS DISABLE (serial data would be lost!!!)
    void idle() {
    }
    void speedConv(){
    if (speed <= 70){
        f = (speed - 2) * 0.6666666;
    }
    else if (speed <=140){
        f = (speed - 1) * 0.6666666;
    }
    else{
        f = speed * 0.6666666;
    }
}
};

#endif
