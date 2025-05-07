enum TestState {
  TEST_LEDS,
  TEST_BUZZER,
  TEST_CLOSE_BARRIER,
  TEST_OPEN_BARRIER,
  TEST_COUNTDOWN_DISPLAY
};

TestState testState = TEST_LEDS;

void test() {
  switch (testState) {
    case TEST_LEDS: // Wacht tot LEDs klaar zijn
      if (testLeds()) {
        testState = TEST_BUZZER;
      }
      break;

    case TEST_BUZZER: // Buzzer afspelen
      if (testBuzzer()) {
        testState = TEST_CLOSE_BARRIER;
      }
      break;

    case TEST_CLOSE_BARRIER: // Wachten tot slagboom dicht is
      closeBarrier();
      if (barrierClosed()) {
        testState = TEST_OPEN_BARRIER;
      }
      break;

    case TEST_OPEN_BARRIER: // Wachten tot slagboom weer open is
      openBarrier();
      if (barrierOpen()) {
        testState = TEST_COUNTDOWN_DISPLAY;
      }
      break;
    case TEST_COUNTDOWN_DISPLAY:
      if(!countdownDisplay()) {
        crossingState = IDLE;
        testing = false;
        testState = TEST_LEDS;  // Reset voor volgende keer
        TESTING_EXIT();
      }
      break;
  }
}
