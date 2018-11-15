/*
Para realizar el sensor de temperatura, realice un programa que lea de un archivo
el tiempo (nanosegundos) que debe esperar para entregar un dato y la
temperatura (°C) que debe entregar. Dicho archivo debe tener por cada fila dos
números (el tiempo y la temperatura) separados por un <tab>. Una vez cumplido
el plazo leído del archivo el programa debe escribir en un pipe la temperatura y
esperar el cumplimiento del siguiente plazo para volver a escribir. Debe continuar
con esta mecánica hasta la finalización del archivo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define BUFF_SIZE 64
#define DELIM "\t"

void *thread_function(void *ptr);

pthread_t thread_sensor;
FILE *file;
char buff[BUFF_SIZE];

int main()
{
	file = fopen("sensor_file", "r");

	pthread_create(&thread_sensor, NULL, thread_function, NULL);

	pthread_join(thread_sensor, NULL);

	fclose(file);
	exit(EXIT_SUCCESS);
}

void *thread_function(void *ptr)
{
	char *aux;
	long nanoseconds;
	float temp;
	printf("Sensor started\n");
	while (!feof(file))
	{
		fgets(buff, BUFF_SIZE, (FILE *)file);

		aux = strtok(buff, DELIM);
		nanoseconds = strtol(aux, NULL, 10);

		aux = strtok(NULL, DELIM);
		temp = strtof(aux, NULL);

		printf("%ld", nanoseconds);
		printf("\t");
		printf("%.2f\n", temp);

		usleep(nanoseconds/1000);
		// no me funcionaba el nanosleep()
	}
}
