# Automatic Car Gate with License Plate Recognition

## Group 2
Authors: Gabriela Ayala & Juan Lajarthe

---

## Project Name
**Automatic Car Gate with License Plate Recognition**

---

## Project Description
This project uses the Wio Terminal board to implement an automatic gate system that controls car access with license plate recognition (LPR). The system integrates several hardware components, including a Wio Terminal, an ultrasonic sensor, a servo motor, and LEDs. The Wio Terminal interacts with a Python script that performs the LPR, allowing access to pre-approved vehicles.

---

## Function Description
1. **Car Detection**:  
   - An ultrasonic sensor detects the presence of a car.
   - The **Yellow LED** signals that the system is awaiting the LPR decision.

2. **License Plate Recognition**:  
   - A Python script captures an image of the car's license plate using OCR and checks it against a database of approved vehicles.

3. **Access Decision**:  
   - **Green LED**: Lights up if the car is approved, and the gate opens (via servo motor).  
   - **Red LED**: Lights up if the car is denied, and the gate remains closed.  

4. **Emergency Scenarios**:  
   - If the plate is unidentified, the **Yellow LED** blinks. A manual override is available using the Wio Terminal joystick for approval or rejection.

5. **Alerts and Feedback**:  
   - The buzzer emits different patterns for various situations, such as cars not moving through the gate after acceptance or not leaving the detection area after rejection.  

6. **Wio Terminal Display**:  
   - Shows the current status of the system (e.g., "Waiting for car," "Car accepted").

---

## Pin Mapping

| **Component**            | **Pin**        | **Function**                                      |
|---------------------------|----------------|--------------------------------------------------|
| Ultrasonic Sensor (Trig)  | D0             | Sends a pulse to trigger the sensor              |
| Ultrasonic Sensor (Echo)  | D1             | Receives echo to calculate distance              |
| Red LED                   | D5             | Lights up when access is denied                 |
| Yellow LED                | D3             | Lights up when a car is detected                |
| Green LED                 | D4             | Lights up when access is granted                |
| Servo Motor               | D2 (PWM)       | Controls the gate mechanism                     |
| Buzzer                    | WIO_BUZZER     | Plays alert sounds                              |
| Joystick Press            | WIO_5S_PRESS   | Manual car license plate rejection              |
| Joystick Left             | WIO_5S_LEFT    | Manual car license plate acceptance             |

---

## Hardware Used
- **Wio Terminal**
- **3 LEDs** (Yellow, Green, Red)
- **Ultrasonic Sensor**
- **Servo Motor**
- **Webcam**
- **Wio Terminal Buzzer**

---

## Software Used
1. **Python**:  
   - Performs License Plate Recognition (LPR) using OCR.
2. **SQLite**:  
   - Stores a database of allowed license plates and their permitted access times.
3. **Arduino/Wio Terminal Libraries**:  
   - Libraries include `Servo`, `rpcWifi`, and `TFT_eSPI`.

---

## How the System Works

1. **Car Detection**:  
   - The ultrasonic sensor detects a car's presence and activates the **Yellow LED**.  
   - A `"car_arrived"` signal is sent to the Python script via Serial.

2. **License Plate Recognition**:  
   - The Python script processes an image of the car's license plate and checks the database for a match.  

3. **Access Granted (Green LED)**:  
   - If the plate is approved:  
     - The **Green LED** turns on, the servo motor rotates 90° to open the gate, and the system waits for the car to pass.  
     - If the car doesn't move after 5 seconds, the **Green LED** blinks, and the buzzer beeps.  
     - Once the car moves, the ultrasonic sensor resets the system after 3 seconds.

4. **Access Denied (Red LED)**:  
   - If the plate is rejected:  
     - The **Red LED** lights up, and the gate remains closed.  
     - If the car doesn't leave after 5 seconds, the **Red LED** blinks, and the buzzer emits a slow-to-fast beep pattern.  

5. **Unidentified Plate (Yellow LED Blinks)**:  
   - If the plate cannot be identified, manual intervention is required.  
   - Use the joystick on the Wio Terminal to approve or reject the car.

---

## Demo Videos
[Final Presentation and Demo Video](https://www.canva.com/design/DAGUMuFqVIk/ifCkBYHrdQoFISihAus7Iw/view)

---

## References
- [Arduino Servo Motor Documentation](https://docs.arduino.cc/learn/electronics/servo-motors/)  
- [Ultrasonic Sensor Example](https://docs.arduino.cc/built-in-examples/sensors/Ping/)  
- [OCR in Python](https://pdf.wondershare.com/ocr/extracting-text-from-image-python.html#Part1.2)  


