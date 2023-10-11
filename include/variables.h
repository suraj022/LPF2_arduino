#define lineThreshold 170
#define noiseThreshold 140

#define CALIBRATION_DURATION 1500

// hold RGB and clear colour values.
<<<<<<< HEAD
uint16_t r = 0, g = 0, b = 0, c = 0;
=======
uint16_t r, g, b, c;
>>>>>>> 726a68bca31aae407f27fc7bfb5b5a083c52055b

int lastPosition = 0;
int errDeadzone = 10;
bool calibrate = false;
bool calibrate_prev = false;
<<<<<<< HEAD
bool intersectionL = false;
bool intersectionR = false;
bool lineOnline = false;
=======
bool intersection = false;
>>>>>>> 726a68bca31aae407f27fc7bfb5b5a083c52055b

int adcMin[8] = {4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096};
int adcMax[8] = {0, 0, 0, 0, 0, 0, 0, 0};
