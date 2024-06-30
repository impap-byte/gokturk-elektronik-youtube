#include "Arduino.h"
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//!!!
// STRUCTURE!!
// TRANSMITTERIN GÖNDERDİĞİYLE AYNI İÇERİĞE SAHİP OLMALI
//ALICI ARDUINO HANGİ TÜR VERİ ALACAĞINI BİLMELİ
typedef struct struct_message {
  int laser;
  bool burglar_mode;
};
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Create a struct_message called myData
struct_message myData;
int received = 0;
bool change = false;
bool first_run = true;
bool burglar_screen = true;
int start_value = 0;
int counter = 0;
// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {

  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.print("Int: ");
  Serial.println(myData.laser);
  Serial.print("Bytes received: ");
  Serial.print("Bool: ");
  Serial.println(myData.burglar_mode);
  change = true;
  received = myData.laser;
  Serial.println();
}

void setup() {
  // Initialize Serial Monitor
  if(first_run)
  {
    myData.burglar_mode = false;
    Serial.begin(115200);
    lcd.init();
    lcd.backlight();
    first_run = false;

  }
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  
}

void loop() {
  if(myData.burglar_mode)
  {
    if(burglar_screen)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  BURGLAR MODE");
      lcd.setCursor(0, 1);
      lcd.print("   ACTIVATED");
      delay(500);
      change = false;
      burglar_screen = false;
    }
    if (change)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("      ENTRY");
      lcd.setCursor(0, 1);
      lcd.print("    DETECTED");
      change = false;
    } 
    //myData.burglar_mode = false;
  }
  else if(change)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Number of Passes");
    String printnum = String(received + start_value);
    lcd.setCursor(0, 1);
    lcd.print("       ");
    lcd.print(printnum);
    change = false;
    burglar_screen = true;
  }
}