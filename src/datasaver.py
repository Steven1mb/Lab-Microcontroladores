#!/usr/bin/python
import serial

ser = serial.Serial(
    port='COM6',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
    timeout=0\
    )

f = open('Predicciones.csv', 'w+') # Sobreescribe el archivo .csv

f.write("aX,aY,aZ,gX,gY,gZ\n") # Header

print("Connected to: " + ser.portstr)

line = []

while True:
    for c in ser.read():
        c=chr(c)
        if c == '\n': # Detecta un fin de linea
            str = ''.join(line)
            print(str)
            f.write(str) # Escribe la linea en el archivo .csv
            line = []
        else:
            line.append(c)
