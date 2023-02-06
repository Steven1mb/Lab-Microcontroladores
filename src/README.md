# Lab-Microcontroladores
# Lab_3

Para que el archivo de simulación .simu funcione correctamente, es necesario obtener el .hex a partir del programa Arduino IDE.

Las pruebas de este laboratorio se realizaron en la plataforma Windows 10 con SimulIDE-0.4.15-SR10.

La conexión serial del programa fue realizada entre COM1 y COM2, para que la transacción de datos sea exitosa se debe correr primero el archivo datasaver.py, luego de esto se debe correr el archivo lab_3.simu y dentro de él: 1. Verificar que Uart1 está configurada hacia COM2. 2. Abrir Uart1. 3. Si se desea comenzar a recibir datos del Arduino, habilitar el switch conectado al pin 7.

El modo de operación puede alternar entre DC y AC al mover el switch que se conectado a los pines analógicos Ax y al pin 2.
