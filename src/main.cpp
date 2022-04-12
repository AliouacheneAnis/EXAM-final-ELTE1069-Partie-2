/*
  Titre      : Servo moteur 
  Auteur     : Anis Aliouachene
  Date       : 12/04/2022
  Description: Partie servo moteur de l'exam ELTE1069
  Version    : 0.0.1
*/


#include <Arduino.h>
#include <Wire.h>
#include<SPI.h>
#include<Servo.h>
#include <WIFIConnector_MKR1010.h>
#include<MQTTConnector.h>

// Fonction 
void receiveEvent(int byte);
char Car; 
unsigned long TempsAvant; 
const int SERVO = 9; 
int Position; 
const int LedPin = 4; 


Servo myservo;

// Setup 
void setup()
{

  wifiConnect(); 
  MQTTConnect();
	Serial.begin(9600);
  pinMode(LedPin, OUTPUT);
	Serial.println("I2C Receiver");
	Wire.begin(4);  // debut communication I2C 
	Wire.onReceive(receiveEvent);  // apeler la fonction a chaque fois on recois une donnees  par I2C 
  myservo.attach(SERVO);
}


void loop(){
  
  // Blink de LED 
  if (Position > 0)
  { 
    
    digitalWrite(LedPin, HIGH);
    if (millis() - TempsAvant > 1000)
    {
      digitalWrite(LedPin, LOW); 
      TempsAvant = millis(); 
    }
    
 
  }else
    digitalWrite(LedPin, LOW);
}

// fonction appller a chaque fois y a une donnees envoyer par I2C du la part du controleur 
void receiveEvent(int byte)      
{  
  Car = Wire.read();  // lire le caractere recu  
  
  // Condition selon le caractere recu 
  if (Car == 'A')
  {
    Position = 180;   
    myservo.write(Position); // Servo moteur vers la position 180

  }else if (Car == 'B')
  {
      Position+= 1;
      myservo.write(Position); // incrementation de 1 
      delay(20);

  } else if (Car == 'C')
  {
      Position-= 1;
      myservo.write(Position); // decrementation de 1 
      delay(20);
  }else if (Car == 'D')
  {
    Position = 0;
    myservo.write(Position); // // Servo moteur vers la position o
    delay(20);
  }
  
  // Envoi chaine MQTT sur thingsBoard
   appendPayload("Position ", Position);  
   sendPayload(); 
}
