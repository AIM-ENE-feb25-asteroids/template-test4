const int buzzer = 7;
const unsigned long buzzerInterval = 100; 
const unsigned long burstTickBuzzerInterval = 750;
unsigned long lastBuzzerTime = 0;
bool buzzerOn = false;

int burstCount = 0;
int tickBurstAmount = 5;
unsigned long lastBurstTime = 0; 
bool inBurst = false; 

const int beepDuration = 1000;  // piepduur

void setupBuzzer() {
  pinMode(buzzer, OUTPUT);
}

bool testBuzzer() {
  unsigned long currentTime = millis();

  if (!buzzerOn) {
    tone(buzzer, 2000);
    buzzerOn = true;
    lastBuzzerTime = currentTime;
    return false;
  } else if (buzzerOn && currentTime - lastBuzzerTime >= beepDuration) {
    noTone(buzzer);
    buzzerOn = false;
    return true;
  }
  return false;
}


void tickBuzzer() {
  if (millis() - lastBuzzerTime >= buzzerInterval) {
    lastBuzzerTime = millis();
    buzzerOn = !buzzerOn;
    digitalWrite(buzzer, buzzerOn ? HIGH : LOW);
  }
}

void burstTickBuzzer() {
  unsigned long currentMillis = millis();
  
  if (burstCount < tickBurstAmount) {
    if (currentMillis - lastBuzzerTime >= buzzerInterval) {
      lastBuzzerTime = currentMillis;
      buzzerOn = !buzzerOn;
      digitalWrite(buzzer, buzzerOn ? HIGH : LOW);
      burstCount++;
    }
  } else {
    if (currentMillis - lastBurstTime >= burstTickBuzzerInterval) {
      burstCount = 0;  
      lastBurstTime = currentMillis;  
      buzzerOn = false; 
      digitalWrite(buzzer, buzzerOn);
    }
  }
}
