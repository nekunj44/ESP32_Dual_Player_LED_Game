#include <Arduino.h>

// LED pins
const int player1LEDs[] = {2, 4, 5, 12, 13, 14};
const int player2LEDs[] = {18, 19, 21, 22, 23, 25};

const int numLEDs = sizeof(player1LEDs) / sizeof(player1LEDs[0]);

// Timing
unsigned long previousMillis = 0;
const long interval = 300;      // LED update interval
const long patternInterval = 5000; // Pattern change interval
unsigned long previousPatternMillis = 0;

// Pattern control
int currentLED = 0;
int currentPattern = 0;
const int totalPatterns = 4;

void setup() {
  for (int i = 0; i < numLEDs; i++) {
    pinMode(player1LEDs[i], OUTPUT);
    pinMode(player2LEDs[i], OUTPUT);
    digitalWrite(player1LEDs[i], LOW);
    digitalWrite(player2LEDs[i], LOW);
  }
}

void loop() {
  unsigned long currentMillis = millis();

  // Switch pattern every few seconds
  if (currentMillis - previousPatternMillis >= patternInterval) {
    previousPatternMillis = currentMillis;
    currentPattern++;
    if (currentPattern >= totalPatterns) currentPattern = 0;
    currentLED = 0; // Reset LED index for new pattern
  }

  // Update LEDs based on pattern interval
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    runPattern(currentPattern);
  }
}

void runPattern(int pattern) {
  switch (pattern) {
    case 0: // Chase pattern
      for (int i = 0; i < numLEDs; i++) {
        digitalWrite(player1LEDs[i], LOW);
        digitalWrite(player2LEDs[i], LOW);
      }
      digitalWrite(player1LEDs[currentLED], HIGH);
      digitalWrite(player2LEDs[currentLED], HIGH);
      currentLED++;
      if (currentLED >= numLEDs) currentLED = 0;
      break;

    case 1: // Blink pattern
      for (int i = 0; i < numLEDs; i++) {
        digitalWrite(player1LEDs[i], !digitalRead(player1LEDs[i]));
        digitalWrite(player2LEDs[i], !digitalRead(player2LEDs[i]));
      }
      break;

    case 2: // Alternating pattern
      for (int i = 0; i < numLEDs; i++) {
        bool state = (i % 2 == currentLED % 2);
        digitalWrite(player1LEDs[i], state);
        digitalWrite(player2LEDs[i], state);
      }
      currentLED++;
      break;

    case 3: // Random pattern
      for (int i = 0; i < numLEDs; i++) {
        digitalWrite(player1LEDs[i], random(0, 2));
        digitalWrite(player2LEDs[i], random(0, 2));
      }
      break;
  }
}
