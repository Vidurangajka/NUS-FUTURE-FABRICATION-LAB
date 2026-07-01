import processing.serial.*;

Serial myPort;
ArrayList<PVector> trail = new ArrayList<PVector>();
float posX = 0, posY = 0;

// Platform Dimensions
int platWidth = 100;  // -50 to +50mm
int platHeight = 80;  // -40 to +40mm
int scale = 6;        // Pixels per mm (Adjust to fit your screen)

void setup() {
  size(800, 600); // Window size
  
  // List all serial ports
  println("Available Serial Ports:");
  printArray(Serial.list());
  
  // Change [0] to the index of your ESP32 port (usually 0, 1, or 2)
  String portName = Serial.list()[0]; 
  myPort = new Serial(this, portName, 115200);
  
  // Only trigger serialEvent when a newline character is received
  myPort.bufferUntil('\n');
}

void draw() {
  background(25); // Dark background
  
  // Move coordinate system to the center of the window
  translate(width/2, height/2);
  
  // 1. Draw the Physical Platform boundary (100x80mm)
  stroke(150);
  strokeWeight(2);
  noFill();
  rectMode(CENTER);
  rect(0, 0, platWidth * scale, platHeight * scale);
  
  // 2. Draw Sensor Cluster markers for reference
  // Set 1 is at -25mm, Set 2 is at +25mm
  fill(255, 50, 50, 150); 
  noStroke();
  ellipse(-25 * scale, 0, 15, 15); // Cluster 1
  ellipse(25 * scale, 0, 15, 15);  // Cluster 2
  
  // 3. Draw the Movement Trail
  noFill();
  strokeWeight(3);
  beginShape();
  for (int i = 0; i < trail.size(); i++) {
    // Fading effect: older points are more transparent
    float alpha = map(i, 0, trail.size(), 0, 255);
    stroke(0, 255, 150, alpha); 
    PVector p = trail.get(i);
    vertex(p.x * scale, p.y * scale);
  }
  endShape();

  // 4. Draw Current Magnet Position (Yellow Dot)
  fill(255, 255, 0);
  noStroke();
  ellipse(posX * scale, posY * scale, 20, 20);
  
  // 5. Interface Text (Reset origin for text)
  resetMatrix();
  fill(255);
  textSize(16);
  text("Magnet Tracker Interface", 20, 30);
  text("Current X: " + nfc(posX, 1) + " mm", 20, 55);
  text("Current Y: " + nfc(posY, 1) + " mm", 20, 80);
  text("Grid: 10mm Resolution", 20, 105);
}

void serialEvent(Serial myPort) {
  // Read the "X,Y" string from ESP32
  String inString = myPort.readStringUntil('\n');
  
  if (inString != null) {
    inString = trim(inString);
    float[] coords = float(split(inString, ","));
    
    // Ensure we received two values
    if (coords.length >= 2) {
      posX = coords[0];
      posY = coords[1];
      
      // Add to trail and keep trail length at 60 points
      trail.add(new PVector(posX, posY));
      if (trail.size() > 60) {
        trail.remove(0);
      }
    }
  }
}