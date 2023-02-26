#include "dht.h"
#define soil_apin A0 // El higrómetro está conectado al PIN A0
#define dht_apin A1 // El DHT11 está conectado al PIN A1
 
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
    int Moisture = analogRead(soil_apin);
    
    Serial.print("Humedad actual = ");
    Serial.println(Moisture);

    Serial.print("Temperatura actual = ");
    Serial.print(DHT.temperature);
    Serial.println("°C");
    
    delay(3000);// Delay para accesar sensores dentro de 3 segundos
 
}//  Fin del programa