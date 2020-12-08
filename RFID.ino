/*
  Example file for communicating with the NFRC522. The program prints the card data.
  Created by Eelco Rouw - Originally adapted by Grant Gibson. 
*/

// Pinout
// SDA  - 2.2
// SCK  - 1.5
// Mosi - 1.7
// Miso - 1.6
// IRQ  - NC
// GND  - GND
// RST  - 1.3
// VCC  - VCC

#include <Mfrc522.h>

// the sensor communicates using SPI, so include the library:
#include <SPI.h>

int chipSelectPin = 10;
int NRSTDP = 5;
int KhoaTu = 2;

Mfrc522 Mfrc522(chipSelectPin,NRSTDP);
unsigned char serNum[5];
unsigned char sectorKey[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
unsigned char readData[16];

void setup() 
{                
  Serial.begin(9600);                       // RFID reader SOUT pin connected to Serial RX pin at 2400bps 
  // Start the SPI library:
  SPI.begin();
  digitalWrite(chipSelectPin, LOW);
  pinMode(KhoaTu, OUTPUT);
  Mfrc522.Init();  
}

void loop()
{
  unsigned char i,tmp;
  unsigned char status;
  unsigned char str[MAX_LEN];
  unsigned char RC_size;
  unsigned char blockAddr;
  String mycardno;
  char* myarray[2]= {"6522421928102","412041779014"};
    
  status = Mfrc522.Request(PICC_REQIDL, str); 
  if (status == MI_OK)
  {
    Serial.println("Card detected");
    Serial.print(str[0],BIN);
    Serial.print(" , ");
    Serial.print(str[1],BIN);
    Serial.println(" ");
  }
  status = Mfrc522.Anticoll(str);
  memcpy(serNum, str, 5);
  if (status == MI_OK)
  {
    Serial.println("The card's number is  : ");
    Serial.print(serNum[0], DEC);
    Serial.print(" , ");
    Serial.print(serNum[1], DEC);
    Serial.print(" , ");
    Serial.print(serNum[2], DEC);
    Serial.print(" , ");
    Serial.print(serNum[3], DEC);
    Serial.print(" , ");
    Serial.print(serNum[4], DEC);
    Serial.println(" ");
    mycardno = String(serNum[0]) += String(serNum[1]) += String(serNum[2]) += String(serNum[3]) += String(serNum[4]); // Appends the content of the serNum array to give a unique card no
    Serial.println(mycardno);
    Mfrc522.SelectTag(serNum);
    status = Mfrc522.Auth(PICC_AUTHENT1A,1,sectorKey,serNum);
    if (status == MI_OK)
      Serial.println("Authenticated...\r\n");
    else
      Serial.println("Error authenticating...\r\n");   
    status = Mfrc522.ReadBlock(1, readData);
    if (status == MI_OK)
    {
      for(i=0; i<16; i++)
      {                  
        Serial.write(readData[i]);
        delay(10);
      }                  
    } 
    else 
      Serial.println("Error reading.");
    int i;
    if (mycardno == myarray[0])
    {
        digitalWrite(KhoaTu,HIGH);
        Serial.print("The 1");
    }
    else 
      if (mycardno == myarray[1])
      {
        digitalWrite(KhoaTu,LOW);
        Serial.print("The 2");
      }
    delay(1000);
    Mfrc522.Init(); 
  }
  Mfrc522.Halt();                         
}
