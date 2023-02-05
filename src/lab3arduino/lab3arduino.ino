#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


// Defino los pines para la pantalla LCD
#define SCLK 13
#define DIN 11
#define DC 10
#define CS 9
#define RST 8

Adafruit_PCD8544 display = Adafruit_PCD8544(SCLK, DIN, DC, CS, RST);

// Declaro los pines con un nombre 
const int entrada4 = A0;
const int entrada3 = A1;
const int entrada2 = A2;
const int entrada1 = A3;
const int switchpin = 2;

void setup() {

  // Inicializar la pantalla LCD 
  display.begin();
  display.setContrast(60); // No estoy seguro si a esta pantalla le sirve el brillo asi
  display.clearDisplay();
  display.display();
  
  // Inicializar pines de entrada y salida
  pinMode(switchPin, INPUT);


  // Inicializar el puerto serial
  Serial.begin(9600);


}

void loop() {
  int estadoswitch = digitalRead(switchpin);
  int v1, v2, v3, v4;

  // Leer voltaje de los 4 canales
  v4 = analogRead(entrada4);
  v3 = analogRead(entrada3);
  v2 = analogRead(entrada2);
  v1 = analogRead(entrada1);

  // Escalar y convertir a volts
  v4 = (v4 * 5.0) / 1023.0;
  v3 = (v3 * 5.0) / 1023.0;
  v2 = (v2 * 5.0) / 1023.0;
  v1 = (v1 * 5.0) / 1023.0;
  // Cuando el switch esta bajo es en DC
    if (estadoswitch == LOW) {
    // Modo DC
}
