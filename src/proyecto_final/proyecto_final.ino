#include "dht.h"
#include <LiquidCrystal_I2C.h>
#define soil_apin A0 // El higrómetro está conectado al PIN A0
#define dht_apin A1 // El DHT11 está conectado al PIN A1
#define pin_agua 2 // Relay de la valvula con PIN 2
#define pin_abanico 4 // Relay del ventilador con PIN 4

unsigned long t_1 = 0;
unsigned long t_2 = 0;
float t_min = 0;
 
dht DHT;
LiquidCrystal_I2C lcd(0x27,16,2);

void regar_planta();

void setup(){
 
  Serial.begin(9600);
  delay(1000);// Esperar para accesar el sensor
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(pin_agua, OUTPUT);
  pinMode(pin_abanico, OUTPUT);
  digitalWrite(pin_agua, LOW);
  digitalWrite(pin_abanico, LOW);
}
 
void loop(){

    // Inicio del programa 

    DHT.read11(dht_apin);
    int Moisture = analogRead(soil_apin);

    t_2 = millis();
    t_min = t_2 - t_1;
    t_min = (t_min/1000)/60;
    
    Serial.print("Humedad actual = ");
    Serial.println(Moisture);

    Serial.print("Temperatura actual = ");
    Serial.print(DHT.temperature);
    Serial.println("°C");

    Serial.print("t_1: ");
    Serial.println(t_1);
    Serial.print("t_2: ");
    Serial.println(t_2);
    Serial.print("t_min: ");
    Serial.println(t_min);   
    
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
    delay(2000);

    if (t_1 != 0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Ultimo riego"); 
      lcd.setCursor (0,1);
      lcd.print("hace: ");
      lcd.print(t_min);
      lcd.print(" min");
      lcd.display();
    }

    int temperatura = DHT.temperature;

    // Enciendo el abanico cuando temperatura es alta para refrescar
    // Apago abanico cuando llega a 30 grados
    if (temperatura >= 35 ){
      digitalWrite(pin_abanico, HIGH);

    }
    else if (temperatura < 30 ){
      digitalWrite(pin_abanico, LOW);
    
    }

    // Enciendo la valvula si la humedad es baja y apago cuando alta

    if (Moisture >= 800){
      regar_planta();
    }
    else if (Moisture <= 200){
      digitalWrite(pin_agua, LOW);
    }

    delay(2000);// Delay para accesar sensores dentro de 2 segundos

}//  Fin del programa

void regar_planta(){
  digitalWrite(pin_agua, HIGH);
  t_1 = millis();
  }