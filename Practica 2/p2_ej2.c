#include <htc.h>

//se necesita para los delay
#define _XTAL_FREQ 1000000

//cuando TMR0 llega a 256, se levanta la interrupcion
//y pone T0IF=1 para informar la interrupcion
void interrupt timer_handler(void)
{
    //swap
    RB4 = !RB4;
    RB5 = !RB5;
    //reset del contador
    TMR0 = 256 - 244;
    //clean del flag
    T0IF = 0; //flag de atencion de interrupt
}

void config_board(void)
{
    // 1 = entrada
    // 0 = salida
    TRISB = 0b11001111;
    TRISA = 0b00000011;
    //prender ambos leds
    RB4 = 1;
    RB5 = 1;
}

void wait_for_key_pressed(void)
{
    while (RA0 != 0 && RA1 != 0);
}

void config_timer(void)
{
    PSA = 0;                         //selecciona el prescaler para el timer0 (modo temporizador)
    OPTION_REG = OPTION_REG & 0b111; //prescaler2 256
    T0CS = 0;                        //como temporizador
    /* 
        ciclo de instruccion = 4 * ciclo de reloj = 4us
        esto itera cada 1,024 ms (4us * prescaler)
        tiempo_deseado[ms]/1,024=contador
        250/1,024 = ~244
    */
    TMR0 = 256 - 244;
    INTCON = 0b10100000; //habilitar interrupcion general y de timer
}
void main(void)
{
    config_board();
    wait_for_key_pressed();
    //alternado
    RB4 = 0;
    config_timer();

    while (1)
    {
        //loop infinito
    }
}