#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Your_wifi";
const char* password = "WifiPassword";

WebServer server(80);
const int ledPin = 19;
bool ledState = false;

void handleRoot() {
  String bg = ledState ? "#fff9e6" : "#1a1a2e";
  String textColor = ledState ? "#333" : "#aaa";
  String bulbColor = ledState ? "#FFD700" : "#444";
  String glowStyle = ledState ? "filter: drop-shadow(0 0 30px #FFD700) drop-shadow(0 0 60px #FFA500);" : "";
  String statusText = ledState ? "Light is ON ✨" : "Light is OFF 🌙";
  String btnOnStyle = ledState ? "background:#FFD700;color:#333;box-shadow:0 0 20px #FFD700;" : "background:#333;color:#888;";
  String btnOffStyle = ledState ? "background:#eee;color:#333;" : "background:#ff4757;color:white;box-shadow:0 0 20px #ff4757;";
  String bgGlow = ledState ? "background: radial-gradient(circle at 50% 40%, #fff9c4, #ffe082, #fff9e6);" : "background: radial-gradient(circle at 50% 40%, #1a1a2e, #0d0d1a);";

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Smart Light</title>";
  html += "<style>";
  html += "* { margin:0; padding:0; box-sizing:border-box; }";
  html += "body { font-family:Arial; min-height:100vh; display:flex; flex-direction:column;";
  html += "align-items:center; justify-content:center; transition: all 0.6s ease;" + bgGlow + "}";
  html += "h1 { font-size:1.8em; margin-bottom:10px; color:" + textColor + "; transition: color 0.6s; }";
  html += ".bulb { font-size:120px; margin:20px; transition: all 0.6s ease;" + glowStyle + "}";
  html += ".status { font-size:1.2em; color:" + textColor + "; margin-bottom:30px; transition: color 0.6s; }";
  html += ".btn { display:inline-block; padding:18px 45px; font-size:1.2em; border:none;";
  html += "border-radius:50px; cursor:pointer; text-decoration:none; margin:10px;";
  html += "transition: all 0.3s ease; font-weight:bold; }";
  html += ".on { " + btnOnStyle + " }";
  html += ".off { " + btnOffStyle + " }";
  html += ".stars { position:fixed; top:0; left:0; width:100%; height:100%; pointer-events:none;";
  html += "opacity:" + String(ledState ? "0" : "1") + "; transition: opacity 0.6s; }";
  html += "</style></head><body>";

  html += "<div class='stars'>⭐</div>";

  html += "<h1>Smart Light Control</h1>";
  html += "<div class='bulb'>" + String(ledState ? "💡" : "🌑") + "</div>";
  html += "<p class='status'>" + statusText + "</p>";
  html += "<div>";
  html += "<a href='/on' class='btn on'>Turn ON ☀️</a>";
  html += "<a href='/off' class='btn off'>Turn OFF 🌙</a>";
  html += "</div></body></html>";

  server.send(200, "text/html", html);
}

void handleOn() {
  ledState = true;
  digitalWrite(ledPin, HIGH);
  handleRoot();
}

void handleOff() {
  ledState = false;
  digitalWrite(ledPin, LOW);
  handleRoot();
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  pinMode(ledPin, OUTPUT);

  Serial.println("=== ESP32 Starting ===");
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 30) {
      Serial.println("\nERROR: Could not connect!");
      return;
    }
  }

  Serial.println("\nWiFi Connected!");
  Serial.println("==============================");
  Serial.print("Open this on your phone:  http://");
  Serial.println(WiFi.localIP());
  Serial.println("==============================");

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
}

void loop() {
  server.handleClient();
}