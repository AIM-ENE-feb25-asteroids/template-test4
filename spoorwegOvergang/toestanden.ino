unsigned long greenTimer = 0;
unsigned long waitGreenTime = 1000; // in ms

unsigned long trainArrivesGreenTimer = 0;
unsigned long trainArrivesWaitGreenTime = 5000; // in ms

unsigned long yellowDuration = 3000; 

unsigned long redTimer = 0;
unsigned long waitRedTime = 3000; // in ms

enum Direction { NONE, NORTH, SOUTH }; // houd bij welke directie als laatste groen had
Direction lastGreen = NONE;

void setupAll() {
  setupServo();
  setupLeds();
  setupDisplay();
  setupBuzzer();
}

void loopAll() {
  turnOnMaintenanceLed();
  loopPotmeter();
  blinkLedsWhenBarrierOpen();
  loopButtons();
  loopOvergang();
  loopPrint();
}

void setTrainArrivesWaitGreenTime(unsigned long time) {
  trainArrivesWaitGreenTime = time;
}

void loopOvergang() {
  switch (crossingState) {
    case IDLE:
      IDLE_DO();
      if(testing) {
        crossingState = TESTING;
      } else if (westTrain) {
        TRAIN_ARRIVAL_WEST_ENTRY();
        crossingState = TRAIN_ARRIVAL_WEST;
      } else if (eastTrain) {
        TRAIN_ARRIVAL_EAST_ENTRY();
        crossingState = TRAIN_ARRIVAL_EAST;
      } else if(northRequest && southRequest && !northGreen && !southGreen) {
        if (lastGreen == SOUTH) {
          NORTH_TO_GREEN_ENTRY();
          crossingState = NORTH_TO_GREEN;
        } else {
          SOUTH_TO_GREEN_ENTRY();
          crossingState = SOUTH_TO_GREEN;
        }
      } else if (northRequest && !northGreen && !southGreen) {
        NORTH_TO_GREEN_ENTRY();
        crossingState = NORTH_TO_GREEN;
      } else if (southRequest && !northGreen && !southGreen) {
        SOUTH_TO_GREEN_ENTRY();
        crossingState = SOUTH_TO_GREEN;
      } else if (northRequest && southGreen) {
        SOUTH_TO_RED_ENTRY();
        crossingState = SOUTH_TO_RED;
      } else if (southRequest && northGreen) { 
        NORTH_TO_RED_ENTRY();
        crossingState = NORTH_TO_RED;
      }
      break;

    case NORTH_TO_RED:
      NORTH_TO_RED_DO();  
      break;

    case SOUTH_TO_GREEN:
      SOUTH_TO_GREEN_DO();
      if(westTrain && millis() - trainArrivesGreenTimer >= trainArrivesWaitGreenTime || eastTrain && millis() - trainArrivesGreenTimer >= trainArrivesWaitGreenTime) {
        crossingState = IDLE;
      } else if(millis() - greenTimer >= waitGreenTime && !westTrain && !eastTrain) {
        crossingState = IDLE; 
      }
      break;

    case SOUTH_TO_RED:
      SOUTH_TO_RED_DO();  
      break;

    case NORTH_TO_GREEN:
      NORTH_TO_GREEN_DO();
      if(westTrain && millis() - trainArrivesGreenTimer >= trainArrivesWaitGreenTime || eastTrain && millis() - trainArrivesGreenTimer >= trainArrivesWaitGreenTime) {
        crossingState = IDLE;
      } else if(millis() - greenTimer >= waitGreenTime && !westTrain && !eastTrain) {
        crossingState = IDLE; 
      }
      break;
    
    case WAIT_AFTER_BOTH_RED:
      WAIT_AFTER_BOTH_RED_DO();
      break;

    case CLEARANCE:
      CLEARANCE_DO();
      if(barrierClosed()) {
        TRAIN_PASSES_ENTRY();
        crossingState = TRAIN_PASSES;
      }
      break;

    case TRAIN_ARRIVAL_WEST: 
      TRAIN_ARRIVAL_WEST_DO();
      break;

    case TRAIN_ARRIVAL_EAST: 
      TRAIN_ARRIVAL_EAST_DO();
      break;

    case TRAIN_PASSES: 
      TRAIN_PASSES_DO();
      if(westTrain && eastTrain) {
        TRAIN_PASSED_ENTRY();
        crossingState = TRAIN_PASSED;
      }
      break;

    case TRAIN_PASSED: 
      TRAIN_PASSED_DO();
      if(barrierOpen()) {
        TRAIN_PASSED_EXIT();
        westTrain = false;
        eastTrain = false;
        WAIT_DISPLAY_ENTRY();
        crossingState = WAIT_DISPLAY;
      }
      break;

    case WAIT_DISPLAY: 
      WAIT_DISPLAY_DO();
      if(!countdownDisplay()) { // returned false wanneer die klaar is met aftellen
        WAIT_DISPLAY_EXIT();
        crossingState = IDLE; 
      }
      break;
    case TESTING: 
      TESTING_DO();
      break;
  }
}

