#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lab_2.h"

game_t simon;

eSystemState state = IDLE;

void main(void)
{
  DDRB = 0b00001100; // PB3, PB2 salidas, PB0 entradas
  DDRD = 0b00000011; // PD6, PD3, PD2 entradas, PD1, PD0 salidas

  MCUCR &= 0b00001010; // Flanco negativo de INT0 e INT1

  
  GIMSK |= (1 << PCIE2 ) ; // Habilitar PCIE2 (activada por PD6)
  GIMSK |= (1 << PCIE0 ) ; // Habilitar PCIE0 (activada por PB0)
  GIMSK |= (1 << INT1 ) ; // Habilitar INT1 (activada por PD3)
  GIMSK |= (1 << INT0 ) ; // Habilitar INT0 (activada por PD2)

  PCMSK |= 0b00000001;
  PCMSK2 |= 0b01000000;

  sei();
  
  while (1) {

    switch (state)
    {
    case IDLE:
      simon.iter = 4;

      break;
    
    default:
      break;
    }
  }
}

ISR( PCINT2_vect ) //SIGNAL tmb sirve pero para mi ISR tiene mas sentido
{
  while (1) {
    PORTB = 0x00; _delay_ms(500);
    PORTB = 0x08; _delay_ms(500);
  }
}

ISR( PCINT0_vect ) //SIGNAL tmb sirve pero para mi ISR tiene mas sentido
{
  while (1) {
    PORTB = 0x00; _delay_ms(500);
    PORTB = 0x08; _delay_ms(500);
  }
}

ISR( INT0_vect ) //SIGNAL tmb sirve pero para mi ISR tiene mas sentido
{
  while (1) {
    PORTB = 0x00; _delay_ms(500);
    PORTB = 0x08; _delay_ms(500);
  }
}

ISR( INT1_vect ) //SIGNAL tmb sirve pero para mi ISR tiene mas sentido
{
  while (1) {
    PORTB = 0x00; _delay_ms(500);
    PORTB = 0x08; _delay_ms(500);
  }
}
