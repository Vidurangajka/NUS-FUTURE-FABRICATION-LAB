#include <CapacitiveSensor.h>

// ==========================================
// --- CONFIGURATION ---
// ==========================================
const int numSensors = 6;
const int threshold = 2000; // Adjust this while watching your Serial Plotter!
const unsigned long debounceDelay = 500; 

// ==========================================
// --- 1-TO-1 PIN MAPPING ---
// ==========================================
const int receivePins[numSensors] = {10, 2, 6, 7, 8, 3};
const int actuatePins[numSensors] = {11, 12, 13, A1, A2, A3};

// Create an array of 6 sensors. (D4 is the Send pin)
CapacitiveSensor sensors[numSensors] = {
  CapacitiveSensor(4, receivePins[0]), // Senses D10 -> Actuates D11
  CapacitiveSensor(4, receivePins[1]), // Senses D2  -> Actuates D12
  CapacitiveSensor(4, receivePins[2]), // Senses D6  -> Actuates D13
  CapacitiveSensor(4, receivePins[3]), // Senses D7  -> Actuates A1
  CapacitiveSensor(4, receivePins[4]), // Senses D8  -> Actuates A2
  CapacitiveSensor(4, receivePins[5])  // Senses D3  -> Actuates A3
};

// ==========================================
// --- STATE TRACKING ---
// ==========================================
bool states[numSensors] = {false, false, false, false, false, false}; 
unsigned long lastTouch[numSensors] = {0, 0, 0, 0, 0, 0}; 

void setup() {
  Serial.begin(9600);
  
  // Initialize all the custom actuator pins as OUTPUTs and turn them OFF
  for (int i = 0; i < numSensors; i++) {
    pinMode(actuatePins[i], OUTPUT);
    digitalWrite(actuatePins[i], LOW);
  }
  
  // Print headers for the Serial Plotter so the graph lines are labeled
  for (int i = 0; i < numSensors; i++) {
    Serial.print("D");
    Serial.print(receivePins[i]);
    if (i < numSensors - 1) {
      Serial.print(",");
    }
  }
  Serial.println();
}

void loop() {
  // Loop through all 6 sensors
  for (int i = 0; i < numSensors; i++) {
    
    // 1. Read the continuous raw analog value
    long sensorValue = sensors[i].capacitiveSensor(30);
    
    // 2. Print the raw value for the Serial Plotter
    Serial.print(sensorValue);
    if (i < numSensors - 1) {
      Serial.print(","); // Comma separation required for the plotter
    }
    
    // 3. Actuation Logic (Toggle ON/OFF based on threshold)
    if (sensorValue >= threshold && (millis() - lastTouch[i] > debounceDelay)) {
      
      states[i] = !states[i]; // Toggle the state
      
      if (states[i]) { 
        digitalWrite(actuatePins[i], HIGH); // Turn ON the mapped actuator
      } else { 
        digitalWrite(actuatePins[i], LOW);  // Turn OFF the mapped actuator
      }
      
      // Record the exact time of this touch to prevent rapid flickering
      lastTouch[i] = millis();
    }
  }
  
  // Print a new line after all 6 sensors have been processed
  // This tells the Serial Plotter to move forward one "tick" in time
  Serial.println(); 
}