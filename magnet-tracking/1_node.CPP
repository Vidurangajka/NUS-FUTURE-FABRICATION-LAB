// Define the analog pins connected to the Hall effect sensors
const int hallPin1 = A1;
const int hallPin2 = A2;
const int hallPin3 = A3;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000); 
  
  // Print the legend/labels once at startup (supported by modern Arduino IDEs)
  Serial.println("Sensor_1,Sensor_2,Sensor_3");
}

void loop() {
  // Read the raw analog values from the sensors
  int val1 = analogRead(hallPin1);
  int val2 = analogRead(hallPin2);
  int val3 = analogRead(hallPin3);

  // Print values separated by commas 
  Serial.print(val1);
  Serial.print(",");
  Serial.print(val2);
  Serial.print(",");
  Serial.println(val3); // println creates the new line indicating the end of this data point

  // A shorter delay makes the graph lines smoother and more responsive
  delay(50); 
}