// specifieke functies
void checkIfYellowLightIsOn() {
  if (southYellowActive) {
    if (millis() - southYellowStartTime >= yellowDuration) {
      setRed(southSide); 
      setRed(northSide);
      crossingState = CLEARANCE; 
    }
  } else if (northYellowActive) {
    if (millis() - northYellowStartTime >= yellowDuration) {
      setRed(northSide);
      setRed(northSide);
      crossingState = CLEARANCE; 
    }
  } else if (!southYellowActive && !northYellowActive) {
    crossingState = CLEARANCE;
  }
}

// -------------------- ENTRY functies
void WAIT_AFTER_BOTH_RED_ENTRY() {
  redTimer = millis();
}

void NORTH_TO_RED_ENTRY() {
  setYellow(northSide); 
}

void SOUTH_TO_GREEN_ENTRY() {
  lastGreen = SOUTH;
  greenTimer = millis();
  trainArrivesGreenTimer = millis();
  setGreen(southSide); 
  southRequest = false;
}

void SOUTH_TO_RED_ENTRY() {
  setYellow(southSide); 
}

void NORTH_TO_GREEN_ENTRY() {
  lastGreen = NORTH;
  greenTimer = millis();
  trainArrivesGreenTimer = millis();
  setGreen(northSide); 
  northRequest = false;
}

void CLEARANCE_ENTRY() {
  setRed(northSide);
  setRed(southSide);
}

void TRAIN_ARRIVAL_WEST_ENTRY() {
  if (northGreen) {
    setYellow(northSide);
  } else if (southGreen) {
    setYellow(southSide);
  }
}

void TRAIN_ARRIVAL_EAST_ENTRY() {
  if (northGreen) {
    setYellow(northSide);
  } else if (southGreen) {
    setYellow(southSide);
  }
}

void TRAIN_PASSES_ENTRY() {
  setRed(northSide);
  setRed(southSide);
}

void TRAIN_PASSED_ENTRY() {
}

void WAIT_DISPLAY_ENTRY() {

}

// ------------------ DO functies
void IDLE_DO() {
}

void WAIT_AFTER_BOTH_RED_DO() {
  if(millis() - redTimer >= waitRedTime) {
    crossingState = IDLE; 
  }
}

void NORTH_TO_RED_DO() {
  if (northYellowActive) {
    if (millis() - northYellowStartTime >= yellowDuration) {
      setRed(northSide);
      if (southGreen) {
        setRed(southSide);
      }
      WAIT_AFTER_BOTH_RED_ENTRY();
      crossingState = WAIT_AFTER_BOTH_RED; 
    }
  }
}

void SOUTH_TO_GREEN_DO() {
}

void SOUTH_TO_RED_DO() {
  if (southYellowActive) {
    if (millis() - southYellowStartTime >= yellowDuration) {
      setRed(southSide); 
      if (northGreen) {
        setRed(northSide);
      }
      crossingState = WAIT_AFTER_BOTH_RED; 
      WAIT_AFTER_BOTH_RED_ENTRY();
    }
  }
}

void NORTH_TO_GREEN_DO() {
}

void TRAIN_ARRIVAL_WEST_DO() {
  checkIfYellowLightIsOn();
}

void TRAIN_ARRIVAL_EAST_DO() {
  checkIfYellowLightIsOn();
}

void CLEARANCE_DO() {
  blinkBothYellow();
  closeBarrier();
  tickBuzzer();
}

void TRAIN_PASSES_DO() {
  blinkBothYellow();
}

void TRAIN_PASSED_DO() {
  blinkBothYellow();
  openBarrier();
  tickBuzzer();
}

void WAIT_DISPLAY_DO() {
  burstTickBuzzer();
  blinkBothYellow();
}

void TESTING_DO() {
  test();
}

// ------------------ EXIT functies
void NORTH_TO_RED_EXIT() {
}

void SOUTH_TO_GREEN_EXIT() {
}

void SOUTH_TO_RED_EXIT() {
}

void NORTH_TO_GREEN_EXIT() {
}

void CLEARANCE_EXIT() {
}

void TRAIN_ARRIVAL_WEST_EXIT() {
}

void TRAIN_ARRIVAL_EAST_EXIT() {
}

void TRAIN_PASSES_EXIT() {
}

void TRAIN_PASSED_EXIT() {
  setRed(northSide);
  setRed(southSide);
}

void WAIT_DISPLAY_EXIT() {
  setRed(northSide); 
  setRed(southSide);
}

void TESTING_EXIT() {
  setRed(northSide);
  setRed(southSide);
}
