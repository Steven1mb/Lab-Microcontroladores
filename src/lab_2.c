#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lab_2.h"

#define POLY_MASK_32 0XB4BCD35C
#define POLY_MASK_31 0X7A5BC2E3

word lfsr32, lfsr31, curr_iter, ignore = 1;

game_t simon;

eSystemState state = IDLE;

void blink_all(int times); // Funcion para hacer los LED parpadear
void init_seq(int iter);   // Funcion donde se crea la secuencia
void show_seq(int iter);   // Funcion para mostrar la secuencia de luces 
int shift_lfsr(word *lfsr, word polynomial_mask); // Funcion donde se hace el shift lfsr
void init_lfsrs(void); // Funcion donde se establecen los seed values
int get_random(void); // Funcion que calcula los numeros random

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
    case IDLE: // Estado inicial IDLE, se espera a que el usuario toque un boton
      simon.iter = 4;
      if (simon.isr_flag == 1) // Para esto, se espera a que se levante el flag por el ISR
      {
        state = BLINK_2; // CUando inicia, se parpadea dos veces
      }
      break;
    
    case BLINK_2:
      blink_all(2);
      state = INIT; // luego al estado inicial
      break;

    case INIT:
      init_seq(simon.iter); // Se crea y muestra la secuencia
      show_seq(simon.iter);
      simon.isr_flag = 0;
      curr_iter = 0;
      ignore = 1;
      state = WAIT_SEQ;
      break;
    
    case WAIT_SEQ: // Se espera a que el usuario toque los botones
      if (simon.isr_flag == 1)
      {
        if (simon.led == simon.seq[curr_iter])
        {
          curr_iter++;
        } else
        {
          state = RESET; // Si el toque fue incorrecto, se va a estado RESET
          break;
        }
        simon.isr_flag = 0;
        ignore = 1;
      }
      if (curr_iter >= simon.iter) state = PASS;
      break;
    
    case PASS: // Si la secuencia ingresada es correcta, se sube de nivel
      if (simon.iter < 14)
      {
        simon.seq[simon.iter] = get_random();
        simon.iter++;
      }
      show_seq(simon.iter); // Se ,uestra la nueva secuencia
      curr_iter = 0;
      simon.isr_flag = 0;
      ignore = 1;
      state = WAIT_SEQ; // Y vuelve a estado de espera al input del usuario
      break;
    
    case RESET: // Si el input fue invalido, se parpadea 3 veces, s reinicia el juego
      blink_all(3); // Y el estado vuelve a IDLE
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

void blink_all(int times) // Funcion para parpadear todos los LEDS
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
      PORTD = 0b11; _delay_ms(10000); 
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
