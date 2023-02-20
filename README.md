# Lab-Microcontroladores

Para poder compilar correctamente el código de este Laboratorio 4 y cargarlo a la tarjeta, es necesario copiar la carpeta "lab_4" que se encuentra dentro de "src" y pegarlo en la ruta: /libopencm3-examples/examples/stm32/f4/stm322f429i-discovery

Luego de esto, se debe ir a la carpeta origen de la librería: /libopencm3-examples y ejecutar make

Por último, entrar a la carpeta de este laboratorio: libopencm3-examples/examples/stm32/f4/stm322f429i-discovery/lab_4 y ejecutar el comando: make Lab_4.c.stlink-flash

Otra alternativa es simplemente entrar a la ruta dicha anteriormente y ejecutar el script automatizado mediante: sh auto_make.sh
