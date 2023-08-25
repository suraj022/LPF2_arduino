#define lineThreshold 170
#define noiseThreshold 80

#define CALIBRATION_DURATION 1500

int lastPosition = 0;
int errDeadzone = 10;
bool calibrate = false;
bool calibrate_prev = false;

int adcMin[8] = {4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096};
int adcMax[8] = {0, 0, 0, 0, 0, 0, 0, 0};
