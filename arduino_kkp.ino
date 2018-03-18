#include<SPI.h>
#include<MFRC522.h>
#include<SoftwareSerial.h>
#include <Ethernet.h>
#define SS_PIN 4 
#define RST_PIN 9
#define No_Of_Card 3
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] =  "192.168.10.1";   //server
IPAddress ip(192,168,10,1);
EthernetClient client;
SoftwareSerial mySerial(8,9);     
MFRC522 rfid(SS_PIN,RST_PIN);
MFRC522::MIFARE_Key key; 
byte id[No_Of_Card][4]={
  {178, 28, 83, 233},             //tag biru
  {16, 18, 252, 164},             //RFID NO-1
  {16, 125, 249, 167}              //RFID NO-2
};
byte id_temp[3][3];
byte i;
int j=0;
void setup()
{ 
  Serial.begin(9600);
  mySerial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  for(byte i=0;i<6;i++)
  {
    key.keyByte[i]=0xFF;
  }
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  delay(1000);
  Serial.println("=====SELAMAT DATANG=====");
  Serial.println("connected");
 }
void loop()
{int m=0;
  if(!rfid.PICC_IsNewCardPresent())
  return;
  if(!rfid.PICC_ReadCardSerial())
  return;
  for(i=0;i<4;i++)
  {
   id_temp[0][i]=rfid.uid.uidByte[i]; 
             delay(50);
  }
  
   for(i=0;i<No_Of_Card;i++)
  {
          if(id[i][0]==id_temp[0][0])
          {
            if(id[i][1]==id_temp[0][1])
            {
              if(id[i][2]==id_temp[0][2])
              {
                if(id[i][3]==id_temp[0][3])
                {
                  Serial.print("\nyour card no :");
                  for(int s=0;s<4;s++)
                  {
                    Serial.print(rfid.uid.uidByte[s]);
                    Serial.print(" ");
                   
                  }
                  Serial.println("\nDATA VALID");
                  Sending_To_DB();
                  j=0;
                            
                            rfid.PICC_HaltA(); rfid.PCD_StopCrypto1();   return; 
                }
              }
            }
          }
   else
   {j++;
    if(j==No_Of_Card)
    {
      Serial.println("\nINVALID");
      Sending_To_DB();
      j=0;
    }
   }
  }
  
     // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
 }

 void Sending_To_DB()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    Serial.println("\nconnected");
    // Make a HTTP request:
    client.print("GET http://localhost/ethernet.php?allow=");     //YOUR URL
    if(j!=No_Of_Card)
    {
      client.print('1');
    }
    else
    {
      client.print('0');
    }
    
    client.print("&id=");
    for(int s=0;s<4;s++)
                  {
                    client.print(rfid.uid.uidByte[s]);
                                  
                  }
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 192.168.10.1");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  client.stop();
 }

