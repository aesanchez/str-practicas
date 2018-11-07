#include <Arduino.h>
#define INCLUDE_vTaskSuspend 1
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

SemaphoreHandle_t semphr_1_to_3;
SemaphoreHandle_t semphr_3_to_2;
SemaphoreHandle_t semphr_2_to_1;

void task1(void* pvParameters){
    
    while(1){
        xSemaphoreTake(semphr_2_to_1, portMAX_DELAY);
        Serial.println((char*)pvParameters);
        //ejecuta cierta tarea que tarda un tiempo
        delay(111);     
        //vTaskDelay(100/portTICK_PERIOD_MS);
        xSemaphoreGive(semphr_1_to_3);
    }
}
void task2(void* pvParameters){
    while(1){
        xSemaphoreTake(semphr_3_to_2, portMAX_DELAY);
        Serial.println((char*)pvParameters);
        //ejecuta cierta tarea que tarda un tiempo
        delay(222);     
        //vTaskDelay(100/portTICK_PERIOD_MS);
        xSemaphoreGive(semphr_2_to_1);
    }
}
void task3(void* pvParameters){
    while(1){
        xSemaphoreTake(semphr_1_to_3, portMAX_DELAY);
        Serial.println((char*)pvParameters);
        //ejecuta cierta tarea que tarda un tiempo
        delay(333);     
        //vTaskDelay(100/portTICK_PERIOD_MS);
        xSemaphoreGive(semphr_3_to_2);
    }
}

void setup(){   
    Serial.begin(9600);
    Serial.println("\nStart");

    vSemaphoreCreateBinary(semphr_1_to_3);
    vSemaphoreCreateBinary(semphr_3_to_2);
    vSemaphoreCreateBinary(semphr_2_to_1);
    //para que entre primero a la tarea 1
    xSemaphoreTake(semphr_1_to_3, portMAX_DELAY);
    xSemaphoreTake(semphr_3_to_2, portMAX_DELAY);

    xTaskCreate(task1, "task_name_1", 100, (void*)("Task 1"), 1, NULL);
    xTaskCreate(task2, "task_name_2", 100, (void*)("Task 2"), 1, NULL);
    xTaskCreate(task3, "task_name_3", 100, (void*)("Task 3"), 1, NULL);

}

void loop(){}