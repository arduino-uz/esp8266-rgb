#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>

#include "index.h"
#include "result.h"

ESP8266WebServer server(80);

const int redPin = 16;
const int greenPin = 14;
const int bluePin = 12;

int currentRed = 0;
int currentGreen = 0;
int currentBlue = 0;

bool randomColor = true;
const unsigned long delayTime = 10;
// const unsigned long delayTime = 1000;

void randomic() {
  uint8_t step;
  uint8_t red = random(256);
  uint8_t green = random(256);
  uint8_t blue = random(256);

  if (abs(currentRed - red) > abs(currentGreen - green)) {
    if (abs(currentRed - red) > abs(currentBlue - blue)) {
      step = abs(currentRed - red);
    } else {
      step = abs(currentBlue - blue);
    }
  } else if (abs(currentGreen - green) > abs(currentBlue - blue)) {
    step = abs(currentGreen - green);
  } else {
    step = abs(currentBlue - blue);
  }

  for (int i = 0; i < step; i++) {
    if (currentRed > red) {
      currentRed--;
    } else if (currentRed != red) {
      currentRed++;
    }
    analogWrite(redPin, currentRed);

    if (currentGreen > green) {
      currentGreen--;
    } else if (currentGreen != green) {
      currentGreen++;
    }
    analogWrite(greenPin, currentBlue);

    if (currentBlue > blue) {
      currentBlue--;
    } else if (currentBlue != blue) {
      currentBlue++;
    }
    analogWrite(bluePin, currentBlue);
    delay(delayTime);
  }
}
void setup() {
  Serial.begin(115200);

  WiFiManager wifiManager;
  wifiManager.autoConnect("RGB");

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", htmlPage);
  });

  server.on("/set", HTTP_POST, []() {
    String redValue = server.arg("red");
    String greenValue = server.arg("green");
    String blueValue = server.arg("blue");
    int red = redValue.toInt();
    int green = greenValue.toInt();
    int blue = blueValue.toInt();
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
    currentRed = red;
    currentGreen = green;
    currentBlue = blue;
    randomColor = false;
    Serial.print("Random Colors => ");
    Serial.println(randomColor);
    server.send(200, "text/html", resultPage);
  });

  server.on("/random", HTTP_GET, []() {
    randomColor = true;
    Serial.print("Random Colors => ");
    Serial.println(randomColor);
    server.send(200, "text/html", resultPage);
  });

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
  if (randomColor) {
    randomic();
  }
}
