#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "protocol.h"

void vVehicleClusterRxTask(void *pvParameters)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("[NODE C - CLUSTER] Pure SIL Mode Active: Receiving...\n");

    uint8_t u8RxBuffer[3];

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));

        if (pVirtualBus != NULL) {
            uint32_t u32TdlrVal = pVirtualBus->node_b_slot; 

            u8RxBuffer[0] = (uint8_t)((u32TdlrVal >> 0)  & 0xFF);
            u8RxBuffer[1] = (uint8_t)((u32TdlrVal >> 8)  & 0xFF);
            u8RxBuffer[2] = (uint8_t)((u32TdlrVal >> 16) & 0xFF);
            uint8_t u8RxCRC     = (uint8_t)((u32TdlrVal >> 24) & 0xFF);

            if (u8RxCRC == u8CalculateAUTOSARCRC8(u8RxBuffer, 3)) {
                uint8_t u8RxID = (u8RxBuffer[0] >> 4) & 0x0F;

                if (u8RxID == CLUSTER_DATA_ID) {
                    uint8_t u8RpmHigh = u8RxBuffer[1];
                    uint8_t u8RpmLow  = u8RxBuffer[2];

                    uint32_t u32Rpm = (uint32_t)(u8RpmHigh << 8) | u8RpmLow;
                    
                    printf("🚗 [CLUSTER DISPLAY] Real-time Engine Speed: %d RPM\n", u32Rpm);
                    fflush(stdout);
                }
            }
        }
    }
}