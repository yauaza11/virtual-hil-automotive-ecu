#include "FreeRTOS.h"
#include "task.h"
#include "stm32f412cx.h"
#include "../common/protocol.h"

void vVehiclePedalTask(void *pvParameters){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(10);

    uint8_t u8DataBuffer[3];

    while(1){

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        
        //여기서 왜 static 쓰는거지? 아 ! 알았다. RTOS니까 다시 돌아와도 고정이 되야하니까? 그냥 while전에 선언하면 또 별로인가?
        static uint8_t u8SeqCounter = 0;
        static uint8_t u8VirtualPedal = 0;
        
        // 페달 데이터 1-100까지 적용늘리기?
        u8VirtualPedal = (u8VirtualPedal+1) % 101;
        u8SeqCounter = (u8SeqCounter+1) % 16;

        // 음.. 굳이 이렇게 해야하나? 해야하네..
        u8DataBuffer[0] = (PEDAL_DATA_ID<<4) | u8SeqCounter;
        u8DataBuffer[1] = u8VirtualPedal; // MainAps
        u8DataBuffer[2] = u8VirtualPedal; // SubAps

        uint8_t CRC8Result = u8CalculateAUTOSARCRC8(u8DataBuffer, 3);

        CAN1->sTxMailBox[0].TDLR =  (u8DataBuffer[0]<< 0) |
                                    (u8DataBuffer[1]<< 8) |
                                    (u8DataBuffer[2]<< 16) |
                                    (CRC8Result << 24);
        CAN1->sTxMailBox[0].TDHR = 0x00000000;
        CAN1->sTxMailBox[0].TDTR = (4<<0);
        CAN1->sTxMailBox[0].TIR = (0x100 << 21);
        CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
    }
}