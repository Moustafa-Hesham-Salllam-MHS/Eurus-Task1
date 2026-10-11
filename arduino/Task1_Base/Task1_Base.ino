/*
 * Eurus Task 1 BASE -- AI-assisted implementation.
 * Compiled successfully and tested in Proteus simulation.
 * No physical Arduino or MQ2 hardware was tested.
 * Proteus uses the ATMEGA328P VSM model, not a complete UNO board model.
 * A1 is a potentiometer substitute for MQ2 analog output, NOT a gas model.
 * GAS_TRIP_ADC is a demonstration threshold, NOT a calibrated ppm limit.
 */
#include <Arduino.h>

const uint8_t VOLTAGE_PIN = A0; // ATmega PC0/ADC0
const uint8_t GAS_PIN = A1;     // ATmega PC1/ADC1
const uint8_t MOTOR_PIN = 8;    // ATmega PB0 -> motor MOSFET gate resistor
const uint8_t BUZZER_PIN = 9;   // ATmega PB1 -> buzzer MOSFET gate resistor

const uint16_t VOLTAGE_TRIP_ADC = 512; // nominal 8 V with 22k/10k, 5 V reference
const uint16_t GAS_TRIP_ADC = 600;     // arbitrary demonstration setting only
const float INPUT_VOLTS_PER_COUNT = 5.0f / 1024.0f * 3.2f;

// The initial schematic specifies a GENERIC ACTIVE 5 V DC buzzer model.
// Set true only if the selected model is confirmed to be a passive sounder.
const bool BUZZER_IS_PASSIVE = false;
bool manualRequest = false;
bool lastAlarmCommand = false;
unsigned long lastTelemetryMs = 0;

void setOutputs(bool alarmCommand) {
  digitalWrite(MOTOR_PIN, alarmCommand ? HIGH : LOW);
  if (BUZZER_IS_PASSIVE) {
    if (alarmCommand && !lastAlarmCommand) tone(BUZZER_PIN, 2000);
    if (!alarmCommand) {
      noTone(BUZZER_PIN);
      digitalWrite(BUZZER_PIN, LOW);
    }
  } else {
    digitalWrite(BUZZER_PIN, alarmCommand ? HIGH : LOW);
  }
  lastAlarmCommand = alarmCommand;
}

void setup() {
  digitalWrite(MOTOR_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  analogReference(DEFAULT); // AVCC; schematic uses nominal 5 V
  Serial.begin(9600);       // 8 data bits, no parity, 1 stop bit
  Serial.println(F("#source,input_V,gas_ADC,voltageTrip,gasTrip,manualRequest,buzzerCommand,motorCommand"));
}

void loop() {
  // Proteus Virtual Terminal uses the manual serial commands: '1' = ON, '0' = OFF.
  // '1' sets manual request; '0' removes it. CR/LF and other chars ignored.
  while (Serial.available() > 0) {
    const char command = (char)Serial.read();
    if (command == '1') manualRequest = true;
    else if (command == '0') manualRequest = false;
  }

  // Discard one reading after switching channels, then retain the next.
  analogRead(VOLTAGE_PIN);
  const uint16_t voltageADC = analogRead(VOLTAGE_PIN);
  analogRead(GAS_PIN);
  const uint16_t gasADC = analogRead(GAS_PIN);
  const bool voltageTrip = voltageADC >= VOLTAGE_TRIP_ADC;
  const bool gasTrip = gasADC >= GAS_TRIP_ADC;
  const bool alarmCommand = voltageTrip || gasTrip || manualRequest;
  setOutputs(alarmCommand);

  const unsigned long now = millis();
  if (now - lastTelemetryMs >= 250UL) {
    lastTelemetryMs = now;
    Serial.print(F("SIM,"));
    Serial.print(voltageADC * INPUT_VOLTS_PER_COUNT, 3);
    Serial.print(','); Serial.print(gasADC);
    Serial.print(','); Serial.print(voltageTrip ? 1 : 0);
    Serial.print(','); Serial.print(gasTrip ? 1 : 0);
    Serial.print(','); Serial.print(manualRequest ? 1 : 0);
    Serial.print(','); Serial.print(alarmCommand ? 1 : 0);
    Serial.print(','); Serial.println(alarmCommand ? 1 : 0);
  }
  delay(10);
}
