# WebArmbot
WebArmBot
ESP8266-Powered WiFi Car with Interactive 3-Servo Arm

Synopsis:

The ESP8266 Car with 3-Servo Arm project introduces an innovative robotic platform powered by the ESP8266 WiFi module. This project seamlessly integrates motor control for movement with a versatile three-servo arm, providing users with a unique web-based interface to interact with and control the car and its manipulative arm remotely. This synthesis of hardware and software engineering opens avenues for both educational exploration and practical applications.![webarmbot concept](https://github.com/user-attachments/assets/a82da8e6-78d2-491d-9f25-988b9a43ea1a)


Circuit Diagram:

<img width="447" alt="Circuit Diagram" src="https://github.com/SJaditya/WebArmbot/assets/100435650/bad9f783-f856-40a3-ab7a-b4bfa91c0184">

Libraries Used:
- ESP8266WiFi: Handles WiFi connectivity for the NodeMCU.
- WiFiClient: Enables communication with the WiFi network.
- ESP8266WebServer: Facilitates the creation of a web server to handle HTTP requests.
- Servo: Provides functionality for controlling servo motors.

Pin Configurations:
- Defines the pin configurations for motor control (ENA, ENB, IN_1, IN_2, IN_3, IN_4) and servo connections (SERVO_1_PIN, SERVO_2_PIN, SERVO_3_PIN).

Global Variables:
- `speedCar`: Initial motor speed for the car.
- `speed_Coeff`: Speed coefficient for turning.
- `servo1Position`, `servo2Position`, `servo3Position`: Initial positions for the three servos.

WiFi Configuration:
- Sets up the NodeMCU as an Access Point (AP) with a specified SSID (`"Prototype 1"`).

Web Server Initialization:
- Creates an instance of the `ESP8266WebServer` class on port 80.
- Defines handlers for root ("/"), control ("/control"), and login ("/login") endpoints.

Motor Control Functions:
- `stopMotors()`: Stops the car by setting motor control pins to LOW.
- `goAhead()`,` goBack()`, `goLeft()`,` goRight()`: Control the car's movement in different directions by configuring motor control pins accordingly.

Servo Control Function:
- `moveServo(int servoNumber, int position)`: Moves the specified servo (1, 2, or 3) to the specified position.

Web Interface:
- Implements a basic login mechanism for the web interface.
- Generates an HTML-based control panel with buttons and sliders for forward, backward, left, right movements, and servo control.
- Utilizes JavaScript to send asynchronous XMLHttpRequests to the server when interacting with the controls.
  ![Wifi Interface](https://github.com/user-attachments/assets/90c5b231-2bfe-48ea-b029-e60d2e7bedf2)

Handling HTTP Requests:
- `HTTP_handleRoot()`: Manages the root endpoint, serving the main control panel page.
- `HTTP_handleLogin()`: Handles login requests, authenticating users with a predefined username and password.
- `HTTP_handleControl()`: Processes control commands received through the `"/control"` endpoint, adjusting car movement, speed, and servo positions accordingly.

Loop Function:
-`Calls server.handleClient()` in the loop to continuously handle incoming HTTP requests.

This code essentially creates a web-based interface for users to control the NodeMCU car and its 3-servo arm. Users can adjust car movement, speed, and servo positions through a browser, providing a user-friendly means of interacting with the IoT device.
