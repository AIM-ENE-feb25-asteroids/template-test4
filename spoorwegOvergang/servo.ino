const int SERVOPIN = 5;

unsigned long previous_millis = 0;
const int speed = 60;
int servo_stand = 90;
Servo servo;

bool hasTracked = false;

void setupServo() {
  servo.attach(SERVOPIN);
  servo.write(90);
  trackMaintenance();
}

void trackMaintenance() {
  barrierOpenCount = EEPROM.get(eepromAdress, barrierOpenCount);
  barrierOpenCount++; 
  Serial.print("Barrier has opened ");
  Serial.print(barrierOpenCount);
  Serial.print(" times\n");
  EEPROM.put(eepromAdress, barrierOpenCount);
}

void openBarrier() {
  long currentMillis = millis();
  if (currentMillis - previous_millis >= speed && servo_stand != 90) {
    previous_millis = currentMillis;
    servo_stand++;
    servo.write(servo_stand);
  }
  
  if (!hasTracked && servo_stand == 90) {
    trackMaintenance();
    hasTracked = true;
  }
}

void closeBarrier() {
  long currentMillis = millis();
  if (currentMillis - previous_millis >= speed && servo_stand != 0) {
    previous_millis = currentMillis;
    servo_stand--;
    servo.write(servo_stand);
  }

  if(hasTracked && servo_stand == 0) {
    hasTracked = false;
  }
}

boolean barrierClosed() {
  if (servo_stand == 0) {
    return true;
  } 
  return false;
}

boolean barrierOpen() {
  if (servo_stand == 90) {
    return true;
  } 
  return false;
}