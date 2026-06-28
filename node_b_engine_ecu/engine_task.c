#include "FreeRTOS.h"
#include "task.h"
#include "stm32f412cx.h"
#include "../common/protocol.h"

// 💡 엔진 제어와 클러스터 송신 태스크가 함께 공유할 글로벌 RPM 변수
static uint32_t gu32EngineRpm = 0;

void vVehicleEngineTask(void *pvParameters){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 여기서도 10 주기?

    uint8_t u8RxBuffer[3];
    uint8_t u8RxCRC=0;

    while(1){
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        uint32_t u32TdlrVal = CAN1->sFIFOMailBox[0].RDLR;

        u8RxBuffer[0] = (uint8_t)((u32TdlrVal >> 0)  & 0xFF); // [🔑ID | 🔢카운터]
        u8RxBuffer[1] = (uint8_t)((u32TdlrVal >> 8)  & 0xFF); // MainAps
        u8RxBuffer[2] = (uint8_t)((u32TdlrVal >> 16) & 0xFF); // SubAps
        u8RxCRC       = (uint8_t)((u32TdlrVal >> 24) & 0xFF); // 날아온 CRC 도장

        uint8_t u8CalcCRC = u8CalculateAUTOSARCRC8(u8RxBuffer, 3);

        if(u8RxCRC == u8CalcCRC){
            uint8_t xu8RxID = (u8RxBuffer[0] >> 4) & 0x0F; // DATA ID 가져오기!

            if(xu8RxID == PEDAL_DATA_ID){
                uint8_t u8MainAps = u8RxBuffer[1]; // 페달값 가져오기
                uint8_t u8SubAps = u8RxBuffer[2];
                /* ======================== */
                /* 이중화 센서 검증 및 RPM 연산 */
                /* ======================== */
                /* Coding Here */

                // 1. u8MainAps와 u8SubAps의 차이값(절댓값)을 구한다.
                uint8_t u8ApsDiff = 0; // 절대값 뭐 어떻게 하지? abs() 쓰나? 실무에서 어떻게 하지?
                if(u8MainAps >= u8SubAps){
                    u8ApsDiff = u8MainAps - u8SubAps;
                }
                else{
                    u8ApsDiff = u8SubAps - u8MainAps;
                }
                
                // 2. 만약 두 값의 차이가 오차범위 5보다 크면 급발진 고장으로 판단
                if(u8ApsDiff > 5){
                    gu32EngineRpm = 0;
                }
                // 3. 고장이 아니라 정상 범위라면, 주 센서 값인 u8MainAps에 60을 곱하여 gu32EngineRpm을 최신화한다. (페달 100% 일 때 6000 RPM)
                else{
                    gu32EngineRpm = u8MainAps * 60;
                }   
            }
            else{
                //error
            }
        }

        
    }
}

void vVehicleClusterTask(void *pvParameters){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(20);

    while(1){
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        /* ========================================================== */
        /* 🛠️ [원칙 4] 차기 진격 영역: 계기판으로 RPM 패킷 송신         */
        /* ========================================================== */
        /* Coding Here */
        // 1. 송신용 로컬 버퍼 배열(예: uint8_t u8TxBuffer[4])을 만든다.
        // 2. 버퍼[0]에는 계기판 데이터 ID인 '0x7'과 시퀀스 카운터를 패킹한다.
        // 3. 글로벌 변수 gu32EngineRpm 값을 1바이트씩 쪼개서 버퍼[1], 버퍼[2]에 장전한다.
        // 4. 패킹된 데이터의 AUTOSAR CRC-8을 계산해서 버퍼[3]에 도장 찍는다.
        // 5. CAN1->sTxMailBox[0] 레지스터 서랍을 열고 냅다 사격(송신)한다!
        /* ========================================================== */
        
        // (1)
        uint8_t u8TxBuffer[4]; 
        static uint8_t u8SeqCounter = 0;

        u8SeqCounter = (u8SeqCounter+1)%16;

        u8TxBuffer[0] = (CLUSTER_DATA_ID<<4) | u8SeqCounter;
        u8TxBuffer[1] = (uint8_t)((gu32EngineRpm >>8 ) & 0xFF);
        u8TxBuffer[2] = (uint8_t)((gu32EngineRpm & 0xFF));
        u8TxBuffer[3] = u8CalculateAUTOSARCRC8(u8TxBuffer, 3);

        CAN1->sTxMailBox[0].TDLR =  (u8TxBuffer[0]<< 0) |
                                    (u8TxBuffer[1]<< 8) |
                                    (u8TxBuffer[2]<< 16) |
                                    (u8TxBuffer[3] << 24);
        CAN1->sTxMailBox[0].TDHR = 0x00000000;
        CAN1->sTxMailBox[0].TDTR = (4<<0);
        CAN1->sTxMailBox[0].TIR = (0x200 << 21);
        CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;

    }
}