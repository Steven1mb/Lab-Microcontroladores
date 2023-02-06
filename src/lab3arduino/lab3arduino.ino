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
const int entrada4 = A3;
const int entrada3 = A2;
const int entrada2 = A1;
const int entrada1 = A0;
const int switchpin = 2;
const int serialpin = 7;

float v1, v2, v3, v4;
char mode[20];
int estadoswitch;

void setup() {

  // Inicializar la pantalla LCD 
  display.begin();
  display.setContrast(60); // No estoy seguro si a esta pantalla le sirve el brillo asi
  display.clearDisplay();
  display.display();
  
  // Inicializar pines de entrada y salida
  pinMode(switchpin, INPUT);
  pinMode(serialpin, INPUT);
  pinMode(4, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  // Inicializar el puerto serial
  Serial.begin(9600);

}

void loop() {
  estadoswitch = digitalRead(switchpin);

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
    v4 = (v4 * 9.6) - 24; // Estas 4 señales se escalan segun
    v3 = (v3 * 9.6) - 24; // el divisor de tensiones aplicado
    v2 = (v2 * 9.6) - 24; // y se restan los 24V de la bateria
    v1 = (v1 * 9.6) - 24; // permanente para hallar valor real
    strcpy(mode,"DC");

    // Encender LEDs de alerta
    if (v1 <= -20 || v1 >= 20 ) digitalWrite(4, HIGH);
    else digitalWrite(4, LOW);
    if (v2 <= -20 || v2 >= 20 ) digitalWrite(8, HIGH);
    else digitalWrite(8, LOW);
    if (v3 <= -20 || v3 >= 20 ) digitalWrite(12, HIGH);
    else digitalWrite(12, LOW);
    if (v4 <= -20 || v4 >= 20 ) digitalWrite(13, HIGH);
    else digitalWrite(13, LOW);
  }
  else { // Modo AC
    v4 = ((v4 * 4.56) + 1.2) / 1.4142; // Estas 4 señales primero se escalan
    v3 = ((v3 * 4.56) + 1.2) / 1.4142; // segun el divisor de tensiones, luego
    v2 = ((v2 * 4.56) + 1.2) / 1.4142; // se compensa la caida de tension de
    v1 = ((v1 * 4.56) + 1.2) / 1.4142; // los 2 diodos y se dividen por raiz de 2
    strcpy(mode,"AC");

    // Encender LEDs de alerta
    if ((v1*1.4142) <= -20 || (v1*1.4142) >= 20 ) digitalWrite(4, HIGH);
    else digitalWrite(4, LOW);
    if ((v2*1.4142) <= -20 || (v2*1.4142) >= 20 ) digitalWrite(8, HIGH);
    else digitalWrite(8, LOW);
    if ((v3*1.4142) <= -20 || (v3*1.4142) >= 20 ) digitalWrite(12, HIGH);
    else digitalWrite(12, LOW);
    if ((v4*1.4142) <= -20 || (v4*1.4142) >= 20 ) digitalWrite(13, HIGH);
    else digitalWrite(13, LOW);
  }
  
  // Un switch habilita la comunicacion serial,
  // se envian los datos de los 4 canales
  if (digitalRead(serialpin)) {
    Serial.print(mode); Serial.print(",");
    Serial.print(v1); Serial.print(" V,");
    Serial.print(v2); Serial.print(" V,");
    Serial.print(v3); Serial.print(" V,");
    Serial.print(v4); Serial.println(" V");
    delay(1000);
  }
}
