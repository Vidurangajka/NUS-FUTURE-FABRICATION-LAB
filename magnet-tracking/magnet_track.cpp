#include <Arduino.h>

// --- PIN CONFIGURATION ---
// Sensors (Right Side ADC1/ADC2)
const int sensors[] = {1, 2, 42, 41, 40, 39}; 
const int numSensors = 6;

// Coils (Left Side Neighbors - Opposite Logic)
const int C1_A = 14; const int C1_B = 13;
const int C2_A = 12; const int C2_B = 11;

// --- TIMING & AVERAGING ---
unsigned long previousMillis = 0;
const long interval = 4000; // 4-Second Report Period
long sums[6] = {0, 0, 0, 0, 0, 0};
long sampleCount = 0;

// --- SENSOR FUNCTION ---
// Samples all 6 sensors and adds to their respective 4-second buffers
void updateSensors() {
  for (int i = 0; i < numSensors; i++) {
    sums[i] += analogRead(sensors[i]);
  }
  sampleCount++;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    Serial.print(">>> [4s AVG DATA] ");
    for (int i = 0; i < numSensors; i++) {
      float avg = (float)sums[i] / sampleCount;
      Serial.printf("S%d:%.1f  ", i + 1, avg);
      sums[i] = 0; // Reset buffer
    }
    Serial.println();
    sampleCount = 0;
    previousMillis = currentMillis;
  }
}

// --- GENTLE COIL FUNCTION ---
// Controls the "Opposite Logic" while keeping the sensors sampling
void smoothFlip(int pinA, int pinB, bool toAHigh, int speed) {
  int valA = ledcRead(pinA); 
  int targetA = toAHigh ? 255 : 0;
  
  Serial.printf("COIL ACTION: Flipping Pins %d/%d to %s...\n", pinA, pinB, toAHigh ? "A-High" : "B-High");

  while (valA != targetA) {
    if (valA < targetA) valA++;
    else valA--;

    int valB = 255 - valA; 
    ledcWrite(pinA, valA);
    ledcWrite(pinB, valB);
    
    updateSensors(); // Keep sampling during the transition
    delay(speed); 
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  // Analog Configuration
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  // PWM Configuration for Coils
  int allPins[] = {C1_A, C1_B, C2_A, C2_B};
  for (int p : allPins) {
    ledcAttach(p, 20000, 8); // 20kHz
  }

  // Initial Power State
  ledcWrite(C1_A, 0); ledcWrite(C1_B, 255);
  ledcWrite(C2_A, 0); ledcWrite(C2_B, 255);
  
  Serial.println("6-Sensor Monitor + Dual Coil Logic Online.");
}

void loop() {
  // 1. Action on Coil 1
  smoothFlip(C1_A, C1_B, true, 5);
  
  // Hold for 1 second while sampling
  unsigned long hold = millis();
  while(millis() - hold < 1000) { updateSensors(); }

  smoothFlip(C1_A, C1_B, false, 5);
  
  hold = millis();
  while(millis() - hold < 1000) { updateSensors(); }

  // 2. Action on Coil 2
  smoothFlip(C2_A, C2_B, true, 5);
  
  hold = millis();
  while(millis() - hold < 1000) { updateSensors(); }

  smoothFlip(C2_A, C2_B, false, 5);
  
  hold = millis();
  while(millis() - hold < 1000) { updateSensors(); }
}