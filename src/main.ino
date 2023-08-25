#include <Arduino.h>
#include <MCP3208.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "LPF2.h"
#include "variables.h"

//-------------------------------------------------------------------
#define RXD2 31
#define TXD2 30
LPF2 sensor(RXD2, TXD2, 62, 115200, &Serial, &SerialUSB);
//-------------------------------------------------------------------
MCP3208 adc;
unsigned long last_reading = 0;
unsigned long last_calibrate_time = 0;
//-------------------------------------------------------------------
Adafruit_TCS34725 tcs = Adafruit_TCS34725(0xF7, TCS34725_GAIN_16X);
//-------------------------------------------------------------------

void CALLBACK_MODE0(byte buf[], byte s)
{
  // Process data received from spike prime.
}
//-------------------------------------------------------------------
void setup()
{
  // Define digital inputs
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);

  pinMode(A0, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);

  // LPF2 emulator intialization
  sensor.enable_debug();
  sensor.create_mode("VALUES", true, DATA8, 7, 5, 0, 0.0f, 512.0f, 0.0f, 1024.0f, 0.0f, 100.0f, "A", 0, ABSOLUTE);
  sensor.create_mode("DISTANCE", true, DATA32, 1, 5, 0);
  sensor.get_mode(0)->setCallback(CALLBACK_MODE0); // attach call back function to mode 0
  delay(100);
  sensor.reset();

  // Initialize MCP3208 ADC
  adc.begin(4, 6, 5, 7);

  // Initialize TCS34725 colour sensor
  // if (tcs.begin())
  // {
  //   SerialUSB.println("Found sensor");
  // }
  // else
  // {
  //   SerialUSB.println("No TCS34725 found ... check your connections");
  //   while (1)
  //     ;
  // }

  delay(200);
}
//-------------------------------------------------------------------
void loop()
{
  // tcs.getRawData(&r, &g, &b, &c);
  int _error = linePosition();
  int sensorValues[8];
  for (int i = 0; i < 8; i++)
  {
    sensorValues[i] = map(adc.readADC(i), adcMin[i], adcMax[i], 0, 255);
    sensorValues[i] = constrain(sensorValues[i], 0, 255);
  }
  // if (!calibrate)
  // {
  //   String x = "";
  //   for (int i = 0; i < 7; i++)
  //     x += (String)((sensorValues[i] > lineThreshold) ? 1 : 0) + ",";
  //   x += (String)((sensorValues[7] > lineThreshold) ? 1 : 0);
  //   x += "\t\t";
  //   x += (String)_error;
  //   SerialUSB.println(x);
  // }

  if ((((sensorValues[0] > lineThreshold) ? true : false) /*and
       ((sensorValues[1] > lineThreshold) ? true : false) and
       ((sensorValues[2] > lineThreshold) ? true : false)*/
       ))
  {
    intersectionL = true;
  }
  if ((/*((sensorValues[5] > lineThreshold) ? true : false) and
   ((sensorValues[6] > lineThreshold) ? true : false) and*/
       ((sensorValues[7] > lineThreshold) ? true : false)))
  {
    intersectionR = true;
  }

  handleCalibration();

  // important!
  sensor.heart_beat();

  if (millis() - last_reading > 10) // send data every 10 millisecond
  {
    int mode = sensor.get_current_mode();
    if (mode == 0)
    {
      byte bb[8] = {0, 0, 0, 0, 0, 0, 0, 0};
      byte byteZero = 0;
      bitWrite(byteZero, 0, digitalRead(A4));
      bitWrite(byteZero, 1, calibrate);
      bitWrite(byteZero, 2, intersectionL);
      bitWrite(byteZero, 3, intersectionR);
      bitWrite(byteZero, 4, lineOnline);
      bitWrite(byteZero, 5, !digitalRead(A0));
      bitWrite(byteZero, 6, !digitalRead(A2));
      bitWrite(byteZero, 7, 0); //currently empty. can be used to send any boolean value

      bb[0] = byteZero;
      bb[1] = map(_error, 0, 700, 0, 255);
      bb[2] = constrain(r, 0, 255);
      bb[3] = constrain(g, 0, 255);
      bb[4] = constrain(b, 0, 255);
      intersectionL = false;
      intersectionR = false;
      sensor.send_data8(bb, 8);
    }
    if (mode == 1)
    {
      sensor.send_data32(adc.readADC(0));
    }
    last_reading = millis();
  }
}
//-------------------------------------------------------------------