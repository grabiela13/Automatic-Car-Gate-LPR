//#include <rpcWiFi.h>
#include <Servo.h>

// Define a control variable for the access denial
int accessPermission = 1;
const unsigned long timeoutDuration = 10000; // 10 seconds timeout for the car to pass

// Define the pins for components on Wio Terminal
const int trigPin = D0;    // Ultrasonic sensor trigger pin
const int echoPin = D1;    // Ultrasonic sensor echo pin
const int redLED = D5;     // Red LED pin
const int yellowLED = D3;  // Yellow LED pin
const int greenLED = D4;   // Green LED pin
const int servoPin = D2;   // Servo pin (PWM)

// Define the joystick pins
const int joystickPress = WIO_5S_PRESS; // Joystick press for YES signal
const int joystickLeft = WIO_5S_LEFT;   // Joystick left for NO signal

// Define distance threshold (in cm) for detecting a car
const int carThresh = 5; // Adjust this value based on the distance

Servo gate;

// Define states for managing system flow
enum SystemState {
  WAITING_FOR_CAR,
  CAR_DETECTED,
  WAITING_FOR_BUTTON_PRESS,
  OPEN_GATE,
  WAITING_FOR_CAR_TO_PASS
};

SystemState currentState = WAITING_FOR_CAR;

void setup() {
  // Set up pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(joystickPress, INPUT);
  pinMode(joystickLeft, INPUT);

  // Attach servo motor to the PWM pin
  gate.attach(servoPin);

  // Initialize servo to closed position (0 degrees)
  gate.write(0);

  // Start Serial monitor (for debugging purposes only)
  Serial.begin(9600);

  // Connect to Wi-Fi if necessary (since we use rpcWiFi.h)
  // WiFi.begin("SSID", "PASSWORD");
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to Wi-Fi...");
  // }
}

void loop() {
  // Measure distance from the ultrasonic sensor (detecting car)
  int distance1 = getDistance(trigPin, echoPin);

  // System state machine to manage the process
  switch (currentState) {
    case WAITING_FOR_CAR:
      // Check if a car is detected
      if ((distance1 < carThresh) && (accessPermission == 1)) {
        digitalWrite(yellowLED, HIGH); // Turn on yellow LED
        currentState = CAR_DETECTED;
        Serial.println("Car detected, waiting for external signal.");
      } else if (accessPermission == 0) {
        if (distance1 > carThresh) {
          accessPermission = 1;
          Serial.println("Refused car no longer within detection range.");
          digitalWrite(redLED, LOW); // Turn off red LED
        } else {
          digitalWrite(redLED, HIGH); // Turn on red LED
          Serial.println("Access still DENIED. Please turn back now.");
        }
      } else {
        Serial.println("Car detection pending...");
        delay(1000);
      }
      break;

    case CAR_DETECTED:
      // Wait for the joystick press (YES) or joystick left (NO)
      if ((digitalRead(joystickPress) == HIGH) && (digitalRead(joystickLeft) == LOW)) {
        digitalWrite(yellowLED, LOW);   // Turn off yellow LED
        digitalWrite(greenLED, HIGH); // Turn on green LED
        currentState = OPEN_GATE;
        Serial.println("Access GRANTED. Gate opening...");
      } else if ((digitalRead(joystickPress) == LOW) && (digitalRead(joystickLeft) == HIGH)) {
        digitalWrite(yellowLED, LOW);   // Turn off yellow LED
        digitalWrite(redLED, HIGH); // Turn on red LED
        Serial.println("Access DENIED. Please turn back and give way for others in queue.");
        accessPermission = 0;
        currentState = WAITING_FOR_CAR;
      } else if (distance1 > carThresh) {
        currentState = WAITING_FOR_CAR;
        Serial.println("Car detection lost.");
        digitalWrite(yellowLED, LOW); // Turn off yellow LED
        delay(1000);
      }
      break;

    case OPEN_GATE:
      // Open the gate by turning the servo to 90 degrees
      gate.write(90);
      delay(500); // Give time for the servo to fully open
      currentState = WAITING_FOR_CAR_TO_PASS;
      Serial.println("Gate opened, waiting for car to pass.");
      break;

    case WAITING_FOR_CAR_TO_PASS:
      // Check if the car is no longer detected
      distance1 = getDistance(trigPin, echoPin); // Refresh distance1 for real-time reading

      // Close the gate if the car has moved away from the detection zone
      if (distance1 > carThresh) {
        Serial.println("Car moving forward. Closing gate in 10 seconds...");
        delay(3000); // Wait for 10 seconds to allow the car to pass fully
        gate.write(0); // Close the gate (back to 0 degrees)
        digitalWrite(greenLED, LOW); // Turn off green LED
        delay(500); // Give time for the gate to close
        currentState = WAITING_FOR_CAR; // Reset state
      } else {
        Serial.println("Car still detected. Gate remains open.");
      }
      break;
  }

  delay(100); // Small delay to avoid reading too frequently
}

// Function to measure distance using the ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
  // Send a short pulse to trigger the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the time it takes for the echo to return
  long duration = pulseIn(echoPin, HIGH);

  // Convert duration to distance (in cm)
  int distance = duration * 0.034 / 2;  // Speed of sound: 343 m/s -> 0.034 cm/us

  return distance;
}
