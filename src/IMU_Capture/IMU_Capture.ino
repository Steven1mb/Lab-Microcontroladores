/*
  IMU Capture
  This example uses the on-board IMU to start reading acceleration and gyroscope
  data from on-board IMU and prints it to the Serial Monitor for one second
  when the significant motion is detected.
  You can also use the Serial Plotter to graph the data.
  The circuit:
  - Arduino Nano 33 BLE or Arduino Nano 33 BLE Sense board.
  Created by Don Coleman, Sandeep Mistry
  Modified by Dominic Pajak, Sandeep Mistry
  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>

const int numSamples = 3000; // Número de muestras deseadas

int samplesRead = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  float aX, aY, aZ, gX, gY, gZ;

  // Dar 5 segundos de margen para iniciar el script de python de toma de muestras
  delay(5000);

  // Tomar 3000 muestras
  while (samplesRead < numSamples) {
    // check if both new acceleration and gyroscope data is
    // available
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      // read the acceleration and gyroscope data
      IMU.readAcceleration(aX, aY, aZ);
      IMU.readGyroscope(gX, gY, gZ);

      // print the data in CSV format
      Serial.print(aX, 3); Serial.print(',');
      Serial.print(aY, 3); Serial.print(',');
      Serial.print(aZ, 3); Serial.print(',');
      Serial.print(gX, 3); Serial.print(',');
      Serial.print(gY, 3); Serial.print(',');
      Serial.print(gZ, 3); Serial.println();

      samplesRead++;
    }
  }
}
