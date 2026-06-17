#include <NewPing.h>

// Ultrasonic Sensors
#define TRIG_PIN 8
#define ECHO_PIN1 9
#define ECHO_PIN2 10
#define ECHO_PIN3 11
#define MAX_DISTANCE 200

NewPing sonar1(TRIG_PIN, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIG_PIN, ECHO_PIN2, MAX_DISTANCE);
NewPing sonar3(TRIG_PIN, ECHO_PIN3, MAX_DISTANCE);

// Stepper Motor Pins
const int dirPin1 = 4;
const int stepPin1 = 5;
const int enPin1  = 7;

const int dirPin2 = 2;
const int stepPin2 = 3;
const int enPin2  = 6;

// Microstepping & steps
#define DETECT_DISTANCE 10
#define MICROSTEPPING 32
const float STEPS_PER_REV = 200.0 * MICROSTEPPING;
#define GEAR_RATIO (64.0 / 21.0)

const long STEPS_M1_90  = (180.0 / 360.0) * STEPS_PER_REV;
const long STEPS_M2_180 = ((180.0 / 360.0) * STEPS_PER_REV) * GEAR_RATIO;
const long STEPS_M2_90  = ((90.0 / 360.0) * STEPS_PER_REV) * GEAR_RATIO;

// Speeds
const int SPEED_US = 400;          // Normal run
const int SPEED_US_FAST = 200;     // Medium fast
const int SPEED_US_SUPERFAST = 80; // Startup FASTEST speed

long posM2 = 0;
bool systemStopped = false;
#define CHECK_INTERVAL 20

bool initialCalibrationDone = false;

void setup() {
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(enPin1,  OUTPUT);

  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(enPin2,  OUTPUT);

  digitalWrite(enPin1, LOW);
  digitalWrite(enPin2, LOW);

  Serial.begin(9600);
  Serial.println("System Ready!");
}

void loop() {

  if (!initialCalibrationDone) {
    startupCalibration();  //Sensors disabled here
    initialCalibrationDone = true;
    Serial.println("Startup Calibration Completed!");

    delay(2000);  // 2 second delay added here
  }

  if(systemStopped) return;

  Serial.println("Motor1 → +90°");
  runMotor1WithCheck(STEPS_M1_90, HIGH);
  if(systemStopped) return;

  Serial.println("Motor2 → -180° (Anti-clock)");
  runMotor2WithCheck(STEPS_M2_180, LOW);
  if(systemStopped) return;

  Serial.println("Motor2 → +180° (Clock)");
  runMotor2WithCheck(STEPS_M2_180, HIGH);
  if(systemStopped) return;

  Serial.println("🔁 Loop Completed\n");
}

// Motor1 with detection
void runMotor1WithCheck(long steps, int direction) {
  digitalWrite(enPin1, LOW);
  digitalWrite(dirPin1, direction);

  for(long i = 0; i < steps; i++) {
    if(i % CHECK_INTERVAL == 0) checkDetection();
    if(systemStopped) return;
    stepMotor(stepPin1);
  }
  digitalWrite(enPin1, HIGH);
}

// Motor2 with detection
void runMotor2WithCheck(long steps, int direction) {
  digitalWrite(enPin2, LOW);
  digitalWrite(dirPin2, direction);

  for(long i = 0; i < steps; i++) {
    if(i % CHECK_INTERVAL == 0) checkDetection();
    if(systemStopped) return;

    stepMotor(stepPin2);
    posM2 += (direction == HIGH ? 1 : -1);
  }
  digitalWrite(enPin2, HIGH);
}

// Regular pulse
void stepMotor(int stepPin) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(SPEED_US);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(SPEED_US);
}

