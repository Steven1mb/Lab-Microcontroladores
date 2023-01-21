typedef unsigned int word;

typedef struct game {
  int seq[14];      // Secuencia de leds
  int iter;         // Iteracion actual
  int isr_flag;     // Reporte de interrupcion
  int led;          // Numero del boton presionado
} game_t;

typedef enum
{
    IDLE,
    BLINK_2,
    INIT,
} eSystemState;
