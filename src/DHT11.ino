#include "dht.h"
#define dht_apin A1 // El sensor esta conectado al PIN A1
 
dht DHT;
 
void setup(){
 
  Serial.begin(9600);
  delay(500);// Delay para dejar que el sistema se inicie
  Serial.println("Sensor DHT11 de temperatura y humedad \n\n");
  delay(1000);// Esperar para accesar el sensor
}//end "setup()"
 
void loop(){

    // Inicio del programa 

    DHT.read11(dht_apin);
    
    Serial.print("Humedad actual = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("Temperatura actual = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
    
    delay(5000);// Delay para accesar el sensor dentro de 5 segundos
 
 
}//  Fin del programa