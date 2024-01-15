#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// Define pin configurations
#define ENA 14   // Enable/speed motors Right (GPIO14 - D5)
#define ENB 12   // Enable/speed motors Left (GPIO12 - D6)
#define IN_1 15   // L298N in1 motors Right (GPIO15 - D8)
#define IN_2 13   // L298N in2 motors Right (GPIO13 - D7)
#define IN_3 2    // L298N in3 motors Left (GPIO2 - D4)
#define IN_4 0    // L298N in4 motors Left (GPIO0 - D3)

#define SERVO_1_PIN 4 // GPIO4 - D2
#define SERVO_2_PIN 5 // GPIO5 - D1
#define SERVO_3_PIN 16 // GPIO16 - D0

Servo servo1; // Create a servo object for servo 1
Servo servo2; // Create a servo object for servo 2
Servo servo3; // Create a servo object for servo 3

int servo1Position = 90; // Initial position for servo 1
int servo2Position = 90; // Initial position for servo 2
int servo3Position = 90; // Initial position for servo 3


String command;      // String to store app command state.
int speedCar = 400;  // Initial motor speed (400 - 1023).
int speed_Coeff = 3; // Speed coefficient for turning.

const char *ssid = "Prototype 1"; // WiFi SSID
const char* http_username = "jmbot1";
const char* http_password = "12345678";

ESP8266WebServer server(80);

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  Serial.begin(115200);
  
  servo1.attach(SERVO_1_PIN);
  servo2.attach(SERVO_2_PIN);
  servo3.attach(SERVO_3_PIN);
  
  // Connect to WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Starting Web Server
  server.on("/", HTTP_handleRoot);
  server.on("/control", HTTP_handleControl);
  server.onNotFound(HTTP_handleRoot);
  server.on("/login", HTTP_handleLogin);
  server.begin();
}

void stopMotors() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
}

void goAhead() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar);
  delay(speedCar); // Add a small delay
  stopMotors();
}

void goBack() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar);
  delay(speedCar); // Add a small delay
  stopMotors();
}

void goLeft() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar);
  delay(speedCar); // Add a small delay
  stopMotors();
}

void goRight() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar);
  delay(speedCar); // Add a small delay
  stopMotors();
}

void moveServo(int servoNumber, int position) {
  if (servoNumber == 1) {
    servo1.write(position);
    servo1Position = position;
  } else if (servoNumber == 2) {
    servo2.write(position);
    servo2Position = position;
  } else if (servoNumber == 3) {
    servo3.write(position);
    servo3Position = position;
  }
}

void loop() {
  server.handleClient();
}

