#include <stdio.h>
#include <dos.h>
#include <conio.h>

//Clock tick interrupt
#define INTR 0x1C
//Clock frequency
#define CLK 1193800L
//Ports address
#define PORT_CONFIG 0x43
#define PORT_IO 0x40

int intr_iteration = 0;
int iteration = 0;
unsigned short freq = 0;
unsigned short latchValue;
char lowByte, highByte;
char keepGoing;
int seconds = 0;

void interrupt ( *oldhandler)();

void interrupt handler()
{
	intr_iteration++;
}

void timer_config(unsigned short f)
{
	//Save
	oldhandler = getvect(INTR);
	//Calculate latch value
	latchValue = CLK / f;
	/*
	Timer config
	Bit	Config
	0	binary format=0 / BCD=1
	1-3	operation mode, square wave=3
	4-5	access mode, 11=first LOW byte then HIGH byte
	6-7	counter number
	----------------
	0b00110110
	*/
	outportb(PORT_CONFIG, 0x36);
	lowByte = (char)(latchValue & 0xFF);
	highByte = (char)(latchValue >> 8);
	outportb(PORT_IO, lowByte);
	outportb(PORT_IO, highByte);

	//install the new interrupt handler
	setvect(INTR, handler);
}

void timer_restore(void)
{
	outportb(PORT_CONFIG, 0x36);
	outportb(PORT_IO, 0xFF);
	outportb(PORT_IO, 0xFF);
		
	//Restore
	setvect(INTR, oldhandler);
}

int main(void)
{
	printf("\nIngrese la frecuencia deseada: ");
	scanf("%hu", &freq);

	timer_config(freq);
	
	keepGoing = 1;
	while (keepGoing)
	{
		if (intr_iteration >= freq)
		{
			//One more second
			seconds++;
			//refresh iteration
			iteration += intr_iteration;
			intr_iteration = 0;

			printf("Second = %d (i= %d)\n", seconds, iteration);

			//Check if it has finished
			if(seconds == 20)
			{
				keepGoing = 0;
			}
		}
	}
	
	printf("Press any key to continue...");
	//clean previous enter
	scanf("%c", &lowByte);
	scanf("%c", &lowByte);

	timer_restore();
		
	return 1;
}