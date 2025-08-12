
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000

PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected() {
  Serial.println("♥ Beat Detected!");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing MAX30100...");

  if (!pox.begin()) {
    Serial.println("FAILED to initialize MAX30100. Please check wiring.");
    while (1);
  } else {
    Serial.println("MAX30100 initialized.");
  }

  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);  // Set LED current
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update();

  // Report every 1 second
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate: ");
    Serial.print(pox.getHeartRate());

    Serial.print(" bpm / SpO2: ");
    Serial.print(pox.getSpO2());
    Serial.println(" %");

    tsLastReport = millis();
  }
}
