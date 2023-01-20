#include <pic14/pic12f675.h>

#define BTN GP3 // Definir a GP3 como el boton

typedef unsigned int word;

// Desabilitar el watchdog timer y poner GP3 como I/O
word __at 0x2007 __CONFIG = (_MCLRE_OFF & _WDT_OFF);

void blink_dice (int rand);

void delay (unsigned int tiempo);

word wait = 200;

// Variables para controlar los rebotes
char BTN_pressed = 0;
char BTN_press = 0;
char BTN_release = 0;
char BTN_released = 0;
char Bouncevalue = 500;

// Variable para generar el numero aleatorio
int rand = 0;

// Funcion infinita que controla la ejecucion
void main(void)
{

    TRISIO 	=	0b00001000; //Pin [3] entrada, los demas son output
	GPIO 	&=	(0b11001000); //Poner pines de salida en bajo
	ANSEL	=	0b00001000; //Pin [3] como entrada analógica
 
    //Loop forever
    while ( 1 )
    {	
		// Generador de numeros aleatorios entre 1 y 6,
        // en cada iteración del programa se incrementa
		if (rand < 6)
        {
            rand++;
        } else {
            rand = 1;
        }

		// Checkear si GP3 es presionado
        if (BTN == 1)
        {
            // Maneja el numero de veces que el boton ha sido constante
            BTN_press++;
            BTN_release = 0;

            // El boton es constante en ser presionado
            if (BTN_press > Bouncevalue)
            {
                // Primera vez que el boton se presiona
                if (BTN_pressed == 0)
                {
                    // Se envia el valor actual del contador como numero random
					blink_dice(rand);

                    // Boton ya presionado
                    BTN_pressed = 1;
                }
                // Habilitar el dejar de presionar el boton
                BTN_press = 0;
				BTN_released = 0;
            }
        }
        else
        {
            // Maneja el numero de veces que el boton ha sido constante
            BTN_release++;
            BTN_press = 0;

            // El boton es constante en ser soltado
            if (BTN_release > Bouncevalue)
            {
				if (BTN_released == 0)
				{
                    // Se espera un corto periodo y los LEDs se apagan
					delay(wait);
					GPIO &= (0b11001000);
					BTN_released = 1;
				}
                // Habilitar el presionar el boton
                BTN_pressed = 0;
                BTN_release = 0;
            }
        }
	}
}

// Se encarga de encender los LEDs apropiados según el número
// aleatorio generado
void blink_dice(int rand)
{
	switch (rand)
	{
		case 1:
			GPIO |= (0b00000001);
			break;
		
		case 2:
			GPIO |= (0b00000010);
			break;
		
		case 3:
			GPIO |= (0b00000011);
			break;
		
		case 4:
			GPIO |= (0b00000111);
			break;
		
		case 5:
			GPIO |= (0b00010111);
			break;
		
		case 6:
			GPIO |= (0b00110111);
			break;
		
		default:
			GPIO |= (0b00000000);
			break;
	}
}

// Función simple para generar un delay
void delay(unsigned int tiempo)
{
	unsigned int i;
	unsigned int j;

	for(i=0;i<tiempo;i++)
	  for(j=0;j<1275;j++);
}
