#ifndef __SHCUSTOMPROTOCOL_H__
#define __SHCUSTOMPROTOCOL_H__

#include <Arduino.h>
#include <TimerOne.h>
#include <BeanMPX.h>
#include <AD520X.h>

unsigned long t;
float f = 0; // 10f = 15 speed
float speed = 0;//1.5 speed = 1 frequency

BeanMPX bean;
uint32_t timer = 0;
uint32_t current_millis = 0;
uint32_t timer2 = 0;
uint32_t current_millis2 = 0;
uint8_t powerSteering[] =         {0x62, 0x21, 0x02}; //Power Steering Fault Clear
uint8_t SRS[] =                   {0x62, 0x7A, 0x01}; //SRS clear
uint8_t stabilityControl[] =      {0x62, 0x91, 0x40, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}; //ABS and TC clear D1 0x40 = Clear | D2 0x20 = Park Brake, 0x10 = Clear, 0x0f ABS On, 0x08 ABS Flash | D7 0x02 = Rapid Trac Flash, 0x04 = Slow Trac Flash, 0x0b = Trac Solid, 0x18 = Trac Off Light 
uint8_t gear[] =                  {0x62, 0x40, 0x20}; //Default to Neutral  P,R,N,D (0x80, 0x40, 0x20, 0x10)
uint8_t cruise[] =                {0x62, 0xc8, 0x12, 0x40, 0x14}; //Cruise Off = 0x40, On = 0x10
uint8_t lights[] =                {0x62, 0x7F, 0x04}; //Lights 0x10 = Low Beam, 0x08 = High Beam, 0x01 = Front Fog
uint8_t outTemp[] =               {0x62, 0xCD, 0x28}; //0x08 = -40, 0x59 = 40
uint8_t engTemp[]  =              {0x62, 0x2C, 0xA5}; //0x5A - 0xFF
uint8_t sportMode[] =             {0x62, 0x9D, 0x20, 0x80, 0x80}; //D2 0x40=6,0x20=5,0x10=4,0x08=3,0x04=2,0x02=1 | D3 0x08 = 1, 0x10 = 2, 0x20 = 3, 0x30 = 4, 0x40 = 5, 0x50 = 6

int cruiseP = 0x40;
int lightsP = 0x04;

int sGear;
int sMaxGear;
int sCruise;
//int sLights;
int sEngTemp;
int sOutTemp;
int sHandbrake;
int sFuel;
int sendIndex = 1;

