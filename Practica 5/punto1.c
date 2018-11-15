/*
Escriba un programa que contenga dos threads que se ejecutan periódicamente
en 1000 oportunidades. Ambos threads se ejecutan 100 veces por segundo y cada
vez que se ejecutan acumulan el tiempo de latencia que pasó desde que se
cumplió una centésima de segundo, hasta que verdaderamente retomó la
ejecución la tarea. En otras palabras, el tiempo que tardó en retomar el thread la
tarea menos la centésima de segundo que debía transcurrir. Una vez que ambos
threads hayan finalizado, imprima el promedio de la latencia acumulada para cada
uno de los dos threads. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define N 1000

void *thread_function(void *ptr);

int main()
{
    pthread_t thread1, thread2;
    const char *name1 = "Thread 1";
    const char *name2 = "Thread 2";

    pthread_create(&thread1, NULL, thread_function, (void *)name1);
    pthread_create(&thread2, NULL, thread_function, (void *)name2);

    printf("Threads created\n");

    // Wait for the threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    exit(EXIT_SUCCESS);
}

void *thread_function(void *ptr)
{
    char *name;
    name = (char *)ptr;

    long delta_us, latencia_iteracion, latencia_total;
    latencia_total = 0;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (int i = 0; i < N; i++)
    {
        //calcular tiempo de latencia
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
        if(i)
            latencia_iteracion = delta_us - 10*1000;
        else
            latencia_iteracion = 0;
        latencia_total += latencia_iteracion;
        printf("%d\t%s\t%ldus \n", i, name, latencia_iteracion);
        //marca temporal justo antes de llamar al sleep
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        usleep(10 * 1000);
    }

    printf("%s Latencia total %ldus. Latencia promedio %fus\n", name, latencia_total, (double)latencia_total / N-1);
}