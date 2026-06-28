#include "FreeRTOS.h"
#include "task.h"
#include "stm32f412cx.h"

void HardwareInit(void);

extern void vVehicleEngineTask(void *pvParameters);
extern void vVehicleClusterTask(void *pvParameters);

int main()
{
    vHardwareInit();

    xTaskCreate(&vVehicleEngineTask, "EngineTask", 256, NULL, 3 ,NULL);
    xTaskCreate(&vVehicleClusterTask, "ClusterTxTask", 256, NULL, 3 ,NULL);

    vTaskStartScheduler();

    while(1){
        //safe state
    }
    return 0;
}

void vHarwareInit(void){
    // empty
}