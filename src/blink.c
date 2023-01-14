#include <pic14/pic12f675.h>
typedef unsigned int word;
// Desabilitar el watchdog timer y poner GP3 como I/O
 word __at 0x2007 __CONFIG = (_MCLRE_OFF & _WDTE_OFF);



 
void delay (unsigned int tiempo);
 
void main(void)
{

    	TRISIO = 0b00001000; //Poner todos los pines como salidas, pero GP3 entrada (boton)
	GPIO = 0x00; //Poner pines en bajo
	ANS3 = 1;
	int random_number;
	
 
    unsigned int time = 100;
 
    //Loop forever
    while ( 1 ){
				random_number =2;
    			if (GP3 == 1){ // Si el boton es activado

    				    					
    				if (random_number > 6){
    					random_number = 1;
    				}

    				if (random_number == 2){
    					GP0 = ~GP0;
    					delay(time);
    					
    				}
    				if (random_number == 3){
    					GP0 = ~GP0;
    					GP1 = ~GP1;
    					delay(time);
    				}
    				if (random_number == 4){
    					GP0 = ~GP0;
    					GP1 = ~GP1;
    					GP2 = ~GP2;
    					delay(time);
    				}
    				if (random_number == 5){
    					GP0 = ~GP0;
    					GP1 = ~GP1;
    					GP2 = ~GP2;
    					GP4 = ~GP4;
    					delay(time);
    				}
    				    				
    				if (random_number == 6){
    					GP0 = ~GP0;
    					GP1 = ~GP1;
    					GP2 = ~GP2;
    					GP4 = ~GP4;
    					GP5 = ~GP5;
    					delay(time);
    				}
    			}
    			
    			// Si se suelta el boton, se apagaran todos los LEDs
    			if (GP3 == 0){
    				GPIO = 0x00; //Poner pines en bajo
    				delay(time);
					random_number = random_number +2
    			}
    				    		
    }
 
}

void delay(unsigned int tiempo){
	unsigned int i;
	unsigned int j;

	for(i=0;i<tiempo;i++)
	  for(j=0;j<1275;j++);
}