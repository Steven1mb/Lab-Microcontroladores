#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lab_2.h"

#define LED_rojo      PB3
#define LED_verde     PB2
#define LED_amarillo  PD0
#define LED_azul      PD1

#define boton_rojo      PB0
#define boton_verde     PD6
#define boton_amarillo  PD2
#define boton_azul      PD3

game_t simon;

eSystemState state = IDLE;

void blink_all(int times);
void init_seq(int seed);
int get_rand(int x0);
void show_seq(void);

// Seed para generar el numero aleatorio
int seed = 0;

int x1; /*xo=seed, x1=next random number that we will generate */
int a,c,m; /*a=constant multiplier, c=increment, m=modulus */

int main(void)
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

    // Generador de numeros aleatorios entre 1 y 6,
    // en cada iteración del programa se incrementa
    if (seed < 1000000)
    {
      seed++;
    } else {
      seed = 1;
    }

    switch (state)
    {
    case IDLE:
      simon.iter = 4;
      if (simon.isr_flag == 1)
      {
        state = BLINK_2;
      }
      break;
    
    case BLINK_2:
      blink_all(2);
      state = INIT;
      break;

    case INIT:
      init_seq(seed);
      show_seq();
      state = IDLE;
      break;
    
    default:
      state = IDLE;
      break;
    }
  }
  return 0;
}

void blink_all(int times)
{
  for (int i = 0; i < times; i++)
  {
    PORTB = 0b00001100;
    PORTD = 0b00000011; _delay_ms(700);
    PORTB = 0x00;
    PORTD = 0x00; _delay_ms(700);
  }
}

void init_seq(int seed)
{
  for (int i = 0; i < simon.iter; i++)
  {
    simon.seq[i] = get_rand(seed);
  }
}

int get_rand(int x0)
{
  a = 4;
  c = 22;
  m = 1000;

  x1 = (a*x0+c) % m;
	x0 = x1;

  return x0 & 0b11;
}

void show_seq(void)
{
  for (int i = 0; i < simon.iter; i++)
  {
    switch (simon.seq[i])
    {
    case 0:
      PORTD = 0x01; _delay_ms(2000);
      break;
    
    case 1:
      PORTB = 0b1000; _delay_ms(2000);
      break;
    
    case 2:
      PORTB = 0b100; _delay_ms(2000);
      break;
    
    case 3:
      PORTD = 0x02; _delay_ms(2000);
      break;
    
    default:
      PORTB = 0b1000;
      PORTD = 0x03; _delay_ms(2000);
      break;
    }
  PORTD = 0x00;
  PORTB = 0x00;
  }
}

ISR( PCINT2_vect ) //SIGNAL tmb sirve pero para mi ISR tiene mas sentido
{
  simon.isr_flag = 1;
  simon.led = 2;
}

ISR( PCINT0_vect ) //SIGNAL tmb sirve pero para mi ISR tiene mas sentido
{
  simon.isr_flag = 1;
  simon.led = 1;
}

ISR( INT0_vect ) //SIGNAL tmb sirve pero para mi ISR tiene mas sentido
{
  simon.isr_flag = 1;
  simon.led = 0;
}

ISR( INT1_vect ) //SIGNAL tmb sirve pero para mi ISR tiene mas sentido
{
  simon.isr_flag = 1;
  simon.led = 3;
}
