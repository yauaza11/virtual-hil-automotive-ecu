#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "protocol.h"

void vVehicleEngineTask(void *pvParameters)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("[NODE B - ENGINE] Pure SIL Mode Active: Bridging...\n");

    uint8_t u8EngineSeq = 0;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));

        if (pVirtualBus != NULL) {
            uint32_t u32RxData = pVirtualBus->node_a_slot; 

            uint8_t u8RxBuffer[3];
            u8RxBuffer[0] = (uint8_t)((u32RxData >> 0)  & 0xFF);
            u8RxBuffer[1] = (uint8_t)((u32RxData >> 8)  & 0xFF);
            u8RxBuffer[2] = (uint8_t)((u32RxData >> 16) & 0xFF);
            uint8_t u8RxCRC = (uint8_t)((u32RxData >> 24) & 0xFF);

            if (u8RxCRC == u8CalculateAUTOSARCRC8(u8RxBuffer, 3)) {
                uint8_t u8PedalVal = u8RxBuffer[1];
                uint32_t u32Rpm = u8PedalVal * 60; 

                u8EngineSeq = (u8EngineSeq + 1) % 16;

                uint8_t u8TxBuffer[3];
                u8TxBuffer[0] = (uint8_t)((CLUSTER_DATA_ID << 4) | u8EngineSeq);
                u8TxBuffer[1] = (uint8_t)((u32Rpm >> 8) & 0xFF);
                u8TxBuffer[2] = (uint8_t)(u32Rpm & 0xFF);

                uint8_t u8TxCRC = u8CalculateAUTOSARCRC8(u8TxBuffer, 3);

                pVirtualBus->node_b_slot = ((uint32_t)u8TxBuffer[0] << 0)  |
                                           ((uint32_t)u8TxBuffer[1] << 8)  |
                                           ((uint32_t)u8TxBuffer[2] << 16) |
                                           ((uint32_t)u8TxCRC       << 24);

                printf("⚙️ [ENGINE] Scaling Active -> Linked RPM: %d\n", u32Rpm);
                fflush(stdout);
            }
        }
    }
}