void HTTP_handleRoot() {
  if (!server.authenticate(http_username, http_password)) {
    return server.requestAuthentication();
  }
   if (!server.authenticate(http_username, http_password)) {
    String loginHtml = "<html><body>";
    loginHtml += "<h1>Login</h1>";
    loginHtml += "<form action='/login' method='POST'>";
    loginHtml += "Username: <input type='text' name='username'><br>";
    loginHtml += "Password: <input type='password' name='password'><br>";
    loginHtml += "<input type='submit' value='Login'>";
    loginHtml += "</form>";
    loginHtml += "</body></html>";
    server.send(401, "text/html", loginHtml);
    return;
  }
  String html = "<html><body>";
  html += "<h1> Control Pannel</h1>";
  html += "<style>";
  html += "body {";
  html += "  font-family: Arial, sans-serif;";
  html += "  text-align: center;";
  html += "}";
  html += "h1 {";
  html += "  color: #333;";
  html += "}";
  html += "button {";
  html += "  background-color: #4CAF50;";
  html += "  color: white;";
  html += "  padding: 10px 24px;";
  html += "  font-size: 16px;";
  html += "  border: none;";
  html += "  cursor: pointer;";
  html += "  margin: 5px;";
  html += " border-radius: 8px;";
  html += "}";
  html += "#controlForm {";
  html += "  display: flex;";
  html += "  flex-direction: column;";
  html += "  align-items: center;";
  html += "}";
  html += ".slider-container {";
  html += "  display: flex;";
  html += "  align-items: center;";
  html += "  margin-bottom: 10px;";
  html += "}";
  html += ".slider-label {";
  html += "  width: 100px;";
  html += "  text-align: right;";
  html += "  margin-right: 10px;";
  html += "}";
  html += ".slider-input {";
  html += "  width: 80%;";
  html += "}";
  html += "#responseDiv {";
  html += "  margin-top: 20px;";
  html += "  font-weight: bold;";
  html += "}";
  html += ".button-row {";
  html += "  display: flex;";
  html += "  justify-content: space-between;";
  html += "}";
  html += "</style>";
  html += "</head><body>";
  
  html += "<form id='controlForm'>";
  html += "<button id='forward' type='button' onclick='sendCommand(\"F\")'>Forward</button><br>";
  html += "<div class='button-row'>";
  html += "<button id='left' type='button' onclick='sendCommand(\"L\")'>Left</button>";
  html += "<button id='right' type='button' onclick='sendCommand(\"R\")'>Right</button>";
  html += "</div>";
  html += "<button id='backward' type='button' onclick='sendCommand(\"B\")'>Backward</button><br>";
// Servo 1 slider
  html += "<div class='slider-container'>";
  html += "<label class='slider-label'>Servo 1:</label>";
  html += "<input type='range' class='slider-input' id='servo1Slider' min='0' max='180' value='" + String(servo1Position) + "' oninput='updateServo(1, this.value)'>";
  html += "<span id='servo1Value'>" + String(servo1Position) + "</span>";
  html += "</div>";

  // Servo 2 slider
  html += "<div class='slider-container'>";
  html += "<label class='slider-label'>Servo 2:</label>";
  html += "<input type='range' class='slider-input' id='servo2Slider' min='0' max='180' value='" + String(servo2Position) + "' oninput='updateServo(2, this.value)'>";
  html += "<span id='servo2Value'>" + String(servo2Position) + "</span>";
  html += "</div>";

  // Servo 3 slider
  html += "<div class='slider-container'>";
  html += "<label class='slider-label'>Servo 3:</label>";
  html += "<input type='range' class='slider-input' id='servo3Slider' min='0' max='180' value='" + String(servo3Position) + "' oninput='updateServo(3, this.value)'>";
  html += "<span id='servo3Value'>" + String(servo3Position) + "</span>";
  html += "</div>";

  html += "<button id='stop' type='button' onclick='sendCommand(\"S\")'>Stop</button><br>";
  html += "</form>";
  // Car speed slider
  html += "<div class='slider-container'>";
  html += "<label class='slider-label'>Car Speed:</label>";
  html += "<input type='range' class='slider-input' id='carSpeedSlider' min='100' max='800' step='10' value='" + String(speedCar) + "' oninput='updateCarSpeed(this.value)'>";
  html += "<span id='carSpeedValue'>" + String(speedCar) + "</span>";
  html += "</div>";
  
  // Add a div to display responses
  html += "<div id='responseDiv'></div>";
  html += "<script>";
  html += "function sendCommand(command) {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.onreadystatechange = function() {";
  html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
  html += "      document.getElementById('responseDiv').innerHTML = xhr.responseText;";
  html += "    }";
  html += "  };";
  html += "  xhr.open('GET', '/control?command=' + command, true);";
  html += "  xhr.send();";
  html += "}";
  html += "function updateServo(servoNumber, value) {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.onreadystatechange = function() {";
  html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
  html += "      document.getElementById('responseDiv').innerHTML = xhr.responseText;";
  html += "    }";
  html += "  };";
  html += "  xhr.open('GET', '/control?command=S' + servoNumber + value, true);";
  html += "  xhr.send();";
  html += "  document.getElementById('servo' + servoNumber + 'Value').innerHTML = value;";
  html += "}";
  html += "function updateCarSpeed(speed) {";
  html += "  var speedCommand = speed.toString();";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.onreadystatechange = function() {";
  html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
  html += "      document.getElementById('responseDiv').innerHTML = xhr.responseText;";
  html += "    }";
  html += "  };";
  html += "  xhr.open('GET', '/control?command=' + speedCommand, true);";
  html += "  xhr.send();";
  html += "  document.getElementById('carSpeedValue').innerHTML = speed;";
  //html += " speedCar = parseInt(speed);";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

//login fuction
void HTTP_handleLogin() {
  if (server.hasArg("username") && server.hasArg("password")) {
    if (server.arg("username") == http_username && server.arg("password") == http_password) {
      server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
      server.sendHeader("Location", "/");
      server.send(302, "text/plain", "");
      return;
    }
  }
  server.send(403, "text/plain", "Authentication Failed");
}

void HTTP_handleControl() {
  String command = server.arg("command");
  String response = "";

  if (command == "F")
    goAhead();
  else if (command == "B")
    goBack();
  else if (command == "L")
    goLeft();
  else if (command == "R")
    goRight();
  else if (command >= "100" && command <= "800")  {
     speedCar = map(command.toInt(), 100, 800, 400, 1023);}
  else if (command.startsWith("S")) {
    int servoNumber = command.substring(1, 2).toInt(); // Extract servo number
    int position = command.substring(2).toInt(); // Extract servo position
    moveServo(servoNumber, position);
    response = "Servo " + String(servoNumber) + " position set to " + String(position);
  } else if (command == "S")
    stopMotors();
  
  server.send(200, "text/plain", response);
}
