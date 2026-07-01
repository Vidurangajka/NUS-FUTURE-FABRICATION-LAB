#include <Arduino.h>

// --- PIN CONFIGURATION ---
// Sensors (Right Side)
const int SENSOR_1 = 1; 
const int SENSOR_2 = 2;

// Coils (Left Side Neighbors)
const int C1_A = 14; const int C1_B = 13;
const int C2_A = 12; const int C2_B = 11;

// --- TIMING & AVERAGING ---
unsigned long previousMillis = 0;
const long interval = 4000; // 4-Second Report Period
long sum1 = 0;
long sum2 = 0;
long sampleCount = 0;

// --- SENSOR FUNCTION ---
// Collects one sample and adds to the 4-second accumulator
void updateSensors() {
  sum1 += analogRead(SENSOR_1);
  sum2 += analogRead(SENSOR_2);
  sampleCount++;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    float avg1 = (float)sum1 / sampleCount;
    float avg2 = (float)sum2 / sampleCount;

    Serial.print(">>> [SENSOR DATA] S1 Avg: "); Serial.print(avg1);
    Serial.print(" | S2 Avg: "); Serial.println(avg2);

    // Reset
    sum1 = 0; sum2 = 0; sampleCount = 0;
    previousMillis = currentMillis;
  }
}

// --- GENTLE COIL FUNCTION ---
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
    
    // CRITICAL: We update sensors inside the loop so we don't stop sampling
    updateSensors(); 
    
    delay(speed); 
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  // Analog Setup
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  // PWM Setup
  int allPins[] = {C1_A, C1_B, C2_A, C2_B};
  for (int p : allPins) {
    ledcAttach(p, 20000, 8); 
  }

  // Initial state: Start Opposite
  ledcWrite(C1_A, 0); ledcWrite(C1_B, 255);
  ledcWrite(C2_A, 0); ledcWrite(C2_B, 255);
  
  Serial.println("System Ready: Dual Coils Active & 4-Second Sensor Averaging Started.");
}

void loop() {
  // 1. Gently flip Coil 1 and keep sampling
  smoothFlip(C1_A, C1_B, true, 5);
  
  // Hold for 1 second, but keep sampling
  unsigned long holdStart = millis();
  while(millis() - holdStart < 1000) { updateSensors(); }

  // 2. Flip Coil 1 Back
  smoothFlip(C1_A, C1_B, false, 5);
  
  holdStart = millis();
  while(millis() - holdStart < 1000) { updateSensors(); }

  // 3. Repeat for Coil 2
  smoothFlip(C2_A, C2_B, true, 5);
  
  holdStart = millis();
  while(millis() - holdStart < 1000) { updateSensors(); }

  smoothFlip(C2_A, C2_B, false, 5);
  
  holdStart = millis();
  while(millis() - holdStart < 1000) { updateSensors(); }
}