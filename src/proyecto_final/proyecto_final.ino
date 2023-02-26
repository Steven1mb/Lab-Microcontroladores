#include "dht.h"
#include <LiquidCrystal_I2C.h>
#define soil_apin A0 // El higrómetro está conectado al PIN A0
#define dht_apin A1 // El DHT11 está conectado al PIN A1
 
dht DHT;
LiquidCrystal_I2C lcd(0x27,16,2); 

void setup(){
 
  Serial.begin(9600);
  delay(1000);// Esperar para accesar el sensor
  lcd.init();
  lcd.backlight();
  lcd.clear();
}
 
void loop(){

    // Inicio del programa 

    DHT.read11(dht_apin);
    int Moisture = analogRead(soil_apin);
    
    Serial.print("Humedad actual = ");
    Serial.println(Moisture);

    Serial.print("Temperatura actual = ");
    Serial.print(DHT.temperature);
    Serial.println("°C");

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Humedad:"); 
    lcd.setCursor(9,0);
    lcd.print(Moisture); 
    lcd.setCursor (0,1);
    lcd.print("Temp: ");
    lcd.setCursor(9,1);
    lcd.print(DHT.temperature);
    lcd.display();

    delay(3000);// Delay para accesar sensores dentro de 3 segundos
 
}//  Fin del programa