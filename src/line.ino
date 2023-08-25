int linePosition()
{
    int returnValue = 0;
    // bool online = false;
    lineOnline = false;
    int avg = 0;
    int sum = 0;
    int sensorValues[8];
    for (int i = 0; i < 8; i++)
    {
        sensorValues[i] = map(adc.readADC(i), adcMin[i], adcMax[i], 0, 255);
        sensorValues[i] = constrain(sensorValues[i], 0, 255);
        if (sensorValues[i] > lineThreshold) // threshold
            lineOnline = true;
        if (sensorValues[i] > noiseThreshold)
        { // noise
            avg += sensorValues[i] * i * 100;
            sum += sensorValues[i];
        }
    }

    if (!lineOnline)
    {
        if (lastPosition > 400)
            returnValue = 700;
        else if (lastPosition < 300)
            returnValue = 0;
        else
            returnValue = 350;
        return returnValue;
    }
    lastPosition = avg / sum;
    returnValue = lastPosition;
    return returnValue;
}

void handleCalibration()
{
    if (!digitalRead(A3))
        calibrate = true;
    if (calibrate)
    {
        if (!calibrate_prev)
        {
            calibrate_prev = true;
            last_calibrate_time = millis();
            SerialUSB.println("start calibrate");
            for (int i = 0; i < 8; i++)
            {
                adcMin[i] = 4096;
                adcMax[i] = 0;
            }
        }
        SerialUSB.println("...");
        for (int i = 0; i < 8; i++)
        {
            int tmp = adc.readADC(i);
            if (tmp < adcMin[i])
                adcMin[i] = tmp;
            if (tmp > adcMax[i])
                adcMax[i] = tmp;
        }
        if (millis() - last_calibrate_time > CALIBRATION_DURATION)
        {
            calibrate = false;
            calibrate_prev = false;
            SerialUSB.println("stop calibrating");

            String x = "int adcMin[8] = { ";
            for (int i = 0; i < 7; i++)
                x += (String)adcMin[i] + ",";
            x += (String)adcMin[7];
            x += "};";
            SerialUSB.println(x);

            x = "int adcMax[8] = { ";
            for (int i = 0; i < 7; i++)
                x += (String)adcMax[i] + ",";
            x += (String)adcMax[7];
            x += "};";
            SerialUSB.println(x);
        }
    }
}