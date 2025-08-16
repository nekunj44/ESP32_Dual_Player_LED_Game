#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "ESP32_Game";
const char* password = "12345678";

WebServer server(80);

// LED pins
const int player1LEDs[3] = {2, 4, 5};
const int player2LEDs[3] = {18, 19, 21};

// Scores
int player1Score = 0;
int player2Score = 0;

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>2 Player LED Game</title>";
  html += "<style>body{text-align:center;font-family:Arial;}button{padding:15px;margin:10px;font-size:18px;}</style></head><body>";
  html += "<h1>2 Player LED Game</h1>";

  // Player 1 controls
  html += "<h2>Player 1</h2>";
  for (int i = 0; i < 3; i++) {
    html += "<button onclick=\"location.href='/p1led" + String(i) + "'\">LED " + String(i+1) + "</button>";
  }
  html += "<p>Score: " + String(player1Score) + "</p>";

  // Player 2 controls
  html += "<h2>Player 2</h2>";
  for (int i = 0; i < 3; i++) {
    html += "<button onclick=\"location.href='/p2led" + String(i) + "'\">LED " + String(i+1) + "</button>";
  }
  html += "<p>Score: " + String(player2Score) + "</p>";

  // Reset button
  html += "<h2>Game Controls</h2>";
  html += "<button style='background:red;color:white' onclick=\"location.href='/reset'\">Reset</button>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleP1Led(int ledIndex) {
  digitalWrite(player1LEDs[ledIndex], !digitalRead(player1LEDs[ledIndex]));
  player1Score++;
  handleRoot();
}

void handleP2Led(int ledIndex) {
  digitalWrite(player2LEDs[ledIndex], !digitalRead(player2LEDs[ledIndex]));
  player2Score++;
  handleRoot();
}

void handleReset() {
  player1Score = 0;
  player2Score = 0;

  // Turn off all LEDs
  for (int i = 0; i < 3; i++) {
    digitalWrite(player1LEDs[i], LOW);
    digitalWrite(player2LEDs[i], LOW);
  }

  handleRoot();
}

void setup() {
  Serial.begin(115200);

  // Set LED pins as output
  for (int i = 0; i < 3; i++) {
    pinMode(player1LEDs[i], OUTPUT);
    pinMode(player2LEDs[i], OUTPUT);
  }

  WiFi.softAP(ssid, password);
  Serial.println("WiFi Access Point started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Routes
  server.on("/", handleRoot);

  server.on("/p1led0", []() { handleP1Led(0); });
  server.on("/p1led1", []() { handleP1Led(1); });
  server.on("/p1led2", []() { handleP1Led(2); });

  server.on("/p2led0", []() { handleP2Led(0); });
  server.on("/p2led1", []() { handleP2Led(1); });
  server.on("/p2led2", []() { handleP2Led(2); });

  server.on("/reset", handleReset);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
