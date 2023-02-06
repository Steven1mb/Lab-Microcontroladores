#!/usr/bin/python
import serial

ser = serial.Serial(
    port='COM2',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
    timeout=0\
    )

f = open('Voltmeter.csv', 'w+') # Sobreescribe el archivo .csv

f.write("Measure_mode,Channel_0,Channel_1,Channel_2,Channel_3\n")

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
