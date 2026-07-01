#include <CapacitiveSensor.h>

// ==========================================
// --- CONFIGURATION ---
// ==========================================
const int numSensors = 6;
const int threshold = 2000; // Adjust this based on the printed values!
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
long sensorValues[numSensors] = {0, 0, 0, 0, 0, 0}; // Array to hold the analog value of all 6 sensors

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
  // 1. Read all the continuous raw analog values into the array
  for (int i = 0; i < numSensors; i++) {
    sensorValues[i] = sensors[i].capacitiveSensor(30);
  }
  
  // 2. Serial print the values formatted for the Serial Plotter (comma separated)
  for (int i = 0; i < numSensors; i++) {
    Serial.print(sensorValues[i]);
    
    // Add a comma between the numbers
    if (i < numSensors - 1) {
      Serial.print(",");
    }
  }
  Serial.println(); // Print a new line to advance the plotter
  
  // 3. Actuation Logic (Toggle ON/OFF based on threshold)
  for (int i = 0; i < numSensors; i++) {
    if (sensorValues[i] >= threshold && (millis() - lastTouch[i] > debounceDelay)) {
      
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
}