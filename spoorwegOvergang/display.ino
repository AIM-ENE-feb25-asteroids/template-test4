const int CLOCK_PIN = 2;
const int LATCH_PIN = 3;
const int DATA_PIN  = 4;

unsigned long displayTimer = 0;
unsigned long displayInterval = 1000; // 1000 ms = 1 seconde
int countdown = 5; // Aftelwaarde

const byte DIGIT_FONT[] = {
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110   // 5
};

void setupDisplay() {
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(DATA_PIN, LOW);
  displayDigit(0);
}

bool isBitSet(byte value, int bitIndex) {
  return (value & (1 << bitIndex)) != 0;
}

bool countdownDisplay() {
  if (millis() - displayTimer >= displayInterval) {
    displayTimer = millis();
    countdown--;

    if (countdown < 0) {
      countdown = 0;
    }

    displayDigit(countdown);
  }

  if (countdown == 0) {
    countdown = 5;
    return false;
  } else {
    return true;
  }
}

void displayDigit(int digitIndex) {
  digitalWrite(LATCH_PIN, LOW);

  for (int bit = 0; bit < 8; bit++) {
    if (isBitSet(DIGIT_FONT[digitIndex], bit)) {
      digitalWrite(DATA_PIN, HIGH);
    } else {
      digitalWrite(DATA_PIN, LOW);
    }
    digitalWrite(CLOCK_PIN, HIGH);
    digitalWrite(CLOCK_PIN, LOW);
  }

  digitalWrite(LATCH_PIN, HIGH);
}