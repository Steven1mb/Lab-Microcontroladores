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

#define POLY_MASK_32 0XB4BCD35C
#define POLY_MASK_31 0X7A5BC2E3

word lfsr32, lfsr31, curr_iter, ignore = 1;

game_t simon;

eSystemState state = IDLE;

void blink_all(int times);
void init_seq(int iter);
void show_seq(int iter);
int shift_lfsr(word *lfsr, word polynomial_mask);
void init_lfsrs(void);
int get_random(void);

int main(void)
{
  DDRB = 0b00011000; // PB4, PB3 salidas, PB0 entradas
  DDRD = 0b00000011; // PD6, PD3, PD2 entradas, PD1, PD0 salidas

  MCUCR &= 0b00001010; // Flanco negativo de INT0 e INT1
  
  GIMSK |= (1 << PCIE2 ) ; // Habilitar PCIE2 (activada por PD6)
  GIMSK |= (1 << PCIE0 ) ; // Habilitar PCIE0 (activada por PB0)
  GIMSK |= (1 << INT1 ) ; // Habilitar INT1 (activada por PD3)
  GIMSK |= (1 << INT0 ) ; // Habilitar INT0 (activada por PD2)

  PCMSK |= 0b00000001;
  PCMSK2 |= 0b01000000;

  sei();
  init_lfsrs();
  
  while (1) {

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
      init_seq(simon.iter);
      show_seq(simon.iter);
      simon.isr_flag = 0;
      curr_iter = 0;
      ignore = 1;
      state = WAIT_SEQ;
      break;
    
    case WAIT_SEQ:
      if (simon.isr_flag == 1)
      {
        if (simon.led == simon.seq[curr_iter])
        {
          curr_iter++;
        } else
        {
          state = RESET;
          break;
        }
        simon.isr_flag = 0;
        ignore = 1;
      }
      if (curr_iter >= simon.iter) state = PASS;
      break;
    
    case PASS:
      if (simon.iter < 14)
      {
        simon.seq[simon.iter] = get_random();
        simon.iter++;
      }
      show_seq(simon.iter);
      curr_iter = 0;
      ignore = 1;
      state = WAIT_SEQ;
      break;
    
    case RESET:
      blink_all(3);
      simon.isr_flag = 0;
      ignore = 1;
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
    PORTB = 0x00;
    PORTD = 0x00; _delay_ms(5000);
    PORTB = 0b00011000; 
    PORTD = 0b00000011; _delay_ms(10000);
    PORTB = 0x00;
    PORTD = 0x00; _delay_ms(5000);
  }
}

void init_seq(int iter)
{
  for (int i = 0; i < iter; i++)
  {
    simon.seq[i] = get_random();
  }
}

void show_seq(int iter)
{
  for (int i = 0; i < iter; i++)
  {
    switch (simon.seq[i])
    {
    case 0:
      PORTD = 0b1; _delay_ms(10000); // Enciende LED amarillo
      break;
    
    case 1:
      PORTB = 0b10000; _delay_ms(10000); // Enciende LED rojo
      break;
    
    case 2:
      PORTB = 0b1000; _delay_ms(10000);// Enciende LED verde
      break;
    
    case 3:
      PORTD = 0b10; _delay_ms(10000); // Enciende LED azul
      break;
    
    default:
      PORTB = 0b1000;
      PORTD = 0b11; _delay_ms(10000); // Enciende LED
      break;
    }
  PORTD = 0x00; // Apago todo
  PORTB = 0x00; _delay_ms(5000); // Espera
  }
}

int shift_lfsr(word *lfsr, word polynomial_mask)
{
	int feedback;

	feedback = *lfsr & 1;
	*lfsr >>= 1;
	if (feedback == 1)
		*lfsr ^= polynomial_mask;
	return *lfsr;
}

void init_lfsrs(void)
{
	lfsr32 = 0xABCDE;	/* seed values */
	lfsr31 = 0x23456789;
}

int get_random(void)
{
	/* This random number generator shifts the 32-bit LFSR twice before XORing
	it with the 31-bit LFSR. The bottom 2 bits are used for the random number */

	shift_lfsr(&lfsr32, POLY_MASK_32);
	return (shift_lfsr(&lfsr32, POLY_MASK_32) ^ shift_lfsr(&lfsr31, POLY_MASK_31)) & 0b11;
}

ISR( PCINT2_vect ) // Activada por PD6
{ 
  if (!ignore) simon.isr_flag = 1; // Simular flanco negativo
  simon.led = 2;
  ignore = 0;
}

ISR( PCINT0_vect ) // Activada por PB0
{ 
  if (!ignore) simon.isr_flag = 1; // Simular flanco negativo
  simon.led = 1;
  ignore = 0;
}

ISR( INT0_vect ) // Activada por PD0
{
  simon.isr_flag = 1;
  simon.led = 0;
}

ISR( INT1_vect ) // Activada por PD1
{
  simon.isr_flag = 1;
  simon.led = 3;
}
