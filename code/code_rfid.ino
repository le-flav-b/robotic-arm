//Libraries
#include <SPI.h>//https://www.arduino.cc/en/reference/SPI
#include <MFRC522.h>//https://github.com/miguelbalboa/rfid

//Constants
#define SS_PIN 5

//Variables
MFRC522 rfid(SS_PIN);

void setup()
{
  //Init Serial USB
  Serial.begin(115200);
  //init rfid
  SPI.begin();
  rfid.PCD_Init();
  
}

void loop()
{
  if (Serial.read() == -1)
    return;
  int cardID = readRFID();
  if (!cardID)
    Serial.println("No card detected.");
  else
  {
    Serial.print("Card ID: ");
    Serial.println(cardID);
  }
}

int readRFID(void)
{
  if (!rfid.PICC_IsNewCardPresent() && !rfid.PICC_IsNewCardPresent())
    return (0);
  if (!rfid.PICC_ReadCardSerial())
    return (0);
  rfid.PCD_StopCrypto1();
  return (rfid.uid.uidByte[1]);
}
