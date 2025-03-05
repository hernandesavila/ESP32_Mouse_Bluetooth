#include <BleMouse.h>

#define PIN_UP 22
#define PIN_DOWN 23
#define PIN_BATERY 36
#define PIN_LED 2
#define PIN_LED_BATERY 21
#define VOLTAGE_AVERAGE_INTERATIONS 50

const float resistor1 = 5450.0;
const float resistor2 = 10000.0;
const float maxVoltage = 4.2;
const float minVoltage = 3.0;

bool restartBluetooth = false;
int lastBateryLoadPercentage = 0;

BleMouse bleMouse;

void setup() {
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_BATERY, INPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_LED_BATERY, OUTPUT);

  bleMouse.begin();
}

void loop() {
  float sumVoltage = 0;

  for (int i = 0; i < VOLTAGE_AVERAGE_INTERATIONS; i++) {
    sumVoltage += analogRead(PIN_BATERY);
  }

  float readVoltage = sumVoltage / VOLTAGE_AVERAGE_INTERATIONS;
  float bateryVoltage = readVoltage / 4095.0 * 3.3;
  float realBateryVoltage = bateryVoltage / (resistor2 / (resistor1 + resistor2));

  int bateryLoadPercentage = map(realBateryVoltage * 100, minVoltage * 100, maxVoltage * 100, 0, 100);
  bateryLoadPercentage = constrain(bateryLoadPercentage, 0, 100);

  if (realBateryVoltage <= 3.0) {
    esp_deep_sleep_start();
  } else {
    if (realBateryVoltage <= 3.4) {
      if (digitalRead(PIN_LED_BATERY) == LOW) {
        digitalWrite(PIN_LED_BATERY, HIGH);
      }
    } else {
      if (digitalRead(PIN_LED_BATERY) == HIGH) {
        digitalWrite(PIN_LED_BATERY, LOW);
      }
    }

    if (bleMouse.isConnected()) {
      restartBluetooth = true;

      if (digitalRead(PIN_LED) == LOW) {
        digitalWrite(PIN_LED, HIGH);
      }

      if (digitalRead(PIN_UP) == LOW && digitalRead(PIN_DOWN) == LOW) {
        for (int i = 0; i < 1000; i++) {
          bleMouse.move(1, 0);
        }
      } else if (digitalRead(PIN_UP) == LOW) {
        bleMouse.move(0, 0, 1);
      } else if (digitalRead(PIN_DOWN) == LOW) {
        bleMouse.move(0, 0, -1);
      }

      if (bateryLoadPercentage != lastBateryLoadPercentage) {
        lastBateryLoadPercentage = bateryLoadPercentage;

        bleMouse.setBatteryLevel((uint8_t)bateryLoadPercentage);
      }
    } else {
      if (restartBluetooth) {
        bleMouse.end();
        delay(1000);
        bleMouse.begin();

        restartBluetooth = false;
      }

      digitalWrite(PIN_LED, !digitalRead(PIN_LED));
    }
  }

  delay(100);
}