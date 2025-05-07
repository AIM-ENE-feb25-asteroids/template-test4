const int northGreenLed = 13;
const int northRedLed = 9; 
const int northYellowLed = 11;
const int southGreenLed = 12;
const int southRedLed = 8;
const int southYellowLed = 10;

const int maintenanceLed = A5;
const int westLed = A3;
const int eastLed = A4;

const int maintenanceAtNumberOfBarrierOpens = 2;

const int blinkInterval = 500; // in ms
unsigned long lastBlinkTime = 0;
bool yellowOn = false;

unsigned long lastBlinkLedsWhenBarrierOpenTime = 0;
bool blinkLedWhenBarrierOpenOn = false;
int ledsWhenBarrierOpenBlinkInterval = 500;

const int testingInterval = 1000; // in ms
unsigned long lastTestingTime = 0;
bool testingLeds = false; // 0=false 1=true 2=done
int testLedIndex = 0;

int testLedPins[] = {
  northGreenLed,
  southGreenLed,
  northYellowLed,
  southYellowLed,
  northRedLed,
  southRedLed
};

const int numOfTestLeds = sizeof(testLedPins) / sizeof(testLedPins[0]);

int testLeds() {
  if(!testingLeds) {
    for(int i = 0; i < numOfTestLeds; i++) {
    analogWrite(testLedPins[i], 0);
    }
    testLedIndex = 0;
    testingLeds = true;
    lastTestingTime = millis();
    analogWrite(testLedPins[testLedIndex], 255);
  } else if(testingLeds && millis() - lastTestingTime >= testingInterval) {
    analogWrite(testLedPins[testLedIndex], 0);

    testLedIndex++;

    if (testLedIndex < numOfTestLeds) {
      analogWrite(testLedPins[testLedIndex], 255);
      lastTestingTime = millis();
    } else {
      testingLeds = false;
      return true;
    }
  }
  return false;
}

void setupLeds() {
  pinMode(northGreenLed, OUTPUT);
  pinMode(northRedLed, OUTPUT);
  pinMode(northYellowLed, OUTPUT);

  pinMode(southGreenLed, OUTPUT);
  pinMode(southRedLed, OUTPUT);
  pinMode(southYellowLed, OUTPUT);

  pinMode(westLed, OUTPUT);
  pinMode(eastLed, OUTPUT);

  setRed(northSide);
  setRed(southSide);
}

void blinkLedsWhenBarrierOpen() {
  if(!barrierClosed()) {
    if (millis() - lastBlinkLedsWhenBarrierOpenTime >= ledsWhenBarrierOpenBlinkInterval) {
    lastBlinkLedsWhenBarrierOpenTime = millis();
    blinkLedWhenBarrierOpenOn = !blinkLedWhenBarrierOpenOn;

    analogWrite(westLed, blinkLedWhenBarrierOpenOn ? 255 : 0);
    analogWrite(eastLed, blinkLedWhenBarrierOpenOn ? 255 : 0);
    }
  } else {
    analogWrite(westLed, 0);
    analogWrite(eastLed, 0);
  }
}

void turnOnMaintenanceLed() {
  if(EEPROM.get(eepromAdress ,barrierOpenCount) > maintenanceAtNumberOfBarrierOpens) {
    analogWrite(maintenanceLed, 225);
  }
}

void blinkBothYellow() {
  analogWrite(northGreenLed, 0);
  analogWrite(southGreenLed, 0);

  if (millis() - lastBlinkTime >= blinkInterval) {
    lastBlinkTime = millis();
    yellowOn = !yellowOn;

    analogWrite(northYellowLed, yellowOn ? 255 : 0);
    analogWrite(southYellowLed, yellowOn ? 255 : 0);
  }
}

void setYellow(int direction) {
  if(direction == northSide) {
    analogWrite(northGreenLed, 0); 
    analogWrite(northYellowLed, 255);  
    northYellowStartTime = millis();   
    northYellowActive = true;          
    northGreen = false; 
  } else if(direction == southSide)  {
    analogWrite(southGreenLed, 0);
    analogWrite(southYellowLed, 255);  
    southYellowStartTime = millis();   
    southYellowActive = true;  
    northGreen = false;   
  }
}

void setRed(int direction) {
  if(direction == northSide) {
    analogWrite(northGreenLed, 0);   
    analogWrite(northYellowLed, 0);  
    digitalWrite(northRedLed, HIGH); 
  northGreen = false;    
  } else if(direction == southSide)  {
    analogWrite(southGreenLed, 0);   
    analogWrite(southYellowLed, 0);  
    digitalWrite(southRedLed, HIGH); 
    southGreen = false;    
  }
}

void setGreen(int direction) {
  if(direction == northSide) {
    analogWrite(southGreenLed, 0);  
    analogWrite(southRedLed, 255);  
    analogWrite(southYellowLed, 0);  

    analogWrite(northYellowLed, 0);  
    digitalWrite(northRedLed, LOW);  
    digitalWrite(northGreenLed, HIGH); 
  northGreen = true;
  } else if(direction == southSide)  {
    analogWrite(northGreenLed, 0);  
    analogWrite(northRedLed, 255);  
    analogWrite(northYellowLed, 0);  

    analogWrite(southYellowLed, 0);  
    digitalWrite(southRedLed, LOW);  
    digitalWrite(southGreenLed, HIGH); 
    southGreen = true;
  }
}