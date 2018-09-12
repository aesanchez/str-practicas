#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <graphics.h>

#define MAX_TEMP 120
#define MAX_SEN 4096

#define X_START 50
#define Y_START 430
#define X_END 600
#define Y_END 40
#define ARROW_OFFSET 10

int current_x = X_START;
int last_x = X_START;
int last_y = Y_START;
int previous_state = 0;

void draw_graph(void);
void init_graph(void);
void set_graph(float);

void draw_graph(){
    int i;
    const char *a[6];
    a[0] = "0";
    a[1] = "1";
    a[2] = "2";
    a[3] = "3";
    a[4] = "4";
    a[5] = "5";    
    setcolor(WHITE);
    setlinestyle(SOLID_LINE, 0, 2);

    //axis x and y
    line(X_START, Y_START, X_START, Y_END);
    line(X_START, Y_START, X_END, Y_START);
    line(X_START-ARROW_OFFSET, Y_END+ARROW_OFFSET, X_START, Y_END);
    line(X_START+ARROW_OFFSET, Y_END+ARROW_OFFSET, X_START, Y_END);
    line(X_END-ARROW_OFFSET, Y_START-ARROW_OFFSET, X_END, Y_START);
    line(X_END-ARROW_OFFSET, Y_START+ARROW_OFFSET, X_END, Y_START);

    //labels
    outtextxy(X_START-20, Y_END - 10, "Volts");
    outtextxy(X_END-20, Y_START + 15, "x100");

    //y scale
    for(i=0; i<6 ; i++){
        if(i != 0)
            line(X_START-5, Y_START - ((Y_START-Y_END)/6)*i, X_START+5, Y_START - ((Y_START-Y_END)/6)*i);
        outtextxy(X_START-15, Y_START - 2 - ((Y_START-Y_END)/6)*i, a[i]);
    }
    //x scale
    for(i=1; i<6 ; i++){
        line(X_START + i*100, Y_START + 5, X_START + i*100, Y_START - 5);
        outtextxy((X_START + i*100) - 10, Y_START + 10, a[i]);
    }


    //DRAW 65 and 85 references
    // x*5/120
    setlinestyle(DOTTED_LINE, 0, 2);
    line(X_START, (int)(Y_START - ((Y_START-Y_END)/6)*(65.0*5/120.0)), X_END, (int)(Y_START - ((Y_START-Y_END)/6)*(65.0*5/120.0)));
    line(X_START, (int)(Y_START - ((Y_START-Y_END)/6)*(85.0*5/120.0)), X_END, (int)(Y_START - ((Y_START-Y_END)/6)*(85.0*5/120.0)));

    setcolor(RED);
    setlinestyle(SOLID_LINE, 0, 2);

}

void init_graph(){
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\BORLANDC\\BGI");
    draw_graph();
}

void set_graph(float volts){
    /*
    0 --> Y_START - ((Y_START-Y_END)/6)*0
    5 --> Y_START - ((Y_START-Y_END)/6)*5
    x --> Y_START - ((Y_START-Y_END)/6)*volts
    */
    line(last_x, last_y, current_x, (int)(Y_START - ((Y_START-Y_END)/6)*volts));
    last_x = current_x;
    current_x++;
    last_y = (int)(Y_START - ((Y_START-Y_END)/6)*volts);

    if(current_x == X_END){
        current_x = X_START;
        last_x = X_START;
        cleardevice();
        draw_graph();
    }        
}

int main(void)
{ 
	unsigned int DIRBAS, STINR, CTREG, ADHIG, ADLOW, VD, OUTBR;
	unsigned int grados;
	unsigned char A, B, C;
	float VA;
	DIRBAS = 0x300;
	OUTBR = DIRBAS + 4;
	STINR = DIRBAS + 0;
	CTREG = DIRBAS + 0;
	ADHIG = DIRBAS + 9;
	ADLOW = DIRBAS + 8;
	
	outportb (CTREG, 1);// Canal 1, FE: +5V

    init_graph();

	while(1){
		C = inportb (ADLOW);//Iniciar conversacion

		//Esperar que termine
		do {
			C = inportb (STINR); // Lee STINR
			C = C && 0x20; // Extrae EOC
		}while (C == 0x0);

		//Leer dato convertido
		A = inportb (ADHIG);
		B = inportb (ADLOW);
		VD = A * 256 + B;

		//Convertir a volts
		VA = (float) 5 / 4096 * VD;
        set_graph(VA);
    
        //Convertir a grados 0-120
		grados = VD/(4096/120);
	
		//sistema de ventilacion
		if(previous_state <= 65 && grados > 65)//prender		
			outportb(OUTBR, 0x09);//1001
		if(previous_state >= 65 && grados < 65)//apagar
			outportb(OUTBR, 0x01);
		
		//Desactivacion de motor
		if(previous_state <+ 85 && grados > 85)//prender			
			outportb(OUTBR, 0x08);//1000
		if(previous_state >= 85 && grados < 85)//apagar	
			outportb(OUTBR, 0x00);
		
        previous_state = grados;

		delay(20);
	}	
	return 1;
}
