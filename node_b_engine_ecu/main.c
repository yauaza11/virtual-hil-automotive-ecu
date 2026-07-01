#include "FreeRTOS.h"
#include "task.h"
#include "protocol.h"

void vHardwareInit(void);
void vVehicleEngineTask(void *pvParameters);

int main()
{
    vInitVirtualCANBus(); // 🔌 공용 버스 동기화 링크 오픈
    vHardwareInit();

    xTaskCreate(vVehicleEngineTask, "EngineTask", 256, NULL, 3, NULL);
    vTaskStartScheduler();

    while(1) {}
    return 0;
}
void vHardwareInit(void) {}