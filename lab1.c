#include <pic14/pic12f675.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <xc.h>


#define LED1 GP0
#define LED2 GP1
#define LED3 GP2
#define LED4 GP3
#define LED5 GP4
#define LED6 GP5
#define boton GP3

 
void main(void)
{

    TRISIO = 0b00001000; //Poner todos los pines como salidas, pero GP3 entrada (boton)
	GPIO = 0x00; //Poner pines en bajo
 
    unsigned int time = 100;
 
    //Loop forever
    while ( 1 )
    {

		while (boton == 1) {
			delay(time);

		}
			
    }
	int dado = rand()%6+1 // Numero aleatorio del 1 al 6

	// Enciendo uno por uno la cantidad de LEDs segun el numero del dado
	for (int i=0; i<dado, i++){
		switch(i) {
			case 0:
				LED1 = 1;
				break;
			
			case 1:
				LED2 = 1;
				break;
			
			case 2:
				LED3 = 1;
				break;
			
			case 3:
				LED4 = 1;
				break;
			
			case 4:
				LED5 = 1;
				break;
			
			case 6:
				LED1 = 1;
				break;
			
		}
	}

	delay(time)

	// Cuando se suelta el boton, hago un delay para mitigar el efecto rebote
	// Y luego pongo los LEDS en 0

	if (boton == 0){
		delay(time)
	}

	LED1 = 0
	LED2 = 0
	LED3 = 0
	LED4 = 0
	LED5 = 0
	LED6 = 0
 
}

// Funcion de delay 
void delay(unsigned int tiempo)
{
	unsigned int i;
	unsigned int j;

	for(i=0;i<tiempo;i++)
	  for(j=0;j<1275;j++);

}