#include <ESP8266WiFi.h>
#include <espnow.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
//!!!!!
IRrecv irrecv(D1);
decode_results results;
// ALICININ MAC ADRESI
uint8_t broadcastAddress[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
unsigned long ok_button = 0xFF38C7;

int photoPin = A0;
int buzzerPin = D2;
// Structure example to send data
// Must match the receiver structuregg
struct struct_message {
  int laser;
  bool burglar_mode;
};
bool burglar_mode = false;
bool a;
bool b;
int measurement = 0;
// Create a struct_message called myData
struct_message myData;
// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success\n");
  } else {
    Serial.println("Delivery fail\n");
  }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  irrecv.enableIRIn();
  pinMode(photoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  myData.laser = 0;
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
void loop() {
  // Set values to send
  //Serial.println(measurement);
  if (irrecv.decode(&results)) {
    if (results.value == ok_button) {
      burglar_mode = !burglar_mode;
      myData.burglar_mode = burglar_mode;
      digitalWrite(buzzerPin, HIGH);
      esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
      delay(400);
      digitalWrite(buzzerPin, LOW);
    }
    irrecv.resume();
  }
  delay(600);
  if (measurement - analogRead(A0) > 100) {
    a = true;
    //Serial.println("a");
  }
  if (measurement - analogRead(A0) < -100) {
    //Serial.println("b");
    b = true;
  }
  if (a == true && b == true) {
    if (burglar_mode)
    {
      digitalWrite(buzzerPin, HIGH);
      Serial.println("Bytes transmitted: ");
      Serial.print("Boolean: ");
      Serial.println(myData.burglar_mode);
      esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
      delay(800);
      digitalWrite(buzzerPin, LOW);
    }
    a = false;
    b = false;
    delay(400);
    measurement = analogRead(A0);
    //Serial.println(myData.laser);

    if (burglar_mode == false) {
      myData.laser += 1;
      Serial.println("Bytes transmitted: ");
      Serial.print("Int: ");
      Serial.println(myData.laser);
      delay(400);
      // Allow data to get out of the UART
      esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
      delay(400);
    }
    //Serial.println("READY");
  }
  measurement = analogRead(A0);
}