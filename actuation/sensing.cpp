#include <CapacitiveSensor.h>

// ==========================================
// --- CONFIGURATION ---
// ==========================================
const int numSensors = 6;
const int threshold = 2000; // <-- Keep your calibrated number here
const unsigned long debounceDelay = 500; 

// ==========================================
// --- 1-TO-1 PIN MAPPING ---
// ==========================================
// Replaced D5 with D2, and D9 with D3
const int receivePins[numSensors] = {10, 2, 6, 7, 8, 3};
const int actuatePins[numSensors] = {11, 12, 13, A1, A2, A3};

// Create an array of 6 sensors.
// Pin D4 is STILL the common "Send" pin for ALL sensors.
CapacitiveSensor sensors[numSensors] = {
  CapacitiveSensor(4, receivePins[0]), // Senses D10
  CapacitiveSensor(4, receivePins[1]), // Senses D2 
  CapacitiveSensor(4, receivePins[2]), // Senses D6
  CapacitiveSensor(4, receivePins[3]), // Senses D7
  CapacitiveSensor(4, receivePins[4]), // Senses D8
  CapacitiveSensor(4, receivePins[5])  // Senses D3 
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
  
  Serial.println("===========================================");
  Serial.println("      6-Pin 1-to-1 System Ready            ");
  Serial.println("===========================================");
  Serial.println("Mapping:");
  Serial.println("D10 -> D11  |  D7 -> A1");
  Serial.println("D2  -> D12  |  D8 -> A2");
  Serial.println("D6  -> D13  |  D3 -> A3");
  Serial.println("===========================================");
}

void loop() {
  for (int i = 0; i < numSensors; i++) {
    long sensorValue = sensors[i].capacitiveSensor(30);
    
    // Check if the pin is touched AND if the debounce time has safely passed
    if (sensorValue >= threshold && (millis() - lastTouch[i] > debounceDelay)) {
      
      states[i] = !states[i]; // Toggle the state (ON/OFF)
      
      if (states[i]) { 
        // --- Turning ON ---
        digitalWrite(actuatePins[i], HIGH); 
      } else { 
        // --- Turning OFF ---
        digitalWrite(actuatePins[i], LOW); 
      }
      
      // Record the exact time of this touch to prevent flickering
      lastTouch[i] = millis();
      
      // Print exactly what happened to the Serial Monitor
      Serial.print("Touched D");
      Serial.print(receivePins[i]); 
      Serial.print("  ->  Actuated ");
      
      // Format the output print cleanly for Analog vs Digital pinsgi
      if (actuatePins[i] == A1) Serial.print("A1");
      else if (actuatePins[i] == A2) Serial.print("A2");
      else if (actuatePins[i] == A3) Serial.print("A3");
      else {
        Serial.print("D");
        Serial.print(actuatePins[i]);
      }
      
      Serial.print("  |  State: ");
      Serial.println(states[i] ? "ON" : "OFF");
    }
  }
}