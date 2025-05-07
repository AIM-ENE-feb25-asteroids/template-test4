int potPin = A0;

unsigned long lastReadingTime = 0;
const int readingAmountPerSec = 10;
int readingInterval = 1000 / readingAmountPerSec;
int waitGreenMultiplier = 5;

int currentReadingAmount = 0;
int potReadings[readingAmountPerSec];
int sum = 0;

void loopPotmeter() {
  if(millis() - lastReadingTime >= readingInterval && currentReadingAmount < readingAmountPerSec) {
    potReadings[currentReadingAmount] = analogRead(potPin);
    currentReadingAmount++;
    lastReadingTime = millis();
  } else if(currentReadingAmount == 10) {
    for(int i=0; i < readingAmountPerSec; i++) {
      sum += potReadings[i];
    }
    int average = sum / readingAmountPerSec;
    if(average < 150) {
      setTrainArrivesWaitGreenTime(0);
    } else {
      setTrainArrivesWaitGreenTime(average * waitGreenMultiplier);
    }
    Serial.println(sum / readingAmountPerSec);
    sum = 0;
    currentReadingAmount = 0;
  }
}