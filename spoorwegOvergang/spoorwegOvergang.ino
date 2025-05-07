#include <Servo.h>   
#include <EEPROM.h>

bool northGreen = false;
bool southGreen = false;

bool northRequest = false;
bool southRequest = false;

bool westTrain = false;
bool eastTrain = false;

bool northYellowActive = false;
bool southYellowActive = false;
unsigned long northYellowStartTime = 0;
unsigned long southYellowStartTime = 0;

bool testing = false;

int northSide = 0;
int southSide = 1;

int barrierOpenCount = 0;
const int eepromAdress = 0;

enum CrossingState {
  IDLE,
  NORTH_TO_RED,
  SOUTH_TO_GREEN,
  SOUTH_TO_RED,
  NORTH_TO_GREEN,
  WAIT_AFTER_BOTH_RED ,
  
  CLEARANCE,        
  TRAIN_ARRIVAL_WEST,    
  TRAIN_ARRIVAL_EAST,
  TRAIN_PASSES,   
  TRAIN_PASSED,    
  WAIT_DISPLAY,     

  TESTING
};

CrossingState crossingState = IDLE;

void setup() {
  Serial.begin(9600);
  setupAll();
}

void loop() {
  loopAll();
}
