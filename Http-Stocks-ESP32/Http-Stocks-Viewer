
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#define BUTTON_BITMASK 0b10000000010000
#define RST_PIN 15
#define SS_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);
RTC_DATA_ATTR bool first_run = true;
int kagit = 0;
const char* ssid = "ssid";
const char* password = "password";
const char* apiURLdrd = "";
const char* apiURLdn_isi = "";
const char* apiURLeczaci = "";
const char* apiURLsoke = "";
const char* apiURLdrd_yuzde = "";
const char* apiURLdn_isi_yuzde = "";
const char* apiURLeczaci_yuzde = "";
const char* apiURLsoke_yuzde = "";

LiquidCrystal_I2C lcd(0x27, 16, 2);
const char* kagitlarim[4];
const char* yuzdeler[4];
unsigned long oncekizaman = 0;
const long beklemesuresi = 30000;

const int buton_ileri = 13;
const int buton_geri = 4;
const int sleepSwitch = 16;

int counter = 0;
unsigned long buton_zamani = 0;
volatile bool ileriPressed = false;
volatile bool geriPressed = false;
unsigned long buton_beklemesuresi = 60000;
// Function prototypes
void kagitGoruntule(int kagit);
void geriInterrupt();
void ileriInterrupt();

void setup() {
  lcd.init();
  lcd.backlight();
  if (first_run) {
    SPI.begin();
    mfrc522.PCD_Init();
    lcd.print("   BORSAMATIK");
    lcd.setCursor(0, 1);
    lcd.print("KART BEKLENIYOR");
    while (first_run) {
      if (!mfrc522.PICC_IsNewCardPresent()) {
        continue;
      }
      if (!mfrc522.PICC_ReadCardSerial()) {
        continue;
      }
      String content = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      content.toUpperCase();
      lcd.clear();
      if (content.substring(1) == "82 B5 5D 51") {
        lcd.print("    MERHABA");
        lcd.setCursor(0, 1);
        lcd.print("     ERTAN");
        first_run = false;
        delay(3000);
        lcd.clear();
        break;
      } else {
        lcd.print(" TANIMLANMAMIS");
        lcd.setCursor(0, 1);
        lcd.print("      KART");
        delay(500);
        continue;
      }
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_AntennaOff();
    mfrc522.PCD_SoftPowerDown();
    SPI.end();
  }
  esp_sleep_enable_ext1_wakeup(BUTTON_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
  delay(1000);

  kagitlarim[0] = apiURLdrd;
  kagitlarim[1] = apiURLdn_isi;
  kagitlarim[2] = apiURLeczaci;
  kagitlarim[3] = apiURLsoke;

  yuzdeler[0] = apiURLdrd_yuzde;
  yuzdeler[1] = apiURLdn_isi_yuzde;
  yuzdeler[2] = apiURLeczaci_yuzde;
  yuzdeler[3] = apiURLsoke_yuzde;
  pinMode(buton_geri, INPUT);
  pinMode(buton_ileri, INPUT);
  pinMode(sleepSwitch, INPUT_PULLUP);
  attachInterrupt(buton_geri, geriInterrupt, RISING);
  attachInterrupt(buton_ileri, ileriInterrupt, RISING);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("  CONNECTING...");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFiYA BAGLANDI!");
  lcd.setCursor(0, 1);
  lcd.print("BIR BUTONA BASIN");
  delay(2000);
  lcd.clear();
}

void loop() {
  if (ileriPressed) {
    buton_zamani = millis();
    ileriPressed = false;
    lcd.backlight();
    if (kagit < 3) {
      kagit += 1;
      kagitGoruntule(kagit);
    } else {
      kagit = 0;
      kagitGoruntule(kagit);
    }
  }

  if (geriPressed) {
    buton_zamani = millis();
    lcd.backlight();
    geriPressed = false;
    if (kagit > 0) {
      kagit -= 1;
      kagitGoruntule(kagit);
    } else {
      kagit = 3;
      kagitGoruntule(kagit);
    }
  }
  unsigned long suankiZaman = millis();
  if (suankiZaman - oncekizaman >= beklemesuresi) {
    counter++;
    oncekizaman = suankiZaman;
    HTTPClient http;
    http.begin(kagitlarim[kagit]);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String response = http.getString();
      response.trim();
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(response);
      http.end();
      HTTPClient http2;
      http2.begin(yuzdeler[kagit]);
      httpResponseCode = http2.GET();
      if (httpResponseCode > 0) {
        String response2 = http2.getString();
        response2.trim();
        if (response2.indexOf("Error parsing document, try a different parse string.") > -1)
        {
          lcd.print("     ");
          lcd.print("%0,00");
          http2.end();
        }
        else{
        lcd.print("     ");
        lcd.print(response2);
        http2.end();
        }
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  NO CONNECTION");
      }
    }
  } else if (suankiZaman - buton_zamani >= buton_beklemesuresi && digitalRead(sleepSwitch)) {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    lcd.noDisplay();
    lcd.noBacklight();
    esp_deep_sleep_start();
  }
}

void kagitGoruntule(int kagit) {
  HTTPClient http;
  http.begin(kagitlarim[kagit]);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    String response = http.getString();
    lcd.clear();
    lcd.setCursor(0, 0);
    if (kagit == 0) {
      lcd.print("1-DARDANEL CANLI");
    } else if (kagit == 1) {
      lcd.print("2-DNISI CANLI");
    } else if (kagit == 2) {
      lcd.print("3-ECZACI CANLI");
    } else if (kagit == 3) {
      lcd.print("4-SOKE UN CANLI");
    }
    lcd.setCursor(0, 1);
    lcd.print(response);
    http.end();
  }
  HTTPClient http2;
  http2.begin(yuzdeler[kagit]);
  httpResponseCode = http2.GET();
  if (httpResponseCode > 0) {
    String response2 = http2.getString();
    response2.trim();
    lcd.print("     ");
    lcd.print(response2);
    http2.end();
  }
}

void IRAM_ATTR geriInterrupt() {
  geriPressed = true;
}

void IRAM_ATTR ileriInterrupt() {
  ileriPressed = true;
}
