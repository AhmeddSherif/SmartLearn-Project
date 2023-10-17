#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_SCK  (13) //Green
#define PN532_MOSI (11) //Orange
#define PN532_MISO (12) //Yellow

#define PN532_SS   (10) //Red
#define PN532_SS_2  (9) //Brown

// ADD Scanner 1 and Scanner 2
Adafruit_PN532 ScannerOne(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
Adafruit_PN532 ScannerTwo(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS_2);

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  // Seyup and Check Scanner 1
  ScannerOne.begin();
  uint32_t versiondata = ScannerOne.getFirmwareVersion();
  if (! versiondata) {
    Serial.println("First PN53X board not found");
    while (1);
  }
  ScannerOne.SAMConfig();

  // Seyup and Check Scanner 2
  ScannerTwo.begin();
  uint32_t versiondata2 = ScannerTwo.getFirmwareVersion();
  if (! versiondata2) {
    Serial.println("Second PN53X board not found");
    while (1); 
  }
  ScannerTwo.SAMConfig();
  
  Serial.println("Waiting for Card ...");
}

void loop(void) {
  uint8_t uidOne[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidTwo[] = { 0, 0, 0, 0, 0, 0, 0 };  
  uint8_t uidLengthOne;                        
  uint8_t uidLengthTwo; 

  // Checking Scanner 1 for Card
  if (ScannerOne.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)) {
      uint8_t success = ScannerOne.readPassiveTargetID(PN532_MIFARE_ISO14443A, uidOne, &uidLengthOne);
  }

  // Checking Scanner 2 for Card
  if (ScannerTwo.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)) {
       uint8_t success2 = ScannerTwo.readPassiveTargetID(PN532_MIFARE_ISO14443A, uidTwo, &uidLengthTwo);
  }

  if (!ScannerOne.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A) && !ScannerTwo.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)) {
    // Scanner 1 and Scanner 2 NOT found
    //delay(1000);
    return;
  } else if (ScannerOne.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A) && !ScannerTwo.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)) {
    // Scanner 1 found
    Serial.print("Scanner 1: UID: ");
    ScannerOne.PrintHex(uidOne, uidLengthOne);
    if(uidOne[0] == 0x53 && uidOne[1] == 0x05 && uidOne[2] == 0x4F && uidOne[3] == 0xFA) {
        Serial.println("Yess, you got it right this is an even number");
        digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(1000);                       // wait for a second
        digitalWrite(2, LOW);
 } else {
          Serial.println("Oops, this is not an even number try again");
          digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
          delay(1000);                       // wait for a second
          digitalWrite(3, LOW);
  }
   // Serial.print("Scanner 2: UID: Not found");
  } else if (!ScannerOne.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A) && ScannerTwo.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)) {
    // Scanner 2 found
  //  Serial.print("Scanner 1: UID: Not found");
    Serial.print("Scanner 2: UID: ");
    ScannerTwo.PrintHex(uidTwo, uidLengthTwo);
    if(uidTwo[0] == 0x33 && uidTwo[1] == 0x26 && uidTwo[2] == 0x84 && uidTwo[3] == 0xFA) {
        Serial.println("Yess, you got it right this is an odd number");
        digitalWrite(2, HIGH);  
        delay(1000);
        digitalWrite(2, LOW); 
        
 } else {
          Serial.println("Oops, this is not an odd number try again");
          digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
          delay(1000);                       // wait for a second
          digitalWrite(3, LOW);
  }

  } else {
    // Scanner 1 and Scanner 2 found
    Serial.print("Scanner 1: UID: ");
    ScannerOne.PrintHex(uidOne, uidLengthOne);
    Serial.print("Scanner 2: UID: ");
    ScannerTwo.PrintHex(uidTwo, uidLengthTwo);
  }
  
 // Scanner 1: UID: 0x53 0x05 0x4F 0xFA
 // Scanner 2: UID: 0x33 0x26 0x84 0xFA



  Serial.println("----------------------------");
  delay(1000);



  
  
}
