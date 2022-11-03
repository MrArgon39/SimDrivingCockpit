//
//    FILE: AD5204_setPercentage.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//    DATE: 2021-10-16
//     URL: https://github.com/RobTillaart/AD520X

// Modified for the setup I use, credit for original code goes to Rob

#include "AD520X.h"

uint32_t start, stop;


//  select, reset, shutdown, data, clock
//  AD5204 pot(10, 255, 255, 8, 9);  // SW SPI
//AD8400 pot = AD8400(10, 255, 255,11,13 );     // HW SPI Uno
AD8400 pot = AD8400(53, 255, 255,51,52 );     // HW SPI Mega

void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);

  pot.begin(4);

  for (int p = 0; p <= 100; p++)
  {
    pot.setPercentage(0, p);
    float pp = pot.getPercentage(0);
    Serial.print(p);
    Serial.print('\t');
    Serial.print(pp, 1);
    Serial.print('\n');
  }

  Serial.println("\nDone...");
}


void loop()
{
  pot.setValue(0, 0); //2 = full, 82 = empty
  delay(1000);
}


// -- END OF FILE --
