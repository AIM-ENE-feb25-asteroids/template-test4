CrossingState lastCrossingState = IDLE;

void loopPrint() {
  if (crossingState != lastCrossingState) {
    lastCrossingState = crossingState;
    switch (crossingState) {
      case IDLE:
        Serial.println("IDLE");
        break;

      case NORTH_TO_RED:
        Serial.println("NORTH_TO_RED");
        break;

      case SOUTH_TO_GREEN:
        Serial.println("SOUTH_TO_GREEN");
        break;

      case SOUTH_TO_RED:
        Serial.println("SOUTH_TO_RED");
        break;

      case NORTH_TO_GREEN:
        Serial.println("NORTH_TO_GREEN");
        break;

      case WAIT_AFTER_BOTH_RED:
        Serial.println("WAIT_AFTER_BOTH_RED");
        break;

      case CLEARANCE:
        Serial.println("CLEARANCE");
        break;

      case TRAIN_ARRIVAL_WEST:
        Serial.println("TRAIN_ARRIVAL_WEST");
        break;

      case TRAIN_ARRIVAL_EAST:
        Serial.println("TRAIN_ARRIVAL_EAST");
        break;

      case TRAIN_PASSES:
        Serial.println("TRAIN_PASSES");
        break;

      case TRAIN_PASSED:
        Serial.println("TRAIN_PASSED");
        break;

      case WAIT_DISPLAY:
        Serial.println("WAIT_DISPLAY");
        break;
      case TESTING:
        Serial.println("TESTING");
        break;
    }
  }
}
