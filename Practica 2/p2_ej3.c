#include <htc.h>

void config_adc()
{
    /*
    ADC
    -----
    10bits
    20us
    8canales
    Puede usarse la tensión interna VDD (ej: 5V) y la de tierra (0V) o bien una tensión externa

    CONFIG
    ------
    Registro ADCON0
    Bits    Nombre      Descripcion
    7,6     ADCS1:0     Seleccion clock
    5,4,3   CHS2:0      Seleccion canal
    2       GO          GO=1 conversion en proceso
    1       --          ---
    0       ADON        =1-->activado

    Registo ADCON1
    Bits    Nombre      Descrip
    7       ADFM        Como se almacena, 1->10 derecha(los 6 bits mas significativos quedan en 0), 0->10bits izquierda
    6,5,4   --          --
    3,2,1,0 PCFG3:0     Config. 1110--> canal 8 analogico, VCC-GND
    */
   ADCON1 = 0b10001110;
   ADCON0 = 0b01000001;
   ADIE = 0; //sin interrupcion
   TRISA = TRISA | 1; //bit 0 en 1, ya que ahi esta el chanel 0 (AN0)
}

void start_adc(void){
    GO = 1;
}

void wait_adc(void){
    while(GO);
}

void write_rb(char d){
    RB0 = d & 0x01;    
    RB1 = (d >> 1) & 0x01;
    RB2 = (d >> 2) & 0x01;
    RB3 = (d >> 3) & 0x01;
    RB4 = (d >> 4) & 0x01;
    RB5 = (d >> 5) & 0x01;
    RB6 = (d >> 6) & 0x01;
    RB7 = (d >> 7) & 0x01;    
}

void print_display(unsigned short v){
    unsigned short HIGH, LOW;
    HIGH = v >> 8;
    LOW = v & 0xFF;
    write_rb(HIGH);
    RD7 = 1;
    RD7 = 0;
    write_rb(LOW);
    RD6 = 1;
    RD6 = 0;
}

void config_display(void){
    TRISB = 0x00; //todos los puertos son de salida
    TRISD = TRISD & 0b00111111;
    print_display(0x0000);   
}

unsigned short value;
void main(void)
{
    config_display();
    config_adc();
    start_adc();
    while (1){
        wait_adc();
        //formato 2bits|8bits
        value = ADRESH << 8 | ADRESL;
        print_display(value);
        start_adc();
    }
}