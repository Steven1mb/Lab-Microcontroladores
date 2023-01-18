#include <avr/io.h>
#include <util/delay.h>

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
