#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Gree.h>
#include <Wire.h>
#include <SFE_BMP180.h>
const uint16_t kIrLed = 4;
IRGreeAC ac(kIrLed);
SFE_BMP180 bmp180;
void setup() {
  ac.begin();
  delay(200);
  ac.setXFan(3) = 4;
  ac.setMode(kGreeCool);
  ac.setTemp(22);  // 16-300
  ac.setFan(false);
  ac.setLight(false);
  ac.setSleep(false);
  ac.setTurbo(false);
}
void loop() {
  double T;
  char status;
  // Now send the IR signal.
  status = bmp180.startTemperature();
  if (status != 0) {
    delay(status);
    status = bmp180.getTemperature(T);
    if (status != 0) {
      if (T > 31.45) {
        ac.on();
        ac.send();
      } else if (T < 29.2) {
        ac.off();
        ac.send();
      }

      else if (0 < 31.45 - T < 0.3 || 0 < T - 29.2 < 0.3) {
        esp_sleep_enable_timer_wakeup(15000000 / 4);
        esp_deep_sleep_start();
      }
    }
  }
  esp_sleep_enable_timer_wakeup(15000000);
  esp_deep_sleep_start();
}