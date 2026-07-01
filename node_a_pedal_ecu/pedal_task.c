#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "protocol.h"

void vVehiclePedalTask(void *pvParameters)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("[NODE A - PEDAL] Pure SIL Mode Active: Transmitting...\n");

    uint8_t u8Sequence = 0;
    uint8_t u8PedalRaw = 0;
    uint8_t u8Direction = 1;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10)); 

        u8Sequence = (u8Sequence + 1) % 16;
        
        if (u8Direction) {
            u8PedalRaw += 5;
            if (u8PedalRaw >= 100) u8Direction = 0;
        } else {
            u8PedalRaw -= 5;
            if (u8PedalRaw <= 0) u8Direction = 1;
        }

        uint8_t u8TxBuffer[3];
        u8TxBuffer[0] = (uint8_t)((PEDAL_DATA_ID << 4) | u8Sequence);
        u8TxBuffer[1] = u8PedalRaw;
        u8TxBuffer[2] = (uint8_t)(100 - u8PedalRaw); 

        uint8_t u8Crc = u8CalculateAUTOSARCRC8(u8TxBuffer, 3);

        if (pVirtualBus != NULL) {
            pVirtualBus->node_a_slot = ((uint32_t)u8TxBuffer[0] << 0)  |
                                       ((uint32_t)u8TxBuffer[1] << 8)  |
                                       ((uint32_t)u8TxBuffer[2] << 16) |
                                       ((uint32_t)u8Crc         << 24);
            
            printf("-> [PEDAL] Sent Packet - Seq: %d, APS: %d%%\n", u8Sequence, u8PedalRaw);
            fflush(stdout);
        }
    }
}