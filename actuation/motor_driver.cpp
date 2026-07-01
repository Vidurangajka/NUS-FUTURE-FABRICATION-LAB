#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 
#define SERVOMAX  600 
#define SERVO_FREQ 50 

// Setup for all 16 channels
const int numServos = 16; 
uint8_t servoPins[numServos] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; 

// --- Timer Variables ---
unsigned long previousMillis = 0;     // Stores the last time the pattern changed
const long patternDuration = 10000;   // How long each pattern runs in milliseconds (10 seconds)
int currentPattern = 1;               // Keeps track of which pattern is currently playing

void setup() {
  Serial.begin(9600);
  Serial.println("16 Channel Pattern Timer");

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  
  delay(10);
}

void loop() {
  // 1. Check the "stopwatch" to see if it is time to switch patterns
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= patternDuration) {
    previousMillis = currentMillis; // Reset the stopwatch
    currentPattern++;               // Move to the next pattern
    
    // If we've gone past our last pattern, loop back to pattern 1
    if (currentPattern > 2) { 
      currentPattern = 1;
    }
    
    Serial.print("Switching to Pattern: ");
    Serial.println(currentPattern);
  }

  // 2. Play the pattern based on what 'currentPattern' is set to
  if (currentPattern == 1) {
    playWavePattern();
  } 
  else if (currentPattern == 2) {
    playPulsePattern();
  }
}

// ==========================================
// CUSTOM FUNCTION: PATTERN 1 (The Wave)
// ==========================================
void playWavePattern() {
  // A fast wave across all 16 channels
  for (int i = 0; i < numServos; i++) {
    pwm.setPWM(servoPins[i], 0, SERVOMAX);
    delay(50); // Fast 50ms delay
    pwm.setPWM(servoPins[i], 0, SERVOMIN);
  }
}

// ==========================================
// CUSTOM FUNCTION: PATTERN 2 (The Pulse)
// ==========================================
void playPulsePattern() {
  // Sweeps all 16 servos out and back together
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen += 5) {
    for (int i = 0; i < numServos; i++) {
      pwm.setPWM(servoPins[i], 0, pulselen);
    }
    delay(5);
  }
  
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen -= 5) {
    for (int i = 0; i < numServos; i++) {
      pwm.setPWM(servoPins[i], 0, pulselen);
    }
    delay(5);
  }
}