// Ultrasonic Check (OFF in startup!)
void checkDetection() {
  if (!initialCalibrationDone) return;

  int dist1 = sonar1.ping_cm();
  int dist2 = sonar2.ping_cm();
  int dist3 = sonar3.ping_cm();

  if(dist3 > 0 && dist3 <= DETECT_DISTANCE) {
    Serial.println("🚨 EMERGENCY Sensor 3 → Stop & 90° CCW");
    stopMotorsImmediately_CCW();   // ⬅️ CHANGED HERE
    systemStopped = true;
    return;
  }

  if(dist1 > 0 && dist1 <= DETECT_DISTANCE) {
    Serial.println("🚨 Sensor 1 → Home + 90° CW");
    goToAngleHome();
    systemStopped = true;
    return;
  }

  if(dist2 > 0 && dist2 <= DETECT_DISTANCE) {
    Serial.println("🚨 Sensor 2 → 180° + 90° CW");
    goToAngle180();
    systemStopped = true;
    return;
  }
}

// Emergency stop (for Sensor 1 & 2 → +90° CW)
void stopMotorsImmediately() {
  Serial.println("⛔ Immediate Stop!");
  digitalWrite(enPin1, HIGH);
  digitalWrite(enPin2, LOW);

  digitalWrite(dirPin2, HIGH); // CW
  for(long i = 0; i < STEPS_M2_90; i++) {
    stepMotor(stepPin2);
  }
  posM2 += STEPS_M2_90;

  digitalWrite(enPin2, HIGH);
  Serial.println("Motor2 +90° CW DONE — Halted");
}

// Emergency stop (Sensor 3 → 90° CCW)
void stopMotorsImmediately_CCW() {
  Serial.println("⛔ Immediate Stop! (Sensor 3 Triggered)");
  digitalWrite(enPin1, HIGH);
  digitalWrite(enPin2, LOW);

  digitalWrite(dirPin2, LOW); // 🔁 Anti-clockwise direction
  for(long i = 0; i < STEPS_M2_90; i++) {
    stepMotor(stepPin2);
  }
  posM2 -= STEPS_M2_90; // Update position accordingly

  digitalWrite(enPin2, HIGH);
  Serial.println("Motor2 -90° CCW DONE — Halted");
}

// Home + 90° CW
void goToAngleHome() {
  long stepsToHome = -posM2;
  moveMotor2(stepsToHome);
  posM2 = 0;
  delay(1000);

  moveMotor2(STEPS_M2_90);
  posM2 += STEPS_M2_90;

  Serial.println("Home +90° CW Finished — Halted");
}

// 180° → wait → +90° CW
void goToAngle180() {
  long stepsNeeded = STEPS_M2_180 - posM2;
  moveMotor2(stepsNeeded);
  posM2 = STEPS_M2_180;
  delay(1000);

  moveMotor2(STEPS_M2_90);
  posM2 += STEPS_M2_90;

  Serial.println("180° +90° CW Finished — Halted");
}

// Motor2 generic move
void moveMotor2(long steps) {
  if(steps == 0) return;

  int direction = (steps > 0 ? HIGH : LOW);
  steps = abs(steps);

  digitalWrite(enPin2, LOW);
  digitalWrite(dirPin2, direction);

  for(long i = 0; i < steps; i++) {
    stepMotor(stepPin2);
  }
  digitalWrite(enPin2, HIGH);
}

/* SUPERFAST STARTUP CALIBRATION
   → Sensors OFF
   → Both Motors 360° CW + 360° CCW at MAX SAFE SPEED
*/
void startupCalibration() {
  Serial.println("⚡ Startup → SUPERFAST 360° CW both motors");

  digitalWrite(enPin1, LOW);
  digitalWrite(enPin2, LOW);

  digitalWrite(dirPin1, HIGH);
  digitalWrite(dirPin2, HIGH);

  for(long i = 0; i < STEPS_M1_90 * 4; i++) {
    stepMotorSuperFast(stepPin1);
    stepMotorSuperFast(stepPin2);
  }

  delay(200);

  Serial.println("⚡ Startup → SUPERFAST 360° CCW both motors");

  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, LOW);

  for(long i = 0; i < STEPS_M1_90 * 4; i++) {
    stepMotorSuperFast(stepPin1);
    stepMotorSuperFast(stepPin2);
  }

  digitalWrite(enPin1, HIGH);
  digitalWrite(enPin2, HIGH);

  Serial.println("Startup Calibration DONE\n");
}

// SUPERFAST stepping
void stepMotorSuperFast(int stepPin) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(SPEED_US_SUPERFAST);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(SPEED_US_SUPERFAST);
}