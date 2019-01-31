#include <stdio.h>
const int EchoPin = 7;
const int TriggerPin = 6;
const int BuzzerPin = 9;
const int FlagPin=5;

void setup() {
   Serial.begin(9600);
   pinMode(TriggerPin, OUTPUT);
   pinMode(EchoPin, INPUT);
   pinMode(BuzzerPin, OUTPUT);
   pinMode(FlagPin,OUTPUT);
   
}
 
void loop() {
   int cm = ping(TriggerPin, EchoPin);
   
   if(cm<=15){
      digitalWrite(FlagPin,HIGH);
   } else {
      digitalWrite(FlagPin,LOW);
   }
   
   delay(50);
  

}
 
int ping(int TriggerPin, int EchoPin) {
   long duration, distanceCm;
   
   digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
   delayMicroseconds(4);
   digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
   delayMicroseconds(10);
   digitalWrite(TriggerPin, LOW);
   
   duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
   
   distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia, en cm
   return distanceCm;
}
