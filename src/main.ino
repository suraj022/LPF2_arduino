#include <Arduino.h>
#include <MCP3208.h>
#include "LPF2.h"
#include "variables.h"

//-------------------------------------------------------------------
#define RXD2 31
#define TXD2 30
//-------------------------------------------------------------------
LPF2 sensor(RXD2, TXD2, 62, 115200, &Serial, &SerialUSB);
MCP3208 adc;
unsigned long last_reading = 0;
unsigned long last_calibrate_time = 0;
//-------------------------------------------------------------------
void CALLBACK_MODE0(byte buf[], byte s)
{
  // ENABLE or disable calibration boolean
}
//-------------------------------------------------------------------
void setup()
{
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  sensor.enable_debug();
  sensor.create_mode("VALUES", true, DATA8, 6, 5, 0, 0.0f, 512.0f, 0.0f, 1024.0f, 0.0f, 100.0f, "XYBD", 0, ABSOLUTE);
  sensor.create_mode("DISTANCE", true, DATA32, 1, 5, 0);
  sensor.get_mode(0)->setCallback(CALLBACK_MODE0); // attach call back function to mode 0
  delay(100);
  sensor.reset();
  adc.begin(4, 6, 5, 7);
  delay(200);
}
//-------------------------------------------------------------------
void loop()
{
  int _error = linePosition();
  if (!calibrate)
  {
    int sensorValues[8];
    for (int i = 0; i < 8; i++)
    {
      sensorValues[i] = map(adc.readADC(i), adcMin[i], adcMax[i], 0, 255);
      sensorValues[i] = constrain(sensorValues[i], 0, 255);
    }
    String x = "";
    for (int i = 0; i < 7; i++)
      x += (String)((sensorValues[i] > lineThreshold) ? 1 : 0) + ",";
    x += (String)((sensorValues[7] > lineThreshold) ? 1 : 0);
    x += "\t\t";
    x += (String)_error;
    SerialUSB.println(x);
  }
  delay(0);

  handleCalibration();

  // important!
  sensor.heart_beat();

  if (millis() - last_reading > 10) // send data every 10 millisecond
  {
    int mode = sensor.get_current_mode();
    if (mode == 0)
    {
      byte bb[8] = {0, 0, 0, 0, 0, 0, 0, 0};
      bb[1] = map(_error,0,700,0,255);
      bb[0] = digitalRead(A4) + 2*calibrate;
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