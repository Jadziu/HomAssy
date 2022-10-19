/*
Test program to check connection and I2C protocol between ESP-01 and SHT21 temp and hum sensor.
*/

#include "Wire.h"
#include "SHT2x.h"

SHT2x sht;

void setup()
{
  Serial.begin(115200);
  delay(50);
  sht.begin(2, 0);
  delay(50);
  uint8_t stat = sht.getStatus();
  Serial.print(stat, HEX);
  Serial.println();
  delay(200);
}


void loop()
{
  sht.read();
  Serial.print("TEMP: ");
  Serial.print(sht.getTemperature(), 1);
  Serial.print("*C");
  Serial.print("\t");
  Serial.print("HUM: ");
  Serial.print(sht.getHumidity(), 1);
  Serial.println("%");
  delay(2000);
}