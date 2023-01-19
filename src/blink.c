#include <avr/io.h>
#include <util/delay.h>

#define LED_rojo      PB0
#define LED_verde     PB1
#define LED_amarillo  PB2
#define LED_azul      PB3

#define boton_rojo      PC0
#define boton_verde     PC1
#define boton_amarillo  PC2
#define boton_azul      PC3

int nivel = 0 // Nivel actual

void init() {
    // Pongo los pins de los leds como salidas
    DDRB = (1 << LED_rojo) | (1 << LED_verde) | (1 << LED_amarillo) | (1 << LED_azul);

    // Pongo los pins de los bottones como entradas
    DDRC = 0;
}
int main(void)
{


  //Parpadear
  while (1) {
    DDRB = 0x01; //Configuracion del puerto
    PORTB = 0x00; _delay_ms(5000);
    PORTB = 0x01; _delay_ms(5000);
    PORTB = 0x00; _delay_ms(5000);

    DDRB = 0x02; //Configuracion del puerto
    PORTB = 0x00; _delay_ms(5000);
    PORTB = 0x02; _delay_ms(5000);
    PORTB = 0x00; _delay_ms(5000);

    DDRB = 0x04; //Configuracion del puerto
    PORTB = 0x00; _delay_ms(5000);
    PORTB = 0x04; _delay_ms(5000);
    PORTB = 0x00; _delay_ms(5000);

    DDRB = 0x08; //Configuracion del puerto
    PORTB = 0x00; _delay_ms(5000);
    PORTB = 0x08; _delay_ms(5000);
    PORTB = 0x00; _delay_ms(5000);



  }
}
