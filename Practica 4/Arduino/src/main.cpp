#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

void task(void* pvParameters){
	while(1){
		Serial.println((char*)pvParameters);
		/*
		Pasa a estado "suspendido" para dejarle tiempo al resto de las tareas,
		hasta que se cumple ese delay.
		Esta especificado en ticks, por eso se usa esa constante para hacer
		la conversion necesaria de ms a ticks.
		*/
		vTaskDelay(100/portTICK_PERIOD_MS);
	}
}

static void task_idle(void* pvParameters){
	while(1){
		Serial.println((char*)pvParameters);
		vTaskDelay(50/portTICK_PERIOD_MS);
	}
}

void setup(){	
	Serial.begin(9600);
	/*
	xTaskCreate(
		puntero a funcion con codigo de la tarea,
		nombre de la tarea, 
		profundidad del stack,
		argumentos de la funcion de la tarea,
		prioridad,
		no se);
	*/
	Serial.println("\nStart");		
	xTaskCreate(task, "task_name_1", 100, (void*)("Task 1"), 3, NULL);
	xTaskCreate(task, "task_name_2", 100, (void*)("Task 2"), 1, NULL);
	xTaskCreate(task, "task_name_3", 100, (void*)("Task 3"), 1, NULL);
	//xTaskCreate(task_idle, "task_name_idle", 100, (void*)("Task IDLE"), 0, NULL);
}

void loop(){}