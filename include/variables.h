#define lineThreshold 170
#define noiseThreshold 80

#define CALIBRATION_DURATION 1500

// hold RGB and clear colour values.
uint16_t r, g, b, c;

int lastPosition = 0;
int errDeadzone = 10;
bool calibrate = false;
bool calibrate_prev = false;
bool intersection = false;

int adcMin[8] = {4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096};
int adcMax[8] = {0, 0, 0, 0, 0, 0, 0, 0};
