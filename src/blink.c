#include <avr/io.h>
#include <util/delay.h>

#define LED_rojo      PB0
#define LED_verde     PB1
#define LED_amarillo  PB2
#define LED_azul      PB3

#define boton_rojo      PD0
#define boton_verde     PD1
#define boton_amarillo  PD2
#define boton_azul      PD3
PCMSK0 |= 0b00000001;

int nivel = 0; // Nivel actual
int patron[13];
void init() {
    // Pongo los pins de los leds como salidas
    DDRB = (1 << LED_rojo) | (1 << LED_verde) | (1 << LED_amarillo) | (1 << LED_azul);

    // Pongo los pins de los bottones como entradas
    DDRD = 0;
}

void generar_patron(){

// Genera un patron de un numero 
    for (int i = 0; i <= nivel; i++) {
        patron[i] = rand() % 4;
    }

} 

void mostrar_patron(){
  for (int i = 0; i<= nivel, i++){
    switch (patron[i]){
      case 0:
          PORTB |= (i << LED_verde);
          _delay_ms(5000);
          PORTB &= ~(1<<LED_verde);
          break;

      case 1:
          PORTB |= (i << LED_rojo);
          _delay_ms(5000);
          PORTB &= ~(1<<LED_rojo);
          break;
      case 2:
          PORTB |= (i << LED_amarillo);
          _delay_ms(5000);
          PORTB &= ~(1<<LED_amarillo);
          break;

      case 3:
          PORTB |= (i << LED_azul);
          _delay_ms(5000);
          PORTB &= ~(1<<LED_azul);
          break;




    }
  }
}

int verificacion_patron() {
    for (int i = 0; i <= level; i++) {
        switch (pattern[i]) {
            case 0:
                if (!(PIND & (1 << boton_verde))) {
                    _delay_ms(5000);
                    continue;
                }
                break;
            case 1:
                if (!(PIND & (1 << boton_rojo))) {
                    _delay_ms(5000);
                    continue;
                }
                break;
            case 2:
                if (!(PIND & (1 << boton_amarillo))) {
                    _delay_ms(5000);
                    continue;
                }
                break;
            case 3:
                if (!(PIND & (1 << boton_azul))) {
                    _delay_ms(5000);
                    continue;
                }
                break;
        }
        return 0;
    }
    return 1;
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
