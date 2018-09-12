#include <htc.h>

//se necesita para los delay
#define _XTAL_FREQ 1000000

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
    while (RA0 != 0 && RA1 != 0)
        ;
}

void main(void)
{
    config_board();
    wait_for_key_pressed();
    //alternado
    RB4 = 0;

    while (1)
    {
        //swap
        RB4 = !RB4;
        RB5 = !RB5;
        __delay_ms(250);
    }
}