AD8400 pot = AD8400(53, 255, 255,51,52 );

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
      pinMode(38, OUTPUT);
      pinMode(39, OUTPUT);
      pot.begin(4);
    }

    // Called when new data is coming from computer
    void read() {
      // EXAMPLE 1 - read the whole message and sent it back to simhub as debug message
      // Protocol formula can be set in simhub to anything, it will just echo it
      // -------------------------------------------------------
     /* speed = FlowSerialReadStringUntil('\n').toFloat();
      
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
      Timer1.pwm(11, 512);*/
      sGear = FlowSerialReadStringUntil(';').toInt();
      sCruise = FlowSerialReadStringUntil(';').toInt();
      int sCruiseSpeed = FlowSerialReadStringUntil(';').toInt();
      int sLowBeam = FlowSerialReadStringUntil(';').toInt();
      int sHighBeam = FlowSerialReadStringUntil(';').toInt();
      int sFog = FlowSerialReadStringUntil(';').toInt();
      sEngTemp = FlowSerialReadStringUntil(';').toInt();
      sOutTemp = FlowSerialReadStringUntil(';').toInt();
      sHandbrake = FlowSerialReadStringUntil(';').toInt();
      sFuel = FlowSerialReadStringUntil(';').toInt();
      int sLeftTurn = FlowSerialReadStringUntil(';').toInt();
      int sRightTurn = FlowSerialReadStringUntil(';').toInt();
      sMaxGear = FlowSerialReadStringUntil('\n').toInt();

      lights[2] = 0x04;
      if (sGear == 0){
        gear[2] = 0x20;
        sportMode[2] = 0x20;
        sportMode[3] = 0x80;
        sportMode[4] = 0x80;
      }
      else if (sGear == -1){
        gear[2] = 0x40;
        sportMode[2] = 0x20;
        sportMode[3] = 0x80;
        sportMode[4] = 0x80;
      }
      else if (sGear >= 1 && sMaxGear >= 7 || sMaxGear == 0){
        gear[2] = 0x10;
        sportMode[2] = 0x20;
        sportMode[3] = 0x80;
        sportMode[4] = 0x80;
      }
      else{
        sportMode[2] = 0x80;
        switch (sGear){
          case 1:
          sportMode[3] = 0x02;
          sportMode[4] = 0x08;
          break;
          case 2:
          sportMode[3] = 0x04;
          sportMode[4] = 0x10;
          break;
          case 3:
          sportMode[3] = 0x08;
          sportMode[4] = 0x20;
          break;
          case 4:
          sportMode[3] = 0x10;
          sportMode[4] = 0x30;
          break;
          case 5:
          sportMode[3] = 0x20;
          sportMode[4] = 0x40;
          break;
          case 6:
          sportMode[3] = 0x40;
          sportMode[4] = 0x50;
          break;
        }
      }
      if (sCruise == 1){
        cruise[3] = 0x10;
      }
      else{
        cruise[3] = 0x40;
      }
      if (sLowBeam == 1){
        lights[2] = lights[2] + 0x10;
      }
      if (sHighBeam == 1){
        lights[2] = lights[2] + 0x08;
      }
      if (sFog == 1){
        lights[2] = lights[2] + 0x01;
      }
      //Eng temp interpret here.
      outTemp[2] = sOutTemp + 48;
      if (sHandbrake == 1){
        stabilityControl[3] = 0x20;
      }
      else{
        stabilityControl[3] = 0x10;
      }
      /*
      if (cruiseP != cruise[3])
      {
        bean.sendMsg(cruise, sizeof(cruise));   
        cruiseP = cruise[3];
      }*/
      if (lightsP != lights[2])
      {
        bean.sendMsg(lights, sizeof(lights));   
        lightsP = lights[2];
      }
      if(sLeftTurn == 1){
        digitalWrite(38, HIGH);
      }
      else{
        digitalWrite(38,LOW);
      }
      if(sRightTurn == 1){
        digitalWrite(39, HIGH);
      }
      else{
        digitalWrite(39,LOW);
      }
      cruise[4] = sCruiseSpeed; 
      sEngTemp = map(sEngTemp, 40, 140, 0x5a, 0xfe);
      engTemp[2] = sEngTemp;
      sFuel = map(sFuel, 0, 100, 82, 0);
     
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

  
      if (current_millis - timer > 45) {
        if (!bean.isBusy()) {
          switch (sendIndex){
           case 1:
            bean.sendMsg(powerSteering, sizeof(powerSteering));
            sendIndex++;
            break;
            case 2:
            bean.sendMsg(SRS, sizeof(SRS));
            sendIndex++;
            pot.setValue(0, sFuel);
            break;
            case 3:
            bean.sendMsg(stabilityControl, sizeof(stabilityControl));
            sendIndex++;
            break;
            case 4:
            bean.sendMsg(gear, sizeof(gear));
            sendIndex++;
            break;
            case 5:
            bean.sendMsg(cruise, sizeof(cruise));
            sendIndex++;
            break;
            case 6:
            bean.sendMsg(engTemp, sizeof(engTemp));
            sendIndex++;
            break;
            case 7:
            bean.sendMsg(outTemp, sizeof(outTemp));
            sendIndex++;
            //FlowSerialDebugPrintLn(millis());
            break;
            case 8:
            bean.sendMsg(sportMode, sizeof(sportMode));
            sendIndex = 1;
            break;
          }
      timer = current_millis;
    }    
  }
    //Timer1.setPeriod(t);
    //Timer1.pwm(11, 512);
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
