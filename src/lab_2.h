typedef struct game {
  int seq[14];      // Secuencia de leds
  int iter;         // Iteracion actual
  int isr_flag;     // Reporte de interrupcion
  int led;          // Numero del boton presionado
} game_t;

typedef enum
{
    IDLE,
    Card_Inserted_State,
    Pin_Eentered_State,
    Option_Selected_State,
    Amount_Entered_State,
} eSystemState;
