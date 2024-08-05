#include <M5Unified.h>

#include <WiFi.h>
#include <WiFiClient.h>

#include <ESPping.h>

#include "config.h"

void setup() {
  auto cfg = M5.config();
  cfg.serial_baudrate = 9600;
  M5.begin(cfg);

  M5.Display.setTextSize(3);
  M5.Display.setBrightness(128);
  M5.Display.setRotation(0);

  // Wifi
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(wifi_ssid, wifi_password);
  WiFi.setSleep(false);
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    switch (count % 4) {
      case 0:
        Serial.println("|");
        M5.Display.fillRect(0, 0, 16, 16, TFT_YELLOW);
        break;
      case 1:
        Serial.println("/");
        break;
      case 2:
        M5.Display.fillRect(0, 0, 16, 16, TFT_BLACK);
        Serial.println("-");
        break;
      case 3:
        Serial.println("\\");
        break;
    }
    count++;
    if (count >= 240) reboot();  // 240 / 4 = 60sec
  }

  M5.Display.fillRect(0, 0, 16, 16, TFT_GREEN);
  Serial.println("WiFi connected!");
  delay(1000);
}

void reboot() {
  Serial.println("REBOOT!!!!!");
  for (int i = 0; i < 30; ++i) {
    M5.Display.fillRect(0, 0, 16, 16, TFT_MAGENTA);
    delay(100);
    M5.Display.fillRect(0, 0, 16, 16, TFT_BLACK);
    delay(100);
  }

  ESP.restart();
}

bool last_status = false;

void loop() {
  M5.update();

  IPAddress addr(target_addr_0, target_addr_1, target_addr_2, target_addr_3);

  M5.Display.fillRect(0, 0, 16, 16, TFT_BLUE);
  Serial.println("WiFi connected!");

  delay(500);

  M5.Display.fillRect(0, 0, 16, 16, TFT_GREEN);
  Serial.println("WiFi connected!");

  if (Ping.ping(addr, 4)) {
		Serial.println("OK");

    if (last_status == false) {
      M5.Display.clear(TFT_BLACK);
    }

    M5.Display.setTextSize(3);
    M5.Display.drawString("PING", 0, 20);

    char buf[256];

    M5.Display.setTextSize(2);
    snprintf(buf, 255, "%u.%u", target_addr_0, target_addr_1);
    M5.Display.drawString(buf, 0, 45);

    snprintf(buf, 255, ".%u.%u", target_addr_2, target_addr_3);
    M5.Display.drawString(buf, 0, 70);

    M5.Display.setTextSize(3);
    M5.Display.drawString("OK", 0, 95);

    last_status = true;
  }
	else {
		Serial.println("FAILED");

    if (last_status == true) {
      M5.Display.clear(TFT_RED);
    }

    M5.Display.setTextSize(3);
    M5.Display.drawString("PING", 0, 20);

    char buf[256];

    M5.Display.setTextSize(2);
    snprintf(buf, 255, "%u.%u", target_addr_0, target_addr_1);
    M5.Display.drawString(buf, 0, 45);

    snprintf(buf, 255, ".%u.%u", target_addr_2, target_addr_3);
    M5.Display.drawString(buf, 0, 70);

    M5.Display.setTextSize(3);
    M5.Display.drawString("NG", 0, 95);

    last_status = false;
  }

  delay(3 * 1000);
}
