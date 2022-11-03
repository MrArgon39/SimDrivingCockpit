#include <BeanMPX.h>

//A little program to demonstrate some of the more obvious MPX messages, there are more that were not used here.

// RX: Len MsgType SOF DID MID DAT0 ... CRC EOM EOF
#define RX_BUFFER_SIZE 20

BeanMPX bean;

// Messages           {DID,  MID,  DAT0, DAT1, DAT2}

uint8_t engTemp[]  =  {0x62, 0x2C, 0xA5}; // Engine Temp D0 90-255
uint8_t gear[] =      {0x62, 0x40, 0x08, 0x00}; // P,R,N,D,M,3,2,L D0 11111111 (128, 64, 32, 16, 8, 4, 2, 1); Manual mode: 5,4,3,2,L D1 ---11111 (16, 8, 4, 2, 1)
uint8_t ect[] =       {0x62, 0xD2, 0x08, 0x10, 0x00}; // ECT PWR, SNOW, CRUISE D0 --11--1- (32,16,2); CRUISE-FLASH, ECT PWR-FLASH D1 -11----- (64,32); Beep D2 ----1--- (8) toggle on/off
uint8_t seatBelt[] =  {0x62, 0xDF, 0x10, 0x80}; // DOOR D0 ---1---- (16);  SEAT BELT D1 1------- (128)

// DOOR-FLASH - Unused on the IS250, only for the IS200
uint8_t batt[] = {0x62, 0xD4, 0x28}; // DOOR-FLASH, BATT, OIL D0 -111---- (64, 32, 16)
uint8_t door[] = {0x62, 0xFA, 0xff}; // DOOR-FLASH D0 11111111 (255); *important mid:0xD4 D0:0x28 D0 -1------ must be set first //

uint32_t timer = 0;
uint32_t current_millis = 0;
uint8_t bean_rx_buffer[RX_BUFFER_SIZE];
uint8_t bean_rx_index = 0;
uint8_t g = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.println("BeanMPX");

  bean.ackMsg((const uint8_t[]) {
    0xFE
  }, 1); // Acknowledge Messages, Length
  bean.begin(8, 9);
}

void loop() {
  current_millis = millis();

  if (bean.available()) {
    bean.getMsg(bean_rx_buffer, RX_BUFFER_SIZE);

    Serial.print((char)bean_rx_buffer[1]);
    Serial.print(" ");
    for (int i = 2; i < bean_rx_buffer[0] + 2; i++) {
      if (bean_rx_buffer[i] < 0x10) {
        Serial.print(0, HEX);
      }
      Serial.print(bean_rx_buffer[i], HEX);
      Serial.print(" ");
    }
    Serial.print("\n");
    memset(bean_rx_buffer, 0, RX_BUFFER_SIZE);
  }

  if (current_millis - timer > 500) {
    if (!bean.isBusy()) {
      switch (g) {
        case 0://Low Beam
          gear[1] = 0x7F;
          gear[2] = 0x10;
          g++;
          break;
        case 1://High Beam
          gear[2] = 0x08;
          g++;
          break;
        case 2://Rear Fog
          gear[2] = 0x02;
          g++;
          break;
        case 3://Front Fog
          gear[2] = 0x01;
          g++;
          break;
        case 4://All lights
          gear[2] = 0xFF;
          g++;
          break;
        case 5://P
          gear[1] = 0x40;
          gear[2] = 0x80;
          g++;
          break;
        case 6://R
          gear[2] = 0x40;
          g++;
          break;
        case 7://N
          gear[2] = 0x20;
          g++;
          break;
        case 8://D
          gear[2] = 0x10;
          g++;
          break;
        case 9://All gears
          gear[2] = 0xFF;
          g++;
          break;
        case 10://PCS Warning Light
          gear[1] = 0xE5;
          gear[2] = 0x10;
          g++;
          break;
        case 11://ECT PWR
          gear[1] = 0x9D;
          gear[2] = 0x40;
          gear[3] = 0x80;
          g++;
          break;
        case 12://ECT SNOW
          gear[1] = 0xD2;
          gear[2] = 0x10;
          g++;
          break;
        case 13://Sports flash, gear restrict 6
          gear[1] = 0x9D;
          gear[2] = 0x42;
          gear[3] = 0x40;
          g++;
          break;
        case 14://Gear restrict 5
          gear[3] = 0x20;
          g++;
          break;
        case 15://Gear restrict 4
          gear[3] = 0x10;
          g++;
          break;
        case 16://Gear restrict 3
          gear[3] = 0x08;
          g++;
          break;
        case 17://Gear restrict 2
          gear[3] = 0x04;
          g++;
          break;
        case 18://Gear restrict 1
          gear[3] = 0x02;
          g = 0;
          break;
      }
      bean.sendMsg(gear, sizeof(gear)); //Send the edited message to the cluster
      timer = current_millis; //Non-blocking delay
    }
  }
}
