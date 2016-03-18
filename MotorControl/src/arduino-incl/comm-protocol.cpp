#include "comm-protocol.h"
#include "Arduino.h"

void CommProtocol::blinkIndicator() {
    if (!DEBUG) return;
    digitalWrite(LED_INDICATOR, HIGH);
    delay(500);
    digitalWrite(LED_INDICATOR, LOW);
}

void CommProtocol::indicateError() {
    const int NUM_BLINKS = 5;
    const int BLINK_TIME = 100;
  
    for (int i = 0; i < NUM_BLINKS; ++i) {
      digitalWrite(LED_INDICATOR, HIGH);
      delay(BLINK_TIME);
      digitalWrite(LED_INDICATOR, LOW);
      delay(BLINK_TIME);
    }
}