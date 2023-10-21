#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

// Define pin constants for PN532
#define PN532_SCK   13 // Green
#define PN532_MOSI  11 // Orange
#define PN532_MISO  12 // Yellow
#define PN532_SS    10 // Red
#define PN532_SS_2   9 // Brown
#define PN532_SS_3   8 // Green

// Define LED pins
#define GREEN_LED 2
#define RED_LED   3

// Create an array of scanners
Adafruit_PN532 scanners[] = {
  Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS),
  Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS_2),
  Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS_3)
};



void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for serial to initialize

  // Initialize LED pins
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Setup and check each scanner
  for (int i = 0; i < 3; i++) {
    scanners[i].begin();
    uint32_t versiondata = scanners[i].getFirmwareVersion();
    if (!versiondata) {
      Serial.print("Scanner ");
      Serial.print(i + 1);
      Serial.println(": Not found");
      while (1); // Halt program
    }
    scanners[i].SAMConfig();
  }

  Serial.println("Waiting for Card ...");
}

void handleCard(int scannerIndex, uint8_t uid[], uint8_t uidLength) {
  Serial.print("Scanner ");
  Serial.print(scannerIndex + 1);
  Serial.print(": UID: ");
  scanners[scannerIndex].PrintHex(uid, uidLength);

  bool isBlank = (uid[0] == 0x00 && uid[1] == 0x00 && uid[2] == 0x00 && uid[3] == 0x00);

 // Scanner 1 Tag: UID: 0x53 0x05 0x4F 0xFA
 // Scanner 1 Card: UID: 0x53 0x72 0xFA 0xAB
 
 // Scanner 2 Tag: UID: 0x33 0x26 0x84 0xFA
 // Scanner 2 Card: UID: 0xA3 0xA5 0xE4 0xAB
 
 // Scanner 3 Tag: UID: 0xB3 0x75 0x39 0xFA
 // Scanner 3 Card: UID: 0xD3 0x85 0x03 0xAC

  bool Cards[] = {
   ((uid[0] == 0x53 && uid[1] == 0x05 && uid[2] == 0x4F && uid[3] == 0xFA) ||
                 (uid[0] == 0x53 && uid[1] == 0x72 && uid[2] == 0xFA && uid[3] == 0xAB)),
                 
   ((uid[0] == 0x33 && uid[1] == 0x26 && uid[2] == 0x84 && uid[3] == 0xFA) ||
                 (uid[0] == 0xA3 && uid[1] == 0xA5 && uid[2] == 0xE4 && uid[3] == 0xAB)),
                 
   ((uid[0] == 0xB3 && uid[1] == 0x75 && uid[2] == 0x39 && uid[3] == 0xFA) ||
                 (uid[0] == 0xD3 && uid[1] == 0x85 && uid[2] == 0x03 && uid[3] == 0xAC)),
  };

    if (Cards[scannerIndex]) {
    Serial.println(" - This is an even number");
    digitalWrite(GREEN_LED, HIGH);
  } else {
      if(!isBlank)
        Serial.println(" - This is not an even number; try again"),
        digitalWrite(RED_LED, HIGH);
  }
  
  delay(1000);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void loop() {
  for (int i = 0; i < 3; i++) {
    uint8_t uid[7];
    uint8_t uidLength;

    if (scanners[i].startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)) {
      uint8_t success = scanners[i].readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
      if (success) {
        handleCard(i, uid, uidLength);
      }
    }
  }
}