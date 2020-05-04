#include "BluetoothSerial.h"
#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
#include <SPI.h> //library responsible for communicating of SPI bus


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


#define SS_PIN    21
#define RST_PIN   22
#define led       13

//used in authentication
MFRC522::MIFARE_Key key;
//authentication return status code
MFRC522::StatusCode status;
// Defined pins to module RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); 


BluetoothSerial SerialBT;
String pesan="";

void setup() 
{
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  SerialBT.begin("ESP32-miniProject"); //Bluetooth device name
  pinMode(led,OUTPUT);
  
  // Init MFRC522
  mfrc522.PCD_Init(); 

  Serial.println("The device started, now you can pair it with bluetooth!");
 
}

String kondisi = "Lampu mati";

void loop() 
{ 
  if ( mfrc522.PICC_IsNewCardPresent()) 
  {
    if ( mfrc522.PICC_ReadCardSerial()) 
    {
      String UID=""; 

      for(byte i = 0; i < mfrc522.uid.size; i++) {
        String satu =  mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
        String dua = String ( mfrc522.uid.uidByte[i], HEX);
        String tampung = satu+dua;
        UID = UID+tampung;
      }

      Serial.print("ID:");
      Serial.println(UID);
      
      SerialBT.print("ID:");
      SerialBT.println(UID);
      mfrc522.PICC_HaltA();

      if(UID == " 7a ae c1 16" && kondisi == "Lampu mati"  ){
        digitalWrite(led,HIGH);
        kondisi = "Lampu nyala";
        Serial.println(kondisi);
      }else{
        digitalWrite(led,LOW);
        kondisi = "Lampu mati";
        Serial.println(kondisi);
      }    
    }
  }
}
