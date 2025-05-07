void handleSerialInput(char input) {
  if (input == '\n' || input == '\r') {
    return;
  }

  switch (input) {
    case '1':
      Serial.println("Button North pressed");
      northRequest = true;
      break;
    case '2':
      Serial.println("Button South pressed");
      southRequest = true;
      break;
    case '3':
      Serial.println("Sensor train west");
      westTrain = true;
      break;
    case '4':
      Serial.println("Sensor train east");
      eastTrain = true;
      break;
    case 't':
      Serial.println("Button test pressed");
      testing = true;
      break;
    case 'r':
      Serial.println("Maintenance count reset");
      EEPROM.put(eepromAdress, 0);
      Serial.println(EEPROM.get(eepromAdress, barrierOpenCount));
      break;
    default:
      Serial.println("Invalid input");
      break;
  }
}

void loopButtons() {
  if (Serial.available() > 0) {
    char input = Serial.read();
    handleSerialInput(input);
  }
}
