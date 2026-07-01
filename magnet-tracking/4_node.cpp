// Define the total number of sensors
const int numSensors = 12;

// Define the array of analog (ADC) pins connected to your sensors.
// IMPORTANT: Update these pin numbers to match the actual analog pins on your board!
const int hallPins[numSensors] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; 

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Dynamically print the legend for the Serial Plotter
  for (int i = 0; i < numSensors; i++) {
    Serial.print("Sensor_");
    Serial.print(i + 1);
    
    // Print a comma after every label EXCEPT the last one
    if (i < numSensors - 1) {
      Serial.print(","); 
    } else {
      Serial.println(); // Print a new line after the final label
    }
  }
}

void loop() {
  // Loop through all 12 sensors, read their values, and print them
  for (int i = 0; i < numSensors; i++) {
    int val = analogRead(hallPins[i]);
    Serial.print(val);
    
    // Print a comma after every value EXCEPT the last one
    if (i < numSensors - 1) {
      Serial.print(","); 
    } else {
      Serial.println(); // Print a new line after the final value to plot the graph
    }
  }

  // A slight delay to keep the plotter responsive
  delay(50); 
}