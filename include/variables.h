#define lineThreshold 170
#define noiseThreshold 140

#define CALIBRATION_DURATION 1500

// hold RGB and clear colour values.
uint16_t r = 0, g = 0, b = 0, c = 0;

int lastPosition = 0;
int errDeadzone = 10;
bool calibrate = false;
bool calibrate_prev = false;

bool intersectionL = false;
bool intersectionR = false;
bool lineOnline = false;

int adcMin[8] = {4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096};
int adcMax[8] = {0, 0, 0, 0, 0, 0, 0, 0};
