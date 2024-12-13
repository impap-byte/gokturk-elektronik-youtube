#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

// Definitions for the RFID reader
#define BUTTON_BITMASK 0b10000000010000 // Button bitmask for wake-up
#define RST_PIN 15 // Reset pin for MFRC522
#define SS_PIN 5   // Slave Select pin for MFRC522

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Variable to check if it's the first run
RTC_DATA_ATTR bool first_run = true;

// Initialize stock index
int stocks = 0;

// WiFi credentials
const char* ssid = "ssid";
const char* password = "password";

// API URLs for stock data and percentages
const char* apiURLdardanel = "";
const char* apiURLdnisi = "";
const char* apiURLeczaci = "";
const char* apiURLsoke = "";
const char* apiURLdardanel_percentage = "";
const char* apiURLdnisi_percentage = "";
const char* apiURLeczaci_percentage = "";
const char* apiURLsoke_percentage = "";

// Initialize LCD with I2C address and dimensions
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Arrays to hold API URLs
const char* stockURLs[4];
const char* percentages[4];

// Timers for updating stocks and handling button presses
unsigned long previousTime = 0;
const long waitTime = 30000;

// Button and switch pins
const int buttonForward = 13;
const int buttonBack = 4;
const int sleepSwitch = 16;

// Variables for button states
int counter = 0;
unsigned long buttonTime = 0;
volatile bool forwardPressed = false;
volatile bool backPressed = false;

// Time before entering sleep mode after no button presses
unsigned long buttonWaitTime = 60000;

// Function prototypes
void showStock(int stock);
void backInterrupt();
void forwardInterrupt();

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // First run: RFID card authentication
  if (first_run) {
    SPI.begin();
    mfrc522.PCD_Init();
    lcd.print("   STOCKMATIC");
    lcd.setCursor(0, 1);
    lcd.print("WAITING FOR CARD");

    while (first_run) {
      // Wait for a new card to be presented
      if (!mfrc522.PICC_IsNewCardPresent()) {
        continue;
      }
      if (!mfrc522.PICC_ReadCardSerial()) {
        continue;
      }

      // Read RFID card content
      String content = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      content.toUpperCase();

      // Check if the card is authorized
      lcd.clear();
      if (content.substring(1) == "82 B5 5D 51") {
        lcd.print("    HELLO");
        lcd.setCursor(0, 1);
        lcd.print("     ERTAN");
        first_run = false;
        delay(3000);
        lcd.clear();
        break;
      } else {
        lcd.print(" UNREGISTERED");
        lcd.setCursor(0, 1);
        lcd.print("      CARD");
        delay(500);
        continue;
      }
    }
    // Shutdown RFID reader after authentication
    mfrc522.PICC_HaltA();
    mfrc522.PCD_AntennaOff();
    mfrc522.PCD_SoftPowerDown();
    SPI.end();
  }

  // Enable wakeup using button bitmask
  esp_sleep_enable_ext1_wakeup(BUTTON_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
  delay(1000);

  // Assign API URLs to arrays
  stockURLs[0] = apiURLdardanel;
  stockURLs[1] = apiURLdnisi;
  stockURLs[2] = apiURLeczaci;
  stockURLs[3] = apiURLsoke;
  percentages[0] = apiURLdardanel_percentage;
  percentages[1] = apiURLdnisi_percentage;
  percentages[2] = apiURLeczaci_percentage;
  percentages[3] = apiURLsoke_percentage;

  // Set button and switch pins
  pinMode(buttonBack, INPUT);
  pinMode(buttonForward, INPUT);
  pinMode(sleepSwitch, INPUT_PULLUP);

  // Attach interrupts for button presses
  attachInterrupt(buttonBack, backInterrupt, RISING);
  attachInterrupt(buttonForward, forwardInterrupt, RISING);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("  CONNECTING...");
  }

  // Indicate WiFi connection
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi CONNECTED!");
  lcd.setCursor(0, 1);
  lcd.print("PRESS A BUTTON");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Handle forward button press
  if (forwardPressed) {
    buttonTime = millis();
    forwardPressed = false;
    lcd.backlight();
    if (stocks < 3) {
      stocks += 1;
      showStock(stocks);
    } else {
      stocks = 0;
      showStock(stocks);
    }
  }

  // Handle back button press
  if (backPressed) {
    buttonTime = millis();
    lcd.backlight();
    backPressed = false;
    if (stocks > 0) {
      stocks -= 1;
      showStock(stocks);
    } else {
      stocks = 3;
      showStock(stocks);
    }
  }

  // Update stock data periodically
  unsigned long currentTime = millis();
  if (currentTime - previousTime >= waitTime) {
    counter++;
    previousTime = currentTime;
    HTTPClient http;
    http.begin(stockURLs[stocks]);
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
      http2.begin(percentages[stocks]);
      httpResponseCode = http2.GET();
      if (httpResponseCode > 0) {
        String response2 = http2.getString();
        response2.trim();
        if (response2.indexOf("Error parsing document, try a different parse string.") > -1) {
          lcd.print("     ");
          lcd.print("%0,00");
          http2.end();
        } else {
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
  } else if (currentTime - buttonTime >= buttonWaitTime && digitalRead(sleepSwitch)) {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    lcd.noDisplay();
    lcd.noBacklight();
    esp_deep_sleep_start();
  }
}

// Function to display stock data
void showStock(int stock) {
  HTTPClient http;
  http.begin(stockURLs[stock]);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    String response = http.getString();
    lcd.clear();
    lcd.setCursor(0, 0);
    if (stock == 0) {
      lcd.print("1-DARDANEL LIVE");
    } else if (stock == 1) {
      lcd.print("2-DNISI LIVE");
    } else if (stock == 2) {
      lcd.print("3-ECZACI LIVE");
    } else if (stock == 3) {
      lcd.print("4-SOKE UN LIVE");
    }
    lcd.setCursor(0, 1);
    lcd.print(response);
    http.end();
  }
  HTTPClient http2;
  http2.begin(percentages[stock]);
  httpResponseCode = http2.GET();
  if (httpResponseCode > 0) {
    String response2 = http2.getString();
    response2.trim();
    lcd.print("     ");
    lcd.print(response2);
    http2.end();
  }
}

// Interrupt service routine for back button
void IRAM_ATTR backInterrupt() {
  backPressed = true;
}

// Interrupt service routine for forward button
void IRAM_ATTR forwardInterrupt() {
  forwardPressed = true;
}
