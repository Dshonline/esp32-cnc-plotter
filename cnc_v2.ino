// ================= LIBRARIES =================
#include <ESP32Servo.h>

// ================= PIN DEFINITIONS =================

// X axis
#define X_IN1 4
#define X_IN2 5
#define X_IN3 8
#define X_IN4 9

// Y axis
#define Y_IN1 15
#define Y_IN2 16
#define Y_IN3 17
#define Y_IN4 18

// Servo
#define SERVO_PIN  6
#define SERVO_UP   0    // degrees
#define SERVO_DOWN 20   // At what point the servo touches the plane 

// ================= SETTINGS =================
int stepDelay = 20;

const int stepSeq[4][4] = {
  {1, 0, 1, 0},
  {0, 1, 1, 0},
  {0, 1, 0, 1},
  {1, 0, 0, 1}
};

int xStep = 0;
int yStep = 0;

// ================= SERVO =================
Servo myServo;

// ================= LOW LEVEL =================
void setPins(int p1, int p2, int p3, int p4, const int s[4]) {
  digitalWrite(p1, s[0]);
  digitalWrite(p2, s[1]);
  digitalWrite(p3, s[2]);
  digitalWrite(p4, s[3]);
}

// ================= STEPPING =================
void stepX(bool forward) {
  xStep = forward ? (xStep + 1) % 4 : (xStep + 3) % 4;
  setPins(X_IN1, X_IN2, X_IN3, X_IN4, stepSeq[xStep]);
}

void stepY(bool forward) {
  yStep = forward ? (yStep + 1) % 4 : (yStep + 3) % 4;
  setPins(Y_IN1, Y_IN2, Y_IN3, Y_IN4, stepSeq[yStep]);
}

// ================= MOTOR RELEASE =================
void releaseMotors() {
  digitalWrite(X_IN1, LOW); digitalWrite(X_IN2, LOW);
  digitalWrite(X_IN3, LOW); digitalWrite(X_IN4, LOW);
  digitalWrite(Y_IN1, LOW); digitalWrite(Y_IN2, LOW);
  digitalWrite(Y_IN3, LOW); digitalWrite(Y_IN4, LOW);
}

// ================= MOTION =================
void move(char cmd, int steps) {
  for (int i = 0; i < steps; i++) {
    switch (cmd) {
      case 'F': stepY(true);  break;
      case 'B': stepY(false); break;
      case 'R': stepX(true);  break;
      case 'L': stepX(false); break;
    }
    delay(stepDelay);
  }
  releaseMotors();
}

// ================= EXECUTE SINGLE TOKEN =================
void executeToken(String token) {
  token.trim();
  if (token.length() == 0) return;

  char cmd = toupper(token[0]);
  int steps = token.substring(1).toInt();  // 0 if no number

  switch (cmd) {

    case 'U':
      myServo.write(SERVO_UP);
      Serial.println("-> Servo UP (0 deg)");
      break;

    case 'D':
      myServo.write(SERVO_DOWN);
      Serial.println("-> Servo DOWN (10 deg)");
      break;

    case 'F':
    case 'B':
    case 'L':
    case 'R':
      if (steps > 0) {
        Serial.print("-> Moving ");
        Serial.print(cmd);
        Serial.print(" ");
        Serial.print(steps);
        Serial.println(" steps");
        move(cmd, steps);
      } else {
        Serial.print("-> Missing step count for: ");
        Serial.println(cmd);
      }
      break;

    default:
      Serial.print("-> Unknown command: ");
      Serial.println(token);
      break;
  }
}

// ================= PARSE & RUN COMMAND STRING =================
void parseAndExecute(String input) {
  input.trim();
  Serial.print("Running: ");
  Serial.println(input);
  Serial.println("----------");

  int start = 0;
  while (start < input.length()) {
    // Find next space
    int spaceIdx = input.indexOf(' ', start);

    String token;
    if (spaceIdx == -1) {
      // Last token
      token = input.substring(start);
      start = input.length();
    } else {
      token = input.substring(start, spaceIdx);
      start = spaceIdx + 1;
    }

    token.trim();
    if (token.length() > 0) {
      executeToken(token);
    }
  }

  Serial.println("----------");
  Serial.println("Done.");
}

// ================= SETUP =================
void setup() {
  pinMode(X_IN1, OUTPUT); pinMode(X_IN2, OUTPUT);
  pinMode(X_IN3, OUTPUT); pinMode(X_IN4, OUTPUT);
  pinMode(Y_IN1, OUTPUT); pinMode(Y_IN2, OUTPUT);
  pinMode(Y_IN3, OUTPUT); pinMode(Y_IN4, OUTPUT);

  releaseMotors();

  myServo.attach(SERVO_PIN);
  myServo.write(SERVO_UP);

  Serial.begin(115200);
  Serial.println();
  Serial.println("ESP32-S3 CNC Jog Controller READY");
  Serial.println("Commands (space separated):");
  Serial.println("  U        -> Servo UP (0 deg)");
  Serial.println("  D        -> Servo DOWN (40 deg)");
  Serial.println("  F<steps> -> Y Forward  e.g. F300");
  Serial.println("  B<steps> -> Y Backward e.g. B300");
  Serial.println("  L<steps> -> X Left     e.g. L300");
  Serial.println("  R<steps> -> X Right    e.g. R300");
  Serial.println();
}

// ================= LOOP =================
void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() == 0) return;
    parseAndExecute(input);
  }
}
